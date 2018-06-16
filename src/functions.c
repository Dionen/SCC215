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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <functions.h>
#include <utils.h>

/** Imprime registros com base no valor de campo especificado */
void procura_campo(char *key, char *value){
	FILE *database;
	REG school;
    int (*cmp_cust)(REG*, void*);
    unsigned char nfound = 1;

    database = fopen("output.dat", "rb");

    if (database == NULL){
		printf("Falha no processamento do arquivo.\n");
        return;
	}

	// Atribui-se uma diferente funcao de comparacao com base no campo escolhido.
    if(!strcmp(key, "codINEP")) {
        *(int*)value = atoi(value);
        cmp_cust = (int (*)(REG*, void*))cmp_INEP;
    } else {
        if(!strcmp(key, "dataAtiv")){
            cmp_cust = (int (*)(REG*, void*))cmp_activation;
        }else if(!strcmp(key, "uf"))
            cmp_cust = (int (*)(REG*, void*))cmp_FU;
        else if(!strcmp(key, "nomeEscola"))
            cmp_cust = (int (*)(REG*, void*))cmp_school;
        else if(!strcmp(key, "municipio"))
            cmp_cust = (int (*)(REG*, void*))cmp_municipality;
        else if(!strcmp(key, "prestadora"))
            cmp_cust = (int (*)(REG*, void*))cmp_provider;
        else {
            printf("Registro inexistente.\n");
            fclose(database);
            return;
        }
    }
    fseek(database, HEADER_SIZE, SEEK_SET);

	// Loop de busca por registros ate o final do arquivo.
    while(fread(&school, REGISTER_SIZE, 1, database)) {
		// Imprime na tela se o registro nao for um removido e a funcao de comparacao retornar true.
        if(school.INEP != -1 && cmp_cust(&school, value)) {
            print_reg(&school);
            nfound = 0;
        }
    }
    if(nfound)
		// Caso nao encontre nenhum registro.
        printf("Registro inexistente.\n");
    fclose(database);
    return;
}

/** Faz a desfragmentacao completa do arquivo de registros */
void desfragmentacao(void) {
    FILE *temp;
    FILE *database;
    REG school = {0};
    unsigned char status = 0;
    int stack = -1;

    database = fopen("output.dat", "r+b");
    // Cria-se um arquivo temporario.
    temp = fopen("temp.bin", "wb");

    if(database && temp) {
		// Escreve o novo cabecalho.
        fwrite(&status, 1, 1, temp);
        fwrite(&stack, 4, 1, temp);
        fseek(database, 5, SEEK_SET);

		// Loop de leitura de registros ate o arquivo acabar.
        while(fread(&school, REGISTER_SIZE, 1, database)) {
			// Se o registro lido nao for um registro removido,
			// escreve no arquivo temporario.
            if(school.INEP != -1)
                fwrite(&school, REGISTER_SIZE, 1, temp);
        }
        status = 1;
        rewind(temp);
        fwrite(&status, 1, 1, temp);
        fclose(temp);
        fclose(database);

        // Deleta-se o arquivo antigo e renomeia-se o temporario.
        remove("output.dat");
        rename("temp.bin", "output.dat");

        printf("Arquivo de dados compactado com sucesso.\n");

    } else {
		printf("Falha no processamento do arquivo.\n");
		if (database != NULL) fclose(database);
		if (temp != NULL) fclose(temp);
	}
}

/** Imprime a pilha em ordem de desempilhamento */
void imprime_pilha(void) {
    FILE *fp;
    int topo_pilha;

    fp = fopen("output.dat", "rb");

    if(fp == NULL) {
        fprintf(stdout, "Falha no processamento do arquivo.\n");

        // Checa-se se ha registros removidos
    } else {
        fseek(fp, 1, SEEK_SET);
        fread(&topo_pilha, sizeof(int), 1, fp);

        if (topo_pilha == -1) {
            fprintf(stdout, "Pilha vazia.\n");
        } else {
            // Loop que le um valor de RRN na pilha e vai ate ele.
            // Quando chega ao primeiro registro removido, para.
            do {
                fprintf(stdout, "%d", topo_pilha);
                fseek(fp, HEADER_SIZE + REGISTER_SIZE*topo_pilha + sizeof(int), SEEK_SET);
                fread(&topo_pilha, sizeof(int), 1, fp);
                if (topo_pilha != -1) fprintf(stdout, " ");
            } while (topo_pilha != -1);
            fprintf(stdout, "\n");
        }
        fclose(fp);
    }
}

/** Remove registro com base no RNN */
void remove_RRN(char *input) {
    FILE *fp;
    char status = 0;
    int RRN, temp, stack, removal = -1;

    RRN = atoi(input);
    fp = fopen("output.dat", "r+b");

    if(fp == NULL) {
        fprintf(stdout, "Falha no processamento do arquivo.\n");

        // Checa-se se o registro existe.
    } else if ((fileLen(fp) - HEADER_SIZE)/REGISTER_SIZE <= RRN) {
        fprintf(stdout, "Registro inexistente.\n");
        fclose(fp);

    } else {
		// Arquivo instavel
        fwrite(&status, 1, 1, fp);

        // Le o topo da stack
        fread(&stack, sizeof(int), 1, fp);

		// Vai para o RRN desejado
        fseek(fp, REGISTER_SIZE*RRN, SEEK_CUR);

        // Checa-se se o registro ja foi removido
        fread(&temp, sizeof(int), 1, fp);
        if (temp == -1) {
            fprintf(stdout, "Registro inexistente.\n");
            fclose(fp);
            return;

            // Remove-se o registro e atualiza-se o cabecalho.
        } else {
            fseek(fp, -4, SEEK_CUR);
            fwrite(&removal, sizeof(int), 1, fp);
            fwrite(&stack, sizeof(int), 1, fp);

            rewind(fp);
            // Arquivo estavel
            status = 1;
			fwrite(&status, 1, 1, fp);
			// Topo da pilha.
            fwrite(&RRN, sizeof(int), 1, fp);
            fclose(fp);
            fprintf(stdout, "Registro removido com sucesso.\n");
        }
    }
}

/** Insere um novo registro no arquivo de dados, com base no topo da pilha */
void insere(char *argv2, char *argv3, char *argv4, char *argv5, char *argv6, char *argv7) {
    FILE *fp;
    int stack, stack_temp, size, fix_size = 28, codINEP_temp = atoi(argv2);
    char dataAtiv_temp[10], UF_temp[2];
    char *nomeEscola_temp = argv5;
    char *municipio_temp = argv6;
    char *prestadora_temp = argv7;
    char status = 0, espaco = 0;

    // Tratamento de entradas nulas
    if (argv3 != NULL) strncpy(dataAtiv_temp, argv3, 10);
    else strncpy(dataAtiv_temp, "0000000000", 10);

    if (argv4 != NULL) strncpy(UF_temp, argv4, 2);
    else strncpy(UF_temp, "00", 2);

    fp = fopen("output.dat", "r+b");

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
        fprintf(stdout, "Registro inserido com sucesso.\n");
    }
}

/** Atualiza um novo registro no arquivo de dados, com base no RRN */
void atualiza(char *argv2, char *argv3, char *argv4, char *argv5, char *argv6, char *argv7, char *argv8) {
    FILE *fp;
    int RRN, temp, size, fix_size = 28, codINEP_temp;
    char dataAtiv_temp[10], UF_temp[2];
    char *nomeEscola_temp = argv6;
    char *municipio_temp = argv7;
    char *prestadora_temp = argv8;
    char espaco = 0;
    char status = 0;

    codINEP_temp = atoi(argv3);
    RRN = atoi(argv2);

    // Tratamento de entradas nulas
    if (argv4 != NULL) strncpy(dataAtiv_temp, argv4, 10);
    else strncpy(dataAtiv_temp, "0000000000", 10);

    if (argv5!= NULL) strncpy(UF_temp, argv5, 2);
    else strncpy(UF_temp, "00", 2);

    fp = fopen("output.dat", "r+b");

    if(fp == NULL) {
        fprintf(stdout, "Falha no processamento do arquivo.\n");

        // Checa-se se o registro existe.
    } else if ((fileLen(fp) - HEADER_SIZE)/REGISTER_SIZE <= RRN) {
        fprintf(stdout, "Registro inexistente.\n");
        fclose(fp);

    } else {
		// Arquivo instavel.
        fwrite(&status, 1, 1, fp);

        // Se existe, checa se foi removido.
        fseek(fp, HEADER_SIZE + REGISTER_SIZE*RRN, SEEK_SET);
        fread(&temp, sizeof(int), 1, fp);

        if (temp == -1) {
            fprintf(stdout, "Registro removido.\n");

        } else {
            // Caso contrario, va para o registro a ser atualizado.
            fseek(fp, RRN*REGISTER_SIZE + HEADER_SIZE, SEEK_SET);

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

            fprintf(stdout, "Registro atualizado com sucesso.\n");
        }
        // Arquivo estavel.
        status = 1;
        rewind(fp);
        fwrite(&status, 1, 1, fp);
        fclose(fp);
    }

}

/** Le um arquivo .csv e o armazena em registros, em um novo arquivo .dat */
void converte(char *filename) {
    FILE *dados, *arq;
    HEADER header;
    REGISTRO_LEITURA registro;
    int counter = 0, i = 0;
    char aux, aux_vec[87];

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

/** Escreve o arquivo de registros na tela */
void imprime_arquivo(void) {
    REGISTRO TEMP;
    int fileSize, temp_size;
    char temp[60];
    char flag = 1;
    FILE *arq;

    arq = fopen("output.dat", "rb");

    if(arq == NULL) {
        fprintf(stdout, "Falha no processamento do arquivo\n");
    } else {
        fileSize = fileLen(arq);
        fseek(arq, 5, SEEK_SET);

        // Enquanto não chegar ao final do arquivo de registros:
        while(ftell(arq) < fileSize) {

            // Le o primeiro campo e checa se o registro foi removido.
            fread(&TEMP.codINEP, sizeof(int), 1, arq);
            if(TEMP.codINEP == -1) {
                // Se foi removido, avança para o proximo registro.
                fseek(arq, 87 - sizeof(int), SEEK_CUR);

            } else {
                int size = 28; // Tamanho dos campos de tamanho fixo + delimitadores.

                // Le o registro e escreve na tela.
                fread(TEMP.dataAtiv, 10, 1, arq);
                fread(TEMP.uf, 2, 1, arq);
                fread(&temp_size, sizeof(int), 1, arq);
                fread(&temp, temp_size, 1, arq);
                size += temp_size;
                // Tratamento de valores nulos.
                if (temp_size != 0) printf("%d %.10s %.2s %d %.*s ", TEMP.codINEP, TEMP.dataAtiv, TEMP.uf, temp_size, temp_size, temp);
                else printf("%d %.10s %.2s %d %.*s", TEMP.codINEP, TEMP.dataAtiv, TEMP.uf, temp_size, temp_size, temp);

                fread(&temp_size, sizeof(int), 1, arq);
                fread(&temp, temp_size, 1, arq);
                size += temp_size;
                if (temp_size != 0) printf("%d %.*s ", temp_size, temp_size, temp);
                else printf("%d %.*s", temp_size, temp_size, temp);

                fread(&temp_size, sizeof(int), 1, arq);
                fread(&temp, temp_size, 1, arq);
                size += temp_size;
                if (temp_size != 0) printf("%d %.*s\n", temp_size, temp_size, temp);
                else printf("0\n");

                // Avança para o proximo registro.
                fseek(arq, 87-size, SEEK_CUR);
                flag = 0;

            }
        }
        // Caso nao tenha encontrado nenhum registro valido.
        if (flag) printf("Registro inexistente.\n");
        fclose(arq);
    }
}

/** Imprime o conteudo do registro de dado RRN */
void imprime_RRN(char *input) {
    REGISTRO TEMP;
    int RRN, fileSize, temp_size;
    char temp[60];
    FILE *arq;

    RRN = atoi(input);

    arq = fopen("output.dat", "rb");

    if(arq == NULL) {
        fprintf(stdout, "Falha no processamento do arquivo\n");

        // Checa-se se o registro existe.
    } else if ((fileLen(arq) - HEADER_SIZE)/REGISTER_SIZE <= RRN) {
        fprintf(stdout, "Registro inexistente.\n");
        fclose(arq);

    } else {
        fseek(arq, HEADER_SIZE + REGISTER_SIZE*RRN, SEEK_SET);

        // Checa se o registro foi removido
        fread(&TEMP.codINEP, sizeof(int), 1, arq);
        if(TEMP.codINEP == -1) {
            fprintf(stdout, "Registro inexistente.\n");

        } else {
            int size = 28; // Tamanho dos campos de tamanho fixo + delimitadores

            // Le e escreve na tela
            fread(TEMP.dataAtiv, 10, 1, arq);
            fread(TEMP.uf, 2, 1, arq);
            fread(&temp_size, sizeof(int), 1, arq);
            fread(&temp, temp_size, 1, arq);
            size += temp_size;
            // Tratamento de valores nulos
            if (temp_size != 0) printf("%d %.10s %.2s %d %.*s ", TEMP.codINEP, TEMP.dataAtiv, TEMP.uf, temp_size, temp_size, temp);
            else printf("%d %.10s %.2s %d %.*s", TEMP.codINEP, TEMP.dataAtiv, TEMP.uf, temp_size, temp_size, temp);

            fread(&temp_size, sizeof(int), 1, arq);
            fread(&temp, temp_size, 1, arq);
            size += temp_size;
            if (temp_size != 0) printf("%d %.*s ", temp_size, temp_size, temp);
            else printf("%d %.*s", temp_size, temp_size, temp);

            fread(&temp_size, sizeof(int), 1, arq);
            fread(&temp, temp_size, 1, arq);
            size += temp_size;
            if (temp_size != 0) printf("%d %.*s\n", temp_size, temp_size, temp);
            else printf("0\n");

        }
        fclose(arq);
    }
}
