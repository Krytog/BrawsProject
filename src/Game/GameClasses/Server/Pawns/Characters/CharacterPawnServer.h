#pragma once

#include <Core/Engine.h>
#include <Game/GameClasses/Server/Pawns/Interfaces/IMovable.h>

class CharacterPawnServer : public IMovable {
public:
    struct ArgPack {
        double health;
        double damage;
        double speed;
    };

    CharacterPawnServer();
    CharacterPawnServer(const ArgPack& arg_pack);
    virtual ~CharacterPawnServer();
    virtual void OnUpdate();

    virtual void Shoot(const Position& position) = 0;

    double GetHealth() const;
    double GetDamage() const;
    double GetSpeed() const;

    void SetHealth(double health);
    void SetDamage(double damage);
    void SetSpeed(double speed);

    size_t GetCerebrateId() const;

protected:
    size_t cerebrate_id;

    double health_;
    double damage_;
    double speed_;
};