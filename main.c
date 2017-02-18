#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>


void handle_request(int client_socket, struct sockaddr_in * client_address)
{
    char *client_ip = inet_ntoa(client_address->sin_addr);
    int client_port = ntohs(client_address->sin_port);

    printf("New request from %s:%d\n", client_ip, client_port);

    const char* web_page =
        "HTTP/1.0 200 OK\r\n"
        "Server: customCGI\r\n"
        "Content-type: text/html\r\n"
        "\r\n"
        "<html>\r\n"
        " <body>\r\n"
        "  <h1> Something works </h1>"
        "  <h4> My congratulations! </h4>"
        " </body>\r\n"
        "</html>\r\n";

      write(client_socket, web_page, strlen(web_page));

      close(client_socket);
}

void server(char* host, int port) {
    int server_socket, client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    socklen_t client_address_size = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_addr.s_addr = inet_addr(host);
    server_address.sin_family = AF_INET;    
    server_address.sin_port = htons(port);

    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    listen(server_socket, 10);
    
    while (client_socket =  accept(server_socket, 
                            (struct sockaddr*) &client_address, 
                            &client_address_size)) {
        handle_request(client_socket, &client_address);
    }
}

int main(int argc, char *argv[]) {
    server(argv[1], atoi(argv[2]));
    
    return 0;
}
