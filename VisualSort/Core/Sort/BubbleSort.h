#pragma once
#include "SortBase.h"


namespace FoxSort {

class BubbleSort : public SortBase {
public:
    BubbleSort();
    void Init(std::vector<int>& values, int delay) override;
    void Step() override;
   
    std::string GetDescription() const override;
    std::vector<float> GetBigOPlotData() const override;
};

}