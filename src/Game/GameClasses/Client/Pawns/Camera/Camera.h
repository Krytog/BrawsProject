#include <Core/GameObject.h>
#include <Core/Colliders.h>

class Camera : public GameObject {
public:
    Camera(GameObject* object_to_follow);
    void OnUpdate() override;

private:
    GameObject* object_to_follow_;
};
