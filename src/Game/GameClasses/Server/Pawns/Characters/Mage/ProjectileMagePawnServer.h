#pragma once

#include <Game/GameClasses/Server/Pawns/Projectiles//ProjectilePawnServer.h>

class ProjectileMagePawnServer : public ProjectilePawnServer {
public:
    ProjectileMagePawnServer(const ProjectilePawnServer::ArgPack& argpack);
    ~ProjectileMagePawnServer();

    static const size_t kTypeId;
};
