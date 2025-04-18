// KeyArrayBase: Base structure for KeyArray
// Author: Eli (Eliyahu) Shif

#ifndef KEYARRAYBASE_HPP
#define KEYARRAYBASE_HPP

#include "KeyPool.hpp"
#include <vector>
#include <optional>
#include <iostream>

template <typename T>
class KeyArrayBase {
public:

    // ───────────────────────────────────────────────────────────── //
    // 🔹 Construction
    // ───────────────────────────────────────────────────────────── //

    // Constructs a key array with keys ranging from 0 to limitKey - 1
    KeyArrayBase(int limitKey = 100);


    // ───────────────────────────────────────────────────────────── //
    // 🔹 Modification
    // ───────────────────────────────────────────────────────────── //

    // Inserts a value into the next available key and returns the key
    virtual int insert(const T& value);

    // Removes the value associated with the given key
    virtual void remove(int key);


    // ───────────────────────────────────────────────────────────── //
    // 🔹 Validation and Query
    // ───────────────────────────────────────────────────────────── //

    // Returns true if the given key is valid and currently in use
    virtual bool hasKey(int key) const;

    // Returns true if the given value exists in the structure (linear search)
    virtual bool contains(const T& value) const;


    // ───────────────────────────────────────────────────────────── //
    // 🔹 Accessors
    // ───────────────────────────────────────────────────────────── //

    // Returns a reference to the value at the given key
    virtual T& at(int key);

    // Returns a const reference to the value at the given key
    virtual const T& at(int key) const;


    // ───────────────────────────────────────────────────────────── //
    // 🔹 State
    // ───────────────────────────────────────────────────────────── //

    // Returns the number of currently stored elements
    size_t size() const;

    // Returns true if the structure is empty
    bool empty() const;


    // ───────────────────────────────────────────────────────────── //
    // 🔹 Maintenance
    // ───────────────────────────────────────────────────────────── //

    // Clears all elements and resets the structure
    virtual void clear();

    // Prints the structure to the output stream
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const KeyArrayBase<U>& array);




protected:
    // -------------------------
    // Core Storage and Metadata
    // -------------------------

    // The highest key allowed (inclusive upper bound)
    int lastKey;

    // Number of active (valid) elements in the structure
    size_t elementCount;

    // Underlying data storage for elements
    std::vector<T> data;

    // Parallel validity flags for each key
    std::vector<bool> valid;

    // Key pool for managing available keys (reuse and allocation)
    KeyPool pool;

};





// ===============================
// KeyArrayBase<T>: Implementations
// ===============================

template <typename T>
KeyArrayBase<T>::KeyArrayBase(int limitKey)
    : lastKey(limitKey - 1), elementCount(0) {
    
    data.resize(lastKey + 1);
    valid.resize(lastKey + 1, false);
    pool = KeyPool(0, lastKey);
}

// Inserts a new value into the structure and returns its assigned key.
// Throws std::runtime_error if no keys are available.
template <typename T>
int KeyArrayBase<T>::insert(const T& value) {
    if (pool.empty()) {
        throw std::runtime_error("KeyPool is empty. No available keys.");
    }

    int key = pool.pop();
    data[key] = value;
    valid[key] = true;
    ++elementCount;
    return key;
}

// Removes the element associated with the given key.
// Throws std::out_of_range if the key is not valid or inactive.
template <typename T>
void KeyArrayBase<T>::remove(int key) {
    if (!hasKey(key)) {
        throw std::out_of_range("Key is not valid or not in use");
    }

    valid[key] = false;
    --elementCount;
    pool.push(key);
}

// Checks if a given key is within range and currently holds a valid value.
template <typename T>
bool KeyArrayBase<T>::hasKey(int key) const {
    return key >= 0 && key <= lastKey && valid[key];
}

// Performs a linear search to check if the given value exists in the structure.
template <typename T>
bool KeyArrayBase<T>::contains(const T& value) const {
    for (int i = 0; i <= lastKey; ++i) {
        if (valid[i] && data[i] == value) {
            return true;
        }
    }
    return false;
}

// Returns a modifiable reference to the value at the given key.
// Throws std::out_of_range if the key is invalid or unused.
template <typename T>
T& KeyArrayBase<T>::at(int key) {
    if (!hasKey(key)) {
        throw std::out_of_range("Invalid key");
    }
    return data[key];
}

// Returns a constant reference to the value at the given key.
// Throws std::out_of_range if the key is invalid or unused.
template <typename T>
const T& KeyArrayBase<T>::at(int key) const {
    if (!hasKey(key)) {
        throw std::out_of_range("Invalid key");
    }
    return data[key];
}

// Returns the number of currently stored elements in the structure.
template <typename T>
size_t KeyArrayBase<T>::size() const {
    return elementCount;
}

// Returns true if the structure contains no elements.
template <typename T>
bool KeyArrayBase<T>::empty() const {
    return elementCount == 0;
}

// Clears all elements in the structure and resets the key pool.
template <typename T>
void KeyArrayBase<T>::clear() {
    data = std::vector<T>(lastKey + 1);
    valid = std::vector<bool>(lastKey + 1, false);
    elementCount = 0;
    pool = KeyPool(0, lastKey);
}

// Prints the contents of the structure to the given output stream.
template <typename T>
std::ostream& operator<<(std::ostream& os, const KeyArrayBase<T>& array) {
    os << "KeyArrayBase (Size: " << array.size() << ") [";
    for (int i = 0; i <= array.lastKey; ++i) {
        if (array.valid[i]) {
            os << "(" << i << ": " << array.data[i] << ") ";
        }
    }
    os << "]";
    return os;
}


#endif // KEYARRAYBASE_HPP
