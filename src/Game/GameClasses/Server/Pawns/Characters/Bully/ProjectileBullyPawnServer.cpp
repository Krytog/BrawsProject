#include "ProjectileBullyPawnServer.h"
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/Server/Cerebrates/Positional/PositionalCerebrateServer.h>
#include <Core/ServerEngine.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Server/Pawns/Projectiles/HitTrail.h>

#define COLLIDER_RADIUS 17.5
#define TRAIL_STAYS_TICKS 10

ProjectileBullyPawnServer::ProjectileBullyPawnServer(const ProjectilePawnServer::ArgPack& argpack): ProjectilePawnServer(argpack) {
    ServerEngine::GetInstance().Invoke(std::chrono::milliseconds(250), &ServerEngine::Destroy, &ServerEngine::GetInstance(), this);
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<PositionalCerebrateServer<ProjectileBullyPawnServer>>(this);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS, Collider::Category::Trigger);
    tag_ = TAGS_PROJECTILE_MAGE;
}

const size_t ProjectileBullyPawnServer::kTypeId = TypeId_Projectile_Bully;

void ProjectileBullyPawnServer::LeaveHitTrail() const {
    ServerEngine::GetInstance().CreateGameObject<HitTrail>(TypeId_Trail_Explosion, TRAIL_STAYS_TICKS, *position_);
}