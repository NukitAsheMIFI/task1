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

	int j = 0;
	size_t arr_size = len / 2;
	int *arr = malloc(arr_size * sizeof(int));;
    char *token = strtok(input, " ");
    while (token != NULL){
    	arr[j++] = atoi(token);
    	token = strtok(NULL, " ");
    }
	
	arr_size = j;
	int *temp_arr = realloc(arr, arr_size * sizeof(int));
	if (temp_arr == NULL){
		perror("allocation memory error");
		return 1;
	}
	arr = temp_arr;
	
	parallel_divide(thread_num, &arr, arr_size);

	for (int i = 0; i < (int)arr_size; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	
	free(arr);
	free(input);
	return 0;
}
