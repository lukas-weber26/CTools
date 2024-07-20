#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int array_size;

void print_array(int a [], int l) {
	for (int i = 0; i < l; i++) {
		printf("%d,", a[i]);
	}
	printf("\n");
}

void print_array_low_high(int a [], int low, int high) {
	for (int i = low; i <= high; i++) {
		printf("%d,", a[i]);
	}
	printf("\n");
}

void initialize(int sortable [], int length) {
	for (int i = 0; i < length; i++) {
		sortable[i] = rand() % 50;
	}
}

void swap(int array [], int low, int high) {
	int temp = array[low];
	array[low] = array[high];
	array[high] = temp;
	return;
}

void quicksort(int array [], int low, int high) {
	if (low == high) {
		return;
	}	

	if (high - low == 1) {
		if (array[high]>array[low]) {
			return;
		}
		swap(array,high,low);
		return;
	}
	
	int left_index = low;
	int right_index = high-1;
	int pivot_starting_point = (high+low)/2;
	int pivot_position = high;
	swap(array,pivot_starting_point,pivot_position);

	while (left_index <= right_index) { //may need to be <=
		int move_left_index = array[left_index] <= array[pivot_position];
		int move_right_index = array[right_index] >= array[pivot_position]; // this one may not be needed

		if (move_left_index) {
			left_index ++;
		}

		if (move_right_index) {
			right_index --;
		}

		if (!move_left_index && ! move_right_index) {
			swap(array, left_index, right_index);	
		}

	}

	swap(array,left_index, pivot_position);
	if (left_index - 1 >= low) {
		quicksort(array, low, left_index-1);
	}

	if (left_index+1 <= high) {
		quicksort(array, left_index+1, high);
	}
	
}

int confirm_sorted(int a[], int l) {
	if (l == 1) {return 1;}

	for (int i = 1; i < l; i++) {
		if (a[i-1] > a[i]) {
			return 0;
		}
	}

	return 1;
}

void case1() {
	int a [1] = {1};
	quicksort(a, 0, 0);
	assert(confirm_sorted(a, 1));
}

void case2() {
	int a [2] = {1,2};
	quicksort(a, 0, 1);
	assert(confirm_sorted(a, 2));

	a[0] = 2;
	a[1] = 1;
	quicksort(a, 0, 1);
	assert(confirm_sorted(a, 2));
}

void case3() {
	int a[3] = {1,2,3};	
	int b[3] = {1,3,2};	
	int c[3] = {3,1,2};	
	int d[3] = {3,2,1};	

	quicksort(a, 0, 2);
	quicksort(b, 0, 2);
	quicksort(c, 0, 2);
	quicksort(d, 0, 2);

	assert(confirm_sorted(a, 3));
	assert(confirm_sorted(b, 3));
	assert(confirm_sorted(c, 3));
	assert(confirm_sorted(d, 3));
}

void case4() {
	int a[4] = {1,2,3,4};
	int b[4] = {4,3,2,1};
	int c[4] = {1,4,3,2};
	int d[4] = {4,2,1,2};

	quicksort(a, 0, 3);
	quicksort(b, 0, 3);
	quicksort(c, 0, 3);
	quicksort(d, 0, 3);

	assert(confirm_sorted(a, 4));
	assert(confirm_sorted(b, 4));
	assert(confirm_sorted(c, 4));
	assert(confirm_sorted(d, 4));
}


void case5() {
	for (int j = 0; j < 1000000; j++){
		int length = rand() % 200 + 1;	
		int current_array[length];
		for (int i = 0; i < length; i++) {
			current_array[i] = rand()%100;
		}
		quicksort(current_array, 0, length-1);
		assert(confirm_sorted(current_array, length));
	}
}

void test_quicksort() {
	case1();
	case2();
	case3();
	case4();
	case5();
}

int main() {
	test_quicksort();
}
