#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace basicCamera {
class CameraProjection {
 public:
  virtual ~CameraProjection() {}
  virtual glm::mat4 getProjection() = 0;
};

class CameraTransform {
 public:
  virtual ~CameraTransform() {}
  virtual glm::mat4 getView() = 0;
};
}  // namespace basicCamera
