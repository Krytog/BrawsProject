#include "Render.h"
#include "Canvas.h"
#include "Camera.h"
#include <iostream>

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
    }

    void RemoveFromRender(const GameObject *vis_object_ptr) {
        if (!visible_objects_.contains(vis_object_ptr)) {
            throw std::runtime_error("Such GameObject didn't register as Visible");
        } else {
            visible_objects_.erase(vis_object_ptr);
        }
    }

    void RenderAll() {
        if (!window_.isOpen()) {
            exit(0);
        }
        sf::Event event;
        Canvas canvas(kCanvasWidth, kCanvasHeight);
        canvas.SetCenter(camera_.GetPosition());

        AddAllToCanvas(&canvas);
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window_.close();
        }
        window_.clear();
        DrawCanvasOnWindow(&canvas);
        window_.display();
    }

    void SetCameraOn(const GameObject *object) {
        camera_.SetCameraOn(object);
    };

    Position GetCameraPosition() const {
        return camera_.GetPosition();
    };

    sf::Window* GetWindowPointer() {
        return &window_;
    }

private:
    std::unordered_map<const GameObject *, const VisibleObject *> visible_objects_;
    sf::RenderWindow window_;
    Camera camera_ = Camera::GetInstance();

    void AddAllToCanvas(Canvas *canvas) {
        for (const auto &[obj_ptr, vis_obj_ptr] : visible_objects_) {
            vis_obj_ptr->RenderIt(canvas);
        }
    }

    void DrawCanvasOnWindow(Canvas *canvas) {
        sf::Texture canvas_texture;
        canvas_texture.loadFromImage(*canvas->canvas_);
        sf::Sprite drawable_canvas;
        drawable_canvas.setTexture(canvas_texture);
        drawable_canvas.setPosition(-((kCanvasWidth - kWindowWidth) / 2),
                                    -((kCanvasHeight - kWindowHeight) / 2));
        window_.draw(drawable_canvas);
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