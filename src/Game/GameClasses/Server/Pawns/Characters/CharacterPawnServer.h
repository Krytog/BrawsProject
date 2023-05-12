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

    virtual void Shoot(const Position& position);

    virtual void Translate(const Vector2D& vector2d) override;
    virtual void UpdatePosition(const Position& position) override;

    virtual void Move(const Vector2D& direction) override;
    virtual void ReceiveDamage(double damage);

    double GetHealth() const;
    double GetDamage() const;
    double GetSpeed() const;

    void SetHealth(double health);
    void SetDamage(double damage);
    void SetSpeed(double speed);

    size_t GetCerebrateId() const;
    GameObject* GetFieldOfView() const;

    void BlockShooting();
    void UnblockShooting();

protected:
    size_t cerebrate_id;
    GameObject* field_of_view_;

    double health_;
    double damage_;
    double speed_;

    bool can_shoot_ = true;
};