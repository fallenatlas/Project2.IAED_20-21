/*
 * File: proj2.h
 * Author: Tiago Antunes
 * Description: Header file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
/* Maximum number of characters for input. */
#define MAXCHAR 65535
/* Maximum number of characters for commands. */
#define MAXCHARCMD 7
/* Number of commands */
#define N_CMD 8


/* Available commands. */
#define HELP "help"
#define QUIT "quit"
#define SET "set"
#define PRINT "print"
#define FIND "find"
#define LIST "list"
#define SEARCH "search"
#define DELETE "delete"

/* Description of the available commands. */
#define HELP_DESC "Imprime os comandos disponíveis."
#define QUIT_DESC "Termina o programa."
#define SET_DESC "Adiciona ou modifica o valor a armazenar."
#define PRINT_DESC "Imprime todos os caminhos e valores."
#define FIND_DESC "Imprime o valor armazenado."
#define LIST_DESC "Lista todos os componentes imediatos de um sub-caminho."
#define SEARCH_DESC "Procura o caminho dado um valor."
#define DELETE_DESC "Apaga um caminho e todos os subcaminhos."


/* General abstractions */
#define ROOT "/"
#define Item diretory
#define name(A) A->name
#define value(A) A->valor
#define eq(A, B) (!strcmp(A, B))
#define less(A, B) (strcmp(A, B) < 0)
#define NULLitem (NULL)

typedef struct STnode *link;

/* Struct that represents a diretory/file */
typedef struct node {
    /* Name of the diretory */
    char *name;
    /* Value of the diretory */
    char *valor;
    /* Previous created diretory in the same level */
    struct node *previous;
    /* Next diretory in the same level */
    struct node *next;
    /* Diretory that has this one as a component */
    struct node *upper;
    /* Pointer to the first diretory of the list of subdiretories */
    struct node *subdiretories;
    /* Pointer to the last diretory of the list of subdiretories */
    struct node *last_subdiretories;
    /* Tree with the subdiretories in alphabetical order */
    link *tree;
} *diretory;


/* Struct that represents the nodes of the AVL tree. */
struct STnode { 
    /* Height of the node */
    int height;
    /* Name of the node, same as the name 
    of the diretory it points to. */
    char *key;
     /* Diretory the node points to. */
    diretory value;
    /* Left node */
    struct STnode *l;
    /* Right node */
    struct STnode *r;
};


/* Functions that execute the commands. */
Item findOrCreateDir(Item, Item, char*);
Item searchSubDirs(Item, char*);
Item insertEndDir(Item, Item, Item);
void getHelp();
void setValue(diretory);
void setValueAux(Item, char**, char*, char*, int, int);
void printPathValue(Item);
void findPathValue(Item);
void findPathValueAux(Item, char**, char*, int, int);
void listSubPath(Item);
void listSubPathAux(Item, char**, char*, int, int);
void deletePath(Item);
void deletePathAux(Item, char**, char*, int, int);
void searchPath(Item);
int searchPathAux(Item, char*);

/* Functions that operate on the file system. */
Item startFS();
Item newDir(char*, char*, Item);
void insertEndDirSub(Item, Item);
void freeDir(Item);

/* Functions that operate the AVL tree. */
void visit(link);
void STinit(link*);
Item STsearch(link*, char*);
void STinsert(link*, Item);
void STdelete(link*, char*);
void STsort(link, void (*visit)(link));
void STfree(link*);

/* Auxiliary functions. */
void getInput();
char **cutPath(char **, char path[], int*);
char *stringdup(char*);
char *strConcat(char*, char*);
char *shortValue(char buf[]);
void copystr(char buf[], int, int, char*);
int isspace_char(char);
char *stringdup(char*);
