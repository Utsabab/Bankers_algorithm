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
int* threadid;
pthread_mutex_t mutex;

void* customer(void* id);

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

    pthread_mutex_init(&mutex, NULL);
    threads = (pthread_t *) malloc(NUMBER_OF_CUSTOMERS * sizeof(pthread_t));
    threadid = (int *) malloc(NUMBER_OF_CUSTOMERS * sizeof(int));

    srand(time(NULL));
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        for (j=0;j<NUMBER_OF_RESOURCES;j++) {
            int t = rand() % available[j];
            maximum[i][j] = t;
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }


    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        threadid[i] = i;
        pthread_create(&threads[i], NULL, customer, (void *) (&threadid[i]));
    }

    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        pthread_join(threads[i], NULL);
    }


    return 0;
}

void *customer(void *id) {
    int param = *((int *) id);
    srand(time(NULL)); 

    pthread_mutex_lock(&mutex);

    printf("\n customer= %d \n", param);
    int i;
    // for (i=0;i<NUMBER_OF_RESOURCES;i++) {
    //     printf("%d \n", need[param][i]);
    // }

    int request[NUMBER_OF_RESOURCES];
    for (i=0;i<NUMBER_OF_RESOURCES;i++) {
        request[i] = rand() % (need[param][i] + 1);
    }
   

    pthread_mutex_unlock(&mutex);
}

int is_request_approved(int request[] ,int id) {
    int i;
    for (i=0;i<NUMBER_OF_RESOURCES;i++) {
        if (available[i] < request[i]) {
            return 1;
        }
    }
    for (i=0;i<NUMBER_OF_RESOURCES;i++) {
        available[i] = available[i] - request[i];
        allocation[id][i] = allocation[id][i] + request[i];
        need[id][i] = need[id][i] - request[i];
    }

    if (is_request_safe == 0) {

    }
}

int is_request_safe() {
    int i;
    int j;
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        int count = 0;
        for (j=0;j<NUMBER_OF_RESOURCES;j++) {
            if (need[i][j] <= available[j] ) {
                count = count + 1;
            }
        }
        if (count == NUMBER_OF_RESOURCES) {
            return 0;
        }
    }
    return 1;
}