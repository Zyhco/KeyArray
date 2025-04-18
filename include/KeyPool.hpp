// KeyPool: Enhanced key pool manager (Header)
// Author: [Your Name]

#ifndef KEYPOOL_HPP
#define KEYPOOL_HPP

#include <stack>
#include <algorithm>
#include <iostream>
#include <stdexcept>

class KeyPool {
public:

    // ──────────────────────────────────────────────
    // 🔹 Constructors
    // ──────────────────────────────────────────────

    // Constructs a key pool with keys ranging from 0 to maxKey (inclusive)
    explicit KeyPool(int maxKey = 99);

    // Constructs a key pool with sorted range from min(value1, value2) to max(value1, value2)
    KeyPool(int value1, int value2);


    // ──────────────────────────────────────────────
    // 🔹 Key Management
    // ──────────────────────────────────────────────

    // Pops the next available key from the pool
    int pop();

    // Pushes a key back into the pool
    void push(int value);

    // Checks if the pool is empty
    bool empty() const;

    // Doubles the maximum value (used for dynamic growth strategies)
    void increaseMaxValue();

    // Resets the pool with a new key range
    void reset(int newStart = 0, int newEnd = 99);


    // ──────────────────────────────────────────────
    // 🔹 Accessors
    // ──────────────────────────────────────────────

    // Returns the current value for next available key
    int getCurrentValue() const;

    // Returns the maximum allowed key
    int getMaxValue() const;


    // ──────────────────────────────────────────────
    // 🔹 Debug Output
    // ──────────────────────────────────────────────

    // Prints key pool metadata to stream
    friend std::ostream& operator<<(std::ostream& os, const KeyPool& pool);


private:

    // The next available key to assign
    int nextKey;

    // The maximum key that can be assigned
    int maxKey;

    // Stack of recycled keys
    std::stack<int> pool;
};

#endif // KEYPOOL_HPP


//
// ░░ Implementation of KeyPool ░░
// ────────────────────────────────────────────────────────────────

// 🔹 Constructors
// ────────────────────────────────────────────────────────────────

// Constructs a key pool from 0 to maxInclusive (default range)
KeyPool::KeyPool(int maxInclusive)
    : nextKey(0), maxKey(maxInclusive) {}


// Constructs a key pool from min(value1, value2) to max(value1, value2)
KeyPool::KeyPool(int value1, int value2) {
    if (value1 > value2) std::swap(value1, value2);
    nextKey = value1;
    maxKey = value2;
}


// 🔹 Key Management
// ────────────────────────────────────────────────────────────────

// Retrieves the next available key from the pool
int KeyPool::pop() {
    if (!pool.empty()) {
        int value = pool.top();
        pool.pop();
        return value;
    }
    if (nextKey > maxKey) throw std::out_of_range("No more keys available");
    return nextKey++;
}


// Recycles a key back into the pool (only if in valid range)
void KeyPool::push(int value) {
    if (value >= nextKey && value <= maxKey) {
        pool.push(value);
    }
}


// Returns true if there are no keys available
bool KeyPool::empty() const {
    return pool.empty() && nextKey > maxKey;
}


// Doubles the maximum key value (used for dynamic growth)
void KeyPool::increaseMaxValue() {
    maxKey *= 2;
}


// Resets the key pool with a new range
void KeyPool::reset(int newStart, int newEnd) {
    if (newStart > newEnd) std::swap(newStart, newEnd);
    std::stack<int> empty;
    std::swap(pool, empty);
    nextKey = newStart;
    maxKey = newEnd;
}


// 🔹 Accessors
// ────────────────────────────────────────────────────────────────

// Returns the current value of the next key
int KeyPool::getCurrentValue() const {
    return nextKey;
}


// Returns the upper bound key value
int KeyPool::getMaxValue() const {
    return maxKey;
}


// 🔹 Debug Output
// ────────────────────────────────────────────────────────────────

// Outputs key pool state to an output stream
std::ostream& operator<<(std::ostream& os, const KeyPool& pool) {
    os << "KeyPool: Current Value = " << pool.nextKey << ", Max Value = " << pool.maxKey;
    return os;
}


