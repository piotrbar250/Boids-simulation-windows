#pragma once
#include <glm/glm.hpp>
#include "utils/BoidSimulationStructures.h"
#include "cuda_functions.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

namespace grid
{
     __inline__ __device__ int pixels2Cell(float px, float py, Params params)
    {
        int xCell = px / params.cellSize;
        int yCell = py / params.cellSize;

        return yCell * params.widthCount + xCell;
    }

    __inline__ __device__ int leftCell(int index, Params params)
    {
        int row = index % params.widthCount;
        if(row == 0)
            return -1;
        return index - 1;
    }

    __inline__ __device__ int rightCell(int index, Params params)
    {
        int row = (index + 1) % params.widthCount;
        if(row == 0)
            return -1;
        return index + 1;
    }

    __inline__ __device__ int topCell(int index, Params params)
    {
        int cell = index + params.widthCount;
        if(cell >= params.cellCount)
            return -1;
        return cell;
    }

    __inline__ __device__ int  bottomCell(int index, Params params)
    {
        int cell = index - params.widthCount;
        if(cell < 0)
            return -1;
        return cell;
    }

    __inline__ __device__ void getAdjacentCells(int index, int*neighs, Params params)
    {
        for(int i = 0; i < 8; i++)
            neighs[i] = -1;

        neighs[0] = leftCell(index, params);
        neighs[1] = rightCell(index, params);
        neighs[2] = topCell(index, params);
        neighs[3] = bottomCell(index, params);

        if(topCell(index, params) != -1)
        {
            neighs[4] = leftCell(topCell(index, params), params);
            neighs[5] = rightCell(topCell(index, params), params);
        }

        if(bottomCell(index, params) != -1)
        {
            neighs[6] = leftCell(bottomCell(index, params), params);
            neighs[7] = rightCell(bottomCell(index, params), params);
        }
    }
}
