
# 🔑 KeyArray

**KeyArray** is a high-performance data structure that combines the speed of array-based access with the flexibility of dynamic key management.  
It is designed to guarantee **O(1)** time complexity (worst-case) for insert, remove, access, and key existence operations – even without hashing or rebalancing.

---

## ⚡ Why KeyArray?

Unlike hash maps, vectors, or linked lists, `KeyArray` is optimized for use-cases where:
- Keys are known to be integers within a known or expandable range.
- You require **worst-case constant time** for inserts and deletions.
- You want complete control over memory layout, capacity, and resizing behavior.
- You prefer using integers as external keys rather than relying on iterators or internal references.

---

## 🚀 Features

- ✅ **O(1)** `insert`, `remove`, `hasKey`, and `at()` – in the worst case.
- ✅ Optional **dynamic resizing** using copy-on-insert strategy.
- ✅ **Overflow queue** for inserts when full and dynamic resizing is disabled.
- ✅ Human-readable `name` tag for easier debugging and saving.
- ✅ Save/load from file with full structural recovery.
- ✅ Optional key offset to support shifted key spaces.
- ✅ Fully iterator-compatible (range-based `for` loops).
- ✅ C++17 compliant and header-only.

---

## 📊 Comparison with Other Data Structures

| Operation           | KeyArray (fixed) |   KeyArray (dynamic)    | `std::unordered_map` | `std::map` |    `std::vector`    |
|---------------------|------------------|-------------------------|----------------------|------------|---------------------|
| Insert              | ✅**O(1)**       | ✅**O(1)** (amortized) | O(1) avg             | O(log n)   | ✅O(1) (amortized) |
| Remove              | ✅**O(1)**       | ✅**O(1)**             | O(1) avg             | O(log n)   | O(n)                |
| Access by key       | ✅**O(1)**       | ✅**O(1)**             | O(1) avg             | O(log n)   | ✅O(1)             |
| Check key existence | ✅**O(1)**       | ✅**O(1)**             | O(1) avg             | O(log n)   | O(n)                |
| Iterate             | ✅O(n)           | ✅O(n)                 | ✅O(n)               | ✅O(n)    | ✅O(n)             |
| Clear structure     | ✅O(1)           | ✅O(1)                 | O(n)                 | O(n)       | O(n)                |

---

## 🧠 Design Principles

- **KeyPool** manages available keys using a stack for efficient reuse.
- **Offset support** allows key shifting (e.g., from 100 to 199).
- **Dynamic resizing** uses copy-on-insert strategy, preparing the next array in the background while using the current one.
- **Overflow queue** ensures you don’t lose values even when dynamic resizing is disabled.
- When dynamic resizing is enabled, there's no need to manually migrate to a larger structure – it grows seamlessly.

---

## 🧪 Example

```cpp
#include "KeyArray.hpp"
#include <iostream>
#include <string>

int main() {
    KeyArray<std::string> arr("MyArray");

    int k1 = arr.insert("apple");
    int k2 = arr.insert("banana");

    arr.at(k2) = "blueberry";

    std::cout << arr.getName() << " contains " << arr.size() << " elements:
";
    std::cout << "Key " << k1 << ": " << arr.at(k1) << "\n";
    std::cout << "Key " << k2 << ": " << arr.at(k2) << "\n";

    if (arr.hasKey(k1)) {
        arr.remove(k1);
    }

    arr.enableDynamicResizing(); // auto-resizes when needed
}
```

---

## 🧩 When to Use

Use `KeyArray` when:
- You want **fast, predictable, and guaranteed O(1)** time for key-based operations.
- You’re mapping external numeric IDs (like handles, sockets, or user indices).
- You want an **alternative to hash maps** without worrying about collisions or hash function overhead.
- You’re managing **fixed-capacity or dynamically resizable pools** (e.g., connection pools, game entities).

---

## 🔐 Advanced Use Cases

- You can use **transformed keys** for security or uniqueness:
  ```cpp
  int safeKey = userID * 997 + 3187;
  ```
- Keys returned from `insert()` can be stored externally and safely reused.
- You can simulate memory-efficient sparse maps by shifting offsets and reusing deleted keys.

---

## 📁 File Structure

```
KeyArray/
├── include/
│   ├── KeyArray.hpp
│   ├── KeyArrayBase.hpp
│   └── KeyPool.hpp
├── examples/
│   └── example.cpp
├── README.md
├── EXPLANATIONS.md
├── GITHUB_TEMPLATE.md
├── .gitignore
```

---

## 🛠️ Build

To compile and run:

```bash
g++ -std=c++17 -Iinclude examples/example.cpp -o example.exe
.\example.exe
```
➡️ See [`example.cpp`](./example.cpp) for a full demonstration of how to use KeyArray in fixed, dynamic, and overflow modes.

---

## 📄 License

MIT License – Free for personal and commercial use.

---
## 🤝 Feedback, Collaboration, and Support

Have a question, idea, or suggestion?  
Feel free to [open an issue](https://github.com/eliShif/KeyArray/issues) — I'm happy to hear feedback and improve this project.

### 💼 Looking to Collaborate?

If you're interested in expanding this project, integrating it into your system, or discussing ways to work together — feel free to reach out via [GitHub](https://github.com/eliShif).

---

> ✅ Built for performance. Designed for clarity. Ready for extension.


