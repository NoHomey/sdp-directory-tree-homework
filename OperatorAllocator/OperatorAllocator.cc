#include "OperatorAllocator.h"
#include <cassert>

void* OperatorAllocator::allocate(const std::size_t bytes) {
    assert(bytes);
    return new unsigned char[bytes];
}

void OperatorAllocator::release(void* ptr, const std::size_t bytes) {
    safeRelease(ptr, bytes);
}

bool OperatorAllocator::safeRelease(void* ptr, const std::size_t) noexcept {
    assert(ptr);
    delete[] reinterpret_cast<unsigned char*>(ptr);
    return true;
}

bool OperatorAllocator::reallocate(void*, const std::size_t, const std::size_t) {
    return false;
}