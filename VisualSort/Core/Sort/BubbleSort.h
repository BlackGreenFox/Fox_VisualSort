#pragma once
#include "SortBase.h"
#include <vector>

class BubbleSort : public SortBase {
public:
    BubbleSort();
    void Init(std::vector<int>& values) override;
    void Step() override;
    void Pause() override;
    void Update(SDL_Renderer* renderer, int offsetX, int offsetY, int width, int height) override;
    void SetDelay(int delay) override;
    float GetComparisonState() const override;
    bool IsSorted() const;
    Uint32 GetLastStep() const override;

private:
    std::vector<int>* values_;
    bool paused_;
    int i_;
    int j_;
    bool swapped_;
    int delay_;
    Uint32 last_step_;
    float comparison_state_;
    bool sorted_;
};
