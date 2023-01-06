#pragma once

#include <memory>

class RenderImplementation;

// some meager api for testing needs
class Render {
public:
    Render();

    void AddToRender();
    void RenderAll();
    void RemoveFromRender();

private:
    std::unique_ptr<RenderImplementation> impl_;
};