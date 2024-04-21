#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BasicCamera/Camera.h>
#include <BasicCamera/Fwd.h>
#include <BasicCamera/basiccamera_export.h>

class basicCamera::OrbitCamera : public CameraTransform {
 public:
  BASICCAMERA_EXPORT OrbitCamera(float distance = 1);
  BASICCAMERA_EXPORT virtual ~OrbitCamera();
  BASICCAMERA_EXPORT virtual glm::mat4 getView();
  BASICCAMERA_EXPORT glm::mat4         getRotation();
  BASICCAMERA_EXPORT float             getXAngle() const;
  BASICCAMERA_EXPORT float             getYAngle() const;
  BASICCAMERA_EXPORT float             getDistance() const;
  BASICCAMERA_EXPORT glm::vec3         getFocus() const;
  BASICCAMERA_EXPORT void              setXAngle(float value);
  BASICCAMERA_EXPORT void              setYAngle(float value);
  BASICCAMERA_EXPORT void              setAngles(glm::vec2 const& angles);
  BASICCAMERA_EXPORT void              setDistance(float value);
  BASICCAMERA_EXPORT void              setFocus(glm::vec3 const& value);
  BASICCAMERA_EXPORT void              addXPosition(float dx);
  BASICCAMERA_EXPORT void              addYPosition(float dy);
  BASICCAMERA_EXPORT void              addXYPosition(glm::vec2 const& value);
  BASICCAMERA_EXPORT void              addXAngle(float value);
  BASICCAMERA_EXPORT void              addYAngle(float value);
  BASICCAMERA_EXPORT void              addAngles(glm::vec2 const& value);
  BASICCAMERA_EXPORT void              addDistance(float value);

 protected:
  float     _xAngle    = 0.f;
  float     _yAngle    = 0.f;
  float     _distance  = 0.f;
  bool      _recompute = true;
  glm::vec3 _position  = glm::vec3(0.f);
  glm::mat4 _view      = glm::mat4(1.f);
  glm::mat4 _rotation  = glm::mat4(1.f);
  void      _compute();
};
