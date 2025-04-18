// KeyArray: Extended structure for KeyArray
// Author: Eli (Eliyahu) Shif

#ifndef KEYARRAY_HPP
#define KEYARRAY_HPP

#include "KeyArrayBase.hpp"
#include <queue>
#include <functional>
#include <fstream>
#include <iostream>

/**
 * @brief KeyArray provides an extended structure over KeyArrayBase.
 *        It supports named instances, offset key mapping,
 *        optional dynamic resizing, and a fallback queue mechanism.
 */
template <typename T>
class KeyArray : public KeyArrayBase<T> {
public:
    using KeyArrayBase<T>::KeyArrayBase;

        // ─────────────────────────────────────────────────────────────
    // 🔹 Construction & Initialization
    // ─────────────────────────────────────────────────────────────

    // Constructs a KeyArray with optional name
    KeyArray(const std::string& name = "");

    // Constructs a KeyArray from 0 to limitKey - 1
    KeyArray(int limitKey, const std::string& name = "");

    // Constructs a KeyArray with given offset and last key (order-independent)
    KeyArray(int a, int b, const std::string& name = "");

    // ─────────────────────────────────────────────────────────────
    // 🔹 Core KeyArray Functionality
    // ─────────────────────────────────────────────────────────────

    // Inserts a value and returns the assigned key
    int insert(const T& value) override;

    // Removes a value by key
    void remove(int key) override;

    // Checks if a given key is currently in use
    bool hasKey(int key) const override;

    // Checks if a given value exists in the structure
    bool contains(const T& value) const override;

    // Access a value by key (modifiable)
    T& at(int key) override;

    // Access a value by key (read-only)
    const T& at(int key) const override;

    // Clears all elements and resets internal state
    void clear() override;

    // ─────────────────────────────────────────────────────────────
    // 🔹 Dynamic Resizing
    // ─────────────────────────────────────────────────────────────

    // Enables dynamic resizing mode
    void enableDynamicResizing();

    // Disables dynamic resizing, optionally purging the copy buffer
    void disableDynamicResizing(bool purgeData = false);

    // Returns whether dynamic resizing is enabled
    bool isDynamicResizingEnabled() const;

    // Performs one step of copying from old to new buffer
    void continueCopyStep();

    // Replaces the old buffer with the resized one
    void switchToResizedData();

    // ─────────────────────────────────────────────────────────────
    // 🔹 Optional Overflow Queue
    // ─────────────────────────────────────────────────────────────

    // Enables queue mode (used when pool is full and dynamic resizing is off)
    void enableQueue();

    // Disables the queue and stops accepting new elements into it
    void disableQueue();

    // Clears the overflow queue
    void clearQueue();

    // Returns the current size of the queue
    size_t getQueueSize() const;

    // Returns a modifiable reference to the queue
    std::queue<T>& getQueue();

    // Returns a const reference to the queue
    const std::queue<T>& getQueue() const;

    // ─────────────────────────────────────────────────────────────
    // 🔹 Metadata and Debugging Utilities
    // ─────────────────────────────────────────────────────────────

    // Sets the name of this KeyArray instance
    void setName(const std::string& newName);

    // Gets the name of this KeyArray instance
    std::string getName() const;

    // Swaps the values of two keys
    void swap(int key1, int key2);

    // Saves the current state to a file
    void saveToFile(const std::string& filename) const;

    // Loads KeyArray state from a file
    void loadFromFile(const std::string& filename);

    // ─────────────────────────────────────────────────────────────
    // 🔹 Accessors
    // ─────────────────────────────────────────────────────────────

    // Returns the offset of the key space
    int getOffset() const;

    // Returns the maximum usable key (inclusive upper bound)
    int getMaxKeyBound() const;

    // Begin iterator (modifiable)
    auto begin();

    // End iterator (modifiable)
    auto end();

    // Begin iterator (const)
    auto begin() const;

    // End iterator (const)
    auto end() const;






    private:

    // ──────────────────────────────────────────────
    // Basic structure configuration
    // ──────────────────────────────────────────────

    // Logical key offset – shifts all external keys by this amount internally
    int offset;

    // Optional human-readable identifier for the structure
    std::string name;


    // ──────────────────────────────────────────────
    // Dynamic resizing configuration
    // ──────────────────────────────────────────────

    // Indicates whether dynamic resizing is enabled
    bool resizingEnabled = false;

    // Indicates whether the copying process is in progress (from current to new arrays)
    bool copyInProgress = false;

    // The index currently being copied to the new array
    int copyIndex = 0;

    // Temporary array to hold new values during dynamic resizing
    std::vector<T> newData;

    // Validity flags for the new array
    std::vector<bool> newValid;

    // KeyPool for keys in the new resized array
    KeyPool newPool;


    // ──────────────────────────────────────────────
    // Overflow queue configuration
    // ──────────────────────────────────────────────

    // Indicates whether the overflow queue is active
    bool queueEnabled = false;

    // Queue for holding values that couldn't be inserted due to full capacity
    std::queue<T> overflowQueue;


};






// ▼▼▼ Implementation of KeyArray ▼▼▼



// ──────────────────────────────────────────────
// Constructors
// ──────────────────────────────────────────────

// Default constructor with optional name
template <typename T>
KeyArray<T>::KeyArray(const std::string& name)
    : KeyArrayBase<T>(), offset(0), name(name) {}


// Constructor with maximum key value and optional name
template <typename T>
KeyArray<T>::KeyArray(int limitKey, const std::string& name)
    : KeyArrayBase<T>(limitKey), offset(0), name(name) {}


// Constructor with two values (interpreted as offset and limit)
template <typename T>
KeyArray<T>::KeyArray(int a, int b, const std::string& name)
    : KeyArrayBase<T>(std::max(a, b) - std::min(a, b)), offset(std::min(a, b)), name(name) {}



// ──────────────────────────────────────────────
// Insertion
// ──────────────────────────────────────────────

// Inserts a value into the array and handles overflow queue or dynamic resizing
template <typename T>
int KeyArray<T>::insert(const T& value) {
    if (this->pool.empty()) {
        if (resizingEnabled) {
            if (copyInProgress) {
                // Complete the ongoing copy if the old array is full
                while (copyInProgress) {
                    continueCopyStep();
                }
            }
            switchToResizedData();
        } else if (queueEnabled) {
            overflowQueue.push(value);
            return -1; // indicator that value was added to queue
        } else {
            throw std::runtime_error("KeyPool is empty. No available keys.");
        }
    }

    int actualKey = KeyArrayBase<T>::insert(value);

    if (resizingEnabled) {
        this->newData[actualKey] = value;
        this->newValid[actualKey] = true;

        if (copyInProgress)
            continueCopyStep(); // Continue one step of copying
    }

    return actualKey + offset;
}



// ──────────────────────────────────────────────
// Removal
// ──────────────────────────────────────────────

// Removes an element by key, adjusted for offset
template <typename T>
void KeyArray<T>::remove(int key) {
    if (key < offset || key > this->lastKey + offset) {
        throw std::out_of_range("Key out of valid range in KeyArray");
    }

    int actualKey = key - offset;
    KeyArrayBase<T>::remove(actualKey);

    if (resizingEnabled) {
        newValid[actualKey] = false;
    }
}



// ──────────────────────────────────────────────
// Key and Value Lookup
// ──────────────────────────────────────────────

// Checks if a specific key is currently active
template <typename T>
bool KeyArray<T>::hasKey(int key) const {
    if (key < offset || key > this->lastKey + offset)
        return false;
    return KeyArrayBase<T>::hasKey(key - offset);
}


// Checks if the given value exists in the structure
template <typename T>
bool KeyArray<T>::contains(const T& value) const {
    return KeyArrayBase<T>::contains(value);
}



// ──────────────────────────────────────────────
// Element Access
// ──────────────────────────────────────────────

// Access element by key (non-const version)
template <typename T>
T& KeyArray<T>::at(int key) {
    if (key < offset || key > this->lastKey + offset)
        throw std::out_of_range("Invalid key in KeyArray");
    return KeyArrayBase<T>::at(key - offset);
}


// Access element by key (const version)
template <typename T>
const T& KeyArray<T>::at(int key) const {
    if (key < offset || key > this->lastKey + offset)
        throw std::out_of_range("Invalid key in KeyArray");
    return KeyArrayBase<T>::at(key - offset);
}



// ──────────────────────────────────────────────
// Clear
// ──────────────────────────────────────────────

// Clears all data and resets queues and dynamic resizing
template <typename T>
void KeyArray<T>::clear() {
    // Clear base structure
    KeyArrayBase<T>::clear();

    // Clear overflow queue
    std::queue<T> emptyQueue;
    std::swap(overflowQueue, emptyQueue);

    // Reset dynamic resizing structures
    newData.clear();
    newValid.clear();
    newPool = KeyPool();
    copyInProgress = false;
    copyIndex = 0;
}



/* =========================================================================
   Dynamic Resizing Management
   =========================================================================
   These methods control the behavior of dynamic resizing. When enabled,
   the structure doubles in size when full and gradually copies elements
   to a secondary structure. Once copying is complete, the new structure
   replaces the old one.
*/

// Enables dynamic resizing for the KeyArray.
// If not already enabled, this prepares the secondary structure for gradual copying.
template <typename T>
void KeyArray<T>::enableDynamicResizing() {
    if (resizingEnabled) return;

    resizingEnabled = true;

    // Start preparing new storage only if copy hasn't started yet
    if (copyIndex == offset) {
        int currentSize = this->lastKey + 1 - offset;
        int newLastKey = offset + currentSize * 2 - 1;

        newData.resize(newLastKey + 1);
        newValid.resize(newLastKey + 1, false);

        // New pool only manages new key range beyond current lastKey
        newPool = KeyPool(this->lastKey + 1 - offset, newLastKey - offset);
    }

    copyInProgress = true;
}

// Disables dynamic resizing.
// If purgeData is true, it also clears all temporary storage.
template <typename T>
void KeyArray<T>::disableDynamicResizing(bool purgeData) {
    resizingEnabled = false;

    if (purgeData) {
        newData.clear();
        newValid.clear();
        newPool = KeyPool();  // reset to empty pool
        copyInProgress = false;
        copyIndex = 0;
    }
}

// Returns whether dynamic resizing is enabled
template <typename T>
bool KeyArray<T>::isDynamicResizingEnabled() const {
    return resizingEnabled;
}

// Performs one step of the copy process during dynamic resizing.
// Each call copies one entry from the old structure to the new one.
template <typename T>
void KeyArray<T>::continueCopyStep() {
    if (!resizingEnabled || !copyInProgress) return;

    if (copyIndex <= this->lastKey) {
        if (this->valid[copyIndex]) {
            newData[copyIndex] = this->data[copyIndex];
            newValid[copyIndex] = true;
        }
        ++copyIndex;
    }

    // Mark copy as complete
    if (copyIndex > this->lastKey) {
        copyInProgress = false;
    }
}

// Replaces the current storage with the newly resized structure.
// Only allowed after copying is completed.
template <typename T>
void KeyArray<T>::switchToResizedData() {
    if (!resizingEnabled || copyInProgress) {
        throw std::runtime_error("Cannot switch data: either resizing is not enabled or copy is not finished.");
    }

    // Replace internal data arrays
    this->data = std::move(newData);
    this->valid = std::move(newValid);
    this->pool = std::move(newPool);

    // Update key range
    this->lastKey = static_cast<int>(this->data.size()) - 1;

    // Reset old copying-related fields
    newData.clear();
    newValid.clear();
    newPool = KeyPool();
    copyIndex = 0;

    // Prepare next resize round if dynamic resizing remains enabled
    if (resizingEnabled) {
        int currentSize = this->lastKey + 1 - offset;
        int newLastKey = offset + currentSize * 2 - 1;

        newData.resize(newLastKey + 1);
        newValid.resize(newLastKey + 1, false);
        newPool = KeyPool(this->lastKey + 1 - offset, newLastKey - offset);

        copyInProgress = true;
        copyIndex = 0;
    }
}


/* =========================================================================
   Overflow Queue Management
   =========================================================================
   These methods provide access to an optional queue that stores values
   when the structure is full and dynamic resizing is not enabled.
   This gives the user control over overflow behavior without data loss.
*/

// Enables the overflow queue.
// Values will be pushed into this queue if insert is called when full.
template <typename T>
void KeyArray<T>::enableQueue() {
    queueEnabled = true;
}

// Disables the overflow queue.
// Further insertions when full will throw an exception unless resizing is enabled.
template <typename T>
void KeyArray<T>::disableQueue() {
    queueEnabled = false;
}

// Clears the overflow queue by swapping it with an empty instance.
template <typename T>
void KeyArray<T>::clearQueue() {
    std::queue<T> empty;
    std::swap(overflowQueue, empty);
}

// Returns the number of elements currently in the overflow queue.
template <typename T>
size_t KeyArray<T>::getQueueSize() const {
    return overflowQueue.size();
}

// Returns a modifiable reference to the overflow queue.
template <typename T>
std::queue<T>& KeyArray<T>::getQueue() {
    return overflowQueue;
}

// Returns a const reference to the overflow queue.
template <typename T>
const std::queue<T>& KeyArray<T>::getQueue() const {
    return overflowQueue;
}

// ──────────────────────────────────────────────
// Basic Metadata and Manipulation
// ──────────────────────────────────────────────

// Sets the name of the current KeyArray instance
template <typename T>
void KeyArray<T>::setName(const std::string& newName) {
    name = newName;
}

// Gets the name of the current KeyArray instance
template <typename T>
std::string KeyArray<T>::getName() const {
    return name;
}

// Swaps the values of two keys, if both are valid
template <typename T>
void KeyArray<T>::swap(int key1, int key2) {
    if (!hasKey(key1) || !hasKey(key2)) {
        throw std::invalid_argument("One or both keys are invalid.");
    }
    std::swap(this->data[key1 - this->offset], this->data[key2 - this->offset]);
}

// Saves the KeyArray's state to a file (including metadata and overflow queue)
template <typename T>
void KeyArray<T>::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for saving.");
    }

    file << "version: 2.0\n";
    file << "name: " << (name.empty() ? "(unnamed)" : name) << '\n';
    file << "offset: " << offset << '\n';
    file << "lastKey: " << this->lastKey << '\n';
    file << "dynamicResizing: " << (resizingEnabled ? "true" : "false") << '\n';
    file << "queueEnabled: " << (queueEnabled ? "true" : "false") << '\n';
    file << "queueSize: " << overflowQueue.size() << '\n';

    file << "entries:\n";
    for (int i = 0; i <= this->lastKey; ++i) {
        if (this->valid[i]) {
            file << (i + offset) << ' ' << this->data[i] << '\n';
        }
    }

    file << "queueEntries:\n";
    if (queueEnabled && !overflowQueue.empty()) {
        std::queue<int> copy = overflowQueue;
        while (!copy.empty()) {
            file << copy.front() << '\n';
            copy.pop();
        }
    }

    file.close();
}

// Loads KeyArray's state from a file (including metadata and overflow queue)
template <typename T>
void KeyArray<T>::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for loading.");
    }

    std::string versionLine;
    std::getline(file, versionLine); // version: 2.0

    std::string label;
    file >> label >> name;
    file >> label >> offset;
    file >> label >> this->lastKey;

    int effectiveSize = this->lastKey + 1;
    this->data.resize(effectiveSize);
    this->valid.resize(effectiveSize, false);
    this->elementCount = 0;
    this->pool.reset(offset, this->lastKey);

    file >> label >> std::boolalpha >> resizingEnabled;
    file >> label >> std::boolalpha >> queueEnabled;

    size_t queueSize = 0;
    file >> label >> queueSize;

    std::string entriesLabel;
    file >> entriesLabel; // entries:

    int key;
    T value;

    while (file >> key) {
        if (key == -1) break; // Separator between entries and queue
        file >> value;
        int actualIndex = key - offset;
        if (actualIndex >= 0 && actualIndex < static_cast<int>(this->data.size())) {
            this->data[actualIndex] = value;
            this->valid[actualIndex] = true;
            ++this->elementCount;
        }
    }

    std::string queueLabel;
    file >> queueLabel; // queueEntries:

    while (file >> key) {
        overflowQueue.push(key);
    }

    file.close();
}

/* =========================================================================
   Structural Info & Iterators
   =========================================================================
   These methods expose basic structural information about the KeyArray
   and provide STL-compatible iterators for traversing the underlying data.
*/

// Returns the offset of the key space (i.e., the first logical key)
template <typename T>
int KeyArray<T>::getOffset() const {
    return offset;
}

// Returns the last valid key (inclusive upper bound for the logical key range)
template <typename T>
int KeyArray<T>::getMaxKeyBound() const {
    return offset + this->lastKey;
}

// Returns a modifiable iterator to the beginning of the internal data array
template <typename T>
auto KeyArray<T>::begin() {
    return this->data.begin();
}

// Returns a modifiable iterator to the end of the internal data array
template <typename T>
auto KeyArray<T>::end() {
    return this->data.end();
}

// Returns a const iterator to the beginning of the internal data array
template <typename T>
auto KeyArray<T>::begin() const {
    return this->data.begin();
}

// Returns a const iterator to the end of the internal data array
template <typename T>
auto KeyArray<T>::end() const {
    return this->data.end();
}








    

#endif // KEYARRAY_HPP
