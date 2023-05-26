#pragma once

class IDamageable {
public:
    virtual void ReceiveDamage(double damage) = 0;
};