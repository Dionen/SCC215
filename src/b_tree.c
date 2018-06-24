#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <b_tree.h>
#include <functions.h>

#define NOT_FOUND 0
#define FOUND 1
#define ERROR 2
#define PROMOTION 3
#define NO_PROMOTION 4



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
  int raiz = 0, altura = 0, last_rrn = 0;

  b_tree = fopen(B_TREE_FNAME, "wb");
  fwrite(&status, 1, 1, b_tree);
  fwrite(&raiz, 4, 1, b_tree);
  fwrite(&altura, 4, 1, b_tree);
  fwrite(&last_rrn, 4, 1, b_tree);  

  init_page(&p);
  fwrite(&p, sizeof(PAGE), 1, b_tree);

  fclose(b_tree);
}

int search(FILE *f, int RRN, int KEY, int *FOUND_RRN, int *FOUND_POS){
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
			return search(f, p.child[POS], KEY, FOUND_RRN, FOUND_POS);
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
  *promoted_rrn = get_last_rrn(b_tree)+1;
  set_last_rrn(b_tree, *promoted_rrn);
  
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
	old_page->child[MIN_KEY] = workch[MIN_KEY];
	old_page->child[9] = -1;	//porque o 'for' não chega até o final

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
	newpage->child[10-5] = workch[10];		//como tem mais child do que value, o 'for' anterior não modifica o ultimo valor
  newpage->keyCounter = 5;
  old_page->keyCounter = 4;
}

int insert(FILE *b_tree, int CURRENT_RRN, DATA *KEY, int *PROMO_R_CHILD, DATA *PROMO_KEY){
	int i, RETURN_VALUE;
	PAGE *page, *newpage;
	int POS, P_B_RRN;
	DATA P_B_KEY;
	
	/*printf_buffer();
	printf("CURRENT RRN: %d\n", CURRENT_RRN);*/

	if(CURRENT_RRN <= -1){ 
		// Chegou no fim da árvore
		PROMO_KEY->key = KEY->key;
		PROMO_KEY->rrn = KEY->rrn;
		*PROMO_R_CHILD = -1;
		return PROMOTION;	
	} else { 
		// Não é o fim da árvore, então lê o nó e procura a posição correta		
		page = get_buffer(b_tree, CURRENT_RRN);	
		for(POS = 0; POS < page->keyCounter && KEY->key > page->value[POS].key; POS++);
	}

	if (KEY->key == page->value[POS].key){
		return ERROR;
	}
	
	//Insere a key na posição (filho) correta
	RETURN_VALUE = insert(b_tree, page->child[POS], KEY, &P_B_RRN, &P_B_KEY);
	
	//Inseriu e não teve promoção de chave
	if (RETURN_VALUE == ERROR || RETURN_VALUE == NO_PROMOTION){
		return RETURN_VALUE;

	} else if (page->keyCounter < MAX_KEYS) { 
		for(i = page->keyCounter; i > 0 && P_B_KEY.key < page->value[i-1].key; i--){
			page->value[i].key = page->value[i-1].key;
			page->value[i].rrn = page->value[i-1].rrn;
			page->child[i+1] = page->child[i];
		}
		page->value[i].key = P_B_KEY.key;
		page->value[i].rrn = P_B_KEY.rrn;
		page->child[i+1] = P_B_RRN;

		page->keyCounter++;
		put_buffer(b_tree, page, CURRENT_RRN);
		//print_page(page);
		
		return NO_PROMOTION;
	} else {
		//Ocorreu uma promoção de chave;
		newpage = malloc(PAGE_SIZE);
		init_page(newpage);

		split(b_tree, P_B_KEY, P_B_RRN, page, PROMO_KEY, PROMO_R_CHILD, newpage);

		put_buffer(b_tree, page, CURRENT_RRN);
		put_buffer(b_tree, newpage, *PROMO_R_CHILD);
		
		/*printf("NOVO FILHO DA ESQUEDA, RRN %d:\n", CURRENT_RRN);
		print_page(page);
		printf("NOVO FILHO DA DIREITA, RRN %d:\n", *PROMO_R_CHILD);
		print_page(newpage);*/
		
		return PROMOTION;
	}
}

/** Le um arquivo .csv e o armazena em registros, em um novo arquivo .dat */
void converte_indice(char *filename) {
    FILE *dados, *arq, *index;
    PAGE *p;
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
            KEY.key = temp; //Correção RRN
            KEY.rrn = (ftell(arq) - HEADER_SIZE) / REGISTER_SIZE;

            //====================== INDICE ========================

            if (insert(index, ROOT, &KEY, &PROMO_R_CHILD, &PROMO_KEY) == PROMOTION){
				p = malloc(PAGE_SIZE);
				init_page(p);
				p->keyCounter = 1;
				p->value[0] = PROMO_KEY;
				p->child[0] = ROOT;
				p->child[1] = PROMO_R_CHILD;
				ROOT = get_last_rrn(index)+1;			// Obtem o RRN da nova pagina
				/*printf("NOVA RAIZ, RRN %d\n", ROOT);
				print_page(p);*/
				
				muda_raiz_buffer(index, p, ROOT);		// Atualiza a raiz no buffer			
				set_last_rrn(index, ROOT);				// Atualiza LAST_RRN
				increase_height(index);				    // Incrementa a altura da arvore
				
				//printf("RAIZ MODIFICADA.\n");
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
				// falta escrever o status
				header.status = 1;
				rewind(index);
                fwrite(&header.status, sizeof(char), 1, index);
				int altura = get_height(index);
				int last_rrn = get_last_rrn(index);
				
				set_root(index, ROOT);  					// Escreve a raiz no cabecalho
				fwrite(&altura, sizeof(int), 1, index);		// Escreve a altura
				fwrite(&last_rrn, sizeof(int), 1, index);	// e o LAST_RRN
				
				flush_buffer(index);						// Tudo que esta no buffer vai para o arquivo
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

/** Insere um novo registro no arquivo de dados, com base no topo da pilha */
void insere_indice(char *argv2, char *argv3, char *argv4, char *argv5, char *argv6, char *argv7) {
    FILE *fp, *index;
    int stack, stack_temp, size, fix_size = 28, codINEP_temp = atoi(argv2);
    char dataAtiv_temp[10], UF_temp[2];
    char *nomeEscola_temp = argv5;
    char *municipio_temp = argv6;
    char *prestadora_temp = argv7;
    char status = 0, espaco = 0;
    
    int saida, ROOT, PROMO_R_CHILD;
    DATA KEY, PROMO_KEY;
    PAGE *p;

    // Tratamento de entradas nulas
    if (argv3 != NULL) strncpy(dataAtiv_temp, argv3, 10);
    else strncpy(dataAtiv_temp, "0000000000", 10);

    if (argv4 != NULL) strncpy(UF_temp, argv4, 2);
    else strncpy(UF_temp, "00", 2);

    fp = fopen("output.dat", "r+b");
    index = fopen(B_TREE_FNAME, "r+b");
    
    if(fp == NULL) {
        fprintf(stdout, "Falha no processamento do arquivo.\n");

    } else {
		// Arquivo instavel
		fwrite(&status, 1, 1, fp);
        // Checa-se se ha registros removidos.
        fread(&stack, sizeof(int), 1, fp);

        if (stack == -1) {
            // Caso nenhum registro tenha sido removido, insere no final.
            fseek(fp, 0, SEEK_END);

        } else {
            // Caso contrario, atualize o cabecalho e va para o registro removido.
            fseek(fp, stack*REGISTER_SIZE + HEADER_SIZE + sizeof(int), SEEK_SET);
            fread(&stack_temp, sizeof(int), 1, fp);
            fseek(fp, 1, SEEK_SET);
            fwrite(&stack_temp, sizeof(int), 1, fp);
            fseek(fp, stack*REGISTER_SIZE + HEADER_SIZE, SEEK_SET);
        }
        
        KEY.key = codINEP_temp;
        KEY.rrn = (ftell(fp) - HEADER_SIZE) / REGISTER_SIZE;

        //====================== INDICE ========================

		saida = insert(index, ROOT, &KEY, &PROMO_R_CHILD, &PROMO_KEY);
        if (saida == PROMOTION){
			p = malloc(PAGE_SIZE);
			init_page(p);
			p->keyCounter = 1;
			p->value[0] = PROMO_KEY;
			p->child[0] = ROOT;
			p->child[1] = PROMO_R_CHILD;
			ROOT = get_last_rrn(index)+1;			// Obtem o RRN da nova pagina
			/*printf("NOVA RAIZ, RRN %d\n", ROOT);
			print_page(p);*/
				
			muda_raiz_buffer(index, p, ROOT);		// Atualiza a raiz no buffer			
			set_last_rrn(index, ROOT);				// Atualiza LAST_RRN
			increase_height(index);				    // Incrementa a altura da arvore
				
			//printf("RAIZ MODIFICADA.\n");
        }
        //======================================================
        
        // Escreve os campos fixos no arquivo.
        fwrite(&codINEP_temp, sizeof(int), 1, fp);
        fwrite(&dataAtiv_temp, sizeof(char), 10, fp);
        fwrite(&UF_temp, sizeof(char), 2, fp);

        // Escreve o tamanho do nome e o nome da escola.
        size = strlen(nomeEscola_temp);
        fix_size += size;
        fwrite(&size, sizeof(int), 1, fp);
        fwrite(nomeEscola_temp, sizeof(char), size, fp);

        // Escreve o tamanho do nome e o nome do municipio.
        size = strlen(municipio_temp);
        fix_size += size;
        fwrite(&size, sizeof(int), 1, fp);
        fwrite(municipio_temp, sizeof(char), size, fp);

        // Escreve o tamanho do nome e o nome da prestadora.
        // Completa o restante do registro com espacos.
        size = strlen(prestadora_temp);
        fix_size += size;
        fwrite(&size, sizeof(int), 1, fp);
        fwrite(prestadora_temp, sizeof(char), size, fp);
        fwrite(&espaco, sizeof(char), REGISTER_SIZE - fix_size, fp);

		// Arquivo estavel
		status = 1;
		rewind(fp);
		fwrite(&status, 1, 1, fp);
        fclose(fp);
        
				int altura = get_height(index);
				int last_rrn = get_last_rrn(index);
				
				set_root(index, ROOT);  					// Escreve a raiz no cabecalho
				fwrite(&altura, sizeof(int), 1, index);		// Escreve a altura
				fwrite(&last_rrn, sizeof(int), 1, index);	// e o LAST_RRN
				
				flush_buffer(index);						// Tudo que esta no buffer vai para o arquivo
        fclose(index);
        if (saida != ERROR) fprintf(stdout, "Registro inserido com sucesso.\n");
    }
}
