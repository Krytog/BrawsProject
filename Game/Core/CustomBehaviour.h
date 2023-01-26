#pragma once

#include "Engine.h"

class CustomBehaviour {
public:
    CustomBehaviour();

    virtual void OnUpdate() = 0;

protected:
    Engine* engine_;
};
