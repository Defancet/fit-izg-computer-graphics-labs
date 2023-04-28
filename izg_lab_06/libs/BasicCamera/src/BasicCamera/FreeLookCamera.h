#pragma once

#include <glm/glm.hpp>

#include <BasicCamera/Camera.h>
#include <BasicCamera/Fwd.h>
#include <BasicCamera/basiccamera_export.h>

class basicCamera::FreeLookCamera : public CameraTransform {
 public:
  BASICCAMERA_EXPORT FreeLookCamera();
  BASICCAMERA_EXPORT virtual ~FreeLookCamera();
  BASICCAMERA_EXPORT virtual glm::mat4 getView();
  BASICCAMERA_EXPORT glm::mat4         getRotation();
  BASICCAMERA_EXPORT void              up(float dy);
  BASICCAMERA_EXPORT void              down(float dy);
  BASICCAMERA_EXPORT void              left(float dx);
  BASICCAMERA_EXPORT void              right(float dx);
  BASICCAMERA_EXPORT void              forward(float dz);
  BASICCAMERA_EXPORT void              back(float dz);
  BASICCAMERA_EXPORT void              move(size_t axis, float d);
  BASICCAMERA_EXPORT void              setPosition(glm::vec3 const& p);
  BASICCAMERA_EXPORT void              addPosition(glm::vec3 const& delta);
  BASICCAMERA_EXPORT glm::vec3         getPosition() const;
  BASICCAMERA_EXPORT float             getXAngle() const;
  BASICCAMERA_EXPORT float             getYAngle() const;
  BASICCAMERA_EXPORT float             getZAngle() const;
  BASICCAMERA_EXPORT float             getAngle(size_t axis) const;
  BASICCAMERA_EXPORT void              setXAngle(float value);
  BASICCAMERA_EXPORT void              setYAngle(float value);
  BASICCAMERA_EXPORT void              setZAngle(float value);
  BASICCAMERA_EXPORT void              setAngle(size_t axis, float value);
  BASICCAMERA_EXPORT void              addXAngle(float delta);
  BASICCAMERA_EXPORT void              addYAngle(float delta);
  BASICCAMERA_EXPORT void              addZAngle(float delta);
  BASICCAMERA_EXPORT void              addAngle(size_t axis, float delta);
  BASICCAMERA_EXPORT void setRotation(glm::vec3 const& viewVector,glm::vec3 const& upVector);

 protected:
  glm::vec3 _angles            = glm::vec3(0.f);
  bool      _recomputeView     = true;
  bool      _recomputeRotation = true;
  glm::vec3 _position          = glm::vec3(0.f);
  glm::mat4 _view              = glm::mat4(1.f);
  glm::mat4 _rotation          = glm::mat4(1.f);
  void      _computeView();
  void      _computeRotation();
};
