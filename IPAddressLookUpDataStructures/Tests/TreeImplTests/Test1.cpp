//
// Created by Abhijith  K A on 11/07/23.
//


#include <iostream>
#include <IPAddressLookUpDataStructures/src/Models/DSModelSingleton.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelTreeImpl/DSModelTreeImpl.h>
#include <vector>
#include <cassert>

int main(){
    DSModelTreeImpl tree;

    // Test case 1: Required capacity is not the exact match, but a capacity higher than the required capacity is available
    int requiredCapacity1 = 1000;
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> result1 = tree.GetBestFitIp(requiredCapacity1);
    if (result1.has_value()) {
        std::cout << "Result 1: " << (*result1)->GetCapacity() << std::endl;
    } else {
        std::cout << "Result 1 is empty." << std::endl;
    }
    assert(result1.has_value());
    tree.UpdateFreeSlotsList(*result1, requiredCapacity1);
    std::vector<TreeMapValueObjectForUnusedObjectInArray> freeSlots1 = tree.GetFreeSlotsList();
    std::cout << "Free space after allocating subnet with capacity 1000: " << freeSlots1[0].GetCapacity() << std::endl;
    std::cout << "==================================================================" << std::endl;

    // Test case 2: Exact Required capacity is available in the list
    int requiredCapacity2 = 2147482647;
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> result2 = tree.GetBestFitIp(requiredCapacity2);
    if (result2.has_value()) {
        std::cout << "Result 2: " << (*result2)->GetCapacity() << std::endl;
    } else {
        std::cout << "Result 2 is empty." << std::endl;
    }
    assert(result2.has_value());
    assert((*result2)->GetCapacity() == requiredCapacity2);
    tree.UpdateFreeSlotsList(*result2, requiredCapacity2);
    std::vector<TreeMapValueObjectForUnusedObjectInArray> freeSlots2 = tree.GetFreeSlotsList();
    std::cout << "Free spaces after allocating subnet with capacity 2147482647: " << freeSlots2.size() << std::endl;
    assert(freeSlots2.empty());
    std::cout << "==================================================================" << std::endl;

    // adding free slots to the list
    tree.SetFreeSlots(200 , 100);
    tree.SetFreeSlots(100, 50);
    tree.SetFreeSlots(400, 200);

    // Test case 3: Required capacity is not available in the list
    int requiredCapacity3 = 300;
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> result3 = tree.GetBestFitIp(requiredCapacity3);
    if (result3.has_value()) {
        std::cout << "Result 3: " << (*result3)->GetCapacity() << std::endl;
    } else {
        std::cout << "Result 3 is empty." << std::endl;
    }
    assert(!result3.has_value());
    std::cout << "==================================================================" << std::endl;

    // Test case 4: Required capacity is not the exact match, but a capacity higher than the required capacity is available
    int requiredCapacity4 = 150;
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> result4 = tree.GetBestFitIp(requiredCapacity4);
    if (result4.has_value()) {
        std::cout << "Result 4: " << (*result4)->GetCapacity() << std::endl;
    } else {
        std::cout << "Result 4 is empty." << std::endl;
    }
    assert(result4.has_value());
    assert((*result4)->GetCapacity() > requiredCapacity4);
    tree.UpdateFreeSlotsList(*result4, requiredCapacity4);
    std::vector<TreeMapValueObjectForUnusedObjectInArray> freeSlots4 = tree.GetFreeSlotsList();
    std::cout << "Free spaces after allocating subnet with capacity 150: " << freeSlots4.size() << std::endl;
    assert(freeSlots4.size() == 3);
    std::cout << "==================================================================" << std::endl;

    std::cout << "All test cases passed!" << std::endl;

    return 0;
}