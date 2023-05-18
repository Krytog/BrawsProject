#pragma once

#include <string>

namespace ClientGameManagement {
    void InitGameClient();
    void InitRegistryForOvermind();
    void ReceiveAndHandleFromServer();
    std::string SerializeInput();
};