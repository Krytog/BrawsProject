#pragma once

#include "GameObject.h"
#include "Position.h"
#include "VisibleObject.h"

#include <memory>
#include <string_view>

class RenderImplementation;

class Render {
public:
  Render();

  void AddToRender(const GameObject *object_ptr,
                   const VisibleObject *vis_obj_ptr);
  void RemoveFromRender(const GameObject *vis_object_ptr);
  void RenderAll();

  ~Render();

private:
  std::unique_ptr<RenderImplementation> impl_;
};
