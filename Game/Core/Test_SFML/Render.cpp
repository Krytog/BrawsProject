#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <unordered_set>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>

#include "Render.h"
#include "../VisibleObject.h"
#include "Window.h"

// may be useful 
size_t FramerateLimit = 60;

class RenderImplementation {
public:
    RenderImplementation(): window_(GetWindow()) {
        window_.setFramerateLimit(FramerateLimit);
    }

    void AddToRender(std::shared_ptr<VisibleObject> object_ptr) {
        visible_objects_.insert(object_ptr);
    }

    void RemoveFromRender(std::unique_ptr<VisibleObject> object_ptr) {
        if (auto it = std::find(visible_objects_.begin(), visible_objects_.end(), object_ptr); it == visible_objects_.end()) {
            throw std::runtime_error("No such object to remove");
        } else {
            visible_objects_.erase(it);
        }
    }

    void RenderAll() {
        if (!window_.isOpen()) {
            // do smth
            return;
        }
        sf::Event event;
        while (window_.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window_.close();
        }
        window_.clear();
        for (const auto& ptr: visible_objects_) {
            ptr->RenderIt();
        }
        window_.display();
    }


private:
    std::unordered_set<std::shared_ptr<VisibleObject>> visible_objects_;
    sf::RenderWindow& window_;
};

Render::Render(): impl_(std::make_unique<RenderImplementation>()) {};
