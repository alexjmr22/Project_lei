#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "projeto.h"

int main(){
    FILE *ficheiro_d = fopen(DATA_FILE_PATH_doentes, "r");
    if (ficheiro_d == NULL) {
        printf("Erro ao abrir o ficheiro <doentes.txt> .\n");
        return 0;
    }
    FILE *ficheiro_reg = fopen(DATA_FILE_PATH_registos, "r");
    if (ficheiro_reg == NULL) {
        printf("Erro ao abrir o ficheiro <registos.txt> .\n");
        return 0;
    }
    pLista doente = cria();
    pLista aux=doente;
    int Id,n;

    retira_doentes_txt(doente,ficheiro_d);
    retira_registos_txt(aux,ficheiro_reg);

    printf("Seja bem-vindo à nossa aplicação!\n\n");
    while(1){
        printf("Menu:\n");
        printf("[1]-> Inserir dados de um novo doente.\n");  //Inserir novo doente
        printf("[2]-> Eliminar dados de um doente.\n"); //Eliminar dados de um doente
        printf("[3]-> Registar dados médicos de um doente.\n"); 
        printf("[4]-> Catalogar doentes, por ordem alfabética.\n");  //
        printf("[5]-> Catalogar doentes, de acordo com a tensão máxima.\n");
        printf("[6]-> Catalogar dados de um doente.\n");
        printf("[0]-> SAIR!\n");
        printf("Insira o número de acordo com a ação que deseja realizar: ");
        scanf("%d",&n);
        switch (n){
            case 0:
                //Sair
                aux = doente->prox;
                //Limpar o ficheiro
                FILE * ficheiro;
                ficheiro = fopen(DATA_FILE_PATH_registos, "w");
                if (ficheiro == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    return 0;
                }
                fclose(ficheiro);

                int count=1;
                while(aux){
                    count = escreve_ficheiro_Dados(aux->dados,DATA_FILE_PATH_registos,count);
                    aux->dados = destroiDados(aux->dados);
                    aux = aux->prox;
                }
                escreve_ficheiro(doente,DATA_FILE_PATH_doentes);
                destroi(doente);
                printf("\nSaída concluída!\nObrigado!\n");
                return 0;
            case 1:
                //Inserir
                inserir_um_doente(doente);
                imprimeEspecifico(doente);
                break;
            case 2:
                //Eliminar
                printf("Insira o id do doente que deseja eliminar:\n");
                scanf("%d",&Id);
                elimina(&doente, Id);
                printf("o doente com id %d foi eliminado",Id);
                break;
            case 3: 
                //Registar
                int num;
                printf("Insira o Id do doente que deseja fazer registos:\n");
                scanf("%d",&num);
                aux =pesquisa_Id(doente,num);
                inserir_registo(aux->dados,num);
                break;
            case 4:
                //Lista alfabetica
                imprime(doente);
                break;
            case 5:
                //Lista tensao
                int tens;
                pLista registo_tens = cria();
                aux = doente;
                printf("Insira a tensão que deseja comparar: ");
                scanf("%d",&tens);
                procura_Tens(aux,registo_tens,tens);
                printf("\n");
                if(!vazia(registo_tens))
                    imprime(registo_tens);
                else{
                    printf("Não existem pacientes com tensões acima desse valor\n\n"); 
                }
                break;
            case 6:
                //Lista doente
                printf("Insira o Id do doente que deseja ver os dados: ");
                scanf("%d",&Id);
                printf("\n");
                aux = pesquisa_Id(doente,Id);
                if(aux!=NULL){
                    imprimeEspecifico(aux);
                    imprimeDados(aux->dados);
                }
                else
                    printf("Não existe um doente com este Id!\n\n");
                break;
            default:
                printf("Número inválido. Insira outro número\n");
                break;
        }
    }
}