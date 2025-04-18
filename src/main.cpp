#include "KeyArray.hpp"
#include <iostream>
#include <string>

int main() {
    KeyArray<std::string> arr("MyArray");

    std::cout << "EXPECTED OUTPUT: Name: MyArray\n";
    std::cout << "Name: " << arr.getName() << "\n";

    std::cout << "EXPECTED OUTPUT: Empty: 1\n";
    std::cout << "Empty: " << arr.empty() << "\n";

    int k1 = arr.insert("apple");
    int k2 = arr.insert("banana");
    int k3 = arr.insert("cherry");

    std::cout << "EXPECTED OUTPUT: Inserted keys: 0, 1, 2\n";
    std::cout << "Inserted keys: " << k1 << ", " << k2 << ", " << k3 << "\n";

    std::cout << "EXPECTED OUTPUT: Size after insert: 3\n";
    std::cout << "Size after insert: " << arr.size() << "\n";

    std::cout << "EXPECTED OUTPUT: At 1: banana\n";
    std::cout << "At 1: " << arr.at(1) << "\n";

    arr.at(1) = "blueberry";
    std::cout << "EXPECTED OUTPUT: Modified 1: blueberry\n";
    std::cout << "Modified 1: " << arr.at(1) << "\n";

    std::cout << "EXPECTED OUTPUT: HasKey 0? 1\n";
    std::cout << "HasKey 0? " << arr.hasKey(0) << "\n";

    std::cout << "EXPECTED OUTPUT: HasKey 99? 0\n";
    std::cout << "HasKey 99? " << arr.hasKey(99) << "\n";

    std::cout << "EXPECTED OUTPUT: Contains apple? 1\n";
    std::cout << "Contains apple? " << arr.contains("apple") << "\n";

    arr.swap(0, 2);
    std::cout << "EXPECTED OUTPUT: After swap - At 0: cherry, At 2: apple\n";
    std::cout << "At 0: " << arr.at(0) << "\n";
    std::cout << "At 2: " << arr.at(2) << "\n";

    arr.remove(0);
    std::cout << "EXPECTED OUTPUT: Size after remove: 2\n";
    std::cout << "Size after remove: " << arr.size() << "\n";
    std::cout << "EXPECTED OUTPUT: HasKey 0? 0\n";
    std::cout << "HasKey 0? " << arr.hasKey(0) << "\n";

    arr.clear();
    std::cout << "EXPECTED OUTPUT: Size after clear: 0\n";
    std::cout << "Size after clear: " << arr.size() << "\n";
    std::cout << "EXPECTED OUTPUT: Empty after clear: 1\n";
    std::cout << "Empty after clear: " << arr.empty() << "\n";

    // דינמית
    arr.enableDynamicResizing();

    for (int i = 0; i < 150; ++i) {
        arr.insert("item" + std::to_string(i));
    }

    std::cout << "EXPECTED OUTPUT: Size after dynamic insert: 150\n";
    std::cout << "Size after dynamic insert: " << arr.size() << "\n";
    std::cout << "EXPECTED OUTPUT: Resizing enabled? 1\n";
    std::cout << "Resizing enabled? " << arr.isDynamicResizingEnabled() << "\n";

    // תור
    arr.disableDynamicResizing(true);
    arr.enableQueue();

    for (int i = 0; i < 200; ++i) {
        arr.insert("overflow_" + std::to_string(i));
    }

    std::cout << "EXPECTED OUTPUT: Queue size (overflowed): >0\n";
    std::cout << "Queue size: " << arr.getQueueSize() << "\n";

    arr.clearQueue();
    std::cout << "EXPECTED OUTPUT: Queue size after clear: 0\n";
    std::cout << "Queue size after clear: " << arr.getQueueSize() << "\n";

    return 0;
}
