#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define BUFFER_SIZE 1024

int findDiff(const char * buffer, const char * expectedResponse) {
    for (int i = 0; i < strlen(buffer); ++i) {
        if (buffer[i] != expectedResponse[i]) return i;
    }
    return -1;
}

void check(int sd, const char *request, const char *expectedResponse) {
    int res;
    char buffer[BUFFER_SIZE];
    res = send(sd, request, strlen(request), 0);
    if (res == -1) {
        printf("Failed while sending HTTP request\n");
        close(sd);
        exit(-1);
    }
    res = recv(sd, buffer, BUFFER_SIZE, 0);
    if (res <= 0) {
        printf("Failed while receiving HTTP request\n");
        close(sd);
        exit(-1);
    }
    buffer[res] = '\0';
    
    printf("buffer: %s \nexpected: %s\n", buffer, expectedResponse);
    printf("buffer: %ld \nexpected: %ld\n", strlen(buffer), strlen(expectedResponse));
    int pos;
    if ((pos = strcmp(buffer, expectedResponse)) == 0) {
        printf("Test: OK\n");
    } else {
        printf("Test: FAIL\n");
        if (strlen(buffer) != strlen(expectedResponse)) {
            printf("Different length: buffer = %ld expectedResponse = %ld", strlen(buffer), strlen(expectedResponse));
            return;
        }
        size_t diff = findDiff(buffer, expectedResponse);
        printf("Diff: %ld %c %c\n", diff, buffer[diff], expectedResponse[diff]);
    }
}

int main(int argc, char* argv[]) {
    char proxy_ip[200];
    char proxy_port[200];
    int sd;
    struct sockaddr_in proxy_sd;

    FILE* fout = fopen("response", "w+");
    if (fout == NULL) {
        printf("Failed while open output file\n");
        return 0;
    }

    if (argc < 3) {
        printf("Usage: ./client proxy_ip proxy_port\n");
        return 0;
    }

    strcpy(proxy_ip, argv[1]);
    strcpy(proxy_port, argv[2]);


    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Failed while socket creating\n");
        return -1;
    }

    int flag = 1;
    if (setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int)) < 0) {
        perror("Failed while setting NODELAY");
        return -1;
    }


    memset(&proxy_sd, 0, sizeof(proxy_sd));
    proxy_sd.sin_family = AF_INET;
    proxy_sd.sin_port = htons(atoi(proxy_port));
    inet_pton(AF_INET, proxy_ip, &proxy_sd.sin_addr);


    if (connect(sd, (struct sockaddr *)&proxy_sd, sizeof(proxy_sd)) == -1) {
        printf("Failed while connect\n");
        return -1;
    }

    check(sd, "POST /query HTTP/1.1\r\n"
        "Accept-Encoding: gzip, deflate, br, zstd\r\n"
        "Accept-Language: ru,en;q=0.9\r\n"
        "Connection: keep-alive\r\n"
        "Content-Length: 104\r\n"
        "Host: localhost:8080\r\n"
        "Origin: http://localhost:8080\r\n"
        "Referer: http://localhost:8080/\r\n"
        "Accept: application/json, multipart/mixed\r\n"
        "Content-type: application/json\r\n"
        "\r\n"
        "query {\r\n"
        "  getPerson(id: 8)\r\n"
        "}", 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "\r\n"
        "{ \"data\": { \"person\": [{\"id\":8,\"name\":\"Ben\"}, \n"
        " {\"id\":8,\"name\":\"Ben\"}, \n"
        " {\"id\":8,\"name\":\"Tom\"}, \n"
        " {\"id\":8,\"name\":\"Tom\"}, \n"
        " {\"id\":8,\"name\":\"Tom\"}] } }");
    
    printf("All tests was executed\n");
    return 0;
}