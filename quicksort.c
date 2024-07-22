#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void swap(void * array, int lower, int upper, int item_size) {
	void * temp = malloc(item_size);
	void * lower_item = (void *) (((char *) array) + item_size*lower);
	void * upper_item = (void *) (((char *) array) + item_size*upper);

	memcpy(temp, lower_item, item_size);
	memcpy(lower_item, upper_item, item_size);
	memcpy(upper_item, temp ,item_size);
}

static void quick_sort(void * array, int upper, int lower, int item_size, int (*compare) (void *, void *)) {
	assert(upper >= lower);
	if (upper - lower == 0 ) {
		return;
	}

	int left_index = lower;
	int right_index = upper - 1;
	int initial_pivot = (lower + upper)/2;
	int pivot = upper;

	swap(array, initial_pivot, pivot, item_size);

	//forgot if this is supposed to be greater equals or just equals
	while (right_index >= left_index) {
		void * left_item = (void *) (((char *) array) + item_size*left_index);
		void * right_item = (void *) (((char *) array) + item_size*right_index);
		void * pivot_item = (void *) (((char *) array) + item_size*pivot);

		int move_left = compare(pivot_item, left_item);
		int move_right = compare(right_item, pivot_item);

		if (move_left) {
			left_index ++;
		}

		if (move_right) {
			right_index --;	
		}

		if (!move_left && !move_right) {
			swap(array, left_index, right_index, item_size);
		}
	}

	swap(array, left_index, pivot, item_size);

	//using 2 here may be slightly bad
	//printf("Fixed:\n");
	//for (int i = lower; i <= upper; i ++) {
	//	printf("%d,", *((int *)(((char *) array) + item_size*i))) ;
	//}
	//printf("\n");
	//printf("New ranges:%d,%d, %d, %d\n", left_index-1, lower, upper, left_index+1);

	if (lower < left_index - 1) {
		quick_sort(array, left_index-1, lower, item_size, compare);
	}
	if (left_index +1 < upper) {
		quick_sort(array, upper, left_index+1, item_size, compare);
	}

}

void sort(void * array, int array_size, int item_size, int (*compare) (void *, void *)) {
	quick_sort(array, array_size-1, 0, item_size, compare);
}

int gte(void * a, void* b) {
	int * A = a;
	int * B = b;
	return (*A) >= (*B);
}

int main() {
	int count = 1000000;
	int sortable [count] = {};

	for (int i = 0; i < count; i ++) {
		sortable[i] = rand()%count;
	}

	sort(sortable, count, sizeof(int), &gte);
	int temp = -1;

	for (int i = 0; i < count; i ++) {
		printf("%d: %d\n",i,sortable[i]);
		if (sortable[i] < temp) {
			printf("Error\n");
			exit(0);
		} else {
			temp = sortable[i];
		}
	}
}

