#pragma once
#include "../../Includes.h"
#include <random>
#include <limits>




template<int D>
class PoissonDisk{
    
private:
    
    static float Length(const std::array<float, D>& v)
    {
        float length = 0.0f;
        
        for(int i = 0; i < D; i++)
            length += (v[i]*v[i]);
        
        return std::sqrt(length);
    }
    
    static float Random(float min, float max)
    {
        static std::default_random_engine rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
        
        return dist(rng);
    }
    
    static std::array<float, D> Random(const std::array<float, D>& min, const std::array<float, D>& max)
    {
        std::array<float, D> p;
        
        for(int i = 0; i < D; i++)
            p[i] = Random(min[i], max[i]);
        
        return p;
    }
    
    static std::array<float, D> Normalize(const std::array<float, D>& v)
    {
        std::array<float, D> u = v;
        
        float length = Length(u);
        
        if(length == 0.0f)
            return u;
        
        for(int i = 0; i < D; i++)
            u[i] /= length;
        
        return u;
    }
    
    static std::array<float, D> RandomWithinSphericalAnnulus(const std::array<float, D>& sample, float minDistance)
    {
        std::array<float, D> minDir;
        for(int i = 0; i < D; i++)
            minDir[i] = -1.0f;
        
        std::array<float, D> maxDir;
        for(int i = 0; i < D; i++)
            maxDir[i] = 1.0f;
        
        std::array<float, D> randomDir = Normalize(Random(minDir, maxDir));
        
        float t = Random(minDistance, 2.0f * minDistance);
        
        std::array<float, D> newSample;
        for(int i = 0; i < D; i++)
            newSample[i] = sample[i] + (randomDir[i] * t);
        
        return newSample;
    }
        
    static std::array<float, D> Subtract(const std::array<float, D>& a, const std::array<float, D>& b)
    {
        std::array<float, D> c;
        
        for(int i = 0; i < D; i++)
            c[i] = a[i] - b[i];
        
        return c;
    }
    
    static std::array<int, D> ComputeGridIndex(const std::array<float, D>& sample, const std::array<float, D>& min, float cellSize)
    {
        std::array<int, D> gridIndex;
        
        for(int i = 0; i < D; i++)
            gridIndex[i] = (sample[i] - min[i]) / cellSize;
        
        return gridIndex;
    }
    
    static std::array<int, D> ComputeNeighborGridIndex(const std::array<int, D>& index, int rank)
    {
        int d, offset, tmp = rank;
        std::array<int, D> neighborIndex = index;
        
        for(d = 0; d < D; d++){
            offset = (tmp % 3) - 1;
            tmp /= 3;
            neighborIndex[d] += offset;
        }
        
        return neighborIndex;
    }
    
    static int ComputeFlattenedGridIndex(const std::array<int, D>& gridIndex, const std::array<int, D>& gridResolutions)
    {
        int flattenedGridIndex = gridIndex[0];
        int stride = gridResolutions[0];
        for(int i = 1; i < D; i++){
            flattenedGridIndex += (gridIndex[i] * stride);
            stride *= gridResolutions[i];
        }
        
        return flattenedGridIndex;
    }
    
    static std::array<int, D> ComputeGridResolutions(float cellSize, const std::array<float, D>& min, const std::array<float, D>& max)
    {
        std::array<int, D> gridResolutions;
        for(int i = 0; i < D; i++)
            gridResolutions[i] = std::ceil((max[i] - min[i]) / cellSize);
        
        return gridResolutions;
    }
    
    static int ComputeTotalNumGridCells(const std::array<int, D>& gridResolutions)
    {
        int totalNumGridCells = 1;
        for(int i = 0; i < D; i++)
            totalNumGridCells *= gridResolutions[i];

        return totalNumGridCells;
    }
    
    static std::array<float, D> ComputeBallCenter(const std::array<float, D>& min, const std::array<float, D>& max)
    {
        std::array<float, D> ballCenter;
        for(int i = 0; i < D; i++)
            ballCenter[i] = (max[i] + min[i]) / 2.0f;
        
        return ballCenter;
    }
    
    static float ComputeBallRadius(const std::array<float, D>& min, const std::array<float, D>& max)
    {
        float ballRadius = std::numeric_limits<float>::infinity();
        for(int i = 0; i < D; i++){
            float extent = (max[i] - min[i]) / 2.0f;
            if(extent < ballRadius)
                ballRadius = extent;
        }
        
        return ballRadius;
    }
    
public:
    
    static std::vector<std::array<float, D>> Generate(int numSamples, int maxNumAttempts, float minDistance, const std::array<float, D>& min, const std::array<float, D>& max)
    {
        int numNeighbors = pow(3, D);
        float cellSize = minDistance / sqrt(D);
        
        std::array<int, D> gridResolutions = ComputeGridResolutions(cellSize, min, max);
        
        int totalNumGridCells = ComputeTotalNumGridCells(gridResolutions);
        
        std::array<float, D> ballCenter = ComputeBallCenter(min, max);
        
        float ballRadius = ComputeBallRadius(min, max);
        
        std::array<float, D> initialSample = ballCenter;
        std::array<int, D> initialSampleGridIndex = ComputeGridIndex(initialSample, min, cellSize);
        int initialSampleFlattenedGridIndex = ComputeFlattenedGridIndex(initialSampleGridIndex, gridResolutions);
        
        std::vector<std::array<float, D>> samples;
        samples.push_back(initialSample);
        
        std::vector<int> activeSamples;
        activeSamples.push_back(0);
        
        std::vector<int> grid(totalNumGridCells, -1);
        grid[initialSampleFlattenedGridIndex] = 0;
        
        while(!activeSamples.empty() && samples.size() < numSamples){
            std::array<float, D> sample = samples[activeSamples[activeSamples.size()-1]];
            
            bool retire = true;
            
            for(int i = 0; i < maxNumAttempts; i++){
                std::array<float, D> candidate = RandomWithinSphericalAnnulus(sample, minDistance);
                
                if(Length(Subtract(candidate, ballCenter)) > ballRadius)
                    continue;
                
                std::array<int, D> candidateGridIndex = ComputeGridIndex(candidate, min, cellSize);
                int candidateFlattenedGridIndex = ComputeFlattenedGridIndex(candidateGridIndex, gridResolutions);
                
                bool candidateApproved = true;
                
                for(int m = 0; m < numNeighbors; m++){
                    
                    std::array<int, D> neighborGridIndex = ComputeNeighborGridIndex(candidateGridIndex, m);
                    
                    if(neighborGridIndex == candidateGridIndex)
                        continue;
                    
                    int neighborFlattenedGridIndex = ComputeFlattenedGridIndex(neighborGridIndex, gridResolutions);
                                        
                    if(grid[neighborFlattenedGridIndex] == -1)
                        continue;
                    
                    std::array<float, D> neighbor = samples[grid[neighborFlattenedGridIndex]];
                    
                    if(Length(Subtract(neighbor, candidate)) < minDistance){
                        candidateApproved = false;
                        break;
                    }
                }
                
                if(candidateApproved){
                    samples.push_back(candidate);
                    activeSamples.push_back(samples.size() - 1);
                    grid[candidateFlattenedGridIndex] = samples.size() - 1;
                    retire = false;
                }
            }
            
            if(retire)
                activeSamples.pop_back();
        }
        
        return samples;
    }
};
