#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <SDL.h>

class SortBase {
public:
    virtual ~SortBase() = default;

    virtual void Init(std::vector<int>& values, int delay) = 0;
    virtual void Step() = 0;
    virtual void Update(SDL_Renderer* renderer, int offsetX, int offsetY, int width, int height) = 0;
    
    void Pause();
    void SetDelay(int delay);
    float GetComparisonState() const;
    bool IsSorted() const;
    Uint32 GetLastStep() const;

    virtual std::string GetDescription() const = 0;
    virtual std::vector<float> GetBigOPlotData() const = 0;

protected:
    std::vector<int>* _values;

    int _delay;
    int _i;
    int _j;
    float _comparison_state;

    bool _paused;
    bool _swapped;
    bool _sorted;

    Uint32 _last_step;
};

