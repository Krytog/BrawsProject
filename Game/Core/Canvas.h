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

class Canvas {
public:
    /* Designed for large images that we do not want to render/load into RAM often */
    class StaticImage {
    public:
        StaticImage() = default;
        void LoadFromFile(std::string_view path_to_file);

        const sf::Image &GetEntireImage() const;
        std::pair<size_t, size_t> GetRealSize() const;
        void SetDefaultImage();

    private:
        sf::Image image_;
    };

    class Image {
    public:
        Image(const Position& pos, const size_t &width, const size_t &height);

        void LoadFromFile(std::string_view path_to_file);
        void LoadFromStaticImage(const StaticImage &static_image, const Position &orig_image_pos,
                                 const Position &rectangle_left_up_corner,
                                 const Position &rectangle_right_down_corner,
                                 const size_t &orig_width, const size_t &orig_height);
        void SetDefaultImage();

        virtual void UpdatePosition(const Position& position);
        virtual void Translate(const Vector2D& vector2D);

        ~Image();

    private:
        sf::Texture image_;
        Position pos_;  /* Coincides with the center of the object, that is, the usual local Position of
the GameObject, where Render will draw it */

        size_t width_;
        size_t height_;

        friend class Canvas;
    };

    Canvas(sf::RenderWindow *window_ptr_);

    void Draw(Image *image);
    void SetCenter(Position global_center);

    ~Canvas();

private:
    sf::RenderWindow *canvas_;
    Position global_center_;  /* Global camera's coordinates */
    friend class RenderImplementation;
};