#define _CRT_SECURE_NO_WARNINGS

#include "app.h"

ST_transaction_t tran = { 0 };
ST_accountsDB_t accountRefrence = { 0 };

void appStart(void)
{
	strcpy(accountRefrence.primaryAccountNumber, "352351256123267591");
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
				if (isValidAccount(&tran.cardHolderData, &accountRefrence) == DECLINED_STOLEN_CARD)
					printf("Declined Invalid Account\n");
				else
				{
					if (isBlockedAccount(&accountRefrence) == BLOCKED_ACCOUNT)
						printf("Stolen Card\n");
					else
					{
						accountRefrence.balance = 10000;
						if (isAmountAvailable(&tran.terminalData, &accountRefrence) == LOW_BALANCE)
							printf("Declined Insuffecient funds\n");
						else
						{
							accountRefrence.balance -= tran.terminalData.transAmount;
							recieveTransactionData(&tran);
							saveTransaction(&tran);
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
