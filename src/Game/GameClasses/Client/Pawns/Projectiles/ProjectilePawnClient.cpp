#include "ProjectilePawnClient.h"

ProjectilePawnClient::ProjectilePawnClient() = default;
ProjectilePawnClient::~ProjectilePawnClient() = default;

void ProjectilePawnClient::UpdateRotation(const Vector2D &rotator) {
    visible_object_->UpdateRotation(rotator);
    visible_object_->Rotate(-start_rotation);
}

void ProjectilePawnClient::OnUpdate() {
    // bruh
}