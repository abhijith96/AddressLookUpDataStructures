//
// Created by Abhijith  K A on 11/07/23.
//


#include <iostream>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <vector>
#include <cassert>

#include <gtest/gtest.h>

void Test1(){



}

int main(){
    DSModelTreeImpl tree;
    tree.SetFreeSlots(200 , 100);
    tree.SetFreeSlots(100, 50);
    tree.SetFreeSlots(400, 200);
//    tree.SetFreeSlots(20, 300);
//    tree.InsertSubnet(MacID{10}, 10);

    // Test case 1: Required capacity is available in the list
    int requiredCapacity1 = 100;
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> result1 = tree.GetBestFitIp(requiredCapacity1);
    if (result1.has_value()) {
        std::cout << "Result 1: " << (*result1)->GetCapacity() << std::endl;
    } else {
        std::cout << "Result 1 is empty." << std::endl;
    }
    assert(result1.has_value());
    assert((*result1)->GetCapacity() == requiredCapacity1);

    // Test case 2: Required capacity is not available in the list
    int requiredCapacity2 = 150;
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> result2 = tree.GetBestFitIp(requiredCapacity2);
    if (result2.has_value()) {
        std::cout << "Result 2: " << (*result2)->GetCapacity() << std::endl;
    } else {
        std::cout << "Result 2 is empty." << std::endl;
    }
    assert(result2.has_value());

    // Test case 3: Required capacity is greater than any available slot
    int requiredCapacity3 = 300;
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> result3 = tree.GetBestFitIp(requiredCapacity3);
    if (result3.has_value()) {
        std::cout << "Result 3: " << (*result3)->GetCapacity() << std::endl;
    } else {
        std::cout << "Result 3 is empty." << std::endl;
    }
    assert(!result3.has_value());

    // Test case 4: Required capacity is smaller than the smallest slot
    int requiredCapacity4 = 50;
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> result4 = tree.GetBestFitIp(requiredCapacity4);
    if (result4.has_value()) {
        std::cout << "Result 4: " << (*result4)->GetCapacity() << std::endl;
    } else {
        std::cout << "Result 4 is empty." << std::endl;
    }
    assert(result4.has_value());
    assert((*result4)->GetCapacity() >= requiredCapacity4);

    std::cout << "All test cases passed!" << std::endl;

    return 0;
}