#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trabalho.h"

void initialize(fila_h* pr) {
    pr->start = NULL;
    pr->end = NULL;
}

int is_empty(fila_h* pr) {
    return pr->start == NULL;
}

void insert(fila_h* pr, char num[], int ano, int mes, int dia, int hora, int minutos) {
    fila2* node = (fila2*)malloc(sizeof(fila2));
    strcpy(node->num, num);
    node->ano = ano;
    node->mes = mes;
    node->dia = dia;
    node->hora = hora;
    node->minutos = minutos;
    node->next = NULL;

    if (is_empty(pr)) {
        pr->start = node;
    } else {
        pr->end->next = node;
    }

    pr->end = node;
}

void remove_node(fila_h* pr, char num[]) {
    if (is_empty(pr)) {
        printf("Fila vazia.\n");
        return ;
    }

    fila2* current = pr->start;
    fila2* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->num, num) == 0) {
            if (prev == NULL) {
                pr->start = current->next;
            } else {
                prev->next = current->next;
            }

            if (current == pr->end) {

                pr->end = prev;
            }

            free(current);
            printf("Reserva removida.\n");
            return ;
        }

        prev = current;
        current = current->next;
    }

    printf("Reserva não encontrada.\n");
}


void delete(fila_h* pr) {
    fila2* node = pr->start;
    fila2* temp;

    while (node != NULL) {
        temp = node;
        node = node->next;
        free(temp);
    }

    pr->start = NULL;
    pr->end = NULL;
}

void save_queue(fila_h* pr){
    FILE *f;
    f = fopen("ficheiro.txt", "w");
    fila2* current = pr->start;

    while (current != NULL) {
        fprintf(f,"%s$%d$%d$%d$%d$%d\n", current->num, current->dia, current->mes, current->ano, current->hora, current->minutos);
        current = current->next;
    }
    fclose(f);

    return;
}

void print_queue(fila_h* pr) {
    fila2* current = pr->start;

    printf("Conteúdo da fila:\n");

    while (current != NULL) {
        printf("Nome: %s\n", current->num);
        printf("Data: %d/%d/%d %d:%d\n", current->dia, current->mes, current->ano, current->hora, current->minutos);
        printf("----------------------\n\n");

        current = current->next;
    }
}