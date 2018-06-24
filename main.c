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
#include <string.h>
#include <functions.h>
#include <utils.h>
#include <buffer.h>
#include <b_tree.h>

/** A funcao main le a funcionalidade escolhida e chama uma funcao respectiva a mesma */
int main(int argc, char* argv[]) {
    int funcionalidade = atoi(argv[1]);

    switch (funcionalidade) {
    case 1:
        converte(argv[2]);
        break;
    case 2:
        imprime_arquivo();
        break;
    case 3:
		procura_campo(argv[2], argv[3]);
        break;
    case 4:
        imprime_RRN(argv[2]);
        break;
    case 5:
        remove_RRN(argv[2]);
        break;
    case 6:
        insere(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
        break;
    case 7:
        atualiza(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
        break;
    case 8:
		desfragmentacao();
        break;
    case 9:
        imprime_pilha();
        break;
    case 10:
		create_btree();
   		cria_buffer();
		converte_indice(argv[2]);
		print_hit_fault();
		destroy_buffer();
		break;
	case 11:
   		cria_buffer();
        //insere_indice(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
		print_hit_fault();
		destroy_buffer();
		break;
	case 12:
		break;
    /*case 15:
		
		// TESTE DO BUFFER
		cria_buffer();
		
		BUFFER *b = bbuffer();
		
		get_buffer(8);
		print_fila(b->BUFFER_POOL->inicio); // nojeira, mas pra teste funciona
		get_buffer(2);
		print_fila(b->BUFFER_POOL->inicio);
		get_buffer(1);
		print_fila(b->BUFFER_POOL->inicio);
		put_buffer(2);
		print_fila(b->BUFFER_POOL->inicio);
		get_buffer(3);
		print_fila(b->BUFFER_POOL->inicio);
		put_buffer(1);
		print_fila(b->BUFFER_POOL->inicio);
		get_buffer(5);
		print_fila(b->BUFFER_POOL->inicio);
		get_buffer(9);
		print_fila(b->BUFFER_POOL->inicio);
		get_buffer(1);
		print_fila(b->BUFFER_POOL->inicio);
		print_hit_fault();
		
		destroy_buffer(b);
		break;*/
    default:
        break;
    }
    return 0;
}
