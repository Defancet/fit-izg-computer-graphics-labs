#include <BasicCamera/FreeLookCamera.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

using namespace basicCamera;

FreeLookCamera::FreeLookCamera() : CameraTransform() {}

FreeLookCamera::~FreeLookCamera() {}

glm::mat4 FreeLookCamera::getView() {
  if (_recomputeView) _computeView();
  return _view;
}

glm::mat4 FreeLookCamera::getRotation() {
  if (_recomputeRotation) _computeRotation();
  return _rotation;
}

void FreeLookCamera::up(float d) { move(1, d); }

void FreeLookCamera::down(float d) { move(1, -d); }

void FreeLookCamera::left(float d) { move(0, -d); }

void FreeLookCamera::right(float d) { move(0, d); }

void FreeLookCamera::forward(float d) { move(2, -d); }

void FreeLookCamera::back(float d) { move(2, d); }

void FreeLookCamera::move(size_t axis, float d) {
  assert(axis < 3);
  _position += d * glm::vec3(glm::row(_rotation, (glm::length_t)axis));
  _recomputeView = true;
}

void FreeLookCamera::setPosition(glm::vec3 const& p) {
  _position      = p;
  _recomputeView = true;
}

void FreeLookCamera::addPosition(glm::vec3 const& delta) {
  _position += delta;
  _recomputeView = true;
}

glm::vec3 FreeLookCamera::getPosition() const { return _position; }

float FreeLookCamera::getXAngle() const { return getAngle(0); }

float FreeLookCamera::getYAngle() const { return getAngle(1); }

float FreeLookCamera::getZAngle() const { return getAngle(2); }

float FreeLookCamera::getAngle(size_t axis) const {
  assert(axis < 3);
  return _angles[(uint32_t)axis];
}

void FreeLookCamera::setXAngle(float value) { setAngle(0, value); }

void FreeLookCamera::setYAngle(float value) { setAngle(1, value); }

void FreeLookCamera::setZAngle(float value) { setAngle(2, value); }

void FreeLookCamera::setAngle(size_t axis, float value) {
  assert(axis < 3);
  if (axis == 0)
    _angles[(uint32_t)axis] =
        glm::clamp(value, -glm::half_pi<float>(), glm::half_pi<float>());
  else
    _angles[(uint32_t)axis] = value;
  _recomputeRotation = true;
  _recomputeView     = true;
}

void FreeLookCamera::addXAngle(float delta) {
  setAngle(0, getAngle(0) + delta);
}

void FreeLookCamera::addYAngle(float delta) {
  setAngle(1, getAngle(1) + delta);
}

void FreeLookCamera::addZAngle(float delta) {
  setAngle(2, getAngle(2) + delta);
}

void FreeLookCamera::addAngle(size_t axis, float delta) {
  assert(axis < 3);
  setAngle(axis, getAngle(axis) + delta);
}

void FreeLookCamera::setRotation(glm::vec3 const& viewVector,
                                 glm::vec3 const& upVector) {
  glm::vec3 xx       = glm::normalize(glm::cross(viewVector, upVector));
  glm::vec3 yy       = glm::normalize(glm::cross(xx, viewVector));
  glm::vec3 zz       = glm::normalize(glm::cross(xx, yy));
  _rotation[0]       = glm::vec4(xx, 0.f);
  _rotation[1]       = glm::vec4(yy, 0.f);
  _rotation[2]       = glm::vec4(zz, 0.f);
  _rotation[3]       = glm::vec4(0.f, 0.f, 0.f, 1.f);
  _rotation          = glm::transpose(_rotation);
  _recomputeRotation = false;
  _recomputeView     = true;
}

void FreeLookCamera::_computeRotation() {
  _rotation          = glm::eulerAngleXYZ(_angles.x, _angles.y, _angles.z);
  _recomputeRotation = false;
}

void FreeLookCamera::_computeView() {
  if (_recomputeRotation) _computeRotation();
  _view          = _rotation * glm::translate(glm::mat4(1.f), -_position);
  _recomputeView = false;
}
