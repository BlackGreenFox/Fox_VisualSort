#pragma once
#include "SortBase.h"

namespace FoxSort {
    class InsertionSort : public SortBase
    {
    public:
        InsertionSort();
        void Init(std::vector<int>& values, int delay) override;
        void Step() override;
        void Update(SDL_Renderer* renderer, int offsetX, int offsetY, int width, int height) override;

        std::string GetDescription() const override;
        std::vector<float> GetBigOPlotData() const override;
    };

}