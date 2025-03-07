#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trabalho.h"



void print_queue(fila_h* pr);
void remove_node(fila_h* pr, char num[]);
void print_queue(fila_h* pr);


int main() {
    int x, y;
    fila_h fila;
    initialize(&fila);

     // carregar o ficheiro
    FILE* file = fopen("ficheiro.txt", "r");
    if (file == NULL) {
        printf("Falha a abrir o ficheiro.\n");
        return 0;
    }
    else{
    char line[100];
    while (fgets(line, 100, file) != NULL) {
        char fields[6][50];
        int fieldCount = 0;
        
        char* token = strtok(line, "$");
        while (token != NULL && fieldCount < 6) {
            strncpy(fields[fieldCount], token, 50 - 1);
            fields[fieldCount][50 - 1] = '\0';
            token = strtok(NULL, "$");
            fieldCount++;
        }
        if (fieldCount == 6) {
            char* nome = fields[0];
            char* dia1 = fields[1];
            char* mes1 = fields[2];
            char* ano1 = fields[3];
            char* hora1 = fields[4];
            char* minutos1 = fields[5];

            insert(&fila, nome,atoi(dia1),atoi(mes1),atoi(ano1),atoi(hora1),atoi(minutos1)); //atoi - converte string em nº
        } else {
            printf("Erro a ler a linha: %s\n", line);
        }
    }
    }
    fclose(file);





    int dia, mes, ano, hora, minutos;
    char num[50];
    while(y !=0){
        printf("Insira o número da função que deseja executar:\n");
        printf("[0] - Sair do menu.\n");
        printf("[1] - Reservar lavagem ou manutenção.\n");
        printf("[2] - Pré-reserva de lavagem ou manutenção.\n");
        printf("[3] - Cancelar uma reserva.\n");
        printf("[4] - Cancelar uma pré-reserva.\n");
        printf("[5] - Listar as reservas e pré-reservas ordenadas por data.\n");
        printf("[6] - Listar as reservas e pré-reservas de um cliente ordenadas por data.\n");
        printf("[7] - Guardar dados.\n");
        printf("[8] - Carregar dados.\n");
    
        
        
        scanf("%d", &y);
        
        if( 0 <= y && y<=8 ){
            if (y==0){
                printf("\n");
            }
            if (y == 1) {
                printf("Insira o número 1 para reserva de lavagem ou o número 2 para manutenção: ");
                scanf("%d", &x);
                
                if (x == 1 || x == 2) {
                    printf("Insira o seu nome: ");
                    scanf("%s", num);
                    printf("Insira a data na ordem dia, mês, ano, hora, minutos: ");
                    scanf("%d %d %d %d %d", &dia, &mes, &ano, &hora, &minutos);
                    if(8<=hora && hora<=18){
                        insert(&fila , num, ano, mes, dia, hora, minutos);
                    }
                    else
                        printf("Fora do horário de funcionamento\n");
                    } 
                else {
                    printf("Opção inválida. Insira o número 1 para lavagem ou o número 2 para manutenção.\n");
                }
            }
            if (y == 2) {
                printf("Insira o número 1 para pré-reserva de lavagem ou o número 2 para manutenção: ");
                scanf("%d", &x);
                
                if (x == 1 || x == 2) {
                    printf("Insira o seu nome: ");
                    scanf("%s", num);
                    
                    printf("Insira a data na ordem dia, mês, ano, hora, minutos: ");
                    scanf("%d %d %d %d %d", &dia, &mes, &ano, &hora, &minutos);
                    
                    if(8<= hora && hora <=18){
                        insert(&fila , num, ano, mes, dia, hora, minutos);
                    }
                
                    else
                        printf("Fora do horário de funcionamento\n");
                }
                
                else {
                    printf("Opção inválida. Insira o número 1 para lavagem ou o número 2 para manutenção.\n");
                }
            }
            if (y==3){
                char num_temp[20];
                printf("Digite o nome que deseja remover:");
                scanf("%s",num_temp);
                remove_node(&fila ,num_temp);
                
            }
            if (y==4){
                char num_temp[20];
                printf("Digite o nome que deseja remover:");
                scanf("%s",num_temp);
                remove_node(&fila ,num_temp);
            }
            if (y==5){
                printf("\n");
                print_queue(&fila); 
            }
            if (y==6){
                save_queue(&fila);
            }
            if (y==7){
                save_queue(&fila);  
            }
            if (y==8){
                printf("\n");
                print_queue(&fila);  
            }
            
    
        }  
        else
            printf("Opção inválida.\n");
    }




    return 0;
}