
# 🔧 KeyArray — Efficient Key-Based Structure

A C++ data structure that behaves like a map with integer keys but guarantees **Θ(1)** worst-case time for all major operations, including insertion, deletion, access, and key reuse.

## 📦 Features
- Constant-time insert, remove, access, and size
- Auto key management via a KeyPool (no gaps)
- Dynamic resizing using background copying
- Optional overflow queue support
- Full iteration and named instances

## 📈 Time Complexity (Worst-Case)
| Operation     | Time    |
|---------------|---------|
| insert        | Θ(1)    |
| remove        | Θ(1)    |
| at(key)       | Θ(1)    |
| hasKey(key)   | Θ(1)    |
| size          | Θ(1)    |
| clear         | Θ(1)    |
| contains(val) | Θ(n)    |

📌 *See `EXPLANATIONS.md` for details on every method.*

## 🧠 When to Use
Use KeyArray when:
- You need fast access and insert by integer keys
- You want to avoid STL `unordered_map` or `map` overhead
- You want safe, sparse key usage with constant-time recycling

## 📁 File Structure
- `KeyArray.hpp` — Main class with full functionality
- `KeyArrayBase.hpp` — Core logic for fixed-sized version
- `KeyPool.hpp` — Lightweight internal key recycler
- `README.md` — Overview and usage
- `EXPLANATIONS.md` — Method-by-method complexity

## ✅ Example
```cpp
KeyArray<std::string> users("UserList");
int id = users.insert("Alice");
users.remove(id);
```

---
Created with ❤️ by [Your Name]
