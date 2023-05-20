#include "DamageZonePawnServer.h"

#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Server/Pawns/Characters/CharacterPawnServer.h>
#include <Game/GameClasses/Server/Cerebrates/Zone/ZoneCerebrateServer.h>
#include <SwarmSystem/Overmind.h>
#include <SwarmSystem/TypeIdList.h>

#define DAMAGE_COOLDOWN 1

DamageZonePawnServer::DamageZonePawnServer(double damage, const Position& center, double width, double height) :
    damage_(damage), width_(width), height_(height) {
    *position_ = center;
    collider_ = std::make_unique<RectangleCollider>(center, width, height, Collider::Category::Trigger);
    tag_ = TAGS_ZONES_HAZARD;
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<ZoneCerebrateServer<DamageZonePawnServer>>(this);
}

DamageZonePawnServer::~DamageZonePawnServer() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id_);
}

void DamageZonePawnServer::OnUpdate() {
    //std::cout << position_->GetCoordinates().first << " " << position_->GetCoordinates().second << std::endl;
    if (timer.EvaluateTime() < DAMAGE_COOLDOWN) {
        return;
    }
    timer.ResetTime();
    auto collisions = ServerEngine::GetInstance().GetPhysicalCollisions(this);
    for (auto& collision : collisions) {
        if (auto character = dynamic_cast<CharacterPawnServer*>(collision.game_object)) {
            character->ReceiveDamage(damage_);
        }
    }
}

double DamageZonePawnServer::GetWidth() const {
    return width_;
}

double DamageZonePawnServer::GetHeight() const {
    return height_;
}

const size_t DamageZonePawnServer::kTypeId = TypeId_Zone_Damage;