#include "Transaction.h"

/***************************** data types *****************************/

extern en_transStat; // global variable used as a flag to change the state of transaction

static ST_accountBalance st_accountBalance [] = {{"123456789","0100.00"},
                                                 {"234567891","6000.00"},
                                                 {"567891234","3250.25"},
                                                 {"456789123","1500.12"},
                                                 {"258649173","0500.00"},
                                                 {"654823719","2100.00"},
                                                 {"971362485","0000.00"},
                                                 {"793148625","0001.00"},
                                                 {"123123456","0010.12"},
                                                 {"456789321","0000.55"},
                                                };


typedef enum EN_SearchStat  //used in searching  of the account number
{
	NOTFOUND,
	FOUND
}EN_SearchStat;


static char Founded_Account_Indexed; //used to access the balance of the founded account


static char receiptReferenceNumber=0;

//extern ptrSt_transaction;
/*******************************************************************************************************/



/******************* convert only one character to int  ********************/
static int ConvCharToInt(char CHAR){
    return (CHAR - '0');
}



/*************************convert string to int ************************************/
static int ConvStringToInt(char * ptr,int exponent){
    int i,total_number=0,number;
    int digit = (int) pow((double) 10,exponent);
    for(i=0;i<exponent;i++){
        number = ConvCharToInt(*(ptr+i));
        total_number += number * digit ;
        digit *=.100000;
    }
    return total_number;
}



/******************** convert the amount string to float ****************/
static float ConvAmountToInt(char *ptr ){
    float total_number=0;
    int number=0;
    float digit=1000;
    int i;
    for(i = 0;i<7;i++){
        if (i==4){
            continue;
        }
        number = ConvCharToInt(*(ptr+i));
        total_number += number * digit ;
        digit *=.100000;
    }
    return total_number ;
}


/*****************receives the data of the card **********************/

void fillCardData(ST_transaction *transaction){

    en_transStat = APPROVED; // init transState as approve every new loop
    printf("Please Enter Card Data:\n");
    printf("Please Enter the Card Holder Name:\n");
    gets(transaction->cardHolderData.cardHolderName);

    printf("Please Enter the Primary Account Number:\n");
    gets(transaction->cardHolderData.primaryAccountNumber);

    printf("Please card Expiry Date:\n");
    gets(transaction->cardHolderData.cardExpirationDate);
}




/******* receives the data from terminal and check the amount and the expiry date *************/

static void checkAmount(ST_transaction *transaction){


    float Amount = ConvAmountToInt(transaction->transData.transAmount);
    transaction->transData.maxTransAmount[0]="5000.00";

    if (Amount >5000.00){
        printf("The transaction is DECLINED\n");
        en_transStat=DECLINED;
    }

}



static void checkExpiryDate(ST_transaction *transaction){
    int expire_date_year,expire_date_month,transaction_date_year,transaction_date_month;



    expire_date_year        =   ConvStringToInt((transaction->cardHolderData.cardExpirationDate)+3,2);
    expire_date_month       =   ConvStringToInt((transaction->cardHolderData.cardExpirationDate)+0,2);
    transaction_date_year   =   ConvStringToInt((transaction->transData.transactionDate)+8,2);
    transaction_date_month  =   ConvStringToInt((transaction->transData.transactionDate)+3,2);

    if (expire_date_year < transaction_date_year ){
        printf("The transaction is DECLINED\n");
        en_transStat=DECLINED;
    }
    else {
        if(expire_date_month < transaction_date_month ){
            printf("The transaction is DECLINED\n");
            en_transStat=DECLINED;
        }
    }

}


void fillTerminalData(ST_transaction *transaction){

    printf("Please Enter Terminal Data:\n");
    printf("Please Enter the transaction Amount:\n");
    gets(transaction->transData.transAmount);

    checkAmount(transaction);

    if (en_transStat==APPROVED){

        printf("Please Enter transaction Date:\n");
        gets(transaction->transData.transactionDate);

        checkExpiryDate(transaction);
    }
}


/***************** check the balance and save the transaction into server **********************/

//this function searches if the account is in the table of the server or not

static EN_SearchStat search_account(ST_transaction *transaction){
        int counter1,counter2;
    EN_SearchStat EN_searchStat = NOTFOUND;

    for(counter1 = 0 ; counter1 <10;counter1 ++){

        for(counter2=0;counter2<9;counter2 ++){

            if( transaction->cardHolderData.primaryAccountNumber[counter2] !=  (st_accountBalance+counter1)->primaryAccountNumber[counter2] ){
                break;
            }
            if (counter2 == 8){
                EN_searchStat = FOUND;
                Founded_Account_Indexed=counter1;
            }

        }
    }
    if (EN_searchStat == NOTFOUND){
        en_transStat=DECLINED;
    }
    return  EN_searchStat ;
}




static void checkBalance(ST_transaction *transaction){
    float account_balance,trans_amount;

    if(search_account(transaction)){

            trans_amount    = ConvAmountToInt(transaction->transData.transAmount);
            account_balance = ConvAmountToInt((st_accountBalance+Founded_Account_Indexed)->balance);


            if (trans_amount>account_balance){
                en_transStat=DECLINED;
            }
    }

}




void saveTransactionIntoServer(ST_transaction *transaction){
    printf("Verifying Data from the server . . .  \n");

    checkExpiryDate(transaction);

    if(en_transStat == APPROVED){
        checkBalance(transaction);
    }


// here we save the transaction by increasing the pointer to the next empty place

    ptrSt_transaction->transResponse.transactionStatus=en_transStat;
    ptrSt_transaction->transResponse.receiptReferenceNumber=receiptReferenceNumber;

    receiptReferenceNumber++;
    ptrSt_transaction++;

   /* int i ;
    for(i=0;i<receiptReferenceNumber;i++){
    printf("saved name : %s\n",(ST_transactionArray+i)->cardHolderData.cardHolderName);}*/ //uses this loop to show saved names

    if(en_transStat == APPROVED){
        printf("The transaction is APPROVED\n");

    }
    else{
        printf("The transaction is DECLINED\n");
    }
}



