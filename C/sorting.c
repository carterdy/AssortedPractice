#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* mergesort(char* input);
char* merge(char* left, char* right);
char* mergesort_r(char* input);
char* quicksort_r(char* input);

typedef struct D_Array {
	char* array;
	size_t size; //number of elements
} D_Array;

D_Array* init_array();
void append_array(D_Array *a, char element);
void set_array(D_Array *a, char* elements);

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Wrong number of arguments. Got %d\n", argc);
		getchar();
		return 1;
	}
	char* input_string = argv[1];
	char sort_type = argv[2][0];

	//Determine the sort type to use
	switch (sort_type) {
	case 'm':
		//merge sort
		printf("%s\n", mergesort_r(input_string));
		break;
	case 'q':
		//quick sort
		printf("%s\n", quicksort_r(input_string));
		break;
	default:
		printf("Hmm...\n");
	}
	getchar();
	return 0;
}

D_Array* init_array() {
	D_Array* a = (D_Array*)malloc(sizeof(D_Array));
	a->array = (char*) malloc(1);
	a->array[0] = '\0';
	a->size = 0;
	return a;
}

/* Add an element to the end of the array */
void append_array(D_Array *a, char element) {
	a->array = (char*) realloc(a->array, a->size + 1);
	a->array[a->size] = element;
	a->size++;
	a->array[a->size] = '\0';
}

void set_array(D_Array* a, char* elements) {
	a->size = strlen(elements);
	a->array = (char*) malloc(a->size + 1);
	memcpy(a->array, elements, a->size);
	a->array[a->size] = '\0';
}

/* Take in a string of characters and return a copy in sorted order using quicksort */
char* quicksort_r(char* input) {
	int length = strlen(input);
	if (length <= 1) {
		//base case
		return input;
	}
	else {
		//Have to pick a pivot, move elements to less than or equal to the pivot, then sort sub arrays
		//This will be dirty because C doesn't have dynamic arrays
		D_Array* pivot_list = init_array();
		D_Array* less =	init_array();
		D_Array* more =	init_array();
		//Add the initial pivot
		char pivot = input[0];
		for (int i = 0; i < strlen(input); i++) {
			if (input[i] < pivot) {
				append_array(less, input[i]);
			}
			else if (input[i] > pivot) {
				append_array(more, input[i]);
			}
			else {
				append_array(pivot_list, input[i]);
			}
		}
		set_array(less, quicksort_r(less->array));
		set_array(more, quicksort_r(more->array));
		char* result = malloc(strlen(input) + 1);
		memcpy(result, less->array, less->size);
		memcpy(result + less->size, pivot_list->array, pivot_list->size);
		memcpy((result + less->size) + pivot_list->size, more->array, more->size);
		result[strlen(input)] = '\0';
		return result;
	}
}

/* Take a string of characters and return a copy in sorted order using mergesort */
char* mergesort(char* input) {
	//Allocate and copy the string
	char* input_copy = (char*)malloc(strlen(input));
	strncpy(input_copy, input, strlen(input));
	return mergesort_r(input_copy);
}

/* Take in two sorted strings and output the sorted merge of the two */
char* merge(char* left, char* right) {
	if (left[0] == '\0') {
		//left is empty
		return right;
	}
	else if (right[0] == '\0') {
		//right is empty
		return left;
	}
	else if (left[0] < right[0]) {
		//first char of left is less than first char of right
		char* result = merge(&left[1], right);
		char* merged = malloc(strlen(result) + 2);
		memcpy(merged, left, 1);
		memcpy(merged + 1, result, strlen(result) + 1);
		return merged;
	}
	else {
		//first char of left is greater than first char of right
		char* result = merge(left, &right[1]);
		char* merged = malloc(strlen(result) + 2);
		memcpy(merged, right, 1);
		memcpy(merged + 1, result, strlen(result) + 1);
		return merged;
	}
}

/* recursive mergesort the given string and return it */
char* mergesort_r(char* input) {
	int length = strlen(input);
	if (length == 1) {
		//base case - single element. Return element
		return input;
	}
	else {
		//Gotta do stuff. Split string into halves, sort the halves, then merge the halves
		if (length == 0) {
			return input;
		}
		int mid = length / 2;
		char *left = malloc(mid + 1);
		char *right = malloc((length - mid)+1);
		memcpy(left, input, mid);
		left[mid] = '\0';
		memcpy(right, &input[mid], (length - mid));
		right[length - mid] = '\0';
		left = mergesort_r(left);
		right = mergesort_r(right);

		return merge(left, right);
	}
}