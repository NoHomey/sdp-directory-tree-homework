#include "FixedCapacityStackOfChars.h"
#include <cstring>
#include <cassert>

FixedCapacityStackOfChars::FixedCapacityStackOfChars(const std::size_t capacity, char* data) noexcept
: FixedCapacityStack<char>{capacity, data} { }

const char* FixedCapacityStackOfChars::data() const noexcept {
    return stackData;
}