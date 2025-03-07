#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "Server.h"

#define SERVER_PORT 9002
#define BUF_SIZE 1024
#define BUFLEN 512
#define SERVER_IP "127.0.0.1"
#define PORT 9876
#define SHM_KEY 1234
#define SHM_SIZE sizeof(NoLista) * 10

int running = 1;
int client = -1;
int s = -1;
int client_count = 0;

// Função de tratamento do sinal SIGINT
void sigint_handler(int signum) {
    printf("\nCtrl+C pressionado. Encerrando o programa.\n");
    exit(signum);
}

void erro(char *msg) {
    printf("Erro: %s\n", msg);
    exit(-1);
}

void handle_sigint(int sig) {
	if(sig==SIGTERM){
		running = 0;
		if(client!=-1){
			close(client);
			client = -1;
		}
		if(s!=-1){
			close(s);
			s=-1;
		}
	}
	

	exit(0);
	
	
}

void udp_server(pid_t tcp_pid){
	struct sockaddr_in si_minha, si_outra;

    int recv_len;
    socklen_t slen = sizeof(si_outra);
    char buf[BUFLEN];
    int num;

    // Cria um socket para recepção de pacotes UDP
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        erro("Erro na criação do socket");
    }

    // Preenchimento da socket address structure
    si_minha.sin_family = AF_INET;
    si_minha.sin_port = htons(PORT);
    si_minha.sin_addr.s_addr = htonl(INADDR_ANY);

    // Associa o socket à informação de endereço
    if (bind(s, (struct sockaddr*)&si_minha, sizeof(si_minha)) == -1) {
        erro("Erro no bind");
    }

    while (1) {
        // Espera recepção de mensagem (a chamada é bloqueante)
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, &slen)) == -1) {
            erro("Erro no recvfrom");
        }
        // Para ignorar o restante conteúdo (anterior do buffer)
        buf[recv_len]='\0';
        FILE* f = fopen("dom.txt","r");
        char *username = strtok(buf," ");
        char *password = strtok(NULL,"\n");
        char line[100];
        int verifica = 0;
       	while(fgets(line,sizeof(line),f)!=NULL){
       		char *fuser = strtok(line, ";");
       		char *fpass = strtok(NULL, ";");
       		char *cargo = strtok(NULL, " ");
       		
       		if((strcmp(fuser,username)==0)&&(strcmp(fpass,password)==0)&&(strcmp(cargo,"administrador\n")==0)){
       			verifica = 1;
       			fclose(f);
       			if (sendto(s, "passou", strlen("passou"), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        			erro("Erro no envio da resposta");
        		}
        		
        		do{
        		
       				if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, &slen)) == -1) {
            			erro("Erro no recvfrom");
        			}
        			// Para ignorar o restante conteúdo (anterior do buffer)
        			buf[recv_len]='\0';
        			
        			char *funcao = strtok(buf," ");
        			char *username = strtok(NULL," ");
        			char *password = strtok(NULL," ");
        			char *cargo = strtok(NULL,"\n");
        			
        			
        			
        			if(strcmp(funcao,"ADD_USER")==0){
        				if((username == NULL || password ==NULL || cargo == NULL)||(strcmp(cargo,"aluno")!=0 && strcmp(cargo,"professor")!=0 && strcmp(cargo,"administrador")!=0)){
        					char resposta[100] = "ADD_USER <username> <password> <aluno/professor/administrador>";
        					if (sendto(s, resposta, strlen(resposta), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        						erro("Erro no envio da resposta");
        					}
        				}else{
        					
       						int contem = 0;
       						char line[100];
       						FILE* f = fopen("dom.txt","r");
       						
       						//ler o ficheiro
       						while(fgets(line, sizeof(line),f)!=NULL){
       							
       							char *user = strtok(line,";");
       							char *pass = strtok(NULL,";");
       							char *carg = strtok(NULL," ");
				
       							if(strcmp(username, user)==0){
       								contem = 1;
       							}
       						}
       						fclose(f);
       						
       						//se existir não adiciona se não passa
       						if(contem == 1){
       							char resposta[100] = "JÁ EXISTE";
        						if (sendto(s, resposta, strlen(resposta), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        							erro("Erro no envio da resposta");
        						}
       						}else if(contem==0){
       							
       							FILE* f = fopen("dom.txt","a");
       							char escreve[100];  
        						strcpy(escreve, "");
        						strcat(escreve, username);
        						strcat(escreve, ";");
        						strcat(escreve, password);
        						strcat(escreve, ";");
        						strcat(escreve, cargo);
        						
        						fprintf(f, "%s\n", escreve);
        						fclose(f);
        						
        						char resposta[100] = "ADICIONADO";
        						if (sendto(s, resposta, strlen(resposta), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        							erro("Erro no envio da resposta");
        						}
       						}
        				}
        				
        			}else if(strcmp(funcao,"DEL")==0){
        				
        				FILE* f = fopen("dom.txt","r");
        				FILE* apaga = fopen("Delete.txt","w");
        				char line[100];
        				char *usercerto = strtok(username,"\n");
        				while(fgets(line,sizeof(line),f)!=NULL){
        					char *user = strtok(line,";");
       						char *pass = strtok(NULL,";");
       						char *carg = strtok(NULL," ");
       						if(strcmp(usercerto,user)!=0){
       							char escreve[100];
       							strcpy(escreve, "");
        						strcat(escreve, user);
        						strcat(escreve, ";");
        						strcat(escreve, pass);
        						strcat(escreve, ";");
        						strcat(escreve, carg);
       							fputs(escreve,apaga);
       						}
        				}
        				fclose(f);
        				fclose(apaga);
        				
        				FILE* users = fopen("dom.txt","w");
        				FILE* apagados = fopen("Delete.txt","r");
        				while(fgets(line,sizeof(line),apagados)!=NULL){
        					fputs(line,users);
        				}
        				fclose(users);
        				fclose(apagados);
        				
        				char resposta[100] = "APAGADO";
        				if (sendto(s, resposta, strlen(resposta), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        					erro("Erro no envio da resposta");
        				}
        				
        			}else if(strcmp(buf,"LIST\n")==0){
        			
        				FILE* f = fopen("dom.txt","r");
        				
        				char line[100];
        				while(fgets(line,sizeof(line),f)!=NULL){
        					char *user = strtok(line,";");
       						char *pass = strtok(NULL,";");
       						char *carg = strtok(NULL," ");
       						if (sendto(s, user, strlen(user), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        					erro("Erro no envio da resposta");
        					}
       						
        					
        				}
        				fclose(f);
        				char resposta[100] = "SAIR";
        				if (sendto(s, resposta, strlen(resposta), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        					erro("Erro no envio da resposta");
        				}
        				
        			}else if(strcmp(funcao,"SAIR\n")==0){
        				
        				char resposta[100] = "parou";
        				if (sendto(s, resposta, strlen(resposta), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        					erro("Erro no envio da resposta");
        				}
        			}else if(strcmp(funcao,"QUIT_SERVER\n")==0){
        				char resposta[100] = "desligou";
        				if (sendto(s, resposta, strlen(resposta), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        					erro("Erro no envio da resposta");
        				}
        				kill(tcp_pid, SIGTERM);
        				close(s);
        				close(client);
        				
        			}
        		
        		}while(strcmp(buf,"SAIR\n")!=0);
       		
       		
       		}
       	
       	
       	
       	}
       	if(verifica ==0){
       		if (sendto(s, "parou", strlen("parou"), 0, (struct sockaddr*)&si_outra, slen) == -1) {
        		erro("Erro no envio da resposta");
        	}
       	}
       	
        
        
        

       

        
        
    }

    // Fecha socket e termina programa (nunca alcançado no loop infinito)
    close(s);
    
}

void process_client(int client_fd, pLista t, Turma e) {
    int nread = 0;
    char buffer[BUF_SIZE];
    int conect = 1;
    do {
        FILE* f;
        int verifica;
        char line[100];
        char escreve[100];
        strcpy(escreve, "");
        verifica = 0;
        nread = read(client_fd, buffer, BUF_SIZE - 1);
        buffer[nread] = '\0';

        if (strcmp(buffer, "SAIR") == 0) {
            conect = 0;
            write(client_fd, "Até logo!\n", strlen("Até logo!\n"));
            break;
        }

        if (conect != 0) {
            f = fopen("dom.txt", "r");
            char *username = strtok(buffer, " ");
            char *password = strtok(NULL, " \n");

            int executou = 0;
            char carg_aceite[100];

                
            while (fgets(line, sizeof(line), f) != NULL) {
                char *fuser = strtok(line, ";");
                char *fpass = strtok(NULL, ";");
                char *cargo = strtok(NULL, " ");

                if ((strcmp(fuser, username) == 0) && (strcmp(fpass, password) == 0)) {
                    strcat(escreve, "passou");
                    write(client_fd, escreve, 1 + strlen(escreve));
                    char func[BUF_SIZE];
                    nread = 0;
                    do {
                        nread = read(client_fd, func, BUF_SIZE - 1);
                        func[nread] = '\0';
                        printf("%s", func);
                        if (strcmp(func, "SAIR") != 0) {
                            if (strcmp(cargo, "professor\n") == 0) {
                                if (strcmp(func, "CREATE_CLASS") == 0) {
                                    printf("Função CREATE_CLASS aceite.\n");
                                    char discip[BUF_SIZE];
                                    nread = read(client_fd, discip, BUF_SIZE - 1);
                                    discip[nread] = '\0';
                                    printf("Disciplina: %s\n", discip);
                                    strncpy(e.disciplina, discip, sizeof(e.disciplina));
                                    e.limite = 25;
                                    e.tam_atual=0;
                                    insere(t, e);

                                } else if (strcmp(func, "SEND") == 0) {
                                    printf("Função SEND aceite\n");
                                } else {
                                    printf("Não tem permissão ou não existe a função\n");
                                }
                            } else if (strcmp(cargo, "aluno\n") == 0) {
                                if (strcmp(func, "LIST_CLASSES") == 0) {
                                    printf("Função LIST_CLASSES aceite\n");
                                    envia_turmas(client_fd, t);
                                    printf("Enviei ver\n");
                                } else if (strcmp(func, "LIST_SUBSCRIBED") == 0) {
                                    printf("Função LIST_SUBSCRIBED aceite\n");
                                    if (pesquisa_aluno(t, username,client_fd) == 1) {
                                        imprimir_turmas_aluno(t, username,client_fd);
                                    }
                                } else if (strcmp(func, "SUBSCRIBE_CLASS") == 0) {
                                    printf("Função SUBSCRIBE_CLASS aceite\n");
                                    subscribe_class(client_fd,t,username);
                                } else {
                                    printf("Não tem permissão ou não existe a função\n");
                                }
                            } else if (strcmp(cargo, "administrador\n") == 0) {
                                printf("Administrador não tem funções específicas\n");
                            }
                        }
                    } while (strcmp(func, "SAIR") != 0);
                }
            }
            write(client_fd,"nao passa",strlen("nao passa"));
            fclose(f);
        }
    } while (conect);
    close(client_fd);
}

int main() {
    // Memória compartilhada
    int shmid;
    pLista lista = cria();
    
    shmid = shmget(SHM_KEY, sizeof(Turma), IPC_CREAT | 0660);
    if (shmid < 0) exit(1);
    Turma *turma; // Pointer to Turma

    turma = (Turma *)shmat(shmid, NULL, 0); // Attach shared memory segment
    if (turma == (Turma *) -1) {
        perror("shmat");
        exit(1);
    }

    // Configuração do servidor
    int fd, client, client_count = 0;
    struct sockaddr_in addr, client_addr;
    int client_addr_size;
    signal(SIGINT, sigint_handler);

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        erro("Erro na função socket");
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        erro("Erro na função bind");
    if (listen(fd, 5) < 0)
        erro("Erro na função listen");
        
    pid_t manager = fork();
    if(manager == 0){
    	udp_server(getppid());
    	exit(0);
    }
    client_addr_size = sizeof(client_addr);
	
	signal(SIGTERM, handle_sigint);
    while (running) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
        client = accept(fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_size);
        if (client > 0) {
            client_count++;
            if (fork() == 0) {
                close(fd);
                printf("** Nova mensagem recebida **\n");
                printf("Cliente %d conectando-se de (IP:porta) %s:%d\n", client_count, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                fflush(stdout);

                process_client(client, lista, *turma);

                printf("Para encerrar, pressione Ctrl+C\n");

                exit(0);
            }
            close(client);
        }
    }

    shmdt((const void *) turma); // Detach shared memory
    lista = destroi(lista);
    close(fd);
    return 0;
}
