// Google Drive Link: https://drive.google.com/drive/folders/1HPhBOxAP3ckbQmNRpMdoPkE6WgGYMeFk?usp=sharing
#include <pthread.h>
#include <stdio.h>

//global variables
#define NUM_BUFFER     5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//storage where producer stores produced item and consumers consumes item from it
int buffer[NUM_BUFFER] = {NULL, NULL, NULL, NULL, NULL};
//to track the index of item
int bufferIndex = 0;
int empty = NUM_BUFFER;
int full = 0;
int numberOfItem = 0;

// consumer consumes item
void *consume(void *threadId){
    int repeat;                 //process for each thread repeats as specified 
    long tid;                   //threadIndex
    tid = (long)threadId;

    for(repeat = 5; repeat > 0; repeat--){
        pthread_mutex_lock(&mutex);
        if(full > 0){
            printf("Consumer %d\t consumed %d at %d\n", tid, buffer[bufferIndex -1], bufferIndex - 1);
            buffer[bufferIndex - 1] = NULL;
            bufferIndex--;
            empty++;
            full--;
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

// producer produces item
void *produce(void *threadId){
    int repeat;             //process for each thread repeats as specified 
    long tid;               //threadIndex
    tid = (long)threadId;

    for(repeat = 5; repeat > 0; repeat--){
        pthread_mutex_lock(&mutex);
        if(empty > 0){
            buffer[bufferIndex] = numberOfItem;
            printf("Producer %d\t produced %d at %d\n", tid, numberOfItem, bufferIndex);
            numberOfItem++;
            bufferIndex++;
            empty--;
            full++;
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main (int argc, char *argv[]){
    long t;
    int numberOfProducer;
    int numberOfConsumer;
    
    // User input for the number of consumer and producer
    printf("Enter the number of producer and consumer: ");
    scanf("%d %d",&numberOfProducer, &numberOfConsumer);

    // pthread of user input size for producer and consumer
    pthread_t producer[numberOfProducer];
    pthread_t consumer[numberOfConsumer];    

    for(t = 0; t < numberOfProducer && t < numberOfConsumer; t++){
        pthread_create(&producer[t], NULL, produce, (void*)t);
        pthread_create(&consumer[t], NULL, consume, (void*)t);
    }

    /* Last thing that main() function should do */
    pthread_mutex_destroy(&mutex);          //free mutex object
    pthread_exit(NULL);                     //
}