#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
static int counter = 0;
void *client_count(void *p_client_sc)
{
    counter++;
    printf("Entered client:%d",counter);
    fflush(stdout);
    int client_sc = *((int *)p_client_sc);
    // free(p_client_sc);
    FILE *html_data;
         
    html_data = fopen("index.html", "r");
 
    char response_data[1024];
    // int i = 0;

    char ch;
    fread(response_data, 1, sizeof(response_data), html_data);

    char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
    strcat(http_header, response_data);
    int len = strlen(http_header) - 1;
    send(client_sc, http_header, len, 0);
    close(client_sc);
    fclose(html_data);

    return NULL;
}

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8001);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    listen(server_socket, 5);

    // int counter = 0;
    // int *p_client_socket = malloc(sizeof(int));

    while (1)
    {
        int client_socket;
        client_socket = accept(server_socket, NULL, NULL);
        pthread_t thr;
        int *p_client_socket = malloc(sizeof(int));
        *p_client_socket = client_socket;

        pthread_create(&thr, NULL, client_count, (void *)p_client_socket);
        pthread_join(thr, NULL);
        if (*p_client_socket == 5)
        {
            break;
        }

        // counter--;
        // printf("Active client: %d", counter);
        // if (counter == 0)
        // {
        //     break;
        // }
    }

    return 0;
}
