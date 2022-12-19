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
#define Ntap 35
#define DCgain 524288
#define HEADROOM_BITS 3

Int16 FIRCoef[Ntap] = {
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
