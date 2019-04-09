#include "Camera.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 1.0f, 0.0f)),
                                                                           MovementSpeed(SPEED),
                                                                           MouseSensitivity(SENSITIVITY),
                                                                           Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;

    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                                                                              MovementSpeed(SPEED),
                                                                                                              MouseSensitivity(SENSITIVITY),
                                                                                                              Zoom(ZOOM) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::moveOnKeyPress(Camera_Movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        Position += Front * velocity;
    } else if (direction == BACKWARD) {
        Position -= Front * velocity;
    } else if (direction == LEFT) {
        Position -= Right * velocity;
    } else if (direction == RIGHT) {
        Position += Right * velocity;
    }
}

void Camera::moveOnKeyPress(Camera_Movement direction, glm::vec3 target, float deltaTime) {
    glm::vec3 dir = glm::normalize(Position - target);

    float dist = glm::distance(Position, target);
    float velocity = MovementSpeed * deltaTime * 10;

    if (direction == FORWARD) {
        dir = glm::rotateX(dir, glm::radians(velocity));
        Pitch += velocity;

        if (Pitch > 90) {
            Pitch -= 180;
        } else if (Pitch < -90) {
            Pitch += 180;
        }

    } else if (direction == BACKWARD) {
        dir = glm::rotateX(dir, -glm::radians(velocity));
        Pitch -= velocity;

        if (Pitch > 90) {
            Pitch -= 180;
        } else if (Pitch < -90) {
            Pitch += 180;
        }

    } else if (direction == LEFT) {
        dir = glm::rotateY(dir, -glm::radians(velocity));
        Yaw += velocity;

    } else if (direction == RIGHT) {
        dir = glm::rotateY(dir, glm::radians(velocity));
        Yaw -= velocity;
    }

    Position = dir * dist;
    Position += target;

    updateCameraVectors();
}

void Camera::moveOnMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (this->Pitch > 89.0f) {
            this->Pitch = 89.0f;
        }
        if (this->Pitch < -89.0f) {
            this->Pitch = -89.0f;
        }
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::moveOnMouseScroll(float yoffset) {
    if (isZoomAllowed()) {
        this->Zoom -= yoffset;
    }

    // Limit on zoom out
    if (this->Zoom <= this->ZoomMin) {
        this->Zoom = this->ZoomMin;
    }

    // Limit on zoom in
    if (this->Zoom >= this->ZoomMax) {
        this->Zoom = this->ZoomMax;
    }
}

bool Camera::isZoomAllowed(){
    return this->Zoom >= this->ZoomMin && this->Zoom <= this->ZoomMax;
}

void Camera::updateCameraVectors() {
    // Calculate the new Front vector

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    this->Front = glm::normalize(front);
    this->Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up = glm::normalize(glm::cross(Right, Front));
}
