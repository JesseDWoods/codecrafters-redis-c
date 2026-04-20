#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "RESP_parser.h"


#define PORT 6379
#define BACKLOG 5
#define BUFFER_SIZE 1024

// Structure to hold client connection info
typedef struct {
    int sockfd;
    struct sockaddr_in addr;
} ClientConnection;
void process_input(ClientConnection*, RESP_list *);
// Thread function to handle a client
void* handle_client(void *arg) {
    ClientConnection *client = (ClientConnection*)arg;

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Echo loop
    while ((bytes_read = read(client->sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        RESP_list *list = parse_list(buffer);
        process_input(client, list);
        //char response[] = "+PONG\r\n";
        //send(client->sockfd, response, strlen(response), 0); // Echo back
    }

    if (bytes_read == 0) {
        printf("[Thread %lu] Client disconnected.\n", pthread_self());
    } else if (bytes_read < 0) {
        printf("Client connection error.");
    }

    close(client->sockfd);
    free(client); // Free heap memory
    return NULL;
}

// Accepts a client and allocates on heap
ClientConnection* accept_client(int server_fd) {
    ClientConnection *client = malloc(sizeof(ClientConnection));
    if (!client) {
        perror("malloc");
        return NULL;
    }

    socklen_t addr_len = sizeof(client->addr);
    client->sockfd = accept(server_fd, (struct sockaddr*)&client->addr, &addr_len);
    if (client->sockfd < 0) {
        perror("accept");
        free(client);
        return NULL;
    }

    return client;
}

int main() {
    //disable output buffering
    setbuf(stdout, NULL);
	setbuf(stderr, NULL);


	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");

    int server_fd;
    //int client_addr_len;
    struct sockaddr_in client_addr;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ((server_fd == -1)) {
        printf("Socket creation failed: %s...\n", strerror(errno));
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
	// ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEADDR failed: %s \n", strerror(errno));
		return 1;
	}

    // Bind socket
    struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(PORT),
									 .sin_addr = { htonl(INADDR_ANY) },
									};

    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) {
        printf("Bind failed: %s \n", strerror(errno));
		return 1;
	}

    // Listen
    if (listen(server_fd, BACKLOG) != 0) {
        printf("listen failed: %s \n", strerror(errno));
        close(server_fd);
        return EXIT_FAILURE;
    }

    //printf("Server listening on port %d...\n", PORT);

    // Main accept loop
    while (1) {
        ClientConnection *client = accept_client(server_fd);
        if (!client) {
            continue; // Try next connection
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, client) != 0) {
            perror("pthread_create");
            close(client->sockfd);
            free(client);
            continue;
        }

        // Detach thread so resources are freed automatically
        pthread_detach(tid);
    }

    close(server_fd);
    return EXIT_SUCCESS;
}
void process_input(ClientConnection * client, RESP_list *list) {
    while (list != NULL && list->head != NULL) {
        const RESP_element *cmd = list->head;

        char tmp[cmd->size + 1];
        for (unsigned int i = 0; i < cmd->size; i++) {
            tmp[i] = (char)tolower((unsigned char)cmd->element[i]);
        }
        tmp[cmd->size] = '\0';
        printf("%s\n", tmp);

        if (strcmp(tmp, "echo") == 0) {
            if (cmd->next != NULL) {
                const RESP_element* arg = cmd->next;
                printf("$%d\r\n%s\r\n", arg->size, arg->element);
                char response[50];
                sprintf(response,"$%d\r\n%s\r\n", arg->size, arg->element);
                send(client->sockfd, response, strlen(response), 0);
                dequeue(list); /* remove command */
                dequeue(list); /* remove argument */
            } else {
                dequeue(list);
            }
        }else if (strcmp(tmp, "ping") == 0) {
            char * response = "+PONG\r\n";
            send(client->sockfd, response, strlen(response), 0);
            dequeue(list);
        }
        else {
            dequeue(list);
        }
    }
    free_list(list);
}