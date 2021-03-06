#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "dask.h"
#include "conio.h"

int int_count=0, int2_count=0;
I16 card=-1, card_number;

void sig1_handler( int signo )
{
    U32 CosData = 0;
    switch( signo ){
        case SIGEVENT1:
            int_count ++;
            break;
    }
    DIO_GetCOSLatchDataInt32(card, 0, &CosData);
    printf("INT1 count = %d, COS Data: %x. \n", int_count, CosData);

    return;
}

void sig2_handler( int signo )
{
    U32 CosData = 0;
    switch( signo ){
        case SIGEVENT2:
            int2_count ++;
            break;
    }
    DIO_GetCOSLatchDataInt32(card, 1, &CosData);
    printf("INT2 count = %d, COS Data: %x. \n", int2_count, CosData);

    return;
}

int main(void)
{
    int err;
    char temp_char;

    setbuf( stdout, NULL );
    printf("\nPCI-7442 Dual Interrupt Sample\n" );
    printf("\nThis program shows the number of occurrence of interrupt\n" );
    printf("for COS0 & COS1 interrupt lines.\n" );

    printf("\nPlease input a card number: ");
    scanf(" %hd", &card_number);

    if ((card=Register_Card(PCI_7442, card_number)) < 0) {
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    usleep(1000);

    err = DIO_SetCOSInterrupt32(card, 0, 0x0001, sig1_handler);
    if( err != NoError ){
        printf(" error from DIO_SetCOSInterrupt32 : %d \n" , err );
        goto exit_after_error;
    }

    err = DIO_SetCOSInterrupt32(card, 1, 0x0001, sig2_handler);
    if( err != NoError ){
        printf(" error from DIO_SetCOSInterrupt32 : %d \n" , err );
        goto exit_after_error;
    }

    printf("\nPress any key to exit...\n" );
    temp_char = getch();

    DIO_SetCOSInterrupt32(card, 0, 0, sig1_handler);
    DIO_SetCOSInterrupt32(card, 1, 0, sig2_handler);

exit_after_error:
    if (card >= 0)
        Release_Card(card);
    exit(0);
}
