//#include <iostream>
//#include <cstdlib>
//#include <ctime>
//#include <chrono>
//#include <thread>
//#include <vector>
//
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"
//#include "nlohman/json.hpp"
//
//#include "utils/BoidSimulationStructures.h"
//#include "utils/global.h"
//#include "Graphics/BoidsRenderer.h"
//
//#include "GPU/cuda_functions.h"
//#include "CPU/Boid.h"
//#include "CPU/Flock.h"
//
//#include "UI/UI.h"
//#include "Graphics/Shader.h"
//
//
//void displayFPS();
//void loadParamsFromFile(Params& params);
//
//bool simulation = true;
//
//int main()
//{
//    Params params;
//    loadParamsFromFile(params);
//
//    if (!glfwInit())
//        return -1;
//
//    const char* glsl_version = getGLSLVersion();
//
//    GLFWwindow* window = glfwCreateWindow(params.width, params.height, "Boids simulation", NULL, NULL);
//    if (window == nullptr)
//        return 1;
//    glfwMakeContextCurrent(window);
//    glfwSwapInterval(1);
//
//    if (glewInit() != GLEW_OK)
//        return  -1;
//
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
//    ImGui::StyleColorsDark();
//
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init(glsl_version);
//
//    Shader shader("res/10.1.instancing.vs", "res/10.1.instancing.fs");
//    shader.use();
//    shader.setVec3("u_Color", glm::vec3(0.0f, 1.0f, 1.0f));
//
//    glm::mat4 projection = glm::ortho(0.0f, (float)params.width, 0.0f, (float)params.height);
//    glm::mat4 model = glm::mat4(1.0f);
//    glm::mat4 mvp = projection * model;
//    shader.setMat4("u_mvp", mvp);
//
//    int boidsCount = params.numberOfBoids;
//
//    Flock flock(boidsCount, params);
//    BoidsRenderer boidsRenderer(boidsCount, flock.translations, flock.colorIndexes, params);
//
//    while (!glfwWindowShouldClose(window))
//    {
//        glfwPollEvents();
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        simulation = toggleSwitch("simulation", simulation);
//        ImGui::SliderFloat("Alignment", &(params.alignmentForce), 0.0f, 2.0f);
//        ImGui::SliderFloat("Cohesion", &(params.cohesionForce), 0.0f, 2.0f);
//        ImGui::SliderFloat("Separation", &(params.separationForce), 0.0f, 2.0f);
//
//        double cursorX, cursorY;
//        glfwGetCursorPos(window, &cursorX, &cursorY);
//        params.cursorX = cursorX;
//        params.cursorY = params.height - cursorY;
//
//        if (simulation)
//        {
//            flock.computeNextFrame(params);
//        }
//
//        boidsRenderer.clear();
//
//        boidsRenderer.update();
//
//        boidsRenderer.draw();
//
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//
//        displayFPS();
//    }
//
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwTerminate();
//    return 0;
//}
//
//auto lastTime = std::chrono::high_resolution_clock::now();
//int frameCount = 0;
//
//void displayFPS() 
//{
//    auto currentTime = std::chrono::high_resolution_clock::now();
//    double timeInterval = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - lastTime).count();
//    frameCount++;
//
//    if (timeInterval >= 2.0) {
//        std::cout << "FPS: " << frameCount << std::endl;
//        frameCount = 0;
//        lastTime = currentTime;
//    }
//}
//
//void loadParamsFromFile(Params& params) 
//{
//    std::fstream file("res/parameters.json");
//    if (!file.is_open())
//    {
//        std::cerr << "Error opening file" << std::endl;
//        exit(1);
//    }
//
//    nlohmann::json j;
//    file >> j;
//
//    params.numberOfBoids = j["numberOfBoids"].get<int>();
//    params.width = j["width"].get<float>();
//    params.height = j["height"].get<float>();
//    params.numberOfSegments = j["numberOfSegments"].get<int>();
//    params.radius = j["radius"].get<float>();
//    params.start_x = j["start_x"].get<float>();
//    params.start_y = j["start_y"].get<float>();
//    params.perception = j["perception"].get<float>();
//    params.borderForce = j["borderForce"].get<float>();
//    params.minSpeed = j["minSpeed"].get<float>();
//    params.maxSpeed = j["maxSpeed"].get<float>();
//    params.maxForce = j["maxForce"].get<float>();
//    params.alignmentForce = j["alignmentForce"].get<float>();
//    params.cohesionForce = j["cohesionForce"].get<float>();
//    params.separationForce = j["separationForce"].get<float>();
//
//    params.cellSize = params.perception;
//    params.widthCount = (params.width + params.cellSize - 1) / params.cellSize;
//    params.heightCount = (params.height + params.cellSize - 1) / params.cellSize;
//    params.cellCount = params.widthCount * params.heightCount;
//}