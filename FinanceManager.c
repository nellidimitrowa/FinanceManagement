#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TYPE_LEN 15
#define COST_TYPE_LEN 10
#define MAX_DATE_LEN 10
char *costType[COST_TYPE_LEN] = {"car", "electricity", "water", "heatingSystem", "phone", "tv", "shopping", "food", "hobby", "rent"};

int findElement(char element[]) {
	for(int i = 0; i < COST_TYPE_LEN; i++) {
		if(strcmp(element, costType[i]) == 0) {
			return(i);
		}
	}
	return(-1);
}

void addCosts() {
	char type[MAX_TYPE_LEN];
	double price;
	char date[MAX_DATE_LEN];

	printf("Type: ");
	scanf("%s", type);
	printf("Price: ");
	scanf("%lf", &price);
	printf("Date: ");
	scanf("%s", date);

	printf("\n");
	int index = findElement(type);
	if(index == -1) {
		printf("Wrong type of the cost.\n");
	} else {
		printf("Type: %s\n", costType[index]);
		printf("Price: %lf\n", price);
		printf("Date: %s\n", date);
	}
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
	char *storagePath = "D:\\WORD\\UNI\\semester6\\SPr\\TASK\\FinanceManager\\FinanceManagement\\Storage\\";
	char *filename = fileName();
	char *filePath = malloc(strlen(storagePath) + strlen(filename) + strlen(".txt") + 1);
	strcpy(filePath, storagePath);
	strcat(filePath, filename);
	strcat(filePath, ".txt");
	FILE *f = fopen(filePath, "w");

	const char *text = "Some text";
	fprintf(f, "Text: %s", text); 

	fclose(f);
}

void choiceAction(int choice) {
	if (choice == 1) {
		printf("You chose 1.\n");
		addCosts();
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

