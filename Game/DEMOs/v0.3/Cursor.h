#include "../../Core/CustomBehaviour.h"

class Cursor: public GameObject, public CustomBehaviour {
public:
    Cursor(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag);

    void OnUpdate() override;
};
