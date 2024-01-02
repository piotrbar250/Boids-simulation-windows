#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "utils/global.h"
#include "../GPU/cuda_functions.h"

class FlockGPU
{
public:
    int boidsCount;
    std::vector<glm::vec2> positions;
    std::vector<glm::vec2> accelerations;
    std::vector<glm::vec2> velocities;
    std::vector<glm::vec2> translations;

    std::vector<glm::vec2> newPositions;
    std::vector<glm::vec2> newVelocities;

    glm::vec2* device_positions;
    glm::vec2* device_velocities;
    glm::vec2* device_newPositions;
    glm::vec2* device_newVelocities;
    glm::vec2* device_accelerations;
    glm::vec2* device_translations;

    Params& params;

    FlockGPU(int boidsCount, Params& params) : boidsCount(boidsCount), params(params)
    {
        positions.resize(boidsCount);
        accelerations.resize(boidsCount);
        velocities.resize(boidsCount);
        translations.resize(boidsCount);
        newPositions.resize(boidsCount);
        newVelocities.resize(boidsCount);

        for (int i = 0; i < boidsCount; i++)
        {
            positions[i] = {randomFloat(100, 700), randomFloat(100, 700)};

            accelerations[i] = glm::vec2(0, 0);

            velocities[i] = {randomFloat(-3, 3), randomFloat(-3, 3)};
            if (length(velocities[i]) == 0)
                velocities[i] = glm::vec2(1, 1);

            velocities[i] = setMagnitude(velocities[i], params.maxSpeed);   
        }

        cuda_functions::allocateDataOnGPU(boidsCount, device_positions, device_velocities, device_newPositions, device_newVelocities, device_accelerations, device_translations);
        cuda_functions::sendDataToGPU(boidsCount, device_positions, device_velocities, positions.data(), velocities.data());
    }

    ~FlockGPU()
    {
        cuda_functions::freeDataOnGPU(device_positions, device_velocities, device_newPositions, device_newVelocities, device_accelerations, device_translations);
    }

    void computeNextFrame(void** cuda_vbo_resource)
    {
        glm::vec2* devPtr = cuda_functions::getMappedPointer(cuda_vbo_resource);

        cuda_functions::computeNextFrame(boidsCount, device_positions, device_velocities, device_newPositions, device_newVelocities, device_accelerations, devPtr);
        cuda_functions::swapFrames(boidsCount, device_positions, device_velocities, device_newPositions, device_newVelocities);
        cuda_functions::unmapResource(cuda_vbo_resource);
    }
};