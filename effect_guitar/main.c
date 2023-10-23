//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *                                                                          
// * Description:  Main function.
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

#define SIZE_BUFFER 1024
#define SAMPLE_RATE 44100
#define RATE 2.5
#define DEPTH 0.95

//extern Int16 aic3204_test( );
//
//int  TestFail    = (int)-1;   
//
//void StopTest()
//{
//    //SW_BREAKPOINT;
//    return;
//}

/*
 *
 *  main( )
 *
 */
int main()
{
	// Teste variable	
	FILE *inputfile;
	FILE *outputfile;
    char *inputFilename = "F:\\Documentos\\Guitar_Sound\\Guitar_Normal.pcm";
    char *outputFilename = "F:\\Documentos\\Guitar_Sound\\Guitar_Tremolo1.pcm";

    Uint16 inputBuffer[SIZE_BUFFER];
    Uint16 outputBuffer[SIZE_BUFFER];
    size_t bytesRead;
    size_t i;
//    int32_t k;
    
//    /* Initialize BSL */
//    EZDSP5502_init( );
    
    inputfile = fopen(inputFilename, "rb");
    if (inputfile == NULL)
    {
        printf("Erro ao abrir o arquivo 1\n");
        fclose(inputfile);
    }

    outputfile = fopen(outputFilename, "wb");
    if (outputfile == NULL)
    {
        printf("Erro ao abrir o arquivo 2\n");
    	fclose(outputfile);
    } 
    
    bytesRead = fread(inputBuffer, sizeof(Uint16), SIZE_BUFFER,inputfile);
    
    while (bytesRead > 0){
	    for (i = 0; i < bytesRead; i++){
		    outputBuffer[i] = inputBuffer[i];
	    }
	
	    fwrite(outputBuffer, sizeof(Uint16), bytesRead, outputfile);
	    bytesRead = fread(inputBuffer, sizeof(Uint16), SIZE_BUFFER, inputfile);
    }
    
    fclose(inputfile);
    fclose(outputfile);
    
    printf("O audio foi exportado");
	
//    /* Initialize BSL */
//    EZDSP5502_init( );
//
//    /* Display test ID */
//    printf( "\nTesting AIC3204...\n");
//
//    /* Call test function */
//    TestFail = aic3204_test( );
//
//    /* Check for test fail */
//    if ( TestFail != 0 )
//    {
//        /* Print error message */
//        printf( "     FAIL... error code %d... quitting\n", TestFail );
//    }
//    else
//    {
//        /* Print pass message */
//        printf( "    PASS\n" );
//        printf( "\n***ALL Tests Passed***\n" );
//    }
//    
//    StopTest();
	return 0;
}
