#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int counter = 0;
void *client_count(void *p_client_sc)

{
    
    pthread_mutex_lock(&lock);
    counter++;

    printf("Logged client: %d\n",counter);

    pthread_mutex_unlock(&lock);
    int client_sc = *((int *)p_client_sc);
    FILE *html_data;
   
    html_data = fopen("//home//mint//Desktop//thread//index.html", "r");
 
    char response_data[1024];
    


    char ch;
    fread(response_data, 1, sizeof(response_data), html_data);
    

    char http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
    strcat(http_header, response_data);
    
    int len = strlen(http_header) - 1;
    send(client_sc, http_header, len, 0);
    
    close(client_sc);
    free(p_client_sc);
    fclose(html_data);
    
    fflush(stdout);
    pthread_exit(NULL);
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
    while (1)
    {
        
        int client_socket;
        client_socket = accept(server_socket, NULL, NULL);
        pthread_t thr;
        int *p_client_socket = malloc(sizeof(int));
        *p_client_socket = client_socket;

        pthread_create(&thr, NULL, client_count, (void *)p_client_socket);
        pthread_detach(thr);
        pthread_mutex_lock(&lock);
        printf("Active client: %d\n",counter);   
        pthread_mutex_unlock(&lock);   
    }

    return 0;
}
