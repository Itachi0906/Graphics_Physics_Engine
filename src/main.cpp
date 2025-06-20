// File: main.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "shader.h"
#include "Grid.h"
#include "Cube.h"
#include "ParticleWorld.h"
#include <vector>
#include <chrono>
#include <thread>

// Use std namespace for convenience
using namespace std;
using namespace std::chrono;

// Fixed timestep in seconds
const float fixedTimeStep = 0.01f;

float getDeltaTime(high_resolution_clock::time_point& lastTime) {
    high_resolution_clock::time_point currentTime = high_resolution_clock::now();
    duration<float> delta = currentTime - lastTime;
    lastTime = currentTime;
    return delta.count(); // returns seconds as float
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool mouseLocked = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!mouseLocked) return;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}


void processInput(GLFWwindow* window) {

    static bool togglePressed = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!togglePressed) {
            mouseLocked = !mouseLocked;
            glfwSetInputMode(window, GLFW_CURSOR, mouseLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            firstMouse = true;
            togglePressed = true;
        }
    }
    else {
        togglePressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.0f * 0.016f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Physics Visualizer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("Shaders/shader.vert", "Shaders/shader.frag");
    Shader gridShader("Shaders/gridShader.vert", "Shaders/gridShader.frag");

    unsigned int VBO, VAO;
    Grics::Grid grid(20);
    std::vector<Grics::Cube> cubes;
    cubes.emplace_back(1.0f, Grics::Vector3(0.0f, 0.5f, 0.0f));
    cubes.emplace_back(1.0f, Grics::Vector3(5.0f, 0.5f, 0.0f));

    Grics::ParticleWorld particleWorld(1,1);
    Grics::Particle p1, p2;
    p1.setPosition(Grics::Vector3(0.0f, 0.5f, 0.0f));
    p2.setPosition(Grics::Vector3(2.0f, 0.5f, 0.0f));
    p1.setDamping(0.99);
    p2.setDamping(0.99);
    p1.setMass(1.0f);
    p2.setMass(1.0f);

    particleWorld.getContactGenerators().push_back(new Grics::ParticleCubeContactGenerator(&p1, &p2,1.0f));

    particleWorld.getParticles().push_back(&p1);
    particleWorld.getParticles().push_back(&p2);

    cubes[0].setPosition(p1.getPosition());
    cubes[1].setPosition(p2.getPosition());

    p1.setVelocity(Grics::Vector3(2.0f, 0.0f, 0.0f));
    p2.setVelocity(Grics::Vector3(0.0f, 0.0f, 0.0f));
    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330"); // or the GLSL version you're using
    

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    
    float accumulator = 0.0f;
    high_resolution_clock::time_point lastTime = high_resolution_clock::now();


    while (!glfwWindowShouldClose(window)) {

        float deltaTime = getDeltaTime(lastTime);
        accumulator += deltaTime;

        while(accumulator >= fixedTimeStep)
        {
            processInput(window);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.use();


            //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

            shader.setMat4("view", view);
            shader.setMat4("projection", projection);

            for (auto& cube : cubes) {
                //cube.setDamping(0.995);
                ////cube.setVelocity(Grics::Vector3(2.0f,0.0f,0.0f));
                //cube.update(0.001f);
                cube.draw(shader);
            }


            particleWorld.startFrame();

            particleWorld.runPhysics(fixedTimeStep);

            cubes[0].setPosition(p1.getPosition());
            cubes[1].setPosition(p2.getPosition());

            gridShader.use();
            gridShader.setMat4("view", view);
            gridShader.setMat4("projection", projection);

            grid.drawGrid();

            glfwSwapBuffers(window);
            glfwPollEvents();

            accumulator -= fixedTimeStep;
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
