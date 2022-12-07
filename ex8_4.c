#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define n 100000

void * run_pi(void * arg){
	int t = *(int*) arg;
	int old_cancel_state;
	double i;
	double m = 1;
	double sum = 0;
	double x = -1;
	for (i = 0; i < n; i++){
		pthread_setcancelstate(PTHREAD_CANCEL_DEFERRED, &old_cancel_state);
		x *= -1;
		sum += x*(1/m);
		m += 2;
		printf("Поточне значення: %f; Поточна ітерація %.0f\n", sum*4, (i+1));
		pthread_setcancelstate(old_cancel_state, NULL);
	}
	printf("Кінцеве значення: %f\n", sum*4);
}

int main(int argc, char **argv){
	if (argc == 1){
		printf("Виконана спроба запуску програми без аргументів...\n");
		printf("Для справної роботи необхідно записати цілочисельний аргумент...\n");
	}else{
		pthread_t thread;
		int a = atoi(argv[1]);
		if (pthread_create(&thread, NULL, &run_pi, &a) != 0) {
				fprintf(stderr, "Error (thread1)\n");
				return 1;
		}
		sleep(atoi(argv[1]));
		pthread_cancel(thread);
		void *result;
		if (!pthread_equal(pthread_self(), thread))
				pthread_join(thread, &result) ;
		if (result == PTHREAD_CANCELED){
			fprintf(stderr, "Canceled\n");
		}else fprintf(stderr, "Succesful exit\n");
	}
	return 0;
}
