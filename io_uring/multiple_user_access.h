
int
reserve_conn_structure(int fd);

int
get_conn_index(int fd, int *index);

void
free_conn_index(int fd);

void
printConns();