#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

enum costType{car, electricity, water, heatingSystem, phone, tv, shopping, food, hobby, rent};

struct Cost {
	costType type;
	double price;
	char *date[10];
}

char *fileName() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *months[12] = {"january", "february", "march", "april", "may", "june", 
				"july", "august", "september", "october", "november", "december"};
	int year = tm.tm_year + 1900;

	char stringYear[32];
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

	const char *text = "Write this to the file";
	fprintf(f, "Some text: %s\n", text);

	fclose(f);
}


// TODO : enterNewCosts()
// 		: checkFilename()

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


int main() {
	menu();
	return 0;
}

