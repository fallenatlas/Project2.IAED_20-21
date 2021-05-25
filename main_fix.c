/*
 * File: main_fix.c
 * Author: Tiago Antunes
 * Description: A program that simulates a file system in C.
*/
#include "proj2.h"

/* List of available commands and descriptions. */
const char *LIST_CMD[N_CMD] = 
{HELP, QUIT, SET, PRINT, FIND, LIST, SEARCH, DELETE};
const char *LIST_CMD_DESC[N_CMD] = 
{HELP_DESC, QUIT_DESC, SET_DESC, PRINT_DESC, FIND_DESC, LIST_DESC, SEARCH_DESC, DELETE_DESC};

const char *slash = "/";

/* Reads commands from stdin and executes the corresponding functions. */
int main() {
    char cmd[MAXCHARCMD];
    Item root = startFS();
    
    while(scanf("%s", cmd) && !eq(cmd, QUIT)) {
        if (eq(cmd, HELP))
            getHelp();
        else if (eq(cmd, SET))
            setValue(root);
        else if (eq(cmd, PRINT))
            printPathValue(root->subdiretories);
        else if (eq(cmd, FIND))
            findPathValue(root);
        else if (eq(cmd, LIST))
            listSubPath(root);
        else if (eq(cmd, SEARCH))
            searchPath(root);
        else if (eq(cmd, DELETE))
            deletePath(root);
    }
    freeDir(root);

    return 0;
}

/* Prints all available commands. */
void getHelp() {
    int i;

    for (i = 0; i < N_CMD; i++) {
        printf("%s: %s\n", LIST_CMD[i], LIST_CMD_DESC[i]);
    }
}

/* Concatenates the string src to the string dest */
char *strConcat(char *dest, char *src) {
    int len1 = strlen(src);
    int len2 = strlen(dest);
    dest = (char *) realloc(dest, sizeof(char) * (len1+len2+1));
    strcat(dest, src);

    return dest;
}

/* Divides path into the names between "/" and returns that list. */
char **cutPath(char **dirs, char path[], int *i) {
    int j=0, n=5;
    char *token;
    dirs = (char**) malloc(n*sizeof(char*));

    token = strtok(path, slash);
    while (token != NULL) {
        char *dir;
        dir = stringdup(token);
        if(j > n-1) {
            n = 2*n;
            dirs = (char**) realloc(dirs, n*sizeof(char *));
        }
        *(dirs+j) = dir;
        j++;
        token = strtok(NULL, slash);
    }

    free(token);
    *i = j-1;
    return dirs;
}

/* Adds or modifies a value in the path given by the user. */
void setValue(diretory root) {
    int i = 0;
    char buffer[MAXCHAR];
    char path[MAXCHAR];
    char buf[MAXCHAR] = {0};
    char *value = NULL;
    char **dirs = NULL;
    char *name = (char *) malloc(sizeof(char));
    name = strcpy(name, ROOT);

    /* Get input */
    fgets(buffer,MAXCHAR,stdin);
    sscanf(buffer, "%s", path);
    sscanf(buffer, "%*s%[^\n]", buf);
    value = shortValue(buf);

    /* Separate paths */
    dirs = cutPath(dirs, path, &i);

    /* Create of modifie the path */
    setValueAux(root, dirs, name, value, 0, i);
    free(value);
    while (i >= 0) {
        free(*(dirs+i));
        i--;
    }
    free(dirs);
}

/* Searches for the directory with name "name" 
   in the subdiretories of root and returns it. */
Item findOrCreateDir(Item root, Item parent, char *name) {
    root = STsearch(root->tree, name);
    if (root == NULL) {
        char *new_name = stringdup(name);
        Item new_dir = newDir(new_name, NULL, parent);
        insertEndDirSub(parent, new_dir);
        STinsert(parent->tree, new_dir);
        root = new_dir;
    }

    return root;
}

/* Finds the position and creates or modifies the directory in the given path. */
void setValueAux(Item root, char **dirs, char *name, char *value, int level, int depth) {
    name = strConcat(name, *(dirs+level));
    root = findOrCreateDir(root, root, name);

    if (level == depth) {
        free(value(root));
        value(root) = stringdup(value);
        return;
    }

    else {
        name = strConcat(name, ROOT);
        setValueAux(root, dirs, name, value, level+1, depth);
    }
}

/* Print all paths and associated values. */
void printPathValue(Item root) {
    /* While still possible go in depth. */
    while (root != NULL && root->subdiretories != NULL) {
        if (value(root) != NULL)
            printf("%s %s\n", name(root), value(root));
        printPathValue(root->subdiretories);
        break;
    }
    /* Go through all components of the path. */
    while (root != NULL) {
        if (root->subdiretories == NULL) {
            if (value(root) != NULL)
                printf("%s %s\n", name(root), value(root));
        }
        printPathValue(root->next);
        break;
    }
}

/* Prints the value stored in the path given by the user. */
void findPathValue(Item root) {
    int i=0;
    char buffer[MAXCHAR];
    char path[MAXCHAR];
    char **dirs = NULL;
    char *name = (char *) malloc(sizeof(char));
    name = strcpy(name, ROOT);

    /* Get input */
    fgets(buffer,MAXCHAR,stdin);
    sscanf(buffer, "%s", path);
    
    /* Separate paths */
    dirs = cutPath(dirs, path, &i);
    
    /* Find the path and print the value. */
    findPathValueAux(root, dirs, name, 0, i);
    while (i >= 0) {
        free(*(dirs+i));
        i--;
    }
    free(dirs);
}

/* Search for the path given and print the value associated. */
void findPathValueAux(Item root, char **dirs, char *name, int level, int depth) {
    name = strConcat(name, *(dirs+level));
    root = STsearch(root->tree, name);
    if (root == NULL) {
        printf("not found\n");
        return;
    }
    if (level == depth) {
        if (value(root) == NULL) {
            printf("no data\n");
        }
        else {
            printf("%s\n", value(root));
        }
        return;
    }
    else {
        name = strConcat(name, ROOT);
        findPathValueAux(root, dirs, name, level+1, depth);
    }
}

/* List all imediate components of a path in alphabetical order. */
void listSubPath(Item root) {
    int i=0;
    char buffer[MAXCHAR];
    char path[MAXCHAR];
    char **dirs = NULL;
    char *name = (char *) malloc(sizeof(char));
    name = strcpy(name, ROOT);

    /* Get input */
    fgets(buffer,MAXCHAR,stdin);
    if (sscanf(buffer, "%s", path) == -1) {
        STsort(*(root->tree), visit);
        return;
    }
    
    /* Separate paths */
    dirs = cutPath(dirs, path, &i);

    /* Search and print the components */
    listSubPathAux(root, dirs, name, 0, i);
    while (i >= 0) {
        free(*(dirs+i));
        i--;
    }
    free(dirs);
}

/* Search for the given path and print all the imediate
   components in alphabetical order */
void listSubPathAux(diretory root, char **dirs, char *name, int level, int depth) {
    name = strConcat(name, *(dirs+level));
    root = STsearch(root->tree, name);
    if (root == NULL) {
        printf("not found\n");
        return;
    }
    if (level == depth) {
        STsort(*(root->tree), visit);
        return;
    }
    else {
        name = strConcat(name, ROOT);
        listSubPathAux(root, dirs, name, level+1, depth);
    }
}

/* Searches for the path that has the value given by the user. */
void searchPath(diretory root) {
    char buffer[MAXCHAR] = {0};
    char *value;

    /* Get input */
    fgets(buffer, MAXCHAR, stdin);
    value = shortValue(buffer);

    /* Search path. */
    if (!searchPathAux(root->subdiretories, value)) {
        printf("not found\n");
    }
    free(value);
}

/* Search for the first diretory with the given value. */
int searchPathAux(diretory root, char *value) {
    if (root != NULL && value(root) != NULL && eq(value(root), value)) {
        printf("%s\n", name(root));
            return 1;
    }
    else {
        int in = 0; /* States if the value was found or not. */
        /* Search in depth. */
        while (root != NULL && root->subdiretories != NULL) {
            in = searchPathAux(root->subdiretories, value);
            break;
        }
        /* Search on same level. */
        while (root != NULL && in != 1) {
            in = searchPathAux(root->next, value);
            break;
        }
        return in;
    }

}

/* Deletes a path and all its subpaths. */
void deletePath(diretory root) {
    int i=0;
    char buffer[MAXCHAR];
    char path[MAXCHAR];
    char **dirs = NULL;
    char *name = (char *) malloc(sizeof(char));
    name = strcpy(name, ROOT);

    /* Get inputs */
    fgets(buffer,MAXCHAR,stdin);
    if (sscanf(buffer, "%s", path) == -1) {
        STfree(root->tree);
        root->subdiretories = NULL;
        root->last_subdiretories = NULL;
        return;
    }
    
    dirs = cutPath(dirs, path, &i);

    /* Delete the path. */
    deletePathAux(root, dirs, name, 0, i);
    while (i >= 0) {
        free(*(dirs+i));
        i--;
    }
    free(dirs);
}

/* Search and delete the given path and all its subpaths. */
void deletePathAux(diretory root, char **dirs, char *name, int level, int depth) {
    diretory target = (diretory) malloc(sizeof(struct node));
    name = strConcat(name, *(dirs+level));
    target = STsearch(root->tree, name);
    if (target == NULL) {
        printf("not found\n");
        return;
    }
    else if (level == depth) {
        if (target->previous == NULL) {
            target->upper->subdiretories = target->next;
            if (target->next != NULL) {
                target->next->previous = NULL;
            }
        }
        if (target->next == NULL) {
            target->upper->last_subdiretories = target->previous;
            if (target->previous != NULL) {
                target->previous->next = NULL;
            }
        }
        if (target->previous != NULL && target->next != NULL) {
            target->previous->next = target->next;
            target->next->previous = target->previous;
        }
        if (target->upper != NULL) {
            STdelete(target->upper->tree, target->name);
        }
        return;
    }
    else {
        name = strConcat(name, ROOT);
        deletePathAux(target, dirs, name, level+1, depth);
    }

}

/* Builds a string with no spaces on start and end. */
char *shortValue(char buf[]) {
    int len = 0;
    int new_len;
    int min, max;
    char *value = NULL;

    len = strlen(buf);
    for(min = 0; isspace_char(buf[min]); min++) {}
    for(max = len-1; isspace_char(buf[max]); max--) {}

    if (min <= max) {
        new_len = max - min + 2;
        value = (char*) malloc(new_len * sizeof(char));
        copystr(buf, min, max, value);
    }
    return value;
}

/* Copies the contents of buf between
   min and max to a new string */
void copystr(char buf[], int min, int max, char *value) {
    int i = 0;

    while(min <= max) {
        *(value + i) = buf[min];
        min++;
        i++;
    }

    *(value+i) = '\0';
}

/* Checks if character c is a space character */
int isspace_char(char c) {
    return c == ' ' || c == '\t' || c == '\0' || c == '\n' || c == '\r';
}

char *stringdup(char *orig) {
    char *cpy;
    if (orig == NULL) {
        cpy = NULL;
    }
    else {
        int len = strlen(orig);
        cpy = (char*) malloc(len+1 * sizeof(char));
        strcpy(cpy, orig);
    }
    return cpy;
}