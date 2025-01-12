#include <stdio.h>

#include "../include/server.h"

int main() {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (start_server() < 0) {
        printf("Failed to start server");
        return 1;
    }

    return 0;
}

