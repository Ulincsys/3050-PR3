#include <stdio.h>
#include <stdlib.h>

#define CMD_FILE "input.txt"

typedef char* String;
typedef String* Strings;
typedef void* Any;
typedef int* Array;
typedef FILE* File;
typedef struct NodeStruct NodeStruct;
typedef struct ListStruct ListStruct;
typedef struct TreeNodeStruct TreeNodeStruct;
typedef NodeStruct* Node;
typedef ListStruct* List;
typedef TreeNodeStruct* TreeNode;

struct TreeNodeStruct {
	TreeNode parent;
	TreeNode left;
	TreeNode right;
	int data;
};

struct NodeStruct {
	Node next;
	Any data;
};

struct ListStruct {
	Node head;
	int size;
};

File cmdIn;

int searchDirect(int floors, int start, int goal, int up, int down);
int searchWithTree(int floors, int start, int goal, int up, int down);
void printSteps(Array array, int goal);
void printTreeSteps(TreeNode goal);
TreeNode makeTreeNode();
TreeNode makeTree(int data, TreeNode parent);
void freeTree(TreeNode root);

void stackPush(List list, Any data);
Any stackPop(List list);
void emptyList(List list);
List makeList();
void freeList(List list);

Any makeArray(int size, Any any, int data_size);
int getsize(Any any);
void freeArray(Any any);
