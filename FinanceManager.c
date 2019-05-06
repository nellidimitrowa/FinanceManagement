#include<stdio.h>

void choiceAction(int choice) {
	if (choice == 1) {
		printf("You chose 1.\n");
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

