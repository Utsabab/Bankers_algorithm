include <stdio.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int available [NUMBER_OF_RESOURCES];
int maximum [NUMBER_OF_CUSTOMERS] [NUMBER_OF_RESOURCES];
int allocation [NUMBER_OF_CUSTOMERS] [NUMBER_OF_RESOURCES];
int need [NUMBER_OF_CUSTOMERS] [NUMBER_OF_RESOURCES];

pthread_t* threads;

int main(int argc, char *argv[]) {
	if(argc<=1) {
        printf("You did not feed me arguments, I will die now :( ...");
        exit(1);
    }  
	
}

need matrix, allocation matrix, available matrix, max matrix, request 