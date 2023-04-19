/*Experimental Database by Agrim Sharma*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#define STRING_MAX 1024

typedef struct node
{
	char *name;
	int lines;
	int runtime;
	int memory_usage;

	struct node *next;
} Node;

typedef struct linked_list
{
	Node *head;
} LinkedList;

void readInputString(char *, int);
void readInputNumber(int *);
void readInputChar(char *);

LinkedList *newList();
char getCommand();

void handleInsert(LinkedList *);
void handleDelete(LinkedList *);
void handleSearch(LinkedList *);
void handlePrint(LinkedList *);
void handleQuit(LinkedList *);

int main()
{
	LinkedList *list = newList();
	char command = '\0';

	printf("Experimental database entry\n");

	while (command != 'Q')
	{
		command = getCommand();

		switch (command)
		{
		case 'I':
			handleInsert(list);
			break;
		case 'D':
			handleDelete(list);
			break;
		case 'S':
			handleSearch(list);
			break;
		case 'P':
			handlePrint(list);
			break;
		case 'Q':
			handleQuit(list);
			break;
		}
	}

	free(list);
	return 0;
}

void handleInsert(LinkedList *list)
{
	Node *newNode = (Node *)malloc(sizeof(Node));

	// If program is running out of memory, return NULL from the function
	if (newNode == NULL)
	{
		return;
	}

	// Create temporary int to get length of file name
	int fileNameLength;

	// Ask for file name length
	printf("\nNumber of characters in file name: ");
	readInputNumber(&fileNameLength);

	// Dynamically create new string with length of file name so that pointer doesn't get lost once this function returns
	char *tempName = (char *)malloc(sizeof(char) * (fileNameLength + 1));

	// Ask and store file name into node
	printf("File name: ");
	readInputString(tempName, fileNameLength + 1);
	newNode->name = tempName;

	// Ask and store file lines into node
	printf("Number of lines in CSV file: ");
	readInputNumber(&newNode->lines);

	// Ask and store file runtimte into node
	printf("Experiment runtime (ms): ");
	readInputNumber(&newNode->runtime);

	// Ask and store file memory usage into node
	printf("Experiment memory usage (B): ");
	readInputNumber(&newNode->memory_usage);

	// If list is empty, insert new node
	if (list->head == NULL)
	{
		newNode->next = NULL;
		list->head = newNode;

		return;
	}

	// Checks for same file name
	Node *current = list->head;
	do
	{
		if (strcmp(newNode->name, current->name) == 0)
		{
			printf("\nAn entry with that file name already exists.\n");
			return;
		}

		current = current->next;
	} while (current != NULL); // Current points to NULL at this point

	// Resetting current node pointer to start again at the head of the linked list
	current = list->head;

	// Checks if the file name of the newNode is less than file name of the first element in the list (ASCII)
	if (strcmp(current->name, newNode->name) > 0)
	{
		newNode->next = current;
		list->head = newNode;
		return;
	}

	// Iterates through linked list and places new node in alphabetical order of file name by comparing ASCII values
	while (current->next != NULL)
	{
		if (strcmp(current->next->name, newNode->name) > 0)
		{
			newNode->next = current->next;
			current->next = newNode;
			return;
		}

		current = current->next;
	}

	// If function hasn't returned by now, new node is greater than last element in the list
	newNode->next = current->next;
	current->next = newNode;
}

void handleDelete(LinkedList *list)
{
	// Temporary string variable for input and including space for NULL character
	char str[STRING_MAX + 1];

	printf("\nEnter file name to delete: ");
	readInputString(str, STRING_MAX + 1);

	Node *current = list->head;

	// If list is empty, no file can be deleted
	if (list->head == NULL)
	{
		printf("An entry for file <%s> does not exist.\n", str);
		return;
	}

	// If deleting first node in list
	if (strcmp(str, list->head->name) == 0)
	{
		printf("Deleting entry for CSV file <%s>\n", str);
		list->head = current->next;
		free(current->name);
		free(current);

		return;
	}

	// Search through all nodes to delete file
	while (current->next != NULL)
	{
		if (strcmp(str, current->next->name) == 0)
		{
			printf("Deleting entry for CSV file <%s>\n", str);
			Node *pNode = current->next->next;
			free(current->next->name);
			free(current->next);
			current->next = pNode;
			return;
		}

		current = current->next;
	}

	// If reached this point, the file does not exist
	printf("An entry for file <%s> does not exist.\n", str);
}

void handleSearch(LinkedList *list)
{
	Node *current = list->head;

	char str[STRING_MAX + 1];
	printf("\nEnter file name to find: ");
	readInputString(str, STRING_MAX + 1);

	// Search for string
	while (current != NULL)
	{
		if (strcmp(str, current->name) == 0)
		{
			printf("File <%s>\n", current->name);
			printf("Lines: %d\n", current->lines);
			printf("Runtime (ms): %d\n", current->runtime);
			printf("Memory usage (B): %d\n", current->memory_usage);
			printf("Throughput: %.2lf\n", (((double)current->lines) * 1000) / ((double)current->runtime));

			return;
		}

		current = current->next;
	}

	// If here, the file does not exist
	printf("An entry for file <%s> does not exist.\n", str);
}

void handlePrint(LinkedList *list)
{
	Node *current = list->head;

	printf("\nData entries:\n");

	if (current == NULL)
	{
		printf("There are no data entries.\n");
		return;
	}

	while (current != NULL)
	{
		printf("\nFile <%s>\n", current->name);
		printf("Lines: %d\n", current->lines);
		printf("Runtime (ms): %d\n", current->runtime);
		printf("Memory usage (B): %d\n", current->memory_usage);
		printf("Throughput: %.2lf\n", (((double)current->lines) * 1000) / ((double)current->runtime));

		current = current->next;
	}
}

void handleQuit(LinkedList *list)
{
	Node *current = list->head;

	if (current == NULL)
	{
		return;
	}

	while (current != NULL)
	{
		printf("Deleting entry for CSV file <%s>\n", current->name);
		Node *pNode = current->next;
		free(current->name);
		free(current);
		current = pNode;
		list->head = pNode;
	}
}

void readInputString(char *str, int length)
{
	int i = 0;
	char c;

	while (i < length && (c = getchar()) != '\n')
	{
		str[i++] = c;
	}

	str[i] = '\0';
}

void readInputNumber(int *number)
{
	char buf[STRING_MAX];
	readInputString(buf, STRING_MAX);
	*number = (int)strtol(buf, (char **)NULL, 10);
}

void readInputChar(char *character)
{
	char buf[2];
	readInputString(buf, 3);
	*character = buf[0];
}

LinkedList *newList()
{
	LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
	list->head = NULL;
	return list;
}

char getCommand()
{
	char command;

	printf("\nSupported commands are:\n");
	printf("  I - Insert\n");
	printf("  D - Delete\n");
	printf("  S - Search\n");
	printf("  P - Print\n");
	printf("  Q - Quit\n");

	printf("\nPlease select a command: ");
	readInputChar(&command);

	while (command != 'I' && command != 'D' && command != 'S' && command != 'P' && command != 'Q')
	{
		printf("Invalid command <%c>, please select a command: ", command);
		readInputChar(&command);
	}

	return command;
}
