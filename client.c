#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    char proxy_ip[200];
    char proxy_port[200];
    char buffer[BUFFER_SIZE];
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

    int bytes = 0;

    while (1) {
        freopen("/dev/tty", "r", stdin);
        memset(buffer, '\0', BUFFER_SIZE);
        printf("Enter HTTP-request (or press Ctrl-D to finish):\n");

        int index = 0;
        char c;
        while ((c = fgetc(stdin)) != EOF) {
            buffer[index++] = c;
            if (index == BUFFER_SIZE) {
                bytes = write(sd, buffer, index);
                if (bytes == -1) {
                    printf("Failed while sending HTTP request\n");
                    return -1;
                }
                memset(buffer, '\0', BUFFER_SIZE);
                index = 0;
            }
        }
        
        // user entered only Ctrl-D to exit program
        if (index == 0 && feof(stdin)) {
            break;
        }

        // send http-request
        bytes = write(sd, buffer, index);
        if (bytes == -1) {
            printf("Failed while sending HTTP request\n");
            return -1;
        }

        int all_bytes = 0;

        printf("Server response:\n");
        while(1) {
            // receive http-response
            bytes = read(sd, buffer, BUFFER_SIZE);
            if (bytes <= 0) {
                break;
            }
            all_bytes += bytes;
            // save response to file
            fwrite(buffer, sizeof(char), bytes, fout);
            fflush(fout);
            break;
        }
        printf("End of response. Common size: %d\n", all_bytes);
    }

    printf("Connection closed\n");

    close(sd);
    return 0;
}