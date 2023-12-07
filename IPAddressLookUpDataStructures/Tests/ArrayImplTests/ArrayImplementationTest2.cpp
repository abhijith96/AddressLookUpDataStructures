//
// Created by Sreedevi R Warrier on 2023-07-16.
//


#ifndef DHCPALLOCATOR_ARRAYIMPL_TEST2_H
#define DHCPALLOCATOR_ARRAYIMPL_TEST2_H

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
    requiredCapacity = 4;
    result = impl.InsertSubnet(MacID(1), requiredCapacity);
    if (result.first) {
        std::cout << "Insert subnet returned start IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result.first && result.second == 1000);

    // currently added subnets
    // subnet 1 -> 0 - 1000
    // subnet 3 -> 1000 - 1003

    // Test case 6: Add host 1 to subnet 1
    ip_t subnet1_start_ip = 1000;
    MacID subnet_1_host_1_mac_id = MacID(11);
    result = impl.InsertSubnetHost(subnet_1_host_1_mac_id, subnet1_start_ip);
    if (result.first) {
        std::cout << "Insert subnet host returned host IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet host failed." << std::endl;
    }
    assert(result.first && result.second == 1001);

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
    assert(result.first && result.second == 1002);

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


    // Test case 9: Add host 2 to subnet 1
    MacID subnet_1_host_3_mac_id = MacID(13);
    result = impl.InsertSubnetHost(subnet_1_host_3_mac_id, subnet1_start_ip);
    if (result.first) {
        std::cout << "Insert subnet host passed." << std::endl;
    } else {
        std::cout << "Insert subnet host failed due to capacity exceeded." << std::endl;
    }
    assert(!result.first);

    // Test case 12:  Delete host from subnet
    auto delete_host_result = impl.DeleteHostFromSubnet(subnet_1_host_1_ip, subnet1_start_ip);
    if (delete_host_result) {
        std::cout << "Delete host returned success for host IP: " << subnet_1_host_1_ip << " - " <<  subnet1_start_ip << std::endl;
    } else {
        std::cout << "Delete host failed." << std::endl;
    }
    assert(delete_host_result);


    // Test case 11:  Get host mac ID using host IP
    mac_id_result = impl.GetMacAddressOfHost(subnet_1_host_2_ip, subnet1_start_ip);
    if (mac_id_result.first) {
        std::cout << "Get MAC ID returned for host IP: " << subnet_1_host_2_ip << " - " <<  mac_id_result.second.GetValue() << std::endl;
    } else {
        std::cout << "Get MAC ID returned for host failed." << std::endl;
    }
    assert(mac_id_result.first && mac_id_result.second.GetValue() == subnet_1_host_2_mac_id.GetValue());


    // Test case 9: Add host 2 to subnet 1
    auto insert_host_result = impl.InsertSubnetHost(subnet_1_host_3_mac_id, subnet1_start_ip);
    if (insert_host_result.first) {
        std::cout << "Get IP returned for host with MAC ID: " << subnet_1_host_3_mac_id.GetValue() << " - " << insert_host_result.second << std::endl;
    } else {
        std::cout << "Get IP returned for host failed." << std::endl;
    }
    assert(insert_host_result.first && insert_host_result.second == 1001);
    ip_t subnet_1_host_3_ip = insert_host_result.second;


    // Test case 11:  Get host mac ID using host IP
    mac_id_result = impl.GetMacAddressOfHost(subnet_1_host_3_ip, subnet1_start_ip);
    if (mac_id_result.first) {
        std::cout << "Get MAC ID returned for host IP: " << subnet_1_host_3_ip << " - " <<  mac_id_result.second.GetValue() << std::endl;
    } else {
        std::cout << "Get MAC ID returned for host failed." << std::endl;
    }
    assert(mac_id_result.first && mac_id_result.second.GetValue() == subnet_1_host_3_mac_id.GetValue());

}

#endif