#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__


/***************************** data types ***************************/


typedef struct ST_terminalData
{
        unsigned char transAmount[8];
		unsigned char maxTransAmount[8];
		unsigned char transactionDate[11];
	}ST_terminalData;


typedef enum EN_transStat
{
	DECLINED,
	APPROVED
}EN_transStat;


typedef struct ST_cardData
{
	unsigned char cardHolderName[25];
	unsigned char primaryAccountNumber[9];
	unsigned char cardExpirationDate[6];
}ST_cardData;



typedef struct ST_accountBalance
{
    unsigned char primaryAccountNumber[9];
	unsigned char balance[8];
}ST_accountBalance;



typedef struct ST_serverData
{
        EN_transStat transactionStatus;
        unsigned int receiptReferenceNumber;
	}ST_serverData;



typedef struct ST_transaction
{
	ST_cardData cardHolderData;
	ST_terminalData transData;
	ST_serverData transResponse;
}ST_transaction;




ST_transaction *ptrSt_transaction ; // pointer to empty place in the array of transactions and increases every saved one
ST_transaction ST_transactionArray [255];


/************************************************************************************/













/******************************prototypes*********************************************/

void fillCardData(ST_transaction *transaction);
void fillTerminalData(ST_transaction *transaction);
void saveTransactionIntoServer(ST_transaction *transaction);



/**************************************************************************************/
#endif
