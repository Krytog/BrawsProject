#pragma once

#include <Core/ClientEngine.h>

class Particles : public PersistentVisibleObject {
public:
    struct ArgPack {
        std::string_view path;
        Position pos;
        size_t width;
        size_t height;
        size_t ticks;
        size_t frames;
        double rotation;
    };

    Particles();
    Particles(const ArgPack& pack);

    void PrepareParticles(const ArgPack& pack);

    virtual ~Particles();

protected:
    void MakeInvokeForDestroy(size_t delay_ticks);
};
