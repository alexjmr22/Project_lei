#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "projeto.h"

pDados criaDados() {
    pDados aux;
    DadosDoente DDe = {-2,{-2,-2,-2},-2,-2,-2,-2};
    aux = (pDados)malloc(sizeof(noDados));
    if (aux != NULL) {
        aux->dadosDoenteLista = DDe;
        aux->prox = NULL;
    }
    return aux;
}

pLista cria() {
    pLista aux;
    Doente e = {-1,-1,"Doentes",{-1,-1,-1},"",""};
    aux = (pLista)malloc(sizeof(noLista));
    if (aux != NULL) {
        aux->doenteLista = e;
        aux->dados = criaDados();
        aux->prox = NULL;
    }
    return aux;
}

int vazia(pLista lista) {
    if (lista->prox == NULL)
        return 1;
    else
        return 0;
}

int vaziaDados(pDados lista) {
    if (lista->prox == NULL)
        return 1;
    else
        return 0;
}

pLista destroi(pLista lista) {
    pLista temp_ptr;
    while (!vazia(lista)) {
        temp_ptr = lista;
        lista = lista->prox;
        free(temp_ptr);
    }
    free(lista);
    return NULL;
}

pDados destroiDados(pDados lista) {
    pDados temp_ptr;
    while (!vaziaDados(lista)) {
        temp_ptr = lista;
        lista = lista->prox;
        free(temp_ptr);
    }
    free(lista);
    return NULL;
}

void procura_Id(pLista lista, int chave, pLista *ant, pLista *atual) {
    *ant = lista;
    *atual = lista->prox;
    while ((*atual != NULL) && ((*atual)->doenteLista.Id != chave)) {
        *ant = *atual;
        *atual = (*atual)->prox;
    }
    if ((*atual != NULL) && ((*atual)->doenteLista.Id != chave))
        *atual = NULL;
}

void procura_Id_Dados(pDados lista, int chave, pDados *ant, pDados *atual) {
    *ant = lista;
    *atual = lista->prox;
    while ((*atual != NULL) && ((*atual)->dadosDoenteLista.Id != chave)) {
        *ant = *atual;
        *atual = (*atual)->prox;
    }
    if ((*atual != NULL) && ((*atual)->dadosDoenteLista.Id != chave))
        *atual = NULL;
}

void procura_Nome(pLista lista, char *chave, pLista *ant, pLista *atual) {
    *ant = lista;
    *atual = lista->prox;
    while ((*atual != NULL) && (strcmp((*atual)->doenteLista.nome, chave) > 0)) {
        *ant = *atual;
        *atual = (*atual)->prox;
    }
    if ((*atual != NULL) && (strcmp((*atual)->doenteLista.nome, chave) != 0)) {
        *atual = NULL;
    }
}

void procura_CC(pLista lista, char *chave, pLista *ant, pLista *atual) {
    *ant = lista;
    *atual = lista->prox;
    while ((*atual != NULL) && (strcmp((*atual)->doenteLista.cc, chave) != 0)) {
        *ant = *atual;
        *atual = (*atual)->prox;
    }
    if ((*atual != NULL) && (strcmp((*atual)->doenteLista.cc, chave) != 0)) {
        *atual = NULL;
    }

}

void procura_email(pLista lista, char *chave, pLista *ant, pLista *atual) {
    *ant = lista;
    *atual = lista->prox;
    while ((*atual != NULL) && (strcmp((*atual)->doenteLista.mail, chave) != 0)) {
        *ant = *atual;
        *atual = (*atual)->prox;
    }
    if ((*atual != NULL) && (strcmp((*atual)->doenteLista.mail, chave) != 0)) {
        *atual = NULL;
    }
}

void procura_telefone(pLista lista, int chave, pLista *ant, pLista *atual) {
    *ant = lista;
    *atual = lista->prox;
    while ((*atual != NULL) && ((*atual)->doenteLista.telefone != chave)) {
        *ant = *atual;
        *atual = (*atual)->prox;
    }
    if ((*atual != NULL) && ((*atual)->doenteLista.telefone != chave))
        *atual = NULL;
}

void procura_Tens(pLista lista,pLista reg_tens, int chave) {
    pLista aux = lista->prox;
    pDados dados_aux;
    while(aux){
        dados_aux=aux->dados;
        while ((dados_aux != NULL) && (dados_aux->dadosDoenteLista.tensmax < chave)){
            dados_aux = dados_aux->prox;
        }
        if ((dados_aux != NULL) && (dados_aux->dadosDoenteLista.tensmax < chave)){
            dados_aux = NULL;
        }
        if(dados_aux!=NULL){
            insereTens(reg_tens, aux->doenteLista);
        }
        aux = aux->prox;
    }
}

pLista pesquisa_Id(pLista lista,int chave){
    pLista ant, atual;
    procura_Id(lista,chave,&ant,&atual);
    return atual;
}

int pesquisa_CC(pLista lista,char *chave){
    pLista ant, atual;
    procura_CC(lista,chave,&ant,&atual);
    if((atual != NULL) && (strcmp(atual->doenteLista.cc,chave)==0))
        return 1;
    return 0;
}

int pesquisa_telefone(pLista lista,int chave){
    pLista ant, atual;
    procura_telefone(lista,chave,&ant,&atual);
    if((atual != NULL) && (atual->doenteLista.telefone == chave))
        return 1;
    return 0;
}

int pesquisa_email(pLista lista,char *chave){
    pLista ant, atual;
    procura_email(lista,chave,&ant,&atual);
    if((atual != NULL) && (strcmp(atual->doenteLista.mail,chave)==0))
        return 1;
    return 0;
}

void elimina(pLista *lista, int chave) {
    pLista ant = NULL, atual = *lista;
    procura_Id(*lista, chave, &ant, &atual);
    if (atual != NULL) {
        if (ant != NULL) {
            ant->prox = atual->prox;
            free(atual);
        } else {
            // If ant is NULL, atual is the head node
            *lista = atual->prox;
            free(atual);
        }
    } else {
        printf("Doente com ID %d não encontrado.\n", chave);
    }
}

pLista insere(pLista doente, pLista doentes) {
    pLista atual,anterior;
    // Verifica se a lista está vazia ou se o novo doente deve ser inserido no início
    if (doentes->prox == NULL || strcmp(doente->doenteLista.nome, doentes->prox->doenteLista.nome) < 0) {
        doente->prox = doentes->prox;
        doentes->prox = doente;
        return doentes;
    }
    atual = doentes->prox;
    anterior = doentes;
    // Encontra o local adequado na lista para inserir o novo doente por ordem alfabética
    while (atual != NULL && strcmp(doente->doenteLista.nome, atual->doenteLista.nome) >= 0) {
        anterior = atual;
        atual = atual->prox;
    }
    doente->prox = atual;
    anterior->prox = doente;
    return doentes;
}

void insereTens(pLista lista, Doente e){
    pLista novo;
    pLista ant, atual;
    novo = (pLista)malloc(sizeof(noLista));
    if (novo != NULL) {
        novo->doenteLista = e;
        procura_Id(lista, e.Id, &ant, &atual);
        if (ant != NULL) {
            novo->prox = ant->prox;
            ant->prox = novo;
        } else {
            novo->prox = lista;
            lista = novo;
        }
    }
}

void insereDados(pDados lista, DadosDoente e) {
    pDados novo;
    pDados ant, atual;
    novo = (pDados)malloc(sizeof(noDados));
    if (novo != NULL) {
        novo->dadosDoenteLista = e;
        procura_Id_Dados(lista, e.Id, &ant, &atual);
        if (ant != NULL) {
            novo->prox = ant->prox;
            ant->prox = novo;
        } else {
            novo->prox = lista;
            lista = novo;
        }
    }
}

int verifica_id(pLista lista) {
    if (lista == NULL || lista->prox == NULL) {
        return -1;
    }    
    int maior_id =0;
    pLista aux = lista->prox;
    while (aux) {
        if(aux->doenteLista.Id){
            if(maior_id<aux->doenteLista.Id)
                maior_id = aux->doenteLista.Id;
        }
        aux = aux->prox;
    }
    return maior_id;
}

void imprime(pLista lista) {
    pLista aux = lista->prox; /* Salta o header */
    if(aux == NULL)
        printf("Não existem doentes para imprimir\n\n");
    else{
        while (aux) {
            printf("ID: %d\n", aux->doenteLista.Id);
            printf("Nome: %s\n", aux->doenteLista.nome);
            printf("Data de Nascimento: %d/%d/%d\n", aux->doenteLista.nascimento.dia, aux->doenteLista.nascimento.mes, aux->doenteLista.nascimento.ano);
            printf("Contacto: %d\n", aux->doenteLista.telefone);
            printf("CC: %s\n", aux->doenteLista.cc);
            printf("Email: %s\n", aux->doenteLista.mail);
            printf("\n");
            aux = aux->prox;
        }
    }
}

void imprimeEspecifico(pLista lista) {
    pLista aux = lista;
    printf("ID: %d\n", aux->doenteLista.Id);
    printf("Nome: %s\n", aux->doenteLista.nome);
    printf("Data de Nascimento: %d/%d/%d\n", aux->doenteLista.nascimento.dia, aux->doenteLista.nascimento.mes, aux->doenteLista.nascimento.ano);
    printf("Contacto: %d\n", aux->doenteLista.telefone);
    printf("CC: %s\n", aux->doenteLista.cc);
    printf("Email: %s\n", aux->doenteLista.mail);
    printf("\n");
}

void imprimeDados(pDados lista) {
    pDados aux = lista;
    if(vaziaDados(aux))
        printf("Este doente não tem registos médicos!\n\n");
    else{
        aux = aux->prox;
        while (aux) {
            printf("Data: %d/%d/%d\n", aux->dadosDoenteLista.data_registo.dia, aux->dadosDoenteLista.data_registo.mes, aux->dadosDoenteLista.data_registo.ano);
            printf("Tensões: %f %f\n", aux->dadosDoenteLista.tensmax,aux->dadosDoenteLista.tensmin);
            printf("Peso: %f\n", aux->dadosDoenteLista.peso);
            printf("Altura: %f\n", aux->dadosDoenteLista.altura);
            printf("\n");
            aux = aux->prox;
        }
    }
}

void escreve_ficheiro(pLista lista, char *caminho){
    FILE *arquivo;
    arquivo = fopen(caminho, "w");
    int count=1;

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    pLista aux = lista->prox; /* Salta o header */
    while (aux) {
        if (count==1)
            fprintf(arquivo,"%d\n", aux->doenteLista.Id);
        else{
            fprintf(arquivo,"\n%d\n", aux->doenteLista.Id);
        }
        fprintf(arquivo,"%s\n", aux->doenteLista.nome);
        fprintf(arquivo,"%d/%d/%d\n", aux->doenteLista.nascimento.dia, aux->doenteLista.nascimento.mes, aux->doenteLista.nascimento.ano);
        fprintf(arquivo,"%s\n", aux->doenteLista.cc);
        fprintf(arquivo,"%d\n", aux->doenteLista.telefone);
        fprintf(arquivo,"%s", aux->doenteLista.mail);
        count=2;
        aux = aux->prox;
    }
    fclose(arquivo);
}

int escreve_ficheiro_Dados(pDados lista, char* caminho, int count){
    FILE *arquivo;
    arquivo = fopen(caminho, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    pDados aux = lista;
    if(!vaziaDados(aux)){
        aux = aux->prox;
        while (aux) {
            if (count==1)
                fprintf(arquivo,"%d\n",aux->dadosDoenteLista.Id);
            else{
                fprintf(arquivo,"\n%d\n",aux->dadosDoenteLista.Id);
            }
            fprintf(arquivo,"%d/%d/%d\n", aux->dadosDoenteLista.data_registo.dia, aux->dadosDoenteLista.data_registo.mes, aux->dadosDoenteLista.data_registo.ano);
            fprintf(arquivo,"%f\n", aux->dadosDoenteLista.tensmax);
            fprintf(arquivo,"%f\n", aux->dadosDoenteLista.tensmin);
            fprintf(arquivo,"%f\n", aux->dadosDoenteLista.peso);
            fprintf(arquivo,"%f", aux->dadosDoenteLista.altura);
            count=2;
            aux = aux->prox;
        }
    }
    fclose(arquivo);
    return count;
}

void retira_doentes_txt(pLista doente, FILE *ficheiro){
    pLista aux;
    char linha[Tam];
    int i = 1, l = 1, lines=1;
    Doente e;
    char ch;
    while ((ch = fgetc(ficheiro)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }
    if (lines%6 !=0){
        printf("ficheiro inutilizável [doentes.txt]\n");
        return;
    }
    fseek(ficheiro, 0, SEEK_SET);
    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        if (i + 6 == l) {
            pLista novo = (pLista)malloc(sizeof(noLista)); 
            novo->doenteLista.Id = e.Id;
            strcpy(novo->doenteLista.nome, e.nome);
            novo->doenteLista.nascimento = e.nascimento;
            strcpy(novo->doenteLista.cc, e.cc);
            novo->doenteLista.telefone = e.telefone;
            strcpy(novo->doenteLista.mail, e.mail);
            novo->prox = NULL; 
            doente = insere(novo, doente);
            aux = pesquisa_Id(doente, e.Id);
            aux->dados = criaDados();
            i += 6;
        }
        if (l - i == 0){
            for(size_t s=2;s<strlen(linha);s++){
                printf("s =%ld\n",s);
                if (linha[s]<'0' || linha[s]>'9') {
                    printf("numero: %d\n",linha[s]);
                    return; 
                }
            }
            if((atoi(linha) == verifica_id(doente)) ){
                printf("ficheiro corrompido [id]");
                return;
            }
            e.Id = atoi(linha);
        }
        else if ((l - i == 1) ){
            if(analisa_nome_comp(&linha[1]))
                strcpy(e.nome, linha);
        }
        else if (l - i == 2) {
            int dia,mes,ano;
            char *token;
            token = strtok(linha, "/");
            dia = atoi(token);
            token = strtok(NULL, "/");
            mes = atoi(token);
            token = strtok(NULL, "/");
            ano = atoi(token);
            if(ver_data(dia,mes,ano)==0){
                e.nascimento.dia =dia;
                e.nascimento.mes =mes;
                e.nascimento.ano =ano;
            }else{
                printf("ficheiro corrompido [data de nascimento]");
                return;
            }
        }else if (l - i == 3){
            for(int s=0;s<8;s++){
                if(linha[s]>'9' || linha[s]<'0'){
                    printf("ficheiro corropido [CC]\n");
                    return;
                }
            }
            if (strlen(linha) != 14 || pesquisa_CC(doente,linha)==1) {
                printf("ficheiro corropido cc\n");
                return;
            }
            strcpy(e.cc, linha);            
        }
        else if (l - i == 4){
            int telefone = atoi(linha);
            if(snprintf(NULL, 0, "%d", telefone) != 9 || pesquisa_telefone(doente,telefone) || (telefone)/900000000 < 1){
                printf("ficheiro corropido [telemovel]\n");
                return;
            }
            e.telefone = telefone;
        }
        else if (l - i == 5){
            if (((strstr(linha, "@") == NULL) && (strstr(linha, ".com") == NULL || strstr(linha, ".pt") == NULL ) ) ||  pesquisa_email(doente,&linha[1])){
                printf("ficheiro corropido [mail]\n");
                return;
            }
            strcpy(e.mail, linha);          
        }
        l++;
    }
    if (l - i > 0) {
        pLista novo = (pLista)malloc(sizeof(noLista)); // Crie um novo nó para o último doente
        // Inicialize os campos do doente corretamente
        novo->doenteLista.Id = e.Id;
        strcpy(novo->doenteLista.nome, e.nome);
        novo->doenteLista.nascimento = e.nascimento;
        strcpy(novo->doenteLista.cc, e.cc);
        novo->doenteLista.telefone = e.telefone;
        strcpy(novo->doenteLista.mail, e.mail);
        novo->prox = NULL; // Certifique-se de que o próximo aponte para NULL, pois será o último nó
        doente = insere(novo, doente);
        aux = pesquisa_Id(doente, e.Id);
        aux->dados = criaDados();
    }
    fclose(ficheiro);
}

void retira_registos_txt(pLista doente, FILE *ficheiro){
    pLista aux;
    char linha[Tam];
    DadosDoente DDe;
    int j=1,k=1,lines=1;
    char ch;
    while ((ch = fgetc(ficheiro)) != EOF) {
        if (ch == '\n') {
            lines++;  
        }
    }
    if (lines%6 !=0){
        printf("ficheiro inutilizável\n");
        return;
    }
    fseek(ficheiro, 0, SEEK_SET);
    while (fgets(linha,sizeof(linha), ficheiro) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        if (j + 6 == k) {
            aux = pesquisa_Id(doente,DDe.Id);
            if(DDe.Id == aux->doenteLista.Id){
                insereDados(aux->dados, DDe);
            }
            j += 6;
        }
        if (k - j == 0)
            DDe.Id = atoi(linha);
        else if (k - j == 1) {
            int dia,mes,ano;
            char *token;
            token = strtok(linha, "/");
            dia = atoi(token);
            token = strtok(NULL, "/");
            mes = atoi(token);
            token = strtok(NULL, "/");
            ano = atoi(token);
            if(ver_data(dia,mes,ano)==0){
                DDe.data_registo.dia =dia;
                DDe.data_registo.mes =mes;
                DDe.data_registo.ano =ano;
            }else{
                printf("ficheiro corrompido [data de registo]");
                return;
            }
        } else if (k - j == 2){
            if(atoi(linha) <0){
                printf("ficheiro corrompido [tensão maxima]");
                return;
            }
            DDe.tensmax = atoi(linha);
        }
        else if (k - j == 3){
            if(atoi(linha) <0 || atoi(linha) > DDe.tensmax){
                printf("ficheiro corrompido [tensão minimo]");
                return;
            }
            DDe.tensmin = atoi(linha);
        }
        else if (k - j == 4){
            if(atoi(linha) <0 || atoi(linha) > 500){
                printf("ficheiro corrompido [peso]");
                return;
            }
            DDe.peso = atoi(linha);
        }
        else if (k - j == 5){
            if(atoi(linha) <0 || atoi(linha) >272){
                printf("ficheiro corrompido [altura]");
                return;
            }
            DDe.altura = atoi(linha);
        }
        k++;
    }
    if(k>j){
        aux = pesquisa_Id(doente,DDe.Id);
        if(DDe.Id == aux->doenteLista.Id){
            insereDados(aux->dados, DDe);
        }
    }
    fclose(ficheiro);
}

int analisa_nome(char *name) {
    for (int i = 1; name[i] != '\0'; i++) {
        if (name[i]<'a' || name[i]>'z') {
            return 1;
        }
    }
    return 0;
}

int analisa_nome_comp(char *full_name) {
    char name_cpy[100];
    strncpy(name_cpy, full_name, sizeof(name_cpy) - 1);
    name_cpy[sizeof(name_cpy) - 1] = '\0';
    char *name = strtok(name_cpy, " ");
    while (name != NULL) {
        if (name[0] < 'A' || name[0] > 'Z') {
            return 1;
        }
        if (analisa_nome(name) != 0) {
            return 1;
        }
        name = strtok(NULL, " ");
    }
    return 0;
}

int ano_bi(int ano) {
    if (ano % 400 == 0) {
        return 0;
    } else if (ano % 100 == 0) {
        return 1;
    } else if (ano % 4 == 0) {
        return 0;
    }
    return 1;
}

int ver_data(int dia, int mes, int ano) {
    if (ano < 1917 || ano > 2024) {
        return 1;
    }
    int dias_do_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ano_bi(ano)) {
        dias_do_mes[1] = 29;
    }
    if (dia > 0 && dia <= dias_do_mes[mes - 1]) {
        return 0;
    }
    return 1;
}

void inserir_um_doente(pLista doente){
    int id_max= verifica_id(doente);
    printf("o id maximo:%d\n",id_max);
    pLista aux=doente;
    Doente novo_doente;
    novo_doente.Id = id_max+1;
    while (getchar() != '\n'); // Limpar o buffer de entrada
    printf("Insira o nome: ");
    while(1){
        fgets(novo_doente.nome, Tam, stdin);
        novo_doente.nome[strcspn(novo_doente.nome, "\n")] = '\0';
        if(analisa_nome_comp(novo_doente.nome)==1){
            printf("Nome invalido, tente outra vez\n");
            printf("Insira o nome: ");
        } 
        else{
            printf("Nome inserido com sucesso\n");
            break;
        }
    }
    printf("Insira a sua data de nascimento [dd mm yy]:");
    while(1){
        int dia,mes,ano;
        int datas = scanf("%d %d %d", &dia, &mes, &ano);
        printf("%d\n",datas);
        if(ver_data(dia,mes,ano)==1){
            printf("Data de de nascimento incorreta\n");
            printf("Insira a sua data de nascimento [dd mm yy]:");
        }else{
            novo_doente.nascimento.dia =dia;
            novo_doente.nascimento.mes =mes;
            novo_doente.nascimento.ano =ano;            
            printf("Data de nascimento inserida com sucesso\n");
            break;
        }
    }
    while (getchar() != '\n'); // Limpar o buffer de entrada0
    printf("Insira o seu CC [00000000-0-XX0] :");
    while(1){
        fgets(novo_doente.cc, Tam, stdin);
        novo_doente.cc[strcspn(novo_doente.cc, "\n")] = '\0';
        for(int i=0;i<8;i++){
            if(novo_doente.cc[i]>'9' || novo_doente.cc[i]<'0'){
                printf("Cartão de cidadão mal inserido\n");   
                printf("Insira o seu CC [00000000-0-XX0] :");
                break;
            }
        }
        if (strlen(novo_doente.cc) != 14 || pesquisa_CC(aux,novo_doente.cc)==1) {
            printf("Cartão de cidadão mal inserido ou já utilizado\n");   
            printf("Insira o seu CC [00000000-0-XX0] :");
        }else{
            printf("Cartão de cidadão inserido com sucesso\n");
            break;
        }
    }
    printf("Insira o seu número de telemovel [9********]:\n");
    while(1){
        scanf("%d", &novo_doente.telefone);
        if(snprintf(NULL, 0, "%d", novo_doente.telefone) != 9 || pesquisa_telefone(doente,novo_doente.telefone) || (novo_doente.telefone)/900000000 < 1){//fazer funçao procura numero
            printf("numero de telemovel errado ou já utilizado\n");
            printf("Insira o seu número de telemovel [9********]:\n");
        }else{
            printf("número de telemovel inserido com sucesso\n");
            break;
        }
    }
    while (getchar() != '\n'); // Limpar o buffer de entrada
    printf("Insira o seu email [****@***.com ou ****@***.pt]:\n");
    while(1){
        fgets(novo_doente.mail, Tam, stdin);
        novo_doente.mail[strcspn(novo_doente.mail, "\n")] = '\0'; // Remover o caractere de nova linha
        if (((strstr(novo_doente.mail, "@") == NULL) && (strstr(novo_doente.mail, ".com") == NULL || strstr(novo_doente.mail, ".pt") == NULL ) ) ||  pesquisa_email(doente,novo_doente.mail)){//fazer funçao procura numero
            printf("email mal introduzido, inezistente ou já utilizado");
            printf("Insira o seu email [****@***.com ou ****@***.pt]:\n");

        }else{
            printf("email introduzido com sucesso\n");
            break;
        }
    }
    pLista novo_no = (pLista)malloc(sizeof(noLista));
    novo_no->doenteLista = novo_doente;
    novo_no->prox = NULL;
    novo_no->dados=criaDados();
    doente = insere(novo_no, doente);
}

void inserir_registo(pDados lista,int id){
    DadosDoente DDe;
    DDe.Id = id;
    int dia,mes,ano;
    double tensmax,tensmin,peso,altura;
    while(1){
        printf("Insira a data de registo: ");
        scanf("%d %d %d", &dia, &mes, &ano);
        if(ver_data(dia,mes,ano)==1){
            printf("Data de de nascimento incorreta\n");
            printf("Insira a sua data de nascimento [dd mm yy]:");
        }else{
            DDe.data_registo.dia =dia;
            DDe.data_registo.mes =mes;
            DDe.data_registo.ano =ano;            
            printf("Data de nascimento inserida com sucesso\n");
            break;
        }
    }
    while(1){
        printf("Insira a tensão máxima do doente: ");
        scanf("%lf",&tensmax);
        if(tensmax<0 || tensmax>200)
            printf("Tensão máxima inválida. Insira uma tensão acima de 0.\n");
        else{
            printf("Tensão máxima inserida com sucesso!\n");
            DDe.tensmax=tensmax;
            break;
        }
    }
    while(1){
        printf("Insira a tensão minima do doente: ");
        scanf("%lf",&tensmin);
        if(tensmin<0 || tensmin>DDe.tensmax)
            printf("Tensão minima inválida. Insira uma tensão acima de 0 e menor que a tensão máxima.\n");
        else{
            printf("Tensão minima inserida com sucesso!\n");
            DDe.tensmin=tensmin;
            break;
        }
    }
    while(1){
        printf("Insira o peso do doente: ");
        scanf("%lf",&peso);
        if(peso<0 || peso>500)
            printf("Peso inválido. Insira um peso acima de 0 e menor que 500.\n");
        else{
            printf("Peso inserido com sucesso!\n");
            DDe.peso=peso;
            break;
        }
    }
    while(1){
        printf("Insira a altura do doente: ");
        scanf("%lf",&altura);
        if(altura<0 || altura>272)
            printf("Altura inválida. Insira uma altura acima de 0 e menor que 272 cm.\n");
        else{
            printf("Altura inserida com sucesso!\n");
            DDe.altura=altura;
            break;
        }
    }

    insereDados(lista,DDe);
    printf("\nRegisto do doente inserios com sucesso!\n");
}