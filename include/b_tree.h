#ifndef _BTREE_H_
#define _BTREE_H_

#include <buffer.h>

int search2(int RRN, int KEY, int *FOUND_RRN, int *FOUND_POS);

void create_btree();
int search(PAGE pg, int key, int counter, int altura);
void split(FILE *b_tree, int key, int rrn, PAGE *old_page, int *promoted_key, int *promoted_rrn, PAGE *newpage);
int insert(FILE *b_tree, int rrn, int key, int *promoted_rrn, int *promoted_key);
int insert2(FILE *b_tree, int CURRENT_RRN, int KEY, int *PROMO_R_CHILD, int *PROMO_KEY);

void converte_indice(char *filename);

#endif
