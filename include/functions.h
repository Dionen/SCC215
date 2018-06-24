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

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <utils.h>

typedef struct Register {
    int codINEP;
    char dataAtiv[10];
    char uf[2];
    char *nomeEscola;
    char *municipio;
    char *prestadora;
} REGISTRO;

typedef struct reg{
  char codINEP[16], dataAtiv[10];
  char uf[2];
  int indicador1, indicador2, indicador3;
  char nomeEscola[60], municipio[60], prestadora[60];
} REGISTRO_LEITURA;

typedef struct head{
	char status;
	int stack_top;
} HEADER;

void procura_campo(char *key, char *value);
void desfragmentacao(void);
void converte(char *);
void remove_RRN(char *);
void insere(char *, char *, char *, char *, char *, char *);
void atualiza(char *, char *, char *, char *, char *, char *, char *);
void imprime_arquivo(void);
void imprime_RRN(char *);
void imprime_pilha(void);

#endif // _FUNCTIONS_H_
