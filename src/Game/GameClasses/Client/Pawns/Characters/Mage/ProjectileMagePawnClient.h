#pragma once

#include <Game/GameClasses/Client/Pawns/Projectiles/ProjectilePawnClient.h>

class ProjectileMagePawnClient : public ProjectilePawnClient {
public:
    ProjectileMagePawnClient();
    ~ProjectileMagePawnClient() override;

    static const size_t kTypeId;
};
