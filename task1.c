#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "task1.h"
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

int parallel_divide(int thread_num, int *arr, int arr_size){
	if (thread_num > arr_size){
		thread_num = arr_size;	
	} 
	int chunk_size = arr_size / thread_num;
	int remainder = arr_size % thread_num;
	pthread_t *threads = malloc(thread_num * sizeof(pthread_t));
	if (threads == NULL){
		perror("memory allocation error");
		return 1;
	}
	
	Arr *chunk_arrays = malloc(thread_num * sizeof(Arr));	
	if (chunk_arrays == NULL){
		perror("memory allocation error");
		free(threads);
		return 1;
	}
	
	int cur_start = 0;
	for (int i = 0; i < thread_num; ++i) {
		int cur_chunk_size = chunk_size + (i < remainder ? 1 : 0); //распределяем остаток на процессоры, чей номер меньше остатка.
		chunk_arrays[i].arr = arr;
		chunk_arrays[i].start = cur_start;  
		chunk_arrays[i].end = cur_start + cur_chunk_size - 1; //тернарный оператор больше не нужен.
		cur_start += cur_chunk_size;
		if (pthread_create(&threads[i], NULL, sort, &chunk_arrays[i]) != 0){
			perror("pthread_create error");
			free(threads);
			free(chunk_arrays);
			return 1;
		}
	}
	
	for (int i = 0; i < thread_num; i++) {
		pthread_join(threads[i], NULL);
	}

	int *temp_arr = malloc(arr_size * sizeof(int)); //temp_arr вместо left_arr и right_arr.
	if (temp_arr == NULL){
		perror("memory allocation error");
		free(threads);
		free(chunk_arrays);
		return 1;
	}	

	int active_chunks = thread_num;
	while (active_chunks > 1) {
		int next_chunks = 0;
		for(int i = 0; i < active_chunks; i += 2){
			if (i + 1 < active_chunks){
				int left = chunk_arrays[i].start;
				int mid = chunk_arrays[i].end;
				int right = chunk_arrays[i + 1].end;
				merge(arr, temp_arr, left, mid, right);
				chunk_arrays[next_chunks].start = left; 
				chunk_arrays[next_chunks].end = right; 
			} 
			else {
				chunk_arrays[next_chunks] = chunk_arrays[i];
			}
			next_chunks++;
		}
		active_chunks = next_chunks;
	}
	
	free(temp_arr);
	free(threads);
	free(chunk_arrays);
	return 0;
}

void* sort(void *arg){
	Arr *sub_arr = (Arr*)arg;
	int *start_ptr = sub_arr->arr + sub_arr->start;
	int sub_size = sub_arr->end - sub_arr->start + 1;
	qsort(start_ptr, sub_size, sizeof(int), compare_func);
	return NULL;
}

int compare_func(const void* arg1, const void* arg2){
	int a = *((int*)arg1);
	int b = *((int*)arg2);
	return a - b;
}

void merge(int *arr, int *temp_arr, int left, int mid, int right){
	int i = left;
	int j = mid + 1;
	int k = left;
	while (i <= mid && j <= right){
		if (arr[i] < arr[j]){
			temp_arr[k++] = arr[i++]; //код укорочен за счет (*arr)[i++]
		}
		else {
			temp_arr[k++] = arr[j++];
		}
	}
	
	while(i <= mid){
		temp_arr[k++] = arr[i++];
	}
	
	while(j <= right){
		temp_arr[k++] = arr[j++];
	}

	for (int i = left; i <= right; i++){
		arr[i] = temp_arr[i];
	}
}
