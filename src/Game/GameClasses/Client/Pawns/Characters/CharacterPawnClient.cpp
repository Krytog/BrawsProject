#include "CharacterPawnClient.h"
#include <string>
#include <type_traits>
#include "Core/Position.h"
#include "Core/Qt/PaintTemplates/BasicSprite.h"
#include "Core/Qt/PaintTemplates/VisibleObject.h"
#include "Core/Qt/PaintTemplates/TextSprite.h"
#include "Core/Qt/Helpers/CommonHelpers/DrawTextHelper.h"
#include "Core/Qt/Painters/DummyPainter.h"
#include <memory>

enum {
    TEXT_WIDTH = 50,
    TEXT_HEIGHT = 40,
    DIST_FROM_CENTER = 100,
    DIST_BETWEEN = 60
};

CharacterPawnClient::CharacterPawnClient() = default;

CharacterPawnClient::~CharacterPawnClient() = default;

void CharacterPawnClient::OnUpdate() {
    if (is_controlled_) {
        auto input = ClientEngine::GetInstance().GetInput();
        auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
        Vector2D rotator = mouse_token.position.GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
        visible_object_->UpdateRotation(rotator);
    }
}

int8_t CharacterPawnClient::GetAmmoLeft() const {
    return ammo_left_;
}

double CharacterPawnClient::GetHealth() const {
    return health_;
}

double CharacterPawnClient::GetCooldown() const {
    return cooldown_;
}

void CharacterPawnClient::SetHealth(double health) {
    health_ = health;
}

void CharacterPawnClient::SetAmmoLeft(int8_t ammo) {
    ammo_left_ = ammo;
}

void CharacterPawnClient::SetCooldown(double cooldown) {
    cooldown_ = cooldown;
}

void CharacterPawnClient::CaptureViewPort() {
    ClientEngine::GetInstance().SetCameraOn(this);
    is_controlled_ = true;
}

void CharacterPawnClient::SetRotation(const Vector2D &rotator) {
    visible_object_->UpdateRotation(rotator);
}

void CharacterPawnClient::AddStatsVisualization() const {
    IFlexibleVisibleObject* visible;
    if ((visible = dynamic_cast<IFlexibleVisibleObject*>(visible_object_.get()))) {
        std::shared_ptr<TextSprite> health = std::make_shared<TextSprite>(std::to_string(int64_t(health_)),
             Position(0, 0), TEXT_WIDTH, TEXT_HEIGHT);
        std::shared_ptr<TextSprite> ammo_left = std::make_shared<TextSprite>(std::to_string(int64_t(ammo_left_)),
             Position(0, 0), TEXT_WIDTH, TEXT_HEIGHT);
        std::shared_ptr<TextSprite> cooldown = std::make_shared<TextSprite>(std::to_string(int64_t(cooldown_)),
             Position(0, 0), TEXT_WIDTH, TEXT_HEIGHT);

        visible->ChangeRenderLogic(
            [this, health, ammo_left, cooldown] (Painter* painter) {
                health->ResetText(std::to_string(health_));
                ammo_left->ResetText(std::to_string(ammo_left_));
                cooldown->ResetText(std::to_string(cooldown_));

                auto text_pos = this->GetPosition();
                text_pos.Translate({-DIST_BETWEEN, DIST_FROM_CENTER});

                auto amo_left_pos = this->GetPosition();
                amo_left_pos.Translate({0, DIST_FROM_CENTER});

                auto cooldown_pos = this->GetPosition();
                cooldown_pos.Translate({DIST_BETWEEN, DIST_FROM_CENTER});

                DrawTextHelper(health.get(), text_pos).Paint(painter);
                DrawTextHelper(ammo_left.get(), amo_left_pos).Paint(painter);
                DrawTextHelper(cooldown.get(), cooldown_pos).Paint(painter);
            }
        );
    }
}
