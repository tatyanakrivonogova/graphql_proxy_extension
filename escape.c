#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

char *escape(const char *s) {
    int len = strlen(s);
    char *t = (char*)malloc((len * 4 + 1)* sizeof(char));
    if (t == NULL) {
        fprintf(stderr, "Malloc failed\n");
        exit(-1);
    }
    
    int j = 0;
    for (int i = 0; i < len; i++) {
        switch (s[i]) {
            case '\a':
                t[j++] = '\\';
                t[j++] = 'a';
                break;
            case '\b':
                t[j++] = '\\';
                t[j++] = 'b';
                break;
            case '\t':
                t[j++] = '\\';
                t[j++] = 't';
                break;
            case '\n':
                t[j++] = '\\';
                t[j++] = 'n';
                break;
            case '\v':
                t[j++] = '\\';
                t[j++] = 'v';
                break;
            case '\f':
                t[j++] = '\\';
                t[j++] = 'f';
                break;
            case '\r':
                t[j++] = '\\';
                t[j++] = 'r';
                break;
            case '\\':
                t[j++] = '\\';
                t[j++] = '\\';
                break;
            case '"':
                t[j++] = '\\';
                t[j++] ='\"';
                break;
            case '\'':
                t[j++] = '\\';
                t[j++] ='\'';
                break;
            default:
                t[j++] = s[i];
                break;
        }
    }
    
    t[j] = '\0';
    return t;
}

int main() {
    char input[BUFFER_SIZE];
    char *escapedStr;

    printf("Input string (ctrl-D to exit):\n");

    int index = 0;
    char c;
    while ((c = fgetc(stdin)) != EOF) {
        input[index++] = c;
    }

    input[index] = '\0';

    escapedStr = escape(input);

    printf("\n%s\n", escapedStr);

    free(escapedStr);
    return 0;
}
