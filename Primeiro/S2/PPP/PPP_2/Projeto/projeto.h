#ifndef PROJETO_H
#define PROJETO_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define Tam 100

typedef struct data {
    int dia, mes, ano;
} Data;

typedef struct DadosDoente {
    int Id;
    Data data_registo;
    double tensmax;
    double tensmin;
    double peso;
    double altura;
} DadosDoente;

typedef struct noDados {
    DadosDoente dadosDoenteLista;
    struct noDados *prox;
} noDados;

typedef noDados *pDados;

typedef struct doente {
    int Id, telefone;
    char nome[Tam];
    Data nascimento;
    char cc[Tam];
    char mail[Tam];
} Doente;

typedef struct noLista {
    Doente doenteLista;
    pDados dados;
    struct noLista *prox;
} noLista;

typedef noLista *pLista;

pDados criaDados();
pLista cria();
int vazia(pLista lista);
int vaziaDados(pDados lista);
pLista destroi(pLista lista);
pDados destroiDados(pDados lista);
void procura_Id(pLista lista, int chave, pLista *ant, pLista *atual);
void procura_Id_Dados(pDados lista, int chave, pDados *ant, pDados *atual);
void procura_Nome(pLista lista, char *chave, pLista *ant, pLista *atual);
void procura_CC(pLista lista, char *chave, pLista *ant, pLista *atual);
void procura_email(pLista lista, char *chave, pLista *ant, pLista *atual);
void procura_telefone(pLista lista, int chave, pLista *ant, pLista *atual);
void procura_Tens(pLista lista,pLista reg_tens, int chave);
pLista pesquisa_Id(pLista lista,int chave);
int pesquisa_CC(pLista lista,char *chave);
int pesquisa_telefone(pLista lista,int chave);
int pesquisa_email(pLista lista,char *chave);
pDados pesquisa_Id_Dados(pDados lista,int chave);
void elimina(pLista *lista, int chave);
pLista insere(pLista doente, pLista doentes);
void insereTens(pLista lista, Doente e);
void insereDados(pDados lista, DadosDoente e);
int verifica_id(pLista lista);
void imprime(pLista lista);
void imprimeEspecifico(pLista lista);
void imprimeDados(pDados lista);
void escreve_ficheiro(pLista lista, char *caminho);
int escreve_ficheiro_Dados(pDados lista, char* caminho, int count);
void retira_doentes_txt(pLista doente, FILE *ficheiro);
void retira_registos_txt(pLista doente, FILE *ficheiro);
void inserir_um_doente(pLista doente);
void inserir_registo(pDados lista,int id);
int analisa_nome(char *name);
int analisa_nome_comp(char *full_name);
int ano_bi(int ano);
int ver_data(int dia, int mes, int ano);
#endif