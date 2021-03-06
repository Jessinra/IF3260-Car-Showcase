#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../modules/glad/glad.h"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float KEYBOARD_SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera {
   public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 Target;

    // Euler Angles
    float Yaw;
    float Pitch;

    float distance = 10.0f;
    float prevRotX = 0.0f;
    float prevRotZ = 0.0f;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    
    glm::mat4 GetViewMatrix();
    
    /* Move Camera */
    void moveOnKeyPress(Camera_Movement direction, float deltaTime);
    void moveOnKeyPress(Camera_Movement direction, glm::vec3 target, float deltaTime);
    void moveOnMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void moveOnMouseScroll(float yoffset);

   private:
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    
    float Zoom;
    float ZoomMax = 45.0F;
    float ZoomMin = 1.0F;

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    bool isZoomAllowed();
};

#endif
