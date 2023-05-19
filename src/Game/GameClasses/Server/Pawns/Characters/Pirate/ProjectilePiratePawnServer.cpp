#include "ProjectilePiratePawnServer.h"
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/Server/Cerebrates/Positional/PositionalCerebrateServer.h>
#include <Game/GameClasses/Server/Cerebrates/HitSynchronizer/HitSynchronizerCerebrateServer.h>
#include <Core/ServerEngine.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Server/Pawns/Projectiles/HitTrail.h>
#include <Game/GameClasses/Server/Pawns/Characters/CharacterPawnServer.h>

#define COLLIDER_RADIUS 15.5
#define DAMAGE_RADIUS 150
#define TRAIL_STAYS_TICKS 10

ProjectilePiratePawnServer::ProjectilePiratePawnServer(const ProjectilePawnServer::ArgPack& argpack, const Position& destination): ProjectilePawnServer(argpack) {
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<PositionalCerebrateServer<ProjectilePiratePawnServer>>(this);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS, Collider::Category::Trigger);
    tag_ = TAGS_PROJECTILE_PIRATE;
    final_destination_ = ServerEngine::GetInstance().CreateGameObject<ProjectilePiratePawnServer::DestinationPoint>(destination);
}

const size_t ProjectilePiratePawnServer::kTypeId = TypeId_Projectile_Pirate;

void ProjectilePiratePawnServer::LeaveHitTrail() const {
    ServerEngine::GetInstance().CreateGameObject<HitTrail>(TypeId_Trail_Smoke, TRAIL_STAYS_TICKS, *position_);
}

ProjectilePiratePawnServer::~ProjectilePiratePawnServer() {
    if (final_destination_) {
        ServerEngine::GetInstance().Destroy(final_destination_);
    }
}

void ProjectilePiratePawnServer::OnUpdate() {
    const double precision = 15;
    const Vector2D vector_from_destination = final_destination_->GetPosition().GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
    if (vector_from_destination.Length() <= precision) {
        ServerEngine::GetInstance().CreateGameObject<ProjectilePiratePawnServer::Explosion>(final_destination_->GetPosition(), damage_);
        ServerEngine::GetInstance().Destroy(final_destination_);
        final_destination_ = nullptr;
        LeaveHitTrail();
        CorrectSelfDestroy();
        return;
    }
    auto collisions = ServerEngine::GetInstance().GetPhysicalCollisions(this);
    if (!collisions.empty()) {
        ServerEngine::GetInstance().CreateGameObject<ProjectilePiratePawnServer::Explosion>(final_destination_->GetPosition(), damage_);
        LeaveHitTrail();
        CorrectSelfDestroy();
        return;
    }
    Translate(direction_ * speed_);
}

ProjectilePiratePawnServer::DestinationPoint::DestinationPoint(const Position &position) {
    GameObject::UpdatePosition(position);
    collider_ = std::make_unique<CircleCollider>(position, COLLIDER_RADIUS, Collider::Category::Trigger);
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<PositionalCerebrateServer<DestinationPoint>>(this);
}

const size_t ProjectilePiratePawnServer::DestinationPoint::kTypeId = TypeId_Projectile_Mark;

Vector2D ProjectilePiratePawnServer::DestinationPoint::GetRotator() const {
    return Vector2D::Down;
}

ProjectilePiratePawnServer::DestinationPoint::~DestinationPoint() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id_);
}

ProjectilePiratePawnServer::Explosion::Explosion(const Position& position, double damage) {
    *position_ = position;
    collider_ = std::make_unique<CircleCollider>(position, DAMAGE_RADIUS, Collider::Category::Trigger);
    ServerEngine::GetInstance().Invoke(0, &ProjectilePiratePawnServer::Explosion::DealDamageAndDisappear, this, damage);
}

void ProjectilePiratePawnServer::Explosion::DealDamageAndDisappear(double damage) {
    auto collisions = ServerEngine::GetInstance().GetPhysicalCollisions(this);
    for (auto& collision : collisions) {
        if (auto character = dynamic_cast<CharacterPawnServer*>(collision.game_object)) {
            character->ReceiveDamage(damage);
        }
    }
    ServerEngine::GetInstance().CreateEvent([](){return true;}, {}, &ServerEngine::Destroy,
                                            std::make_tuple(&ServerEngine::GetInstance(), this), EventStatus::Disposable);
}