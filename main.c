#include"stdio.h"
#include "math.h"
#include"Transaction.h"


EN_transStat en_transStat ;




int main(){

    ptrSt_transaction = ST_transactionArray;
    char yOrNLerrer[2];



    while(1){


        fillCardData(ptrSt_transaction);

        fillTerminalData(ptrSt_transaction);

        /************** checks the transaction state before sending to server ***************************/

        if (en_transStat==DECLINED){
            printf("Do you want to continue (y/n)?: \n");
            gets(yOrNLerrer);

            if(yOrNLerrer[0] == 'n'){
                return 0;
            }
            else {
                continue;
            }
        }


        /**************** send data to the server******************************/

        saveTransactionIntoServer(ptrSt_transaction);

        printf("Do you want to continue (y/n)?: \n");
        gets(yOrNLerrer);

        if(yOrNLerrer[0] == 'n'){
            return 0;
        }


    }
return 0;
}


