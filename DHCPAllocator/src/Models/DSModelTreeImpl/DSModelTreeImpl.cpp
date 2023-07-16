//
// Created by Abhijith  K A on 11/07/23.
//

#include <iostream>
#include "DSModelTreeImpl.h"

std::pair<bool, ip_t> DSModelTreeImpl::InsertSubnet(MacID sub_net_mac_id, int capacity) {
    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> requiredSlotItr = GetBestFitIp(capacity);

    if (requiredSlotItr.has_value()) {
        ip_t start_ip = (*requiredSlotItr)->GetStartIP();
        UpdateFreeSlotsList(*requiredSlotItr, capacity);
        subnet_routing_map_.insert({start_ip, TreeMapValueObject(capacity, sub_net_mac_id)});
        subnets_.insert({start_ip, Subnet(sub_net_mac_id, capacity, start_ip)});
        subnet_mac_ip_map_.insert({sub_net_mac_id, start_ip});
        return {true, start_ip};
    } else {
        return {false, 0};
    }
}

std::pair<bool, ip_t> DSModelTreeImpl::InsertSubnetHost(MacID host_mac_id, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        Subnet subnet = it->second;
        return InsertHost(subnet, host_mac_id, subnet_ip);
    }
    return {false, 0};
}

bool DSModelTreeImpl::DeleteSubnet(ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        // Remove from hosts list in subnet object
        Subnet subnet = it->second;
        MacID macId = subnet.GetMacId();
        subnet_mac_ip_map_.erase(macId); // Remove subnet mac id to ip mapping
        subnet_routing_map_.erase(subnet_ip);
        SetFreeSlots(subnet_ip, subnet.GetCapacity());
    }
    subnets_.erase(subnet_ip);
}

bool DSModelTreeImpl::DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        // Remove from hosts list in subnet object
        Subnet subnet = it->second;
        boost::container::flat_map<ip_t, Host> &hosts = subnet.GetHosts();
        hosts.erase(host_ip);

        // Add removed host ip to unused host ip list
        std::vector<ip_t> &unused_host_ips = subnet.GetUnusedHostIps();
        unused_host_ips.push_back(host_ip);
    }
}

std::pair<bool, ip_t> DSModelTreeImpl::GetNetWorkIP(ip_t hostIp) {
    return {false, 0};
}

std::pair<bool, ip_t> DSModelTreeImpl::GetHostIpAddress(MacID macId, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        // Remove from hosts list in subnet object
        Subnet subnet = it->second;
        boost::container::flat_map<ip_t, Host> &hosts = subnet.GetHosts();
        // TODO: Figure out if required or not
        // If yes, figure out how to do (need to iterate and get using mac id)
    }
    return {false, 0};
}

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

std::pair<bool, ip_t> DSModelTreeImpl::InsertHost(Subnet subnet, MacID host_mac_id, ip_t subnet_ip) {
    boost::container::flat_map<ip_t , Host> & hosts = subnet.GetHosts();
    int subnet_capacity = subnet.GetCapacity();
    ip_t broadcast_ip = subnet_ip + subnet_capacity - 1;
    ip_t last_assigned_host_ip = subnet.GetLastAssignedHostIp();
    ip_t host_ip = last_assigned_host_ip + 1;

    // Check if the host ip is within the subnet range
    if (host_ip < broadcast_ip) { // -1 to ignore the broadcast address
        // Put the entry to hosts
        hosts.insert({host_ip, Host(host_ip, host_mac_id, false)});
        add_host_mac_ip_mapping(subnet, host_ip, host_mac_id); // Add host mac id to ip mapping
        return {true, host_ip};
    } else {
        std::vector<ip_t> &unused_host_ips = subnet.GetUnusedHostIps();
        // Check if any ip is not assigned in hosts
        int unused_hosts_size = unused_host_ips.size();
        if (unused_hosts_size == 0) { // No ip left to assign
            return {false, 0};
        } else {
            host_ip = unused_host_ips.back(); // Get an unused host ip from list
            hosts.insert({host_ip, Host(host_ip, host_mac_id, false)});
            unused_host_ips.pop_back();  // Remove the assigned host ip from unused list
            add_host_mac_ip_mapping(subnet, host_ip, host_mac_id); // Add host mac id to ip mapping
            return {true, host_ip};
        }
    }
}

void DSModelTreeImpl::add_host_mac_ip_mapping(Subnet subnet, ip_t host_ip, MacID host_mac_id) {
    // Add host mac id to ip mapping
    auto &mac_ip_map = subnet.GetHostMacIpMap();
    mac_ip_map.insert({host_mac_id, host_ip});
}

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

[[maybe_unused]] void DSModelTreeImpl::SetFreeSlots(ip_t startIp, int freeCapacity) {
    TreeMapValueObjectForUnusedObjectInArray newObj(startIp, freeCapacity);
    auto insertPos = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(), newObj,
                                      [](const TreeMapValueObjectForUnusedObjectInArray& lhs,
                                         const TreeMapValueObjectForUnusedObjectInArray& rhs) {
                                          return lhs.GetCapacity() < rhs.GetCapacity();
                                      });
    free_slots_list_.insert(insertPos, newObj);
}

std::vector<TreeMapValueObjectForUnusedObjectInArray> DSModelTreeImpl::GetFreeSlotsList() {
    return free_slots_list_;
}
