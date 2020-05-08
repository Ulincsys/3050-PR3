#include "functions.h"

/* LIST CODE -----------------------------------------------------------------------------*/

Any stackPop(List list) {
	Any result = 0;
	if(list->size > 0) {
		Node temp = list->head;
		list->head = list->head->next;
		result = temp->data;
		free(temp);
		list->size--;
	}
	return result;
}

void stackPush(List list, Any data) {
	Node temp = malloc(sizeof(Node));
	if(temp) {
		temp->data = data;
		temp->next = list->head;
		list->head = temp;
		list->size++;
	}
}

List makeList() {
	return malloc(sizeof(ListStruct));
}

void emptyList(List list) {
	while(list->size > 0) {
		stackPop(list);
	}
}

void freeList(List list) {
	emptyList(list);
	free(list);
}

/* TREE CODE -----------------------------------------------------------------------------*/

TreeNode makeTreeNode() {
	return malloc(sizeof(TreeNodeStruct));
}

TreeNode makeTree(int data, TreeNode parent) {
	TreeNode temp = makeTreeNode();
	if(!temp) { return NULL; }
	temp->data = data;
	temp->parent = parent;
	return temp;
}

void freeTree(TreeNode root) {
	if(!root) {
		return;
	}
	freeTree(root->left);
	freeTree(root->right);
	free(root);
}

void printTreeSteps(TreeNode goal) {
	if(goal && (goal->left || goal->right)) {
		printTreeSteps(goal->parent);
		printf("%s%d", (goal->parent) ? " -> " : "", goal->data);
	} else if(goal && !goal->left && !goal->right) {
		printTreeSteps(goal->parent);
		printf("%s%d\n", (goal->parent) ? " -> " : "", goal->data);
	}
}

/* Accepts starting information for the search, and builds a binary tree row-by-row to perform a Breadth
 * First Search for the supplied goal. Once the goal is reached, processing of the current row is stopped
 * and the steps taken are placed to stdout by recursively printing from the goal node to the root node
 * through the TreeNode->parent pointers in reverse order. An array of size "floors" is created and used
 * as a hash table to keep track of previously visited nodes. If a node has not been visited, a new node
 * is created and assigned as the appropriate child of its parent node. The new nodes are then added to a
 * "row" stack which is used to process the next row after the current inner iteration ends. If it is not
 * possible to reach the goal, we can determine this by marking all nodes visited without reaching the
 * goal node, in which case the function returns 0. Returns 1 on success.
 */
int searchWithTree(int floors, int start, int goal, int up, int down) {
	List row = makeList();
	Array visited = makeArray(floors + 1, NULL, sizeof(int));
	TreeNode root = makeTree(start, NULL);
	stackPush(row, root);
	visited[start] = 1;

	while(row->size > 0) {
		List nextRow = makeList();
		while(row->size > 0) {
			TreeNode temp = stackPop(row);
			int left = temp->data - down;
			int right = temp->data + up;
			if(left > 0 && !visited[left]) {
				temp->left = makeTree(left, temp);
				stackPush(nextRow, temp->left);
				visited[left] = 1;
			} if(right <= floors && !visited[right]) {
				temp->right = makeTree(right, temp);
				stackPush(nextRow, temp->right);
				visited[right] = 1;
			}
			if(left == goal) {
				printTreeSteps(temp->left);
				freeTree(root);
				freeList(row);
				freeList(nextRow);
				freeArray(visited);
				return 1;
			} else if(right == goal) {
				printTreeSteps(temp->right);
				freeTree(root);
				freeList(row);
				freeList(nextRow);
				freeArray(visited);
				return 1;
			}
		}
		freeList(row);
		row = nextRow;
	}
	freeTree(root);
	freeList(row);
	freeArray(visited);
	return 0;
}

/* Uses an iterative direct approach to search for a given value in a (virtual) tree using a horizontal
 * search. If the goal is not reachable, we can tell when the number of iterations becomes greater than
 * the number of floors, in which case the function returns 0. Returns 1 on success.
 */
int searchDirect(int floors, int start, int goal, int up, int down) {
	Array array = makeArray(floors + 1, NULL, sizeof(int));
	int count = 0, current = start;
	while(count <= floors) {
		array[count++] = current;
		if(current == goal) {
			break;
		} else if(goal > current) {
			if(current + up > floors) {
				current -= down;
			} else {
				current += up;
			}
		} else {
			if(current - down < 1) {
				current += up;
			} else {
				current -= down;
			}
		}
	}

	if(count <= floors) {
		printSteps(array, goal);
		freeArray(array);
		return 1;
	}
	freeArray(array);
	return 0;
}

/* Accepts an array of integers representing steps in a search sequence, and prints the steps
 * separated by right-facing arrows. This function assumes the search was successful, and
 * that the goal was only reached once.
 * Parameters:
 * Array array		- An array of integers representing steps in a search sequence.
 * int goal			- An integer representing the last number in the sequence to be printed.
 */
void printSteps(Array array, int goal) {
	int i = 0;
	while(array[i] != goal) {
		printf("%d -> ",array[i++]);
	}
	printf("%d\n",array[i]);
}

/* Takes an array pointer of any type with the size stored at the pre-index location of -1 int spaces,
 * and returns the size. Will segfault if passed a NULL or invalid pointer.
 * Parameters:
 * 	Any any 		- An array pointer which has an integer representing its size at index -1.
 */
int getsize(Any any) {
	Array temp = (Array)any;
	return temp[-1];
}

/* Creates an array with size stored at integer index -1, and also copies any given array data into
 * the newly allocated memory. Requires the size of the data type to be given. If the data array parameter
 * is NULL, the array will be initialized to 0. The data array given MUST be at least as long as the size
 * given, but any elements after the given size will be ignored. The data array given doesn't necessarily
 * need to be of the same type as the data_size given, but it must be at least the same size in bytes.
 * Will return NULL if allocating memory failed.
 * Parameters:
 * 	int size 		- The size of the array to create in terms of the number of elements.
 * 	Any any 		- An array of data to be entered into the newly created array, will be ignored if NULL.
 * 	int data_size 	- The size of the array data type (IE: use sizeof(int) for an integer array etc..).
 */
Any makeArray(int size, Any any, int data_size) {
	String init = malloc((data_size * size) + sizeof(int));
	if(!init) { return NULL; }

	Array temp = (Array)init;
	temp[0] = size;
	init += sizeof(int);

	if(any) {
		String data = (String)any;
		for(int i = 0; i < (data_size * size); ++i) {
			init[i] = data[i];
		}
	} else {
		for(int i = 0; i < (data_size * size); ++i) {
			init[i] = 0;
		}
	}
	return init;
}

/* Takes an array pointer of any type with the size stored at the pre-index location of -1 int spaces,
 * and frees the block of memory allocated to it. Will segfault if passed a NULL or invalid pointer.
 * Parameters:
 * 	Any any 		- An array pointer which has an integer representing its size at index -1.
 */
void freeArray(Any any) {
	Array temp = (Array)any;
	free(temp - 1);
}
