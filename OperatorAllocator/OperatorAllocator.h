#pragma once

#include <cstddef>

class OperatorAllocator {
public:
    OperatorAllocator() noexcept = default;

    ~OperatorAllocator() noexcept = default;

    OperatorAllocator(const OperatorAllocator& other) = delete;

    OperatorAllocator(OperatorAllocator&& other) noexcept = default;

    OperatorAllocator& operator=(const OperatorAllocator& other) = delete;

    OperatorAllocator& operator=(OperatorAllocator&& other) noexcept = delete;

public:
    void* allocate(const std::size_t bytes);

    void release(void* ptr, const std::size_t bytes);

    bool safeRelease(void* ptr, const std::size_t bytes) noexcept;

    bool reallocate(void* ptr, const std::size_t oldSizeInBytes, const std::size_t newSizeInBytes);
};