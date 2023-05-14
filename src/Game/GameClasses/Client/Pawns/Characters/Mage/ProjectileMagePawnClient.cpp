#include "ProjectileMagePawnClient.h"
#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>

#include <Core/ClientEngine.h>

enum {
    SPRITE_WIDTH = 50,
    SPRITE_HEIGHT = 50
};

ProjectileMagePawnClient::ProjectileMagePawnClient() {
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_PROJECTILES_MAGE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, 0, IVisibleObject::RenderLayers::Projectiles);
    tag_ = TAGS_PROJECTILE_MAGE;
}

ProjectileMagePawnClient::~ProjectileMagePawnClient() {}

const size_t ProjectileMagePawnClient::kTypeId = TypeId_Projectile_Mage;