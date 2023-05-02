#include <stdexcept>
#include <unistd.h>
#include <cstring>

#include "Communicator.h"


/* Что надо сделать
 * Написать бинд на сокет
 *      *Пока без epolla, потом надо будет его завезти
 * Дальше по запросу вскрываем датаграмму по сокету
 *      *Узнать как внутри устроена очередь датаграмм
*/

// добавить пересылку больших датаграмм

Communicator &Communicator::GetInstance() {
    static Communicator instance;
    return instance;
}

extern "C" int BindToSock(char *service);

Communicator::Communicator() {
    sock_fd_ = BindToSock(client_port);
    if (sock_fd_ < 0) {
        throw std::logic_error("Unable to bind port\n");
    }

    std::random_device rdev;
    rand_gen_ = std::mt19937_64(rdev());
}

Communicator::~Communicator() {
    if (sock_fd_) {
        close(sock_fd_);
    }
}

uint64_t Communicator::GenId() {
    do {
        uint64_t num = rand_gen_();
        if (!ids_.contains(num)) {
            ids_.insert(num);
            return num;
        }
    } while (true);
}

int64_t Communicator::RegUser() {
    std::unique_ptr<struct sockaddr_in> cli_addr(new struct sockaddr_in);
    char buf[kMaxDtgrmLen];
    socklen_t cliaddr_len = sizeof(struct sockaddr_in);

    int n = recvfrom(sock_fd_, buf, kMaxDtgrmLen, 0,
                     reinterpret_cast<struct sockaddr *>(cli_addr.get()), &cliaddr_len);
    if (n == -1) {
        return 0;
    }

    auto usr_id = GenId();
    char payload[sizeof(usr_id)];
    memcpy(payload, &usr_id, sizeof(usr_id));

    sendto(sock_fd_, payload, sizeof(usr_id), MSG_CONFIRM, reinterpret_cast<struct sockaddr *>(cli_addr.get()),
           cliaddr_len);
    register_[usr_id].first = std::move(cli_addr);
    register_[usr_id].second = cliaddr_len;
    return usr_id;
}

void Communicator::SendToClient(uint64_t client_id, std::string_view data) {
    sendto(sock_fd_, data.data(), data.size(), 0, reinterpret_cast<struct sockaddr *>(register_[client_id].first.get()), register_[client_id].second);
}

std::string Communicator::ReceiveFromClient(uint64_t client_id) {
    if (!message_log_[client_id].empty()) {
        std::string last_message = std::move(message_log_[client_id]);
        message_log_[client_id] = std::string();
        return std::move(last_message);
    }

    std::string cur_message;
    cur_message.resize(kMaxDtgrmLen);
    int n = recv(sock_fd_, &cur_message[0], kMaxDtgrmLen, MSG_DONTWAIT);
    if (n == -1 || n == 0) {
        return "";
    }
    cur_message.resize(n);

    int64_t id;
    std::memcpy(&id, &cur_message[0], sizeof(id));
    if (id == client_id) {
        return std::move(cur_message);
    }

    message_log_[id] = std::move(cur_message);
    return "";
}
