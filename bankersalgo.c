#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

pthread_t* threads;

int main(int argc, char *argv[]) {
	int i;
    int j;
	if(argc<=NUMBER_OF_RESOURCES) {
        printf("You did not feed me arguments, I will die now :( ...");
        exit(1);
    }  

    else {
    	for (i=0;i<NUMBER_OF_RESOURCES;i++) {
    		available[i] = atoi(argv[i+1]);
        }
    }	

    srand(time(NULL));
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        for (j=0;j<NUMBER_OF_RESOURCES;j++) {
            int t = rand() % available[j];
            maximum[i][j] = t;
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j] - allocation[i][j];
            printf("%d \n", maximum[i][j], allocation[i][j], need[i][j]);
        }
    }
    return 0;
}