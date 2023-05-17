#pragma once

#include <Core/GameObject.h>

class HitTrail : public GameObject {
public:
    HitTrail(size_t type, uint64_t expire_ticks, const Position& pos);
    size_t GetType() const;

    void OnUpdate() override;

    ~HitTrail();

private:
    size_t cerebrate_id_;
    const size_t type_;
    void DelayedSelfDestruction(uint64_t ticks_to_go);
};
