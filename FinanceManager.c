#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/types.h> 
#include<sys/wait.h>


#define FALSE 0
#define TRUE 1
#define MAX_TYPE_LEN 15
#define COST_TYPE_LEN  10
#define MAX_DATE_LEN 10
#define CURRENT_MONTH 0
#define PREVIOUS_MONTH -1
#define STORAGE_PATH "D:\\WORD\\UNI\\semester6\\SPr\\TASK\\FinanceManager\\FinanceManagement\\Storage\\"
char *costType[COST_TYPE_LEN] = {"car", "electricity", "water", "pets", "phone", "tv", "shopping", "food", "hobby", "rent"};

typedef struct Cost {
	char type[MAX_TYPE_LEN];
	double price;
	char date[MAX_DATE_LEN];
}costStruct;

int userInput();
int addCost(int isPreviousMonth);
int typeValidation(char type[]);
int priceValidation(double price);
int dateValidation(char date[], int isPreviousMonth);
char *getFileName(int isPreviousMonth);
char *getFilePath(int isPreviousMonth);
void printCosts(int isPreviousMonth);
void choiceAction(int choice);
void updateCost(costStruct cost, int isPreviousMonth);
int findCostByType(char type[], int isPreviousMonth);
double totalPrice(costStruct cost, char *filePath);
int menu();



int main(int argc, char *argv[]) {
	int fd[2];
	int choice;


	while (1) {
		if (pipe(fd) == -1) {
			fprintf(stderr, "Pipe failed\n");
			return 1;
		}

		pid_t pid = fork();
		if (pid < 0) {
			fprintf(stderr, "fork Failed\n");
		} else if (pid > 0) {
			close(fd[0]);
			choice = menu();
			write(fd[1], &choice, sizeof(choice));
			close(fd[1]);

			wait(NULL);
		} else {
			close(fd[1]);

			read(fd[0], &choice, sizeof(choice));
			choiceAction(choice);
			if(choice == 6) {
				kill(pid, SIGKILL);
			}

			close(fd[0]);
		}
	}
	return 0;
}


int menu() {
	int choice;
	printf("\n");
	printf("┌──┬──────────────╢MENU╟──────────────────┐\n");
	printf("│1.│ Add cost to the current month        │\n");
	printf("│2.│ Review the cost of the current month │\n");
	printf("│3.│ Add cost to the previous month       │\n");
	printf("│4.│ Review the cost of the previous month│\n");
	printf("│5.│ Review the cost of the last 12 months│\n");
	printf("│6.│ Exit                                 │\n");
	printf("└──┴──────────────────────────────────────┘\n");
	printf("Enter choice: ");
	scanf("%d", &choice);
	return choice;
}


void choiceAction(int choice) {
	if (choice == 1) {
		addCost(CURRENT_MONTH);
	} else if (choice == 2) {
		printCosts(CURRENT_MONTH);
	} else if(choice == 3) {
		addCost(PREVIOUS_MONTH);
	} else if(choice == 4) {
		printCosts(PREVIOUS_MONTH);
	}  else if(choice == 5) {
		printf("You chose 5.\n");
	} else if(choice == 6) {
		printf("Bye.\n");
	} else {
		printf("Wrong operation.\nTry again.\n");
	}
}


int addCost(int isPreviousMonth) {
	costStruct cost;

	printf("Type: ");
	scanf("%s", cost.type);
	int index = typeValidation(cost.type);
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

	printf("Date [dd.mm.yyyy]: ");
	scanf("%s", cost.date);
	int checkDate = dateValidation(cost.date, isPreviousMonth);
	if(checkDate == FALSE) {
		printf("The date format is wrong\n");
		return FALSE;
	}

	char *filePath = getFilePath(isPreviousMonth);
	int fileDescriptor = open(filePath, O_WRONLY | O_CREAT | O_APPEND, 0644);

	int res;
	if((res = findCostByType(cost.type, isPreviousMonth)) == TRUE) {
		printf("HERE I AM!\n");
		updateCost(cost, isPreviousMonth);
	} else {
		write(fileDescriptor, &cost, sizeof(costStruct));
	}

	close(fileDescriptor);
	return TRUE;
}


int typeValidation(char type[]) {
	for(int i = 0; i < COST_TYPE_LEN ; i++) {
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


int dateValidation(char date[], int isPreviousMonth) {
	int number = 0;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	if(strlen(date) < 10 || strlen(date) > 10) {
		return FALSE;
	}

	number = date[0] - '0';
	if(number < 0 || number > 3) {
		return FALSE;
	}

	number = date[3] - '0';
	if(number < 0 || number > 1) {
		return FALSE;
	}

	number = date[4] - '0';
	if((isPreviousMonth == CURRENT_MONTH && number != tm.tm_mon+1) || 
		(isPreviousMonth == PREVIOUS_MONTH && number != tm.tm_mon)) {
		return FALSE;
	}

	number = date[6] - '0';
	if(number != 2) {
		return FALSE;
	} 

	number = date[7] - '0';
	if (number != 0) {
		return FALSE;
	}

	number = date[8] - '0';
	if (number != 1) {
		return FALSE;
	}

	return TRUE;	
}


char *getFileName(int isPreviousMonth) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char *months[12] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", 
					"october", "november", "december"};

	int year = tm.tm_year + 1900;

	char stringYear[4];
	sprintf(stringYear, "%d", year);

	char *filename = malloc(strlen(months[tm.tm_mon]) + 5);
	if (isPreviousMonth == PREVIOUS_MONTH) {
		strcpy(filename, months[tm.tm_mon-1]);
	} else {
		strcpy(filename, months[tm.tm_mon]);
	}
    strcat(filename, stringYear);
    return filename;
}


char *getFilePath(int isPreviousMonth) {
	char *filename = getFileName(isPreviousMonth);
	char *filePath = malloc(strlen(STORAGE_PATH) + strlen(filename) + strlen(".txt") + 1);
	strcpy(filePath, STORAGE_PATH);
	strcat(filePath, filename);
	strcat(filePath, ".txt");

	return filePath;
}


void printCosts(int isPreviousMonth) {
	costStruct result;
	char *filePath = getFilePath(isPreviousMonth);
    int fileDescriptor = open(filePath, O_RDONLY);
    int readret;

    printf("\nCOST\t\tPRICE\t\tDATE\n");
    while((readret = read(fileDescriptor, &result, sizeof(costStruct))) > 0) {
    	printf("%s\t\t", result.type);
    	printf("%.2f\t\t", result.price);
    	printf("%s\n", result.date);
    }
    printf("Total price for this month: %.2lf\n", totalPrice(result, filePath));
    close(fileDescriptor);
}

void updateCost(costStruct cost, int isPreviousMonth) {
	costStruct element;
    int found;
	char *filePath  = getFilePath(isPreviousMonth);
    int fileDescriptor = open(filePath, O_RDONLY);
    char *tmpFilePath = malloc(strlen(STORAGE_PATH) + strlen("tmp.txt") + 1);
    strcpy(tmpFilePath, STORAGE_PATH);
    strcat(tmpFilePath, "tmp.txt");
    int tmpFileDescriptor = open(tmpFilePath, O_WRONLY | O_CREAT);

    int readret;
    while((readret = read(fileDescriptor, &element, sizeof(costStruct))) > 0) {
    	if (strcmp(cost.type, element.type) == 0) {
    		element.price = element.price + cost.price;
    		strcpy(element.date, cost.date);
    	}
    	write(tmpFileDescriptor, &element, sizeof(costStruct));
    }
    close(fileDescriptor);
    close(tmpFileDescriptor);
    remove(filePath);
    rename(tmpFilePath, filePath);
}

int findCostByType(char type[], int isPreviousMonth) {
	costStruct element;

	char *filePath = getFilePath(isPreviousMonth);
    int fileDescriptor = open(filePath, O_RDONLY);


	int readret;
    while((readret = read(fileDescriptor, &element, sizeof(costStruct))) > 0) {
    	if (strcmp(type, element.type) == 0) {
    		return TRUE;
    	}
    }
    close(fileDescriptor);
    return FALSE;
}

double totalPrice(costStruct cost, char *filePath) {
	double sum = 0;
    int fileDescriptor = open(filePath, O_RDONLY);

    int readret;
    while((readret = read(fileDescriptor, &cost, sizeof(costStruct))) > 0) {
    	sum = sum + cost.price;
    }

    return sum;
}