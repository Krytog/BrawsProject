#pragma once

#include <Core/ServerEngine.h>
#include <Core/MyTime.h>
#include <Game/GameClasses/Server/Pawns/Interfaces/IDamageable.h>

class CharacterPawnServer : public GameObject, public IDamageable {
public:
    struct ArgPack {
        double health_max;
        double damage;
        double speed;
        uint8_t ammo_max;
        double cooldown_duration;
    };

    CharacterPawnServer();
    CharacterPawnServer(const ArgPack& arg_pack);
    virtual ~CharacterPawnServer();

    virtual void OnUpdate() override;

    virtual void Shoot(const Position& position);

    virtual void Translate(const Vector2D& vector2d) override;
    virtual void UpdatePosition(const Position& position) override;

    virtual void SetDirection(const Position& looking_at);
    Vector2D GetDirection() const;

    void Move(const Vector2D& direction);
    virtual void ReceiveDamage(double damage) override;
    virtual void Heal(double health);

    double GetHealthCur() const;
    double GetHealthMax() const;
    uint8_t GetAmmoLeft() const;
    uint8_t GetAmmoMax() const;
    double GetCooldownDuration() const;
    double GetCooldown() const;
    double GetDamage() const;
    double GetSpeed() const;

    void SetHealthCur(double health);
    void SetHealthMax(double max_health);
    void SetAmmoLeft(uint8_t ammo_left);
    void SetAmmoMax(uint8_t ammo_max);
    void SetCooldownDuration(double duration);
    void SetDamage(double damage);
    void SetSpeed(double speed);

    size_t GetCerebrateId() const;
    GameObject* GetFieldOfView() const;

    void BlockShooting();
    void UnblockShooting();

    bool IsDead() const;

protected:
    size_t cerebrate_id;
    GameObject* field_of_view_;
    Event* is_dead_event_;

    double health_cur_;
    double health_max_;
    double damage_;
    double speed_;
    Vector2D direction_;

    uint8_t ammo_cur_;
    uint8_t ammo_max_;
    double cooldown_duration_;
    MyTime cooldown_timer_;

    MyTime heal_timer_;
    double heal_rate_ = 0.1;

    bool can_shoot_ = true;

    virtual void MoveImplementation(const Vector2D& direction);

private:
    Event* CreateIsDeadEvent() const;

    void HandleOverTimeHealing();
    void HandleReloading();

    void Reload();
};