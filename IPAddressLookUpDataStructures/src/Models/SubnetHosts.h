//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_SUBNETHOSTS_H
#define DHCPALLOCATOR_SUBNETHOSTS_H

#include <vector>
#include <unordered_map>
#include <IPAddressLookUpDataStructures/src/Models/IPAddress.h>
#include <IPAddressLookUpDataStructures/src/Models/MacID.h>



class SubnetHosts {
private:
    ip_t nextFreeIpAvailable_;
    ip_t capacity_;
    std::vector<ip_t> freeHostSlotsList_;
    std::unordered_map<MacID, ip_t, HashMacId, EqualsMacId> hostMacToIpMap_;
public:
    explicit SubnetHosts(ip_t subnetIp, ip_t capacity);
    std::pair<bool, ip_t> InsertHost(ip_t subnetIp, MacID hostMacID);
    std::pair<bool, ip_t> DeleteHost(ip_t subnetIp, MacID hostMacId);
    std::pair<bool, ip_t> GetHostIp(ip_t subnetIp, MacID hostMacId);
    std::pair<bool, MacID> GetHostMacId(ip_t subnetIp, ip_t hostIp);

    [[nodiscard]] ip_t GetCapacity() const;

};


#endif //DHCPALLOCATOR_SUBNETHOSTS_H
