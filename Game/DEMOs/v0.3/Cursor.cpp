#include "Cursor.h"

Cursor::Cursor(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr,
               std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag)
    : GameObject(pos_ptr, coll_ptr, vis_ptr, tag) {
}

void Cursor::OnUpdate() {
    auto input = engine_->GetInput();
    auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
    UpdatePosition(mouse_token.position);
}