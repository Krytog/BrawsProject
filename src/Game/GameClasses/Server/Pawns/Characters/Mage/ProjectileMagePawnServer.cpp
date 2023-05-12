#include "ProjectileMagePawnServer.h"
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/Server/Cerebrates/Positional/PositionalCerebrateServer.h>
#include <Core/ServerEngine.h>
#include <Game/GameClasses/GameObjectTags.h>

enum {
    COLLIDER_RADIUS = 50,
};

ProjectileMagePawnServer::ProjectileMagePawnServer(const ProjectilePawnServer::ArgPack& argpack): ProjectilePawnServer(argpack) {
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<PositionalCerebrateServer<ProjectileMagePawnServer>>(this);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS, Collider::Category::Trigger);
    tag_ = TAGS_PROJECTILE_MAGE;
}

ProjectileMagePawnServer::~ProjectileMagePawnServer() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id_);
}

const size_t ProjectileMagePawnServer::kTypeId = TypeId_Projectile_Mage;