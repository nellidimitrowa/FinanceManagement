#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_TYPE_LEN 15
#define costStructYPE_LEN 10
#define MAX_DATE_LEN 10
#define STORAGE_PATH "D:\\WORD\\UNI\\semester6\\SPr\\TASK\\FinanceManager\\FinanceManagement\\Storage\\"

char *costType[costStructYPE_LEN] = {"car", "electricity", "water", "heatingSystem", "phone", "tv", "shopping", "food", "hobby", "rent"};

typedef struct Cost
{
	char type[MAX_TYPE_LEN];
	double price;
	char date[MAX_DATE_LEN];
}costStruct;

int findElement(char element[]) {
	for(int i = 0; i < costStructYPE_LEN; i++) {
		if(strcmp(element, costType[i]) == 0) {
			return(i);
		}
	}
	return(-1);
}

struct Cost addCost() {
	costStruct cost;
	printf("Type: ");
	scanf("%s", cost.type);

	int index = findElement(cost.type);
	// if(index == -1) {
	// 	printf("Wrong type of the cost.\n");
	// 	return;
	// }

	printf("Price: ");
	scanf("%lf", &cost.price);
	printf("Date: ");
	scanf("%s", cost.date);

	return cost;
}


char *fileName() {
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

char *filePath() {
	char *filename = fileName();
	char *filePath = malloc(strlen(STORAGE_PATH) + strlen(filename) + strlen(".txt") + 1);
	strcpy(filePath, STORAGE_PATH);
	strcat(filePath, filename);
	strcat(filePath, ".txt");

	return filePath;
}

void saveFile() {
	int fileDescriptor = open(filePath, O_WRONLY | O_CREAT | O_APPEND, 0644);

    costStruct cost = addCost();
    write(fileDescriptor, &cost, sizeof(costStruct));
}


void choiceAction(int choice) {
	if (choice == 1) {
		saveFile();
	} else if (choice == 2) {
		printf("You chose 2.\n");
	} else if(choice == 3) {
		printf("You chose 3.\n");
	} else if(choice == 4) {
		printf("You chose 4.\n");
	} else {
		printf("Wrong operation.\nTry again.\n");
	}
}
void menu() {
	int choice;
	do {
		printf("\n");
		printf("┌──┬──────────────╢MENU╟──────────────────┐\n");
		printf("│1.│ Enter the cost of a new month        │\n");
		printf("│2.│ Review the cost of a previous month  │\n");
		printf("│3.│ Review the cost of the last 12 months│\n");
		printf("│4.│ Exit                                 │\n");
		printf("└──┴──────────────────────────────────────┘\n");
		printf("Enter choice: ");
		scanf("%d", &choice);
		choiceAction(choice);
	} while(choice != 4);
}

int main(int argc, char *argv[]) {
	menu();
	return 0;
}