//
// Created by Sreedevi R Warrier on 2023-07-16.
//

#ifndef DHCPALLOCATOR_ARRAYIMPL_TEST1_H
#define DHCPALLOCATOR_ARRAYIMPL_TEST1_H

#include <iostream>
#include <IPAddressLookUpDataStructures/src/Models/DSModelSingleton.h>
#include <cassert>
#include <IPAddressLookUpDataStructures/src/Models/DSModelArrayImpl/DSModelArrayImpl.h>

int main() {
    DSModelArrayImpl impl;

    // Test case 1: Required capacity is not the exact match, but a capacity higher than the required capacity is available
    int requiredCapacity = 1000;
    auto result = impl.InsertSubnet(MacID(1), requiredCapacity);
    if (result.first) {
        std::cout << "Insert subnet returned start IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result.first && result.second == 0);

    // Test case 2: Required capacity is not the exact match, but a capacity higher than the required capacity is available
    requiredCapacity = 1000;
    result = impl.InsertSubnet(MacID(1), requiredCapacity);
    if (result.first) {
        std::cout << "Insert subnet returned start IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result.first && result.second == 1000);
    
    // Test case 3: Delete subnet with given start ip = 0, leaving a free slot
    ip_t start_ip = 0;
    bool result3 = impl.DeleteSubnet(start_ip);
    if (result3) {
        std::cout << "Delete subnet succeeded" << std::endl;
    } else {
        std::cout << "Delete subnet failed." << std::endl;
    }
    assert(result3);

    // Test case 4: Required capacity is not the exact match, but a capacity higher than the required capacity is available in the free slot
    requiredCapacity = 500;
    result = impl.InsertSubnet(MacID(1), requiredCapacity);
    if (result.first) {
        std::cout << "Insert subnet returned start IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result.first && result.second == 0);


    // Test case 5: Required capacity is not the exact match, but a capacity higher than the required capacity is available in the free slot
    requiredCapacity = 500;
    result = impl.InsertSubnet(MacID(1), requiredCapacity);
    if (result.first) {
        std::cout << "Insert subnet returned start IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result.first && result.second == 500);

    // currently added subnets
    // subnet 1 -> 0 - 499
    // subnet 2 -> 500 - 999
    // subnet 3 -> 1000 - 1999

    // Test case 6: Add host 1 to subnet 1
    ip_t subnet1_start_ip = 0;
    MacID subnet_1_host_1_mac_id = MacID(11);
    result = impl.InsertSubnetHost(subnet_1_host_1_mac_id, subnet1_start_ip);
    if (result.first) {
        std::cout << "Insert subnet host returned host IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet host failed." << std::endl;
    }
    assert(result.first && result.second == 1);

    //save the host id to test the upcoming cases
    ip_t subnet_1_host_1_ip = result.second;

    // Test case 7:  Get host IP using MAC ID
    auto host_ip_result = impl.GetHostIpAddress(subnet_1_host_1_mac_id, subnet1_start_ip);
    if (host_ip_result.first) {
        std::cout << "Get IP returned for host with MAC ID: " << subnet_1_host_1_mac_id.GetValue() << " - " << host_ip_result.second << std::endl;
    } else {
        std::cout << "Get IP returned for host failed." << std::endl;
    }
    assert(host_ip_result.first && host_ip_result.second == subnet_1_host_1_ip);

    // Test case 8:  Get host mac ID using host IP
    auto mac_id_result = impl.GetMacAddressOfHost(subnet_1_host_1_ip, subnet1_start_ip);
    if (mac_id_result.first) {
        std::cout << "Get MAC ID returned for host IP: " << subnet_1_host_1_ip << " - " << mac_id_result.second.GetValue() << std::endl;
    } else {
        std::cout << "Get MAC ID returned for host failed." << std::endl;
    }
    assert(mac_id_result.first && mac_id_result.second.GetValue() == subnet_1_host_1_mac_id.GetValue());

    // Test case 9: Add host 2 to subnet 1
    MacID subnet_1_host_2_mac_id = MacID(12);
    result = impl.InsertSubnetHost(subnet_1_host_2_mac_id, subnet1_start_ip);
    if (result.first) {
        std::cout << "Insert subnet host returned host IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet host failed." << std::endl;
    }
    assert(result.first && result.second == 2);

    //save the host id to test the upcoming cases
    ip_t subnet_1_host_2_ip = result.second;

    // Test case 10:  Get host IP using MAC ID
    host_ip_result = impl.GetHostIpAddress(subnet_1_host_2_mac_id, subnet1_start_ip);
    if (host_ip_result.first) {
        std::cout << "Get IP returned for host with MAC ID: " << subnet_1_host_2_mac_id.GetValue() << " - " << host_ip_result.second << std::endl;
    } else {
        std::cout << "Get IP returned for host failed." << std::endl;
    }
    assert(host_ip_result.first && host_ip_result.second == subnet_1_host_2_ip);

    // Test case 11:  Get host mac ID using host IP
    mac_id_result = impl.GetMacAddressOfHost(subnet_1_host_2_ip, subnet1_start_ip);
    if (mac_id_result.first) {
        std::cout << "Get MAC ID returned for host IP: " << subnet_1_host_2_ip << " - " <<  mac_id_result.second.GetValue() << std::endl;
    } else {
        std::cout << "Get MAC ID returned for host failed." << std::endl;
    }
    assert(mac_id_result.first && mac_id_result.second.GetValue() == subnet_1_host_2_mac_id.GetValue());

    //todo test insert host case after host deletion

    // Test case 12:  Delete host from subnet
    auto delete_host_result = impl.DeleteHostFromSubnet(subnet_1_host_1_ip, subnet1_start_ip);
    if (delete_host_result) {
        std::cout << "Delete host returned success for host IP: " << subnet_1_host_1_ip << " - " <<  subnet1_start_ip << std::endl;
    } else {
        std::cout << "Delete host failed." << std::endl;
    }
    assert(delete_host_result);

}

#endif