#include <BasicCamera/OrbitCamera.h>

using namespace basicCamera;

OrbitCamera::OrbitCamera(float distance)
    : CameraTransform(),
      _xAngle(0),
      _yAngle(0),
      _distance(distance),
      _recompute(true) {}

OrbitCamera::~OrbitCamera() {}

glm::mat4 OrbitCamera::getView() {
  if (_recompute) {
    _compute();
    _recompute = false;
  }
  return _view;
}

glm::mat4 OrbitCamera::getRotation() {
  if (_recompute) {
    _compute();
    _recompute = false;
  }
  return _rotation;
}

float OrbitCamera::getXAngle() const {
  return _xAngle;
}

float OrbitCamera::getYAngle() const {
  return _yAngle;
}

float OrbitCamera::getDistance() const {
  return _distance;
}

glm::vec3 OrbitCamera::getFocus() const {
  return _position;
}

void OrbitCamera::setXAngle(float value) {
  _xAngle    = glm::clamp(value, -glm::half_pi<float>(), glm::half_pi<float>());
  _recompute = true;
}

void OrbitCamera::setYAngle(float value) {
  _yAngle    = value;
  _recompute = true;
}

void OrbitCamera::setAngles(glm::vec2 const& angles) {
  setXAngle(angles.x);
  setYAngle(angles.y);
}

void OrbitCamera::setDistance(float value) {
  _distance  = value;
  _recompute = true;
}

void OrbitCamera::setFocus(glm::vec3 const& value) {
  _position  = value;
  _recompute = true;
}

void OrbitCamera::addXPosition(float dx) {
  _position += glm::vec3(glm::row(_rotation, 0)) * dx;
  _recompute = true;
}

void OrbitCamera::addYPosition(float dy) {
  _position += glm::vec3(glm::row(_rotation, 1)) * dy;
  _recompute = true;
}

void OrbitCamera::addXYPosition(glm::vec2 const& value) {
  addXPosition(value.x);
  addYPosition(value.y);
}

void OrbitCamera::addXAngle(float value) {
  setXAngle(getXAngle() + value);
}

void OrbitCamera::addYAngle(float value) {
  setYAngle(getYAngle() + value);
}

void OrbitCamera::addAngles(glm::vec2 const& value) {
  addXAngle(value.x);
  addYAngle(value.y);
}

void OrbitCamera::addDistance(float value) {
  setDistance(getDistance() + value);
}

void OrbitCamera::_compute() {
  _rotation = glm::rotate(glm::mat4(1.f), _xAngle, glm::vec3(1.f, 0.f, 0.f)) *
              glm::rotate(glm::mat4(1.f), _yAngle, glm::vec3(0.f, 1.f, 0.f));
  _view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -_distance)) *
          _rotation * glm::translate(glm::mat4(1.f), _position);
}
