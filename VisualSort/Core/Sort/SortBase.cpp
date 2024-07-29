#include "SortBase.h"

Uint32 SortBase::GetLastStep() const {
    return _last_step;
}

void SortBase::Pause() {
    _paused = !_paused;
}

void SortBase::SetDelay(int delay) {
    _delay = delay;
}

float SortBase::GetComparisonState() const {
    return _comparison_state;
}

bool SortBase::IsSorted() const {
    return _sorted;
}