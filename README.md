# MobilenetV1 Imagenet Classification on GAP8 

This reference design aims at demonstrating the end-to-end deployment of a state-of-the-art 
deep netowork, such as a quantized INT8 MobilenetV1, on the GAP8 platfom. 
Throuought this project we show:
* How to generate C code for the infernce task with quantized network network
* How to run a NN inference task on GAP8 chip
* How to train and dump a INT8 quantized network from Pytorch

## Getting Started

### Requirements
To run this example code, the SDK tag > 3.1 must be installed from [here](https://github.com/GreenWaves-Technologies/gap_sdk),
optionally including the [Autotiler] (https://github.com/GreenWaves-Technologies/gap_sdk#getting-the-autotiler) tool.
For training or dumping the network model, Pytorch==1.1 with GPU support must be installed.

### Running the inference task
You can run the compile the code and run the inference task on a GAPUINO board ith the command:
~~~~~shell
make clean all run
~~~~~
To avoid reloading data on the FLASH memory at every run:
~~~~~shell
make clean all run ALREADY_FLASHED=1
~~~~~
Alternatively, the code can run on the [virtual platform]
(https://github.com/GreenWaves-Technologies/gap_sdk#using-the-virtual-platform):
~~~~~shell
make run platform=gvsoc
~~~~~
If the Autotiler tool is installed, the MobileNetV1 C code can be regenereted by:
~~~~~shell
make clean model
~~~~~
### Dumping MobilenetV1 graph description from Pytorch
~~~~~shell
python3 dump_mobilenet_to_GAP8.py --ckp_file './fake_quant_models/imagenet/mobilenetv1_224_1.0/checkpoint.pth.tar' --img_file './image144.jpeg'
~~~~~

### Quantization-aware retraining
To replicate the quantization-aware retraining flow, you can type:
~~~~~shell
cd training/
python3 main.py -a mobilenet --mobilenet_width 1.0 --mobilenet_input 224 --save Imagenet/mobilenet_224_1.0 --dataset imagenet --type_quant 'PerLayerSymPACT' --weight_bits 8 --activ_bits 7 --activ_type learned --gpus 0,1,2,3 -j 32 --epochs 12 -b 128 --save_check --quantizer --batch_fold_delay 1 --batch_fold_type folding_weights
~~~~~
More details can be found [here].

## Performance
The table below details the performance when running a MobilenetV1 model scoring 70% on Imagenet on a GAPUINO board.


| #Layer  | Cycles    | Operation | Operation/Cycle |
|---------|-----------|-----------|-----------------|
| Layer0  | 6087978   | 11239424  | 1.846167        |
| Layer1  | 4960060   | 4014080   | 0.809281        |
| Layer2  | 8925415   | 25690112  | 2.878310        |
| Layer3  | 4469552   | 2007040   | 0.449047        |
| Layer4  | 5960158   | 25690112  | 4.310307        |
| Layer5  | 4800098   | 4014080   | 0.836250        |
| Layer6  | 19110305  | 51380224  | 2.688614        |
| Layer7  | 2230192   | 1003520   | 0.449970        |
| Layer8  | 6694392   | 25690112  | 3.837557        |
| Layer9  | 1888219   | 2007040   | 1.062928        |
| Layer10 | 9490620   | 51380224  | 5.413790        |
| Layer11 | 1676586   | 501760    | 0.299275        |
| Layer12 | 4433797   | 25690112  | 5.794156        |
| Layer13 | 272611    | 1003520   | 3.681143        |
| Layer14 | 9557498   | 51380224  | 5.375907        |
| Layer15 | 296501    | 1003520   | 3.384542        |
| Layer16 | 9563842   | 51380224  | 5.372341        |
| Layer17 | 271815    | 1003520   | 3.691923        |
| Layer18 | 9540258   | 51380224  | 5.385622        |
| Layer19 | 275515    | 1003520   | 3.642343        |
| Layer20 | 9566414   | 51380224  | 5.370897        |
| Layer21 | 288077    | 1003520   | 3.483513        |
| Layer22 | 9542325   | 51380224  | 5.384456        |
| Layer23 | 116249    | 250880    | 2.158126        |
| Layer24 | 4871496   | 25690112  | 5.273557        |
| Layer25 | 188627    | 501760    | 2.660064        |
| Layer26 | 14247062  | 51380224  | 3.606373        |
| Layer27 | 62292     | 50176     | 0.805497        |
| Layer28 | 4660682   | 1024000   | 0.219710        |
|         |           |           |                 |
| Total   | 154048636 | 571123712 | 3.707425        |



