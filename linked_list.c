#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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

void * list_get_tail(list * list) {
	node * tail = get_tail(list);
	if (tail) {
		return  tail -> cargo;
	} 
	return NULL;
} 

list * create_list(int data_size, void (* cargo_delete) (void *)) {
	list * new_list = malloc(sizeof(list));
	new_list -> head = NULL;
	new_list -> length = 0;
	new_list -> cargo_delete = cargo_delete;
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

void print_list(list * list) {
	node * temp = list->head;
	while (temp) {
		list->print(temp);
		temp = temp -> next;
	}
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

int list_replace_tail(list * list) {
	return list_delete_index(list ->length -1, list);
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

void * list_step (list * list) {
	struct node * current = store_node_pointer(NULL);
	return current -> cargo;
};
 
int main() {

}

//what should I make for these generics? 
//list -> seems to be done!

//dictionary? -> think this would be good... //good exercise, also helps with million row challenge 
//Figure out how to deal with case sensitivity and werid characters... 


//tree
//expect this to be as simple as the list 

//RB tree (build on top of tree)
//kind of hard to get right

//quicksort
//high priority pain in the arse. do tonight.

