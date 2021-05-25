/*
 * File: file_system_fix.c
 * Author: Tiago Antunes
 * Description: Functions that manipulate the file system.
*/
#include "proj2.h"

/* Creates the root diretory. */
Item startFS() {
    Item new_root = (Item) malloc(sizeof(struct node));
    new_root = newDir(ROOT, NULLitem, NULLitem);

    return new_root;
}

/* Create a new diretory and return a pointer to it. */
Item newDir(char *name, char *value, diretory upper) {
    Item new_dir = (Item) malloc(sizeof(struct node));
    name(new_dir) = stringdup(name);
    value(new_dir) = (char *) malloc(sizeof(char));
    if(value == NULL) {
        value(new_dir) = NULL;
    }
    else {
        free(value(new_dir));
        value(new_dir) = stringdup(value);
    }
    new_dir->next = (Item) malloc(sizeof(struct node));
    new_dir->next = NULL;
    new_dir->previous = (Item) malloc(sizeof(struct node));
    new_dir->previous = NULL;
    new_dir->upper = upper;
    new_dir->subdiretories = NULL;
    new_dir->last_subdiretories = NULL;
    new_dir->tree = (link*) malloc(sizeof(link));
    STinit(new_dir->tree);

    return new_dir;
}

/* Inserts the diretory "next" to the end of the list
   of subdiretories of the diretory "upper" */
void insertEndDirSub(Item upper, Item next) {
    if (upper->subdiretories == NULLitem) {
        upper->subdiretories = next;
    }
    else {
        next->previous = upper->last_subdiretories;
        upper->last_subdiretories->next = next;
    }
    upper->last_subdiretories = next;
}

/* Frees the diretory */
void freeDir(Item dir) {
    STfree(dir->tree);
    free(name(dir));
    free(value(dir));
    free(dir);
}
