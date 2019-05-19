#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/types.h> 
#include<sys/wait.h>
#include <dirent.h>


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
int updateCost(costStruct cost, int isPreviousMonth);
int findCostByType(char type[], int isPreviousMonth);
void totalPrice(costStruct cost, char *filePath);
void maxPrice(costStruct cost, char *filePath);
void deleteFile(char month[], int year);
void printAllCosts();
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
			return 1;
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
		printAllCosts();
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
		updateCost(cost, isPreviousMonth);
	} else {
		if(write(fileDescriptor, &cost, sizeof(costStruct)) < 0) {
			printf("Something went wring.\n");
			return FALSE;
		}
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
	int lastyear = tm.tm_year + 1900 -1;

	char stringYear[4];
	sprintf(stringYear, "%d", year);
	
	deleteFile(months[tm.tm_mon], lastyear);

	char *filename = malloc(strlen(months[tm.tm_mon]) + 5);
	if (isPreviousMonth == PREVIOUS_MONTH) {
		strcpy(filename, months[tm.tm_mon-1]);
	} else {
		strcpy(filename, months[tm.tm_mon]);
	}
	strcat(filename, stringYear);
    return filename;
}


void deleteFile(char month[], int year) {
	char stringLastYear[4];
	sprintf(stringLastYear, "%d", year);

	char *filename = malloc(strlen(month) + 5);
	strcpy(filename, month);
	strcat(filename, stringLastYear);

	char *lastYearPath = malloc(strlen(STORAGE_PATH) + strlen(filename) + strlen(".txt") + 1);
	strcpy(lastYearPath, STORAGE_PATH);
	strcat(lastYearPath, filename);
	strcat(lastYearPath, ".txt");
	remove(lastYearPath);
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
    totalPrice(result, filePath);
    maxPrice(result, filePath);
    close(fileDescriptor);
}


int updateCost(costStruct cost, int isPreviousMonth) {
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
    	if(write(tmpFileDescriptor, &element, sizeof(costStruct)) < 0) {
    		printf("Something went wrong;\n");
    		return FALSE;
    	}
    }
    close(fileDescriptor);
    close(tmpFileDescriptor);
    remove(filePath);
    rename(tmpFilePath, filePath);
    return TRUE;
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


void totalPrice(costStruct cost, char *filePath) {
	double sum = 0;
    int fileDescriptor = open(filePath, O_RDONLY);

    int readret;
    while((readret = read(fileDescriptor, &cost, sizeof(costStruct))) > 0) {
    	sum = sum + cost.price;
    }
    printf("This month your total pay: %.2lf lv\n", sum);
}


void maxPrice(costStruct cost, char *filePath) {
	double max = 0;
	char type[MAX_TYPE_LEN];
	int fileDescriptor = open(filePath, O_RDONLY);

	int readret;
    while((readret = read(fileDescriptor, &cost, sizeof(costStruct))) > 0) {
    	if(cost.price > max) {
    		max = cost.price;
    		strcpy(type, cost.type);
    	}
    }
    printf("You gave the most money for the: %s - %.2lf lv\n", type, max);
}


void printAllCosts() {
	int fileCount = 0;
	DIR *dir;
	struct dirent *entry;
	costStruct cost;
	int readret;
	dir = opendir(STORAGE_PATH);

	while ((entry = readdir(dir)) != NULL) {
		printf("\n%s", entry->d_name);
		printf("\nCOST\t\tPRICE\t\tDATE\n");
    	char *filePath = malloc(strlen(STORAGE_PATH) + strlen(entry->d_name) + 1);
    	strcpy(filePath, STORAGE_PATH);
    	strcat(filePath, entry->d_name);
	    int fileDescriptor = open(filePath, O_RDONLY);
	    while((readret = read(fileDescriptor, &cost, sizeof(costStruct))) > 0) {
	    	printf("%s\t\t", cost.type);
	    	printf("%.2f\t\t", cost.price);
	    	printf("%s\n", cost.date);
    	}
    	totalPrice(cost, filePath);
	    maxPrice(cost, filePath);
	    close(fileDescriptor);
	}
	closedir(dir);
}