#include "ProjectilePiratePawnServer.h"
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/Server/Cerebrates/Positional/PositionalCerebrateServer.h>
#include <Game/GameClasses/Server/Cerebrates/HitSynchronizer/HitSynchronizerCerebrateServer.h>
#include <Core/ServerEngine.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Server/Pawns/Projectiles/HitTrail.h>

#define COLLIDER_RADIUS 15.5
#define TRAIL_STAYS_TICKS 60

ProjectilePiratePawnServer::ProjectilePiratePawnServer(const ProjectilePawnServer::ArgPack& argpack): ProjectilePawnServer(argpack) {
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<PositionalCerebrateServer<ProjectilePiratePawnServer>>(this);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS, Collider::Category::Trigger);
    tag_ = TAGS_PROJECTILE_PIRATE;
}

const size_t ProjectilePiratePawnServer::kTypeId = TypeId_Projectile_Pirate;

void ProjectilePiratePawnServer::LeaveHitTrail() const {
    ServerEngine::GetInstance().CreateGameObject<HitTrail>(TypeId_Trail_Smoke, TRAIL_STAYS_TICKS, *position_);
}

ProjectilePiratePawnServer::~ProjectilePiratePawnServer() {
    ServerEngine::GetInstance().Destroy(final_destination_);
}

void ProjectilePiratePawnServer::OnUpdate() {
    const double precision = 0.005;
    const Vector2D vector_from_destination = final_destination_->GetPosition().GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
    if (vector_from_destination.Length() <= precision) {
        LeaveHitTrail();
        CorrectSelfDestroy();
    }
    ProjectilePawnServer::OnUpdate();
}

DestinationPoint::DestinationPoint(const Position &position) {
    GameObject::UpdatePosition(position);
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<HitSynchronizerCerebrateServer<DestinationPoint>>(this);
}

size_t DestinationPoint::GetType() const {
    return TypeId_Trail_Mark;
}

DestinationPoint::~DestinationPoint() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id_);
}