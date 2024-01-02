#pragma once
#include <glm/glm.hpp>

struct Params
{
    int numberOfBoids;
    int numberOfSegments;
    float radius;
    float start_x;
    float start_y;
    float perception;
    float borderForce;
    float minSpeed;
    float maxSpeed;
    float maxForce;
    float alignmentForce;
    float cohesionForce;
    float separationForce;

    int width;
    int height;
    int cellSize;
    int widthCount;
    int heightCount;
    int cellCount;

    float cursorX;
    float cursorY;
};

struct BoidData
{
    glm::vec2* device_positions;
    glm::vec2* device_velocities;
    glm::vec2* device_newPositions;
    glm::vec2* device_newVelocities;
    glm::vec2* device_accelerations;
    glm::vec2* device_translations;

    int* device_gridCellIndex;
    int* device_gridCellStart;
    int* device_gridCellEnd;
    int* boidSequence;

    glm::vec2* device_positionsSorted;
    glm::vec2* device_velocitiesSorted;

    int* colorIndex;
    int* colorSorted;

    Params params;
};