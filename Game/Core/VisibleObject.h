#include <memory>

#include "Position.h"

class VisibleObject {
public:
    VisibleObject() = default;
    
    virtual void RenderIt() = 0;
};