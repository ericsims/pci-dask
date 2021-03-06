#include <stdio.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0;
    U16 out_value;
    int ch;
    int volt1;
    I16 volt;
    char c;

    setbuf(stdout, NULL);

    if((card = Register_Card(PCI_6308V, card_number))<0){
        printf("Can't open device file: PCI6308V\n");
        exit(-1);
    }

    while(1){
        clrscr();
        printf("PCI-6208V AO Sample :\n\n");
        printf("-------------------------------------------------\n");
        printf("Please type the channel number [0-7]: ");
        scanf(" %d", &ch);
        if((ch<0)||(ch>7)){
            printf("The channel number is invalid\n");
            goto endwhile;
        }
        printf("Please specify a voltage to output (-10V~10V): ");
        scanf(" %d", &volt1);
        volt = (I16)volt1;
        if((volt<-10)||(volt>10)){
            printf("The voltage value is invalid\n");
            goto endwhile;
        }
        out_value = (4096/20)*(volt+10);
        if(out_value>4096)
            out_value = 0xffff;
        printf("The output value is %d.\n", out_value);
        AO_WriteChannel(card, ch, out_value);
        printf("-------------------------------------------------\n");
endwhile:
        printf("[q]Quit [c]Continue ? ");
        //scanf(" %c", &c);
        c = getch();
        if((c=='q')||(c=='Q'))
            break;
    }
    putchar('\n');

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}
