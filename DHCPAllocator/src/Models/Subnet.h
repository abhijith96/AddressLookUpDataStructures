//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_SUBNET_H
#define DHCPALLOCATOR_SUBNET_H

#include <DHCPAllocator/src/Models/IPAddress.h>
#include <DHCPAllocator/src/Models/MacID.h>
#include <DHCPAllocator/src/Models/Host.h>


#include <vector>
#include <unordered_map>
#include <boost/container/flat_map.hpp>

class Subnet {

private:
    MacID mac_id_;
    ip_t capacity_;
    ip_t last_assigned_host_ip_;
    std::vector<ip_t> unused_host_ips_;
    boost::container::flat_map<ip_t, Host> hosts_;
    std::unordered_map<MacID, ip_t, HashMacId, EqualsMacId> host_mac_ip_map_;

public:
    explicit Subnet(MacID mac_id, int capacity, ip_t last_assigned_host_ip);

    [[nodiscard]] MacID GetMacId() const;

    [[nodiscard]] ip_t GetCapacity() const;

    [[nodiscard]] ip_t GetLastAssignedHostIp() const;

    void SetLastAssignedHostIp(ip_t last_assigned_host_ip);

    std::vector<ip_t> &GetUnusedHostIps();

    [[nodiscard]] boost::container::flat_map<ip_t, Host> &GetHosts();

    std::unordered_map<MacID, ip_t, HashMacId, EqualsMacId> &GetHostMacIpMap();

    std::unordered_map<MacID, ip_t, HashMacId, EqualsMacId> GetHostMacIpMapCopy() const;

};

#endif //DHCPALLOCATOR_SUBNET_H
