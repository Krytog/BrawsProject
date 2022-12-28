#include <memory>

class VisibleObjectImpl;

class VisibleObject {
public:

    VisibleObject();

    void RenderIt();

private:
    std::unique_ptr<VisibleObjectImpl> visible_; 
};