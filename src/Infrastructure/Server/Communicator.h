#include <string_view>
#include <string>
#include <unordered_map>
#include <memory>
#include <netinet/in.h>


namespace {
    char client_port[] = "31337";
    const int kMaxDtgrmLen = 1024;
    const socklen_t socklen = sizeof(struct sockaddr_in);
}

class Communicator {
public:
    static Communicator &GetInstance();

    // rewrite with epoll
    // REWRITE WHEN REGISTRATION WILL BE READY
    void RegUser();

    // rewrite with epoll
    std::string ReceiveFromClient(uint64_t client_id);

    void SendToClient(uint64_t client_id, std::string_view data);

private:
    Communicator();
    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    ~Communicator();

    int sock_fd_;
    std::unordered_map<int64_t, std::unique_ptr<struct sockaddr_in>> register_;
    std::unordered_map<int64_t, std::string> message_log_;
};