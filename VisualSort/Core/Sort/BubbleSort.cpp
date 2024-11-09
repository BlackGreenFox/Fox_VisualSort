#include "BubbleSort.h"

namespace FoxSort {
    BubbleSort::BubbleSort()
    {
        _values = nullptr;
        _paused = true;
        _i = 0;
        _j = 0;
        _delay = 0;
        _swapped = false;
        _last_step = SDL_GetTicks();
        _comparison_state = 0.0f;
        _sorted = false;
    }

    void BubbleSort::Init(std::vector<int>& values, int delay) {
        _values = &values;
        _paused = true;
        _i = 0;
        _j = 0;
        _delay = delay;
        _swapped = false;
        _last_step = SDL_GetTicks();
        _comparison_state = 0.0f;
        _sorted = false;
    }


    void BubbleSort::Step() {
        if (_paused || _values == nullptr || _i >= _values->size() - 1) {
            return;
        }

        Uint32 current_time = SDL_GetTicks();
        if (current_time - _last_step < _delay) {
            return;
        }
        _last_step = current_time;

        if (_j < _values->size() - _i - 1) {
            // Increment comparison counter
            if ((*_values)[_j] > (*_values)[_j + 1]) {
                std::swap((*_values)[_j], (*_values)[_j + 1]);
                _swapped = true;
                _comparison_state++;  // Reset comparison counter on swap
            }
            else {
                _comparison_state = 0;
            }
            _j++;
        }
        else {
            if (!_swapped) {
                _i = _values->size();
                _sorted = true;  // Indicate that the array is sorted
            }
            _j = 0;
            _i++;
            _swapped = false;
        }
    }


    std::string BubbleSort::GetDescription() const {
        return "Bubble sort, sometimes referred to as sinking sort, is a simple sorting algorithm that repeatedly steps through the input list element by element, comparing the current element with the one after it, swapping their values if needed. These passes through the list are repeated until no swaps have to be performed during a pass, meaning that the list has become fully sorted. The algorithm, which is a comparison sort, is named for the way the larger elements 'bubble' up to the top of the list.";
    }

    std::vector<float> BubbleSort::GetBigOPlotData() const {
        std::vector<float> big_o_plot(_values->size());
        for (size_t i = 0; i < _values->size(); ++i) {
            big_o_plot[i] = static_cast<float>(i * i);
        }
        return big_o_plot;
    }

}