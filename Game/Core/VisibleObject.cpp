#include "VisibleObject.h"
#include "Position.h"

class VisibleObjectImpl {
public:
    
    void RenderIt() {
        // some basic render
    };

private:
    Position top_left_;
    double width_;
    double height_;
};

VisibleObject::VisibleObject() {
    visible_ = std::make_unique<VisibleObjectImpl>();
}

void VisibleObject::RenderIt() {
    visible_->RenderIt();
}