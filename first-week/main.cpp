// #include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct point
{
    int x;
    int y;
} point;

int main(int argc, char *argv[]) {
    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);


    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    point polygonPoints[] = {
        point{.x = 1, .y = 1},
        point{.x = 10, .y = 10},
        point{.x = 20, .y = 20},
        point{.x = 20, .y = 10},
        point{.x = 1, .y = 10}
    };

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_LINES);
        glColor4f(1, 1, 1, 1);
        for (auto polygonPoint : polygonPoints) {
            glVertex2f(polygonPoint.x, polygonPoint.y);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}