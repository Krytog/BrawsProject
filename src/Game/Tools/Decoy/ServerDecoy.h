#pragma once

#include <string>

namespace DecoyServer {
    std::string ReceiveFromServer();
    void SendToServer(std::string_view data);
}