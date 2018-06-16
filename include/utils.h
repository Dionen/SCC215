/* Universidade de Sao Paulo
 * Instituto de Ciências Matemáticas e de Computação
 * Departamento de Ciências de Computação - SCC0215 (Turma B)
 * 
 * Profa. Dra. Cristina Dutra de Aguiar Ciferri
 * PAE Joao Pedro de Carvalho Castro
 * 
 * Primeira Parte do Trabalho Prático (Parte I)
 * 
 * 10295311 - Alexis de Almeida Coutinho
 * 9897988  - Guilherme Marchetto
 * 9283607  - João Vitor Guino Rieswick
 * 9779392  - Wallace Cruz de Souza */

#ifndef _UTILS_H_
#define _UTILS_H_

#define activsz 10
#define schoratio 73
#define muniratio 27
#define HEADER_SIZE 5
#define REGISTER_SIZE 87

typedef struct registro {
    int INEP;
    char activ[activsz], FU[2], varfields[REGISTER_SIZE-4-activsz-2];
} REG;

unsigned long fileLen(FILE *);
void print_reg(REG *);

int cmp_INEP(REG *, void *);
int cmp_activation(REG *, void *);
int cmp_FU(REG *, void *FU);
int cmp_school(REG *, void *);
int cmp_municipality(REG *, void *);
int cmp_provider(REG *, void *);

#endif // _UTILS_H_
