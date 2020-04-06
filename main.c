/*
 * Copyright 2019 GreenWaves Technologies, SAS
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include "Gap.h"
#include "dnn_utils.h"
#include "MN_Kernels.h"
#include "rt/rt_api.h"


// input binary file
#define NUM_FILES (1) 
char *layer_b_names[] =
{
    "L0_INPUT.bin"
};
int file_byte_size[NUM_FILES];
RT_L2_DATA char  *net_in     [NUM_FILES];

signed char *__restrict ptr_input_L3;

// declared by the autotiler
extern L1_CL_MEM char *L1_Memory;
extern L2_MEM char *L2_Memory;

// Filesystem and HyperRam structures
struct pi_device HyperRam;
RT_L2_DATA rt_fs_t    *FileSystem;

//cluster parallel task
void Process()
{
  gap_cl_starttimer();  gap_cl_resethwtimer();

  MobileNetCNN(ptr_input_L3);
}

//pmsis main task
int mobilenet()
{

  struct pi_device cluster_dev;
  struct pi_cluster_conf cl_conf;
  cl_conf.id = 0;
  pi_open_from_conf(&cluster_dev, (void *) &cl_conf);
  if (pi_cluster_open(&cluster_dev))
  {
      printf("Cluster open failed !\n");
      pmsis_exit(-7);
  }

  printf("Configuring Hyperram..\n");
  struct pi_hyperram_conf hyper_conf;

  pi_hyperram_conf_init(&hyper_conf);
  pi_open_from_conf(&HyperRam, &hyper_conf);

  if (pi_ram_open(&HyperRam))
  {
      printf("Error: cannot open Hyperram!\n");
      pmsis_exit(-2);
  }

  printf("HyperRAM config done\n");

  // The hyper chip need to wait a bit.
  // TODO: find out need to wait how many times.
  pi_time_wait_us(1*1000*1000);

  printf("Configuring Hyperflash and FS..\n");
  struct pi_device fs;
  struct pi_device flash;
  struct pi_fs_conf fsconf;
  struct pi_hyperflash_conf flash_conf;
  pi_fs_conf_init(&fsconf);

  pi_hyperflash_conf_init(&flash_conf);
  pi_open_from_conf(&flash, &flash_conf);

  if (pi_flash_open(&flash))
  {
      printf("Error: Flash open failed\n");
      pmsis_exit(-3);
  }
  fsconf.flash = &flash;

  pi_open_from_conf(&fs, &fsconf);

  int error = pi_fs_mount(&fs);
  if (error)
  {
      printf("Error: FS mount failed with error %d\n", error);
      pmsis_exit(-3);
  }

  printf("FS mounted\n");

  // voltage-frequency settings
  rt_freq_set(RT_FREQ_DOMAIN_FC, 50000000);
  rt_freq_set(RT_FREQ_DOMAIN_CL,150000000);
  PMU_set_voltage(1200,0);


  // reading input data
  for (unsigned int i = 0; i < NUM_FILES; i++)
  {
      net_in[i] = loadLayerFromFsToL3(&fs, layer_b_names[i], 
        &HyperRam, &file_byte_size[i]);
      printf("Weight Size = %d at address %x\n", file_byte_size[i], net_in[i]);
  }
  ptr_input_L3    = net_in[ 0];

  // network construct
  if (MobileNetCNN_Construct()) printf("Construct failed\n");
  else printf("Construct OK\n");

  struct pi_cluster_task *task = pmsis_l2_malloc(sizeof(struct pi_cluster_task));
  if(task==NULL) {
      printf("pi_cluster_task alloc Error!\n");
      pmsis_exit(-1);
  }
  memset(task, 0, sizeof(struct pi_cluster_task));
  task->entry = Process;
  task->arg = (void *) NULL;
  task->stack_size = (uint32_t) 4*1024;
  task->slave_stack_size = (uint32_t) 1024;

//  while(1)
//  {
      pi_cluster_send_task_to_cl(&cluster_dev, task);

      int checksum = 0;
      int max = -40000;
      int class = -1;
      short int *ptr16 = L2_Memory+0;
      for(int i=0;i<1000;i++){
        checksum += ptr16[i];
        if (ptr16[i]>max){
          max = ptr16[i];
          class = i;
        }
      }
      printf("The final class is %d\n", class);
      if (class == 144) printf("Correct Checksum!\n");
      else printf("Checksum wrong! Current value is %d\n", class);

      {
        unsigned int TotalCycles = 0, TotalOper = 0;
        printf("\n");
        for (int i=0; i<(sizeof(MNPerf)/sizeof(unsigned int)); i++) {
          printf("%35s: %10d, Operation: %10d, Operation/Cycle: %f\n", MNLName[i], MNPerf[i], MNOperCount[i], ((float) MNOperCount[i])/ MNPerf[i]);
          TotalCycles += MNPerf[i]; TotalOper += MNOperCount[i];
        }
        printf("\n");
        printf("%35s: %10d, Operation: %10d, Operation/Cycle: %f\n", "Total", TotalCycles, TotalOper, ((float) TotalOper)/ TotalCycles);
        printf("\n");
      }
//  }

  // network destruct
  MobileNetCNN_Destruct();
  
  pi_cluster_close(&cluster_dev);

  // Exit Test
  if (class == 144){
    printf("Test success\n");
    pmsis_exit(0);
  }
  else{
    printf("Test not success\n");
    pmsis_exit(-1) ;
  }

}


int main(void)
{
    printf("\n\n\t *** MobileNet ***\n\n");
    return pmsis_kickoff((void *) mobilenet);
}

