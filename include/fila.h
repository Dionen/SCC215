#ifndef _FILA_H_
#define _FILA_H_

#define BUFFER_LENGTH 4
#define MAX_KEYS 9
#define MIN_KEY 4
#define B_TREE_HEADER 9 //Bytes de header
#define PAGE_SIZE 116

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
boolean remove_fila(FILA *f);
boolean insert_fila(FILA *f, int n_page, PAGE *page);
PAGE *busca_fila(FILA *f, int n_page);
NODE_F *busca_node_fila(FILA *f, int RRN);
void destroy_fila(FILA *f);
void print_fila(NODE_F *d);

#endif
