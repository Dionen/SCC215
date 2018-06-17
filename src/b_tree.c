#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <b_tree.h>
/**
Arquivo de teste de funções da árvore
FALTA AJUSTAR BTREAD E BTWRITE, (REMOÇÃO NO ARQUIVO DE INDICES??)
**/

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

  b_tree = fopen(name, "wb");
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

void split(FILE *b_tree, int key, int rrn, PAGE *old_page, int *promoted_key,
                                                int *promoted_rrn, PAGE *newpage){
  int i;
  int workkey[MAX_KEYS+1];
  int workch[MAX_KEYS+2];

  for(i = 0; i < MAX_KEYS; i++){
    workkey[i] = old_page->value[i].key;
    workch[i] = old_page->child[i];
  }
  workch[i] = old_page->child[i];

  for(int i = MAX_KEYS; key < workkey[i-1] && i > 0; i--){
    workkey[i] = workkey[i-1];
    workch[i+1] = workch[i];
  }
  workkey[i] = key;
  workch[i+1] = rrn;

  *promoted_rrn = (fileLen(b_tree) - B_TREE_HEADER)/PAGE_SIZE;
  // pageinit(newpage);

  for(int i = 0; i < MIN_KEY; i++){
    old_page->value[i].key = workkey[i];
    old_page->child[i] = workch[i];
    newpage->value[i].key = workkey[i+1+MIN_KEY];
    newpage->child[i] = workch[i+1+MIN_KEY];
    old_page->value[i+MIN_KEY].key = -1;
    old_page->child[i+MIN_KEY] = -1;
  }
  old_page->child[MIN_KEY] = workch[MIN_KEY];
  newpage->child[MIN_KEY] = workch[i+1+MIN_KEY];
  newpage->keyCounter = MAX_KEYS - MIN_KEY;
  old_page->keyCounter = MIN_KEY;
  *promoted_key = workkey[MIN_KEY];
}

int insert(FILE *b_tree, int rrn, int key, int *promoted_rrn, int *promoted_key){
  int i;
  PAGE page, newpage;
  int found, promoted;
  int pos, below_rrn, below_key;

  if(rrn == -1){
    *promoted_key = key;
    *promoted_rrn = -1;
    return 1;
  }

  // btread(rrn, &page); //necessário?
  fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * rrn, SEEK_SET);
  memcpy(&page, b_tree, PAGE_SIZE);

  for(i = 0; i < page.keyCounter && key > page.value[i].key; i++);
  pos = i; //Encontrou a posicao correta

  promoted = insert(b_tree, page.child[pos], key, &below_rrn, &below_key);
  if(!promoted) return 0;
  if(page.keyCounter < MAX_KEYS){ //Inserindo a chave promovida
    for(int i = page.keyCounter; below_key < page.value[i].key && i > 0; i--){
      page.value[i].key = page.value[i-1].key;
      page.child[i+1] = page.child[i];
    }
    page.keyCounter++;
    page.value[i].key = below_key;
    page.child[i+1] = below_rrn;

    // btwrite(rrn, &page);
    fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * rrn, SEEK_SET);
    memcpy(b_tree, &page, PAGE_SIZE);
    return 0;
  }
  else {
    split(b_tree, below_key, below_rrn, &page, promoted_key, promoted_rrn, &newpage);
    // btwrite(rrn, page);
    fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * rrn, SEEK_SET);
    memcpy(b_tree, &page, PAGE_SIZE);
    // btwrite(*promoted_rrn, &newpage);
    fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * (*promoted_rrn), SEEK_SET);
    memcpy(b_tree, &newpage, PAGE_SIZE);
    return 1;
  }
}

/*int main(int argc, char const *argv[]) {
  FILE *tree;

  tree = create_btree("tree.txt");

  return 0;
}*/
