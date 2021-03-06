/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/09                                                       */
/*                                                                            */
/* This sample performs DI pattern match functionality.                       */
/* In PCIe-7350, there are 32 programmable DIO channels. Each 8 channels are  */
/* devided into one port that is the DIO configuration unit.                  */
/* You can set a pattern for the set DI channels. Internal interrupt will be  */
/* asserted while the input digital signal is matched to the configured       */
/* pattern.                                                                   */
/* AFI port can also be set as the COS event out mode, and the set AFI port   */
/* will generate a pulse while COS occurs.                                    */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "conio.h"
#include "dask.h"

I16 card;
U32 PMLData;
U32 Counter=0;

#define MATCH_PATTERN 0x2f01

void sig_handler(int signo)
{
	I16 err = DIO_GetPMLatchData32(card, 0, &PMLData);
	if(err<0){
		printf("DIO_GetPMLatchData32 Error: %d\n", err);
	}
	printf("Pattern Matched; Latched Data: 0x%lx\n\n\n", PMLData);
	/*
	* PCI-7350 Pattern Match is a one pulse event, so reconfigure
	* and restart is necessary if you want to match a new pattern.
	*/
	//Set DI to zero
	DO_WritePort(card, P7350_DIO_A, 0);
	DO_WritePort(card, P7350_DIO_C, 0);
	//restart Pattern Match
	err = DIO_PMControl(card, 0, PATMATCH_RESTART, sig_handler);
	if(err<0){
		printf("DIO_PMControl Error: %d\n", err);
	}
}


int main(int argc, char **argv)
{
    I16 err;
    U16 card_num;
    U32 vi;
    U32 DIRead1, DIRead2;
    //BOOLEAN fStop;
    U16 pattern = MATCH_PATTERN;
    U16 PM_ChnType;
    U32 PMEventOutLen = 100; //100 system clock - 100 * 8 ns

    printf("This sample performs pattern match functionality.\n");
    printf("DIO Port A/C are configured as a DO port while Port B/D\n");
    printf("are configured as a DI Port. Signal will be asserted\n");
    printf("and a %ld ns pulse will be generated to AFI0 while\n", 8*PMEventOutLen);
    printf("the set pattern is matched.\n");
    printf("This sample will match the pattern: 0x%x from Port B/D.\n", MATCH_PATTERN);

    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*Open and Initialize Device*/
    card = Register_Card(PCI_7350, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure 7350 DIO Port*/
    err = DIO_PortConfig(card, P7350_DIO_A, OUTPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_A Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_B, INPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_B Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_C, OUTPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_A Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_D, INPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_B Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    DO_WritePort(card, P7350_DIO_A, 0);
    DO_WritePort(card, P7350_DIO_C, 0);
    usleep(1000);

    /*
     * Configure Pattern Match
     */
    for(vi=8; vi<16; vi++){
        /*Port B: Channel 8~15*/
        PM_ChnType = ((pattern>>(vi-8))&0x1)? PATMATCH_Level_H:PATMATCH_Level_L;
        err = DIO_PMConfig(card, vi, PATMATCH_CHNEnable, PM_ChnType);
        if(err<0){
            printf("DIO_PMConfig Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
    }
    for(vi=24; vi<32; vi++){
        /*Port D: Channel 24~31*/
        PM_ChnType = ((pattern>>(vi-16))&0x1)? PATMATCH_Level_H:PATMATCH_Level_L;
        err = DIO_PMConfig(card, vi, PATMATCH_CHNEnable, PM_ChnType);
        if(err<0){
            printf("DIO_PMConfig Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
    }

    /*
     * Start Pattern Match
     */
    err = DIO_PMControl(card, 0, PATMATCH_START, sig_handler);
    if(err<0){
        printf("DIO_PMControl Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set PM event out to AFI port
     */
    err = DIO_7350_AFIConfig(card, P7350_AFI_0, 1, P7350_AFI_PMTrigOut, PMEventOutLen);
    if(err<0){
        printf("DIO_7350_AFIConfig Port_A Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    do{
        /*
         * DO output to Port A/C
         */
        //printf("Output Data?\n");
        //scanf(" %lx", &vi);
		usleep(1000000);
		clrscr();
    printf("This sample performs pattern match functionality.\n");
    printf("DIO Port A/C are configured as a DO port while Port B/D\n");
    printf("are configured as a DI Port. Signal will be asserted\n");
    printf("and a %ld ns pulse will be generated to AFI0 while\n", 8*PMEventOutLen);
    printf("the set pattern is matched.\n");
    printf("This sample will match the pattern: 0x%x from Port B/D.\n\n", MATCH_PATTERN);
printf("Press ENTER to Quit.\n\n\n");
		if(Counter%4==0)
			vi = 0;
		else if(Counter%4==1)
			vi = 12033;
		else if(Counter%4==2)
			vi = 0;
		else
			vi = 303;
        DO_WritePort(card, P7350_DIO_A, vi&0xff);
	usleep(1000);
        DI_ReadPort(card, P7350_DIO_B, &DIRead1);
        DO_WritePort(card, P7350_DIO_C, (vi>>8)&0xff);
	usleep(1000);
        DI_ReadPort(card, P7350_DIO_D, &DIRead2);
        printf("Write to Port A: 0x%lx, Port C: 0x%lx; Read from Port B: 0x%lx, Port D: 0x%lx;\n\n\n", vi&0xff, (vi>>8)&0xff, DIRead1, DIRead2);
		Counter++;
        //printf("Press ENTER to Quit.\n");
        //fStop = getch();
    }while(!kbhit()/*(fStop!='q')&&(fStop!='Q')*/);

    /*
     * Stop Pattern Match
     */
    err = DIO_PMControl(card, 0, PATMATCH_STOP, NULL);
    if(err<0){
        printf("DIO_PMControl Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    for(vi=8; vi<16; vi++){
        /*Port B: Channel 8~15*/
        err = DIO_PMConfig(card, vi, PATMATCH_CHNDisable, 0);
        if(err<0){
            printf("DIO_PMConfig Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
    }
    for(vi=24; vi<32; vi++){
        /*Port D: Channel 24~31*/
        err = DIO_PMConfig(card, vi, PATMATCH_CHNDisable, 0);
        if(err<0){
            printf("DIO_PMConfig Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
    }

    err = DIO_7350_AFIConfig(card, P7350_AFI_0, 0, 0, 0);
    if(err<0){
        printf("DIO_7350_AFIConfig Error: %d\n", err);
    }

    //printf("\nPress any key to exit...\n");
    //getch();
    Release_Card(card);
    return 0;
}
