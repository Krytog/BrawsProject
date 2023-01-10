#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>

#include "Position.h"

#include <memory>
#include <string_view>

class Canvas {
public:

    class Image {
    public:
        Image(const Position* pos, const double& width, const double& height);

        void LoadFromFile(std::string_view path_to_file);
        void Clear();

        ~Image();
    private:
        std::unique_ptr<sf::Image> image_;
        const Position* pos_;

        friend class Canvas;
    };

    Canvas(const double& width, const double& height);

    void Draw(const Image* image);
    void Clear();

    ~Canvas();
private:
    std::unique_ptr<sf::Image> canvas_;

    friend class RenderImplementation;
};