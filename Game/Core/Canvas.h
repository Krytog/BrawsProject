#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>

#include "Position.h"

#include <memory>
#include <string_view>

const int32_t kWindowWidth = 1200;
const int32_t kWindowHeight = 760;

const int32_t kCanvasWidth = 2400;
const int32_t kCanvasHeight = 1520;

class Canvas {
public:
    class Image {
    public:
        Image(const Position *pos, const double &width, const double &height);

        // Expensive operation, extra copying, but so far the only way
        void Resize(uint32_t width, uint32_t height);
        void LoadFromFile(std::string_view path_to_file);
        void Clear();

        ~Image();

    private:
        std::unique_ptr<sf::Image> image_;
        const Position *pos_;  // Тот же Position, что и в Sprite

        double width_;
        double height_;

        friend class Canvas;

        void ResizeImage(const sf::Image &originalImage, sf::Image &resizedImage);
    };

    Canvas(const double &width, const double &height);

    void Draw(const Image *image);
    void SetCenter(Position global_center);
    bool OnCanvas(double x, double y);
    void Clear();

    ~Canvas();

private:
    std::unique_ptr<sf::Image> canvas_;
    Position global_center_;  // Глобальные координаты камеры
    friend class RenderImplementation;
};