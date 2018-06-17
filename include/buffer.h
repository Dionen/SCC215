#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <fila.h>

typedef struct buffer {
	char flag_raiz;				// Flag modificacao raiz
	int rrn_raiz;			// RRN da pagina da raiz do indice.
	PAGE *raiz;
	FILA *BUFFER_POOL;
} BUFFER;

BUFFER *bbuffer();
void cria_buffer();
void muda_raiz_buffer(int);

PAGE *get_buffer(int);
void put_buffer(int);

void flush_buffer();
void flush_buffer_node(NODE_F *d);
void flush_page(NODE_F *d);

void destroy_buffer();
void print_hit_fault();

#endif
