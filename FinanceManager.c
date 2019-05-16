#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FALSE 0
#define TRUE 1
#define MAX_TYPE_LEN 15
#define costStructYPE_LEN 10
#define MAX_DATE_LEN 10
#define STORAGE_PATH "D:\\WORD\\UNI\\semester6\\SPr\\TASK\\FinanceManager\\FinanceManagement\\Storage\\"
char *costType[costStructYPE_LEN] = {"car", "electricity", "water", "heatingSystem", "phone", "tv", "shopping", "food", "hobby", "rent"};

int addCost();
int findCostByType(char type[]);
int priceValidation(double price);
char *getFileName();
char *getFilePath();
void printAllCostsForCurrentMonth();
void choiceAction(int choice);
void menu();

typedef struct Cost
{
	char type[MAX_TYPE_LEN];
	double price;
	char date[MAX_DATE_LEN];
}costStruct;


int main(int argc, char *argv[]) {
	menu();
	return 0;
}


void menu() {
	int choice;
	do {
		printf("\n");
		printf("┌──┬──────────────╢MENU╟──────────────────┐\n");
		printf("│1.│ Add cost                             │\n");
		printf("│2.│ Review the cost of the current month │\n");
		printf("│3.│ Add cost for the previous month      │\n");
		printf("│4.│ Review the cost of the previous month│\n");
		printf("│5.│ Review the cost of the last 12 months│\n");
		printf("│6.│ Exit                                 │\n");
		printf("└──┴──────────────────────────────────────┘\n");
		printf("Enter choice: ");
		scanf("%d", &choice);
		choiceAction(choice);
	} while(choice != 6);
}


void choiceAction(int choice) {
	if (choice == 1) {
		addCost();
	} else if (choice == 2) {
		printAllCostsForCurrentMonth();
	} else if(choice == 3) {
		printf("You chose 3.\n");
	} else if(choice == 4) {
		printf("You chose 4.\n");
	}  else if(choice == 5) {
		printf("You chose 5.\n");
	} else if(choice == 6) {
		printf("You chose 6.\n");
	}else {
		printf("Wrong operation.\nTry again.\n");
	}
}


int addCost() {
	costStruct cost;
	printf("Type: ");
	scanf("%s", cost.type);

	int index = findCostByType(cost.type);
	if(index == -1) {
		printf("Wrong type of the cost.\n");
		return FALSE;
	}

	printf("Price: ");
	scanf("%lf", &cost.price);

	int checkPrice = priceValidation(cost.price);
	if(checkPrice == FALSE) {
		printf("The price cannot be less than zero.\n");
		return FALSE;
	}

	printf("Date: ");
	scanf("%s", cost.date);

	char *filePath = getFilePath();
	int fileDescriptor = open(filePath, O_WRONLY | O_CREAT | O_APPEND, 0644);
    write(fileDescriptor, &cost, sizeof(costStruct));

    close(fileDescriptor);
	return TRUE;
}


int findCostByType(char type[]) {
	for(int i = 0; i < costStructYPE_LEN; i++) {
		if(strcmp(type, costType[i]) == 0) {
			return(i);
		}
	}
	return(-1);
}


int priceValidation(double price) {
	if(price < 0) {
		return FALSE;
	}
	return TRUE;
}


char *getFileName() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *months[12] = {"january", "february", "march", "april", "may", "june", 
				"july", "august", "september", "october", "november", "december"};
	int year = tm.tm_year + 1900;

	char stringYear[4];
	sprintf(stringYear, "%d", year);

	char *filename = malloc(strlen(months[tm.tm_mon]) + 5);
	strcpy(filename, months[tm.tm_mon]);
    strcat(filename, stringYear);
    return filename;
}


char *getFilePath() {
	char *filename = getFileName();
	char *filePath = malloc(strlen(STORAGE_PATH) + strlen(filename) + strlen(".txt") + 1);
	strcpy(filePath, STORAGE_PATH);
	strcat(filePath, filename);
	strcat(filePath, ".txt");

	return filePath;
}


void printAllCostsForCurrentMonth() {
	costStruct result;
	char *filePath = getFilePath();
    int fileDescriptor = open(filePath, O_RDONLY);
    int readret;

    printf("\nCOST\tPRICE\tDATE\n");
    while((readret = read(fileDescriptor, &result, sizeof(costStruct))) > 0) {
    	printf("%s\t", result.type);
    	printf("%.2f\t", result.price);
    	printf("%s\n", result.date);
    }
    close(fileDescriptor);
}