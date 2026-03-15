#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "task1.h"
#include <string.h>

#define BUF_SIZE 256

int main(int argc, char **argv){
	char *opts = "t:";
	int opt;
	int thread_num = 1; // в случае отсутствия парметра будет один поток
	while ((opt = getopt(argc, argv, opts)) != -1){
		thread_num = atoi(optarg); // считывание количества потоков из командной строки
	}	

	//считываем введенные в командную строку числа
	size_t len = 0;
	char *input = malloc(1 * sizeof(char));
	if (input == NULL){
		perror("memory allocation error");
		return 1;
	} 	
	input[0] = '\0';
	char buf[BUF_SIZE];
	while (fgets(buf, BUF_SIZE, stdin) != NULL){
		size_t chunk_len = strlen(buf);
		char *temp_input = realloc(input, len + chunk_len + 1);
		if (temp_input == NULL){
			perror("memory allocation error");
			free(input);
			return 1;
		} 
		input = temp_input;
		strcat(input, buf);
		len += chunk_len;
	}

	int j = 0;
	size_t arr_size = len;
	int *arr = malloc(arr_size * sizeof(int));
    char *token = strtok(input, " ");
    while (token != NULL){
    	arr[j++] = atoi(token);
    	token = strtok(NULL, " ");
    }
	free(input);
	arr_size = j;
	int *temp_arr = realloc(arr, arr_size * sizeof(int));
	if (temp_arr == NULL){
		perror("memory allocation error");
		return 1;
	}
	arr = temp_arr;
	
	if (parallel_divide(thread_num, &arr, arr_size) != 0) {
		free(arr);
		return 0;	
	}

	for (int i = 0; i < (int)arr_size; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	free(arr);
	return 0;
}
