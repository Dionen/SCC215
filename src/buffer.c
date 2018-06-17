#include <stdio.h>
#include <stdlib.h>
#include <fila.h>
#include <buffer.h>

BUFFER *b;
int PAGE_HIT, PAGE_FAULT;

/* FUNCAO SO PARA TESTE */
BUFFER *bbuffer(){
	return b;
}

/** Aloca os elementos do buffer-pool na memoria principal */
void cria_buffer(){
	b = malloc(sizeof(BUFFER));
	b->rrn_raiz = 0;
	b->raiz = NULL;
	b->BUFFER_POOL = cria_fila();
	PAGE_HIT = PAGE_FAULT = 0;
}

/** Atualiza o arquivo de saída do buffer-pool. */
void print_hit_fault(){
	FILE *fp;
	
	fp = fopen("buffer-info.txt", "a");
	if (fp == NULL) return;
	
	fprintf(fp, "Page fault: %d; Page hit: %d.\n", PAGE_FAULT, PAGE_HIT);
	fclose(fp);
}

/** Muda a raiz no buffer */
void muda_raiz_buffer(int RRN){
	free(b->raiz); // Deve-se colocar a raiz antiga na fila do buffer? se sim tem que mudar essa aqui.
	b->raiz = get_buffer(RRN);
	b->rrn_raiz = RRN; 
}

/**Retorna o endereco de uma pagina da arvore no buffer.
 * Alteracoes na pagina alterarao tambem o buffer!
 * Deve-se chamar put_buffer apos ou antes das alteracoes para marca-la como alterada. */
PAGE *get_buffer(int RRN){
	PAGE *p;
	
	if (RRN == b->rrn_raiz){
		return b->raiz;
	} else {
		p = busca_fila(b->BUFFER_POOL, RRN);

		if (p == NULL){
			// TO DO: FAZ ACESSO A DISCO E PEGA O RRN DO ARQUIVO DE INDICE
			insert_fila(b->BUFFER_POOL, RRN, 10);	//somente para teste
			
			PAGE_FAULT++;
			return NULL; //para teste
		} else {
			PAGE_HIT++;
			return p;
		}
	}
}

/** Marca uma pagina do buffer como alterada */
void put_buffer(int RRN){
	NODE_F *d;
	
	if (RRN == b->rrn_raiz){
		b->flag_raiz = TRUE;
	} else {
		d = busca_node_fila(b->BUFFER_POOL, RRN);		
		
		if (d != NULL) {
			d->flag = TRUE;					 // Marca a pagina como modificada.
			busca_fila(b->BUFFER_POOL, RRN); // Rearranja.
			PAGE_HIT++;
		}
	}
}

/** Escreve todas as paginas modificadas do buffer no arquivo de indices */
void flush_buffer(){
	if (b->flag_raiz){
		// TO DO: ATUALIZA A PAGINA NO ARQUIVO DE INDICE
		b->flag_raiz = FALSE;
	}
	flush_buffer_node(b->BUFFER_POOL->inicio);
}

void flush_buffer_node(NODE_F *d){
	if (d != NULL){
		if (d->flag){
			// TO DO: ATUALIZA A PAGINA NO ARQUIVO DE INDICE
			d->flag = FALSE;
		}
		flush_buffer_node(d->next);
	}
}

/** Escreve uma pagina modificada no arquivo de indices */
void flush_page(NODE_F *d){
	if (d != NULL && d->flag){
		// TO DO: ATUALIZA A PAGINA NO ARQUIVO DE INDICE
		d->flag = FALSE;
	}
}

/** Remove todos os elementos do buffer da memoria principal */
void destroy_buffer(){
	//free(b->raiz);
	destroy_fila(b->BUFFER_POOL);
	free(b);
}

