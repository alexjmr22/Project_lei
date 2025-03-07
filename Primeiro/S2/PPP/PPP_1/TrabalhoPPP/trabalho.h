#ifndef trabalho_h
#define trabalho_h

typedef struct fila2 {
    char num[50];
    int ano;
    int mes;
    int dia;
    int hora;
    int minutos;
    struct fila2* next;
    struct fila2* prev;
} fila2;

typedef struct {
    fila2* start;
    fila2* end;
} fila_h;

void initialize(fila_h* pr);
int is_empty(fila_h* pr);
void insert(fila_h* pr, char num[], int ano, int mes, int dia, int hora, int minutos);
void remove_node(fila_h* pr, char num[]);
void delete(fila_h* pr);
void save_queue(fila_h* pr);
void print_queue(fila_h* pr);

#endif