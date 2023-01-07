#include <iostream>
#include "maps/ArrayMap.h"

int main() {
    maps::ArrayMap<int, int> map = maps::ArrayMap<int, int>([](int key) { return key; }, [](int a, int b) { return a == b; });
    map.put(25, 69);
    map.put(50, 420);
    map.put(150, 1337);

    // tests

    for (maps::Node<int, int>& node : map) {
        std::cout << "Key(" << node.mKey << ") = " << node.mVal << std::endl;
    }

    std::cout << "Removing 50..." << std::endl;
    map.remove(50);

    for (maps::Node<int, int>& node : map) {
        std::cout << "Key(" << node.mKey << ") = " << node.mVal << std::endl;
    }

    std::cout << "Replacing key 25 with value 2020..." << std::endl;
    map.put(25, 2020);

    for (maps::Node<int, int>& node : map) {
        std::cout << "Key(" << node.mKey << ") = " << node.mVal << std::endl;
    }

    return 0;
}
