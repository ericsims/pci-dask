/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name : 7432INT.C                                              ;;
;;      Purpose   : This sample program demonstrates how handle interrupt  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "dask.h"
#include "conio.h"

int int_count=0, int2_count=0;
I16 card=-1, card_number = 0;

void sig1_handler( int signo )
{
    switch( signo ){
        case SIGEVENT1:
            int_count ++;
            break;
    }
    printf("INT1 count = %d\n", int_count);
    printf("press any key to generate an interrupt or press ESC for exit\n" );

    return;
}

void sig2_handler( int signo )
{
    switch( signo ){
        case SIGEVENT2:
            int2_count ++;
            break;
    }
    printf("INT2 count = %d\n", int2_count);
    printf("press any key to generate an interrupt or press ESC for exit\n" );

    return;
}

int main(void)
{
    int  err;
    char temp_char;

    setbuf( stdout, NULL );
    printf("PCI-7432 Dual Interrupt Sample\n" );
    printf("This program shows the number of occurrence of interrupt\n" );
    printf("for Dual interrupt lines.\n" );
    printf("INT1 interrupt is generated by external signal from DI bit-0.\n");
    printf("INT2 interrupt is generated by external signal from DI bit-1.\n");
    printf("If you connect DO bit-0 to DI bit-0 and DO bit-1 to DI bit-1,\n");
    printf("press any key can generate two interrupts.\n");
    printf("One for INT1 and the other for INT2.\n");
    printf("Or press ESC for exit\n\n" );

    int_count = int2_count=0;
    if ((card=Register_Card(PCI_7432, card_number)) < 0) {
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    err = DIO_SetDualInterrupt(card, INT1_EXT_SIGNAL, INT2_EXT_SIGNAL, sig1_handler, sig2_handler);

    if( err != NoError ){
        printf(" error from DIO_SetDualInterrupt : %d \n" , err );
        goto exit_after_error;
    }

    while(1){
        temp_char = getch();
        if( temp_char == 0x1b )
            break;

        DO_WritePort(card,0, 0x3);
        usleep(1000);
        DO_WritePort(card,0, 0x0);
    }

    DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_DISABLE, NULL, NULL);

exit_after_error:
    if (card >= 0)
        Release_Card(card);
    exit(0);
}
