#pragma once

#include <Core/VisibleGameObject.h>

class Cursor : public VisibleGameObject {
public:
    Cursor();

    void OnUpdate() override;
};
