#pragma once

#include <Core/ServerEngine.h>

class LudusMapServer : public PersistentObject {
public:
    LudusMapServer();

    static const size_t kWidth;
    static const size_t kHeight;
};