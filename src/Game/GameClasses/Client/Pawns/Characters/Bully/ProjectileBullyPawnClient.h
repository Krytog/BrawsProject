#pragma once

#include <Game/GameClasses/Client/Pawns/Projectiles/ProjectilePawnClient.h>

class ProjectileBullyPawnClient : public ProjectilePawnClient {
public:
    ProjectileBullyPawnClient();
    ~ProjectileBullyPawnClient() override;

    static const size_t kTypeId;
};
