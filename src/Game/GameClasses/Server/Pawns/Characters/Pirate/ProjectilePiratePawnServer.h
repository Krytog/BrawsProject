#pragma once

#include <Game/GameClasses/Server/Pawns/Projectiles//ProjectilePawnServer.h>

class ProjectilePiratePawnServer : public ProjectilePawnServer {
public:
    ProjectilePiratePawnServer(const ProjectilePawnServer::ArgPack& argpack, const Position& destination);
    ~ProjectilePiratePawnServer();

    void OnUpdate() override;

    static const size_t kTypeId;

private:

    class DestinationPoint : public PersistentObject {
    public:
        DestinationPoint(const Position& position);
        ~DestinationPoint();

        Vector2D GetRotator() const;

        static const size_t kTypeId;
    private:
        size_t cerebrate_id_;
    };

    class Explosion : public PersistentObject {
    public:
        Explosion(const Position& position, double damage, GameObject* owner);
        void DealDamageAndDisappear(double damage);
    private:
        GameObject* owner_;
    };

    void LeaveHitTrail() const override;
    GameObject* final_destination_;
};
