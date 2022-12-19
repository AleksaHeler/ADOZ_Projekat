//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *
// *
// * Description: Lab exercise 3
// * Course: OAiS DSP1
// * Year: 2020
// *
// * Author: <TO DO - add student name and index>
// * Faculty of Technical Sciences, Novi Sad
// *
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "Dsplib.h"
#include "WAVheader.h"
#include "my_fir.h"

static WAV_HEADER outputWAVhdr;
static WAV_HEADER inputWAVhdr;

#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(OutputBufferL,4)
Int16 OutputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferR,4)
Int16 OutputBufferR[AUDIO_IO_SIZE];

Uint16 stateL = 0;
Uint16 stateR = 0;

Int16 historyL[AUDIO_IO_SIZE];
Int16 historyR[AUDIO_IO_SIZE];

Int16 bufferL[AUDIO_IO_SIZE];
Int16 bufferR[AUDIO_IO_SIZE];

/*
 *
 *  main( )
 *
 */
void main( void )
{   
	Int32 i, j;
	Int32 number_of_blocks;

    /* Initialize BSL */
    EZDSP5535_init( );

    printf("\n OAiS DSP 1 - Vezba 3b \n");
		
    /* Initialise hardware interface and I2C for code */
    aic3204_hardware_init();

    aic3204_set_input_filename("../streams/18.wav");
	aic3204_set_output_filename("../streams/output.wav");
	
    /* Initialise the AIC3204 codec */
	aic3204_init();
          
    aic3204_dma_init();
    
    aic3204_read_wav_header(&inputWAVhdr);

    // Set up output WAV header
    outputWAVhdr = inputWAVhdr;

    number_of_blocks = inputWAVhdr.data.SubChunk2Size/
    		(inputWAVhdr.fmt.NumChannels*inputWAVhdr.fmt.BitsPerSample*AUDIO_IO_SIZE/8);

    aic3204_write_wav_header(&outputWAVhdr);

    /* Initialize history buffers to 0 */
	for (i = 0; i < AUDIO_IO_SIZE; i++) {
		historyL[i] = 0;
		historyR[i] = 0;
	}

	for(i = 0; i < number_of_blocks; i++)
	{
		aic3204_read_block(InputBufferL, InputBufferR);

		for(j=0; j < AUDIO_IO_SIZE; j++)
		{
			bufferL[j] = my_fir_circular(InputBufferL[j], FIRCoef_129, historyL, Ntap, &stateL);
			bufferR[j] = my_fir_circular(InputBufferR[j], FIRCoef_129, historyR, Ntap, &stateR);
			OutputBufferR[j] = bufferL[j];
			OutputBufferL[j] = bufferR[j];
		}

		aic3204_write_block(OutputBufferL, OutputBufferR);
	}

	/* Disable I2S and put codec into reset */ 
    aic3204_disable();

    printf( "\n***Program has Terminated***\n" );
	SW_BREAKPOINT;
}

