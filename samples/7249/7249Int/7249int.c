/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name   :   7249int.c                                          ;;
;;      Purpose     :   This sample program demonstrates how to DIO through;;
;;                      interrupt operation                                ;;
;;      Date        :   07/07/1999                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
#include <stdio.h>
#include <unistd.h>
#include "dask.h"

I16 card = -1, card_number = 0;
int int2_count = 0;
int total_count = 100;
int sample_rate = 1000;
unsigned int buffer[32768], in_buffer[32768];
int c1 = 20, c2 = 100;

void sig_handler2(int signo)
{
    if(int2_count<total_count){
        DO_WritePort(card,Channel_P2A, buffer[int2_count]);
        DI_ReadPort(card,Channel_P1A,&in_buffer[int2_count]);
        int2_count++;
    }
    else if(int2_count==total_count){
        // reset the Timer to stop generate INT2
        CTR_Setup (card, 1, RATE_GENERATOR, 0, BIN);
        CTR_Setup (card, 2, RATE_GENERATOR, 0, BIN);
        printf("\n\n INT2 Count:%d \n\n\n",int2_count);
        printf(" press ENTER for display data \n"); // this will be catched by getchar() in main()
        int2_count++;
    }

   return;
}


int main(void)
{
    int i, k = 0, err;

    setbuf(stdout, NULL);

    printf("PCI-7249 Interrupt Samples (INT2)\n\n");
    printf("This program performs digital output on Port A of\n");
    printf("CN2 and digital input on Port A of CN1 when an\n");
    printf("interrupt happens. The INT2 is used, and interrupt\n");
    printf("signals are generated by internal Timer Output.\n");

    int2_count = 0;

    if((card = Register_Card(PCI_7249, card_number))<0){
        printf ("Can't open device file: PCI7249\n");
        exit(-1);
    }
    CTR_Setup(card, 1, RATE_GENERATOR, 0, BIN);
    CTR_Setup(card, 2, RATE_GENERATOR, 0, BIN);
    usleep(1000);

    //initialize the input/output array
    for(i=0; i<total_count; i++){
        buffer[i]=k;
        k++;
        if(k>255)
            k = 0;
    }

    for(i=0; i<total_count; i++)
        in_buffer[i]=0;

    //configure the port
    err = DIO_PortConfig(card,Channel_P1A, INPUT_PORT);
    if(err!=NoError){
        goto exit_after_error;
    }
    err = DIO_PortConfig(card,Channel_P2A, OUTPUT_PORT);
    if(err!=NoError){
        goto exit_after_error;
    }
    err = DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_TIMER_COUNTER, NULL, sig_handler2);
    if(err!=NoError){
        printf(" error from DIO_SetDualInterrupt : %d \n" , err);
        goto exit_after_error;
    }

    //setup the Timer clock
    err = CTR_Setup(card, 1, 2, c1, BIN);
    if(err!=NoError){
        printf(" error from CTR_Setup : %d \n", err);
        goto end_of_proc;
    }
    err = CTR_Setup(card, 2, 2, c2, BIN);
    if(err!=NoError){
        printf(" error from CTR_Setup : %d \n", err);
        goto end_of_proc;
    }

    printf("** The INT2 Singal will be sent back in 10 secs , or you can press ENTER for exit **\n");
    getchar();

    if(int2_count<total_count){
        CTR_Setup(card, 1, RATE_GENERATOR, 0, BIN);
        CTR_Setup(card, 2, RATE_GENERATOR, 0, BIN);
        goto end_of_proc;
    }
    else{ //display the data
        for(i=0; i<total_count; i++){
            printf(" %d, ",in_buffer[i]);
            if((i+1)%10==0)
                putchar('\n');
        }
    }

end_of_proc:
    DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_DISABLE, NULL, NULL);

exit_after_error:
    if(card>=0)
        Release_Card(card);
    exit(0);
}
