#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>
#include "utils/global.h"
#include "utils/BoidSimulationStructures.h"

class Boid
{
public:
    int index;
    glm::vec2 position;
    glm::vec2 acceleration;
    glm::vec2 velocity;
    glm::vec2 translation;
    int colorIndex;

    Params& params;

    Boid(Params& params) : params(params)
    {
        index = 0;
        position = { randomFloat(params.width / 4, 3 * params.width / 4), randomFloat(params.height / 4, 3 * params.height / 4) };

        acceleration = glm::vec2(0, 0);

        velocity = {randomFloat(-3, 3), randomFloat(-3, 3)};
        if (length(velocity) == 0)
            velocity = glm::vec2(1, 1);

        velocity = setMagnitude(velocity, params.maxSpeed);

        colorIndex = (int)randomFloat(0, 5);
    }

    bool checkSpecies(const Boid& neigh)
    {

        if (colorIndex == neigh.colorIndex)
            return true;
        return false;
    }
    
    void antiBorderCollisionThrough()
    {
        if (position.x < params.radius)
            position.x = params.width - params.radius;

        if (position.x + params.radius > params.width)
            position.x = params.radius;

        if (position.y < params.radius)
            position.y = params.height - params.radius;

        if (position.y + params.radius > params.height)
            position.y = params.radius;
    }

    glm::vec2 alignmentForce(const std::vector<Boid> &neighs)
    {
        glm::vec2 target = glm::vec2(0, 0);
        int neighsCount = 0;
        for (auto& boid : neighs)
        {
            if (!checkSpecies(boid))
                continue;
            target += boid.velocity;
            neighsCount++;
        }

        if (neighsCount > 0)
            target /= neighsCount;
        else
            target = velocity;

        return steeringForce(target);
    }

    glm::vec2 cohesionForce(const std::vector<Boid> &neighs)
    {
        glm::vec2 target = glm::vec2(0, 0);
        int neighsCount = 0;
        for (auto& boid : neighs)
        {
            if (!checkSpecies(boid))
                continue;
            target += boid.position;
            neighsCount++;
        }

        if (neighsCount > 0)
            target /= neighsCount;
        else
            target = position;

        return steeringForce(target - position); 
    }

    glm::vec2 separationForce(const std::vector<Boid> &neighs)
    {
        glm::vec2 target = glm::vec2(0, 0);
        for (auto &boid : neighs)
        {
            glm::vec2 offset = position - boid.position;
            if(length(offset) == 0)
                continue;

            glm::vec2 value = offset * (1 / length(offset));
            target += value;
        }    

        if (neighs.size() > 0)
            target /= neighs.size();
        else
            return glm::vec2(0,0);

        return steeringForce(target); 
    }

    glm::vec2 steeringForce(glm::vec2 target)
    {
        glm::vec2 targetOffset = target;

        glm::vec2 desiredVelocity = {0, 0};
        glm::vec2 steeringForce = {0, 0};

        if (length(targetOffset) > 0)
        {
            desiredVelocity = normalize(targetOffset) * params.maxSpeed;
            glm::vec2 steeringVelocity = desiredVelocity - velocity;
            steeringForce = limit(steeringVelocity, params.maxForce);
        }

        return steeringForce;
    }

    void applyForces(const std::vector<Boid> &neighs)
    {
        acceleration *= 0;
        acceleration += params.alignmentForce * alignmentForce(neighs);
        acceleration += params.cohesionForce * cohesionForce(neighs);
        acceleration += params.separationForce * separationForce(neighs);
    }

    void computeNextFrame(int index, const std::vector<Boid> &neighs)
    {
        this->index = index;

        applyForces(neighs);
        velocity += acceleration;

        position += velocity;

        antiBorderCollisionThrough();

        translation = position - glm::vec2(params.start_x, params.start_y);;

        static float mx = 0;
        if(length(velocity) > mx)
        {
            mx = length(velocity);
        }
    }
};