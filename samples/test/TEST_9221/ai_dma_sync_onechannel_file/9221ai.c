#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define ReadCount 10000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    //U16 ConfigCtrl = P9221_AI_Differential|P9221_AI_ExtTimeBase|P9221_TimeBaseSRC_GPI1;
    //U16 TrigCtrl = P9221_TRGMOD_ExtD|P9221_TRGSRC_GPI2;
    U16 ConfigCtrl = P9221_AI_SingEnded;
    U16 TrigCtrl = P9221_TRGMOD_SOFT;
    BOOLEAN AutoResetBuf = TRUE;
    U32 ScanIntrv = 160;
    U32 SampIntrv = 160;
    U16 Buffer[ReadCount];
    U16 Channel = 0;
    U16 AdRange = AD_B_5_V;
    U16 BufferId;
    F64 voltage = 0.0;
//    FILE* fout;
    int i;
    U8 *FileName = "acq";
/*    
    if(!(fout=fopen("acq_data","w"))){
        printf("fopen Failed\n");
    }
*/
    memset(Buffer, '\0', ReadCount*2);
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err = AI_SetTimeOut(card, 1000);
    if(err < 0){
        printf("AI_SetTimeOut error: %d\n", err);
        Release_Card(card);
	exit(1);
    }

    err = AI_9221_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err < 0){
        printf("AI_9221_CounterInterval error: %d\n", err); 
        Release_Card(card);
	exit(1);
    }

    err = AI_9221_Config(card, ConfigCtrl, TrigCtrl, AutoResetBuf);
    if(err < 0){
        printf("AI_9221_Config error: %d\n", err);
        Release_Card(card);
	exit(1);
    }
   
    //printf("[APP] Virtual Address of Setup Buffer :0x%x\n", Buffer);
    err = AI_ContBufferSetup(card, Buffer, ReadCount, &BufferId);
    if(err < 0){
        printf("AI_ContBufferSetup error: %d\n", err);
        Release_Card(card);
	exit(1);
    }

    err = AI_ContReadChannelToFile(card, Channel, AdRange, FileName, ReadCount, 0.0, SYNCH_OP);    
    if(err < 0){
        printf("AI count Read Channels to file error\n");
        Release_Card(card);
	exit(1);
    }
    
    if(!AutoResetBuf){
        err = AI_ContBufferReset(card);
        if(err < 0){
            printf("AI_ContBufferReset error: %d\n", err);
            Release_Card(card);
	    exit(1);
	}
    }
/*   
    for(i=0; i<ReadCount; i++){
        err = AI_VoltScale(card, AdRange, Buffer[i], &voltage);
	if(err < 0)
	    printf("AI_VoltScale Failed\n");
	fprintf(fout,"%f\n", (float)voltage);
    }

    fclose(fout);
*/  Release_Card(card);
    printf("Press any key to exit...\n");
    getch();
    
    return 0;
}
