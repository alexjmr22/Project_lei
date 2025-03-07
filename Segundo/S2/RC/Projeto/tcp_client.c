#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 256

int fd;

// Função de tratamento do sinal SIGINT
void sigint_handler(int signum) {
    printf("\nCtrl+C pressionado. Encerrando o programa.\n");
    write(fd, "SAIR\n", strlen("SAIR\n"));
    close(fd);
    exit(signum);
}

// Função de tratamento do sinal SIGTERM
void sigterm_handler(int signum) {
    printf("\nEncerrando o cliente.\n");
    write(fd, "SAIR\n", strlen("SAIR\n"));
    close(fd);
    exit(signum);
}

void erro(char *msg) {
    perror(msg);
    exit(-1);
}

int main(int argc, char *argv[]) {
    char endServer[100];
    char Login[100];
    char Func[100];
    struct sockaddr_in addr;
    struct hostent *hostPtr;

    if (argc != 3) {
        printf("cliente <host> <port>\n");
        exit(-1);
    }

    strcpy(endServer, argv[1]);
    if ((hostPtr = gethostbyname(endServer)) == NULL)
        erro("Não consegui obter endereço");

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        erro("Erro ao criar socket");
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        erro("Erro ao conectar");

    // Configurando os manipuladores de sinal
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigterm_handler);

    do {
        printf("Login:\n");
        if (fgets(Login, sizeof(Login), stdin) == NULL) {
            break;
        }
        Login[strcspn(Login, "\n")] = '\0';
        write(fd, Login, strlen(Login) + 1); // Envia o login para o servidor

        char buffer[BUF_SIZE];
        int nread = read(fd, buffer, BUF_SIZE - 1); // Lê a resposta do servidor
        if (nread < 0) {
            perror("Erro ao ler do servidor");
            break;
        }
        buffer[nread] = '\0';

        if (strcmp(buffer, "passou") == 0) {
            do {
                printf("Função que deseja executar:\n");
                if (fgets(Func, sizeof(Func), stdin) == NULL) {
                    break;
                }
                Func[strcspn(Func, "\n")] = '\0';
                write(fd, Func, strlen(Func) + 1); // Envia a função para o servidor

                if(strcmp(Func, "CREATE_CLASS") == 0) {
                    char disciplina[BUF_SIZE];
                    printf("Digite o nome da disciplina:\n");
                    if (fgets(disciplina, sizeof(disciplina), stdin) == NULL) {
                        break;
                    }
                    disciplina[strcspn(disciplina, "\n")] = '\0';
                    write(fd, disciplina, strlen(disciplina) + 1); // Envia a disciplina para o servidor
                }else if (strcmp(Func, "LIST_CLASSES") == 0) {
                    nread = read(fd, buffer, BUF_SIZE - 1);
                    buffer[nread] = '\0';
                    printf("Servidor: \n%s\n", buffer);
                }else if (strcmp(Func, "LIST_SUBSCRIBED") == 0) {
                    nread = read(fd, buffer, BUF_SIZE - 1);
                    buffer[nread] = '\0';
                    printf("Servidor: %s\n", buffer);
                }else if (strcmp(Func, "SUBSCRIBE_CLASS") == 0) {
                    printf("insira a turma em que se deseja inscrever: ");
                    if (fgets(buffer, sizeof(Func), stdin) == NULL) {
                        printf("turma nao inserida\n");
                        break;
                    }
                    buffer[strcspn(buffer, "\n")] = '\0';
                    write(fd, buffer, strlen(buffer) + 1); // Envia a função para o servidor
                    //ler se subscreveu
                    nread = read(fd, buffer, BUF_SIZE - 1);
                    buffer[nread] = '\0';
                    printf("Servidor: %s\n", buffer);

                } 
                 
            } while (strcmp(Func, "SAIR") != 0);
        }
        fflush(stdout);
    } while (strcmp(Login, "SAIR") != 0);

    close(fd);
    return 0;
}
