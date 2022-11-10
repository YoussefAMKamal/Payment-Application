#define _CRT_SECURE_NO_WARNINGS

#include "server.h"

ST_accountsDB_t account_Data[255] = { 0 };
ST_transaction_t trans_Data[255] = { 0 };
uint8_t state[20];
uint8_t store = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&transData->cardHolderData, &account_Data) == ACCOUNT_NOT_FOUND)
		return FRAUD_CARD;
	if (isAmountAvailable(&transData->terminalData, &account_Data) == LOW_BALANCE)
		return DECLINED_INSUFFECIENT_FUND;
	if(isBlockedAccount(&account_Data) == BLOCKED_ACCOUNT)
		return DECLINED_STOLEN_CARD;
	if (saveTransaction(transData) == SAVING_FAILED)
		return INTERNAL_SERVER_ERROR;
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	for(int i=0;i<255;i++)
	{
		if (accountRefrence[i].primaryAccountNumber != NULL)
		{
			if (strcmp(accountRefrence[i].primaryAccountNumber, cardData->primaryAccountNumber) == 0)
				store = 1;
		}
	}
	if (store == 1)
	{
		store = 0;
		return SERVER_OK;
	}
	else
	{
		//printf("Error: Account Not Found\n");
		return ACCOUNT_NOT_FOUND;
	}
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if(accountRefrence->state == RUNNING)
		return SERVER_OK;
	else
	{
		//printf("Error: Account Is Blocked");
		return BLOCKED_ACCOUNT;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	if (termData->transAmount > accountRefrence->balance)
	{
		//printf("Error: Amount In Balance Is Not Enough\n");
		return LOW_BALANCE;
	}
	else
		return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	if (transData->transactionSequenceNumber > 0 && transData->transactionSequenceNumber < 255)
	{
		trans_Data->cardHolderData = transData->cardHolderData;
		trans_Data->terminalData = transData->terminalData;
		trans_Data->transState = transData->transState;
		trans_Data->transactionSequenceNumber = transData->transactionSequenceNumber;
		listSavedTransactions();
		printf("Transaction Saved Successfully\n");
		return SERVER_OK;
	}
	else
	{
		printf("Error: Saving Failed No Data Received\n");
		return SAVING_FAILED;
	}
}
	
void listSavedTransactions(void)
{
	for(int i=0;i<255;i++)
	{
		if(trans_Data[i].transactionSequenceNumber < 255)
		{
			if (trans_Data[i].transactionSequenceNumber != NULL)
			{
				if (trans_Data[i].transState == DECLINED_STOLEN_CARD)
					strcpy(state, "DECLINED_STOLEN_CARD");
				else if (trans_Data[i].transState == DECLINED_INSUFFECIENT_FUND)
					strcpy(state, "DECLINED_INSUFFECIENT_FUND");
				else if (trans_Data[i].transState == FRAUD_CARD)
					strcpy(state, "FRAUD_CARD");
				else if (trans_Data[i].transState == INTERNAL_SERVER_ERROR)
					strcpy(state, "INTERNAL_SERVER_ERROR");
				else
					strcpy(state, "APPROVED");
				printf("\t\t#########################\n\t\tTransaction Sequence Number: %d\
			\n\t\tTransaction Date: %s\n\t\tTransaction Amount: %f\n\t\tTranstion State: %s\
			\n\t\tTerminal Max Amount: %f\n\t\tCardholder Name: %s\n\t\tPAN: %s\
			\n\t\tCard Expiration Date: %s\n\t\t#########################\n",
					trans_Data[i].transactionSequenceNumber, trans_Data[i].terminalData.transactionDate,
					trans_Data[i].terminalData.transAmount, state, trans_Data[i].terminalData.maxTransAmount,
					trans_Data[i].cardHolderData.cardHolderName, trans_Data[i].cardHolderData.primaryAccountNumber,
					trans_Data[i].cardHolderData.cardExpirationDate);
			}
		}
	}
}

void recieveTransactionDataTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : recieveTransactionData \n");

	strcpy(account_Data->primaryAccountNumber, "12345678901234567");
	account_Data->state = RUNNING;
	account_Data->balance = 3000.0;

	ST_transaction_t tD;

	printf("Test Case 1:\n\
	Input Data: No input data\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(tD.cardHolderData.cardExpirationDate, "\0");
	strcpy(tD.cardHolderData.cardHolderName, "\0");
	strcpy(tD.cardHolderData.primaryAccountNumber, "\0");
	strcpy(tD.terminalData.transactionDate, "\0");
	tD.terminalData.maxTransAmount = 0.0;
	tD.terminalData.transAmount = 0.0;
	tD.transactionSequenceNumber = NULL;

	recieveTransactionData(&tD);

	printf("Test Case 2:\n\
	Input Data: \n\
	\tCard Holder Name: Youssef Ahmed Mohamed\n\
	\tCard Exp. Date: 05/22\n\
	\tCard PAN: 12345678901234567 (As in DB & it isn't blocked)\n\
	\tTransaction Amount: 2000 (Less than blance)\n\
	\tMax Amount: 5000\n\
	\tTransaction Date: 16/03/2022\n\
	\tTransaction Seq. no.: 1\n\
	Expected Result: No Error\n\
	Actual Result: \n");
	strcpy(tD.cardHolderData.cardExpirationDate, "05/22");
	strcpy(tD.cardHolderData.cardHolderName, "Youssef Ahmed Mohamed");
	strcpy(tD.cardHolderData.primaryAccountNumber, "12345678901234567");
	strcpy(tD.terminalData.transactionDate, "16/03/2022");
	tD.terminalData.maxTransAmount = 5000.0;
	tD.terminalData.transAmount = 2000.0;
	tD.transactionSequenceNumber = 1;

	recieveTransactionData(&tD);

	printf("Test Case 3:\n\
	Input Data: \n\
	\tCard Holder Name: Youssef Ahmed Mohamed\n\
	\tCard Exp. Date: 05/22\n\
	\tCard PAN: 15349445848148523 (Not as in DB & it isn't blocked)\n\
	\tTransaction Amount: 2000 (Less than blance)\n\
	\tMax Amount: 5000\n\
	\tTransaction Date: 16/03/2022\n\
	\tTransaction Seq. no.: 2\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(tD.cardHolderData.cardExpirationDate, "05/22");
	strcpy(tD.cardHolderData.cardHolderName, "Youssef Ahmed Mohamed");
	strcpy(tD.cardHolderData.primaryAccountNumber, "15349445848148523");
	strcpy(tD.terminalData.transactionDate, "16/03/2022");
	tD.terminalData.maxTransAmount = 5000.0;
	tD.terminalData.transAmount = 2000.0;
	tD.transactionSequenceNumber = 2;

	recieveTransactionData(&tD);

	printf("Test Case 4:\n\
	Input Data: \n\
	\tCard Holder Name: Youssef Ahmed Mohamed\n\
	\tCard Exp. Date: 05/22\n\
	\tCard PAN: 12345678901234567 (As in DB & it isn't blocked)\n\
	\tTransaction Amount: 4000 (More than blance)\n\
	\tMax Amount: 5000\n\
	\tTransaction Date: 16/03/2022\n\
	\tTransaction Seq. no.: 3\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(tD.cardHolderData.cardExpirationDate, "05/22");
	strcpy(tD.cardHolderData.cardHolderName, "Youssef Ahmed Mohamed");
	strcpy(tD.terminalData.transactionDate, "16/03/2022");
	strcpy(tD.cardHolderData.primaryAccountNumber, "12345678901234567");
	tD.terminalData.maxTransAmount = 5000.0;
	tD.terminalData.transAmount = 4000.0;
	tD.transactionSequenceNumber = 3;

	recieveTransactionData(&tD);

	printf("Test Case 5:\n\
	Input Data: \n\
	\tCard Holder Name: Youssef Ahmed Mohamed\n\
	\tCard Exp. Date: 05/22\n\
	\tCard PAN: 12345678901234567 (As in DB & it is blocked)\n\
	\tTransaction Amount: 2000 (Less than blance)\n\
	\tMax Amount: 5000\n\
	\tTransaction Date: 16/03/2022\n\
	\tTransaction Seq. no.: 4\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(tD.cardHolderData.cardExpirationDate, "05/22");
	strcpy(tD.cardHolderData.cardHolderName, "Youssef Ahmed Mohamed");
	strcpy(tD.cardHolderData.primaryAccountNumber, "12345678901234567");
	strcpy(tD.terminalData.transactionDate, "16/03/2022");
	tD.terminalData.maxTransAmount = 5000.0;
	tD.terminalData.transAmount = 2000.0;
	tD.transactionSequenceNumber = 4;

	account_Data->state = BLOCKED;
	recieveTransactionData(&tD);
}

void isValidAccountTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : isValidAccount \n");
	ST_cardData_t cardData;
	ST_accountsDB_t acc;

	printf("Test Case 1:\n\
	Input Data: CardPAN is in DB \n\
	Expected Result: No Error\n\
	Actual Result: ");
	strcpy(cardData.primaryAccountNumber, "1234");
	strcpy(acc.primaryAccountNumber, "1234");
	isValidAccount(&cardData, &acc);

	printf("\nTest Case 2:\n\
	Input Data: CardPAN isn't in DB \n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.primaryAccountNumber, "1234");
	strcpy(acc.primaryAccountNumber, "1534");
	isValidAccount(&cardData, &acc);
}

void isBlockedAccountTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : isBlockedAccount \n");
	ST_accountsDB_t acc;

	printf("Test Case 1:\n\
	Input Data: Not Blocked \n\
	Expected Result: No Error\n\
	Actual Result: ");
	acc.state = RUNNING;
	isBlockedAccount(&acc);

	printf("\nTest Case 2:\n\
	Input Data: Blocked \n\
	Expected Result: Error\n\
	Actual Result: ");
	acc.state = BLOCKED;
	isBlockedAccount(&acc);
}

void isAmountAvailableTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : isAmountAvailable \n");
	ST_terminalData_t term;
	ST_accountsDB_t acc;

	printf("Test Case 1:\n\
	Input Data: transAmount = 1234 & Balance  = 12345 \n\
	Expected Result: No Error\n\
	Actual Result: ");
	term.transAmount = 1234;
	acc.balance = 12345;
	isAmountAvailable(&term, &acc);

	printf("\nTest Case 2:\n\
	Input Data: transAmount = 12345 & Balance  = 1234 \n\
	Expected Result: Error\n\
	Actual Result: ");
	term.transAmount = 12345;
	acc.balance = 1234;
	isAmountAvailable(&term, &acc);
}

void saveTransactionTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : saveTransaction \n");
	ST_transaction_t tran;
	
	printf("Test Case 1:\n\
	Input Data: No input data \n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(tran.cardHolderData.cardExpirationDate, "\0");
	strcpy(tran.cardHolderData.cardHolderName, "\0");
	strcpy(tran.cardHolderData.primaryAccountNumber, "\0");
	strcpy(tran.terminalData.transactionDate, "\0");
	tran.terminalData.maxTransAmount = 0;
	tran.terminalData.transAmount = 0;
	tran.transactionSequenceNumber = NULL;
	saveTransaction(&tran);

	printf("Test Case 2:\n\
	Input Data: \n\
	\tCard Holder Name: Youssef Ahmed Mohamed\n\
	\tCard Exp. Date: 05/22\n\
	\tCard PAN: 12345678901234567\n\
	\tTransaction Amount: 2000\n\
	\tMax Amount: 5000\n\
	\tTransaction Date: 16/03/2022\n\
	\tTransaction Seq. no.: 10\n\
	\tTransaction State: Approved\n\
	Expected Result: Print them all\n\
	Actual Result: \n");
	strcpy(tran.cardHolderData.cardExpirationDate, "05/22");
	strcpy(tran.cardHolderData.cardHolderName, "Youssef Ahmed Mohamed");
	strcpy(tran.cardHolderData.primaryAccountNumber, "12345678901234567");
	tran.terminalData.maxTransAmount = 5000.0;
	strcpy(tran.terminalData.transactionDate, "16/03/2022");
	tran.terminalData.transAmount = 2000.0;
	tran.transactionSequenceNumber = 10;
	tran.transState = APPROVED;
	saveTransaction(&tran);
}

void listSavedTransactionsTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : listSavedTransactions \n");

	printf("Test Case 1:\n\
	Input Data: No input data \n\
	Expected Result: Nothing will be printed\n\
	Actual Result: \n");
	strcpy(trans_Data[0].cardHolderData.cardExpirationDate, "\0");
	strcpy(trans_Data[0].cardHolderData.cardHolderName, "\0");
	strcpy(trans_Data[0].cardHolderData.primaryAccountNumber, "\0");
	trans_Data[0].terminalData.maxTransAmount = 0;
	strcpy(trans_Data[0].terminalData.transactionDate, "\0");
	trans_Data[0].terminalData.transAmount = 0;
	trans_Data[0].transactionSequenceNumber = NULL;

	listSavedTransactions();

	printf("Test Case 2:\n\
	Input Data: \n\
	  Transaction 1:\n\
	\tCard Holder Name: Youssef Ahmed Mohamed\n\
	\tCard Exp. Date: 05/22\n\
	\tCard PAN: 12345678901234567\n\
	\tTransaction Amount: 2000\n\
	\tMax Amount: 5000\n\
	\tTransaction Date: 16/03/2022\n\
	\tTransaction Seq. no.: 10\n\
	\tTransaction State: Approved\n\
	  Transaction 2:\n\
	\tCard Holder Name: Ahmed Mohamed Kamal\n\
	\tCard Exp. Date: 11/22\n\
	\tCard PAN: 19826465127865427\n\
	\tTransaction Amount: 500\n\
	\tMax Amount: 10000\n\
	\tTransaction Date: 28/08/2022\n\
	\tTransaction Seq. no.: 20\n\
	\tTransaction State: DECLINED_STOLEN_CARD\n\
	Expected Result: Print them all\n\
	Actual Result: \n");
	strcpy(trans_Data[0].cardHolderData.cardExpirationDate, "05/22");
	strcpy(trans_Data[0].cardHolderData.cardHolderName, "Youssef Ahmed Mohamed");
	strcpy(trans_Data[0].cardHolderData.primaryAccountNumber, "12345678901234567");
	trans_Data[0].terminalData.maxTransAmount = 5000.0;
	strcpy(trans_Data[0].terminalData.transactionDate, "16/03/2022");
	trans_Data[0].terminalData.transAmount = 2000.0;
	trans_Data[0].transactionSequenceNumber = 10;
	trans_Data[0].transState = APPROVED;

	strcpy(trans_Data[1].cardHolderData.cardExpirationDate, "11/22");
	strcpy(trans_Data[1].cardHolderData.cardHolderName, "Ahmed Mohamed Kamal");
	strcpy(trans_Data[1].cardHolderData.primaryAccountNumber, "19826465127865427");
	trans_Data[1].terminalData.maxTransAmount = 10000.0;
	strcpy(trans_Data[1].terminalData.transactionDate, "28/08/2022");
	trans_Data[1].terminalData.transAmount = 500.0;
	trans_Data[1].transactionSequenceNumber = 20;
	trans_Data[1].transState = DECLINED_STOLEN_CARD;

	listSavedTransactions();
}