#pragma once

#include "VisibleObject.h"
#include <string_view>

/* Maybe fix layter using QLineEdit */
class TextSprite: public IVisibleObject {
public:
    TextSprite(std::string_view text, const Position& position,
        const size_t& width, const size_t& height, const RenderLayers& layer = RenderLayers::Objects);

    void RenderIt(Painter *painter) override;
    bool IsDisplayed() const override;

    void ResetText(std::string_view text);
    std::string_view GetText() const;
    std::pair<size_t, size_t> GetSize() const;

    ~TextSprite() = default;
private:
    std::string_view text_;
    size_t width_;
    size_t height_;
};