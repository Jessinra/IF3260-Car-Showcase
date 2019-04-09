cmake_minimum_required(VERSION 3.6)

project(suzuki_jimny)

set(CMAKE_CXX_STANDARD 17)

set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)

set(DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(VIEW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/view")
set(MODEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/model")
set(MODEL_DIR_MODULES "")

add_executable(${PROJECT_NAME} main.cpp)
add_executable(asset compileAsset.cpp)

# OpenGL things
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(glfw ${GLFW_LIBRARIES})

set(GLAD_DIR "${DEP_DIR}/glad")
add_library("glad" "${DEP_DIR}/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Other libs
add_library("otherlib" "${LIB_DIR}/glmutils.cpp" "${VIEW_DIR}/Camera.cpp" "${MODEL_DIR}/Model.cpp" "${MODEL_DIR}/Shader.cpp")
target_include_directories("otherlib" PRIVATE "${LIB_DIR}" "${MODEL_DIR}" "${VIEW_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${LIB_DIR}" "${MDOEL_DIR}" "${VIEW_DIR}")
target_link_libraries(${PROJECT_NAME} "otherlib" "${CMAKE_DL_LIBS}")

# other exec
target_link_libraries(asset stdc++fs)