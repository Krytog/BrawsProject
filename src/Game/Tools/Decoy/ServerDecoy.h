#pragma once

#include <string>

namespace DecoyServer {
    std::string Receive();
    void Send(std::string_view data);
}