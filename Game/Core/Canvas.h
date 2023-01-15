#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>

#include "Position.h"

#include <memory>
#include <string_view>

const size_t kWindowWidth = 1200;
const size_t kWindowHeight = 760;

const float kFrameWidth = 1200;
const float kFrameHeight = 760;

class Canvas {
public:
    class Image {
    public:
        Image(const Position *pos, const double &width, const double &height);

        // Expensive operation, extra copying, but so far the only way
        void Resize(const int width, const int height);
        void LoadFromFile(std::string_view path_to_file);
        void Clear();

        ~Image();

    private:
        std::unique_ptr<sf::Image> image_;
        const Position *pos_;

        double width_;
        double height_;

        friend class Canvas;

        void ResizeImage(const sf::Image &originalImage, sf::Image &resizedImage);
    };

    Canvas(const double &width, const double &height);

    void Draw(const Image *image);
    void Clear();

    ~Canvas();

private:
    std::unique_ptr<sf::Image> canvas_;

    friend class RenderImplementation;
};