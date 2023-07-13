//
// Created by Prabhleen Kaur on 2023-07-13.
//

#include <iostream>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <vector>
#include <cassert>

void Test2(){



}

int main(){
    DSModelTreeImpl tree;
    tree.SetFreeSlots(200 , 100);
    tree.SetFreeSlots(100, 50);
    tree.SetFreeSlots(300, 80);
    tree.SetFreeSlots(400, 200);


    // Test case 1: When exact match is there
    int requiredCapacity1 = 80;
    auto iter1 = tree.GetBestFitIp(requiredCapacity1);
    assert(iter1.has_value());

    tree.UpdateFreeSlotsList(*iter1, requiredCapacity1);

    // Verify the updated free slots list
    std::vector<TreeMapValueObjectForUnusedObjectInArray> updatedFreeSlots1 = tree.GetFreeSlotsList();
    assert(updatedFreeSlots1.size() == 3);
    assert(updatedFreeSlots1[0].GetCapacity() == 50);
    assert(updatedFreeSlots1[0].GetStartIP() == 100);
    assert(updatedFreeSlots1[1].GetCapacity() == 100);
    assert(updatedFreeSlots1[1].GetStartIP() == 200);
    assert(updatedFreeSlots1[2].GetCapacity() == 200);
    assert(updatedFreeSlots1[2].GetStartIP() == 400);
    std::cout << "Test case 1 passed!" << std::endl;


    // Test case 2: When required capacity is less than the actual capacity
    int requiredCapacity2 = 80;
    auto iter2 = tree.GetBestFitIp(requiredCapacity2);
    assert(iter2.has_value());

    tree.UpdateFreeSlotsList(*iter2, requiredCapacity2);

    // Verify the updated free slots list
    std::vector<TreeMapValueObjectForUnusedObjectInArray> updatedFreeSlots2 = tree.GetFreeSlotsList();
    assert(updatedFreeSlots2.size() == 3);
    assert(updatedFreeSlots2[0].GetCapacity() == 20);
    assert(updatedFreeSlots2[0].GetStartIP() == 280);
    assert(updatedFreeSlots2[1].GetCapacity() == 50);
    assert(updatedFreeSlots2[1].GetStartIP() == 100);
    assert(updatedFreeSlots2[2].GetCapacity() == 200);
    assert(updatedFreeSlots2[2].GetStartIP() == 400);
    std::cout << "Test case 2 passed!" << std::endl;

    return 0;
}