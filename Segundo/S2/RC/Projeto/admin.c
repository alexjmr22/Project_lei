#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 9876
#define BUFLEN 512

void erro(char *s) {
    perror(s);
    exit(1);
}

int main(void) {
    struct sockaddr_in si_servidor;
    int s, i, slen=sizeof(si_servidor), recv_len;
    char buf[BUFLEN];

    // Cria um socket UDP
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        erro("Erro na criação do socket");
    }

    // Preenche a estrutura do endereço do servidor
    memset((char *) &si_servidor, 0, sizeof(si_servidor));
    si_servidor.sin_family = AF_INET;
    si_servidor.sin_port = htons(PORT);
    if (inet_aton(SERVER_IP , &si_servidor.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() falhou\n");
        exit(1);
    }
    char login[100];
    do{
    	printf("Login:\n");
    	
    	fgets(login,sizeof(login),stdin);
    	if (sendto(s, login, strlen(login) , 0 , (struct sockaddr *) &si_servidor, slen)==-1) {       
        	erro("Erro no envio");
    	}
    	if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_servidor, &slen)) == -1) {
        	erro("Erro no recvfrom");
    	}
		
    	buf[recv_len]='\0';
    	if(strcmp(buf,"passou")==0){
    		char funcao[100];
    		do{
			
    			// Solicita que o usuário insira um número decimal
    			printf("funções:\n");
    			printf("ADD_USER <username> <password> <aluno/professor/administrador>\n");
    			printf("DEL <username>\n");
    			printf("LIST\n");
    			printf("Insira a função: \n");
    			
    			fgets(funcao, sizeof(funcao),stdin);
    			char *func = funcao;
    			
    			
				
				
    			// Envia o número para o servidor
    			if (sendto(s, func, strlen(func) , 0 , (struct sockaddr *) &si_servidor, slen)==-1) {       
        			erro("Erro no envio");
    			}
    			
    			// Espera pela resposta do servidor
    			if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_servidor, &slen)) == -1) {
        			erro("Erro no recvfrom");
    			}
				
    			
    			buf[recv_len]='\0';
    			printf("%s\n",buf);
    			if(strcmp(funcao,"LIST\n")==0){
    				while(strcmp(buf,"SAIR")!=0){
    					if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_servidor, &slen)) == -1) {
        				erro("Erro no recvfrom");
    					}
    					buf[recv_len]='\0';
    					if(strcmp(buf,"SAIR")!=0){
    						printf("%s\n", buf);
    					}
    					
    				}
					
    				
				}
				if(strcmp(funcao,"QUIT_SERVER")==0){
    				close(s);
    				
    			}
				
    			
    			printf("\n\n\n");
    		
    		}while(strcmp(funcao,"SAIR\n")!=0);

    	
    	}
    	if(strcmp(buf,"parou")==0){
    		printf("Não é administrador\n");
    	}
    }while(strcmp(login,"SAIR\n")!=0);
	
    // Fecha o socket
    close(s);
    return 0;
}

