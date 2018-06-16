#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
PÁGINA DE TESTE DAS FUNÇÕES DA ÁRVORE B
ESCREVENDO INSERT....
ASSIM QUE FOR FINALIZADO ESSA PÁGINA SERÁ ATUALIZADA
**/

#define MAX_KEYS 9
#define B_TREE_HEADER 9 //Bytes de header
#define PAGE_SIZE 116
#define B_TREE_FNAME "b_tree.dat"

typedef struct data{
  int key, rrn;
}DATA;

typedef struct INFO_PAGE{ //Struct que contém todos os dados da página
  int keyCounter; //quantas chaves já existem nessa página
  DATA value[MAX_KEYS]; //vetores de CODINEP e RRN do arquivo de dados
  int child[MAX_KEYS+1]; //RRNs dos filhos
}PAGE;

//char *disk_read(char *name, int pos, int tam){
//   char *result;
//   result = (char *) malloc(sizeof(char) * tam);
//
//   fname = fopen(name, "r");
//   fseek(fname, pos, SEEK_SET);
//
//   memcpy(result, fname, tam);
//   return result;
// }

FILE *create_btree(char *name){
  FILE *b_tree;
  char status = 0;
  int raiz = 0, altura = 0;

  b_tree = fopen(name, "w");
  fwrite(&status, 1, 1, b_tree);
  fwrite(&raiz, 4, 1, b_tree);
  fwrite(&altura, 4, 1, b_tree);

  fclose(b_tree);
  return b_tree;
}

int search(PAGE pg, int key, int counter, int altura){ //Retorna o RRN do arquivo de dados;
  if(counter > altura) return -1; //Passou da já verificou a árvore inteira

  int i = 0;
  while(i < pg.keyCounter && key > pg.value[i].key) i++; //Procura dentro da página
  if(i < pg.keyCounter && key == pg.value[i].key) return pg.value[i].rrn; //Encontrou
  else{
    FILE *fname = fopen(B_TREE_FNAME, "r");
    fseek(fname, B_TREE_HEADER + PAGE_SIZE * pg.child[i], SEEK_SET);
    memcpy(&pg, fname, PAGE_SIZE);
    fclose(fname);
    return search(pg, key, counter++, altura);
  }
}

void insert(short rrn, char key, short *promo_r_child, char *promo_key){
  PAGE pgm newpg;
  int found, promoted;
  short pos, p_b_rrn;
  char p_b_key;

  if(rrn == NIL){
    *promo_key = key;
    *promo_r_child = NIL;
    return YES;
  }
  btread(rrn, &page);
  found = search_node(key, &page, &pos);
  if(found){
    printf("Error: key já inserida\n");
    return 0;
  }
  promoted = insert(page.child[pos], key, &p_b_rrn, &p_b_key);
  if(!promoted) return NO;
  if(page.keyconut < MAX_KEYS){
    ins_in_page(p_b_key, p_b_rrn, &page);
    btwrite(rrn, &page);
    return NO;
  }
  else{
    split(p_b_key, p_b_rrn, &page, promo_key, prmo_r_child, &newpage);
    btwrite(rrn, &page);
    btwrite(*promo_r_child, &newpage);
    return YES;
  }
}

int main(int argc, char const *argv[]) {
  FILE *tree;

  tree = create_btree("tree.txt");

  return 0;
}
