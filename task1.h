typedef struct{
	int *arr;
	int start;
	int end;
} Arr;

int parallel_divide(int thread_num, int **arr, int arr_size);
void* sort(void *arg);
int compare_func(const void* arg1, const void* arg2);
void merge(int **arr, int *temp_arr, int left, int mid, int right);
