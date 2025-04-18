
# KeyArray: Explanations & Time Complexities

This document provides detailed explanations for each public method in the `KeyArray` structure, including the worst-case time complexity.

## ✅ Constant Time Operations (Θ(1) Worst-Case)
These operations are guaranteed to run in constant time:

| Function               | Description                                                      |
|------------------------|------------------------------------------------------------------|
| `insert(value)`        | Inserts a value into the structure and returns its assigned key |
| `remove(key)`          | Removes a key and makes it available again                      |
| `at(key)`              | Accesses the value at a given key (modifiable or read-only)     |
| `hasKey(key)`          | Checks if a key is currently valid                              |
| `size()`               | Returns the number of active entries                            |
| `empty()`              | Checks whether the structure is empty                           |
| `getOffset()`          | Returns the offset used for key indexing                        |
| `getMaxKeyBound()`     | Returns the highest usable key                                   |
| `getName()` / `setName(name)` | Gets or sets a human-readable name                    |
| `enableDynamicResizing()` | Enables doubling when structure fills                        |
| `disableDynamicResizing(purge)` | Disables dynamic resizing, optionally purging buffers |
| `clear()`              | Resets the structure (O(1) with efficient reuse of memory)       |
| `swap(key1, key2)`     | Swaps the values between two keys                               |
| `enableQueue()` / `disableQueue()` | Enables or disables overflow queuing              |
| `getQueue()`           | Returns reference to the overflow queue                         |
| `clearQueue()`         | Clears the overflow queue                                        |
| `getQueueSize()`       | Returns the number of elements in the queue                     |

## ⚠️ Linear Time Operations (Θ(n))
These operations may traverse the entire structure:

| Function        | Description                                   |
|------------------|-----------------------------------------------|
| `contains(value)`| Returns true if value is found (linear scan)  |

## Remarks
- Keys are always recycled efficiently thanks to `KeyPool`, avoiding gaps.
- Structures support full range iteration (`begin()` / `end()`).
- Dynamic resizing works via incremental copy and seamless handover.
