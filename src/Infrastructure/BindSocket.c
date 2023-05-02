#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

// returns fd to socket | takes UDP_PORT
int BindToSock(const char *service) {
    struct addrinfo *res = NULL;
    struct addrinfo hint = {
            .ai_family = AF_INET,
            .ai_socktype = SOCK_DGRAM,
    };

    int gai_err = getaddrinfo(NULL, service, &hint, &res);
    if (gai_err != 0) {
        return -1;
    }

    int sock = -1;
    for (struct addrinfo *ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock < 0) {
            continue;
        }
        if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            close(sock);
            sock = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    return sock;
}
