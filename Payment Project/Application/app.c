#define _CRT_SECURE_NO_WARNINGS

#include "app.h"

ST_transaction_t tran = { 0 };
ST_accountsDB_t accountRefrence[255] = {0};
uint32_t number = 1;

void appStart(void)
{
	strcpy(accountRefrence[number].primaryAccountNumber, "352351256123267591");
	//accountRefrence.state = BLOCKED;
	getCardHolderName(&tran.cardHolderData);
	getCardExpiryDate(&tran.cardHolderData);
	getCardPAN(&tran.cardHolderData);
	getTransactionDate(&tran.terminalData);
	if (isCardExpired(tran.cardHolderData, tran.terminalData) == EXPIRED_CARD)
		printf("Declined Expired Card\n");
	else
	{
		if(isValidCardPAN(&tran.cardHolderData) == INVALID_CARD)
			printf("Declined Invalid Primary Account Number\n");
		else
		{
			getTransactionAmount(&tran.terminalData);
			setMaxAmount(&tran.terminalData);
			if (isBelowMaxAmount(&tran.terminalData) == EXCEED_MAX_AMOUNT)
				printf("Declined Amount Exceeding Limit\n");
			else
			{
				if (isValidAccount(&tran.cardHolderData, &accountRefrence[number]) == ACCOUNT_NOT_FOUND)
					printf("Declined Invalid Account\n");
				else
				{
					if (isBlockedAccount(&accountRefrence[number]) == BLOCKED_ACCOUNT)
						printf("Stolen Card\n");
					else
					{
						accountRefrence[number].balance = 10000;
						if (isAmountAvailable(&tran.terminalData, &accountRefrence[number]) == LOW_BALANCE)
							printf("Declined Insuffecient funds\n");
						else
						{
							printf("Balance Before Transaction Was %f\n", accountRefrence[number].balance);
							accountRefrence[number].balance -= tran.terminalData.transAmount;
							printf("Balance After Transaction is %f\n", accountRefrence[number].balance);
							tran.transactionSequenceNumber = number++;
							recieveTransactionData(&tran);
						}
					}
				}
			}
		}
	}
	
	//getCardExpiryDateTest();
	//getCardPANTest();
	//getTransactionDateTest();
	//isCardExpriedTest();
	//isValidCardPANTest();
	//getTransactionAmountTest();
	//isBelowMaxAmountTest();
	//setMaxAmountTest();
	//recieveTransactionDataTest();
	//isValidAccountTest();
	//isBlockedAccountTest();
	//isAmountAvailableTest();
	//saveTransactionTest();
	//listSavedTransactionsTest();
}