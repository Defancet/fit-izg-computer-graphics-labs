#pragma once

#include <BasicCamera/Camera.h>
#include <BasicCamera/Fwd.h>
#include <BasicCamera/basiccamera_export.h>

class basicCamera::PerspectiveCamera
    : public CameraProjection {
 public:
  BASICCAMERA_EXPORT PerspectiveCamera(float fovy   = glm::radians(90.f),
                    float aspect = 1024.f / 768.f,
                    float near   = 0.1f,
                    float far    = 1000.f);
  BASICCAMERA_EXPORT virtual ~PerspectiveCamera();
  BASICCAMERA_EXPORT virtual glm::mat4 getProjection();
  BASICCAMERA_EXPORT float             getNear() const;
  BASICCAMERA_EXPORT float             getFar() const;
  BASICCAMERA_EXPORT float             getFovy() const;
  BASICCAMERA_EXPORT float             getAspect() const;
  BASICCAMERA_EXPORT void              setNear(float value);
  BASICCAMERA_EXPORT void              setFar(float value);
  BASICCAMERA_EXPORT void              setFovy(float value);
  BASICCAMERA_EXPORT void              setAspect(float value);

 protected:
  float     _near = 1.f;
  float     _far = 100.f; 
  float     _fovy = glm::half_pi<float>();
  float     _aspect = 1.f;
  glm::mat4 _projection = glm::mat4(1.f);
  bool      _recompute = true;
  void      _compute();
};
