#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "utils/global.h"
#include "cuda_functions.h"
#include <chrono>
#include <thread>

class FlockGridGPU
{
public:
	int boidsCount;
	std::vector<glm::vec2> positions;
	std::vector<glm::vec2> accelerations;
	std::vector<glm::vec2> velocities;
	std::vector<glm::vec2> translations;


	glm::vec2* device_positions;
	glm::vec2* device_velocities;
	glm::vec2* device_newPositions;
	glm::vec2* device_newVelocities;
	glm::vec2* device_accelerations;
	glm::vec2* device_translations;

	int* device_gridCellIndex;
	int* device_gridCellStart;
	int* device_gridCellEnd;
	int* device_boidSequence;

	glm::vec2* device_positionsSorted;
	glm::vec2* device_velocitiesSorted;

	std::vector<int> colorIndexes;

	int* device_colorIndexes;
	int* device_colorSorted;

	BoidData boidData;

	FlockGridGPU(int boidsCount, Params params) : boidsCount(boidsCount)
	{
		positions.resize(boidsCount);
		accelerations.resize(boidsCount);
		velocities.resize(boidsCount);
		translations.resize(boidsCount);
		colorIndexes.resize(boidsCount);

		for (int i = 0; i < boidsCount; i++)
		{
			positions[i] = { randomFloat(params.width / 4, 3 * params.width / 4), randomFloat(params.height / 4, 3 * params.height / 4) };

			accelerations[i] = glm::vec2(0, 0);

			velocities[i] = { randomFloat(-3, 3), randomFloat(-3, 3) };
			if (glm::length(velocities[i]) == 0)
				velocities[i] = glm::vec2(1, 1);

			velocities[i] = setMagnitude(velocities[i], params.maxSpeed);

			colorIndexes[i] = { (int)randomFloat(0, 5) };
		}

		cuda_functions::allocateDataOnGPU(boidsCount, device_positions, device_velocities, device_newPositions, device_newVelocities, device_accelerations, device_translations);
		cuda_functions::sendDataToGPU(boidsCount, device_positions, device_velocities, positions.data(), velocities.data());
		cuda_functions_grid::allocateGrid(boidsCount, params.cellCount, device_gridCellIndex, device_boidSequence, device_gridCellStart, device_gridCellEnd, device_positionsSorted, device_velocitiesSorted, device_colorIndexes, device_colorSorted);
		cuda_functions::sendColorsToGPU(boidsCount, device_colorIndexes, colorIndexes.data());
		boidData.device_positions = device_positions;
		boidData.device_velocities = device_velocities;
		boidData.device_newPositions = device_newPositions;
		boidData.device_newVelocities = device_newVelocities;
		boidData.device_accelerations = device_accelerations;
		boidData.device_translations = device_translations;
		boidData.device_gridCellIndex = device_gridCellIndex;
		boidData.device_gridCellStart = device_gridCellStart;
		boidData.device_gridCellEnd = device_gridCellEnd;
		boidData.boidSequence = device_boidSequence;
		boidData.device_positionsSorted = device_positionsSorted;
		boidData.device_velocitiesSorted = device_velocitiesSorted;
		boidData.colorIndex = device_colorIndexes;
		boidData.colorSorted = device_colorSorted;
	}

	~FlockGridGPU()
	{
		cuda_functions::freeDataOnGPU(device_positions, device_velocities, device_newPositions, device_newVelocities, device_accelerations, device_translations);
	}

	void computeNextFrame(void** cuda_vbo_resource, Params params)
	{
		cuda_functions_grid::computeGridCellIndex(boidsCount, params, device_positions, device_velocities, device_gridCellIndex, device_gridCellStart, device_gridCellEnd, device_boidSequence, device_positionsSorted, device_velocitiesSorted, device_colorIndexes, device_colorSorted);

		glm::vec2* devPtr = cuda_functions::getMappedPointer(cuda_vbo_resource);
		boidData.device_translations = devPtr;
		boidData.params = params;

		cuda_functions_grid::computeNextFrame(boidsCount, boidData);
		cuda_functions_grid::swapFrames(boidsCount, device_positions, device_velocities, device_newPositions, device_newVelocities);

		cuda_functions::unmapResource(cuda_vbo_resource);
	}
};