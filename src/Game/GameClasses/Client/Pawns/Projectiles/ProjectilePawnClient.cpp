#include "ProjectilePawnClient.h"

ProjectilePawnClient::ProjectilePawnClient() = default;
ProjectilePawnClient::~ProjectilePawnClient() = default;

void ProjectilePawnClient::UpdateRotation(const Vector2D &rotator) {
    visible_object_->UpdateRotation(rotator);
    visible_object_->Rotate(-start_rotation);
}

#include <iostream>

void ProjectilePawnClient::OnUpdate() {
    std::cout << position_->GetCoordinates().first << " " << position_->GetCoordinates().second << std::endl;
}