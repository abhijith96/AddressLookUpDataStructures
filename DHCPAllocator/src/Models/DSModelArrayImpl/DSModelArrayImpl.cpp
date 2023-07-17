//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#include "DSModelArrayImpl.h"

std::pair<bool, ip_t> DSModelArrayImpl::InsertSubnet(MacID sub_net_mac_id, ip_t capacity) {

    std::optional<std::vector<FreeSlotObject>::iterator> required_slot_iter_optional = GetBestFitIp(capacity);

    if(required_slot_iter_optional.has_value()){

        auto iterator = *required_slot_iter_optional;

        ip_t start_ip = iterator->GetStartIP();

        subnet_routing_map_.insert({start_ip, ArrayValueObject(capacity, sub_net_mac_id)});
        subnets_.insert({start_ip, Subnet(sub_net_mac_id, capacity, start_ip)});
        subnet_mac_ip_map_.insert({sub_net_mac_id, start_ip});

        ip_t new_start_ip = start_ip + capacity;

        UpdateFreeSlotsList(iterator, capacity, new_start_ip);

        return {true, start_ip};

    } else {

        return {false, 0};
    }
}

std::pair<bool, ip_t> DSModelArrayImpl::InsertSubnetHost(MacID host_mac_id, ip_t subnet_ip) {

    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        Subnet subnet = it->second;
        return InsertHost(subnet, host_mac_id, subnet_ip);
    }
    return {false, 0};
}

void DSModelArrayImpl::DeleteSubnet(ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {
        // remove from hosts list in subnet object
        Subnet subnet = it->second;
        MacID macId = subnet.GetMacId();
        subnet_mac_ip_map_.erase(macId); //remove subnet mac id to ip mapping
        subnet_routing_map_.erase(subnet_ip);
//        SetFreeSlots(subnet_ip, subnet.GetCapacity());//todo insert to free slot
    }
    subnets_.erase(subnet_ip);
}

void DSModelArrayImpl::DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {

        // remove from hosts list in subnet object
        Subnet subnet = it->second;
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
        }
    }

}

std::pair<bool, ip_t> DSModelArrayImpl::GetNetWorkIP(ip_t hostIp) {
    return {false, 0}; //todo main
}

std::pair<bool, ip_t> DSModelArrayImpl::GetHostIpAddress(MacID macId, ip_t subnet_ip) {
    auto it = subnets_.find(subnet_ip);
    if (it != subnets_.end()) {

        // remove from hosts list in subnet object
        Subnet subnet = it->second;

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

std::optional<std::vector<FreeSlotObject>::iterator> DSModelArrayImpl::GetBestFitIp(int requiredCapacity) {

    auto compare = [] (const FreeSlotObject& arrayItem, const FreeSlotObject requiredItem){
        return arrayItem.GetCapacity() < requiredItem.GetCapacity();
    };

    FreeSlotObject dummyForRequiredCapacity{0,requiredCapacity};

    auto iter = std::lower_bound(
            free_slots_list_.begin(), free_slots_list_.end(),dummyForRequiredCapacity, compare
    );
    if(iter == free_slots_list_.end()){
        if(requiredCapacity > free_slots_list_.back().GetCapacity()){
            return std::optional<std::vector<FreeSlotObject>::iterator>{};
        }
        else{
            return free_slots_list_.begin();
        }
    }
    else {

        if(iter->GetCapacity() == requiredCapacity) {
            return iter;
        }
        else{
            ++iter;
            if(iter != free_slots_list_.end()){
                return iter;
            }
            return std::optional<std::vector<FreeSlotObject>::iterator>{};
        }
    }

}

void DSModelArrayImpl::UpdateFreeSlotsList(std::vector<FreeSlotObject>::iterator iter,
                                          int requiredCapacity, ip_t newStartIp) {
    if(iter->GetCapacity() == requiredCapacity){
        free_slots_list_.erase(iter);

    }

    else{
        int newCapacity = iter->GetCapacity() - requiredCapacity;
        FreeSlotObject newIpSlot{newStartIp, newCapacity};
        free_slots_list_.erase(iter);

        auto compare = [] (const FreeSlotObject& arrayItem, const FreeSlotObject requiredItem){
            return arrayItem.GetCapacity() < requiredItem.GetCapacity();
        };


        auto iterToElementBeforeNewItem = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(), newIpSlot,
                                                           compare);
        free_slots_list_.insert(iterToElementBeforeNewItem + 1, newIpSlot);

    }
}

void DSModelArrayImpl::getFreeIPInSubnet(boost::container::flat_map<ip_t, Host> map) {

}

std::pair<bool, ip_t>  DSModelArrayImpl::InsertHost(Subnet subnet, MacID host_mac_id, ip_t subnet_ip) {

    boost::container::flat_map<ip_t , Host> & hosts = subnet.GetHosts();

    int subnet_capacity = subnet.GetCapacity();
    ip_t broadcast_ip = subnet_ip + subnet_capacity - 1;

    ip_t last_assigned_host_ip = subnet.GetLastAssignedHostIp();
    ip_t host_ip = last_assigned_host_ip + 1;

    //check if the host ip is within the subnet range
    if (host_ip < broadcast_ip) { // -1 to ignore the broadcast address
        //put the entry to hosts
        hosts.insert({host_ip, Host(host_ip, host_mac_id, false)});

        add_host_mac_ip_mapping(subnet, host_ip, host_mac_id); // add host mac id to ip mapping

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

void DSModelArrayImpl::add_host_mac_ip_mapping(Subnet subnet, ip_t host_ip, MacID host_mac_id) {
    // add host mac id to ip mapping

    auto & mac_ip_map = subnet.GetHostMacIpMap();
    mac_ip_map.insert({host_mac_id, host_ip});
}