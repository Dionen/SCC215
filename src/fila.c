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

#include <stdio.h>
#include <stdlib.h>
#include <fila.h>
#include <buffer.h>

FILA *cria_fila(){
	FILA *f;
	
	f = malloc(sizeof(FILA));
	f->size = 0;
	f->inicio = NULL;
	f->fim = NULL;
	
	return f;
}

boolean vazia_fila(FILA *f){
	if (f->size == 0) return TRUE;
	else return FALSE;
}


/**Busca e retorna o endereco de uma pagina no buffer.
 * Se encontrar, tambem rearranja a fila (coloca a pagina em ultimo).
 * Retorna NULL se nao encontrar a pagina desejada. */
PAGE *busca_fila(FILA *f, int n_page){
	NODE_F *p, *anterior = NULL;
	
	p = f->inicio;
	
	while (p != NULL){
		if (n_page == p->n_page){			
			if (f->size == 2){
				if (anterior == NULL){
					p->next->next = p;
					f->fim = p;
					f->inicio = p->next;
					p->next = NULL;	
				}
			} else if (f->size > 2){
				if (anterior != NULL){
					if (f->fim != p){
						anterior->next = p->next;
						f->fim->next = p;
						f->fim = p;
						p->next = NULL;
					}
				} else {
						f->inicio = p->next;
						f->fim->next = p;
						f->fim = p;
						p->next = NULL;
				}	
			}
			
			return p->page;
			
		} else {
			anterior = p;
			p = p->next;
		}
	}
	
	return NULL;
}

/** Busca um node e o retorna */
NODE_F *busca_node_fila(FILA *f, int RRN){
	NODE_F *p;
	
	p = f->inicio;
	
	while (p != NULL){
		if (p->n_page == RRN) return p;
		p = p->next;
	}
	return NULL;
}

/** Insere uma novo node na fila 
 *  A FLAG indica se o node deve ser marcado como *modificado* no buffer. */
boolean insert_fila(FILE *b_tree, FILA *f, int n_page, PAGE *page, boolean FLAG){
	NODE_F *p;
	
	p = malloc(sizeof(NODE_F));
	if (p == NULL) return FALSE;

	p->n_page = n_page;
	p->page = page;
	p->next = NULL;
	p->flag = FLAG;

	if (vazia_fila(f) == TRUE) {
		f->inicio = p;
		f-> fim = p;
	} else {
		f->fim->next = p;
		f->fim = p;
	}
	
	f->size++;
	if (f->size > BUFFER_LENGTH) remove_fila(b_tree, f);
	return TRUE;
}

/** Remove o primeiro node da fila */
boolean remove_fila(FILE *b_tree, FILA *f){
	NODE_F *p;
		
	if (vazia_fila(f) == TRUE) return FALSE;
	
	p = f->inicio;
	f->inicio = p->next;
	
	if (f->inicio == NULL) f->fim == NULL;
	
	f->size--;
		
	flush_page(b_tree, p);
	free(p->page);
	free(p);
	return TRUE;
}

/** Funcoes de destruicao */
void free_node_f(NODE_F *d){
	if (d->next != NULL) free_node_f(d->next);
	free(d->page);
	free(d);
}

void destroy_fila(FILA *f){
	if (vazia_fila(f) == FALSE) free_node_f(f->inicio);
	free(f);
}

/** Imprime o buffer */
void print_fila(NODE_F *d){
	if (d != NULL){
		printf("%d ", d->n_page);
		print_fila(d->next);
	} else {
		printf("\n");
	}
}
