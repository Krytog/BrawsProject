#pragma once

#include <Game/GameClasses/Server/Pawns/Projectiles//ProjectilePawnServer.h>

class DestinationPoint : public GameObject {
public:
    DestinationPoint(const Position& position);
    ~DestinationPoint();

    size_t GetType() const;
private:
    size_t cerebrate_id_;
};

class ProjectilePiratePawnServer : public ProjectilePawnServer {
public:
    ProjectilePiratePawnServer(const ProjectilePawnServer::ArgPack& argpack);
    ~ProjectilePiratePawnServer();

    void OnUpdate() override;

    static const size_t kTypeId;
private:
    void LeaveHitTrail() const override;
    DestinationPoint* final_destination_;
};
