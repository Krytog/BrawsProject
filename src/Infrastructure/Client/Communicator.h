#include <string_view>
#include <string>
#include <unordered_map>
#include <memory>
#include <netinet/in.h>
#include <arpa/inet.h>


namespace {
    char server_port[] = "31337";
    int server_port_num = 31337;
    struct sockaddr_in server_addr = {.sin_family = AF_INET, .sin_port = ntons(server_port_num)};
    const int kMaxDtgrmLen = 1024;
    const socklen_t socklen = sizeof(struct sockaddr_in);
}

class Communicator {
public:
    static Communicator &GetInstance();

    // rewrite with epoll
    // REWRITE WHEN REGISTRATION WILL BE READY
    void RegOnServer();

    // rewrite with epoll
    std::string ReceiveFromServer();
    void SendToServer(std::string_view data);

private:
    Communicator();
    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    ~Communicator();

    int sock_fd_;
};