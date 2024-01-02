#pragma once
#include <random>
#include <iostream>
#include <stdexcept>
#include <glm/glm.hpp>
#include "utils/BoidSimulationStructures.h"

static std::random_device rd;
static std::mt19937 eng(rd());

inline float randomFloat(float l, float r)
{
    std::uniform_real_distribution<> distr(l, r);
    return distr(eng);
}

inline glm::vec2 limit(glm::vec2 v, float l)
{
    if(glm::length(v) > l)
    {
        if(length(v) > 0)
            v = glm::normalize(v) * l;
    }
    return v;
}

inline glm::vec2 setMagnitude(glm::vec2 v, float l)
{
    if(length(v) == 0)
    {
        throw std::runtime_error("sm_length(v) == 0");
    }
    v = glm::normalize(v) * l;
    return v;
}