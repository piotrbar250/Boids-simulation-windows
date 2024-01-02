#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "utils/global.h"
#include "Boid.h"

class Flock
{
public:
	int boidsCount;
	std::vector<Boid> boids;
	std::vector<glm::vec2> translations;
	std::vector<int> colorIndexes;
	Params& params;

	Flock(int boidsCount, Params& params) : boidsCount(boidsCount), params(params)
	{
		for (int i = 0; i < boidsCount; i++)
		{
			boids.emplace_back(params);
			colorIndexes.push_back(boids.back().colorIndex);
		}
		translations.resize(boidsCount);

	}

	void computeNextFrame(Params& params)
	{
		params = params;
		for (int i = 0; i < boidsCount; i++)
		{
			Boid& boid = boids[i];
			std::vector<Boid> neighs = getNeighs(i);
			boid.computeNextFrame(i, neighs);
			translations[i] = boid.translation;
		}
	}

	std::vector<Boid> getNeighs(int index)
	{
		std::vector<Boid> neighs;

		for (int i = 0; i < boidsCount; i++)
		{
			if (i == index)
				continue;
			if (fabs(glm::length(boids[i].position - boids[index].position)) <= params.perception)
				neighs.push_back(boids[i]);
		}
		return neighs;
	}
};