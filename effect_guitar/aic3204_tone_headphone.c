//////////////////////////////////////////////////////////////////////////////
// * File name: aic3204_tone_headphone.c
// *                                                                          
// * Description:  AIC3204 Loop test.
// *                                                                          
// * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
// * Copyright (C) 2011 Spectrum Digital, Incorporated
// *                                                                          
// *                                                                          
// *  Redistribution and use in source and binary forms, with or without      
// *  modification, are permitted provided that the following conditions      
// *  are met:                                                                
// *                                                                          
// *    Redistributions of source code must retain the above copyright        
// *    notice, this list of conditions and the following disclaimer.         
// *                                                                          
// *    Redistributions in binary form must reproduce the above copyright     
// *    notice, this list of conditions and the following disclaimer in the   
// *    documentation and/or other materials provided with the                
// *    distribution.                                                         
// *                                                                          
// *    Neither the name of Texas Instruments Incorporated nor the names of   
// *    its contributors may be used to endorse or promote products derived   
// *    from this software without specific prior written permission.         
// *                                                                          
// *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     
// *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       
// *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR   
// *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT    
// *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   
// *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        
// *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   
// *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   
// *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     
// *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   
// *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    
// *                                                                          
//////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"
#include "math.h"

#define PI 3.14
#define DATA_MARK (u_int32_t)0x61746164
#define DATA_AMPL (float)32768.0
#define SIZE_BUFFFER 1024

#define SIZE_BUFFER 1024
#define SAMPLE_RATE 44100
#define RATE 2.5
#define DEPTH 0.95

extern Int16 AIC3204_rset( Uint16 regnum, Uint16 regval);

/*
 *
 *  AIC3204 Tone
 *      Outputs a 1KHz tone on STEREO OUT
 *
 */
 
 // Função para gerar uma amostra da onda senoidal
Int16 generateSample(double frequency, double amplitude, double phase, double n);

Int16 variableDelay(int delay_swing, int delay_average, float flanger_speed, int n, int periodo_sampling);

typedef struct {
    char chunkId[4];
    uint32_t chunkSize;
    char format[4];
    char subChunk1Id[4];
    uint32_t subChunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subChunk2Id[4];
    uint32_t subChunk2Size;
} WavHeader;

Int16 aic3204_tone_headphone( )
{		
    Int16 sec, msec, sample,sinewave;
//    Int16 amplitude = 1;
    Int16 frequencia = 500;
    Int16 tempo = 1;
    Int16 fase = 0;
    
        // Configurações da onda senoidal
    double frequency = 50.0;  // Frequência de 440 Hz (A4)
    double amplitude = 2000.0;  // Amplitude máxima para um sinal de 16 bits
    double duration = 5.0;  // Duração em segundos
    double samplingRate = 44100.0;  // Taxa de amostragem de 44100 Hz

    int numSamples = (int)(duration * samplingRate);

    // Calcular o período de amostragem
    double samplePeriod = 1.0 / samplingRate;

    // Inicializar o tempo e a fase
    double time = 0.0;
    double phase = 0.0;
    
    // Flanger
    float g = 1;
    int delay_swing = 1;
    int delay_average = 6;
    float flanger_speed = 0.1;
    
    int16_t sample_size;  // Tamanho da amostra em bytes.
	int num_samples;
	Int16 audioData[SIZE_BUFFFER];
	
	// Teste variable	
	FILE *inputfile, *outputfile;
    char *inputFilename = "F:\\Documentos\\UFBA\\Lab Integrado III\\LAB3-Problema1\\python\\Guitar_Sound\\Guitar_Normal.pcm";
    char *outputFilename = "F:\\Documentos\\UFBA\\Lab Integrado III\\LAB3-Problema1\\python\\Guitar_Sound\\Guitar_Tremolo.pcm";

    Uint8 inputBuffer[SIZE_BUFFER];
    Uint8 outputBuffer[SIZE_BUFFER];
    size_t bytesRead;
    size_t i;
    int32_t k;

     /* Pre-generated sine wave data, 16-bit signed samples */
    Int16 sinetable[48] = {
        0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
        0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
        0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
        0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
        0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
        0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
    };
    
    inputfile = fopen(inputFilename, "rb");
    if (inputfile == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    outputfile = fopen(outputFilename, "wb");
    if (outputfile == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    } 
    
    bytesRead = fread(inputBuffer, sizeof(Uint8), SIZE_BUFFER,inputfile);
    
    while (bytesRead > 0)
    {
        for (i = 0; i < bytesRead; i++){
            outputBuffer[i] = inputBuffer[i];
            
        }

        fwrite(outputBuffer, sizeof(Uint8), bytesRead, outputfile);
        bytesRead = fread(inputBuffer, sizeof(Uint8),SIZE_BUFFER,inputfile);
        k = k + SIZE_BUFFER;
        
    }

    fclose(inputfile);
    fclose(outputfile);
    
    printf("O audio foi exportado");
    
// 	WavHeader wavHeader;
//	FILE *wavFile; 
//	wavFile = fopen ("F:\\Documentos\\UFBA\\Lab Integrado III\\Trabalho1_Lab3\\Guitar_Sound\\Guitar_Normal.pcm", "rb");
//	if (wavFile == NULL) {
//	    printf("Erro ao abrir o arquivo PCM.\n");
//	    return 1;
//	}
//
//	// Supondo que o áudio seja armazenado em amostras de 16 bits (2 bytes).
//	num_samples = wavHeader.subChunk2Size / (wavHeader.bitsPerSample / 8);
//	
//	bytesRead = fread(audioData, sizeof(Int16),SIZE_BUFFFER, wavFile);

//	  FILE *srcFile, *resFile;
//
//  srcFile = fopen("/Guitar_Sound/Guitar_Normal.wav", "r");
////  resFile = fopen("../assets/flanger_float.wav", "w+");
//
//  if (!srcFile /*|| !resFile*/) {
//    printf("Error while opening the file.\n");
//    return 0;
//  }
//	
	  
    /* ---------------------------------------------------------------- *
     *  Configure AIC3204                                               *
     * ---------------------------------------------------------------- */
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 1, 1 );      // Reset codec
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 1, 8 );      // Disable crude AVDD generation from DVDD
    AIC3204_rset( 2, 1 );      // Enable Analog Blocks, use LDO power
    AIC3204_rset( 0, 0 );
    
    /* PLL and Clocks config and Power Up  */
    AIC3204_rset( 27, 0x0d );  // BCLK and WCLK are set as o/p; AIC3204(Master)
    AIC3204_rset( 28, 0x00 );  // Data ofset = 0
    AIC3204_rset( 4, 3 );      // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3204_rset( 6, 7 );      // PLL setting: J=7
    AIC3204_rset( 7, 0x06 );   // PLL setting: HI_BYTE(D=1680)
    AIC3204_rset( 8, 0x90 );   // PLL setting: LO_BYTE(D=1680)
    AIC3204_rset( 30, 0x9C );  // For 32 bit clocks per frame in Master mode ONLY
                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3204_rset( 5, 0x91 );   // PLL setting: Power up PLL, P=1 and R=1
    AIC3204_rset( 13, 0 );     // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3204_rset( 11, 0x82 );  // Power up NDAC and set NDAC value to 2
    AIC3204_rset( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2

    /* DAC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x0c, 8 );   // LDAC AFIR routed to HPL
    AIC3204_rset( 0x0d, 8 );   // RDAC AFIR routed to HPR
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 64, 2 );     // Left vol=right vol
    AIC3204_rset( 65, 0);      // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 9, 0x30 );   // Power up HPL,HPR
    AIC3204_rset( 0x10, 0x00 );// Unmute HPL , 0dB gain
    AIC3204_rset( 0x11, 0x00 );// Unmute HPR , 0dB gain
    AIC3204_rset( 0, 0 );      // Select page 0
    EZDSP5502_waitusec( 100 ); // wait
        
    /* ADC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x34, 0x30 );// STEREO 1 Jack
                               // IN2_L to LADC_P through 40 kohm
    AIC3204_rset( 0x37, 0x30 );// IN2_R to RADC_P through 40 kohmm
    AIC3204_rset( 0x36, 3 );   // CM_1 (common mode) to LADC_M through 40 kohm
    AIC3204_rset( 0x39, 0xc0 );// CM_1 (common mode) to RADC_M through 40 kohm
    AIC3204_rset( 0x3b, 0 );   // MIC_PGA_L unmute
    AIC3204_rset( 0x3c, 0 );   // MIC_PGA_R unmute
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 0x51, 0xc0 );// Powerup Left and Right ADC
    AIC3204_rset( 0x52, 0 );   // Unmute Left and Right ADC
    AIC3204_rset( 0, 0 );    
    EZDSP5502_waitusec( 200 ); // Wait

    /* Initialize McBSP */
    EZDSP5502_MCBSP_init( );
    
//    /* Play Tone for 5 seconds */
//    for ( sec = 0 ; sec < 5 ; sec++ )
//    {
//        for ( msec = 0 ; msec < 1000 ; msec++ )
//        {
//            for ( sample = 0 ; sample < 48 ; sample++ )
//            {
////               EZDSP5502_MCBSP_write( sinetable[sample]);      // TX left channel first (FS Low)
////               EZDSP5502_MCBSP_write( sinetable[sample]);      // TX right channel next (FS High)
////				 Int16 ln = variableDelay(delay_swing, delay_average, flanger_speed, sample, time); 
//				 Int16 sample_audio = generateSample(frequency, amplitude, phase,sample);
////				 Int16 delay = sample - ln;
////				 Int16 y = sample_audio + g * generateSample(frequency, amplitude, delay, phase);
////				 Int16 y1 = sinetable[sample] + g * sinetable[delay];
//				 printf("HELLO!!!\n");
//			  	 EZDSP5502_MCBSP_write(bytesRead[sample]);
//				 EZDSP5502_MCBSP_write(bytesRead[sample]);
////				 time += samplePeriod;
//            }
//        }
//    }
//    
//    
    EZDSP5502_MCBSP_close(); // Disable McBSP
    AIC3204_rset( 1, 1 );    // Reset codec
    
    return 0;
}

Int16 generateSample(double frequency, double amplitude, double phase, double n) {
    return (Int16)(amplitude * sin(2 * PI * frequency * n + phase));
}

Int16 variableDelay(int delay_swing, int delay_average, float flanger_speed, int n, int periodo_sampling){
	return (Int16)(delay_average * (1 + delay_swing*sin(2*PI*flanger_speed*n*periodo_sampling)));
}
