#ifndef _BTREE_H_
#define _BTREE_H_

#include <buffer.h>

#define NOT_FOUND 0
#define FOUND 1
#define ERROR 2
#define PROMOTION 3
#define NO_PROMOTION 4

int search2(int RRN, int KEY, int *FOUND_RRN, int *FOUND_POS);

void create_btree();
int search(PAGE pg, int key, int counter, int altura);
int search3(FILE *f, int RRN, int KEY, int *FOUND_RRN, int *FOUND_POS);
void split(FILE *b_tree, DATA key, int rrn, PAGE *old_page, DATA *promoted_key, int *promoted_rrn, PAGE *newpage);
int insert(FILE *b_tree, int CURRENT_RRN, DATA *KEY, int *PROMO_R_CHILD, DATA *PROMO_KEY);

void converte_indice(char *filename);

#endif
