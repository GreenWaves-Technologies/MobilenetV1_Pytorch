#include "MN_Kernels.h"
L1_CL_MEM AT_L1_POINTER L1_Memory;
L2_MEM AT_L2_POINTER L2_Memory;
AT_HYPERRAM_POINTER L3_Memory;
extern AT_HYPERRAM_T HyperRam;
static AT_HYPERFLASH_FS_T HyperFlash;
void  __attribute__ ((noinline)) Layer0(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 27328 bytes, L2 buffer: 12960 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan2;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerSetBias_fpd_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_DP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerDP_fps_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Total=0, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: 1][Tile0 Dim: 112][D0 Dim: 1]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 112 logical tiles, 112 physical tiles
			Total Size: 401408 [D1, 1 x 401408][Tile0, 112:[112x1, 110:112x1, 112x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 401408][Tile0, 112:[112x1, 110:112x1, 112x1], 1]
		Tile0: [0, 3584, 112], Tile1: [112, 3584, 112], Tile2; [224, 3584, 112]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 1], T2: [D1: 0][Tile0: 2]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, 1 x 32]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 32]
		Tile0: [0, 32, 32], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0], T1: [D1: 0], T2: [D1: 0]
	Ker Arg: MulBias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, 1 x 32]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 32]
		Tile0: [0, 32, 32], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0], T1: [D1: 0], T2: [D1: 0]
	Ker Arg: Filter, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 864 [D1, 1 x 864][D0, 1 x 864]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 864][D0, 1 x 864]
		Tile0: [0, 864, 864], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 0], T2: [D1: 0][D0: 0]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 112 logical tiles, 112 physical tiles
			Total Size: 150528 [D0, 1 x 150528][Tile0, 112:[224x3, 110:224x3, 224x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 112:[224x3, 110:224x3, 224x2], 1][D0, 1 x 150528]
		Tile0: [0, 2016, 672], Tile1: [448, 2016, 672], Tile2; [896, 2016, 672]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 112 logical tiles, 1 physical tiles
			Total Size: 1605632 [D1, 1 x 1605632][Tile0, 112:[112x1, 110:112x1, 112x1], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 1605632][Tile0, 112:[112x1, 110:112x1, 112x1], 4]
		Tile0: [0, 1605632, 1605632], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (L1_Memory+12992);
	KerArg0->W = (unsigned short int) (112);
	KerArg0->H = (unsigned short int) (1);
	KerArg0->OutFeatures = (unsigned short int) (32);
	KerArg0->Bias = (signed char * __restrict__) (L1_Memory+4032);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->W = (unsigned short int) (224);
	KerArg1->UsedW = (unsigned short int) (224);
	KerArg1->InFeatures = (unsigned short int) (3);
	KerArg1->OutFeatures = (unsigned short int) (32);
	KerArg1->Out = (DP_fps_T * __restrict__) (L1_Memory+12992);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->TotalInFeatures = (short int) (3);
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (DP_fps_T * __restrict__) (L1_Memory+12992);
	KerArg2->W = (unsigned short int) (112);
	KerArg2->H = (unsigned short int) (1);
	KerArg2->Norm = (unsigned char) (Norm);
	KerArg2->NormBias = (unsigned char) (NormMulBias);
	KerArg2->InFeatures = (unsigned short int) (32);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (127);
	KerArg2->MulBias = (signed char * __restrict__) (L1_Memory+4064);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3584; _LC_Out=112;
	_SPP_Out=0; _SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+4032), 32, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+4064), 32, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read MulBias */
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+4096+0), 864, 0, &DmaR_Evt3);
	_NN_In=448; _SN_In=2016;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+7168+0), 2016, 50176, 672, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+448), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+7168+2016), 2016, 50176, 672, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+7168+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 2016, 0, &DmaR_Evt4);
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1;
		/*================================= Prepare Tiles ===================================*/
		_SN_Filter = 0;
		
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+4096+864*((D1Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt3);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<112; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==111), T0Ind_NextLast = ((T0Ind+1)==111), T0Ind_NextNextLast = ((T0Ind+2)==111);
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerParSetBias_DP_fps, (void *) KerArg0);
			__CALL(KerParSetBias_DP_fps, KerArg0);
			{ /* Single iteration on D0 */
				int D0Ind_Last = 1, D0Ind_NextLast = 1, D0Ind_NextNextLast = 1;
				/*================================= Prepare Tiles ===================================*/
				_SNN_In = 0;
				if (!(T0Ind_Last)) {
					if (!(T0Ind_NextLast)) {
						_NN_In = _NN_In + (448); _LNN_In = ((T0Ind_NextNextLast)?448:672); _SNN_In = (3*_LNN_In); 
					} else if (!(1)) {
						_NN_In = _NN_In + (-49728); _LNN_In = (672); _SNN_In = (3*_LNN_In); 
					}
				} else if (!((1))) {
					_NN_In = _NN_In + (150528); _LNN_In = (672); _SNN_In = (3*_LNN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
				if (_SNN_In) {
					AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+7168+2016*((D0Ind_Total)%2)),
							_SNN_In, 50176, _LNN_In, 0, &Uchan1);
				}
				AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+7168+2016*((D0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+2016*((D0Ind_Total+1)%2)),
							_SN_In, 0, &DmaR_Evt4);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (L1_Memory+0+2016*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (3-0*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->UsedH = (unsigned short int) (3-0*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->Filter = (signed char * __restrict__) (L1_Memory+4096+864*((D1Ind_Total)%2));
				KerArg1->Pad = (v4s) ((v4s){0,1,0*(T0Ind==0),1*(T0Ind_Last)});
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride2_DP_fps, (void *) KerArg1);
				__CALL(KerParConv3x3Stride2_DP_fps, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				
				_SN_In = _SNN_In;_LN_In = _LNN_In;
				/*============================= End Update Arg Pipeline =============================*/
				D0Ind_Total++;
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (signed char * __restrict__) (L1_Memory+5824+3584*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg2);
			__CALL(KerDPMulBiasScalar_fps, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+3584*((T0Ind_Total+-1)%2)),
						_SP_Out, 12544, _LP_Out, 1, &Uchan2);
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+3584*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+5824+3584*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (112); _LC_Out = (112); _SC_Out = (32*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		/*============================= End Update Arg Pipeline =============================*/
		D1Ind_Total++;
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+3584*((T0Ind_Total+-1)%2)), _SP_Out, 12544, _LP_Out, 1, &Uchan2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer1(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46768 bytes, L2 buffer: 25248 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	AT_HYPERRAM_CL_EVENT Uchan2;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 4][Tile0 Dim: 19]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 76 logical tiles, 76 physical tiles
			Total Size: 401408 [D0, 4 x 100352][Tile0, 19:[112x7, 17:112x8, 112x5], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 4 x 100352][Tile0, 19:[112x7, 17:112x8, 112x5], 1]
		Tile0: [0, 6272, 784], Tile1: [560, 7168, 896], Tile2; [1232, 7168, 896]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 32 [D0, 4 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 4 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 32 [D0, 4 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 4 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 288 [D0, 4 x 72]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 4 x 72]
		Tile0: [0, 72, 72], Tile1: [72, 72, 72], Tile2; [144, 72, 72]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 76 logical tiles, 76 physical tiles
			Total Size: 401408 [D0, 4 x 100352][Tile0, 19:[112x6, 17:112x6, 112x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 4 x 100352][Tile0, 19:[112x6, 17:112x6, 112x4], 1]
		Tile0: [0, 5376, 672], Tile1: [672, 5376, 672], Tile2; [1344, 5376, 672]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 76 logical tiles, 1 physical tiles
			Total Size: 1605632 [D0, 4 x 401408][Tile0, 19:[112x6, 17:112x6, 112x4], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 4 x 401408][Tile0, 19:[112x6, 17:112x6, 112x4], 4]
		Tile0: [0, 1605632, 1605632], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (112);
	KerArg0->UsedW = (unsigned short int) (112);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+25264);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+25264);
	KerArg1->W = (unsigned short int) (112);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->InFeatures = (unsigned short int) (8);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_NN_In=560; _SN_In=7168;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+10752+0), 6272, 12544, 784, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+560), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+10752+7168), 7168, 12544, 896, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+10752+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 6272, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+14336+0), 8, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+14352+0), 8, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+14368+0), 72, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=5376; _LC_Out=672;
	_SPP_Out=0; _SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<4; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==3), D0Ind_NextLast = ((D0Ind+1)==3), D0Ind_NextNextLast = ((D0Ind+2)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (8); _SN_Bias = (8); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (8); _SN_MulBias = (8); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (72); _SN_Filter = (72); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+14336+8*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+14352+8*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+14368+72*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<19; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==18), T0Ind_NextLast = ((T0Ind+1)==18), T0Ind_NextNextLast = ((T0Ind+2)==18);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (672); _LNN_In = ((T0Ind_NextNextLast)?560:896); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (100352)+(-11984); _LNN_In = (784); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (672-112); _LNN_In = (896); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+10752+7168*((T0Ind_Total)%2)),
						_SNN_In, 12544, _LNN_In, 0, &Uchan1);
			}
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+10752+7168*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+7168*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+7168*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (((T0Ind_Last)?5:8)-1*(T0Ind==0));
			KerArg0->UsedH = (unsigned short int) (((T0Ind_Last)?5:8)-1*(T0Ind==0));
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+14368+72*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+14336+8*((D0Ind_Total)%2));
			KerArg0->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->H = (unsigned short int) (T0Ind_Last?4:6);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+14512+5376*((T0Ind_Total)%2));
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+14352+8*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+5376*((T0Ind_Total+-1)%2)),
						_SP_Out, 12544, _LP_Out, 1, &Uchan2);
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+5376*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+14512+5376*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (672); _LC_Out = ((T0Ind_NextLast)?448:672); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (100352)+(-12096); _LC_Out = (672); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+5376*((T0Ind_Total+-1)%2)), _SP_Out, 12544, _LP_Out, 1, &Uchan2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer2(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 47520 bytes, L2 buffer: 47424 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	AT_HYPERRAM_CL_EVENT Uchan2;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 54][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 54 logical tiles, 54 physical tiles
			Total Size: 401408 [Tile1, 54:[32x236, 52:32x236, 32x36], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 54:[32x236, 52:32x236, 32x36], 1]
		Tile0: [0, 7552, 236], Tile1: [236, 7552, 236], Tile2; [472, 7552, 236]
		T0: [Tile1: 0], T1: [Tile1: 0 +7/12], T2: [Tile1: 0 +14/24]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2048 [Tile0, 1:[32x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[32x64], 1]
		Tile0: [0, 2048, 2048], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 54 logical tiles, 1 physical tiles
			Total Size: 32 [Tile1, 54:[32x1, 52:32x1, 32x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 54:[32x1, 52:32x1, 32x1], 1]
		Tile0: [0, 32, 32], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile1: 0], T1: [Tile1: 0], T2: [Tile1: 0]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x64], 1]
		Tile0: [0, 64, 64], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: MulBias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x64], 1]
		Tile0: [0, 64, 64], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 54 logical tiles, 54 physical tiles
			Total Size: 802816 [Tile1, 54:[64x236, 52:64x236, 64x36], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 54:[64x236, 52:64x236, 64x36], 1]
		Tile0: [0, 15104, 236], Tile1: [236, 15104, 236], Tile2; [472, 15104, 236]
		T0: [Tile1: 0], T1: [Tile1: 0 +3/44], T2: [Tile1: 0 +7/24]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (L1_Memory+45344);
	KerArg0->W_In1 = (unsigned short int) (32);
	KerArg0->Bias = (signed char * __restrict__) (L1_Memory+47392);
	KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+47456);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+0);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (1);
	/*================================= Read Tiles Prolog ===============================*/
	_NN_In2=236; _SN_In2=7552;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+30208+0), 7552, 12544, 236, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+236), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+30208+7552), 7552, 12544, 236, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+30208+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+32+0), 7552, 0, &DmaR_Evt1);
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+45344), 2048, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+47392), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+47456), 64, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
	_C_Out=0; _SC_Out=15104; _LC_Out=236;
	_SPP_Out=0; _SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<54; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==53), T1Ind_NextLast = ((T1Ind+1)==53), T1Ind_NextNextLast = ((T1Ind+2)==53);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (236); _LNN_In2 = ((T1Ind_NextNextLast)?36:236); _SNN_In2 = (32*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+30208+7552*((T1Ind_Total)%2)),
					_SNN_In2, 12544, _LNN_In2, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+30208+7552*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+32+7552*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->H_In1 = (unsigned short int) (64);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+32+7552*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?36:236);
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+15136+15104*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?36:236);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*64);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+15104*((T1Ind_Total+-1)%2)),
					_SP_Out, 12544, _LP_Out, 1, &Uchan2);
		AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+15104*((T1Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+15136+15104*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (236); _LC_Out = ((T1Ind_NextLast)?36:236); _SC_Out = (64*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+15104*((T1Ind_Total+-1)%2)), _SP_Out, 12544, _LP_Out, 1, &Uchan2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer3(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 45872 bytes, L2 buffer: 33312 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 8][Tile0 Dim: 8]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 64 logical tiles, 64 physical tiles
			Total Size: 200704 [D0, 8 x 25088][Tile0, 8:[56x7, 6:56x7, 56x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 25088][Tile0, 8:[56x7, 6:56x7, 56x7], 1]
		Tile0: [0, 3136, 392], Tile1: [392, 3136, 392], Tile2; [784, 3136, 392]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 64 [D0, 8 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 64 [D0, 8 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 576 [D0, 8 x 72]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 72]
		Tile0: [0, 72, 72], Tile1: [72, 72, 72], Tile2; [144, 72, 72]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 64 logical tiles, 64 physical tiles
			Total Size: 802816 [D0, 8 x 100352][Tile0, 8:[112x15, 6:112x15, 112x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 100352][Tile0, 8:[112x15, 6:112x15, 112x14], 1]
		Tile0: [0, 13440, 1680], Tile1: [1568, 13440, 1680], Tile2; [3136, 13440, 1680]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 64 logical tiles, 1 physical tiles
			Total Size: 802816 [D0, 8 x 100352][Tile0, 8:[56x7, 6:56x7, 56x7], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 100352][Tile0, 8:[56x7, 6:56x7, 56x7], 4]
		Tile0: [0, 802816, 802816], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (112);
	KerArg0->UsedW = (unsigned short int) (112);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+33328);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+33328);
	KerArg1->W = (unsigned short int) (56);
	KerArg1->H = (unsigned short int) (7);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->InFeatures = (unsigned short int) (8);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3136; _LC_Out=392;
	_SP_Out=0;
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+26880+0), 8, 0, &DmaR_Evt1);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+26896+0), 8, 0, &DmaR_Evt2);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+26912+0), 72, 0, &DmaR_Evt3);
	_NN_In=1568; _SN_In=13440;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+0), 13440, 12544, 1680, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+1568), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+13440), 13440, 12544, 1680, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 13440, 0, &DmaR_Evt4);
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<8; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==7), D0Ind_NextLast = ((D0Ind+1)==7), D0Ind_NextNextLast = ((D0Ind+2)==7);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (8); _SN_Bias = (8); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (8); _SN_MulBias = (8); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (72); _SN_Filter = (72); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+26880+8*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+26896+8*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+26912+72*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt3);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<8; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==7), T0Ind_NextLast = ((T0Ind+1)==7), T0Ind_NextNextLast = ((T0Ind+2)==7);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (1568); _LNN_In = ((T0Ind_NextNextLast)?1568:1680); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (100352)+(-10976); _LNN_In = (1680); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (1568); _LNN_In = (1680); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+13440*((T0Ind_Total)%2)),
						_SNN_In, 12544, _LNN_In, 0, &Uchan1);
			}
			AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+13440*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+13440*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt4);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+13440*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (15-0*(T0Ind==0)-1*(T0Ind_Last));
			KerArg0->UsedH = (unsigned short int) (15-0*(T0Ind==0)-1*(T0Ind_Last));
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+26912+72*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+26880+8*((D0Ind_Total)%2));
			KerArg0->Pad = (v4s) ((v4s){0,1,0*(T0Ind==0),1*(T0Ind_Last)});
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride2_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride2_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+27056+3136*((T0Ind_Total)%2));
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+26896+8*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27056+3136*((T0Ind_Total)%2)),
					_SC_Out, 3136, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (392); _LC_Out = (392); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (25088)+(-2744); _LC_Out = (392); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer4(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 40768 bytes, L2 buffer: 40576 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 38][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 38 logical tiles, 38 physical tiles
			Total Size: 200704 [Tile1, 38:[64x84, 36:64x84, 64x28], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 38:[64x84, 36:64x84, 64x28], 1]
		Tile0: [0, 5376, 84], Tile1: [84, 5376, 84], Tile2; [168, 5376, 84]
		T0: [Tile1: 0], T1: [Tile1: 0 +1/20], T2: [Tile1: 0 +2/40]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 8192 [Tile0, 1:[64x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[64x128], 1]
		Tile0: [0, 8192, 8192], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 38 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 38:[64x1, 36:64x1, 64x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 38:[64x1, 36:64x1, 64x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile1: 0], T1: [Tile1: 0], T2: [Tile1: 0]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x128], 1]
		Tile0: [0, 128, 128], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: MulBias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x128], 1]
		Tile0: [0, 128, 128], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 38 logical tiles, 38 physical tiles
			Total Size: 401408 [Tile1, 38:[128x84, 36:128x84, 128x28], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 38:[128x84, 36:128x84, 128x28], 1]
		Tile0: [0, 10752, 84], Tile1: [84, 10752, 84], Tile2; [168, 10752, 84]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 0 +1/40]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (L1_Memory+32320);
	KerArg0->W_In1 = (unsigned short int) (64);
	KerArg0->Bias = (signed char * __restrict__) (L1_Memory+40512);
	KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+40640);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+0);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (1);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+64+0), 5376, 3136, 84, 0, &DmaR_Evt1);
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+32320), 8192, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+40512), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+40640), 128, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
	_C_Out=0; _SC_Out=10752; _LC_Out=84;
	_SPP_Out=0; _SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<38; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==37), T1Ind_NextLast = ((T1Ind+1)==37);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (84); _LN_In2 = ((T1Ind_NextLast)?28:84); _SN_In2 = (64*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+64+5376*((T1Ind_Total+1)%2)),
					_SN_In2, 3136, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->H_In1 = (unsigned short int) (128);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+64+5376*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?28:84);
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+10816+10752*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?28:84);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*128);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+10752*((T1Ind_Total+-1)%2)),
					_SP_Out, 3136, _LP_Out, 1, &Uchan1);
		AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+10752*((T1Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+10816+10752*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (84); _LC_Out = ((T1Ind_NextLast)?28:84); _SC_Out = (128*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+10752*((T1Ind_Total+-1)%2)), _SP_Out, 3136, _LP_Out, 1, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer5(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 48560 bytes, L2 buffer: 25248 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	AT_HYPERRAM_CL_EVENT Uchan2;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 16][Tile0 Dim: 5]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 80 logical tiles, 80 physical tiles
			Total Size: 401408 [D0, 16 x 25088][Tile0, 5:[56x14, 3:56x15, 56x5], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 25088][Tile0, 5:[56x14, 3:56x15, 56x5], 1]
		Tile0: [0, 6272, 784], Tile1: [672, 6720, 840], Tile2; [1400, 6720, 840]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 128 [D0, 16 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 128 [D0, 16 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 1152 [D0, 16 x 72]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 72]
		Tile0: [0, 72, 72], Tile1: [72, 72, 72], Tile2; [144, 72, 72]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 80 logical tiles, 80 physical tiles
			Total Size: 401408 [D0, 16 x 25088][Tile0, 5:[56x13, 3:56x13, 56x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 25088][Tile0, 5:[56x13, 3:56x13, 56x4], 1]
		Tile0: [0, 5824, 728], Tile1: [728, 5824, 728], Tile2; [1456, 5824, 728]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 80 logical tiles, 1 physical tiles
			Total Size: 1605632 [D0, 16 x 100352][Tile0, 5:[56x13, 3:56x13, 56x4], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 100352][Tile0, 5:[56x13, 3:56x13, 56x4], 4]
		Tile0: [0, 1605632, 1605632], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (56);
	KerArg0->UsedW = (unsigned short int) (56);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+25264);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+25264);
	KerArg1->W = (unsigned short int) (56);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->InFeatures = (unsigned short int) (8);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_NN_In=672; _SN_In=6720;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+0), 6272, 3136, 784, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+672), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+6720), 6720, 3136, 840, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 6272, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+13440+0), 8, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+13456+0), 8, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+13472+0), 72, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=5824; _LC_Out=728;
	_SPP_Out=0; _SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<16; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==15), D0Ind_NextLast = ((D0Ind+1)==15), D0Ind_NextNextLast = ((D0Ind+2)==15);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (8); _SN_Bias = (8); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (8); _SN_MulBias = (8); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (72); _SN_Filter = (72); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+13440+8*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+13456+8*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+13472+72*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<5; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==4), T0Ind_NextLast = ((T0Ind+1)==4), T0Ind_NextNextLast = ((T0Ind+2)==4);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (728); _LNN_In = ((T0Ind_NextNextLast)?280:840); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (25088)+(-2856); _LNN_In = (784); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (728-56); _LNN_In = (840); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+6720*((T0Ind_Total)%2)),
						_SNN_In, 3136, _LNN_In, 0, &Uchan1);
			}
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+6720*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+6720*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+6720*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (((T0Ind_Last)?5:15)-1*(T0Ind==0));
			KerArg0->UsedH = (unsigned short int) (((T0Ind_Last)?5:15)-1*(T0Ind==0));
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+13472+72*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+13440+8*((D0Ind_Total)%2));
			KerArg0->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->H = (unsigned short int) (T0Ind_Last?4:13);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+13616+5824*((T0Ind_Total)%2));
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+13456+8*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
			if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+13440+5824*((T0Ind_Total+-1)%2)),
						_SP_Out, 3136, _LP_Out, 1, &Uchan2);
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+13440+5824*((T0Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+13616+5824*((T0Ind_Total)%2)),
					_SC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
			_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (728); _LC_Out = ((T0Ind_NextLast)?224:728); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (25088)+(-2912); _LC_Out = (728); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+13440+5824*((T0Ind_Total+-1)%2)), _SP_Out, 3136, _LP_Out, 1, &Uchan2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 41344 bytes, L2 buffer: 41088 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	AT_HYPERRAM_CL_EVENT Uchan2;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_In2;
	unsigned int _SN_In2, _SNN_In2;
	unsigned int _LN_In2, _LNN_In2;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 66][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 66 logical tiles, 66 physical tiles
			Total Size: 401408 [Tile1, 66:[128x48, 64:128x48, 128x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 66:[128x48, 64:128x48, 128x16], 1]
		Tile0: [0, 6144, 48], Tile1: [48, 6144, 48], Tile2; [96, 6144, 48]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16384 [Tile0, 1:[128x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[128x128], 1]
		Tile0: [0, 16384, 16384], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 66 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 66:[128x1, 64:128x1, 128x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 66:[128x1, 64:128x1, 128x1], 1]
		Tile0: [0, 128, 128], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile1: 0], T1: [Tile1: 0], T2: [Tile1: 0]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x128], 1]
		Tile0: [0, 128, 128], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: MulBias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x128], 1]
		Tile0: [0, 128, 128], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 66 logical tiles, 66 physical tiles
			Total Size: 401408 [Tile1, 66:[128x48, 64:128x48, 128x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 66:[128x48, 64:128x48, 128x16], 1]
		Tile0: [0, 6144, 48], Tile1: [48, 6144, 48], Tile2; [96, 6144, 48]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (L1_Memory+24704);
	KerArg0->W_In1 = (unsigned short int) (128);
	KerArg0->Bias = (signed char * __restrict__) (L1_Memory+41088);
	KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+41216);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+0);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (1);
	/*================================= Read Tiles Prolog ===============================*/
	_NN_In2=48; _SN_In2=6144;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+12288+0), 6144, 3136, 48, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In2 */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+48), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+12288+6144), 6144, 3136, 48, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+12288+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+128+0), 6144, 0, &DmaR_Evt1);
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24704), 16384, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+41088), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+41216), 128, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
	_C_Out=0; _SC_Out=6144; _LC_Out=48;
	_SPP_Out=0; _SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<66; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==65), T1Ind_NextLast = ((T1Ind+1)==65), T1Ind_NextNextLast = ((T1Ind+2)==65);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In2 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In2 = _NN_In2 + (48); _LNN_In2 = ((T1Ind_NextNextLast)?16:48); _SNN_In2 = (128*_LNN_In2); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In2 */
		if (_SNN_In2) {
			AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In2+_NN_In2), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+12288+6144*((T1Ind_Total)%2)),
					_SNN_In2, 3136, _LNN_In2, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+12288+6144*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+128+6144*((T1Ind_Total+1)%2)),
					_SN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->H_In1 = (unsigned short int) (128);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+128+6144*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?16:48);
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+12416+6144*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?16:48);
			KerArg0->OutFirstCol = (unsigned short int) ((0)*128);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+6144*((T1Ind_Total+-1)%2)),
					_SP_Out, 3136, _LP_Out, 1, &Uchan2);
		AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+6144*((T1Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12416+6144*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In2 = _SNN_In2;_LN_In2 = _LNN_In2;
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (48); _LC_Out = ((T1Ind_NextLast)?16:48); _SC_Out = (128*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+6144*((T1Ind_Total+-1)%2)), _SP_Out, 3136, _LP_Out, 1, &Uchan2);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer7(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 48112 bytes, L2 buffer: 34656 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 16][Tile0 Dim: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 100352 [D0, 16 x 6272][Tile0, 2:[28x15, 28x13], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 6272][Tile0, 2:[28x15, 28x13], 1]
		Tile0: [0, 3360, 420], Tile1: [420, 2912, 364], Tile2; [6272, 3360, 420]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 1][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 128 [D0, 16 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 128 [D0, 16 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 1152 [D0, 16 x 72]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 72]
		Tile0: [0, 72, 72], Tile1: [72, 72, 72], Tile2; [144, 72, 72]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 401408 [D0, 16 x 25088][Tile0, 2:[56x31, 56x26], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 25088][Tile0, 2:[56x31, 56x26], 1]
		Tile0: [0, 13888, 1736], Tile1: [1680, 11648, 1456], Tile2; [25088, 13888, 1736]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 1][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 32 logical tiles, 1 physical tiles
			Total Size: 401408 [D0, 16 x 25088][Tile0, 2:[28x15, 28x13], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 25088][Tile0, 2:[28x15, 28x13], 4]
		Tile0: [0, 401408, 401408], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (56);
	KerArg0->UsedW = (unsigned short int) (56);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+34672);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+34672);
	KerArg1->W = (unsigned short int) (28);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->InFeatures = (unsigned short int) (8);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3360; _LC_Out=420;
	_SP_Out=0;
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27776+0), 8, 0, &DmaR_Evt1);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27792+0), 8, 0, &DmaR_Evt2);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27808+0), 72, 0, &DmaR_Evt3);
	_NN_In=1680; _SN_In=11648;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+0), 13888, 3136, 1736, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+1680), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+13888), 11648, 3136, 1456, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 13888, 0, &DmaR_Evt4);
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<16; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==15), D0Ind_NextLast = ((D0Ind+1)==15), D0Ind_NextNextLast = ((D0Ind+2)==15);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (8); _SN_Bias = (8); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (8); _SN_MulBias = (8); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (72); _SN_Filter = (72); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27776+8*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27792+8*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27808+72*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt3);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<2; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==1), T0Ind_NextLast = ((T0Ind+1)==1), T0Ind_NextNextLast = ((T0Ind+2)==1);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (1680); _LNN_In = ((T0Ind_NextNextLast)?1456:1736); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (25088)+(-1680); _LNN_In = (1736); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (1680); _LNN_In = (1456); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+13888*((T0Ind_Total)%2)),
						_SNN_In, 3136, _LNN_In, 0, &Uchan1);
			}
			AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+13888*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+13888*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt4);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+13888*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (((T0Ind_Last)?26:31)-0*(T0Ind==0));
			KerArg0->UsedH = (unsigned short int) (((T0Ind_Last)?26:31)-0*(T0Ind==0));
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+27808+72*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+27776+8*((D0Ind_Total)%2));
			KerArg0->Pad = (v4s) ((v4s){0,1,0*(T0Ind==0),1*(T0Ind_Last)});
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride2_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride2_fps, KerArg0);
			KerArg1->H = (unsigned short int) (T0Ind_Last?13:15);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+27952+3360*((T0Ind_Total)%2));
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+27792+8*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27952+3360*((T0Ind_Total)%2)),
					_SC_Out, 784, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (420); _LC_Out = ((T0Ind_NextLast)?364:420); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (6272)+(-420); _LC_Out = (420); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer8(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46496 bytes, L2 buffer: 46224 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 22][Tile0 Dim: 4]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 1 physical tiles
			Total Size: 128 [Tile1, 22:[128x1, 20:128x1, 128x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[128x1, 20:128x1, 128x1], 1]
		Tile0: [0, 128, 128], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile1: 0], T1: [Tile1: 0], T2: [Tile1: 0]
	Ker Arg: In1, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 32768 [Tile0, 4:[128x72, 2:128x72, 128x40], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 4:[128x72, 2:128x72, 128x40], 1]
		Tile0: [0, 9216, 9216], Tile1: [9216, 9216, 9216], Tile2; [18432, 9216, 9216]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [Tile1, 22:[128x36, 20:128x36, 128x28], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[128x36, 20:128x36, 128x28], 1]
		Tile0: [0, 4608, 36], Tile1: [36, 4608, 36], Tile2; [72, 4608, 36]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Bias, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 256 [Tile0, 4:[1x72, 2:1x72, 1x40], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 4:[1x72, 2:1x72, 1x40], 1]
		Tile0: [0, 72, 72], Tile1: [72, 72, 72], Tile2; [144, 72, 72]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: MulBias, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 256 [Tile0, 4:[1x72, 2:1x72, 1x40], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 4:[1x72, 2:1x72, 1x40], 1]
		Tile0: [0, 72, 72], Tile1: [72, 72, 72], Tile2; [144, 72, 72]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 200704 [Tile1, 22:[256x36, 20:256x36, 256x28], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[256x36, 20:256x36, 256x28], 1]
		Tile0: [0, 9216, 36], Tile1: [36, 9216, 36], Tile2; [72, 9216, 36]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (128);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+0);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (1);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27776+0), 9216, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+128+0), 4608, 784, 36, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+46208+0), 72, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+46352+0), 72, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=9216; _LC_Out=36;
	_SPP_Out=0; _SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<22; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==21), T1Ind_NextLast = ((T1Ind+1)==21);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (36); _LN_In2 = ((T1Ind_NextLast)?28:36); _SN_In2 = (128*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+128+4608*((T1Ind_Total+1)%2)),
					_SN_In2, 784, _LN_In2, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<4; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==3), T0Ind_NextLast = ((T0Ind+1)==3);
			/*================================= Prepare Tiles ===================================*/
			_SN_In1 = 0;
			if (!(T0Ind_Last)) {
				_N_In1 = _N_In1 + (9216); _SN_In1 = ((T0Ind_NextLast)?5120:9216); 
			} else if (!(T1Ind_Last)) {
				_N_In1 = _N_In1 + (-27648); _SN_In1 = (9216); 
			}
			_SN_Bias = 0;
			if (!(T0Ind_Last)) {
				_N_Bias = _N_Bias + (72); _SN_Bias = ((T0Ind_NextLast)?40:72); 
			} else if (!(T1Ind_Last)) {
				_N_Bias = _N_Bias + (-216); _SN_Bias = (72); 
			}
			_SN_MulBias = 0;
			if (!(T0Ind_Last)) {
				_N_MulBias = _N_MulBias + (72); _SN_MulBias = ((T0Ind_NextLast)?40:72); 
			} else if (!(T1Ind_Last)) {
				_N_MulBias = _N_MulBias + (-216); _SN_MulBias = (72); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
			if (_SN_In1) {
				AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27776+9216*((T0Ind_Total+1)%2)),
						_SN_In1, 0, &DmaR_Evt1);
			}
			AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
			if (_SN_Bias) {
				AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+46208+72*((T0Ind_Total+1)%2)),
						_SN_Bias, 0, &DmaR_Evt3);
			}
			AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
			if (_SN_MulBias) {
				AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+46352+72*((T0Ind_Total+1)%2)),
						_SN_MulBias, 0, &DmaR_Evt4);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+27776+9216*((T0Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T0Ind_Last?40:72);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+128+4608*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?28:36);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+46208+72*((T0Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+46352+72*((T0Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+9344+9216*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?28:36);
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*72);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+9216*((T1Ind_Total+-1)%2)),
					_SP_Out, 784, _LP_Out, 1, &Uchan1);
		AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+9216*((T1Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9344+9216*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (36); _LC_Out = ((T1Ind_NextLast)?28:36); _SC_Out = (256*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+9216*((T1Ind_Total+-1)%2)), _SP_Out, 784, _LP_Out, 1, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer9(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 49008 bytes, L2 buffer: 24800 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 32][Tile0 Dim: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 64 logical tiles, 64 physical tiles
			Total Size: 200704 [D0, 32 x 6272][Tile0, 2:[28x28, 28x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 32 x 6272][Tile0, 2:[28x28, 28x2], 1]
		Tile0: [0, 6272, 784], Tile1: [728, 448, 56], Tile2; [6272, 6272, 784]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 1][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 256 [D0, 32 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 32 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 256 [D0, 32 x 8]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 32 x 8]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 2304 [D0, 32 x 72]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 32 x 72]
		Tile0: [0, 72, 72], Tile1: [72, 72, 72], Tile2; [144, 72, 72]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 64 logical tiles, 64 physical tiles
			Total Size: 200704 [D0, 32 x 6272][Tile0, 2:[28x27, 28x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 32 x 6272][Tile0, 2:[28x27, 28x1], 1]
		Tile0: [0, 6048, 756], Tile1: [756, 224, 28], Tile2; [6272, 6048, 756]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 1][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 64 logical tiles, 1 physical tiles
			Total Size: 802816 [D0, 32 x 25088][Tile0, 2:[28x27, 28x1], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 32 x 25088][Tile0, 2:[28x27, 28x1], 4]
		Tile0: [0, 802816, 802816], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (28);
	KerArg0->UsedW = (unsigned short int) (28);
	KerArg0->InFeatures = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (8);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+24816);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+24816);
	KerArg1->W = (unsigned short int) (28);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->InFeatures = (unsigned short int) (8);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_NN_In=728; _SN_In=448;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+0), 6272, 784, 784, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+728), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+6272), 448, 784, 56, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 6272, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12544+0), 8, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12560+0), 8, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12576+0), 72, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=6048; _LC_Out=756;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<32; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==31), D0Ind_NextLast = ((D0Ind+1)==31), D0Ind_NextNextLast = ((D0Ind+2)==31);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (8); _SN_Bias = (8); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (8); _SN_MulBias = (8); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (72); _SN_Filter = (72); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12544+8*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12560+8*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12576+72*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<2; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==1), T0Ind_NextLast = ((T0Ind+1)==1), T0Ind_NextNextLast = ((T0Ind+2)==1);
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(T0Ind_Last)) {
				if (!(T0Ind_NextLast)) {
					_NN_In = _NN_In + (756); _LNN_In = ((T0Ind_NextNextLast)?56:812); _SNN_In = (8*_LNN_In); 
				} else if (!(D0Ind_Last)) {
					_NN_In = _NN_In + (6272)+(-728); _LNN_In = (784); _SNN_In = (8*_LNN_In); 
				}
			} else if (!(D0Ind_Last)) {
				_NN_In = _NN_In + (756-28); _LNN_In = (56); _SNN_In = (8*_LNN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+6272*((T0Ind_Total)%2)),
						_SNN_In, 784, _LNN_In, 0, &Uchan1);
			}
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+6272*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+6272*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+6272*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (((T0Ind_Last)?2:29)-1*(T0Ind==0));
			KerArg0->UsedH = (unsigned short int) (((T0Ind_Last)?2:29)-1*(T0Ind==0));
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+12576+72*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+12544+8*((D0Ind_Total)%2));
			KerArg0->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->H = (unsigned short int) (T0Ind_Last?1:27);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+12720+6048*((T0Ind_Total)%2));
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+12560+8*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12720+6048*((T0Ind_Total)%2)),
					_SC_Out, 784, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (756); _LC_Out = ((T0Ind_NextLast)?28:756); _SC_Out = (8*_LC_Out); 
			} else if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (6272)+(-756); _LC_Out = (756); _SC_Out = (8*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer10(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 45472 bytes, L2 buffer: 45136 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _P_Out, _C_Out;
	unsigned int _SPP_Out, _SP_Out, _SC_Out;
	unsigned int _LPP_Out, _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 33][Tile0 Dim: 7]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 33 logical tiles, 1 physical tiles
			Total Size: 256 [Tile1, 33:[256x1, 31:256x1, 256x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 33:[256x1, 31:256x1, 256x1], 1]
		Tile0: [0, 256, 256], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile1: 0], T1: [Tile1: 0], T2: [Tile1: 0]
	Ker Arg: In1, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 65536 [Tile0, 7:[256x40, 5:256x40, 256x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[256x40, 5:256x40, 256x16], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 33 logical tiles, 33 physical tiles
			Total Size: 200704 [Tile1, 33:[256x24, 31:256x24, 256x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 33:[256x24, 31:256x24, 256x16], 1]
		Tile0: [0, 6144, 24], Tile1: [24, 6144, 24], Tile2; [48, 6144, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Bias, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 256 [Tile0, 7:[1x40, 5:1x40, 1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[1x40, 5:1x40, 1x16], 1]
		Tile0: [0, 40, 40], Tile1: [40, 40, 40], Tile2; [80, 40, 40]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: MulBias, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 256 [Tile0, 7:[1x40, 5:1x40, 1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[1x40, 5:1x40, 1x16], 1]
		Tile0: [0, 40, 40], Tile1: [40, 40, 40], Tile2; [80, 40, 40]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -2, Max Pipe Depth: 0
		KerArgItSpace: 33 logical tiles, 33 physical tiles
			Total Size: 200704 [Tile1, 33:[256x24, 31:256x24, 256x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 33:[256x24, 31:256x24, 256x16], 1]
		Tile0: [0, 6144, 24], Tile1: [24, 6144, 24], Tile2; [48, 6144, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (256);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+0);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (1);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24832+0), 10240, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+256+0), 6144, 784, 24, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+45312+0), 40, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+45392+0), 40, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=6144; _LC_Out=24;
	_SPP_Out=0; _SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<33; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==32), T1Ind_NextLast = ((T1Ind+1)==32);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (24); _LN_In2 = ((T1Ind_NextLast)?16:24); _SN_In2 = (256*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+256+6144*((T1Ind_Total+1)%2)),
					_SN_In2, 784, _LN_In2, 0, &DmaR_Evt2);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<7; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==6), T0Ind_NextLast = ((T0Ind+1)==6);
			/*================================= Prepare Tiles ===================================*/
			_SN_In1 = 0;
			if (!(T0Ind_Last)) {
				_N_In1 = _N_In1 + (10240); _SN_In1 = ((T0Ind_NextLast)?4096:10240); 
			} else if (!(T1Ind_Last)) {
				_N_In1 = _N_In1 + (-61440); _SN_In1 = (10240); 
			}
			_SN_Bias = 0;
			if (!(T0Ind_Last)) {
				_N_Bias = _N_Bias + (40); _SN_Bias = ((T0Ind_NextLast)?16:40); 
			} else if (!(T1Ind_Last)) {
				_N_Bias = _N_Bias + (-240); _SN_Bias = (40); 
			}
			_SN_MulBias = 0;
			if (!(T0Ind_Last)) {
				_N_MulBias = _N_MulBias + (40); _SN_MulBias = ((T0Ind_NextLast)?16:40); 
			} else if (!(T1Ind_Last)) {
				_N_MulBias = _N_MulBias + (-240); _SN_MulBias = (40); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
			if (_SN_In1) {
				AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24832+10240*((T0Ind_Total+1)%2)),
						_SN_In1, 0, &DmaR_Evt1);
			}
			AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
			if (_SN_Bias) {
				AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+45312+40*((T0Ind_Total+1)%2)),
						_SN_Bias, 0, &DmaR_Evt3);
			}
			AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
			if (_SN_MulBias) {
				AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+45392+40*((T0Ind_Total+1)%2)),
						_SN_MulBias, 0, &DmaR_Evt4);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+24832+10240*((T0Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T0Ind_Last?16:40);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+256+6144*((T1Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T1Ind_Last)?16:24);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+45312+40*((T0Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+45392+40*((T0Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+12544+6144*((T1Ind_Total)%2));
			KerArg0->W_Out = (unsigned short int) ((T1Ind_Last)?16:24);
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*40);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA write Out */
		if (_SP_Out) AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+6144*((T1Ind_Total+-1)%2)),
					_SP_Out, 784, _LP_Out, 1, &Uchan1);
		AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+6144*((T1Ind_Total)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12544+6144*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SPP_Out = _SP_Out;_LPP_Out = _LP_Out;
		_P_Out = _C_Out;_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (24); _LC_Out = ((T1Ind_NextLast)?16:24); _SC_Out = (256*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	if (_SPP_Out) AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA write Out */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Out+_P_Out), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+6144*((T1Ind_Total+-1)%2)), _SP_Out, 784, _LP_Out, 1, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait current uDMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer11(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 44256 bytes, L2 buffer: 31680 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _LN_In, _LNN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 16][Tile0 Dim: 1]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 50176 [D0, 16 x 3136][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 3136][Tile0, 1:[14x14], 1]
		Tile0: [0, 3136, 196], Tile1: [3136, 3136, 196], Tile2; [6272, 3136, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 256 [D0, 16 x 16]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 16]
		Tile0: [0, 16, 16], Tile1: [16, 16, 16], Tile2; [32, 16, 16]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 256 [D0, 16 x 16]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 16]
		Tile0: [0, 16, 16], Tile1: [16, 16, 16], Tile2; [32, 16, 16]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 2304 [D0, 16 x 144]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 144]
		Tile0: [0, 144, 144], Tile1: [144, 144, 144], Tile2; [288, 144, 144]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 200704 [D0, 16 x 12544][Tile0, 1:[28x28], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 12544][Tile0, 1:[28x28], 1]
		Tile0: [0, 12544, 784], Tile1: [12544, 12544, 784], Tile2; [25088, 12544, 784]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 16 logical tiles, 1 physical tiles
			Total Size: 200704 [D0, 16 x 12544][Tile0, 1:[14x14], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 16 x 12544][Tile0, 1:[14x14], 4]
		Tile0: [0, 200704, 200704], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (28);
	KerArg0->UsedW = (unsigned short int) (28);
	KerArg0->InFeatures = (unsigned short int) (16);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+31712);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Pad = (v4s) ((v4s){0,1,0,1});
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+31712);
	KerArg1->W = (unsigned short int) (14);
	KerArg1->H = (unsigned short int) (14);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->InFeatures = (unsigned short int) (16);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3136; _LC_Out=196;
	_SP_Out=0;
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25088+0), 16, 0, &DmaR_Evt1);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25120+0), 16, 0, &DmaR_Evt2);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25152+0), 144, 0, &DmaR_Evt3);
	_NN_In=12544; _SN_In=12544;
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+50176+0), 12544, 784, 784, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+12544), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+50176+12544), 12544, 784, 784, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+50176+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 12544, 0, &DmaR_Evt4);
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<16; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==15), D0Ind_NextLast = ((D0Ind+1)==15), D0Ind_NextNextLast = ((D0Ind+2)==15);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (16); _SN_Bias = (16); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (16); _SN_MulBias = (16); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (144); _SN_Filter = (144); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25088+16*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25120+16*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25152+144*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt3);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1, T0Ind_NextNextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SNN_In = 0;
			if (!(D0Ind_Last)) {
				if (!(D0Ind_NextLast)) {
					_NN_In = _NN_In + (12544); _LNN_In = (784); _SNN_In = (16*_LNN_In); 
				}
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In */
			if (_SNN_In) {
				AT_HYPERRAM_CL_COPY2D(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+50176+12544*((T0Ind_Total)%2)),
						_SNN_In, 784, _LNN_In, 0, &Uchan1);
			}
			AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+50176+12544*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+12544*((T0Ind_Total+1)%2)),
						_SN_In, 0, &DmaR_Evt4);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+12544*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (29-0*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (29-0*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+25152+144*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+25088+16*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride2_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride2_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+25440+3136*((T0Ind_Total)%2));
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+25120+16*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25440+3136*((T0Ind_Total)%2)),
					_SC_Out, 196, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			
			_SN_In = _SNN_In;_LN_In = _LNN_In;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (3136); _LC_Out = (196); _SC_Out = (16*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer12(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46816 bytes, L2 buffer: 46480 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 13][Tile0 Dim: 10]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 50176 [Tile0, 10:[256x20, 8:256x20, 256x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 10:[256x20, 8:256x20, 256x16], 1]
		Tile0: [0, 5120, 20], Tile1: [20, 5120, 20], Tile2; [40, 5120, 20]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 13 logical tiles, 13 physical tiles
			Total Size: 131072 [Tile1, 13:[256x40, 11:256x40, 256x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 13:[256x40, 11:256x40, 256x32], 1]
		Tile0: [0, 10240, 10240], Tile1: [10240, 10240, 10240], Tile2; [20480, 10240, 10240]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 1 physical tiles
			Total Size: 256 [Tile0, 10:[256x1, 8:256x1, 256x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 10:[256x1, 8:256x1, 256x1], 1]
		Tile0: [0, 256, 256], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Bias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 13 logical tiles, 13 physical tiles
			Total Size: 512 [Tile1, 13:[1x40, 11:1x40, 1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 13:[1x40, 11:1x40, 1x32], 1]
		Tile0: [0, 40, 40], Tile1: [40, 40, 40], Tile2; [80, 40, 40]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: MulBias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 13 logical tiles, 13 physical tiles
			Total Size: 512 [Tile1, 13:[1x40, 11:1x40, 1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 13:[1x40, 11:1x40, 1x32], 1]
		Tile0: [0, 40, 40], Tile1: [40, 40, 40], Tile2; [80, 40, 40]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 13 logical tiles, 13 physical tiles
			Total Size: 100352 [Tile1, 13:[196x40, 11:196x40, 196x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 13:[196x40, 11:196x40, 196x32], 1]
		Tile0: [0, 7840, 7840], Tile1: [7840, 7840, 7840], Tile2; [15680, 7840, 7840]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (256);
	KerArg0->W_Out = (unsigned short int) (196);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+36320);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (0);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+36576+0), 5120, 196, 20, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 10240, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+20480+0), 40, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+20560+0), 40, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=7840;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<13; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==12), T1Ind_NextLast = ((T1Ind+1)==12);
		/*================================= Prepare Tiles ===================================*/
		_SN_In1 = 0;
		if (!(T1Ind_Last)) {
			_N_In1 = _N_In1 + (10240); _SN_In1 = ((T1Ind_NextLast)?8192:10240); 
		}
		_SN_Bias = 0;
		if (!(T1Ind_Last)) {
			_N_Bias = _N_Bias + (40); _SN_Bias = ((T1Ind_NextLast)?32:40); 
		}
		_SN_MulBias = 0;
		if (!(T1Ind_Last)) {
			_N_MulBias = _N_MulBias + (40); _SN_MulBias = ((T1Ind_NextLast)?32:40); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+10240*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+20480+40*((T1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+20560+40*((T1Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<10; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==9), T0Ind_NextLast = ((T0Ind+1)==9);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (20); _LN_In2 = ((T0Ind_NextLast)?16:20); _SN_In2 = (256*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-180); _LN_In2 = (20); _SN_In2 = (256*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+36576+5120*((T0Ind_Total+1)%2)),
						_SN_In2, 196, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+0+10240*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?32:40);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+36576+5120*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?16:20);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+20480+40*((T1Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+20560+40*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+20640+7840*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*20);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+20640+7840*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (7840); _SC_Out = ((T1Ind_NextLast)?6272:7840); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer13(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 38160 bytes, L2 buffer: 19296 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 22][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 4608 [D0, 22 x 216]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 216]
		Tile0: [0, 216, 216], Tile1: [216, 216, 216], Tile2; [432, 216, 216]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 1 physical tiles
			Total Size: 401408 [D0, 22 x 18816][Tile0, 1:[14x14], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 18816][Tile0, 1:[14x14], 4]
		Tile0: [0, 401408, 401408], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (14);
	KerArg0->UsedW = (unsigned short int) (14);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+19344);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+19344);
	KerArg1->W = (unsigned short int) (14);
	KerArg1->H = (unsigned short int) (14);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 4704, 196, 196, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+0), 24, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+0), 24, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+0), 216, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704; _LC_Out=196;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<22; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==21), D0Ind_NextLast = ((D0Ind+1)==21);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (216); _SN_Filter = ((D0Ind_NextLast)?72:216); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+24*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+24*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+216*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(D0Ind_Last)) {
				_N_In = _N_In + (4704); _LN_In = (196); _SN_In = (((D0Ind_NextLast)?8:24)*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+4704*((T0Ind_Total+1)%2)),
						_SN_In, 196, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+4704*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->OutFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+9504+216*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+9408+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+9936+4704*((T0Ind_Total)%2));
			KerArg1->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+9456+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9936+4704*((T0Ind_Total)%2)),
					_SC_Out, 196, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (4704); _LC_Out = (196); _SC_Out = (((D0Ind_NextLast)?8:24)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer14(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46880 bytes, L2 buffer: 46320 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 22][Tile0 Dim: 17]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 17 logical tiles, 17 physical tiles
			Total Size: 100352 [Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		Tile0: [0, 6144, 12], Tile1: [12, 6144, 12], Tile2; [24, 6144, 12]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 262144 [Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		Tile0: [0, 12288, 12288], Tile1: [12288, 12288, 12288], Tile2; [24576, 12288, 12288]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 17 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Bias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: MulBias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		Tile0: [0, 4704, 4704], Tile1: [4704, 4704, 4704], Tile2; [9408, 4704, 4704]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (512);
	KerArg0->W_Out = (unsigned short int) (196);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+34080);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (0);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+0), 6144, 196, 12, 0, &DmaR_Evt1);
	_NN_In1=12288; _SN_In1=12288;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+100352+0), 12288, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+12288), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+100352+12288), 12288, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+100352+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 12288, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+0), 24, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+0), 24, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<22; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==21), T1Ind_NextLast = ((T1Ind+1)==21), T1Ind_NextNextLast = ((T1Ind+2)==21);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (12288); _SNN_In1 = ((T1Ind_NextNextLast)?4096:12288); 
			}
		}
		_SN_Bias = 0;
		if (!(T1Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((T1Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(T1Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((T1Ind_NextLast)?8:24); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+100352+12288*((T1Ind_Total)%2)),
					_SNN_In1, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+100352+12288*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+12288*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+24*((T1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+24*((T1Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<17; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==16), T0Ind_NextLast = ((T0Ind+1)==16);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (12); _LN_In2 = ((T0Ind_NextLast)?4:12); _SN_In2 = (512*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-192); _LN_In2 = (12); _SN_In2 = (512*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+6144*((T0Ind_Total+1)%2)),
						_SN_In2, 196, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+0+12288*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?8:24);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+34592+6144*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?4:12);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+24576+24*((T1Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+24624+24*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+24672+4704*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*12);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24672+4704*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In1 = _SNN_In1;
		
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (4704); _SC_Out = ((T1Ind_NextLast)?1568:4704); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer15(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 38160 bytes, L2 buffer: 19296 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 22][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 4608 [D0, 22 x 216]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 216]
		Tile0: [0, 216, 216], Tile1: [216, 216, 216], Tile2; [432, 216, 216]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 1 physical tiles
			Total Size: 401408 [D0, 22 x 18816][Tile0, 1:[14x14], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 18816][Tile0, 1:[14x14], 4]
		Tile0: [0, 401408, 401408], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (14);
	KerArg0->UsedW = (unsigned short int) (14);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+19344);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+19344);
	KerArg1->W = (unsigned short int) (14);
	KerArg1->H = (unsigned short int) (14);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 4704, 196, 196, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+0), 24, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+0), 24, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+0), 216, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704; _LC_Out=196;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<22; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==21), D0Ind_NextLast = ((D0Ind+1)==21);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (216); _SN_Filter = ((D0Ind_NextLast)?72:216); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+24*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+24*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+216*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(D0Ind_Last)) {
				_N_In = _N_In + (4704); _LN_In = (196); _SN_In = (((D0Ind_NextLast)?8:24)*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+4704*((T0Ind_Total+1)%2)),
						_SN_In, 196, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+4704*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->OutFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+9504+216*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+9408+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+9936+4704*((T0Ind_Total)%2));
			KerArg1->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+9456+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9936+4704*((T0Ind_Total)%2)),
					_SC_Out, 196, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (4704); _LC_Out = (196); _SC_Out = (((D0Ind_NextLast)?8:24)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer16(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46880 bytes, L2 buffer: 46320 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 22][Tile0 Dim: 17]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 17 logical tiles, 17 physical tiles
			Total Size: 100352 [Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		Tile0: [0, 6144, 12], Tile1: [12, 6144, 12], Tile2; [24, 6144, 12]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 262144 [Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		Tile0: [0, 12288, 12288], Tile1: [12288, 12288, 12288], Tile2; [24576, 12288, 12288]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 17 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Bias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: MulBias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		Tile0: [0, 4704, 4704], Tile1: [4704, 4704, 4704], Tile2; [9408, 4704, 4704]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (512);
	KerArg0->W_Out = (unsigned short int) (196);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+34080);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (0);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+0), 6144, 196, 12, 0, &DmaR_Evt1);
	_NN_In1=12288; _SN_In1=12288;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+0), 12288, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+12288), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+12288), 12288, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 12288, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+0), 24, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+0), 24, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<22; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==21), T1Ind_NextLast = ((T1Ind+1)==21), T1Ind_NextNextLast = ((T1Ind+2)==21);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (12288); _SNN_In1 = ((T1Ind_NextNextLast)?4096:12288); 
			}
		}
		_SN_Bias = 0;
		if (!(T1Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((T1Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(T1Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((T1Ind_NextLast)?8:24); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+12288*((T1Ind_Total)%2)),
					_SNN_In1, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+12288*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+12288*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+24*((T1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+24*((T1Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<17; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==16), T0Ind_NextLast = ((T0Ind+1)==16);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (12); _LN_In2 = ((T0Ind_NextLast)?4:12); _SN_In2 = (512*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-192); _LN_In2 = (12); _SN_In2 = (512*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+6144*((T0Ind_Total+1)%2)),
						_SN_In2, 196, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+0+12288*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?8:24);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+34592+6144*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?4:12);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+24576+24*((T1Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+24624+24*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+24672+4704*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*12);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24672+4704*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In1 = _SNN_In1;
		
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (4704); _SC_Out = ((T1Ind_NextLast)?1568:4704); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer17(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 38160 bytes, L2 buffer: 19296 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 22][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 4608 [D0, 22 x 216]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 216]
		Tile0: [0, 216, 216], Tile1: [216, 216, 216], Tile2; [432, 216, 216]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 1 physical tiles
			Total Size: 401408 [D0, 22 x 18816][Tile0, 1:[14x14], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 18816][Tile0, 1:[14x14], 4]
		Tile0: [0, 401408, 401408], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (14);
	KerArg0->UsedW = (unsigned short int) (14);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+19344);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+19344);
	KerArg1->W = (unsigned short int) (14);
	KerArg1->H = (unsigned short int) (14);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 4704, 196, 196, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+0), 24, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+0), 24, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+0), 216, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704; _LC_Out=196;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<22; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==21), D0Ind_NextLast = ((D0Ind+1)==21);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (216); _SN_Filter = ((D0Ind_NextLast)?72:216); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+24*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+24*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+216*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(D0Ind_Last)) {
				_N_In = _N_In + (4704); _LN_In = (196); _SN_In = (((D0Ind_NextLast)?8:24)*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+4704*((T0Ind_Total+1)%2)),
						_SN_In, 196, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+4704*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->OutFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+9504+216*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+9408+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+9936+4704*((T0Ind_Total)%2));
			KerArg1->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+9456+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9936+4704*((T0Ind_Total)%2)),
					_SC_Out, 196, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (4704); _LC_Out = (196); _SC_Out = (((D0Ind_NextLast)?8:24)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer18(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46880 bytes, L2 buffer: 46320 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 22][Tile0 Dim: 17]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 17 logical tiles, 17 physical tiles
			Total Size: 100352 [Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		Tile0: [0, 6144, 12], Tile1: [12, 6144, 12], Tile2; [24, 6144, 12]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 262144 [Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		Tile0: [0, 12288, 12288], Tile1: [12288, 12288, 12288], Tile2; [24576, 12288, 12288]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 17 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Bias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: MulBias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		Tile0: [0, 4704, 4704], Tile1: [4704, 4704, 4704], Tile2; [9408, 4704, 4704]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (512);
	KerArg0->W_Out = (unsigned short int) (196);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+34080);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (0);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+0), 6144, 196, 12, 0, &DmaR_Evt1);
	_NN_In1=12288; _SN_In1=12288;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+0), 12288, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+12288), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+12288), 12288, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 12288, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+0), 24, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+0), 24, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<22; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==21), T1Ind_NextLast = ((T1Ind+1)==21), T1Ind_NextNextLast = ((T1Ind+2)==21);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (12288); _SNN_In1 = ((T1Ind_NextNextLast)?4096:12288); 
			}
		}
		_SN_Bias = 0;
		if (!(T1Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((T1Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(T1Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((T1Ind_NextLast)?8:24); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+0+12288*((T1Ind_Total)%2)),
					_SNN_In1, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+0+12288*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+12288*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+24*((T1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+24*((T1Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<17; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==16), T0Ind_NextLast = ((T0Ind+1)==16);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (12); _LN_In2 = ((T0Ind_NextLast)?4:12); _SN_In2 = (512*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-192); _LN_In2 = (12); _SN_In2 = (512*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+6144*((T0Ind_Total+1)%2)),
						_SN_In2, 196, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+0+12288*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?8:24);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+34592+6144*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?4:12);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+24576+24*((T1Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+24624+24*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+24672+4704*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*12);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24672+4704*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In1 = _SNN_In1;
		
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (4704); _SC_Out = ((T1Ind_NextLast)?1568:4704); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer19(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 38160 bytes, L2 buffer: 19296 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 22][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 4608 [D0, 22 x 216]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 216]
		Tile0: [0, 216, 216], Tile1: [216, 216, 216], Tile2; [432, 216, 216]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 1 physical tiles
			Total Size: 401408 [D0, 22 x 18816][Tile0, 1:[14x14], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 18816][Tile0, 1:[14x14], 4]
		Tile0: [0, 401408, 401408], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (14);
	KerArg0->UsedW = (unsigned short int) (14);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+19344);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+19344);
	KerArg1->W = (unsigned short int) (14);
	KerArg1->H = (unsigned short int) (14);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 4704, 196, 196, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+0), 24, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+0), 24, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+0), 216, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704; _LC_Out=196;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<22; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==21), D0Ind_NextLast = ((D0Ind+1)==21);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (216); _SN_Filter = ((D0Ind_NextLast)?72:216); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+24*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+24*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+216*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(D0Ind_Last)) {
				_N_In = _N_In + (4704); _LN_In = (196); _SN_In = (((D0Ind_NextLast)?8:24)*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+4704*((T0Ind_Total+1)%2)),
						_SN_In, 196, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+4704*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->OutFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+9504+216*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+9408+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+9936+4704*((T0Ind_Total)%2));
			KerArg1->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+9456+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9936+4704*((T0Ind_Total)%2)),
					_SC_Out, 196, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (4704); _LC_Out = (196); _SC_Out = (((D0Ind_NextLast)?8:24)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer20(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46880 bytes, L2 buffer: 46320 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 22][Tile0 Dim: 17]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 17 logical tiles, 17 physical tiles
			Total Size: 100352 [Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		Tile0: [0, 6144, 12], Tile1: [12, 6144, 12], Tile2; [24, 6144, 12]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 262144 [Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		Tile0: [0, 12288, 12288], Tile1: [12288, 12288, 12288], Tile2; [24576, 12288, 12288]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 17 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Bias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: MulBias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		Tile0: [0, 4704, 4704], Tile1: [4704, 4704, 4704], Tile2; [9408, 4704, 4704]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (512);
	KerArg0->W_Out = (unsigned short int) (196);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+34080);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (0);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+0), 6144, 196, 12, 0, &DmaR_Evt1);
	_NN_In1=12288; _SN_In1=12288;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+0), 12288, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+12288), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+12288), 12288, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 12288, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+0), 24, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+0), 24, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<22; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==21), T1Ind_NextLast = ((T1Ind+1)==21), T1Ind_NextNextLast = ((T1Ind+2)==21);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (12288); _SNN_In1 = ((T1Ind_NextNextLast)?4096:12288); 
			}
		}
		_SN_Bias = 0;
		if (!(T1Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((T1Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(T1Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((T1Ind_NextLast)?8:24); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+12288*((T1Ind_Total)%2)),
					_SNN_In1, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+12288*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+12288*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+24*((T1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+24*((T1Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<17; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==16), T0Ind_NextLast = ((T0Ind+1)==16);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (12); _LN_In2 = ((T0Ind_NextLast)?4:12); _SN_In2 = (512*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-192); _LN_In2 = (12); _SN_In2 = (512*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+6144*((T0Ind_Total+1)%2)),
						_SN_In2, 196, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+0+12288*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?8:24);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+34592+6144*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?4:12);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+24576+24*((T1Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+24624+24*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+24672+4704*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*12);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24672+4704*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In1 = _SNN_In1;
		
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (4704); _SC_Out = ((T1Ind_NextLast)?1568:4704); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer21(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 38160 bytes, L2 buffer: 19296 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 22][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [D0, 22 x 24]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 24]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 4608 [D0, 22 x 216]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 216]
		Tile0: [0, 216, 216], Tile1: [216, 216, 216], Tile2; [432, 216, 216]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [D0, 22 x 4704][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 4704][Tile0, 1:[14x14], 1]
		Tile0: [0, 4704, 196], Tile1: [4704, 4704, 196], Tile2; [9408, 4704, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 1 physical tiles
			Total Size: 401408 [D0, 22 x 18816][Tile0, 1:[14x14], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 22 x 18816][Tile0, 1:[14x14], 4]
		Tile0: [0, 401408, 401408], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (14);
	KerArg0->UsedW = (unsigned short int) (14);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+19344);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+19344);
	KerArg1->W = (unsigned short int) (14);
	KerArg1->H = (unsigned short int) (14);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 4704, 196, 196, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+0), 24, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+0), 24, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+0), 216, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704; _LC_Out=196;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<22; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==21), D0Ind_NextLast = ((D0Ind+1)==21);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((D0Ind_NextLast)?8:24); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (216); _SN_Filter = ((D0Ind_NextLast)?72:216); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9408+24*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9456+24*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9504+216*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(D0Ind_Last)) {
				_N_In = _N_In + (4704); _LN_In = (196); _SN_In = (((D0Ind_NextLast)?8:24)*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+4704*((T0Ind_Total+1)%2)),
						_SN_In, 196, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+4704*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (16-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->OutFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+9504+216*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+9408+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+9936+4704*((T0Ind_Total)%2));
			KerArg1->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+9456+24*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9936+4704*((T0Ind_Total)%2)),
					_SC_Out, 196, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (4704); _LC_Out = (196); _SC_Out = (((D0Ind_NextLast)?8:24)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer22(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46880 bytes, L2 buffer: 46320 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 22][Tile0 Dim: 17]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 17 logical tiles, 17 physical tiles
			Total Size: 100352 [Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x12, 15:512x12, 512x4], 1]
		Tile0: [0, 6144, 12], Tile1: [12, 6144, 12], Tile2; [24, 6144, 12]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 262144 [Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[512x24, 20:512x24, 512x8], 1]
		Tile0: [0, 12288, 12288], Tile1: [12288, 12288, 12288], Tile2; [24576, 12288, 12288]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 17 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 17:[512x1, 15:512x1, 512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Bias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: MulBias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 512 [Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[1x24, 20:1x24, 1x8], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 22 logical tiles, 22 physical tiles
			Total Size: 100352 [Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 22:[196x24, 20:196x24, 196x8], 1]
		Tile0: [0, 4704, 4704], Tile1: [4704, 4704, 4704], Tile2; [9408, 4704, 4704]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (512);
	KerArg0->W_Out = (unsigned short int) (196);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+34080);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (0);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+0), 6144, 196, 12, 0, &DmaR_Evt1);
	_NN_In1=12288; _SN_In1=12288;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+0), 12288, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+12288), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+12288), 12288, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 12288, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+0), 24, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+0), 24, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=4704;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<22; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==21), T1Ind_NextLast = ((T1Ind+1)==21), T1Ind_NextNextLast = ((T1Ind+2)==21);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (12288); _SNN_In1 = ((T1Ind_NextNextLast)?4096:12288); 
			}
		}
		_SN_Bias = 0;
		if (!(T1Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((T1Ind_NextLast)?8:24); 
		}
		_SN_MulBias = 0;
		if (!(T1Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((T1Ind_NextLast)?8:24); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+200704+12288*((T1Ind_Total)%2)),
					_SNN_In1, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+200704+12288*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+12288*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+24*((T1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+24*((T1Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<17; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==16), T0Ind_NextLast = ((T0Ind+1)==16);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (12); _LN_In2 = ((T0Ind_NextLast)?4:12); _SN_In2 = (512*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-192); _LN_In2 = (12); _SN_In2 = (512*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+34592+6144*((T0Ind_Total+1)%2)),
						_SN_In2, 196, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+0+12288*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?8:24);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+34592+6144*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?4:12);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+24576+24*((T1Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+24624+24*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+24672+4704*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*12);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24672+4704*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In1 = _SNN_In1;
		
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (4704); _SC_Out = ((T1Ind_NextLast)?1568:4704); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer23(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 45312 bytes, L2 buffer: 32640 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 8][Tile0 Dim: 1]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 25088 [D0, 8 x 3136][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 3136][Tile0, 1:[7x7], 1]
		Tile0: [0, 3136, 49], Tile1: [3136, 3136, 49], Tile2; [6272, 3136, 49]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 512 [D0, 8 x 64]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 64]
		Tile0: [0, 64, 64], Tile1: [64, 64, 64], Tile2; [128, 64, 64]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 512 [D0, 8 x 64]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 64]
		Tile0: [0, 64, 64], Tile1: [64, 64, 64], Tile2; [128, 64, 64]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 4608 [D0, 8 x 576]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 576]
		Tile0: [0, 576, 576], Tile1: [576, 576, 576], Tile2; [1152, 576, 576]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 100352 [D0, 8 x 12544][Tile0, 1:[14x14], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 12544][Tile0, 1:[14x14], 1]
		Tile0: [0, 12544, 196], Tile1: [12544, 12544, 196], Tile2; [25088, 12544, 196]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 8 logical tiles, 1 physical tiles
			Total Size: 100352 [D0, 8 x 12544][Tile0, 1:[7x7], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 8 x 12544][Tile0, 1:[7x7], 4]
		Tile0: [0, 100352, 100352], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (14);
	KerArg0->UsedW = (unsigned short int) (14);
	KerArg0->InFeatures = (unsigned short int) (64);
	KerArg0->OutFeatures = (unsigned short int) (64);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+32768);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Pad = (v4s) ((v4s){0,1,0,1});
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+32768);
	KerArg1->W = (unsigned short int) (7);
	KerArg1->H = (unsigned short int) (7);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->InFeatures = (unsigned short int) (64);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3136; _LC_Out=49;
	_SP_Out=0;
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25088+0), 64, 0, &DmaR_Evt1);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25216+0), 64, 0, &DmaR_Evt2);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25344+0), 576, 0, &DmaR_Evt3);
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 12544, 196, 196, 0, &DmaR_Evt4);
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<8; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==7), D0Ind_NextLast = ((D0Ind+1)==7);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (64); _SN_Bias = (64); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (64); _SN_MulBias = (64); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (576); _SN_Filter = (576); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25088+64*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25216+64*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+25344+576*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt3);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(D0Ind_Last)) {
				_N_In = _N_In + (12544); _LN_In = (196); _SN_In = (64*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+12544*((T0Ind_Total+1)%2)),
						_SN_In, 196, _LN_In, 0, &DmaR_Evt4);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+12544*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (15-0*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (15-0*(1)-1*(1));
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+25344+576*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+25088+64*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride2_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride2_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+26496+3136*((T0Ind_Total)%2));
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+25216+64*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+26496+3136*((T0Ind_Total)%2)),
					_SC_Out, 49, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (3136); _LC_Out = (49); _SC_Out = (64*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer24(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 43920 bytes, L2 buffer: 43360 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 43][Tile0 Dim: 4]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 25088 [Tile0, 4:[512x16, 2:512x16, 512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 4:[512x16, 2:512x16, 512x1], 1]
		Tile0: [0, 8192, 16], Tile1: [16, 8192, 16], Tile2; [32, 8192, 16]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 43 logical tiles, 43 physical tiles
			Total Size: 524288 [Tile1, 43:[512x24, 41:512x24, 512x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 43:[512x24, 41:512x24, 512x16], 1]
		Tile0: [0, 12288, 12288], Tile1: [12288, 12288, 12288], Tile2; [24576, 12288, 12288]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 4:[512x1, 2:512x1, 512x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 4:[512x1, 2:512x1, 512x1], 1]
		Tile0: [0, 512, 512], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Bias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 43 logical tiles, 43 physical tiles
			Total Size: 1024 [Tile1, 43:[1x24, 41:1x24, 1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 43:[1x24, 41:1x24, 1x16], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: MulBias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 43 logical tiles, 43 physical tiles
			Total Size: 1024 [Tile1, 43:[1x24, 41:1x24, 1x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 43:[1x24, 41:1x24, 1x16], 1]
		Tile0: [0, 24, 24], Tile1: [24, 24, 24], Tile2; [48, 24, 24]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 43 logical tiles, 43 physical tiles
			Total Size: 50176 [Tile1, 43:[49x24, 41:49x24, 49x16], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 43:[49x24, 41:49x24, 49x16], 1]
		Tile0: [0, 1176, 1176], Tile1: [1176, 1176, 1176], Tile2; [2352, 1176, 1176]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (512);
	KerArg0->W_Out = (unsigned short int) (49);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+27024);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (0);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27536+0), 8192, 49, 16, 0, &DmaR_Evt1);
	_NN_In1=12288; _SN_In1=12288;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+75264+0), 12288, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+12288), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+75264+12288), 12288, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+75264+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 12288, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+0), 24, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+0), 24, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=1176;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<43; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==42), T1Ind_NextLast = ((T1Ind+1)==42), T1Ind_NextNextLast = ((T1Ind+2)==42);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (12288); _SNN_In1 = ((T1Ind_NextNextLast)?8192:12288); 
			}
		}
		_SN_Bias = 0;
		if (!(T1Ind_Last)) {
			_N_Bias = _N_Bias + (24); _SN_Bias = ((T1Ind_NextLast)?16:24); 
		}
		_SN_MulBias = 0;
		if (!(T1Ind_Last)) {
			_N_MulBias = _N_MulBias + (24); _SN_MulBias = ((T1Ind_NextLast)?16:24); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+75264+12288*((T1Ind_Total)%2)),
					_SNN_In1, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+75264+12288*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+12288*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24576+24*((T1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24624+24*((T1Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<4; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==3), T0Ind_NextLast = ((T0Ind+1)==3);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (16); _LN_In2 = ((T0Ind_NextLast)?1:16); _SN_In2 = (512*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-48); _LN_In2 = (16); _SN_In2 = (512*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+27536+8192*((T0Ind_Total+1)%2)),
						_SN_In2, 49, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+0+12288*((T1Ind_Total)%2));
			KerArg0->H_In1 = (unsigned short int) (T1Ind_Last?16:24);
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+27536+8192*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?1:16);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+24576+24*((T1Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+24624+24*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+24672+1176*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*16);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+24672+1176*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In1 = _SNN_In1;
		
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (1176); _SC_Out = ((T1Ind_NextLast)?784:1176); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer25(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 46368 bytes, L2 buffer: 24192 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	KerConv_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerDP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 10][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 50176 [D0, 10 x 5488][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 10 x 5488][Tile0, 1:[7x7], 1]
		Tile0: [0, 5488, 49], Tile1: [5488, 5488, 49], Tile2; [10976, 5488, 49]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 1024 [D0, 10 x 112]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 10 x 112]
		Tile0: [0, 112, 112], Tile1: [112, 112, 112], Tile2; [224, 112, 112]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: MulBias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 1024 [D0, 10 x 112]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 10 x 112]
		Tile0: [0, 112, 112], Tile1: [112, 112, 112], Tile2; [224, 112, 112]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 9216 [D0, 10 x 1008]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 10 x 1008]
		Tile0: [0, 1008, 1008], Tile1: [1008, 1008, 1008], Tile2; [2016, 1008, 1008]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 10 physical tiles
			Total Size: 50176 [D0, 10 x 5488][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 10 x 5488][Tile0, 1:[7x7], 1]
		Tile0: [0, 5488, 49], Tile1: [5488, 5488, 49], Tile2; [10976, 5488, 49]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 10 logical tiles, 1 physical tiles
			Total Size: 200704 [D0, 10 x 21952][Tile0, 1:[7x7], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 10 x 21952][Tile0, 1:[7x7], 4]
		Tile0: [0, 200704, 200704], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (7);
	KerArg0->UsedW = (unsigned short int) (7);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+24416);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->TotalInFeatures = (short int) (NormBias);
	KerArg0->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg0->Orientation = (unsigned char) (1);
	KerArg1->In = (DP_fps_T * __restrict__) (L1_Memory+24416);
	KerArg1->W = (unsigned short int) (7);
	KerArg1->H = (unsigned short int) (7);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->NormBias = (unsigned char) (NormMulBias);
	KerArg1->LB = (int) (0);
	KerArg1->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 5488, 49, 49, 0, &DmaR_Evt1);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+10976+0), 112, 0, &DmaR_Evt2);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+11200+0), 112, 0, &DmaR_Evt3);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+11424+0), 1008, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=5488; _LC_Out=49;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<10; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==9), D0Ind_NextLast = ((D0Ind+1)==9);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (112); _SN_Bias = ((D0Ind_NextLast)?16:112); 
		}
		_SN_MulBias = 0;
		if (!(D0Ind_Last)) {
			_N_MulBias = _N_MulBias + (112); _SN_MulBias = ((D0Ind_NextLast)?16:112); 
		}
		_SN_Filter = 0;
		if (!(D0Ind_Last)) {
			_N_Filter = _N_Filter + (1008); _SN_Filter = ((D0Ind_NextLast)?144:1008); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+10976+112*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+11200+112*((D0Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+11424+1008*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(D0Ind_Last)) {
				_N_In = _N_In + (5488); _LN_In = (49); _SN_In = (((D0Ind_NextLast)?16:112)*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+5488*((T0Ind_Total+1)%2)),
						_SN_In, 49, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+5488*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (9-1*(1)-1*(1));
			KerArg0->UsedH = (unsigned short int) (9-1*(1)-1*(1));
			KerArg0->InFeatures = (unsigned short int) (D0Ind_Last?16:112);
			KerArg0->OutFeatures = (unsigned short int) (D0Ind_Last?16:112);
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+11424+1008*((D0Ind_Total)%2));
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+10976+112*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParConvDWDP3x3Stride1_fps, (void *) KerArg0);
			__CALL(KerParConvDWDP3x3Stride1_fps, KerArg0);
			KerArg1->Out = (signed char * __restrict__) (L1_Memory+13440+5488*((T0Ind_Total)%2));
			KerArg1->InFeatures = (unsigned short int) (D0Ind_Last?16:112);
			KerArg1->MulBias = (signed char * __restrict__) (L1_Memory+11200+112*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDPMulBiasScalar_fps, (void *) KerArg1);
			__CALL(KerDPMulBiasScalar_fps, KerArg1);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+13440+5488*((T0Ind_Total)%2)),
					_SC_Out, 49, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (5488); _LC_Out = (49); _SC_Out = (((D0Ind_NextLast)?16:112)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer26(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		signed char * __restrict__ Bias,
		signed char * __restrict__ MulBias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias,
		unsigned int NormMulBias)

{
	/* Shared L1: 34608 bytes, L2 buffer: 33568 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerMatMul_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast, T1Ind_NextNextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _NN_In1;
	unsigned int _SN_In1, _SNN_In1;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_MulBias;
	unsigned int _SN_MulBias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 128][Tile0 Dim: 7]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 50176 [Tile0, 7:[1024x8, 5:1024x8, 1024x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[1024x8, 5:1024x8, 1024x1], 1]
		Tile0: [0, 8192, 8], Tile1: [8, 8192, 8], Tile2; [16, 8192, 8]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: In1, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 128 logical tiles, 128 physical tiles
			Total Size: 1048576 [Tile1, 128:[1024x8, 126:1024x8, 1024x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 128:[1024x8, 126:1024x8, 1024x8], 1]
		Tile0: [0, 8192, 8192], Tile1: [8192, 8192, 8192], Tile2; [16384, 8192, 8192]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 1024 [Tile0, 7:[1024x1, 5:1024x1, 1024x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[1024x1, 5:1024x1, 1024x1], 1]
		Tile0: [0, 1024, 1024], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Bias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 128 logical tiles, 128 physical tiles
			Total Size: 1024 [Tile1, 128:[1x8, 126:1x8, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 128:[1x8, 126:1x8, 1x8], 1]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: MulBias, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 128 logical tiles, 128 physical tiles
			Total Size: 1024 [Tile1, 128:[1x8, 126:1x8, 1x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 128:[1x8, 126:1x8, 1x8], 1]
		Tile0: [0, 8, 8], Tile1: [8, 8, 8], Tile2; [16, 8, 8]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 128 logical tiles, 128 physical tiles
			Total Size: 50176 [Tile1, 128:[49x8, 126:49x8, 49x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 128:[49x8, 126:49x8, 49x8], 1]
		Tile0: [0, 392, 392], Tile1: [392, 392, 392], Tile2; [784, 392, 392]
		T0: [Tile1: 0], T1: [Tile1: 1], T2: [Tile1: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W_In1 = (unsigned short int) (1024);
	KerArg0->H_In1 = (unsigned short int) (8);
	KerArg0->W_Out = (unsigned short int) (49);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (L1_Memory+17200);
	KerArg0->LB = (int) (0);
	KerArg0->UB = (int) (127);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->NormMulBias = (unsigned char) (NormMulBias);
	KerArg0->ColFirst = (unsigned char) (0);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In2=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+18224+0), 8192, 49, 8, 0, &DmaR_Evt1);
	_NN_In1=8192; _SN_In1=8192;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+100352+0), 8192, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+8192), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+100352+8192), 8192, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+100352+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 8192, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+16384+0), 8, 0, &DmaR_Evt3);
	_N_MulBias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+16400+0), 8, 0, &DmaR_Evt4);
	_C_Out=0; _SC_Out=392;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<128; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==127), T1Ind_NextLast = ((T1Ind+1)==127), T1Ind_NextNextLast = ((T1Ind+2)==127);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In1 = 0;
		if (!(T1Ind_Last)) {
			if (!(T1Ind_NextLast)) {
				_NN_In1 = _NN_In1 + (8192); _SNN_In1 = (8192); 
			}
		}
		_SN_Bias = 0;
		if (!(T1Ind_Last)) {
			_N_Bias = _N_Bias + (8); _SN_Bias = (8); 
		}
		_SN_MulBias = 0;
		if (!(T1Ind_Last)) {
			_N_MulBias = _N_MulBias + (8); _SN_MulBias = (8); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read In1 */
		if (_SNN_In1) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In1+_NN_In1), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+100352+8192*((T1Ind_Total)%2)),
					_SNN_In1, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
		if (_SN_In1) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+100352+8192*((T1Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+8192*((T1Ind_Total+1)%2)),
					_SN_In1, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+16384+8*((T1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read MulBias */
		if (_SN_MulBias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) MulBias+_N_MulBias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+16400+8*((T1Ind_Total+1)%2)),
					_SN_MulBias, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<7; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==6), T0Ind_NextLast = ((T0Ind+1)==6);
			/*================================= Prepare Tiles ===================================*/
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (8); _LN_In2 = ((T0Ind_NextLast)?1:8); _SN_In2 = (1024*_LN_In2); 
			} else if (!(T1Ind_Last)) {
				_N_In2 = _N_In2 + (-48); _LN_In2 = (8); _SN_In2 = (1024*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) L1_Memory+18224+8192*((T0Ind_Total+1)%2)),
						_SN_In2, 49, _LN_In2, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char * __restrict__) (L1_Memory+0+8192*((T1Ind_Total)%2));
			KerArg0->In2 = (signed char * __restrict__) (L1_Memory+18224+8192*((T0Ind_Total)%2));
			KerArg0->W_In2 = (unsigned short int) ((T0Ind_Last)?1:8);
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+16384+8*((T1Ind_Total)%2));
			KerArg0->MulBias = (signed char * __restrict__) (L1_Memory+16400+8*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+16416+392*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((T0Ind)*8);
			AT_FORK(gap_ncore(), (void *) KerParMatMulScale_fps, (void *) KerArg0);
			__CALL(KerParMatMulScale_fps, KerArg0);
			/*================================= Update Arg Pipeline =============================*/
			
			/*============================= End Update Arg Pipeline =============================*/
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+16416+392*((T1Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_In1 = _SNN_In1;
		
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (392); _SC_Out = (392); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer27(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 48800 bytes, L2 buffer: 48800 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerReLUPool_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 3][Tile0 Dim: 1]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 1024 [D0, 3 x 488][Tile0, 1:[1x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 3 x 488][Tile0, 1:[1x1], 1]
		Tile0: [0, 488, 1], Tile1: [488, 488, 1], Tile2; [976, 48, 1]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 50176 [D0, 3 x 23912][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 3 x 23912][Tile0, 1:[7x7], 1]
		Tile0: [0, 23912, 49], Tile1: [23912, 23912, 49], Tile2; [47824, 2352, 49]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (7);
	KerArg0->UsedW = (unsigned short int) (7);
	KerArg0->H = (unsigned short int) (7);
	KerArg0->UsedH = (unsigned short int) (7);
	KerArg0->Pad = (v4s) 0;
	KerArg0->M = (unsigned char) (7);
	KerArg0->S = (unsigned char) (7);
	KerArg0->Orientation = (unsigned char) (1);
	KerArg0->Oper = (unsigned char) (2);
	KerArg0->LB = (int) (-128);
	KerArg0->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=488; _LC_Out=1;
	_SP_Out=0;
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 23912, 49, 49, 0, &DmaR_Evt1);
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<3; D0Ind++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==2), D0Ind_NextLast = ((D0Ind+1)==2);
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*================================= Prepare Tiles ===================================*/
			_SN_In = 0;
			if (!(D0Ind_Last)) {
				_N_In = _N_In + (23912); _LN_In = (49); _SN_In = (((D0Ind_NextLast)?48:488)*_LN_In); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
			if (_SN_In) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+23912*((T0Ind_Total+1)%2)),
						_SN_In, 49, _LN_In, 0, &DmaR_Evt1);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In = (signed char * __restrict__) (L1_Memory+0+23912*((T0Ind_Total)%2));
			KerArg0->OutFeatures = (unsigned short int) (D0Ind_Last?48:488);
			KerArg0->Out = (signed char * __restrict__) (L1_Memory+47824+488*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParPoolNxNStrideS_fps, (void *) KerArg0);
			__CALL(KerParPoolNxNStrideS_fps, KerArg0);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+47824+488*((T0Ind_Total)%2)),
					_SC_Out, 1, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D0Ind_Last)) {
				_C_Out = _C_Out + (488); _LC_Out = (1); _SC_Out = (((D0Ind_NextLast)?48:488)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) Layer28(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 33920 bytes, L2 buffer: 33920 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT Uchan1;
	KerLinearLayerReLU_fps_fps_fp_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast, D0Ind_NextNextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _NN_Filter;
	unsigned int _SN_Filter, _SNN_Filter;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 63][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1024 [Tile0, 1:[1x1], 1024]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 1024]
		Tile0: [0, 1024, 1024], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 63 logical tiles, 63 physical tiles
			Total Size: 1024000 [D0, 63 x 16384]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 63 x 16384]
		Tile0: [0, 16384, 16384], Tile1: [16384, 16384, 16384], Tile2; [32768, 16384, 16384]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Bias, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 63 logical tiles, 63 physical tiles
			Total Size: 2000 [D0, 63 x 32]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 63 x 32]
		Tile0: [0, 32, 32], Tile1: [32, 32, 32], Tile2; [64, 32, 32]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	Ker Arg: Out, Tiled Space: D0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 63 logical tiles, 63 physical tiles
			Total Size: 2000 [D0, 63 x 32]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 63 x 32]
		Tile0: [0, 32, 32], Tile1: [32, 32, 32], Tile2; [64, 32, 32]
		T0: [D0: 0], T1: [D0: 1], T2: [D0: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char * __restrict__) (L1_Memory+0);
	KerArg0->InSize = (unsigned short int) (1024);
	KerArg0->TotalInSize = (unsigned short int) (1024);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->LB = (int) (-32768);
	KerArg0->UB = (int) (32767);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 1024, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	_NN_Filter=16384; _SN_Filter=16384;
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+2000+0), 16384, 0, &Uchan1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read Filter */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Filter+16384), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+2000+16384), 16384, 0, &Uchan1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+2000+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1024+0), 16384, 0, &DmaR_Evt2);
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+33792+0), 32, 0, &DmaR_Evt3);
	_C_Out=0; _SC_Out=32;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (D0Ind=0; D0Ind<63; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
		int D0Ind_Last = (D0Ind==62), D0Ind_NextLast = ((D0Ind+1)==62), D0Ind_NextNextLast = ((D0Ind+2)==62);
		/*================================= Prepare Tiles ===================================*/
		_SNN_Filter = 0;
		if (!(D0Ind_Last)) {
			if (!(D0Ind_NextLast)) {
				_NN_Filter = _NN_Filter + (16384); _SNN_Filter = ((D0Ind_NextNextLast)?8192:16384); 
			}
		}
		_SN_Bias = 0;
		if (!(D0Ind_Last)) {
			_N_Bias = _N_Bias + (32); _SN_Bias = ((D0Ind_NextLast)?16:32); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1); /* Wait previous uDMA read Filter */
		if (_SNN_Filter) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Filter+_NN_Filter), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory+2000+16384*((D0Ind_Total)%2)),
					_SNN_Filter, 0, &Uchan1);
		}
		AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) L2_Memory+2000+16384*((D0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1024+16384*((D0Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt2);
		}
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+33792+32*((D0Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt3);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutSize = (unsigned short int) (D0Ind_Last?8:16);
			KerArg0->Filter = (signed char * __restrict__) (L1_Memory+1024+16384*((D0Ind_Total)%2));
			KerArg0->Bias = (short int * __restrict__) (L1_Memory+33792+32*((D0Ind_Total)%2));
			KerArg0->Out = (short int * __restrict__) (L1_Memory+33856+32*((D0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParLinearLayerReLU_fps_fps_fp, (void *) KerArg0);
			__CALL(KerParLinearLayerReLU_fps_fps_fp, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+33856+32*((D0Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SN_Filter = _SNN_Filter;
		
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(D0Ind_Last)) {
			_C_Out = _C_Out + (32); _SC_Out = ((D0Ind_NextLast)?16:32); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
int MobileNetCNN_Construct()

{
	AT_HYPERFLASH_FS_FC_EVENT Uchan1;
	AT_HYPERRAM_FC_EVENT Uchan2;
	AT_HYPERFLASH_FS_CONF_T HyperFlashConf;
	int Error;
	AT_HYPERFLASH_FS_CONF_INIT(&HyperFlashConf, AT_MEM_L3_HFLASH, 0);
	AT_HYPERFLASH_FS_OPEN(&HyperFlash, &HyperFlashConf, "MN_L3_Flash_Const.bin", &Error);
	if (Error) return 1;
	L3_Memory = (AT_HYPERRAM_POINTER) AT_HYPERRAM_ALLOC(&HyperRam, 5423056);
	L2_Memory = (AT_L2_POINTER) AT_L2_ALLOC(0, 315200);
	L1_Memory = (AT_L1_POINTER) AT_L1_ALLOC(0, 49008);
	/* Moving FL4, size 8192 from HyperFlash at 4162560 to (size 8192) HyperRam at 5366784 */
	{
		int Size = 8192, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4162560+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5366784+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL6, size 16384 from HyperFlash at 4136960 to (size 16384) HyperRam at 5341184 */
	{
		int Size = 16384, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4136960+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5341184+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL8, size 32768 from HyperFlash at 4104192 to (size 32768) HyperRam at 5308416 */
	{
		int Size = 32768, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4104192+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5308416+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL10, size 65536 from HyperFlash at 4038656 to (size 65536) HyperRam at 5242880 */
	{
		int Size = 65536, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4038656+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5242880+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL12, size 131072 from HyperFlash at 3907584 to (size 131072) HyperRam at 5111808 */
	{
		int Size = 131072, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 3907584+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5111808+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL12, size 512 from HyperFlash at 4216944 to (size 512) HyperRam at 5410768 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4216944+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5410768+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML12, size 512 from HyperFlash at 4217456 to (size 512) HyperRam at 5411280 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4217456+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5411280+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL13, size 4608 from HyperFlash at 4170752 to (size 4608) HyperRam at 5374976 */
	{
		int Size = 4608, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4170752+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5374976+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL13, size 512 from HyperFlash at 4217968 to (size 512) HyperRam at 5411792 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4217968+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5411792+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML13, size 512 from HyperFlash at 4218480 to (size 512) HyperRam at 5412304 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4218480+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5412304+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL14, size 262144 from HyperFlash at 0 to (size 262144) HyperRam at 1204224 */
	{
		int Size = 262144, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 0+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 1204224+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL14, size 512 from HyperFlash at 4218992 to (size 512) HyperRam at 5412816 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4218992+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5412816+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML14, size 512 from HyperFlash at 4219504 to (size 512) HyperRam at 5413328 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4219504+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5413328+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL15, size 4608 from HyperFlash at 4175360 to (size 4608) HyperRam at 5379584 */
	{
		int Size = 4608, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4175360+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5379584+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL15, size 512 from HyperFlash at 4220016 to (size 512) HyperRam at 5413840 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4220016+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5413840+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML15, size 512 from HyperFlash at 4220528 to (size 512) HyperRam at 5414352 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4220528+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5414352+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL16, size 262144 from HyperFlash at 262144 to (size 262144) HyperRam at 1466368 */
	{
		int Size = 262144, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 262144+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 1466368+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL16, size 512 from HyperFlash at 4221040 to (size 512) HyperRam at 5414864 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4221040+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5414864+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML16, size 512 from HyperFlash at 4221552 to (size 512) HyperRam at 5415376 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4221552+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5415376+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL17, size 4608 from HyperFlash at 4179968 to (size 4608) HyperRam at 5384192 */
	{
		int Size = 4608, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4179968+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5384192+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL17, size 512 from HyperFlash at 4222064 to (size 512) HyperRam at 5415888 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4222064+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5415888+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML17, size 512 from HyperFlash at 4222576 to (size 512) HyperRam at 5416400 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4222576+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5416400+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL18, size 262144 from HyperFlash at 524288 to (size 262144) HyperRam at 1728512 */
	{
		int Size = 262144, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 524288+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 1728512+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL18, size 512 from HyperFlash at 4223088 to (size 512) HyperRam at 5416912 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4223088+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5416912+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML18, size 512 from HyperFlash at 4223600 to (size 512) HyperRam at 5417424 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4223600+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5417424+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL19, size 4608 from HyperFlash at 4184576 to (size 4608) HyperRam at 5388800 */
	{
		int Size = 4608, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4184576+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5388800+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL19, size 512 from HyperFlash at 4224112 to (size 512) HyperRam at 5417936 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4224112+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5417936+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML19, size 512 from HyperFlash at 4224624 to (size 512) HyperRam at 5418448 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4224624+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5418448+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL20, size 262144 from HyperFlash at 786432 to (size 262144) HyperRam at 1990656 */
	{
		int Size = 262144, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 786432+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 1990656+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL20, size 512 from HyperFlash at 4225136 to (size 512) HyperRam at 5418960 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4225136+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5418960+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML20, size 512 from HyperFlash at 4225648 to (size 512) HyperRam at 5419472 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4225648+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5419472+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL21, size 4608 from HyperFlash at 4189184 to (size 4608) HyperRam at 5393408 */
	{
		int Size = 4608, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4189184+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5393408+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL21, size 512 from HyperFlash at 4226160 to (size 512) HyperRam at 5419984 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4226160+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5419984+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML21, size 512 from HyperFlash at 4226672 to (size 512) HyperRam at 5420496 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4226672+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5420496+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL22, size 262144 from HyperFlash at 1048576 to (size 262144) HyperRam at 2252800 */
	{
		int Size = 262144, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 1048576+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 2252800+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL22, size 512 from HyperFlash at 4227184 to (size 512) HyperRam at 5421008 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4227184+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5421008+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML22, size 512 from HyperFlash at 4227696 to (size 512) HyperRam at 5421520 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4227696+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5421520+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL23, size 4608 from HyperFlash at 4193792 to (size 4608) HyperRam at 5398016 */
	{
		int Size = 4608, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4193792+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5398016+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL23, size 512 from HyperFlash at 4228208 to (size 512) HyperRam at 5422032 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4228208+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5422032+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML23, size 512 from HyperFlash at 4228720 to (size 512) HyperRam at 5422544 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4228720+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5422544+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL24, size 524288 from HyperFlash at 1310720 to (size 524288) HyperRam at 2514944 */
	{
		int Size = 524288, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 1310720+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 2514944+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL24, size 1024 from HyperFlash at 4209360 to (size 1024) HyperRam at 5404624 */
	{
		int Size = 1024, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4209360+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5404624+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML24, size 1024 from HyperFlash at 4210384 to (size 1024) HyperRam at 5405648 */
	{
		int Size = 1024, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4210384+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5405648+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL25, size 9216 from HyperFlash at 4153344 to (size 9216) HyperRam at 5357568 */
	{
		int Size = 9216, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4153344+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5357568+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL25, size 1024 from HyperFlash at 4211408 to (size 1024) HyperRam at 5406672 */
	{
		int Size = 1024, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4211408+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5406672+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML25, size 1024 from HyperFlash at 4212432 to (size 1024) HyperRam at 5407696 */
	{
		int Size = 1024, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4212432+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5407696+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL26, size 1048576 from HyperFlash at 1835008 to (size 1048576) HyperRam at 3039232 */
	{
		int Size = 1048576, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 1835008+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 3039232+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL26, size 1024 from HyperFlash at 4213456 to (size 1024) HyperRam at 5408720 */
	{
		int Size = 1024, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4213456+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5408720+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving ML26, size 1024 from HyperFlash at 4214480 to (size 1024) HyperRam at 5409744 */
	{
		int Size = 1024, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4214480+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5409744+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL28, size 1024000 from HyperFlash at 2883584 to (size 1024000) HyperRam at 4087808 */
	{
		int Size = 1024000, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 2883584+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 4087808+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving BL28, size 2000 from HyperFlash at 4205056 to (size 2000) HyperRam at 5402624 */
	{
		int Size = 2000, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4205056+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 0, &Uchan1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5402624+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), Chunk, 1, &Uchan2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &Uchan2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving FL0, size 864 from HyperFlash at 4215504 to (size 864) L2 at 310016 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4215504), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 310016), 864, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL0, size 32 from HyperFlash at 4232848 to (size 32) L2 at 315072 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232848), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 315072), 32, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML0, size 32 from HyperFlash at 4232880 to (size 32) L2 at 315104 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232880), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 315104), 32, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving FL1, size 288 from HyperFlash at 4229232 to (size 288) L2 at 311456 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4229232), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 311456), 288, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL1, size 32 from HyperFlash at 4232912 to (size 32) L2 at 315136 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232912), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 315136), 32, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML1, size 32 from HyperFlash at 4232944 to (size 32) L2 at 315168 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232944), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 315168), 32, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving FL2, size 2048 from HyperFlash at 4203008 to (size 2048) L2 at 305664 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4203008), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 305664), 2048, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL2, size 64 from HyperFlash at 4232592 to (size 64) L2 at 314816 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232592), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314816), 64, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML2, size 64 from HyperFlash at 4232656 to (size 64) L2 at 314880 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232656), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314880), 64, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving FL3, size 576 from HyperFlash at 4216368 to (size 576) L2 at 310880 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4216368), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 310880), 576, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL3, size 64 from HyperFlash at 4232720 to (size 64) L2 at 314944 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232720), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314944), 64, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML3, size 64 from HyperFlash at 4232784 to (size 64) L2 at 315008 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232784), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 315008), 64, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL4, size 128 from HyperFlash at 4231568 to (size 128) L2 at 313792 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4231568), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 313792), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML4, size 128 from HyperFlash at 4231696 to (size 128) L2 at 313920 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4231696), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 313920), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving FL5, size 1152 from HyperFlash at 4207056 to (size 1152) L2 at 307712 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4207056), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 307712), 1152, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL5, size 128 from HyperFlash at 4231824 to (size 128) L2 at 314048 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4231824), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314048), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML5, size 128 from HyperFlash at 4231952 to (size 128) L2 at 314176 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4231952), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314176), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL6, size 128 from HyperFlash at 4232080 to (size 128) L2 at 314304 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232080), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314304), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML6, size 128 from HyperFlash at 4232208 to (size 128) L2 at 314432 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232208), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314432), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving FL7, size 1152 from HyperFlash at 4208208 to (size 1152) L2 at 308864 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4208208), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 308864), 1152, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL7, size 128 from HyperFlash at 4232336 to (size 128) L2 at 314560 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232336), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314560), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML7, size 128 from HyperFlash at 4232464 to (size 128) L2 at 314688 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4232464), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 314688), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL8, size 256 from HyperFlash at 4229520 to (size 256) L2 at 311744 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4229520), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 311744), 256, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML8, size 256 from HyperFlash at 4229776 to (size 256) L2 at 312000 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4229776), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 312000), 256, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving FL9, size 2304 from HyperFlash at 4198400 to (size 2304) L2 at 301056 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4198400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 301056), 2304, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL9, size 256 from HyperFlash at 4230032 to (size 256) L2 at 312256 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4230032), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 312256), 256, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML9, size 256 from HyperFlash at 4230288 to (size 256) L2 at 312512 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4230288), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 312512), 256, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL10, size 256 from HyperFlash at 4230544 to (size 256) L2 at 312768 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4230544), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 312768), 256, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML10, size 256 from HyperFlash at 4230800 to (size 256) L2 at 313024 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4230800), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 313024), 256, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving FL11, size 2304 from HyperFlash at 4200704 to (size 2304) L2 at 303360 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4200704), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 303360), 2304, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving BL11, size 256 from HyperFlash at 4231056 to (size 256) L2 at 313280 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4231056), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 313280), 256, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving ML11, size 256 from HyperFlash at 4231312 to (size 256) L2 at 313536 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) 0 + 4231312), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) L2_Memory + 313536), 256, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	return 0;
}
int MobileNetCNN_Destruct()

{
	AT_HYPERRAM_FREE(&HyperRam, L3_Memory, 5423056);
	AT_L2_FREE(0, L2_Memory, 315200);
	AT_L1_FREE(0, L1_Memory, 49008);
	AT_HYPERFLASH_FS_CLOSE(&HyperFlash);
	return 0;
}
unsigned int MNPerf[29];
unsigned int MNOperCount[29] = {
	11239424,
	4014080,
	25690112,
	2007040,
	25690112,
	4014080,
	51380224,
	1003520,
	25690112,
	2007040,
	51380224,
	501760,
	25690112,
	1003520,
	51380224,
	1003520,
	51380224,
	1003520,
	51380224,
	1003520,
	51380224,
	1003520,
	51380224,
	250880,
	25690112,
	501760,
	51380224,
	50176,
	1024000,
};
char *MNLName[29] = {
	"Layer0",
	"Layer1",
	"Layer2",
	"Layer3",
	"Layer4",
	"Layer5",
	"Layer6",
	"Layer7",
	"Layer8",
	"Layer9",
	"Layer10",
	"Layer11",
	"Layer12",
	"Layer13",
	"Layer14",
	"Layer15",
	"Layer16",
	"Layer17",
	"Layer18",
	"Layer19",
	"Layer20",
	"Layer21",
	"Layer22",
	"Layer23",
	"Layer24",
	"Layer25",
	"Layer26",
	"Layer27",
	"Layer28",
};
short int *__restrict Out;
int MobileNetCNN(
		signed char *__restrict In)

{
	AT_HYPERRAM_CL_EVENT Uchan0;
	AT_HYPERRAM_CL_EVENT Uchan1;
	AT_HYPERRAM_CL_EVENT Uchan2;
	AT_HYPERRAM_CL_EVENT Uchan3;
	AT_HYPERRAM_CL_EVENT Uchan4;
	MNPerf[0] = gap_cl_readhwtimer();
	Layer0(
		(signed char *__restrict) In, /* In */
		(signed char *__restrict) (L2_Memory+310016), /* Filter */
		(signed char * __restrict__) (L2_Memory+315072), /* Bias */
		(signed char *__restrict) (L2_Memory+315104), /* MulBias */
		(signed char *__restrict) (L3_Memory+0), /* Out */
		9, /* Norm */
		11, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[0] = gap_cl_readhwtimer() - MNPerf[0];
	MNPerf[1] = gap_cl_readhwtimer();
	Layer1(
		(signed char *__restrict) (L3_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+311456), /* Filter */
		(signed char * __restrict__) (L2_Memory+315136), /* Bias */
		(signed char *__restrict) (L2_Memory+315168), /* MulBias */
		(signed char *__restrict) (L3_Memory+802816), /* Out */
		2, /* Norm */
		1, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[1] = gap_cl_readhwtimer() - MNPerf[1];
	MNPerf[2] = gap_cl_readhwtimer();
	Layer2(
		(signed char *__restrict) (L3_Memory+802816), /* In2 */
		(signed char *__restrict) (L2_Memory+305664), /* In1 */
		(signed char * __restrict__) (L2_Memory+314816), /* Bias */
		(signed char *__restrict) (L2_Memory+314880), /* MulBias */
		(signed char *__restrict) (L3_Memory+0), /* Out */
		6, /* Norm */
		5, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[2] = gap_cl_readhwtimer() - MNPerf[2];
	/* Moving FL4, size 8192 from HyperRam at 5366784 to (size 8192) L2 at 227584 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5366784), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 227584), 8192, 0, &Uchan0);
	MNPerf[3] = gap_cl_readhwtimer();
	Layer3(
		(signed char *__restrict) (L3_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+310880), /* Filter */
		(signed char * __restrict__) (L2_Memory+314944), /* Bias */
		(signed char *__restrict) (L2_Memory+315008), /* MulBias */
		(signed char *__restrict) (L2_Memory+26880), /* Out */
		1, /* Norm */
		0, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[3] = gap_cl_readhwtimer() - MNPerf[3];
	/* Waiting completion of transfer of FL4 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	MNPerf[4] = gap_cl_readhwtimer();
	Layer4(
		(signed char *__restrict) (L2_Memory+26880), /* In2 */
		(signed char *__restrict) (L2_Memory+227584), /* In1 */
		(signed char * __restrict__) (L2_Memory+313792), /* Bias */
		(signed char *__restrict) (L2_Memory+313920), /* MulBias */
		(signed char *__restrict) (L3_Memory+0), /* Out */
		6, /* Norm */
		5, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[4] = gap_cl_readhwtimer() - MNPerf[4];
	/* Moving FL6, size 16384 from HyperRam at 5341184 to (size 16384) L2 at 25088 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5341184), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 25088), 16384, 0, &Uchan0);
	MNPerf[5] = gap_cl_readhwtimer();
	Layer5(
		(signed char *__restrict) (L3_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+307712), /* Filter */
		(signed char * __restrict__) (L2_Memory+314048), /* Bias */
		(signed char *__restrict) (L2_Memory+314176), /* MulBias */
		(signed char *__restrict) (L3_Memory+401408), /* Out */
		4, /* Norm */
		2, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[5] = gap_cl_readhwtimer() - MNPerf[5];
	/* Waiting completion of transfer of FL6 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	MNPerf[6] = gap_cl_readhwtimer();
	Layer6(
		(signed char *__restrict) (L3_Memory+401408), /* In2 */
		(signed char *__restrict) (L2_Memory+25088), /* In1 */
		(signed char * __restrict__) (L2_Memory+314304), /* Bias */
		(signed char *__restrict) (L2_Memory+314432), /* MulBias */
		(signed char *__restrict) (L3_Memory+0), /* Out */
		6, /* Norm */
		5, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[6] = gap_cl_readhwtimer() - MNPerf[6];
	/* Moving FL8, size 32768 from HyperRam at 5308416 to (size 32768) L2 at 128128 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5308416), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 128128), 32768, 0, &Uchan0);
	MNPerf[7] = gap_cl_readhwtimer();
	Layer7(
		(signed char *__restrict) (L3_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+308864), /* Filter */
		(signed char * __restrict__) (L2_Memory+314560), /* Bias */
		(signed char *__restrict) (L2_Memory+314688), /* MulBias */
		(signed char *__restrict) (L2_Memory+27776), /* Out */
		6, /* Norm */
		5, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[7] = gap_cl_readhwtimer() - MNPerf[7];
	/* Waiting completion of transfer of FL8 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	MNPerf[8] = gap_cl_readhwtimer();
	Layer8(
		(signed char *__restrict) (L2_Memory+27776), /* In2 */
		(signed char *__restrict) (L2_Memory+128128), /* In1 */
		(signed char * __restrict__) (L2_Memory+311744), /* Bias */
		(signed char *__restrict) (L2_Memory+312000), /* MulBias */
		(signed char *__restrict) (L3_Memory+0), /* Out */
		6, /* Norm */
		5, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[8] = gap_cl_readhwtimer() - MNPerf[8];
	/* Moving FL10, size 65536 from HyperRam at 5242880 to (size 65536) L2 at 213248 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5242880), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 213248), 65536, 0, &Uchan0);
	MNPerf[9] = gap_cl_readhwtimer();
	Layer9(
		(signed char *__restrict) (L3_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+301056), /* Filter */
		(signed char * __restrict__) (L2_Memory+312256), /* Bias */
		(signed char *__restrict) (L2_Memory+312512), /* MulBias */
		(signed char *__restrict) (L2_Memory+0), /* Out */
		5, /* Norm */
		4, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[9] = gap_cl_readhwtimer() - MNPerf[9];
	/* Waiting completion of transfer of FL10 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	MNPerf[10] = gap_cl_readhwtimer();
	Layer10(
		(signed char *__restrict) (L2_Memory+0), /* In2 */
		(signed char *__restrict) (L2_Memory+213248), /* In1 */
		(signed char * __restrict__) (L2_Memory+312768), /* Bias */
		(signed char *__restrict) (L2_Memory+313024), /* MulBias */
		(signed char *__restrict) (L3_Memory+0), /* Out */
		6, /* Norm */
		5, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[10] = gap_cl_readhwtimer() - MNPerf[10];
	/* Moving FL12, size 131072 from HyperRam at 5111808 to (size 131072) L2 at 150528 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5111808), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 150528), 131072, 0, &Uchan0);
	MNPerf[11] = gap_cl_readhwtimer();
	Layer11(
		(signed char *__restrict) (L3_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+303360), /* Filter */
		(signed char * __restrict__) (L2_Memory+313280), /* Bias */
		(signed char *__restrict) (L2_Memory+313536), /* MulBias */
		(signed char *__restrict) (L2_Memory+0), /* Out */
		7, /* Norm */
		7, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[11] = gap_cl_readhwtimer() - MNPerf[11];
	/* Moving BL12, size 512 from HyperRam at 5410768 to (size 512) L2 at 286208 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5410768), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 286208), 512, 0, &Uchan1);
	/* Moving ML12, size 512 from HyperRam at 5411280 to (size 512) L2 at 286720 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5411280), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 286720), 512, 0, &Uchan2);
	/* Moving FL13, size 4608 from HyperRam at 5374976 to (size 4608) L2 at 281600 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5374976), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 281600), 4608, 0, &Uchan3);
	/* Waiting completion of transfer of FL12 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	/* Waiting completion of transfer of BL12 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1);
	/* Waiting completion of transfer of ML12 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	MNPerf[12] = gap_cl_readhwtimer();
	Layer12(
		(signed char *__restrict) (L2_Memory+0), /* In2 */
		(signed char *__restrict) (L2_Memory+150528), /* In1 */
		(signed char * __restrict__) (L2_Memory+286208), /* Bias */
		(signed char *__restrict) (L2_Memory+286720), /* MulBias */
		(signed char *__restrict) (L2_Memory+50176), /* Out */
		7, /* Norm */
		7, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[12] = gap_cl_readhwtimer() - MNPerf[12];
	/* Moving BL13, size 512 from HyperRam at 5411792 to (size 512) L2 at 0 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5411792), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 0), 512, 0, &Uchan1);
	/* Moving ML13, size 512 from HyperRam at 5412304 to (size 512) L2 at 512 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5412304), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 512), 512, 0, &Uchan2);
	/* Waiting completion of transfer of FL13 using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan3);
	/* Waiting completion of transfer of BL13 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1);
	/* Waiting completion of transfer of ML13 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	MNPerf[13] = gap_cl_readhwtimer();
	Layer13(
		(signed char *__restrict) (L2_Memory+50176), /* In */
		(signed char *__restrict) (L2_Memory+281600), /* Filter */
		(signed char * __restrict__) (L2_Memory+0), /* Bias */
		(signed char *__restrict) (L2_Memory+512), /* MulBias */
		(signed char *__restrict) (L2_Memory+150528), /* Out */
		5, /* Norm */
		3, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[13] = gap_cl_readhwtimer() - MNPerf[13];
	/* Moving BL14, size 512 from HyperRam at 5412816 to (size 512) L2 at 124928 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5412816), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 124928), 512, 0, &Uchan0);
	/* Moving ML14, size 512 from HyperRam at 5413328 to (size 512) L2 at 125440 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5413328), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 125440), 512, 0, &Uchan1);
	/* Moving FL15, size 4608 from HyperRam at 5379584 to (size 4608) L2 at 250880 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5379584), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 250880), 4608, 0, &Uchan2);
	/* Waiting completion of transfer of BL14 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	/* Waiting completion of transfer of ML14 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1);
	MNPerf[14] = gap_cl_readhwtimer();
	Layer14(
		(signed char *__restrict) (L2_Memory+150528), /* In2 */
		(signed char *__restrict) (L3_Memory+1204224), /* In1 */
		(signed char * __restrict__) (L2_Memory+124928), /* Bias */
		(signed char *__restrict) (L2_Memory+125440), /* MulBias */
		(signed char *__restrict) (L2_Memory+0), /* Out */
		7, /* Norm */
		7, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[14] = gap_cl_readhwtimer() - MNPerf[14];
	/* Moving BL15, size 512 from HyperRam at 5413840 to (size 512) L2 at 200704 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5413840), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 200704), 512, 0, &Uchan1);
	/* Moving ML15, size 512 from HyperRam at 5414352 to (size 512) L2 at 201216 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5414352), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 201216), 512, 0, &Uchan2);
	/* Waiting completion of transfer of FL15 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	/* Waiting completion of transfer of BL15 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1);
	/* Waiting completion of transfer of ML15 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	MNPerf[15] = gap_cl_readhwtimer();
	Layer15(
		(signed char *__restrict) (L2_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+250880), /* Filter */
		(signed char * __restrict__) (L2_Memory+200704), /* Bias */
		(signed char *__restrict) (L2_Memory+201216), /* MulBias */
		(signed char *__restrict) (L2_Memory+100352), /* Out */
		5, /* Norm */
		3, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[15] = gap_cl_readhwtimer() - MNPerf[15];
	/* Moving BL16, size 512 from HyperRam at 5414864 to (size 512) L2 at 229888 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5414864), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 229888), 512, 0, &Uchan0);
	/* Moving ML16, size 512 from HyperRam at 5415376 to (size 512) L2 at 230400 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5415376), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 230400), 512, 0, &Uchan1);
	/* Moving FL17, size 4608 from HyperRam at 5384192 to (size 4608) L2 at 225280 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5384192), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 225280), 4608, 0, &Uchan2);
	/* Moving BL17, size 512 from HyperRam at 5415888 to (size 512) L2 at 230912 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5415888), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 230912), 512, 0, &Uchan3);
	/* Moving ML17, size 512 from HyperRam at 5416400 to (size 512) L2 at 231424 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5416400), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 231424), 512, 0, &Uchan4);
	/* Waiting completion of transfer of BL16 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	/* Waiting completion of transfer of ML16 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1);
	MNPerf[16] = gap_cl_readhwtimer();
	Layer16(
		(signed char *__restrict) (L2_Memory+100352), /* In2 */
		(signed char *__restrict) (L3_Memory+1466368), /* In1 */
		(signed char * __restrict__) (L2_Memory+229888), /* Bias */
		(signed char *__restrict) (L2_Memory+230400), /* MulBias */
		(signed char *__restrict) (L2_Memory+0), /* Out */
		7, /* Norm */
		7, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[16] = gap_cl_readhwtimer() - MNPerf[16];
	/* Waiting completion of transfer of FL17 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	/* Waiting completion of transfer of BL17 using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan3);
	/* Waiting completion of transfer of ML17 using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan4);
	MNPerf[17] = gap_cl_readhwtimer();
	Layer17(
		(signed char *__restrict) (L2_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+225280), /* Filter */
		(signed char * __restrict__) (L2_Memory+230912), /* Bias */
		(signed char *__restrict) (L2_Memory+231424), /* MulBias */
		(signed char *__restrict) (L2_Memory+100352), /* Out */
		5, /* Norm */
		4, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[17] = gap_cl_readhwtimer() - MNPerf[17];
	/* Moving BL18, size 512 from HyperRam at 5416912 to (size 512) L2 at 24576 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5416912), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 24576), 512, 0, &Uchan0);
	/* Moving ML18, size 512 from HyperRam at 5417424 to (size 512) L2 at 25088 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5417424), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 25088), 512, 0, &Uchan1);
	/* Waiting completion of transfer of BL18 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	/* Waiting completion of transfer of ML18 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1);
	MNPerf[18] = gap_cl_readhwtimer();
	Layer18(
		(signed char *__restrict) (L2_Memory+100352), /* In2 */
		(signed char *__restrict) (L3_Memory+1728512), /* In1 */
		(signed char * __restrict__) (L2_Memory+24576), /* Bias */
		(signed char *__restrict) (L2_Memory+25088), /* MulBias */
		(signed char *__restrict) (L2_Memory+200704), /* Out */
		7, /* Norm */
		7, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[18] = gap_cl_readhwtimer() - MNPerf[18];
	/* Moving FL19, size 4608 from HyperRam at 5388800 to (size 4608) L2 at 100352 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5388800), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 100352), 4608, 0, &Uchan0);
	/* Moving BL19, size 512 from HyperRam at 5417936 to (size 512) L2 at 104960 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5417936), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 104960), 512, 0, &Uchan1);
	/* Moving ML19, size 512 from HyperRam at 5418448 to (size 512) L2 at 105472 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5418448), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 105472), 512, 0, &Uchan2);
	/* Waiting completion of transfer of FL19 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	/* Waiting completion of transfer of BL19 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1);
	/* Waiting completion of transfer of ML19 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	MNPerf[19] = gap_cl_readhwtimer();
	Layer19(
		(signed char *__restrict) (L2_Memory+200704), /* In */
		(signed char *__restrict) (L2_Memory+100352), /* Filter */
		(signed char * __restrict__) (L2_Memory+104960), /* Bias */
		(signed char *__restrict) (L2_Memory+105472), /* MulBias */
		(signed char *__restrict) (L2_Memory+0), /* Out */
		5, /* Norm */
		4, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[19] = gap_cl_readhwtimer() - MNPerf[19];
	/* Moving BL20, size 512 from HyperRam at 5418960 to (size 512) L2 at 229888 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5418960), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 229888), 512, 0, &Uchan0);
	/* Moving ML20, size 512 from HyperRam at 5419472 to (size 512) L2 at 230400 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5419472), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 230400), 512, 0, &Uchan1);
	/* Moving FL21, size 4608 from HyperRam at 5393408 to (size 4608) L2 at 225280 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5393408), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 225280), 4608, 0, &Uchan2);
	/* Moving BL21, size 512 from HyperRam at 5419984 to (size 512) L2 at 230912 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5419984), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 230912), 512, 0, &Uchan3);
	/* Moving ML21, size 512 from HyperRam at 5420496 to (size 512) L2 at 231424 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5420496), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 231424), 512, 0, &Uchan4);
	/* Waiting completion of transfer of BL20 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	/* Waiting completion of transfer of ML20 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan1);
	MNPerf[20] = gap_cl_readhwtimer();
	Layer20(
		(signed char *__restrict) (L2_Memory+0), /* In2 */
		(signed char *__restrict) (L3_Memory+1990656), /* In1 */
		(signed char * __restrict__) (L2_Memory+229888), /* Bias */
		(signed char *__restrict) (L2_Memory+230400), /* MulBias */
		(signed char *__restrict) (L2_Memory+100352), /* Out */
		6, /* Norm */
		5, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[20] = gap_cl_readhwtimer() - MNPerf[20];
	/* Moving BL22, size 512 from HyperRam at 5421008 to (size 512) L2 at 229888 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5421008), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 229888), 512, 0, &Uchan3);
	/* Moving ML22, size 512 from HyperRam at 5421520 to (size 512) L2 at 230400 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5421520), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 230400), 512, 0, &Uchan4);
	/* Waiting completion of transfer of FL21 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	/* Waiting completion of transfer of BL21 using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan3);
	/* Waiting completion of transfer of ML21 using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan4);
	MNPerf[21] = gap_cl_readhwtimer();
	Layer21(
		(signed char *__restrict) (L2_Memory+100352), /* In */
		(signed char *__restrict) (L2_Memory+225280), /* Filter */
		(signed char * __restrict__) (L2_Memory+230912), /* Bias */
		(signed char *__restrict) (L2_Memory+231424), /* MulBias */
		(signed char *__restrict) (L2_Memory+0), /* Out */
		6, /* Norm */
		5, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[21] = gap_cl_readhwtimer() - MNPerf[21];
	/* Moving FL23, size 4608 from HyperRam at 5398016 to (size 4608) L2 at 225280 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5398016), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 225280), 4608, 0, &Uchan2);
	/* Moving BL23, size 512 from HyperRam at 5422032 to (size 512) L2 at 230912 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5422032), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 230912), 512, 0, &Uchan3);
	/* Moving ML23, size 512 from HyperRam at 5422544 to (size 512) L2 at 231424 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5422544), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 231424), 512, 0, &Uchan4);
	/* Waiting completion of transfer of BL22 using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan3);
	/* Waiting completion of transfer of ML22 using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan4);
	MNPerf[22] = gap_cl_readhwtimer();
	Layer22(
		(signed char *__restrict) (L2_Memory+0), /* In2 */
		(signed char *__restrict) (L3_Memory+2252800), /* In1 */
		(signed char * __restrict__) (L2_Memory+229888), /* Bias */
		(signed char *__restrict) (L2_Memory+230400), /* MulBias */
		(signed char *__restrict) (L2_Memory+100352), /* Out */
		8, /* Norm */
		9, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[22] = gap_cl_readhwtimer() - MNPerf[22];
	/* Moving BL24, size 1024 from HyperRam at 5404624 to (size 1024) L2 at 200704 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5404624), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 200704), 1024, 0, &Uchan3);
	/* Moving ML24, size 1024 from HyperRam at 5405648 to (size 1024) L2 at 201728 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5405648), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 201728), 1024, 0, &Uchan4);
	/* Waiting completion of transfer of FL23 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	/* Waiting completion of transfer of BL23 using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan3);
	/* Waiting completion of transfer of ML23 using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan4);
	MNPerf[23] = gap_cl_readhwtimer();
	Layer23(
		(signed char *__restrict) (L2_Memory+100352), /* In */
		(signed char *__restrict) (L2_Memory+225280), /* Filter */
		(signed char * __restrict__) (L2_Memory+230912), /* Bias */
		(signed char *__restrict) (L2_Memory+231424), /* MulBias */
		(signed char *__restrict) (L2_Memory+50176), /* Out */
		7, /* Norm */
		7, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[23] = gap_cl_readhwtimer() - MNPerf[23];
	/* Moving FL25, size 9216 from HyperRam at 5357568 to (size 9216) L2 at 100352 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5357568), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 100352), 9216, 0, &Uchan2);
	/* Moving BL25, size 1024 from HyperRam at 5406672 to (size 1024) L2 at 109568 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5406672), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 109568), 1024, 0, &Uchan3);
	/* Moving ML25, size 1024 from HyperRam at 5407696 to (size 1024) L2 at 110592 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5407696), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 110592), 1024, 0, &Uchan4);
	/* Waiting completion of transfer of BL24 using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan3);
	/* Waiting completion of transfer of ML24 using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan4);
	MNPerf[24] = gap_cl_readhwtimer();
	Layer24(
		(signed char *__restrict) (L2_Memory+50176), /* In2 */
		(signed char *__restrict) (L3_Memory+2514944), /* In1 */
		(signed char * __restrict__) (L2_Memory+200704), /* Bias */
		(signed char *__restrict) (L2_Memory+201728), /* MulBias */
		(signed char *__restrict) (L2_Memory+0), /* Out */
		7, /* Norm */
		7, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[24] = gap_cl_readhwtimer() - MNPerf[24];
	/* Moving BL26, size 1024 from HyperRam at 5408720 to (size 1024) L2 at 116736 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5408720), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 116736), 1024, 0, &Uchan3);
	/* Moving ML26, size 1024 from HyperRam at 5409744 to (size 1024) L2 at 117760 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5409744), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 117760), 1024, 0, &Uchan4);
	/* Waiting completion of transfer of FL25 using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan2);
	/* Waiting completion of transfer of BL25 using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan3);
	/* Waiting completion of transfer of ML25 using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan4);
	MNPerf[25] = gap_cl_readhwtimer();
	Layer25(
		(signed char *__restrict) (L2_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+100352), /* Filter */
		(signed char * __restrict__) (L2_Memory+109568), /* Bias */
		(signed char *__restrict) (L2_Memory+110592), /* MulBias */
		(signed char *__restrict) (L2_Memory+50176), /* Out */
		2, /* Norm */
		1, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[25] = gap_cl_readhwtimer() - MNPerf[25];
	/* Waiting completion of transfer of BL26 using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan3);
	/* Waiting completion of transfer of ML26 using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan4);
	MNPerf[26] = gap_cl_readhwtimer();
	Layer26(
		(signed char *__restrict) (L2_Memory+50176), /* In2 */
		(signed char *__restrict) (L3_Memory+3039232), /* In1 */
		(signed char * __restrict__) (L2_Memory+116736), /* Bias */
		(signed char *__restrict) (L2_Memory+117760), /* MulBias */
		(signed char *__restrict) (L2_Memory+0), /* Out */
		7, /* Norm */
		7, /* NormBias */
		7 /* NormMulBias */
	);
	MNPerf[26] = gap_cl_readhwtimer() - MNPerf[26];
	/* Moving BL28, size 2000 from HyperRam at 5402624 to (size 2000) L2 at 51200 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) L3_Memory + 5402624), ((AT_HYPERRAM_INT_ADDR_TYPE) L2_Memory + 51200), 2000, 0, &Uchan0);
	MNPerf[27] = gap_cl_readhwtimer();
	Layer27(
		(signed char *__restrict) (L2_Memory+0), /* In */
		(signed char *__restrict) (L2_Memory+50176) /* Out */
	);
	MNPerf[27] = gap_cl_readhwtimer() - MNPerf[27];
	/* Waiting completion of transfer of BL28 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &Uchan0);
	MNPerf[28] = gap_cl_readhwtimer();
	Layer28(
		(signed char *__restrict) (L2_Memory+50176), /* In */
		(signed char *__restrict) (L3_Memory+4087808), /* Filter */
		(short int * __restrict__) (L2_Memory+51200), /* Bias */
		(short int *__restrict) (L2_Memory+0), /* Out */
		0, /* Norm */
		0 /* NormBias */
	);
	MNPerf[28] = gap_cl_readhwtimer() - MNPerf[28];
	return 0;
}
