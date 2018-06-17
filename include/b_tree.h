#ifndef _BTREE_H_
#define _BTREE_H_

#include <buffer.h>

FILE *create_btree(char *name);
int search(PAGE pg, int key, int counter, int altura);
void split(FILE *b_tree, int key, int rrn, PAGE *old_page, int *promoted_key, int *promoted_rrn, PAGE *newpage);
int insert(FILE *b_tree, int rrn, int key, int *promoted_rrn, int *promoted_key);

#endif
