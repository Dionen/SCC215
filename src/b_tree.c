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




void print_page(PAGE *p){
  printf("keyCounter: %d\nVALUES: ", p->keyCounter);
  for (int i = 0; i < MAX_KEYS; i++) printf("%d/%d ", p->value[i].key, p->value[i].rrn);
  printf("\nCHILDS: ");
  for (int i = 0; i < MAX_KEYS + 1; i++) printf("%d ", p->child[i]);
  printf("\n\n");
}

void init_page(PAGE *p){
	p->keyCounter = 0;
  for (int i = 0; i < MAX_KEYS; i++) {
	  p->value[i].key = -1;
	  p->value[i].rrn = -1;
  }
  for (int i = 0; i < MAX_KEYS + 1; i++) p->child[i] = -1;
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

  init_page(&p);
  fwrite(&p, sizeof(PAGE), 1, b_tree);

  fclose(b_tree);
}
/*
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
}*/

int search3(FILE *f, int RRN, int KEY, int *FOUND_RRN, int *FOUND_POS){
	PAGE p;
	int POS = 0;

	if (RRN == -1){
		return NOT_FOUND;
	} else {
		fseek(f, B_TREE_HEADER + PAGE_SIZE * RRN, SEEK_SET);
		fread(&p, PAGE_SIZE, 1, f);

		while (POS < p.keyCounter && KEY > p.value[POS].key) POS++;

		if (KEY == p.value[POS].key){
			*FOUND_RRN = RRN;
			*FOUND_POS = POS;
			return FOUND;
		} else {
			return search3(f, p.child[POS], KEY, FOUND_RRN, FOUND_POS);
		}
	}
}

void split(FILE *b_tree, DATA key, int rrn, PAGE *old_page, DATA *promoted_key,
                                                int *promoted_rrn, PAGE *newpage){
  int i;
  DATA workkey[MAX_KEYS+1];
  int workch[MAX_KEYS+2];

  for(i = 0; i < MAX_KEYS; i++){
    workkey[i] = old_page->value[i];
    workch[i] = old_page->child[i];
  }
  workch[MAX_KEYS] = old_page->child[MAX_KEYS];

  for(i = MAX_KEYS; key.key < workkey[i-1].key && i > 0; i--){
    workkey[i] = workkey[i-1];
    workch[i+1] = workch[i];
  }
  workkey[i] = key;
  workch[i+1] = rrn; //NÃO ESTAVA FALTANDO ESSA LINHA??

  *promoted_key = workkey[MIN_KEY];
  *promoted_rrn = (fileLen(b_tree) - B_TREE_HEADER)/PAGE_SIZE;

  for(int i = 0; i < MAX_KEYS; i++){
	  if (i < MIN_KEY){
		  old_page->value[i] = workkey[i];
		  old_page->child[i] = workch[i];
		} else {
			old_page->value[i].key = -1;
			old_page->value[i].rrn = -1;
			old_page->child[i] = -1;
		}
	}

	for(int i = 0; i < MAX_KEYS+1; i++){
	  if (i > MIN_KEY){
		  newpage->value[i-5] = workkey[i];
		  newpage->child[i-5] = workch[i];
		} else {
			newpage->value[i].key = -1;
			newpage->value[i].rrn = -1;
			newpage->child[i] = -1;
		}
	}

  old_page->child[MIN_KEY] = workch[MIN_KEY];
  newpage->child[MIN_KEY] = workch[1+MIN_KEY];

  newpage->keyCounter = 5;
  old_page->keyCounter = 4;
}

int insert(FILE *b_tree, int CURRENT_RRN, DATA *KEY, int *PROMO_R_CHILD, DATA *PROMO_KEY){
	int i, RETURN_VALUE;
	PAGE page, newpage;
	int POS, P_B_RRN;
	DATA P_B_KEY;

	if(CURRENT_RRN <= -1){ //Chegou no fim da árvore
		PROMO_KEY->key = KEY->key;
		PROMO_KEY->rrn = KEY->rrn;
		*PROMO_R_CHILD = -1;
		return PROMOTION;
	} else { // Não é o fim da árvore, então lê o nó e encontra a posição correta
		fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * CURRENT_RRN, SEEK_SET);
		fread(&page, sizeof(PAGE), 1, b_tree);
		for(POS = 0; POS < page.keyCounter && KEY->key > page.value[POS].key; POS++);
	}

	if (KEY->key == page.value[POS].key){
		printf("ERRO: Inserção de chave duplicada.\n");
		return ERROR;
	}
  //Insere a key na posição (filho) correta
	RETURN_VALUE = insert(b_tree, page.child[POS], KEY, &P_B_RRN, &P_B_KEY);
  //Inseriu e não teve promoção de chave
	if (RETURN_VALUE == ERROR || RETURN_VALUE == NO_PROMOTION){
		return RETURN_VALUE;

	} else if (page.keyCounter < MAX_KEYS) { //Ocorreu uma promoção de chave;
    for(i = page.keyCounter; i > 0 && P_B_KEY.key < page.value[i-1].key; i--){
      page.value[i].key = page.value[i-1].key;  //Ordenando as chaves
      page.value[i].rrn = page.value[i-1].rrn;
      page.child[i+1] = page.child[i];
    }
    page.value[i].key = P_B_KEY.key;
    page.value[i].rrn = P_B_KEY.rrn;
    page.child[i+1] = P_B_RRN;

		// PRECISA SER INSERIDO ORDENADO AQUI ====================================
		// page.value[page.keyCounter] = P_B_KEY; // NO MOMENTO INSERE NO PRIMEIRO LIVRE
		// page.child[page.keyCounter+1] = P_B_RRN;
		// =======================================================================

		page.keyCounter++;
		fseek(b_tree, B_TREE_HEADER + PAGE_SIZE * CURRENT_RRN, SEEK_SET);
		fwrite(&page, PAGE_SIZE, 1, b_tree);
		print_page(&page);
		return NO_PROMOTION;
	} else {
		init_page(&newpage);
		split(b_tree, P_B_KEY, P_B_RRN, &page, PROMO_KEY, PROMO_R_CHILD, &newpage);

		fseek(b_tree, B_TREE_HEADER + sizeof(PAGE)*CURRENT_RRN, SEEK_SET);
		fwrite(&page, sizeof(PAGE), 1, b_tree);
		fseek(b_tree, B_TREE_HEADER + sizeof(PAGE)*(*PROMO_R_CHILD), SEEK_SET);
		fwrite(&newpage, sizeof(PAGE), 1, b_tree);

		printf("NOVO FILHO DA ESQUEDA, RRN %d:\n", CURRENT_RRN);
		print_page(&page);
		printf("NOVO FILHO DA DIREITA, RRN %d:\n", *PROMO_R_CHILD);
		print_page(&newpage);
		return PROMOTION;
	}
}

/** Le um arquivo .csv e o armazena em registros, em um novo arquivo .dat */
void converte_indice(char *filename) {
    FILE *dados, *arq, *index;
    PAGE p;
    HEADER header;
    REGISTRO_LEITURA registro;
    int counter = 0, i = 0, ROOT = 0, PROMO_R_CHILD;
    char aux, aux_vec[87];
    DATA KEY, PROMO_KEY;

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
            int size = 28; 				// Tamanho dos registros fixos
            char zero = 0;

            int temp = atoi(registro.codINEP);
            KEY.key = temp;
            KEY.rrn = ftell(arq);

            //====================== INDICE ========================

            if (insert(index, ROOT, &KEY, &PROMO_R_CHILD, &PROMO_KEY) == PROMOTION){
				init_page(&p);
				p.keyCounter = 1;
				p.value[0] = PROMO_KEY;
				p.child[0] = ROOT;
				p.child[1] = PROMO_R_CHILD;
				ROOT = (fileLen(index) - B_TREE_HEADER)/PAGE_SIZE;
				printf("NOVA RAIZ, RRN %d\n", ROOT);
				print_page(&p);
				fseek(index, 0, SEEK_END);
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
