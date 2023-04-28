#include <BasicCamera/PerspectiveCamera.h>

using namespace basicCamera;

PerspectiveCamera::PerspectiveCamera(float fovy,
                                     float aspect,
                                     float near,
                                     float far)
    : CameraProjection() {
  _fovy      = fovy;
  _near      = near;
  _far       = far;
  _aspect    = aspect;
  _recompute = true;
}

PerspectiveCamera::~PerspectiveCamera() {}

glm::mat4 PerspectiveCamera::getProjection() {
  if (_recompute) {
    _compute();
    _recompute = false;
  }
  return _projection;
}

float PerspectiveCamera::getNear() const { return _near; }
float PerspectiveCamera::getFar() const { return _far; }
float PerspectiveCamera::getFovy() const { return _fovy; }
float PerspectiveCamera::getAspect() const { return _aspect; }

void PerspectiveCamera::setNear(float value) {
  _near      = value;
  _recompute = true;
}
void PerspectiveCamera::setFar(float value) {
  _far       = value;
  _recompute = true;
}
void PerspectiveCamera::setFovy(float value) {
  _fovy      = value;
  _recompute = true;
}
void PerspectiveCamera::setAspect(float value) {
  _aspect    = value;
  _recompute = true;
}

void PerspectiveCamera::_compute() {
  if (_far == std::numeric_limits<float>::infinity())
    _projection = glm::infinitePerspective(_fovy, _aspect, _near);
  else
    _projection = glm::perspective(_fovy, _aspect, _near, _far);
}
