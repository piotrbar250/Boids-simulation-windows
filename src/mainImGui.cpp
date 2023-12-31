#include <iostream>
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "global.hpp"
#include "Shader.h"
#include "CPU/BoidsRenderer.h"
#include "CPU/Boid.h"
#include "CPU/Flock.h"

#include "GPU/FlockGPU.h"
#include "GPU/FlockGridGPU.h"
#include "GPU/cuda_functions.h"
#include <chrono>
#include <thread>

#include "nlohman/json.hpp"


using namespace std;
using json = nlohmann::json;

void displayFPS();

void loadParamsFromFile(Params& params) {
    fstream file("C:/Users/Piotr Baranowski/source/repos/Boids-simulation-windows/src/parameters.json");
    if (!file.is_open()) 
    {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    nlohmann::json j;
    file >> j;

    params.width = j["width"].get<int>();
    params.height = j["height"].get<int>();
    params.numberOfSegments = j["numberOfSegments"].get<int>();
    params.radius = j["radius"].get<float>();
    params.start_x = j["start_x"].get<float>();
    params.start_y = j["start_y"].get<float>();
    params.perception = j["perception"].get<float>();
    params.borderForce = j["borderForce"].get<float>();
    params.minSpeed = j["minSpeed"].get<float>();
    params.maxSpeed = j["maxSpeed"].get<float>();
    params.maxForce = j["maxForce"].get<float>();
    params.alignmentForce = j["alignmentForce"].get<float>();
    params.cohesionForce = j["cohesionForce"].get<float>();
    params.separationForce = j["separationForce"].get<float>();
    
    params.cellSize = params.perception;
    params.widthCount = (params.width + params.cellSize - 1) / params.cellSize;
    params.heightCount = (params.height + params.cellSize - 1) / params.cellSize;
    params.cellCount = params.widthCount * params.heightCount;
}

int main()
{
    GLFWwindow *window;
    if (!glfwInit())
    {
        return -1;
    }

    window = glfwCreateWindow(screenWidth, screenHeight, "Window!", NULL, NULL);

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);


    if (glewInit() != GLEW_OK)
        return  -1;

//    IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//     ImGui::StyleColorsDark();
//     //ImGui::StyleColorsLight();

//     // Setup Platform/Renderer backends
//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     const char* glsl_version = "#version 150";
//     ImGui_ImplOpenGL3_Init(glsl_version);

//     bool show_demo_window = true;
//     bool show_another_window = false;
//     ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    //   if (show_demo_window)
    //         ImGui::ShowDemoWindow(&show_demo_window);

    //  ImGui::Begin("Hello, world!");    
            //  ImGui::Text("Hello from another window!");

    //Shader shader("src/10.1.instancing.vs", "src/10.1.instancing.fs");
    Shader shader("C:/Users/Piotr Baranowski/source/repos/Boids-simulation-windows/src/10.1.instancing.vs", "C:/Users/Piotr Baranowski/source/repos/Boids-simulation-windows/src/10.1.instancing.fs");
    shader.use();
    shader.setVec3("u_Color", glm::vec3(0.0f, 1.0f, 1.0f));

    glm::mat4 projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight);
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 mvp = projection * model;

    shader.setMat4("u_mvp", mvp);


    int boidsCount = 7000;

    Params params;

    loadParamsFromFile(params);

    FlockGridGPU flockGridGPU(boidsCount, params);
    BoidsRenderer boidsRenderer(boidsCount, flockGridGPU.translations, flockGridGPU.colorIndex); 
    
    while (!glfwWindowShouldClose(window))
    {
        loadParamsFromFile(params);
        glfwPollEvents();
        
        double cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        params.cursorX = cursorX;
        params.cursorY = params.height - cursorY;

        flockGridGPU.computeNextFrame(&(boidsRenderer.cuda_vbo_resource), params);

        boidsRenderer.clear();

        boidsRenderer.draw();

        glfwSwapBuffers(window);

        displayFPS();
    }
    glfwTerminate();
    return 0;
}

auto lastTime = chrono::high_resolution_clock::now();
int frameCount = 0;

void displayFPS()
{
    auto currentTime = chrono::high_resolution_clock::now();
    frameCount++;

    if (chrono::duration_cast<chrono::seconds>(currentTime - lastTime).count() >= 2)
    {
        double fps = frameCount / 2.0;
        cout << "FPS: " << fps << endl;

        lastTime = currentTime;
        frameCount = 0;
    }
}
