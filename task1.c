#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "task1.h"
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

int parallel_divide(int thread_num, int **arr, int arr_size){
	if (thread_num > arr_size){
		thread_num = arr_size;	
	} 
	int chunk_size = arr_size / thread_num;
	pthread_t *threads = malloc(thread_num * sizeof(pthread_t));
	Arr *chunk_arrays = malloc(thread_num * sizeof(Arr));
	for (int i = 0; i < thread_num; ++i) {
		chunk_arrays[i].arr = *arr;
		chunk_arrays[i].start = i * chunk_size;
		chunk_arrays[i].end = (i == thread_num - 1) ? arr_size - 1 : chunk_arrays[i].start + chunk_size - 1;
		if (pthread_create(&threads[i], NULL, sort, &chunk_arrays[i]) != 0){
			perror("pthread_create error");
			return 1;
		}
	}
	
	for (int i = 0; i < thread_num; i++) {
		pthread_join(threads[i], NULL);
	}
	
	int step = chunk_size;
	while (step < arr_size) {
		for (int i = 0; i < arr_size - step; i += 2 * step) {
			int left = i;
			int mid = i + step - 1;
			int right = min(i + 2 * step - 1, arr_size - 1);
			merge(arr, left, mid, right); 
		}
		step *= 2;
	}
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

int merge(int **arr, int left, int mid, int right){
	int i, j, k;
	int len1 = mid - left + 1;
	int len2 = right - mid;
	int *left_array = (int*)malloc(len1 * sizeof(int));
	int *right_array = (int*)malloc(len2 * sizeof(int));
	for(i = 0; i < len1; ++i){
		left_array[i] = (*arr)[left+i];
	}
	for(j = 0; j < len2; ++j){
		right_array[j] = (*arr)[mid + j + 1];
	}
	
	i = 0; 
	j = 0;
	k = left;
	while(i < len1 && j < len2){
		if (left_array[i] < right_array[j]){
			(*arr)[k] = left_array[i];
			++i;
		}
		else{
			(*arr)[k] = right_array[j];
			++j;
		}
		++k;
	}
	
	while(i < len1){
		(*arr)[k] = left_array[i];
		++i;
		++k;
	}
	
	while(j < len2){
		(*arr)[k] = right_array[j];
		++j;
		++k;
	}
	
	free(left_array);
	free(right_array);
	return 0;
}
