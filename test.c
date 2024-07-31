#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t test_number = 1;
size_t ok = 0;
size_t fail = 0;

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
    // printf("buffer: %ld \nexpected: %ld\n", strlen(buffer), strlen(expectedResponse));
    int pos;
    if ((pos = strcmp(buffer, expectedResponse)) == 0) {
        printf("\t\tTest %ld: OK\n", test_number++);
        ++ok;
    } else {
        printf("\t\tTest %ld: FAIL\n", test_number++);
        ++fail;

        if (strlen(buffer) != strlen(expectedResponse)) {
            printf("Different length: buffer = %ld expectedResponse = %ld\n", strlen(buffer), strlen(expectedResponse));
            // for (int i = 0; i < strlen(expectedResponse); ++i) {
            //     printf("buf[%d] = %d exp[%d] = %d\n", i, buffer[i], i, expectedResponse[i]);
            // }
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

    // 1. Query person by id. Return empty response
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nquery {\n  getPerson(id: 123) {\n    id\n    name\n  }\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{ \"data\": { \"person\":  } }");

    // 2. Insert mutation. Return null-data
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nmutation {\n  createPerson(id: 123, name: \"Tom\") {\n    id\n    name\n  }\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{ \"data\": { (null) } }");

    // 3. Insert mutation with default id value. Return null-data
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nmutation {\n  createPerson(name: \"Tom\")\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{ \"data\": { (null) } }");

    // 4. Insert mutation with unspecified value for non-null argument. Return bad request
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nmutation {\n  createPerson(id: 123) {\n    id\n    name\n  }\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\n  \"errors\": [\n    {\n      \"message\": \"Bad request\"\n    }\n  ]\n}\n");

    // 5. Two insert mutations in one operation. Return bad request
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nmutation {\n  createPerson(name: \"Tom\") {\n    id\n    name\n  }\n  createPerson(name: \"Tom\") {\n    id\n    name\n  }\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\n  \"errors\": [\n    {\n      \"message\": \"Bad request\"\n    }\n  ]\n}\n");

    // 6. Two insert operations in one request. Return bad request
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nmutation {\n  createPerson(name: \"Tom\") {\n    id\n    name\n  }\n}\nmutation {\n  createPerson(name: \"Tom\") {\n    id\n    name\n  }\n}", 
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\n  \"errors\": [\n    {\n      \"message\": \"Bad request\"\n    }\n  ]\n}\n");

    // 7. Update mutation. Return null-data
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nmutation {\n  updatePerson(id: 123, name: \"Tim\") {\n    id\n    name\n  }\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{ \"data\": { (null) } }");

    // 8. Query person by id. Return inserted and updated row
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nquery {\n  getPerson(id: 123) {\n    id\n    name\n  }\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{ \"data\": { \"person\": [{\"id\":123,\"name\":\"Tim\"}] } }");

    // 9. Delete mutation by id. Return null-data
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nmutation {\n  deletePerson(id: 123) {\n    id\n    name\n  }\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{ \"data\": { (null) } }");

    // 10. Query person by id. Return empty result
    check(sd, "POST /query HTTP/1.1\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: ru,en;q=0.9\nConnection: keep-alive\nContent-Length: 104\nHost: localhost:8080\nOrigin: http://localhost:8080\nReferer: http://localhost:8080/\nAccept: application/json, multipart/mixed\nContent-type: application/json\n\nquery {\n  getPerson(id: 123) {\n    id\n    name\n  }\n}",
              "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{ \"data\": { \"person\":  } }");

    printf("All tests was executed. OK: %ld FAIL: %ld\n", ok, fail);
    return 0;
}