#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "task1.h"
#include <string.h>

#define BUF_SIZE 256
#define ASCII0 48

int main(int argc, char **argv){
	char *opts = "t:";
	int opt;
	int thread_num = 1; // в случае отсутствия парметра будет один поток
	while ((opt = getopt(argc, argv, opts)) != -1){
		thread_num = atoi(optarg); // считывание количества потоков из командной строки
	}	

	//printf("%d\n", thread_num);
	//считываем введенные в командную строку числа
	size_t len = 0;
	char *input = calloc(len, sizeof(char));
	if (input == NULL){
		perror("calloc error");
		return 1;
	} 	
	
	char buf[BUF_SIZE];
	while (fgets(buf, BUF_SIZE, stdin) != NULL){
		size_t chunk_len = strlen(buf);
		char *temp_input = realloc(input, len + chunk_len);
		if (temp_input == NULL){
			perror("realloc error");
			free(input);
			return 1;
		} 
		input = temp_input;
		strcat(input, buf);
		len += chunk_len;
	}

	//записываем данные в массив
	size_t arr_size = len / 2;
	int *arr = calloc(arr_size, sizeof(int));
	int j = 0;
	for (int i = 0; i < (int)arr_size; i += 2){
		arr[j] = input[i] - ASCII0;
		j += 1;
	}
	
	//printf("%s%lu\n", input, len);
	/*for (int i = 0; i < (int)len/2; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");*/

	parallel_divide(thread_num, &arr, arr_size);

	for (int i = 0; i < (int)arr_size; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	free(arr);
	free(input);
	return 0;
}
