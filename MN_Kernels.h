#ifndef __MN_KERNEL_H__
#define __MN_KERNEL_H__

#include "AutoTilerLibTypes.h"
#include "CNN_BasicKernels.h"
#define _L1_Memory_SIZE 49008
#define _L2_Memory_SIZE 48800
extern char *L1_Memory; /* Size given for generation: 50000 bytes, used: 49008 bytes */
extern char *L2_Memory; /* Size used for generation: 48800 bytes */
extern void Layer0(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer1(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer2(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer3(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer4(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer5(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer7(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer8(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer9(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer10(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer11(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer12(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer13(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer14(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer15(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer16(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer17(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer18(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer19(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer20(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer21(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer22(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer23(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer24(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer25(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer26(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias);
extern void Layer27(
		signed char * __restrict__ In,
		signed char * __restrict__ Out);
extern void Layer28(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern int MobileNetCNN_Construct();
extern int MobileNetCNN_Destruct();
extern short int *__restrict Out;
extern int MobileNetCNN(
		signed char *__restrict In);
extern unsigned int MNPerf[29];
extern char * MNLName[29];
extern unsigned int MNOperCount[29];
#endif
