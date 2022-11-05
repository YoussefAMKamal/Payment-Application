#define _CRT_SECURE_NO_WARNINGS

#include "terminal.h"

int luhn[19], store, t, u, sum = 0, i = 0, l;
uint8_t date[5]={0}, c_y[2] = {0}, t_y[2] = {0};

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	printf("Please enter transcation data: ");
	scanf("%s", &termData->transactionDate);
	if (termData->transactionDate == NULL || strlen(termData->transactionDate) < 10 ||
		termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/' ||
		isdigit(termData->transactionDate[0]) == 0 || isdigit(termData->transactionDate[1]) == 0 ||
		isdigit(termData->transactionDate[3]) == 0 || isdigit(termData->transactionDate[4]) == 0 || 
		isdigit(termData->transactionDate[6]) == 0 || isdigit(termData->transactionDate[7]) == 0 ||
		isdigit(termData->transactionDate[8]) == 0 || isdigit(termData->transactionDate[9]) == 0 ||
		TerminalDate(&termData->transactionDate) == WRONG_DATE)
	{
		printf("Error: Wrong Transaction Date\n");
		return WRONG_DATE;
	}
	else
		return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	for (int i=0;i<5;i++)
		if (i < 3)
			date[i] = termData.transactionDate[i + 3];
		else
			date[i] = termData.transactionDate[i + 5];
	for (int i = 0; i < 2; i++)
	{
		c_y[i] = cardData.cardExpirationDate[i + 3];
		t_y[i] = date[i + 3];
	}
	if(strcmp(c_y,t_y) < 0)
	{
		//printf("Error: Expired Card\n");
		return EXPIRED_CARD;
	}
	else if (strcmp(cardData.cardExpirationDate, date) <= 0)
	{
		//printf("Error: Expired Card\n");
		return EXPIRED_CARD;
	}
	else
		return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	l = strlen(cardData->primaryAccountNumber);
	sum = 0; i = 0;
	for (int n = 0; n < l ; n++)
	{
		luhn[n] = (cardData->primaryAccountNumber[n] - '0');
	}
	while(i < l)
	{
		if ((i + 1) % 2 == 0)
		{
			store = luhn[l - (i + 1)] * 2;
			if (store > 9)
			{
				u = store % 10;
				t = store / 10;
				store = t + u;
			}
			sum += store;
		}
		else
			sum += luhn[l - (i + 1)];
		i++;
	}
	if (sum % 10 != 0)
	{
		//printf("Error: Not Valid Card PAN\n");
		return INVALID_CARD;
	}
	else
		return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("Please enter transcation amount: ");
	scanf("%f", &termData->transAmount);
	if (termData->transAmount <= 0)
	{
		printf("Error:Invalid Amount\n");
		return INVALID_AMOUNT;
	}
	else
		return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
	{
		//printf("Error: Exceeded max amount\n");
		return EXCEED_MAX_AMOUNT;
	}
	else
		return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	printf("Please enter transcation max amount: ");
	scanf("%f", &termData->maxTransAmount);
	if (termData->maxTransAmount <= 0)
	{
		printf("Error: Invalid Max Amount\n");
		return INVALID_MAX_AMOUNT;
	}
	else
		return TERMINAL_OK;
}

EN_terminalError_t TerminalDate(uint8_t* T_Data)
{
	if ((T_Data[3] - '0') * 10 + (T_Data[4] - '0') > 12)
		return WRONG_DATE;
	if ((T_Data[3] - '0') * 10 + (T_Data[4] - '0') == 2 &&
		(T_Data[0] - '0') * 10 + (T_Data[1] - '0') > 28)
		return WRONG_DATE;
	if ((T_Data[3] - '0') + (T_Data[4] - '0') == (4 | 6 | 9 | 11) &&
		(T_Data[0] - '0') * 10 + (T_Data[1] - '0') > 30)
		return WRONG_DATE;
	if ((T_Data[0] - '0') * 10 + (T_Data[1] - '0') > 31)
		return WRONG_DATE;
	return TERMINAL_OK;
}

void getTransactionDateTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : getTransactionDate \n"); 
	ST_terminalData_t termData;

	printf("Test Case 1:\n\
	Input Data: 45/12/2022\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(termData.transactionDate, "45/12/2022");
	getTransactionDate(&termData);

	printf("Test Case 2:\n\
	Input Data: 22/22/2022\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(termData.transactionDate, "22/22/2022");
	getTransactionDate(&termData);

	printf("Test Case 3:\n\
	Input Data: 22/22/022\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(termData.transactionDate, "22/22/022");
	getTransactionDate(&termData);

	printf("Test Case 4:\n\
	Input Data: 22/12/2022\n\
	Expected Result: No Error\n\
	Actual Result: ");
	strcpy(termData.transactionDate, "22/12/2022");
	getTransactionDate(&termData);

	printf("\nTest Case 5:\n\
	Input Data: 22/1t/2022\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(termData.transactionDate, "22/1t/2022");
	getTransactionDate(&termData);
}

void isCardExpriedTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : isCardExpried \n"); \
	ST_cardData_t cardData;
	ST_terminalData_t termData;

	printf("Test Case 1:\n\
	Input Data: card(10/22) , trans(20/10/2022)\n\
	Expected Result: No Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "10/22");
	strcpy(termData.transactionDate, "20/10/2022");
	isCardExpired(cardData, termData);

	printf("\nTest Case 2:\n\
	Input Data: card(11/22) , trans(20/10/2022)\n\
	Expected Result: No Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "11/22");
	strcpy(termData.transactionDate, "20/10/2022");
	isCardExpired(cardData, termData);

	printf("\nTest Case 3:\n\
	Input Data: card(10/22) , trans(20/11/2022)\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "10/22");
	strcpy(termData.transactionDate, "20/11/2022");
	isCardExpired(cardData, termData);

	printf("Test Case 4:\n\
	Input Data: card(11/20) , trans(20/10/2022)\n\
	Expected Result: No Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "11/20");
	strcpy(termData.transactionDate, "20/10/2022");
	isCardExpired(cardData, termData);
}

void isValidCardPANTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : isValidCardPAN \n"); \
	ST_cardData_t cardData;

	printf("Test Case 1:\n\
	Input Data: 352351256123267591 \n\
	Expected Result: No Error\n\
	Actual Result: ");
	strcpy(cardData.primaryAccountNumber, "352351256123267591");
	isValidCardPAN(&cardData);

	printf("\nTest Case 2:\n\
	Input Data: 6589512875395654\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.primaryAccountNumber, "6589512875395654");
	isValidCardPAN(&cardData);
}

void getTransactionAmountTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : getTransactionAmount \n"); \
	ST_terminalData_t termData;

	printf("Test Case 1:\n\
	Input Data: 0.0 \n\
	Expected Result: Error\n\
	Actual Result: ");
	termData.transAmount = 0.0;
	getTransactionAmount(&termData);

	printf("Test Case 2:\n\
	Input Data: 300.0 \n\
	Expected Result: No Error\n\
	Actual Result: ");
	termData.transAmount = 300.0;
	getTransactionAmount(&termData);

	printf("\nTest Case 3:\n\
	Input Data: -200.0 \n\
	Expected Result: No Error\n\
	Actual Result: ");
	termData.transAmount = -200.0;
	getTransactionAmount(&termData);
}

void isBelowMaxAmountTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : isBelowMaxAmount \n"); \
	ST_terminalData_t termData;

	printf("Test Case 1:\n\
	Input Data: trans(200.0), maxtrans(1000.0) \n\
	Expected Result: No Error\n\
	Actual Result: ");
	termData.transAmount = 200.0;
	termData.maxTransAmount = 1000.0;
	isBelowMaxAmount(&termData);

	printf("\nTest Case 2:\n\
	Input Data: trans(1000.0), maxtrans(200.0) \n\
	Expected Result: Error\n\
	Actual Result: ");
	termData.transAmount = 1000.0;
	termData.maxTransAmount = 200.0;
	isBelowMaxAmount(&termData);
}

void setMaxAmountTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : setMaxAmountTest \n"); \
	ST_terminalData_t termData;

	printf("Test Case 1:\n\
	Input Data: 1000.0 \n\
	Expected Result: No Error\n\
	Actual Result: ");
	termData.maxTransAmount = 1000.0;
	setMaxAmount(&termData);

	printf("\nTest Case 2:\n\
	Input Data: 0.0 \n\
	Expected Result: Error\n\
	Actual Result: ");
	termData.maxTransAmount = 0.0;
	setMaxAmount(&termData);
}