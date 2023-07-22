//
// Created by Sreedevi R Warrier on 2023-07-16.
//

#ifndef DHCPALLOCATOR_ARRAYIMPL_TEST3_H
#define DHCPALLOCATOR_ARRAYIMPL_TEST3_H


#include <iostream>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <cassert>
#include <DHCPAllocator/src/Models/DSModelArrayImpl/DSModelArrayImpl.h>

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
    result = impl.InsertSubnet(MacID(2), requiredCapacity);
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
    requiredCapacity = 200;
    result = impl.InsertSubnet(MacID(3), requiredCapacity);
    if (result.first) {
        std::cout << "Insert subnet returned start IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result.first && result.second == 0);


    // Test case 5: Required capacity is not the exact match, but a capacity higher than the required capacity is available in the free slot
    requiredCapacity = 400;
    result = impl.InsertSubnet(MacID(4), requiredCapacity);
    if (result.first) {
        std::cout << "Insert subnet returned start IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }
    assert(result.first && result.second == 200);

    // currently added subnets
    // subnet 1 -> 0 - 199
    // subnet 2 -> 200 - 599
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

    //Test IP routing
    auto get_network_ip_result = impl.GetNetWorkIP(subnet_1_host_1_ip);
    if (get_network_ip_result.first) {
        std::cout << "GetNetWorkIP returned network IP: " << get_network_ip_result.second << std::endl;
    } else {
        std::cout << "GetNetWorkIP failed." << std::endl;
    }
    assert(get_network_ip_result.first && get_network_ip_result.second == subnet1_start_ip);

    //Test IP routing

    // Testing IP routing with IP within range unassigned to any subnets
    get_network_ip_result = impl.GetNetWorkIP(800);
    if (get_network_ip_result.first) {
        std::cout << "GetNetWorkIP returned network IP: " << get_network_ip_result.second << std::endl;
    } else {
        std::cout << "GetNetWorkIP returned false for IP not assigned to any subnet." << std::endl;
    }
    assert(!get_network_ip_result.first);

    // Testing IP routing with IP = network IP
    get_network_ip_result = impl.GetNetWorkIP(subnet1_start_ip);
    if (get_network_ip_result.first) {
        std::cout << "GetNetWorkIP returned network IP: " << get_network_ip_result.second << std::endl;
    } else {
        std::cout << "GetNetWorkIP failed." << std::endl;
    }
    assert(get_network_ip_result.first && get_network_ip_result.second == subnet1_start_ip);

    // Testing IP routing with IP = broadcast IP
    ip_t broadcast_ip = subnet1_start_ip + 200 - 1; // 200 = capacity
    get_network_ip_result = impl.GetNetWorkIP(broadcast_ip);
    if (get_network_ip_result.first) {
        std::cout << "GetNetWorkIP returned network IP: " << get_network_ip_result.second << std::endl;
    } else {
        std::cout << "GetNetWorkIP failed." << std::endl;
    }
    assert(get_network_ip_result.first && get_network_ip_result.second == subnet1_start_ip);

    get_network_ip_result = impl.GetNetWorkIP(subnet_1_host_2_ip);
    if (get_network_ip_result.first) {
        std::cout << "GetNetWorkIP returned network IP: " << get_network_ip_result.second << std::endl;
    } else {
        std::cout << "GetNetWorkIP failed." << std::endl;
    }
    assert(get_network_ip_result.first && get_network_ip_result.second == subnet1_start_ip);

    // Test case 6: Add host 1 to subnet 1
    ip_t subnet2_start_ip = 200;
    MacID subnet_2_host_1_mac_id = MacID(201);
    result = impl.InsertSubnetHost(subnet_2_host_1_mac_id, subnet2_start_ip);
    if (result.first) {
        std::cout << "Insert subnet host returned host IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet host failed." << std::endl;
    }
    assert(result.first && result.second == 201);

    //save the host id to test the upcoming cases
    ip_t subnet_2_host_1_ip = result.second;

    // Test case 9: Add host 2 to subnet 1
    MacID subnet_2_host_2_mac_id = MacID(202);
    result = impl.InsertSubnetHost(subnet_2_host_2_mac_id, subnet2_start_ip);
    if (result.first) {
        std::cout << "Insert subnet host returned host IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet host failed." << std::endl;
    }
    assert(result.first && result.second == 202);

    //save the host id to test the upcoming cases
    ip_t subnet_2_host_2_ip = result.second;

    //Test IP routing
    get_network_ip_result = impl.GetNetWorkIP(subnet_2_host_1_ip);
    if (get_network_ip_result.first) {
        std::cout << "GetNetWorkIP returned network IP: " << get_network_ip_result.second << std::endl;
    } else {
        std::cout << "GetNetWorkIP failed." << std::endl;
    }
    assert(get_network_ip_result.first && get_network_ip_result.second == subnet2_start_ip);

    //Test IP routing
    get_network_ip_result = impl.GetNetWorkIP(subnet_2_host_2_ip);
    if (get_network_ip_result.first) {
        std::cout << "GetNetWorkIP returned network IP: " << get_network_ip_result.second << std::endl;
    } else {
        std::cout << "GetNetWorkIP failed." << std::endl;
    }
    assert(get_network_ip_result.first && get_network_ip_result.second == subnet2_start_ip);

    auto new_assignments = impl.optimizeSubnetAllocationSpace();

    for (const auto& subnet_pair : new_assignments) {
        MacID subnet_mac_id = subnet_pair.first;
        auto subnet = subnet_pair.second;
        ip_t subnet_start_ip = subnet.first;
        std::cout << "Inform subnet with MAC ID - " << subnet_mac_id.GetValue() << " about new IP assigned - "
                  << subnet_start_ip << std::endl;

        for (const auto &host: subnet.second) {
            MacID host_macId = host.first;
            ip_t host_ip = host.second;
            std::cout << "Inform host with MAC ID - " << host_macId.GetValue() << "in subnet - " << subnet_start_ip
                      << " about new IP assigned - " << host_ip << std::endl;
        }
    }

//    // Test case 7:  Get host IP using MAC ID
//    auto host_ip_result = impl.GetMacAddressOfHost(1, 0);
//    if (host_ip_result.first) {
//        std::cout << "Get IP returned for host with MAC ID: " << host_ip_result.second.GetValue() << std::endl;
//    } else {
//        std::cout << "Get IP returned for host failed." << std::endl;
//    }
//    assert(host_ip_result.first);

}

#endif