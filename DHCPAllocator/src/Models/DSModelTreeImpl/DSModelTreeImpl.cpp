//
// Created by Abhijith K A on 11/07/23.
//

#include <iostream>
#include "DSModelTreeImpl.h"

/**
 * Inserts a subnet into the tree.
 *
 * @param sub_net_mac_id The MAC ID of the subnet.
 * @param capacity The capacity of the subnet.
 * @return A pair indicating the success status and the starting IP of the inserted subnet.
 */
std::pair<bool, ip_t> DSModelTreeImpl::InsertSubnet(MacID sub_net_mac_id, int capacity) {
    // Find the required slot in the free slots list
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> requiredSlotItr = GetBestFitIp(capacity);

    print_subnet_routing_tree_();
    if (requiredSlotItr.has_value()) {
        ip_t start_ip = (*requiredSlotItr)->GetStartIP();
        UpdateFreeSlotsList(*requiredSlotItr, capacity);

        // Insert the subnet into the routing tree
        subnet_routing_tree_.insert({start_ip, TreeMapValueObject(capacity, sub_net_mac_id)});
        subnets_.insert({start_ip, Subnet(sub_net_mac_id, capacity, start_ip)});
        subnet_mac_ip_map_.insert({sub_net_mac_id, start_ip});

        return {true, start_ip};
    } else {
        return {false, 0};
    }
}

/**
 * Inserts a host into a subnet.
 *
 * @param host_mac_id The MAC ID of the host.
 * @param subnet_ip The IP of the subnet.
 * @return A pair indicating the success status and the IP of the inserted host.
 */
std::pair<bool, ip_t> DSModelTreeImpl::InsertSubnetHost(MacID host_mac_id, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        Subnet subnet = it->second;
        return InsertHost(subnet, host_mac_id, subnet_ip);
    }
    return {false, 0};
}

/**
 * Deletes a subnet from the tree.
 *
 * @param subnet_ip The IP of the subnet to delete.
 */
bool DSModelTreeImpl::DeleteSubnet(ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        // Remove from hosts list in subnet object
        Subnet subnet = it->second;
        MacID macId = subnet.GetMacId();
        subnet_mac_ip_map_.erase(macId); // Remove subnet MAC ID to IP mapping
        subnet_routing_tree_.erase(subnet_ip);
        SetFreeSlots(subnet_ip, subnet.GetCapacity());
    }
    subnets_.erase(subnet_ip);
    print_subnet_routing_tree_();
}

/**
 * Deletes a host from a subnet.
 *
 * @param host_ip The IP of the host to delete.
 * @param subnet_ip The IP of the subnet.
 */
bool DSModelTreeImpl::DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        // Remove from hosts list in subnet object
        Subnet subnet = it->second;
        boost::container::flat_map<ip_t, Host> &hosts = subnet.GetHosts();
        hosts.erase(host_ip);

        // Add removed host IP to unused host IP list
        std::vector<ip_t> &unused_host_ips = subnet.GetUnusedHostIps();
        unused_host_ips.push_back(host_ip);
    }
}

/**
 * Gets the subnet IP for a given host IP.
 *
 * @param hostIp The IP of the host.
 * @return A pair indicating the success status and the subnet IP of the host.
 */
std::pair<bool, ip_t> DSModelTreeImpl::GetNetWorkIP(ip_t hostIp) {
    auto it = subnet_routing_tree_.upper_bound(hostIp); // Find the first element with a key greater than hostIp

    if (it != subnet_routing_tree_.begin()) {
        --it; // Move the iterator back to the previous element

        ip_t subnet_ip = it->first;
        int capacity = it->second.GetCapacity();
        ip_t upper_bound = subnet_ip + capacity;

        if (hostIp >= subnet_ip && hostIp < upper_bound) {
            // The host IP is within the subnet IP and its capacity range
            return {true, subnet_ip};
        }
    }

    // Host IP is not found in any subnet
    return {false, 0};
}


/**
 * Gets the IP address of a host in a subnet based on its MAC ID.
 *
 * @param macId The MAC ID of the host.
 * @param subnet_ip The IP of the subnet.
 * @return A pair indicating the success status and the IP address of the host.
 */
std::pair<bool, ip_t> DSModelTreeImpl::GetHostIpAddress(MacID macId, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        // Remove from hosts list in subnet object
        Subnet subnet = it->second;
        boost::container::flat_map<ip_t, Host> &hosts = subnet.GetHosts();
        // TODO: Figure out if required or not
        // If yes, figure out how to do (need to iterate and get using MAC ID)
    }
    return {false, 0};
}

/**
 * Gets the MAC address of a host based on its IP address and subnet IP.
 *
 * @param hostIpAddress The IP address of the host.
 * @param subnet_ip The IP of the subnet.
 * @return A pair indicating the success status and the MAC address of the host.
 */
std::pair<bool, MacID> DSModelTreeImpl::GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        Subnet subnet = it->second;
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

/**
 * Inserts a host into a subnet.
 *
 * @param subnet The subnet object.
 * @param host_mac_id The MAC ID of the host.
 * @param subnet_ip The IP of the subnet.
 * @return A pair indicating the success status and the IP address of the inserted host.
 */
std::pair<bool, ip_t> DSModelTreeImpl::InsertHost(Subnet subnet, MacID host_mac_id, ip_t subnet_ip) {
    boost::container::flat_map<ip_t, Host> &hosts = subnet.GetHosts();
    int subnet_capacity = subnet.GetCapacity();
    ip_t broadcast_ip = subnet_ip + subnet_capacity - 1;
    ip_t last_assigned_host_ip = subnet.GetLastAssignedHostIp();
    ip_t host_ip = last_assigned_host_ip + 1;

    // Check if the host IP is within the subnet range
    if (host_ip < broadcast_ip) { // -1 to ignore the broadcast address
        // Put the entry to hosts
        hosts.insert({host_ip, Host(host_ip, host_mac_id, false)});
        add_host_mac_ip_mapping(subnet, host_ip, host_mac_id); // Add host MAC ID to IP mapping
        return {true, host_ip};
    } else {
        std::vector<ip_t> &unused_host_ips = subnet.GetUnusedHostIps();
        // Check if any IP is not assigned in hosts
        int unused_hosts_size = unused_host_ips.size();
        if (unused_hosts_size == 0) { // No IP left to assign
            return {false, 0};
        } else {
            host_ip = unused_host_ips.back(); // Get an unused host IP from the list
            hosts.insert({host_ip, Host(host_ip, host_mac_id, false)});
            unused_host_ips.pop_back();  // Remove the assigned host IP from unused list
            add_host_mac_ip_mapping(subnet, host_ip, host_mac_id); // Add host MAC ID to IP mapping
            return {true, host_ip};
        }
    }
}

/**
 * Adds a host MAC ID to IP mapping in the subnet.
 *
 * @param subnet The subnet object.
 * @param host_ip The IP address of the host.
 * @param host_mac_id The MAC ID of the host.
 */
void DSModelTreeImpl::add_host_mac_ip_mapping(Subnet subnet, ip_t host_ip, MacID host_mac_id) {
    // Add host MAC ID to IP mapping
    auto &mac_ip_map = subnet.GetHostMacIpMap();
    mac_ip_map.insert({host_mac_id, host_ip});
}

/**
 * Gets the iterator of the required slot in the free slots list.
 *
 * @param requiredCapacity The required capacity.
 * @return An iterator to the required slot if found, or an empty optional otherwise.
 */
std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> DSModelTreeImpl::GetBestFitIp(int requiredCapacity) {
    auto compare = [](const TreeMapValueObjectForUnusedObjectInArray& arrayItem, const TreeMapValueObjectForUnusedObjectInArray& requiredItem) {
        return arrayItem.GetCapacity() < requiredItem.GetCapacity();
    };

    TreeMapValueObjectForUnusedObjectInArray dummyForRequiredCapacity{0, requiredCapacity};
    auto iter = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(), dummyForRequiredCapacity, compare);

    if (iter != free_slots_list_.end() && iter->GetCapacity() >= requiredCapacity) {
        return iter;
    } else {
        return std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator>{};
    }
}

/**
 * Updates the free slots list after inserting a subnet.
 *
 * @param iter An iterator to the required slot.
 * @param requiredCapacity The required capacity.
 */
void DSModelTreeImpl::UpdateFreeSlotsList(std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator iter, int requiredCapacity) {
    if (iter->GetCapacity() == requiredCapacity) {
        free_slots_list_.erase(iter);
    } else {
        int newCapacity = iter->GetCapacity() - requiredCapacity;
        ip_t newStartIp = iter->GetStartIP() + requiredCapacity;
        free_slots_list_.erase(iter);
        SetFreeSlots(newStartIp, newCapacity);
    }
}

/**
 * Sets the free slots in the free slots list.
 *
 * @param startIp The starting IP of the free slots.
 * @param freeCapacity The capacity of the free slots.
 */
[[maybe_unused]] void DSModelTreeImpl::SetFreeSlots(ip_t startIp, int freeCapacity) {
    TreeMapValueObjectForUnusedObjectInArray newObj(startIp, freeCapacity);
    auto insertPos = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(), newObj,
                                      [](const TreeMapValueObjectForUnusedObjectInArray& lhs,
                                         const TreeMapValueObjectForUnusedObjectInArray& rhs) {
                                          return lhs.GetCapacity() < rhs.GetCapacity();
                                      });
    free_slots_list_.insert(insertPos, newObj);
}

/**
 * Gets the free slots list.
 *
 * @return The free slots list.
 */
std::vector<TreeMapValueObjectForUnusedObjectInArray> DSModelTreeImpl::GetFreeSlotsList() {
    return free_slots_list_;
}

void DSModelTreeImpl::print_subnet_routing_tree_() {
    if(!subnet_routing_tree_.empty()){
        for (const auto& entry : subnet_routing_tree_) {
            ip_t subnet_ip = entry.first;
            const TreeMapValueObject& value_object = entry.second;
            MacID macId = value_object.GetMacAddress();
            int capacity = value_object.GetCapacity();
            std::cout << "Subnet Ip: " << subnet_ip << std::endl;
            std::cout << "Mac ID: " << macId.GetValue()<< std::endl;
            std::cout << "Capacity: " << capacity << std::endl;
            std::cout << "==================================================================" << std::endl;
        }
    }else{
        std::cout << "subnet_routing_tree_ is empty" << std::endl;
        std::cout << "==================================================================" << std::endl;
    }
}
