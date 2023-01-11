#include "Render.h"
#include "Canvas.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>

// may be usefull
const size_t kFramerateLimit = 60;

const std::string kWindowTitle = "BablKvas";

class RenderImplementation {
public:
  RenderImplementation()
      : window_(sf::RenderWindow(sf::VideoMode(kWindowWidth, kWindowHeight),
                                 kWindowTitle)) {
    // window_.setFramerateLimit(FramerateLimit);
  }

  void AddToRender(const GameObject *object_ptr,
                   const VisibleObject *vis_obj_ptr) {
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
      // надо как-то прервать весь игровой цикл как-то видимо, но это потом
      // подумаем
      exit(0);
    }

    sf::Event event;
    Canvas canvas(kWindowWidth + kFrameWidth, kWindowHeight + kFrameHeight);

    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window_.close();
    }
    window_.clear();
    for (const auto &[obj_ptr, vis_obj_ptr] : visible_objects_) {
      vis_obj_ptr->RenderIt(&canvas);
    }
    sf::Texture canvas_texture;
    canvas_texture.loadFromImage(*canvas.canvas_);
    sf::Sprite drawable_canvas;
    drawable_canvas.setTexture(canvas_texture);
    drawable_canvas.setPosition(-kFrameWidth, -kFrameHeight);

    window_.draw(drawable_canvas);
    window_.display();
  }

private:
  std::unordered_map<const GameObject *, const VisibleObject *>
      visible_objects_;
  sf::RenderWindow window_;
};

Render::Render() : impl_(std::make_unique<RenderImplementation>()){};

void Render::AddToRender(const GameObject *object_ptr,
                         const VisibleObject *vis_obj_ptr) {
  impl_->AddToRender(object_ptr, vis_obj_ptr);
}
void Render::RemoveFromRender(const GameObject *vis_object_ptr) {
  impl_->RemoveFromRender(vis_object_ptr);
}

void Render::RenderAll() { impl_->RenderAll(); }

Render::~Render() = default;