#ifndef LISTAS_LIGADAS_H
#define LISTAS_LIGADAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define TAM 100
#define SHM_KEY 1234
#define SHM_SIZE sizeof(NoLista) * 10 // Ajuste conforme necessário
#define BUF_SIZE 1024

typedef struct aluno {
    char nome[TAM];
} Aluno;

typedef struct turma {
    char disciplina[TAM];
    int tam_atual;
    int limite;
    Aluno alunos[TAM];
} Turma;

typedef struct noLista {
    Turma turma;
    struct noLista *prox;
} NoLista;

typedef NoLista* pLista;

// Função para obter um segmento de memória compartilhada
pLista cria_memoria_compartilhada() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    pLista lista = (pLista) shmat(shmid, NULL, 0);
    if (lista == (pLista) -1) {
        perror("shmat");
        exit(1);
    }

    return lista;
}

// Função para liberar o segmento de memória compartilhada
void libera_memoria_compartilhada(pLista lista) {
    if (shmdt(lista) == -1) {
        perror("shmdt");
        exit(1);
    }

    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
}

pLista cria() {
    pLista lista = cria_memoria_compartilhada();
    if (lista != NULL) {
        memset(lista, 0, SHM_SIZE);
    }
    return lista;
}

int vazia(pLista lista) {
    return lista->prox == NULL;
}

pLista destroi(pLista lista) {
    pLista temp_ptr;
    while (!vazia(lista)) {
        temp_ptr = lista->prox;
        lista->prox = temp_ptr->prox;
    }
    libera_memoria_compartilhada(lista);
    return NULL;
}

void procura_disciplina(pLista lista, char *chave, pLista *ant, pLista *atual) {
    *ant = lista;
    *atual = lista->prox;
    while ((*atual != NULL) && (strcmp((*atual)->turma.disciplina, chave) != 0)) {
        *ant = *atual;
        *atual = (*atual)->prox;
    }
    if ((*atual != NULL) && (strcmp((*atual)->turma.disciplina, chave) != 0)) {
        *atual = NULL;
    }
}

int pesquisa_disciplina(pLista lista, char *chave) {
    pLista ant, atual;
    procura_disciplina(lista, chave, &ant, &atual);
    if ((atual != NULL) && (strcmp(atual->turma.disciplina, chave) == 0))
        return 1;
    return 0;
}

void procura_aluno(pLista lista, char *nome_aluno, pLista *ant, pLista *atual) {
    if (lista == NULL) {
        *ant = NULL;
        *atual = NULL;
        return;
    }
    *ant = lista;
    *atual = lista->prox;

    while (*atual != NULL) {
        Aluno *alunos = (*atual)->turma.alunos;
        int i = 0;
        while (alunos[i].nome[0] != '\0') { // Assumindo que o array termina com um aluno de nome vazio
            if (strcmp(alunos[i].nome, nome_aluno) == 0) {
                return;
            }
            i++;
        }
        *ant = *atual;
        *atual = (*atual)->prox;
    }
}

int pesquisa_aluno(pLista lista, char *nome_aluno, int client_fd) {
    pLista ant, atual;
    procura_aluno(lista, nome_aluno, &ant, &atual);

    // Verifique se o aluno foi encontrado
    if (atual != NULL) {
        Aluno *alunos = atual->turma.alunos;
        int i = 0;
        while (alunos[i].nome[0] != '\0') { // Assumindo que o array termina com um aluno de nome vazio
            if (strcmp(alunos[i].nome, nome_aluno) == 0) {
                return 1;
            }
            i++;
        }
    }else
        write(client_fd, "Nâo está inscrito em nenhuma turma", 1 + strlen("Nâo está inscrito em nenhuma turma"));
    return 0;
}

void imprime(pLista lista) {
    pLista aux = lista->prox;
    if (aux == NULL) {
        printf("Lista vazia\n");
    } else {
        while (aux) {
            printf("Disciplina: %s com %d alunos\n", aux->turma.disciplina, aux->turma.tam_atual);
            aux = aux->prox;
        }
    }
}



void elimina(pLista lista, char chave[]) {
    pLista ant, atual;
    procura_disciplina(lista, chave, &ant, &atual);
    if (atual != NULL) {
        ant->prox = atual->prox;
        // A memória compartilhada não deve ser liberada aqui, apenas ajustamos a lista
    }
}

void insere(pLista lista, Turma t1) {
    pLista ant, atual;
    procura_disciplina(lista, t1.disciplina, &ant, &atual);

    pLista novo_no = (pLista) malloc(sizeof(NoLista));
    if (novo_no == NULL) {
        perror("malloc");
        exit(1);
    }

    novo_no->turma = t1;
    novo_no->prox = ant->prox;
    ant->prox = novo_no;
}
void imprimir_turmas_aluno(pLista lista, char *nome_aluno, int client_fd) {
    pLista atual = lista->prox;
    char buffer[BUF_SIZE];
    buffer[0] = '\0';
    int aluno_encontrado = 0;
    if(atual ==NULL){
        strcpy(buffer,"Não existem turmas");
        write(client_fd, buffer, strlen(buffer));
        return;
    }
    while (atual != NULL) {
        for (int i = 0; i < atual->turma.tam_atual; i++) {
            if (strcmp(atual->turma.alunos[i].nome, nome_aluno) == 0) {
                if (strlen(buffer) + strlen(atual->turma.disciplina) + 2 >= BUF_SIZE) {
                    write(client_fd, buffer, strlen(buffer));
                    buffer[0] = '\0';
                }
                strcat(buffer, atual->turma.disciplina);
                strcat(buffer, "\n");
                aluno_encontrado = 1;
                break;
            }
        }
        atual = atual->prox;
    }

    if (!aluno_encontrado) {
        strcpy(buffer, "não está inscrito em nenhuma turma\n");
    } else if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
    }

    write(client_fd, buffer, strlen(buffer));
}

void subscribe_class(int client_fd, pLista lista, char *username) {
    pLista ant, atual;
    char buffer[BUF_SIZE];
    int nread = read(client_fd, buffer, BUF_SIZE - 1);
    buffer[nread] = '\0';
    printf("TURMA: %s\n", buffer);
    procura_disciplina(lista, buffer, &ant, &atual);
    if(atual ==NULL){
        strcpy(buffer,"Essa turma não existe");
        write(client_fd, buffer, strlen(buffer));
        return;
    }
    int i = atual->turma.tam_atual;
    int MAX_STUDENTS = atual->turma.limite;
    if (i <= MAX_STUDENTS) {
        strcpy(atual->turma.alunos[i].nome, username);
        atual->turma.tam_atual += 1;
    } else {
        write(client_fd,"A turma está cheia",strlen("A turma está cheia"));
        return;
    }
    char escreve[BUF_SIZE];
    snprintf(escreve, sizeof(escreve), "o aluno: %s inscreveu-se com sucesso", atual->turma.alunos[i].nome);
    write(client_fd,escreve,strlen(escreve));
    printf("%s\n", escreve);
}

void envia_turmas(int client_fd, pLista lista) {
    pLista aux = lista->prox;
    char buffer[BUF_SIZE];
    buffer[0] = '\0';
    if(aux == NULL){
        strcpy(buffer,"Nao há turmas");
        write(client_fd, buffer, strlen(buffer));
        return;
    }

    while (aux) {
        strcat(buffer, aux->turma.disciplina);
        strcat(buffer, "\n");
        aux = aux->prox;
    }

    if (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
    }

    write(client_fd, buffer, strlen(buffer));
}

#endif
