#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define max 10

static  pthread_key_t key;
static pthread_once_t once = PTHREAD_ONCE_INIT;

typedef struct thread_data {                 
   pthread_t id;
   char message [20];
} data;

void printMessage(){
   int i;
   srand(time(NULL)); 
   int number = rand() % 10 + 1;  
   for (i = 0; i < number; i++){
   		int random = rand() % 10 + 1;
   		printf("%s%lu.Ітерація: %d. Номер: %d.\n" ,((data*)pthread_getspecific(key))->message,((data*)pthread_getspecific(key))->id , i + 1, random);				
   		sleep(1);
   }
}

static void once_creator(void) {
   pthread_key_create( &key, NULL);
}

void * print(void * arg){
	int value = *(int*) arg;
    pthread_once(&once, once_creator);
    pthread_setspecific(key, malloc(sizeof(data)));
    data *thr = pthread_getspecific(key); 
    thr->id = pthread_self();
    char *mesg = "Потік з id: ";
    strcpy(thr->message, mesg);
    printMessage();
}


int main(int argc, char **argv){
pthread_t *pt;
	if (argc == 1){
		printf("Виконана спроба запуску програми без аргументів...\n");
		printf("Для справної роботи необхідно записати цілочисельний аргумент...\n");
	}else{
		int i;
		int number = atoi(argv[1]);
		pt = malloc (sizeof(pthread_t) * number);

		for (i = 0; i < number; ++i){
			if (pthread_create(&pt[i], NULL, &print, &i) != 0) {
				fprintf(stderr, "Error (thread)\n");
				return 1;
			}	
		}
		for(i = 0; i < number; ++i){
			if (pthread_join(pt[i], NULL) == 0) {
				printf("Thread %d finished sucessfully.\n", i);		
			}
		}
	}
	free(pt);
	return 0;
}
