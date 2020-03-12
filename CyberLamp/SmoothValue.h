#pragma once

#include <Arduino.h>
#include <EasingBase.h>
#include <type_traits>

template <class T, class K>
class SmoothValue {
    T value;
    K smoother;
    unsigned long timeout;

  public:
	SmoothValue(T);
    T getValue();
	T getSmoothValue();
	void setValue(T, int = 1000);
	bool isChanging();
};

template <class T, class K>
SmoothValue<T, K>::SmoothValue(T value) : value(value), timeout(0) {
    static_assert(std::is_base_of<EasingBase, K>::value, "parameter K must derive from EasingBase");
}

template <class T, class K>
T SmoothValue<T, K>::getValue() {
    return value;
}

template <class T, class K>
T SmoothValue<T, K>::getSmoothValue() {
    if (!timeout)
        return value;

    uint64_t time = millis();
    if (time >= timeout) {
        timeout = 0;
        return value;
    }

    return value - smoother.easeInOut(timeout - time);
}

template <class T, class K>
void SmoothValue<T, K>::setValue(T newValue, int duration) {
    if (value == newValue)
        return;
        
    smoother.setDuration(duration);
    smoother.setTotalChangeInPosition(newValue - value);
    value = newValue;
    timeout = millis() + duration;
}

template <class T, class K>
bool SmoothValue<T, K>::isChanging() {
    return timeout;
}