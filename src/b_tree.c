#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <b_tree.h>
#include <functions.h>

/**
Arquivo de teste de funções da árvore
FALTA AJUSTAR BTREAD E BTWRITE, (REMOÇÃO NO ARQUIVO DE INDICES??)
**/

#define NOT_FOUND 0
#define FOUND 1
#define ERROR 2
#define PROMOTION 3
#define NO_PROMOTION 4

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

void print_page(PAGE *p){
  printf("keyCounter: %d\nVALUES: ", p->keyCounter);
  for (int i = 0; i < MAX_KEYS; i++) printf("%d/%d ", p->value[i].key, p->value[i].rrn);
  printf("\nCHILDS: ");
  for (int i = 0; i < MAX_KEYS + 1; i++) printf("%d ", p->child[i]);
  printf("\n\n");
}

void create_btree(){
  FILE *b_tree;
  PAGE p;
  char status = 0;
  int raiz = 0, altura = 0;

  b_tree = fopen(B_TREE_FNAME, "wb");
  fwrite(&status, 1, 1, b_tree);
  fwrite(&raiz, 4, 1, b_tree);
  fwrite(&altura, 4, 1, b_tree);
  
  p.keyCounter = 0;
  for (int i = 0; i < MAX_KEYS; i++) {
	  p.value[i].key = -1; 
	  p.value[i].rrn = -1;
  }
  for (int i = 0; i < MAX_KEYS + 1; i++) p.child[i] = -1;
  fwrite(&p, sizeof(PAGE), 1, b_tree);

  fclose(b_tree);
}

int search(PAGE pg, int key, int counter, int altura){ //Retorna o RRN do arquivo de dados;
  if(counter > altura) return -1; //Passou da já verificou a árvore inteira

  int i = 0;
  while(i < pg.keyCounter && key > pg.value[i].key) i++; //Procura dentro da página
  if(i < pg.keyCounter && key == pg.value[i].key) return pg.value[i].rrn; //Encontrou
  else{
    FILE *fname = fopen(B_TREE_FNAME, "rb");
    fseek(fname, B_TREE_HEADER + PAGE_SIZE * pg.child[i], SEEK_SET);
    memcpy(&pg, fname, PAGE_SIZE);
    fclose(fname);
    return search(pg, key, counter++, altura);
  }
}

int search2(int RRN, int KEY, int *FOUND_RRN, int *FOUND_POS){
	PAGE p;
	int POS = 0;
	
	if (RRN == -1){
		return NOT_FOUND;
	} else {
		FILE *fname = fopen(B_TREE_FNAME, "rb");
		fseek(fname, B_TREE_HEADER + PAGE_SIZE * RRN, SEEK_SET);
		memcpy(&p, fname, PAGE_SIZE);
		fclose(fname);
		
		while (POS < p.keyCounter && KEY > p.value[POS].key) POS++;
		
		if (KEY == p.value[POS].key){
			*FOUND_RRN = RRN;
			*FOUND_POS = POS;
			return FOUND;
		} else {
			return search2(p.child[POS], KEY, FOUND_RRN, FOUND_POS);
		}
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

int insert(FILE *b_tree, int rrn, int key, int *promoted_rrn, int *promoted_key){/*
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
  }*/
}

void sort_data(DATA *vector, int size){
	int i, j;
	DATA temp;
	
	for (i = 0; i < size; i++){
		for (j = i+1; j < size; j++){
			if (vector[j].key < vector[i].key){
				temp = vector[j];
				vector[j] = vector[i];
				vector[i] = temp;
			}
		}
	}
}

int insert2(FILE *b_tree, int CURRENT_RRN, int KEY, int *PROMO_R_CHILD, int *PROMO_KEY){
	int i, RETURN_VALUE;
	PAGE page, newpage;
	int POS, P_B_RRN, P_B_KEY;

	if(CURRENT_RRN == -1){
		*PROMO_KEY = KEY;
		*PROMO_R_CHILD = -1;
		return PROMOTION;
	} else {
		fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * CURRENT_RRN, SEEK_SET);
		//printf("FSEEK INDEX. ftell: %li\n", ftell(b_tree));
		fread(&page, sizeof(PAGE), 1, b_tree);
	}
	
	for(POS = 0; POS < page.keyCounter && KEY > page.value[POS].key; POS++);
	//printf("loop INDEX. POS: %d\n", POS);
	
	if (KEY == page.value[POS].key){
		printf("ERRO: Inserção de chave duplicada.\n");
		return ERROR;
	}

	//printf("page.child[POS] = %d\tKEY = %d\tP_B_RRN: %d\tP_B_KEY: %d\n", page.child[POS], KEY, P_B_RRN, P_B_KEY);
	RETURN_VALUE = insert2(b_tree, page.child[POS], KEY, &P_B_RRN, &P_B_KEY);
	//printf("KEY = %d\tP_B_RRN: %d\tP_B_KEY: %d\n", KEY, P_B_RRN, P_B_KEY);
	
	if (RETURN_VALUE == ERROR || RETURN_VALUE == NO_PROMOTION){
		return RETURN_VALUE;
		
	} else if (page.keyCounter < MAX_KEYS) {
		page.value[page.keyCounter].key = P_B_KEY;
		page.value[page.keyCounter].rrn = CURRENT_RRN;
		
		page.keyCounter++;
		sort_data(page.value, page.keyCounter);
		
		fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * CURRENT_RRN, SEEK_SET);
		fwrite(&page, PAGE_SIZE, 1, b_tree);
		//print_page(&page);
		return NO_PROMOTION;
	}
	
  /*if(!promoted) return 0;
  if(page.keyCounter < MAX_KEYS){ //Inserindo a chave promovida
    for(int i = page.keyCounter; P_B_KEY < page.value[i].key && i > 0; i--){
      page.value[i].key = page.value[i-1].key;
      page.child[i+1] = page.child[i];
    }
    page.keyCounter++;
    page.value[i].key = P_B_KEY;
    page.child[i+1] = P_B_RRN;

    // btwrite(CURRENT_RRN, &page);
    fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * CURRENT_RRN, SEEK_SET);
    memcpy(b_tree, &page, PAGE_SIZE);
    return 0;
  }
  else {
    split(b_tree, P_B_KEY, P_B_RRN, &page, PROMO_KEY, PROMO_R_CHILD, &newpage);
    // btwrite(CURRENT_RRN, page);
    fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * CURRENT_RRN, SEEK_SET);
    memcpy(b_tree, &page, PAGE_SIZE);
    // btwrite(*PROMO_R_CHILD, &newpage);
    fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * (*PROMO_R_CHILD), SEEK_SET);
    memcpy(b_tree, &newpage, PAGE_SIZE);
    return 1;
  }*/
}

/** Le um arquivo .csv e o armazena em registros, em um novo arquivo .dat */
void converte_indice(char *filename) {
    FILE *dados, *arq, *index;
    PAGE p;
    HEADER header;
    REGISTRO_LEITURA registro;
    int counter = 0, i = 0, ROOT = 0, KEY, PROMO_R_CHILD, PROMO_KEY;
    char aux, aux_vec[87];

	index = fopen(B_TREE_FNAME, "r+b");
	if(index == NULL) {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }
    
    dados = fopen(filename, "rb");
    if(dados == NULL) {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    arq = fopen("output.dat", "wb");
    if(arq == NULL) {
        printf("Falha no carregamento do arquivo.\n");
        return;
    }

    header.status = 0;
    header.stack_top = -1;
    fwrite(&header.status, sizeof(char), 1, arq);
    fwrite(&header.stack_top, sizeof(int), 1, arq);

    int dados_size = fileLen(dados);

    while(!feof(dados)) { 												// Enquanto não for o fim do arquivo leia todo caracter
        while(fscanf(dados, "%c", &aux) && (aux != ';' && aux != 10))	// até que esse seja diferente de ';' ou quebra de linha.
            aux_vec[i++] = aux;     									// Armazena esses caracteres em aux_vec.

        counter++;			// Assim que encontrar ';' ou quebra de linha, incrementa o contador
        switch (counter) {  // que identifica a região lida.
        case 1:  // Se o contador é 1, então temos em aux_vec a prestadora do serviço
            registro.indicador1 = i;
            memcpy(registro.prestadora, &aux_vec, i); // copia na struct
            break;

        case 2: // se o contador é 2 temos a data
            if (i == 0) strncpy(registro.dataAtiv, "0000000000", 10);
            else memcpy(registro.dataAtiv, &aux_vec, i);
            break;

        case 3: // se o contador é 3, temos o código INEP
            memcpy(registro.codINEP, &aux_vec, i);
            break;

        case 4: // se o contador é 4, temos o nome da escola
            registro.indicador2 = i;
            memcpy(registro.nomeEscola, &aux_vec, i);
            break;

        case 5: // se 5, temos a cidade
            registro.indicador3 = i;
            memcpy(registro.municipio, &aux_vec, i);
            break;

        case 6: // E caso seja 6 (quando encontra a quebra de linha), temos o UF
            if (i == 0) strncpy(registro.uf, "00", 2);
            else memcpy(registro.uf, &aux_vec, 2);
            break;
        }

        i = 0;
        if(aux == 10) { 				// O último caracter encontrado foi uma quebra de linha,
            counter = 0; 				// então zeramos o contador e escrevemos no arquivo o conteúdo.
            int size = 28; 	// Tamanho dos registros fixos
            char zero = 0;
            
            int temp = atoi(registro.codINEP);
            
            //====================== INDICE ========================
            
            if (insert2(index, ROOT, temp, &PROMO_R_CHILD, &PROMO_KEY) == PROMOTION){
				fseek(index, 0, SEEK_END);
				p.keyCounter = 1;
				p.value[0].key = PROMO_KEY;
				p.value[0].rrn = (ftell(arq)-HEADER_SIZE)/REGISTER_SIZE;
				p.child[0] = ROOT;
				p.child[1] = PROMO_R_CHILD;
				ROOT = (ftell(index) - B_TREE_HEADER)/PAGE_SIZE;
				fwrite(&p, 1, sizeof(p), index);
            }
            //======================================================

            fwrite(&temp, sizeof(int), 1, arq);
            fwrite(registro.dataAtiv, 10, 1, arq);
            fwrite(registro.uf, 2, 1, arq);
            
            size += registro.indicador2;
            fwrite(&registro.indicador2, sizeof(int), 1, arq);
            if(registro.indicador2 != 0) // Se o nome é NULL, não escrevemos nada
                fwrite(registro.nomeEscola, registro.indicador2, 1, arq);

            size += registro.indicador3;
            fwrite(&registro.indicador3, sizeof(int), 1, arq);
            if(registro.indicador3 != 0)
                fwrite(registro.municipio, registro.indicador3, 1, arq);

            size += registro.indicador1;
            fwrite(&registro.indicador1, sizeof(int), 1, arq);
            if(registro.indicador1 != 0)
                fwrite(registro.prestadora, registro.indicador1, 1, arq);
            fwrite(&zero, sizeof(char), 87-size, arq); // Completa com 0 ate completar 87 bytes.

            // Chegou ao fim do arquivo
            if (ftell(dados) == dados_size) {
				
				fseek(index, 1, SEEK_SET);
				fwrite(&ROOT, sizeof(int), 1, index);
				fclose(index);
				
				// Arquivo estavel
                header.status = 1;
                rewind(arq);
                fwrite(&header.status, sizeof(char), 1, arq);
                fprintf(stdout, "Arquivo carregado.\n");
                fclose(dados); // Ao final fechamos os arquivos utilizados
                fclose(arq);
                return;
            }
        }
    }
    fclose(dados); //Ao final fechamos os arquivos utilizados
    fclose(arq);
    return;
}
