//
// Created by Sreedevi R Warrier on 2023-07-16.
//


#include <iostream>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <cassert>
#include <DHCPAllocator/src/Models/DSModelArrayImpl/DSModelArrayImpl.h>

int main(){
    DSModelArrayImpl impl;

    // Test case 1: Required capacity is not the exact match, but a capacity higher than the required capacity is available
    int requiredCapacity1 = 1000;
    auto result1 = impl.InsertSubnet(MacID(1), requiredCapacity1);
    if (result1.first) {
        std::cout << "Insert subnet returned start IP: " << result1.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result1.first);

    // Test case 2: Required capacity is not the exact match, but a capacity higher than the required capacity is available
    int requiredCapacity2 = 1000;
    auto result2 = impl.InsertSubnet(MacID(1), requiredCapacity1);
    if (result2.first) {
        std::cout << "Insert subnet returned start IP: " << result2.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result2.first);

    // Test case 3: Delete subnet with given start ip = 0, leaving a free slot
    ip_t start_ip = 0;
    bool result3 = impl.DeleteSubnet(start_ip);
    if (result3) {
        std::cout << "Delete subnet succeeded" << std::endl;
    } else {
        std::cout << "Delete subnet failed." << std::endl;
    }
    assert(result3);

    // Test case 2: Required capacity is not the exact match, but a capacity higher than the required capacity is available
    int requiredCapacity4 = 1000;
    auto result4 = impl.InsertSubnet(MacID(1), requiredCapacity4);
    if (result4.first) {
        std::cout << "Insert subnet returned start IP: " << result4.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result4.first);
}