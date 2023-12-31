//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#ifndef DHCP_DSMODELARRAYIMPL_H
#define DHCP_DSMODELARRAYIMPL_H


#include <IPAddressLookUpDataStructures/src/Models/SubnetHosts.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelmpl.h>
#include <IPAddressLookUpDataStructures/src/Models/MacID.h>
#include <IPAddressLookUpDataStructures/src/Models/IPAddress.h>
#include <IPAddressLookUpDataStructures/src/Models/Subnet.h>

#include <IPAddressLookUpDataStructures/src/Models/DSModelArrayImpl/ArrayValueObject.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelArrayImpl/FreeSlotObject.h>

#include <boost/container/flat_map.hpp>

#include <map>
#include <vector>
#include <limits>


class DSModelArrayImpl : public DSModelmpl {

    boost::container::flat_map<ip_t, ArrayValueObject> subnet_routing_map_;
    std::unordered_map<ip_t, Subnet> subnets_;
    std::unordered_map<MacID, ip_t, HashMacId, EqualsMacId> subnet_mac_ip_map_;

    std::vector<FreeSlotObject> free_slots_list_{FreeSlotObject(0, std::numeric_limits<int32_t>::max())};

private:

public:

    DSModelArrayImpl() = default;

    std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, ip_t capacity) override;

    std::pair<bool, ip_t> InsertSubnetHost(MacID host_mac_id, ip_t subnet_ip) override;

    bool DeleteSubnet(ip_t subnet_ip) override;

    bool DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) override;

    std::pair<bool, ip_t> GetNetWorkIP(ip_t hostIp) override;

    std::pair<bool, ip_t> GetHostIpAddress(MacID macId, ip_t subnet_ip) override;

    std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) override;

    bool RequestHostRenewal(ip_t hostIpAddress, ip_t subnet_ip) ;

    bool DeleteNonRenewedHosts();

    void getFreeIPInSubnet(boost::container::flat_map<ip_t, Host> map);

    std::pair<bool, ip_t> InsertHost(Subnet &subnet, MacID host_mac_id, ip_t subnet_ip);

    void add_host_mac_ip_mapping(Subnet &subnet, ip_t ip, MacID id);

    std::optional<std::vector<FreeSlotObject>::iterator> GetBestFitIp(ip_t requiredCapacity);

    void UpdateFreeSlotsList(std::vector<FreeSlotObject>::iterator iter, ip_t requiredCapacity);

    [[maybe_unused]] void SetFreeSlots(ip_t startIp, ip_t freeCapacity);

    std::vector<FreeSlotObject> GetFreeSlotsList();

    std::unordered_map<MacID, std::pair<ip_t, std::unordered_map<MacID,ip_t,HashMacId, EqualsMacId>>, HashMacId, EqualsMacId> optimizeSubnetAllocationSpace();

};

#endif //DHCP_DSMODELARRAYIMPL_H
