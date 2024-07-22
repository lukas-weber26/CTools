#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>

typedef struct node {
	struct node * next;
	void * cargo;	
} node; 

typedef struct list {
	node * head;
	int length;
	int cargo_size; // this may prove to be unnecessary 
	void (* cargo_delete)(void *); 
	void (* print)(void *); 
	//plenty of room to add more functions here if there is an interest.
} list;

//core functions
static node * create_node();
static void delete_data(list * list, void * cargo);
static void delete_node_data(list * list, node * node);
static void delete_node (list * list, node * node);
static node * get_tail(list * list);
void * list_get_tail(list * list);
void * list_get_head(list * list);
void * list_get_index(int index, list * list);
list * list_create(int data_size, void (* cargo_delete) (void *), void (* print) (void *));
int list_insert(list * list, void * data);
int list_insert_head(list * list, void * data);
int list_insert_index(int index, list * list, void * data);
void list_print(list * list);
int list_length(list * list);
int list_delete_index (int index, list * list);
int list_delete_head(list * list);
int list_delete_tail(list * list);
int list_replace_head(list * list, void * data);
int list_replace_tail(list * list, void * data);
int list_replace_index (int index, list * list, void * data);
void list_step_init(list *list);
void * list_step ();
static node * store_node_pointer(node * node);

//for testing only 
typedef struct test_struct {
	int val;
} test_struct;

void test_cargo_delete(void * cargo) {} //does nothing because the test struct does not have any malloced memory
void test_cargo_print(void * cargo) {
	printf("%d,", ((test_struct *) cargo ) -> val);
}

void small_test() {
	test_struct * x = malloc(sizeof(test_struct));
	x->val = 10;

	list * test_list = list_create(sizeof(test_struct), &test_cargo_delete, &test_cargo_print);
	assert(list_length(test_list) == 0);

	list_insert_head(test_list, x);
	assert(list_get_index(0, test_list) == x);
	assert(list_length(test_list) == 1);
	assert(list_get_head(test_list) == x);
	assert(list_length(test_list) == 1);
	assert(list_get_tail(test_list) == x);
	assert(list_length(test_list) == 1);
	list_delete_head(test_list);

	x = malloc(sizeof(test_struct));
	x->val = 10;
	
	assert(list_length(test_list) == 0);
	list_delete_head(test_list);

	x = malloc(sizeof(test_struct));
	x->val = 10;
	
	assert(list_length(test_list) == 0);
	list_insert(test_list, x);
	assert(list_length(test_list) == 1);
	list_delete_tail(test_list);

	x = malloc(sizeof(test_struct));
	x->val = 10;
	
	assert(list_length(test_list) == 0);
	list_insert_index(0,test_list, x);
	assert(list_length(test_list) == 1);
	list_delete_index(0,test_list);

	x = malloc(sizeof(test_struct));
	x->val = 10;
	
	assert(list_length(test_list) == 0);
	list_delete_index(10,test_list);
	assert(list_length(test_list) == 0);

	x = malloc(sizeof(test_struct));
	x->val = 10;
	test_struct * y = malloc(sizeof(test_struct));
	y->val = 100;
	test_struct * z = malloc(sizeof(test_struct));
	z->val = 1000;

	list_insert(test_list, x);
	list_insert(test_list, y);
	list_insert(test_list, z);
	
	printf("%d\n", ((test_struct *) list_get_head(test_list)) -> val);
	printf("%d\n", ((test_struct *) list_get_index(1,test_list)) -> val);
	printf("%d\n", ((test_struct *) list_get_tail(test_list)) -> val);

	list_print(test_list);

	list_step_init(test_list);
	void * p;
	while ((p = list_step()) != NULL) {
		printf("%d\n", ((test_struct *) p) -> val);
	}

	list_delete_index(1, test_list);
	list_print(test_list);
}

//compare the linked lists getters and state to a validation array.
void test_list_integrity (int * validation_array, int validation_array_size, list * test_list) {
	assert(validation_array_size == list_length(test_list));

	if (validation_array_size > 0) {
		assert(validation_array[0] == ((test_struct *) list_get_head(test_list)) ->val);
		assert(validation_array[validation_array_size -1] == ((test_struct *) list_get_tail(test_list)) ->val);
	} else {
		assert(NULL == list_get_head(test_list));
		assert(NULL == list_get_tail(test_list));
	}
	for (int i = 0; i < validation_array_size; i++) {
		test_struct * result = list_get_index(i, test_list);
		assert(result ->val == validation_array[i]);
	}
}

//call after deletes
void test_array_clean(int test_array [], int test_array_length) {
	int copy_to = 0;
	int copy_from = 0;
	for (copy_from = 0; copy_from < test_array_length; copy_from ++) {
		if (test_array[copy_from] >= 0) {
			test_array[copy_to] = test_array[copy_from];
			copy_to ++;
		}
	}
}

void test_array_delete(int test_array[], int delete_index) {
	test_array[delete_index] = -1;	
}

void test_array_replace(int test_array[], int replace_index, int val) {
	test_array[replace_index] = val;
}

void test_array_insert(int test_array[], int test_array_length ,int insert_index, int val) {
	int prev = val;
	for (int i = insert_index; i <= test_array_length; i ++) {
		int temp = test_array[i];
		test_array[i] = prev;
		prev = temp;
	}
}

int test_array_length_increment(int test_array_length, int delta) {
	test_array_length += delta;
	if (test_array_length > 0) {
		return test_array_length;
	} 
	return 0;
}

test_struct * test_generate_struct() {
	test_struct * new = malloc(sizeof(test_struct));
	new ->val = rand()%1000;
	return new;
}

//int list_delete_head(list * list);
//int list_delete_tail(list * list);

//int list_replace_head(list * list, void * data);
//int list_replace_tail(list * list);
//int list_replace_index (int index, list * list, void * data);
//

void test_print_array(int array [], int length) {
	for (int i = 0; i < length; i++) {
		printf("%d,", array[i]);
	}
	printf("\n");
}

void test_large_automated_test() {
	for (int j = 0; j < 100000; j ++) {

		int test_array[10001] = {0};
		int test_array_length = 0;
		list * test_list = list_create(sizeof(test_struct), &test_cargo_delete, &test_cargo_print);

		for (int i = 0; i < 10000; i++) {
			int test_number = rand()%9;
			test_struct * s;
			int index;
			switch (test_number) {
				case(0): 
					s = test_generate_struct();
					list_insert(test_list, s);
					test_array_insert(test_array, test_array_length, test_array_length, s->val);	
					test_array_length = test_array_length_increment(test_array_length, 1);
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				case(1): 
					s = test_generate_struct();
					list_insert_head(test_list, s);
					test_array_insert(test_array, test_array_length, 0, s->val);
					test_array_length = test_array_length_increment(test_array_length, 1);
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				case(2):
					s = test_generate_struct();
					index = rand()%100;
					list_insert_index(index,test_list, s);
					if (index <= test_array_length) {
						test_array_insert(test_array, test_array_length, index, s->val);
						test_array_length = test_array_length_increment(test_array_length, 1);
					}
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				case(3):
					index = rand()%100;
					list_delete_index(index, test_list);
					if (index < test_array_length) {
						test_array_delete(test_array, index);
						test_array_clean(test_array, test_array_length); 
						test_array_length = test_array_length_increment(test_array_length, -1);
					}
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				case(4):
					index = 0;
					list_delete_head(test_list);
					if (index < test_array_length) {
						test_array_delete(test_array, index);
						test_array_clean(test_array, test_array_length); 
						test_array_length = test_array_length_increment(test_array_length, -1);
					}
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				case(5):
					index = test_array_length-1;
					list_delete_tail(test_list);
					if (index < test_array_length) {
						test_array_delete(test_array, index);
						test_array_clean(test_array, test_array_length); 
						test_array_length = test_array_length_increment(test_array_length, -1);
					}
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				//replacement tests
				case(6):
					index = rand()%100;
					s = test_generate_struct();
					list_replace_index(index, test_list,s);
					if (index < test_array_length) {
						test_array_replace(test_array, index, s->val);
					}
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				case(7):
					index = 0;
					s = test_generate_struct();
					list_replace_head(test_list,s);
					if (index < test_array_length) {
						test_array_replace(test_array, index, s->val);
					}
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				case(8):
					index = test_array_length - 1;
					s = test_generate_struct();
					list_replace_tail(test_list,s);
					if (index < test_array_length) {
						test_array_replace(test_array, index, s->val);
					}
					test_list_integrity(test_array, test_array_length, test_list);	
					break;
				default:
					printf("Cation, invalid test case\n");
					break;
			}
		}

	}
	
	//list_print(test_list);
	//test_print_array(test_array, test_array_length);

	//the test array has a fixed size, ie will never be realloced.
	//only positive numbers will be used during tests. A clean function will reogranize the array if negative numbers are present.
	//maximum test size if 10000 item list.
}

int main() {
	test_large_automated_test();
}

void list_delete(list * list) {
	while (list->length > 0) {
		list_delete_head(list);
	}
	free(list);
}

static node * create_node() {
	node * new_node = malloc(sizeof(node));
	new_node -> next = NULL;
	new_node ->cargo = NULL;
	return new_node;
}

static void delete_data(list * list, void * cargo) {
	if (list -> cargo_delete) {
		list ->cargo_delete(cargo);
	} else {
		free(cargo);
	}
}

static void delete_node_data(list * list, node * node) {
	delete_data(list, node -> cargo);
	node ->cargo= NULL;
}

static void delete_node (list * list, node * node){
	delete_node_data(list, node);
	free(node);
}


static node * get_tail(list * list) {
	if (list->head) {
		node * temp = list->head;
		while (temp -> next) {
			temp = temp -> next;
		}
		return temp;
	} else{
		return NULL;
	}
}

void * list_get_head(list * list) {
	if (list ->head) {
		return list -> head -> cargo;
	}	
	return NULL;
} 

void * list_get_index(int index, list * list) {
	if (index == 0) {
		return list_get_head(list);
	} if (index < list->length && index > 0) {
		node * temp = list -> head;
		for (int i = 0; i < index; i ++) {
			temp = temp -> next;
		}
		return temp -> cargo;
	}
	return NULL;
}

void * list_get_tail(list * list) {
	return list_get_index(list->length - 1, list);
} 


list * list_create(int data_size, void (* cargo_delete) (void *), void (*print) (void *)) {
	list * new_list = malloc(sizeof(list));
	new_list -> head = NULL;
	new_list -> length = 0;
	new_list -> cargo_delete = cargo_delete;
	new_list -> print= print;
	new_list ->cargo_size = data_size;
	return new_list;
}

int list_insert(list * list, void * data) {
	node * new_node = create_node();
	node * tail;
	new_node ->cargo = data;
	if ((tail = get_tail(list)) != NULL) {
		tail ->	next = new_node;
	} else {
		list ->head = new_node;	
	}
	
	list ->length ++;
	return list -> length;
}

int list_insert_head(list * list, void * data) {
	node * new_node = create_node();
	new_node -> cargo = data;
	new_node ->next = list ->head;
	list -> head = new_node;
	list->length ++;
	return 0;
}

int list_insert_index(int index, list * list, void * data) {
	if (index <= list ->length && index >= 0) {
		if (index == 0) {
			return list_insert_head(list, data);
		} else if (index == list ->length) {
			return list_insert(list, data);
		} else {
			node * temp = list ->head;
			int i = 0;
			while (i < index - 1) {
				temp = temp->next;
				i++;
			}

			node * new_node = create_node();
			new_node -> cargo = data;
			new_node ->next = temp ->next;
			temp ->next = new_node;
			list->length++;
			return index;
		}
	}

	return  -1;
}

void list_print(list * list) {
	node * temp = list->head;
	while (temp) {
		list->print(temp->cargo);
		temp = temp -> next;
	}
	printf("\n");
}

int list_length(list * list) {
	return list ->length;
}

int list_delete_head(list * list) {
	if (list ->length == 1) {
		delete_node(list, list->head);
		list ->length --;
		list->head = NULL;
		return list->length;
	} else if (list -> length > 1) {
		node * temp = list->head;
		list->head = list -> head ->next;
		delete_node(list, temp);
		list ->length --;
		return list->length;
	} else {
		return -1;	
	}
}

int list_delete_index (int index, list * list) {
	if (index < list ->length && index >= 0) {
		if (index == 0) {
			return list_delete_head(list);
		} else {
			node * temp = list ->head;
			int i = 0;
			while (i < index - 1) {
				temp = temp->next;
				i++;
			}
			
			node * delete = temp -> next;
			temp -> next = delete -> next;
			delete_node(list,delete);	

			list->length--;
			return index;
		}
	}

	return  -1;
}

int list_delete_tail(list * list) {
	return list_delete_index(list ->length -1, list);
}

int list_replace_head(list * list, void * data) {
	if (list ->length >= 1) {
		delete_node_data(list, list->head);
		list ->head -> cargo = data;
		return list->length;
	} else {
		return -1;	
	}
}

int list_replace_index (int index, list * list, void * data) {
	if (index < list ->length && index >= 0) {
		if (index == 0) {
			return list_replace_head(list, data);
		} else {
			node * temp = list ->head;
			int i = 0;
			while (i < index) {
				temp = temp->next;
				i++;
			}
			
			delete_node_data(list, temp);
			temp -> cargo = data;

			return list->length;
		}
	}

	return  -1;
}

int list_replace_tail(list * list, void * data) {
	return list_replace_index(list ->length -1, list, data);
}

static node * store_node_pointer(node * node) {
	static struct node * stored_node = NULL;
	if (node != NULL) {
		stored_node = node;
		return stored_node;
	} else {
		if (stored_node) {
			struct node * temp = stored_node;
			stored_node = stored_node -> next;
			return temp;
		}
		return NULL;
	}
}

void list_step_init(list * list) {
	store_node_pointer(list -> head);
}

void * list_step () {
	struct node * current = store_node_pointer(NULL);
	if (current) {
		return current -> cargo;
	}
	return NULL;
};
//what should I make for these generics? 
//list -> seems to be done!

//dictionary? -> think this would be good... //good exercise, also helps with million row challenge 
//Figure out how to deal with case sensitivity and werid characters... 


//tree
//expect this to be as simple as the list 

//RB tree (build on top of tree)
//quicksort

