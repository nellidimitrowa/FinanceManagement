#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TYPE_LEN 15
#define COST_TYPE_LEN 10
#define MAX_DATE_LEN 10
#define STORAGE_PATH "D:\\WORD\\UNI\\semester6\\SPr\\TASK\\FinanceManager\\FinanceManagement\\Storage\\"

char *costType[COST_TYPE_LEN] = {"car", "electricity", "water", "heatingSystem", "phone", "tv", "shopping", "food", "hobby", "rent"};

int findElement(char element[]) {
	for(int i = 0; i < COST_TYPE_LEN; i++) {
		if(strcmp(element, costType[i]) == 0) {
			return(i);
		}
	}
	return(-1);
}

char  *addCost() {
	char type[MAX_TYPE_LEN];
	double price;
	char date[MAX_DATE_LEN];

	printf("Type: ");
	scanf("%s", type);

	int index = findElement(type);
	if(index == -1) {
		printf("Wrong type of the cost.\n");
		return("fail");
	} 

	printf("Price: ");
	scanf("%lf", &price);
	printf("Date: ");
	scanf("%s", date);

	char stringPrice[10];
	sprintf(stringPrice, "%.2lf", price);

	char *cost = malloc(strlen(type) + strlen(stringPrice) + strlen(date) + 1);
	strcpy(cost, type);
	strcat(cost, "\n");
	strcat(cost, stringPrice);
	strcat(cost, "\n");
	strcat(cost, date);
	strcat(cost, "\n");

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

void saveFile() {
	char *filename = fileName();
	char *filePath = malloc(strlen(STORAGE_PATH) + strlen(filename) + strlen(".txt") + 1);
	strcpy(filePath, STORAGE_PATH);
	strcat(filePath, filename);
	strcat(filePath, ".txt");
	FILE *f = fopen(filePath, "w");

	const char *text = addCost();
	fprintf(f, "%s", text); 

	fclose(f);
}

void choiceAction(int choice) {
	if (choice == 1) {
		printf("You chose 1.\n");
		saveFile();
	} else if (choice == 2) {
		printf("You chose 2.\n");
	} else if(choice == 3) {
		printf("You chose 3.\n");
	} else {
		printf("Wrong operation.\nTry again.\n");
	}
}
void menu() {
	int choice;
	do {
		printf("\n");
		printf("┌──┬─────────────╢MENU╟─────────────────┐\n");
		printf("│1.│ Enter the cost of a new month      │\n");
		printf("│2.│ Review the cost of a previous month│\n");
		printf("│3.│ Exit                               │\n");
		printf("└──┴────────────────────────────────────┘\n");
		printf("Enter choice: ");
		scanf("%d", &choice);
		choiceAction(choice);
	} while(choice != 3);
}


int main(int argc, char *argv[]) {
	menu();
	return 0;
}

