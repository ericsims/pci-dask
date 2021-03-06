#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

#define MAX_SIZE  1000   //in words 
U32 data_size=MAX_SIZE, j=0, count;
FILE *fp;
I16 card=-1, card_number = 0;
char *file_name="7300b.dat";
BOOLEAN clear_op;
U16 in_buf[MAX_SIZE], in_buf2[MAX_SIZE], in_buf3[MAX_SIZE], in_buf4[MAX_SIZE];
U16 in_buf5[MAX_SIZE], in_buf6[MAX_SIZE], in_buf7[MAX_SIZE], in_buf8[MAX_SIZE];
U16 in_buf9[MAX_SIZE], in_buf10[MAX_SIZE], in_buf11[MAX_SIZE], in_buf12[MAX_SIZE];
 
U16 viewidx[10000];
U32 t=0;
U16 sts=0;

BOOLEAN write_to_file( U16 *buf);

void signal_handler(int sig_no)
{
  BOOLEAN HalfReady;  
   
       HalfReady=0;
       DI_AsyncMultiBufferNextReady(card, &HalfReady, &viewidx[j%10000]);
       switch(viewidx[j%10000]) {
           case 0:
              write_to_file( in_buf );
              break;
           case 1:
              write_to_file( in_buf2 );
              break;
           case 2:
              write_to_file( in_buf3 );
              break;
           case 3:
              write_to_file( in_buf4 );
              break;
           case 4:
              write_to_file( in_buf5 );
              break;
           case 5:
              write_to_file( in_buf6 );
              break;
           case 6:
              write_to_file( in_buf7 );
              break;
           case 7:
              write_to_file( in_buf8 );
              break;
           case 8:
              write_to_file( in_buf9 );
              break;
           case 9:
              write_to_file( in_buf10 );
              break;
           case 10:
              write_to_file( in_buf11 );
              break;
           case 11:
              write_to_file( in_buf12 );
              break;
       }
       DI_AsyncDblBufferTransfer(card, NULL);  //notify the buffer have handled
       DI_AsyncDblBufferOverrun(card, 0, &sts);
       if(sts==1) {
                t++;
                DI_AsyncDblBufferOverrun(card, 1, &sts);
       }
       printf("\roverrun:%d cb:%d vd: %d\n", t, j, viewidx[j%10000]);
       j++;    
}

void signal_handler2(int sig_no)
{
     printf("\nfinish\n");
}

int main(void)
{
    I16 err;
    U32 i;
    U16 BufferId;

    setbuf( stdout, NULL );
    clrscr();
    printf(" ========================================================================\n");
    printf(" ==           7300 SAMPLE : PCI-7300 Multiple Buffered DMA             ==\n");
    printf(" ========================================================================\n\n\n");
    printf(" The output data is a sine wave and store data to file '%s'. It will not stop until you press a key.\n\nPress any key to start the operation.\n", file_name );

    getch();
  
  if ((card=Register_Card(PCI_7300A_RevB, card_number)) < 0) {
        printf("Register_Card error=%d\n", card);
        exit(1);
  }
   if( (fp = fopen( file_name, "w+b" ) ) == NULL )
        return -10;

  for (i=0; i<MAX_SIZE; i++) {
       in_buf[i] = (U16)(0);
       in_buf2[i] = (U16)(0);
  }

  err=DI_7300B_Config(card, 16, /*TRIG_CLK_10MHZ*/TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_ON, 0, 1, 1);
  if (err !=0) {
               printf("DI_7300B_Config error=%d\n", err);
               Release_Card( card );
               exit(1);
        }         
    //setup signal handler
    DI_EventCallBack (card, 1, DBEvent, signal_handler);
    DI_EventCallBack (card, 1, DIEnd, signal_handler2);

	err = DI_ContMultiBufferSetup (card, in_buf, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf2, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }	        
	err = DI_ContMultiBufferSetup (card, in_buf3, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf4, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf5, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf6, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf7, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf8, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf9, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf10, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf11, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf12, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
        err = DI_ContMultiBufferStart (card, 0, 1000);
        if (err < 0) {
               printf("DI_ContMultiBufferStart error=%d", err);
               DO_AsyncClear(card, &i);
               Release_Card( card );
               exit(1);
        }  
        printf("\n\nPress any key to stop input operation.");
        printf("\n\nData count : \n"); 
        getch();

        err = DI_AsyncClear(card, &count);        
        if( err != 0 ){
           printf("D2K_AI_AsyncClear Failed !!!, Ready to system hang !!!\n" );
           getch();
        }
        //for(t=0; t<j; t++)
          //printf("%d ",viewidx[t]);
       //printf("\n");
       //clear signal handler
       DI_EventCallBack (card, 0, DBEvent, NULL);
       DI_EventCallBack (card, 0, DIEnd, NULL);

       Release_Card(card);
       fclose(fp);
       exit(1);
}

BOOLEAN write_to_file(U16 *buf)
{
    int vi;
    for(vi=0; vi<MAX_SIZE; vi++){
        fprintf(fp, "%x\n", buf[vi]);
    }

  //fwrite(buf, sizeof(U16), data_size, fp);

  return TRUE;
}

