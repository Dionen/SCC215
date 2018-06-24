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

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <fila.h>

typedef struct buffer {
	char flag_raiz;				// Flag modificacao raiz
	int rrn_raiz;			    // RRN da pagina da raiz do indice.
	PAGE *raiz;
	FILA *BUFFER_POOL;
} BUFFER;

void cria_buffer();
void muda_raiz_buffer(FILE *f, PAGE *p, int RRN);

PAGE *get_buffer(FILE *, int);
void put_buffer(FILE *b_tree, PAGE *p, int RRN);

void flush_buffer(FILE *b_tree);
void flush_buffer_node(FILE *b_tree, NODE_F *d);
void flush_page(FILE *b_tree, NODE_F *d);

void printf_buffer();
void destroy_buffer();
void print_hit_fault();

#endif
