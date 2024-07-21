#include <liburing.h>

#define MAX_CONNECTIONS      (10)
#define MAX_MESSAGE_LEN      (512)

char bufs[MAX_CONNECTIONS][MAX_MESSAGE_LEN];

typedef struct conn_info {
    int fd;
    unsigned type;
} conn_info;

conn_info conns[MAX_CONNECTIONS];

enum {
    ACCEPT,
    READ,
    WRITE,
};

void
add_accept(struct io_uring *ring, int fd, struct sockaddr *client_addr, socklen_t *client_len);

void
add_socket_read(struct io_uring *ring, int fd, size_t size);

void
add_socket_write(struct io_uring *ring, int fd, size_t size);