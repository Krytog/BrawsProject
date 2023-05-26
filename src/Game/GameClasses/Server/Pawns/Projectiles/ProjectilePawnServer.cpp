#include "ProjectilePawnServer.h"
#include <Core/ServerEngine.h>
#include <Game/GameClasses/Server/Pawns/Interfaces/IDamageable.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/CommandsList.h>
#include <Game/GameClasses/GameObjectTags.h>


ProjectilePawnServer::ProjectilePawnServer(const ArgPack& argpack): speed_(argpack.speed), damage_(argpack.damage), direction_(argpack.direction) {
    *position_ = argpack.position;
    //ServerEngine::GetInstance().Invoke(std::chrono::milliseconds(15000), &ServerEngine::Destroy, &ServerEngine::GetInstance(), this);
}


Vector2D ProjectilePawnServer::GetRotator() const {
    return direction_;
}

void ProjectilePawnServer::OnUpdate() {
    auto collisions = ServerEngine::GetInstance().GetAllCollisions(this);
    for (auto& collision : collisions) {
        if (collision.tag.starts_with(TAGS_ZONES_STARTSWITH)) {
            continue;
        }
        if (auto damageable = dynamic_cast<IDamageable*>(collision.game_object)) {
            damageable->ReceiveDamage(damage_);
        }
        LeaveHitTrail();
        CorrectSelfDestroy();
        return;
    }
    Translate(direction_ * speed_);
}

ProjectilePawnServer::~ProjectilePawnServer() {
    if (!Overmind::GetInstance().GetCerebrateWithId(cerebrate_id_)->IsDeprecated()) {
        Overmind::GetInstance().DestroyCerebrate(cerebrate_id_);
    }
}

void ProjectilePawnServer::CorrectSelfDestroy() {
    ServerEngine::GetInstance().Invoke(0, &ServerEngine::Destroy, &ServerEngine::GetInstance(), this);
}

void ProjectilePawnServer::ReceiveDamage(double damage) {
    CorrectSelfDestroy();
}