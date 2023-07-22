//
// Created by Abhijith  K A on 11/07/23.
//

#include "Subnet.h"

#include "vector"

Subnet::Subnet(MacID macId, int capacity, ip_t last_assigned_host_ip) :
        mac_id_(macId), capacity_(capacity), last_assigned_host_ip_(last_assigned_host_ip){
}

MacID Subnet::GetMacId() const {
    return mac_id_;
}

ip_t Subnet::GetCapacity() const {
    return capacity_;
}

ip_t Subnet::GetLastAssignedHostIp() const {
    return last_assigned_host_ip_;
}

void Subnet::SetLastAssignedHostIp(ip_t last_assigned_host_ip) {
    last_assigned_host_ip_ = last_assigned_host_ip;
}

std::vector<ip_t> & Subnet::GetUnusedHostIps()  {
    return unused_host_ips_;
}

boost::container::flat_map<ip_t , Host> & Subnet::GetHosts() {
    return hosts_;
}

std::unordered_map<MacID, ip_t, HashMacId, EqualsMacId> &Subnet::GetHostMacIpMap() {
    return host_mac_ip_map_;
}

std::unordered_map<MacID, ip_t, HashMacId, EqualsMacId> Subnet::GetHostMacIpMapCopy() const {
    return host_mac_ip_map_;
}

