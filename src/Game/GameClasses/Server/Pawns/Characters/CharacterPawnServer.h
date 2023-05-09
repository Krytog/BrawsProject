#pragma once

#include <Core/ServerEngine.h>
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

    virtual void OnUpdate() override;

    virtual void Shoot(const Position& position) = 0;

    virtual void Translate(const Vector2D& vector2d) override;
    virtual void UpdatePosition(const Position& position) override;

    double GetHealth() const;
    double GetDamage() const;
    double GetSpeed() const;

    void SetHealth(double health);
    void SetDamage(double damage);
    void SetSpeed(double speed);

    size_t GetCerebrateId() const;
    GameObject* GetFieldOfView() const;

protected:
    size_t cerebrate_id;
    GameObject* field_of_view_;

    double health_;
    double damage_;
    double speed_;
};