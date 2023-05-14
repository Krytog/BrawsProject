#include "ProjectileMagePawnClient.h"
#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>

#include <Core/ClientEngine.h>
#include <Core/MyTime.h>

enum {
    SPRITE_WIDTH = 200,
    SPRITE_HEIGHT = 200
};

ProjectileMagePawnClient::ProjectileMagePawnClient() {
    MyTime time;
    visible_object_ = std::make_unique<AnimatedSprite>(RES_PATH_PROJECTILES_MAGE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, 1, 14, true, 0, IVisibleObject::RenderLayers::Projectiles);
    tag_ = TAGS_PROJECTILE_MAGE;
    std::cout << "CREATING TOOK " << time.EvaluateTime() * 1000 << std::endl;
}

ProjectileMagePawnClient::~ProjectileMagePawnClient() {}

const size_t ProjectileMagePawnClient::kTypeId = TypeId_Projectile_Mage;