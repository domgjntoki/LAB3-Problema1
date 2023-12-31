//////////////////////////////////////////////////////////////////////////////
// * File name: aic3204_loop_linein.c
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
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"


#define DELAY 10 //mS
#define WIDTH 1 //mS

#define MAX_DELAY (DELAY+WIDTH)*48 // em AMOSTRAS
#define MIN_DELAY DELAY*48 //em AMOSTRAS
#define RATE 1
#define MIX 0.75

#define BUFFER_SIZE MAX_DELAY*2 // Representa o delay em amostras

#define TREMOLO_DEPTH 0.95
#define TREMOLO_RATE 2.5 
#define PI 3.14159265359


extern Int16 AIC3204_rset( Uint16 regnum, Uint16 regval);

/*
 *
 *  AIC3204 Loop
 *      Loops audio from LINE IN to LINE OUT
 *
 */
Int16 aic3204_loop_linein_flanger( )
{
   Int16 sinetable[48] = {
        24, 27, 30, 33, 36, 39, 41, 43,
        45, 46, 47, 48, 48, 48, 47, 46,
        45, 43, 41, 39, 36, 33, 30, 27,
        24, 21, 18, 15, 12, 9, 7, 5,
        3, 2, 1, 0, 0, 0, 1, 2,
        3, 5, 7, 9, 12, 15, 18, 21
    };
    
    Int16 sample, inputL, inputR, outputL, outputR, delayedL, delayedR;
    Int16 bufferL[BUFFER_SIZE] = {0};
    Int16 bufferR[BUFFER_SIZE] = {0};
    Int32 bufferIndex = 0;
    Int32 sineIndex = 0;
    Int16 delaySamples;
    float gain = 0.4;
  
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
    EZDSP5502_waitusec( 1000000 ); // wait
        
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
    
    
    inputL = 0;
    inputR = 0;
    sineIndex = 0;

    while (1) {
        if (sineIndex >= 48)
            sineIndex = 0;
        EZDSP5502_MCBSP_read(&inputL); // Read right channel
        delaySamples = sinetable[sineIndex] * (DELAY + WIDTH) * 2;
        if (delaySamples > bufferIndex) {
            delayedL = bufferL[bufferIndex - delaySamples + BUFFER_SIZE] * MIX;
        } else {
            delayedL = bufferL[bufferIndex - delaySamples] * MIX;
        }
        outputL = inputL * 0.6 + delayedL * gain;
        EZDSP5502_MCBSP_write(outputL); // Send to left channel (FS Low)
        bufferL[bufferIndex] = outputL;

        EZDSP5502_MCBSP_read(&inputR); // Read left channel
        if (delaySamples > bufferIndex) {
            delayedR = bufferR[bufferIndex - delaySamples + BUFFER_SIZE] * MIX;
        } else {
            delayedR = bufferR[bufferIndex - delaySamples] * MIX;
        }
        outputR = inputR * 0.6 + delayedR * gain;
        EZDSP5502_MCBSP_write(outputR); // Send to right channel (FS High)
        bufferR[bufferIndex] = outputR;

        if (++sineIndex >= 48) {
            sineIndex = 0;
        }
        if (++bufferIndex >= BUFFER_SIZE) {
            bufferIndex = 0;
        }
    }

    EZDSP5502_MCBSP_close(); // Disable McBSP
    AIC3204_rset(1, 1); // Reset codec
    
    return 0;
}
