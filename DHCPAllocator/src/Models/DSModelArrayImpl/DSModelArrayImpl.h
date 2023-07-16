//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#ifndef DHCP_DSMODELARRAYIMPL_H
#define DHCP_DSMODELARRAYIMPL_H


#include "DHCPAllocator/src/Models/SubnetHost.h"
#include "DHCPAllocator/src/Models/DSModelmpl.h"
#include "DHCPAllocator/src/Models/MacID.h"
#include "DHCPAllocator/src/Models/IPAddress.h"
#include <DHCPAllocator/src/Models/Subnet.h>

#include <DHCPAllocator/src/Models/DSModelArrayImpl/ArrayValueObject.h>
#include <DHCPAllocator/src/Models/DSModelArrayImpl/FreeSlotObject.h>

#include <boost/container/flat_map.hpp>

#include <map>
#include <vector>
#include <limits>


class DSModelArrayImpl : public DSModelmpl {

    boost::container::flat_map <ip_t , ArrayValueObject> subnet_routing_map_;
    std::unordered_map<ip_t , Subnet> subnets_;
    std::unordered_map<MacID , ip_t , HashMacId, EqualsMacId> subnet_mac_ip_map_;

    std::vector<FreeSlotObject> free_slots_list_ {FreeSlotObject(0, std::numeric_limits<int32_t>::max())};

private:

public:

    DSModelArrayImpl() = default;

    std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, int capacity) override;

    std::pair<bool, ip_t> InsertSubnetHost(MacID host_mac_id, ip_t subnet_ip) override;

    bool DeleteSubnet(ip_t subnet_ip) override;

    bool DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) override;

    std::pair<bool, ip_t> GetNetWorkIP(ip_t hostIp) override;

    std::pair<bool, ip_t> GetHostIpAddress(MacID macId, ip_t subnet_ip) override;

    std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) override;

    void getFreeIPInSubnet(boost::container::flat_map<ip_t, Host> map);

    std::pair<bool, ip_t>  InsertHost(Subnet &subnet, MacID host_mac_id, ip_t subnet_ip);

    void add_host_mac_ip_mapping(Subnet &subnet, ip_t ip, MacID id);

    std::optional<std::vector<FreeSlotObject>::iterator> GetBestFitIp(int requiredCapacity);

    void UpdateFreeSlotsList(std::vector<FreeSlotObject>::iterator iter, int requiredCapacity);

    [[maybe_unused]] void SetFreeSlots(ip_t startIp, int freeCapacity);

    std::vector<FreeSlotObject> GetFreeSlotsList();

};


#endif //DHCP_DSMODELARRAYIMPL_H
