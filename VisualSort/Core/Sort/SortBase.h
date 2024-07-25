#pragma once
#include <vector>
#include <SDL.h>

class SortBase {
public:
    virtual ~SortBase() = default;

    virtual void Init(std::vector<int>& values) = 0;
    virtual void Step() = 0;
    virtual void Pause() = 0;
    virtual void Update(SDL_Renderer* renderer, int offsetX, int offsetY, int width, int height) = 0;
    virtual void SetDelay(int delay) = 0;
    virtual float GetComparisonState() const = 0;  // Add this line
    virtual Uint32 GetLastStep() const = 0;
};

