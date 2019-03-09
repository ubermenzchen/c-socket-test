#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "structs.h"
#include "util.h"

#define MAX_CLIENTS 5

// O primeiro argumento do programa é a porta usada para escuta TCP
int main(int argc, char **argv) {
    // --- Sem utilidade no momento ... ---
    client *clients[MAX_CLIENTS];
    t_conn *threads[MAX_CLIENTS];

    uint32_t clients_count = 0;

    // ------------------------------------

    // File descriptor do socket do servidor
    int socket_fd;
    
    // Strutura com informações do endereço do socket
    struct sockaddr_in socket_addr;

    memset((void*)&socket_addr, 0, sizeof(socket_addr));

    // Define tipo de familia (IP)
    socket_addr.sin_family = AF_INET;
    // Porta a ser escutada (passada como argumento na timeline)
    socket_addr.sin_port = htons(atoi(argv[1]));
    // Qual endereço ouvir (qualquer um)
    socket_addr.sin_addr.s_addr = INADDR_ANY;

    // Cria o socket, associando a uma variável
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
        err("Socket error");

    // Faz o bind das informações do endereço ao socket
    if(bind(socket_fd, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) < 0)
        err("Bind error");

    // Loop infinito (não muito profissional né ASDKJJASFA)
    /*
        Basicamente enquanto puder aceitar conexões, ele vai aceitar.
        Um máximo de 5 conexões (definido no começo do código)
        Não está totalmente pronto para funcionar corretamente com várias conexões
        Provavelmente o servidor vai fazer um broadcast para todos os clientes...
    */
    while(1) {
        struct sockaddr_in client_addr; 
        socklen_t client_len;

        memset((void*)&client_addr, 0, sizeof(client_addr));
        client_len = sizeof(client_addr);

        t_conn *thr = (t_conn*)calloc(sizeof(t_conn), 1);
        client *cli = (client*)calloc(sizeof(client), 1);
        // Essa estrutura será utilizada para passa o FD da conexão para a thread
        t_args *args = (t_args*)calloc(sizeof(t_args), 1);

        listen(socket_fd , 5);

        // Aceita uma nova conexão
        int client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &client_len);
        if(client_fd < 0)
            err("Accept error");

        printf("%s\n", "Cliente conectado!");

        // Verifica se atingiu o máximo de conexões
        if(clients_count >= MAX_CLIENTS) {
            printf("Há clientes em demasia!\nFechando conexão!\n");
            close(client_fd);
            free(thr);
            free(cli);
            free(args);
            continue;
        }

        cli->fd = client_fd;
        cli->id = clients_count;
        args->fd = client_fd;

        clients[clients_count] = cli;
        threads[clients_count++] = thr;

        // Cria as threads passando o file descriptor como argumentos
        pthread_create(&thr->receiver, NULL, receiver, (void*)args);
        pthread_create(&thr->sender, NULL, sender, (void*)args);

    }

    //Nunca vai chegar aqui, mas, quem sabe em uma futura implementação...
    for(int i = 0; i < clients_count; i++){
        free(threads[i]);
        free(clients[i]);
    }
    
    return 0;
}