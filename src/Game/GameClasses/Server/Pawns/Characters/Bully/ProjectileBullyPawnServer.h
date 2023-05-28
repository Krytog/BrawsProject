#pragma once

#include <Game/GameClasses/Server/Pawns/Projectiles//ProjectilePawnServer.h>
#include <Core/ServerEngine.h>
#include <Core/MyTime.h>

class ProjectileBullyPawnServer : public ProjectilePawnServer {
public:
    ProjectileBullyPawnServer(const ProjectilePawnServer::ArgPack& argpack);

    static const size_t kTypeId;
private:
    void LeaveHitTrail() const override;
};
