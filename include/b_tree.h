/* Universidade de Sao Paulo
 * Instituto de Ciências Matemáticas e de Computação
 * Departamento de Ciências de Computação - SCC0215 (Turma B)
 * 
 * Profa. Dra. Cristina Dutra de Aguiar Ciferri
 * PAE Joao Pedro de Carvalho Castro
 * 
 * Primeira Parte do Trabalho Prático (Parte II)
 * 
 * 10295311 - Alexis de Almeida Coutinho
 * 9897988  - Guilherme Marchetto
 * 9283607  - João Vitor Guino Rieswick
 * 9779392  - Wallace Cruz de Souza */

#ifndef _BTREE_H_
#define _BTREE_H_

#include <buffer.h>

void create_btree();
int search(FILE *f, int RRN, int KEY, int *FOUND_RRN, int *FOUND_POS);
void split(FILE *b_tree, DATA key, int rrn, PAGE *old_page, DATA *promoted_key, int *promoted_rrn, PAGE *newpage);
int insert(FILE *b_tree, int CURRENT_RRN, DATA *KEY, int *PROMO_R_CHILD, DATA *PROMO_KEY);

void converte_indice(char *filename);
void insere_indice(char *argv2, char *argv3, char *argv4, char *argv5, char *argv6, char *argv7);
void search_indice(char *key);


#endif
