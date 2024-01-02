#pragma once
#include "GLFw/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include <stdio.h>

inline const char* getGLSLVersion()
{
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    return glsl_version;
}

inline bool toggleSwitch(const char* label, bool& isToggled)
{
    ImGui::Text("%s", label);
    ImGui::SameLine();

    ImVec2 p = ImGui::GetCursorScreenPos();
    float height = ImGui::GetFrameHeight();
    float width = height * 2.3f;
    float radius = height * 0.50f;
    float sliderPos = isToggled ? (width - radius) : radius;

    ImGui::InvisibleButton("##toggle", ImVec2(width, height));
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        isToggled = !isToggled;

    ImU32 col_bg = isToggled ? IM_COL32(30, 144, 255, 255) : IM_COL32(150, 150, 150, 255);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + sliderPos, p.y + radius), radius - 1.0f, IM_COL32(255, 255, 255, 255));

    return isToggled;
}


inline bool toggleSwitchSlider(const char *label, bool &isToggled)
{
    ImGui::Text("%s", label);
    ImGui::SameLine();

    ImVec2 p = ImGui::GetCursorScreenPos();
    float height = ImGui::GetFrameHeight();
    float width = height * 2.3f;
    float radius = height * 0.50f;
    float sliderPos = isToggled ? (width - radius) : radius;

    ImGui::InvisibleButton("##toggle", ImVec2(width, height));
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        sliderPos = ImGui::GetIO().MousePos.x - p.x - radius;
        sliderPos = (sliderPos < radius) ? radius : (sliderPos > width - radius) ? width - radius : sliderPos;
    }

    isToggled = sliderPos > width * 0.5f;

    ImU32 col_bg = isToggled ? IM_COL32(30, 144, 255, 255) : IM_COL32(150, 150, 150, 255);
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + sliderPos, p.y + radius), radius - 1.0f, IM_COL32(255, 255, 255, 255));

    return isToggled;
}