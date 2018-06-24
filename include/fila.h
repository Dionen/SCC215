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

#ifndef _FILA_H_
#define _FILA_H_

#define BUFFER_LENGTH 4
#define MAX_KEYS 9
#define MIN_KEY (MAX_KEYS/2)
#define B_TREE_HEADER 13
#define PAGE_SIZE 116

#define STATUS_HEADER_POS 0
#define ROOT_HEADER_POS 1
#define HEIGHT_HEADER_POS 5
#define RRN_HEADER_POS 9


#define B_TREE_FNAME "b_tree.dat"
#define BUFFER_FNAME "buffer-info.txt"


typedef int boolean;

#define FALSE 0
#define TRUE 1

typedef struct data {
	int key, rrn;
} DATA;

typedef struct INFO_PAGE {  //Struct que contém todos os dados da página
	int keyCounter; 		//quantas chaves já existem nessa página
	DATA value[MAX_KEYS];   //vetores de CODINEP e RRN do arquivo de dados
	int child[MAX_KEYS+1];  //RRNs dos filhos
} PAGE;

typedef struct node_f {
	char flag;
	int n_page;
	PAGE *page;
	struct node_f *next;
} NODE_F;

typedef struct {
	int size;
	NODE_F *inicio;
	NODE_F *fim;
} FILA;

FILA *cria_fila();
boolean vazia_fila(FILA *f);
boolean remove_fila(FILE *b_tree, FILA *f);
boolean insert_fila(FILE *b_tree, FILA *f, int n_page, PAGE *page, boolean FLAG);
PAGE *busca_fila(FILA *f, int n_page);
NODE_F *busca_node_fila(FILA *f, int RRN);
void destroy_fila(FILA *f);
void print_fila(NODE_F *d);

void max_rrn(NODE_F *d, int *maior);

#endif
