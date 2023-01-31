#pragma once

#include "Engine.h"

class CustomBehaviour {
public:
    CustomBehaviour(): engine_(&Engine::GetInstance()) {};

    virtual void OnUpdate() = 0;

protected:
    Engine* engine_;
};
