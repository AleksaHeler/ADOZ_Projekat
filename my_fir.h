/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Rectangular Window
Sampling Frequency: 48 KHz
Cut Frequency: 1.000000 KHz
Coefficents Quantization: 16-bit
***************************************************************/
#define Ntap 129
#define DCgain 524288
#define HEADROOM_BITS 3

Int16 FIRCoef_35[35] = {
	 8022,
	 9054,
	10079,
	11087,
	12071,
	13022,
	13932,
	14793,
	15597,
	16337,
	17006,
	17599,
	18111,
	18535,
	18870,
	19111,
	19256,
	19305,
	19256,
	19111,
	18870,
	18535,
	18111,
	17599,
	17006,
	16337,
	15597,
	14793,
	13932,
	13022,
	12071,
	11087,
	10079,
	 9054,
	 8022
};

Int16 FIRCoef_77[77] = {
	-4210,
	-4269,
	-4266,
	-4196,
	-4056,
	-3844,
	-3558,
	-3199,
	-2764,
	-2255,
	-1674,
	-1023,
	 -305,
	  476,
	 1316,
	 2210,
	 3151,
	 4133,
	 5149,
	 6191,
	 7251,
	 8320,
	 9391,
	10453,
	11500,
	12520,
	13507,
	14450,
	15343,
	16177,
	16944,
	17639,
	18254,
	18784,
	19224,
	19571,
	19821,
	19972,
	20023,
	19972,
	19821,
	19571,
	19224,
	18784,
	18254,
	17639,
	16944,
	16177,
	15343,
	14450,
	13507,
	12520,
	11500,
	10453,
	 9391,
	 8320,
	 7251,
	 6191,
	 5149,
	 4133,
	 3151,
	 2210,
	 1316,
	  476,
	 -305,
	-1023,
	-1674,
	-2255,
	-2764,
	-3199,
	-3558,
	-3844,
	-4056,
	-4196,
	-4266,
	-4269,
	-4210
};

Int16 FIRCoef_129[129] = {
	 2485,
	 2510,
	 2497,
	 2445,
	 2353,
	 2222,
	 2052,
	 1843,
	 1598,
	 1319,
	 1008,
	  669,
	  305,
	  -78,
	 -476,
	 -885,
	-1298,
	-1710,
	-2114,
	-2505,
	-2875,
	-3219,
	-3531,
	-3803,
	-4031,
	-4208,
	-4329,
	-4391,
	-4387,
	-4315,
	-4171,
	-3953,
	-3659,
	-3289,
	-2842,
	-2319,
	-1722,
	-1052,
	 -313,
	  489,
	 1354,
	 2273,
	 3241,
	 4250,
	 5295,
	 6366,
	 7456,
	 8556,
	 9657,
	10750,
	11825,
	12875,
	13889,
	14860,
	15778,
	16635,
	17424,
	18138,
	18771,
	19316,
	19769,
	20126,
	20383,
	20538,
	20590,
	20538,
	20383,
	20126,
	19769,
	19316,
	18771,
	18138,
	17424,
	16635,
	15778,
	14860,
	13889,
	12875,
	11825,
	10750,
	 9657,
	 8556,
	 7456,
	 6366,
	 5295,
	 4250,
	 3241,
	 2273,
	 1354,
	  489,
	 -313,
	-1052,
	-1722,
	-2319,
	-2842,
	-3289,
	-3659,
	-3953,
	-4171,
	-4315,
	-4387,
	-4391,
	-4329,
	-4208,
	-4031,
	-3803,
	-3531,
	-3219,
	-2875,
	-2505,
	-2114,
	-1710,
	-1298,
	 -885,
	 -476,
	  -78,
	  305,
	  669,
	 1008,
	 1319,
	 1598,
	 1843,
	 2052,
	 2222,
	 2353,
	 2445,
	 2497,
	 2510,
	 2485
};


Int16 my_fir_circular(Int16 input, Int16 *coeffs, Int16 *history, Uint16 n_coeff, Uint16 *p_state) {
	Int16 i;
	Uint16 state;
	Int32 ret_val;

	state = *p_state; /* copy the filter's state to a local */

	/* store input at the beginning of the delay line */
	history[state] = input;
	if (++state >= n_coeff) { /* incr state and check for wrap */
		state = 0;
	}

	/* calc FIR and shift data */
	ret_val = 0;
	for (i = n_coeff - 1; i >= 0; i--) {
		ret_val = _smac(ret_val, coeffs[i] >> HEADROOM_BITS, history[state]);

		if (++state >= n_coeff) { /* incr state and check for wrap */
			state = 0;
		}
	}

	*p_state = state; /* return new state to caller */

	return (Int16)(ret_val >> 16);
}
