//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#include "DSModelArrayImpl.h"
#include <iostream>

std::pair<bool, ip_t> DSModelArrayImpl::InsertSubnet(MacID sub_net_mac_id, int capacity) {
    if(capacity < 3) { //todo check if the check has to be <2 or <3
        // every subnet requires 2 reserved IPs for subnet IP and broadcast IP
        return {false, 0};
    } else {
        // getting the best free slot available
        auto required_slot_iter_optional = GetBestFitIp(capacity);

        if (required_slot_iter_optional.has_value()) {

            auto iterator = *required_slot_iter_optional;

            ip_t start_ip = iterator->GetStartIP();

            subnet_routing_map_.insert({start_ip, ArrayValueObject(capacity, sub_net_mac_id)});
            subnets_.insert({start_ip, Subnet(sub_net_mac_id, capacity, start_ip)});
            subnet_mac_ip_map_.insert({sub_net_mac_id, start_ip});

            ip_t new_start_ip = start_ip + capacity;
            UpdateFreeSlotsList(iterator, capacity);

            return {true, start_ip};

        } else {
            // no slot available for the given capacity
            return {false, 0};
        }
    }
}

std::pair<bool, ip_t> DSModelArrayImpl::InsertSubnetHost(MacID host_mac_id, ip_t subnet_ip) {

    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        Subnet &subnet = it->second;
        return InsertHost(subnet, host_mac_id, subnet_ip);
    }
    return {false, 0};
}

bool DSModelArrayImpl::DeleteSubnet(ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        // remove from hosts list in subnet object
        Subnet &subnet = it->second;
        MacID macId = subnet.GetMacId();
        subnet_mac_ip_map_.erase(macId); //remove subnet mac id to ip mapping
        subnet_routing_map_.erase(subnet_ip);
        SetFreeSlots(subnet_ip, subnet.GetCapacity());
        subnets_.erase(subnet_ip);
        return true;
    } else
        return false;

}

bool DSModelArrayImpl::DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {

        // remove from hosts list in subnet object
        Subnet &subnet = it->second;
        boost::container::flat_map<ip_t, Host> &hosts = subnet.GetHosts();

        auto host_it = hosts.find(host_ip);
        if (host_it != hosts.end()) {

            // get mac id before deletion so as to remove mac to ip map
            Host host = host_it->second;
            MacID host_mac_id = host.GetMacID();

            //delete host entry from hosts list
            hosts.erase(host_ip);

            //add the removed host ip entry to unused host ip list
            std::vector<ip_t> &unused_host_ips = subnet.GetUnusedHostIps();
            unused_host_ips.push_back(host_ip);

            //remove mac id to ip mapping
            auto &mac_to_ip_map = subnet.GetHostMacIpMap();
            mac_to_ip_map.erase(host_mac_id);

            return true;
        }
    }
    return false; // if subnet with given start ip
}

std::pair<bool, ip_t> DSModelArrayImpl::GetNetWorkIP(ip_t hostIp) {

    auto subnet_it = subnet_routing_map_.upper_bound(hostIp);  // get the first subnet IP greater than hostIP (binary search on flat map)

    if (subnet_it != subnet_routing_map_.begin()) {
        --subnet_it;  // get the previous subnet IP
        ip_t subnet_ip = subnet_it->first;
        ArrayValueObject arrayValueObject = subnet_it->second;

        if (hostIp < subnet_ip + arrayValueObject.GetCapacity() ) {
//            auto subnet_element_it = subnets_.find(subnet_ip);
//            if (subnet_element_it != subnets_.end()) {
//                Subnet &subnet = subnet_element_it->second;
//                auto &hosts = subnet.GetHosts();
//                auto host_it = hosts.find(hostIp); //check if host IP is assigned to any host
//                if (host_it != hosts.end()) {
//                    return { true, subnet_ip };
//                }
//            }
            return { true, subnet_ip };
        }
    }
    return {false, 0};
}

std::pair<bool, ip_t> DSModelArrayImpl::GetHostIpAddress(MacID macId, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {

        // remove from hosts list in subnet object
        Subnet &subnet = it->second;

        // get host mac to ip map from the subnet
        auto &host_mac_ip_map = subnet.GetHostMacIpMap();
        auto host_it = host_mac_ip_map.find(macId);

        if (host_it != host_mac_ip_map.end()) {
            // get the host ip mapped to the host mac ID
            ip_t host_ip = host_it->second;
            return {true, host_ip};
        }
    }

    // return false if subnet ip does not exist or when the host mac does not exist in the given subnet
    return {false, 0};

}

std::pair<bool, MacID> DSModelArrayImpl::GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        Subnet &subnet = it->second;
        boost::container::flat_map<ip_t, Host> &hosts = subnet.GetHosts();
        auto host_it = hosts.find(hostIpAddress);
        if (host_it != hosts.end()) {
            Host host = host_it->second;
            return {true, host.GetMacID()};
        } else {
            return {false, MacID(0)};
        }
    } else {
        return {false, MacID(0)};
    }
}

std::optional<std::vector<FreeSlotObject>::iterator> DSModelArrayImpl::GetBestFitIp(int requiredCapacity) {

    auto compare = [](const FreeSlotObject& arrayItem, const FreeSlotObject& requiredItem) {
        return arrayItem.GetCapacity() < requiredItem.GetCapacity();
    };

    FreeSlotObject dummyForRequiredCapacity{0, requiredCapacity};
    auto iter = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(), dummyForRequiredCapacity, compare);

    if (iter != free_slots_list_.end() && iter->GetCapacity() >= requiredCapacity) {
        return iter;
    } else {
        return std::optional<std::vector<FreeSlotObject>::iterator>{};
    }

}

void DSModelArrayImpl::UpdateFreeSlotsList(std::vector<FreeSlotObject>::iterator iter, int requiredCapacity) {
    if (iter->GetCapacity() == requiredCapacity) {
        free_slots_list_.erase(iter);
    } else {
        int newCapacity = iter->GetCapacity() - requiredCapacity;
        ip_t newStartIp = iter->GetStartIP() + requiredCapacity;
        free_slots_list_.erase(iter);
        SetFreeSlots(newStartIp, newCapacity);
    }
}

[[maybe_unused]] void DSModelArrayImpl::SetFreeSlots(ip_t startIp, int freeCapacity) {
    FreeSlotObject free_slot = FreeSlotObject(startIp, freeCapacity);

    auto compare = [] (const FreeSlotObject& arrayItem, const FreeSlotObject requiredItem){
        return arrayItem.GetCapacity() < requiredItem.GetCapacity();
    };


    auto insertPos = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(), free_slot,compare);
    free_slots_list_.insert(insertPos, free_slot);
}

std::pair<bool, ip_t>  DSModelArrayImpl::InsertHost(Subnet &subnet, MacID host_mac_id, ip_t subnet_ip) {

    auto &hosts = subnet.GetHosts();

    int subnet_capacity = subnet.GetCapacity();
    ip_t broadcast_ip = subnet_ip + subnet_capacity - 1;

    ip_t last_assigned_host_ip = subnet.GetLastAssignedHostIp();
    ip_t host_ip = last_assigned_host_ip + 1;

    //check if the host ip is within the subnet range
    if (host_ip < broadcast_ip) { // -1 to ignore the broadcast address
        //put the entry to hosts
        hosts.insert({host_ip, Host(host_ip, host_mac_id, false)});

        add_host_mac_ip_mapping(subnet, host_ip, host_mac_id); // add host mac id to ip mapping

        subnet.SetLastAssignedHostIp(host_ip);

        return {true, host_ip};

    } else {
        std::vector<ip_t> & unused_host_ips = subnet.GetUnusedHostIps();
        //check if any ip is not assigned in hosts
        int unused_hosts_size = unused_host_ips.size();

        if (unused_hosts_size == 0) { // no ip left to assign

            return {false, 0};
        } else {

            host_ip = unused_host_ips.back(); //get an unused host ip from list
            hosts.insert({host_ip, Host(host_ip, host_mac_id, false)});
            unused_host_ips.pop_back();  //remove the assigned host ip from unused list

            add_host_mac_ip_mapping(subnet, host_ip, host_mac_id); // add host mac id to ip mapping

            return {true, host_ip};
        }
    }
}

void DSModelArrayImpl::add_host_mac_ip_mapping(Subnet &subnet, ip_t host_ip, MacID host_mac_id) {
    // add host mac id to ip mapping

    auto &mac_ip_map = subnet.GetHostMacIpMap();
    mac_ip_map.insert({host_mac_id, host_ip});
}

std::unordered_map<MacID, std::pair<ip_t, std::unordered_map<MacID,ip_t,HashMacId, EqualsMacId>>, HashMacId, EqualsMacId> DSModelArrayImpl::optimizeSubnetAllocationSpace() {

    std::unordered_map<MacID, std::pair<ip_t, std::unordered_map<MacID,ip_t,HashMacId, EqualsMacId>>, HashMacId, EqualsMacId> new_assignments;

    //make copy of subnets (it has all the details)
    auto  subnets_copy = subnets_;

    //clear all subnet maps
    subnets_.clear();
    subnet_routing_map_.clear();
    subnet_mac_ip_map_.clear();
    free_slots_list_.clear();
    free_slots_list_.insert(free_slots_list_.begin(), FreeSlotObject(0, std::numeric_limits<int32_t>::max()));

    //loop through the subnets_copy and recreate the subnets without leaving any free slots in between
    for (const auto& pair : subnets_copy) {
        ip_t network_start_ip = pair.first;
        Subnet subnet = pair.second;
        MacID subnet_mac_id = subnet.GetMacId();
        int subnet_capacity = subnet.GetCapacity();

//        std::cout << "Creating subnet with MAC ID - " << subnet_mac_id.GetValue() << " old network ip - " << network_start_ip << " capacity - " << subnet_capacity << std::endl;

        auto host_mac_ip_map_copy = subnet.GetHostMacIpMap();

        //create subnets with required capacity
        auto insert_subnet_response = InsertSubnet(subnet_mac_id, subnet_capacity);
        if (insert_subnet_response.first) {
            ip_t new_subnet_ip = insert_subnet_response.second;
//            std::cout << "Created subnet with MAC ID - " << subnet_mac_id.GetValue() << " new IP - " << new_subnet_ip << std::endl;

            std::unordered_map<MacID, ip_t, HashMacId, EqualsMacId> new_host_assignments;

            // iterate and create the subnet hosts
            for (const auto &host_pair: host_mac_ip_map_copy) {

                MacID host_mac_id = host_pair.first;
//                std::cout << "Creating host " << host_mac_id.GetValue() << " in subnet - "<< new_subnet_ip << std::endl;
                auto insert_subnet_host_response = InsertSubnetHost(host_mac_id, new_subnet_ip);

                if (insert_subnet_host_response.first) {
                    ip_t new_host_ip = insert_subnet_host_response.second;
//                    std::cout << "Created host with MAC ID - " << host_mac_id.GetValue() << " in subnet - " << new_subnet_ip << " new IP - " << new_host_ip << std::endl;

                    new_host_assignments.insert({host_mac_id, new_host_ip}); //insert the newly assigned IP to the map

                }
                new_assignments.insert({subnet_mac_id, {new_subnet_ip, new_host_assignments}}); //insert the newly assigned IP to the map
            }
        }
    }
    return new_assignments;
}
