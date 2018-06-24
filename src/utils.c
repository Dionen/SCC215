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
#include "utils.h"

static int LAST_RRN;
static int HEIGHT;

void init_values(FILE *f){
	fseek(f, 5, SEEK_SET);
	fread(&HEIGHT, sizeof(int), 1, f);
	fread(&LAST_RRN, sizeof(int), 1, f);
}

int get_root(FILE *f){
	int a;
	fseek(f, 1, SEEK_SET);
	fread(&a, sizeof(int), 1, f);
	return a;
}

void set_root(FILE *f, int RRN){
	fseek(f, 1, SEEK_SET);
	fwrite(&RRN, sizeof(int), 1, f);
}

/** Retorna o valor do ultimo RRN adicionado ao arquivo de indices 
 * TAO RECEBENDO FILE COMO ARGUMENTO PQ É SOBRA DE UMA VERSÃO ANTIGA E N TO AFIM DE MUDAR TUDO*/
int get_last_rrn(FILE *f){
	return LAST_RRN;
}

void set_last_rrn(FILE *f, int RRN){
	LAST_RRN = RRN;
}

int get_height(){
	return HEIGHT;
}

/** Aumenta o valor da altura no header do arquivo de indices */
void increase_height(FILE *f){
	HEIGHT++;
}

/** Retorna o tamanho de dado arquivo. */
unsigned long fileLen(FILE *fp){
	long size;

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	return size;
}

/** Imprime um registro */
void print_reg(REG *school) {
    printf("%u ", school->INEP);
    fwrite(school->activ, activsz, 1, stdout);
    printf(" ");
    fwrite(school->FU, 2, 1, stdout);
    printf(" ");

    for(unsigned char i = 0, *varfield = school->varfields; i < 3; i++) {
		printf("%d ", *(int*)varfield);
        fwrite(varfield+4, *(int*)varfield, 1, stdout);
        if (*(int*)varfield != 0 && i < 2) printf(" ");
        varfield += 4+*(int*)varfield;
    }
    printf("\n");
}

/** Funcoes de comparacao de campos */
int cmp_INEP(REG *school, void *INEP) {
    return school->INEP == *(unsigned int*)INEP;
}

int cmp_activation(REG *school, void *activ) {
    return !memcmp(school->activ, activ, 10);
}

int cmp_FU(REG *school, void *FU) {
    return !memcmp(school->FU, FU, 2);
}

int cmp_school(REG *school, void *scho) {
    return !memcmp(school->varfields+4, scho, strlen(scho));
}

int cmp_municipality(REG *school, void *muni) {
    unsigned char *varfield = school->varfields+4+*(int*)school->varfields;

    return !memcmp(varfield+4, muni, strlen(muni));
}

int cmp_provider(REG *school, void *prov) {
    unsigned char *varfield = school->varfields+4+*(int*)school->varfields;

    varfield += 4+*(int*)varfield;
    return !memcmp(varfield+4, prov, strlen(prov));
}
