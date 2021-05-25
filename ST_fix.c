/*
 * File: ST_fix.c
 * Author: Tiago Antunes
 * Description: AVL tree implementation.
*/
#include "proj2.h"

/* Create a new node */
link NEW(Item item, link l, link r) {
    link x = (link) malloc(sizeof(struct STnode));
    x->key = stringdup(name(item));
    x->value = (diretory) malloc(sizeof(struct node)); 
    x->value = item;
    x->l = (link) malloc(sizeof(struct STnode));
    x->l = l;
    x->r = (link) malloc(sizeof(struct STnode));
    x->r = r;
    x->height = 1;
    return x;
}

/* Iniciate the tree */
void STinit(link *head) {
    *head = NULL;
}

/* Return the height of the node */
int height(link h) {
    if (h == NULL) return 0;
    return h->height;
}

/* Updates the hight of the node */
void update_height(link h) {
    int height_left = height(h->l);
    int height_right = height(h->r);
    h->height = height_left > height_right ? height_left + 1 : 
    height_right + 1;
}

/* Rotates the node to the left */
link rotL(link h) {
    link x = h->r;
    h->r = x->l;
    x->l = h;
    update_height(h);
    update_height(x);
    return x;
}

/* Rotates the node to the right */
link rotR(link h) {
    link x = h->l;
    h->l = x->r;
    x->r = h;
    update_height(h);
    update_height(x);
    return x;
}

/* Applies a left and then right
   rotation to the tree */
link rotLR(link h) {
    if (h == NULL) return h;
    h->l = rotL(h->l);
    return rotR(h);
}

/* Applies a right and then left
   rotation to the tree. */
link rotRL(link h) {
    if (h == NULL) return h;
    h->r = rotR(h->r);
    return rotL(h);
}

/* Return the balance of the link */
int Balance(link h) {
    if (h == NULL) return 0;
    return height(h->l)-height(h->r);
}

/* Balances the AVL tree */
link AVLbalance(link h) {
    int balanceFactor;
    if (h == NULL) return h;
    balanceFactor = Balance(h);
    if (balanceFactor>1) {
        if (Balance(h->l)>=0)
            h = rotR(h);
        else
            h = rotLR(h);
    }
    else if(balanceFactor<-1) {
        if (Balance(h->r)<=0)
            h = rotL(h);
        else   
            h = rotRL(h);
    }
    else   
        update_height(h);
    return h;
}

/* Inserts a node in the tree */
link insertR(link h, Item item) {
    if (h == NULL) {
        return NEW(item, NULL, NULL);
    }
    if (less(name(item), h->key))
        h->l = insertR(h->l, item);
    else
        h->r = insertR(h->r, item);
    h = AVLbalance(h);
    return h;
}

/* Calls the function that inserts
   the node in the tree */
void STinsert(link *head, Item item) {
    *head = insertR(*head, item);
}

/* Return the max value of the
   left branch of the node */
link max(link h) {
    if (h == NULL || h->r == NULL)
        return h;
    else    
        return max(h->r);
}

/* Deletes a node from the tree */
link deleteR(link h, char *name) {
    if (h == NULL) return h;
    else if (less(name, h->key))
        h->l = deleteR(h->l, name);
    else if (less(h->key, name))
        h->r = deleteR(h->r, name);
    else {
        if (h->l != NULL) {
            link aux = max(h->l);
            {Item x;
            char *tmp = NULL;
            x = h->value;
            h->value = aux->value;
            tmp = stringdup(h->key);
            h->key = aux->key;
            aux->key = tmp;
            aux->value = x;}
            h->l = deleteR(h->l, aux->key);
        }
        else {
            link aux = (link) malloc(sizeof(struct STnode));
            aux = h;
            if (h->l == NULL && h->r == NULL)
                h = NULL;
            else if (h->l == NULL)
                h = h->r;
            else 
                h = h->l;
            free(aux->key);
            freeDir(aux->value);
            free(aux);
        }
    }
    h = AVLbalance(h);
    return h;
}

/* Calls the function that deletes
   the node from the tree */
void STdelete(link* head, char *name) {
    *head = deleteR(*head, name);
}

/* Searches for a link with the key equal to name */
Item searchR(link h, char *name) {
    if (h == NULL) 
        return NULLitem;
    if (eq(name, h->key))
        return h->value;
    if (less(name, h->key))
        return searchR(h->l, name);
    else
        return searchR(h->r, name);
}

/* Calls the function that searches
   for the node with the key name */
Item STsearch(link *head, char *name) {
    return searchR(*head, name);
}

/* Prints the name of the diretory without the path before it */
void visit(link h) {
    int i = 1;
    int len = strlen(h->key);
    while (i <= len && (*(h->key+i) == *(h->value->upper->name+i))) {
        ++i;
    }
    if (i > 1)
        i++;
    printf("%s\n", (h->key+i));
}

/* Visits all nodes of the tree in order */
void sortR(link h, void (*visit)(link)) {
    if (h == NULL)
        return;
    sortR(h->l, visit);
    visit(h);
    sortR(h->r, visit);
}

/* Calls the function that prints all the
   values in the tree in order */
void STsort(link head, void (*visit)(link)) {
    sortR(head, visit);
}

/* Visits and deletes all nodes */
link freeR(link h) {
    if (h == NULL) {
        return h;
    }
    h->l = freeR(h->l);
    h->r = freeR(h->r);
    return deleteR(h, h->key);
}

/* Free the tree */
void STfree(link *head) {
    *head = freeR(*head);
}