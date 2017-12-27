#pragma once

#include "../../../FixedCapacityStack/FixedCapacityStack.thd"

class FixedCapacityStackOfChars: public FixedCapacityStack<char> {
public:
    FixedCapacityStackOfChars() noexcept = delete;

    ~FixedCapacityStackOfChars() noexcept = default;

    FixedCapacityStackOfChars(const std::size_t capacity, char* data) noexcept;

    FixedCapacityStackOfChars(const FixedCapacityStackOfChars& other) = delete;

    FixedCapacityStackOfChars(FixedCapacityStackOfChars&& other) noexcept = default;

    FixedCapacityStack& operator=(const FixedCapacityStack& other) = delete;

    FixedCapacityStack& operator=(FixedCapacityStack&& other) noexcept = delete;

public:
    const char* data() const noexcept;

public:
    void pop(const std::size_t count) noexcept;

    void push(const char* str, const std::size_t strLength) noexcept;

    void push(const char c) noexcept;
};