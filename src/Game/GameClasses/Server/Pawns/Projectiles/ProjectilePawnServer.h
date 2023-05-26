#pragma once

#include <Core/GameObject.h>
#include <Game/GameClasses/Server/Pawns/Interfaces/IDamageable.h>

class ProjectilePawnServer : public GameObject, public IDamageable {
public:
    struct ArgPack {
        Position position;
        Vector2D direction;
        double speed;
        double damage;
    };

    ProjectilePawnServer(const ArgPack& argpack);
    virtual ~ProjectilePawnServer();

    void ReceiveDamage(double damage) override;

    virtual void OnUpdate() override;

    Vector2D GetRotator() const;

protected:
    size_t cerebrate_id_;
    double speed_;
    double damage_;
    Vector2D direction_;

    virtual void LeaveHitTrail() const = 0;
    void CorrectSelfDestroy();
};
