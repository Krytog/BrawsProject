#pragma once

#include "GameObject.h"
#include "Position.h"
#include "VisibleObject.h"

#include <memory>
#include <string_view>

enum LEVELS {
    BACKGROUND_LEVEL = 0,
    FIRST_USER_LEVEL = 1,
    SECOND_USER_LEVEL = 2,
    THIRD_USER_LEVEL = 3
};

class RenderImplementation;

class Render {
public:
    Render();

    void AddToRender(const GameObject *object_ptr, const VisibleObject *vis_obj_ptr);
    void RemoveFromRender(const GameObject *vis_object_ptr);
    void RenderAll();

    void SetCameraOn(const GameObject *object);
    Position GetCameraPosition() const;

    ~Render();

    // temporary, need to get mouse position over the window
    sf::Window *GetWindowPointer() const;

private:
    std::unique_ptr<RenderImplementation> impl_;
};
