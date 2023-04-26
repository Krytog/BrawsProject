#include "Render.h"
#include "Canvas.h"
#include "Camera.h"
#include <iostream>
#include <unordered_set>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>

const std::string kWindowTitle = "BablKvas";

class RenderImplementation {
public:
    RenderImplementation()
        : window_(sf::RenderWindow(sf::VideoMode(kWindowWidth, kWindowHeight), kWindowTitle)) {
        window_.setMouseCursorVisible(false);
    }

    void AddToRender(const GameObject *object_ptr, const VisibleObject *vis_obj_ptr) {
        visible_objects_[object_ptr] = vis_obj_ptr;
        render_levels_[vis_obj_ptr->GetRenderLevel()].insert(vis_obj_ptr);
    }

    void RemoveFromRender(const GameObject *object_ptr) {
        if (visible_objects_.contains(object_ptr)) {
            const VisibleObject* vis_obj_ptr = visible_objects_.at(object_ptr);
            render_levels_[vis_obj_ptr->GetRenderLevel()].erase(vis_obj_ptr);
            visible_objects_.erase(object_ptr);
        }
    }

    void RenderAll() {
        if (!window_.isOpen()) {
            exit(0);
        }

        sf::Event event;
        Canvas canvas(&window_);

        canvas.SetCenter(camera_.GetPosition());

        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window_.close();
        }
        window_.clear();
        AddAllToCanvas(&canvas);
        window_.display();
    }

    void SetCameraOn(const GameObject *object) {
        camera_.SetCameraOn(object);
    };

    Position GetCameraPosition() const {
        return camera_.GetPosition();
    };

    sf::Window *GetWindowPointer() {
        return &window_;
    }

private:
    std::unordered_map<const GameObject *, const VisibleObject *> visible_objects_;
    std::map<size_t, std::unordered_set<const VisibleObject*>> render_levels_;
    sf::RenderWindow window_;
    Camera camera_ = Camera::GetInstance();

    void AddAllToCanvas(Canvas *canvas) {
        for (const auto &[level, vis_objects]: render_levels_) {
            for (const auto &vis_obj: vis_objects) {
                vis_obj->RenderIt(canvas);
            }
        }
    }
};

Render::Render() : impl_(std::make_unique<RenderImplementation>()){};

void Render::AddToRender(const GameObject *object_ptr, const VisibleObject *vis_obj_ptr) {
    impl_->AddToRender(object_ptr, vis_obj_ptr);
}
void Render::RemoveFromRender(const GameObject *vis_object_ptr) {
    impl_->RemoveFromRender(vis_object_ptr);
}

void Render::RenderAll() {
    impl_->RenderAll();
}

void Render::SetCameraOn(const GameObject *object) {
    impl_->SetCameraOn(object);
}

Position Render::GetCameraPosition() const {
    return impl_->GetCameraPosition();
}

Render::~Render() = default;

sf::Window *Render::GetWindowPointer() const {
    return impl_->GetWindowPointer();
}

Render& Render::GetInstance() {
    static Render render;
    return render;
}