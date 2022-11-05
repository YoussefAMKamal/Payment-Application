#define _CRT_SECURE_NO_WARNINGS

#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	printf("Please enter card's holder name: ");
	scanf("%[^\n]s", &cardData->cardHolderName);
	if (strlen(cardData->cardHolderName) > 24 || strlen(cardData->cardHolderName) < 20 || cardData->cardHolderName == NULL)
	{
		printf("Error: Name must be from 20 to 24 characters\n");
		return WRONG_NAME;
	}
	else
		return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	//printf("Please enter the card expiry date: ");
	//scanf("%s", &cardData->cardExpirationDate);
	if (strlen(cardData->cardExpirationDate) != 5 || cardData->cardExpirationDate == NULL || 
		cardData->cardExpirationDate[2] != '/' || isdigit(cardData->cardExpirationDate[0]) == 0 ||
		isdigit(cardData->cardExpirationDate[1]) == 0 || isdigit(cardData->cardExpirationDate[3]) == 0 || 
		isdigit(cardData->cardExpirationDate[4]) == 0 || CardDate(&cardData->cardExpirationDate) == WRONG_EXP_DATE)
	{
		printf("Error: Wrong expiration date format\n");
		return WRONG_EXP_DATE;
	}
	else
		return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	printf("Please enter the card's primary account number: ");
	scanf("%s", &cardData->primaryAccountNumber);
	if (strlen(cardData->primaryAccountNumber) > 19 || strlen(cardData->primaryAccountNumber) < 16 || cardData->primaryAccountNumber == NULL)
	{
		printf("Error: Number must be from 16 to 19 digits\n");
		return WRONG_PAN;
	}
	else
		return CARD_OK;
}

EN_cardError_t CardDate(uint8_t* Data)
{
	if ((Data[0] - '0') * 10 + (Data[1] -'0') > 12)
		return WRONG_EXP_DATE;
	else
		return CARD_OK;
}

void getCardHolderNameTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : getCardHolderName\n");
	ST_cardData_t cardData;

	printf("Test Case 1: \n\
	Input Data: Youssef Ahmed\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardHolderName , "Youssef Ahmed");
	getCardHolderName(&cardData);

	printf("Test Case 2: \n\
	Input Data: Youssef Ahmed Mohamed\n\
	Expected Result: No Error\n\
	Actual Result: ");
	strcpy(cardData.cardHolderName, "Youssef Ahmed Mohamed");
	getCardHolderName(&cardData);

	printf("\nTest Case 3: \n\
	Input Data: Youssef Ahmed Mohamed Kamal\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardHolderName, "Youssef Ahmed Mohamed kamal");
	getCardHolderName(&cardData);

}

void getCardExpiryDateTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : getCardExpiryDate \n");\
	ST_cardData_t cardData;

	printf("Test Case 1: \n\
	Input Data: asfds\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "asfds");
	getCardExpiryDate(&cardData);

	printf("Test Case 2: \n\
	Input Data: 20/55\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "20/55");
	getCardExpiryDate(&cardData);

	printf("Test Case 3: \n\
	Input Data: 05/22\n\
	Expected Result: No Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "05/22");
	getCardExpiryDate(&cardData);

	printf("\nTest Case 4: \n\
	Input Data: 05122\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "05122");
	getCardExpiryDate(&cardData);

	printf("Test Case 5: \n\
	Input Data: 05/022\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "05/022");
	getCardExpiryDate(&cardData);

	printf("Test Case 6: \n\
	Input Data: 0f/66\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "0f/66");
	getCardExpiryDate(&cardData);

	printf("Test Case 7: \n\
	Input Data: 0/66\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "0/66");
	getCardExpiryDate(&cardData);

	printf("Test Case 8: \n\
	Input Data: f2/66\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "f2/66");
	getCardExpiryDate(&cardData);

	printf("Test Case 9: \n\
	Input Data: 06/t6\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "06/t6");
	getCardExpiryDate(&cardData);

	printf("Test Case 10: \n\
	Input Data: 05/6h\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.cardExpirationDate, "05/6h");
	getCardExpiryDate(&cardData);
	exit();
}

void getCardPANTest(void)
{
	printf("Tester Name: Youssef Ahmed Mohamed\n\
	Function Name : getCardPAN \n"); \
	ST_cardData_t cardData;
	
	printf("Test Case 1: \n\
	Input Data: 0123456789\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.primaryAccountNumber, "0123456789");
	getCardPAN(&cardData);

	printf("Test Case 2: \n\
	Input Data: 01234567890123456\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.primaryAccountNumber, "01234567890123456");
	getCardPAN(&cardData);

	printf("\nTest Case 3: \n\
	Input Data: 01234567890123456789\n\
	Expected Result: Error\n\
	Actual Result: ");
	strcpy(cardData.primaryAccountNumber, "01234567890123456789");
	getCardPAN(&cardData);
}