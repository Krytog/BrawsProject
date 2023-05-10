#include "TextSprite.h"
#include <utility>
#include "VisibleObject.h"
#include "../Helpers/CommonHelpers/DrawTextHelper.h"

TextSprite::TextSprite(std::string_view text, const Position& position,
    const size_t& width, const size_t& height, const double& angle, const RenderLayers& layer):
         IVisibleObject(position, angle, layer), text_(text), width_(width), height_(height) {}

void TextSprite::RenderIt(Painter *painter) {
    DrawTextHelper(this, angle_).Paint(painter);
}

/* Мб потом просто в IVisible сделать такую реализацию */
bool TextSprite::IsDisplayed() const {
    return true;
}

void TextSprite::ResetText(std::string_view text) {
    text_ = text;
}

std::pair<size_t, size_t> TextSprite::GetSize() const {
    return {width_, height_};
}

std::string_view TextSprite::GetText() const {
    return text_;
}