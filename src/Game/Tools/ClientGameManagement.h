#pragma once

#include <string>
#include <NormInfrastructure/Client/Communicator.h>

namespace ClientGameManagement {
    void InitGameClient();
    void InitRegistryForOvermind();
    void ReceiveAndHandleFromServer(Communicator&);
    std::string SerializeInput();
};