#include "ProjectilePawnServer.h"
#include <Core/ServerEngine.h>
#include <Game/GameClasses/Server/Pawns/Characters/CharacterPawnServer.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/CommandsList.h>

ProjectilePawnServer::ProjectilePawnServer(const ArgPack& argpack): speed_(argpack.speed), damage_(argpack.damage), direction_(argpack.direction) {
    *position_ = argpack.position;
    ServerEngine::GetInstance().Invoke(std::chrono::milliseconds(15000), &ServerEngine::Destroy, &ServerEngine::GetInstance(), this);
}


Vector2D ProjectilePawnServer::GetRotator() const {
    return direction_;
}

void ProjectilePawnServer::OnUpdate() {
    auto collisions = ServerEngine::GetInstance().GetPhysicalCollisions(this);
//    for (auto& collision : collisions) {
//        if (auto character = dynamic_cast<CharacterPawnServer*>(collision.game_object)) {
//            character->ReceiveDamage(damage_);
//            //ServerEngine::GetInstance().Invoke(0, &ServerEngine::Destroy, &ServerEngine::GetInstance(), this);
//            return;
//        }
//        //ServerEngine::GetInstance().Invoke(0, &ServerEngine::Destroy, &ServerEngine::GetInstance(), this);
//        return;
//    }
    //Translate(direction_ * speed_);
}

ProjectilePawnServer::~ProjectilePawnServer() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id_);
}