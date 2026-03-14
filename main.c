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
	int thread_num = 1;
	while ((opt = getopt(argc, argv, opts)) != -1){
		thread_num = atoi(optarg); // считывание количества потоков из командной строки
	}	

	printf("%d\n", thread_num);
	//считываем введенные в командную строку числа
	size_t len = 1;
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
	
	printf("%s", input);
	free(input);
	return 0;
	

	//parallel_divide(thread_num);
		
}
