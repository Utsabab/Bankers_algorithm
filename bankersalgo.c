#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

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
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int request[]);
int is_request_approved(int request[], int id);
int is_request_safe();
void timestamp();

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

    int i;
    // for (i=0;i<NUMBER_OF_RESOURCES;i++) {
    //     printf("%d \n", need[param][i]);
    // }
    bool retry = false;
    int request[NUMBER_OF_RESOURCES];
    while (1) {       
        if (!retry) {
           for (i=0;i<NUMBER_OF_RESOURCES;i++) {
                if (need[param][i] == 0) {
                    request[i] = 0;
                }
                else {
                    request[i] = rand() % (need[param][i] + 1);
                }    
            } 
        }

            pthread_mutex_lock(&mutex);
            timestamp();
            printf("customer= %d, ", param);
            for (i=0;i<NUMBER_OF_RESOURCES;i++) {
                printf("%d, ", request[i]);
            }
            printf("\n");
            if (request_resources(param, request) == 0) {
                timestamp();
                printf("customer= %d, Request satisfied.\n", param);
                int count = 0;
                for (i=0;i<NUMBER_OF_RESOURCES;i++) {
                    if (need[param][i] == 0) {
                        count = count + 1;
                    }
                }
                if (count == 3) {
                    printf ("Is it oka?");
                    int random_time = rand() % 100000;
                    usleep(random_time);
                    release_resources(param, request);
                    timestamp();
                    printf("customer= %d, Resources released.\n", param);
                }
                else {
                        int random_time = rand() % 1000;
                        usleep(random_time);
                    }           
            }
            else {
                int random_time = rand() % 1000;
                usleep(random_time);
                retry = true;
                timestamp();
                printf("customer= %d, Request denied.\n", param);
            }
    pthread_mutex_unlock(&mutex);
    }
        
        return NULL;
}


int is_request_approved(int request[], int id) {
    printf ("id: %d", id);
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

    if (is_request_safe() == 0) {
        return 0;
    }

    else {
        for (i=0;i<NUMBER_OF_RESOURCES;i++) {
            available[i] = available[i] + request[i];
            allocation[id][i] = allocation[id][i] - request[i];
            need[id][i] = need[id][i] + request[i];
        }
        int random_time = rand() % 10000;
        usleep(random_time);
        return 1;
    }
}

int is_request_safe() {
    int finish[NUMBER_OF_CUSTOMERS]; 
    int work[NUMBER_OF_RESOURCES];
    int i;
    int j;
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        finish[i] = 0;
    }

    for (i=0;i<NUMBER_OF_RESOURCES;i++) {
        work[i] = available[i];
    }
    
    bool found = false;
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        if (finish[i] == 0) {
            for (j=0;j<NUMBER_OF_RESOURCES;j++) {
                if (need[i][j] > work[j])
                    break;
            }
            if (j == NUMBER_OF_RESOURCES) {
                for (int k=0;k<NUMBER_OF_RESOURCES;k++) {
                    work[k] += allocation[i][k]; 
                }            
                finish[i] = 1;
                found = true;
            }
        }
    }
    if (found == false) {
        return 1;
    }
    return 0;
}

int request_resources(int customer_num, int request[]) {
    printf("Customernum: %d", customer_num);
    int i;
    for (i=0;i<NUMBER_OF_RESOURCES;i++) {
        if (request[i] > need[customer_num][i]) {
            printf("Error!! Request is greater than need");
        }
    }
    int return_is_approved = is_request_approved(request, customer_num);
    return return_is_approved;
}

int release_resources(int customer_num, int request[]) {
    int i;
    for (i = 0;i<NUMBER_OF_RESOURCES;i++) {
        available[i] = available[i] + allocation[customer_num][i];
        allocation[customer_num][i] = 0;
        int t = rand() % (available[i] + 1);
        maximum[customer_num][i] = t;
        need[customer_num][i] = maximum[customer_num][i] - allocation[customer_num][i];
    }
    return 0;
}

void timestamp()
{
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    printf("%s",asctime( localtime(&ltime) ) );
}