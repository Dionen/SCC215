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
#include <utils.h>
#include <stdlib.h>
#include <string.h>
#include <fila.h>
#include <buffer.h>

BUFFER *b;
int PAGE_HIT, PAGE_FAULT;

void printf_buffer(){
	printf("BUFFER ==== raiz: %d buffer: ", b->rrn_raiz);
	print_fila(b->BUFFER_POOL->inicio);
}

/** Aloca os elementos do buffer-pool na memoria principal */
void cria_buffer(){
	FILE *f = fopen(B_TREE_FNAME, "r+b");
	b = malloc(sizeof(BUFFER));
	b->raiz = malloc(PAGE_SIZE);
	
	b->rrn_raiz = get_root(f);
	fseek(f, B_TREE_HEADER + (PAGE_SIZE * b->rrn_raiz), SEEK_SET);
	fread(b->raiz, PAGE_SIZE, 1, f);
	
	init_values(f);
	
	b->BUFFER_POOL = cria_fila();
	PAGE_HIT = PAGE_FAULT = 0;
	fclose(f);
}

/** Atualiza o arquivo de saída do buffer-pool. */
void print_hit_fault(){
	FILE *fp;
	
	fp = fopen(BUFFER_FNAME, "a");
	if (fp == NULL) return;
	
	fprintf(fp, "Page fault: %d; Page hit: %d.\n", PAGE_FAULT, PAGE_HIT);
	fclose(fp);
}

/** Muda a raiz no buffer */
void muda_raiz_buffer(FILE *f, PAGE *p, int RRN){
	// Insere a raiz antiga na fila do buffer
	insert_fila(f, b->BUFFER_POOL, b->rrn_raiz, b->raiz, b->flag_raiz);
	
	// A nova toma o lugar da antiga.
	b->raiz = p;
	b->rrn_raiz = RRN; 
	b->flag_raiz = TRUE;
}

/**Retorna o endereco de uma copia de uma pagina da arvore no buffer.
 * Se a pagina nao estiver no buffer, a funcao busca a pagina no arquivo e
 * a coloca no buffer. Se o buffer estiver cheio, remove o LRU. */
PAGE *get_buffer(FILE *f, int RRN){
	PAGE *result = NULL, *p;
				
	if (RRN == b->rrn_raiz){
		// Caso a pagina requerida seja a raiz, ela ja esta no buffer.
		result = malloc(PAGE_SIZE);
		memcpy(result, b->raiz, PAGE_SIZE);
		PAGE_HIT++;
		
	} else {
		// Busca a pagina no buffer
		p = busca_fila(b->BUFFER_POOL, RRN);

		if (p == NULL){
			// Se nao encontrar a pagina no buffer, busca no arquivo.
			p = malloc(PAGE_SIZE);
			
			fseek(f, B_TREE_HEADER + (RRN*PAGE_SIZE), SEEK_SET);
			fread(p, PAGE_SIZE, 1, f);
									
			insert_fila(f, b->BUFFER_POOL, RRN, p, FALSE);  // A funcao de insercao remove o elemento LRU do buffer
															// se o buffer estiver cheio.
			result = malloc(PAGE_SIZE);
			memcpy(result, p, PAGE_SIZE);
			
			PAGE_FAULT++;
		} else {
			// Encontrou a pagina no buffer.
			result = malloc(PAGE_SIZE);
			memcpy(result, p, PAGE_SIZE);
			PAGE_HIT++;
		}
	}
	
	return result;
}

/** Insere uma pagina no buffer. Se ela ja estiver no buffer, é
 *  atualizada e marcada como alterada. */
void put_buffer(FILE *b_tree, PAGE *p, int RRN){
	NODE_F *d;
	
	if (RRN == b->rrn_raiz){
		// Se a pagina alterada for a raiz, atualiza a raiz no buffer.
		free(b->raiz);
		b->raiz = p;
		b->flag_raiz = TRUE;
	} else {
		// Procura se a pagina ja esta no buffer.
		d = busca_node_fila(b->BUFFER_POOL, RRN);		
		
		if (d != NULL) {
			// Caso a pagina ja estiver no buffer, a atualiza.
			free(d->page);
			d->page = p;
			d->flag = TRUE;					 // Marca a pagina como modificada.
			busca_fila(b->BUFFER_POOL, RRN); // Rearranja.
			PAGE_HIT++;
		} else {
			// Caso a pagina nao esteja no buffer, a insere no buffer.
			insert_fila(b_tree, b->BUFFER_POOL, RRN, p, TRUE);
		}
	}
}

/** Escreve todas as paginas modificadas do buffer no arquivo de indices */
void flush_buffer(FILE *b_tree){
	fseek(b_tree, B_TREE_HEADER + (PAGE_SIZE*b->rrn_raiz), SEEK_SET);
	fwrite(b->raiz, PAGE_SIZE, 1, b_tree);
	flush_buffer_node(b_tree, b->BUFFER_POOL->inicio);
}

void flush_buffer_node(FILE *b_tree, NODE_F *d){
	if (d != NULL){
		if (d->flag){
			fseek(b_tree, B_TREE_HEADER + (PAGE_SIZE*d->n_page), SEEK_SET);
			fwrite(d->page, PAGE_SIZE, 1, b_tree);
			d->flag = FALSE;
		}
		flush_buffer_node(b_tree, d->next);
	}
}

/** Escreve uma pagina modificada no arquivo de indices */
void flush_page(FILE *b_tree, NODE_F *d){
	if (d != NULL && d->flag){
		fseek(b_tree, B_TREE_HEADER + (PAGE_SIZE * d->n_page), SEEK_SET);
		fwrite(d->page, PAGE_SIZE, 1, b_tree);
	}
}

/** Remove todos os elementos do buffer da memoria principal */
void destroy_buffer(){
	if (b->raiz != NULL ) free(b->raiz);
	if (b->BUFFER_POOL != NULL) destroy_fila(b->BUFFER_POOL);
	free(b);
}

