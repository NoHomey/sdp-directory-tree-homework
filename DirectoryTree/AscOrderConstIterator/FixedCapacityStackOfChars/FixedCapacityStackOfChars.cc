#include "FixedCapacityStackOfChars.h"
#include <cstring>
#include <cassert>

FixedCapacityStackOfChars::FixedCapacityStackOfChars(const std::size_t capacity, char* data) noexcept
: FixedCapacityStack<char>{capacity, data} { }

const char* FixedCapacityStackOfChars::data() const noexcept {
    return stackData;
}

void FixedCapacityStackOfChars::pop(const std::size_t count) noexcept {
    assert(stackSize >= count);
    stackSize -= count;
}

void FixedCapacityStackOfChars::push(const char* str, const std::size_t strLength) noexcept {
    assert((stackSize + strLength) <= stackCapacity);
    std::memcpy(stackData + stackSize, str, strLength);
    stackSize += strLength;
}

void FixedCapacityStackOfChars::push(const char c) noexcept {
    assert(!isFull());
    stackData[stackSize++] = c;
}