//
// Created by Prabhleen Kaur on 2023-07-13.
//

#include <iostream>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <vector>
#include <cassert>

int main(){
    DSModelTreeImpl tree;

    // Test case 1: When required capacity is less than the actual capacity
    int requiredCapacity1 = 1000;
    auto iter2 = tree.GetBestFitIp(requiredCapacity1);
    assert(iter2.has_value());

    tree.UpdateFreeSlotsList(*iter2, requiredCapacity1);

    // Verify the updated free slots list
    std::vector<TreeMapValueObjectForUnusedObjectInArray> updatedFreeSlots1 = tree.GetFreeSlotsList();
    assert(updatedFreeSlots1.size() == 1);
    assert(updatedFreeSlots1[0].GetCapacity() == 2147482647);
    assert(updatedFreeSlots1[0].GetStartIP() == 1000);
    std::cout << "Test case 1 passed!" << std::endl;

    // Test case 1: When exact match is there
    int requiredCapacity2 = 2147482647;
    auto iter1 = tree.GetBestFitIp(requiredCapacity2);
    assert(iter1.has_value());

    tree.UpdateFreeSlotsList(*iter1, requiredCapacity2);

    // Verify the updated free slots list
    std::vector<TreeMapValueObjectForUnusedObjectInArray> updatedFreeSlots2 = tree.GetFreeSlotsList();
    assert(updatedFreeSlots2.empty());
    std::cout << "Test case 2 passed!" << std::endl;

    return 0;
}