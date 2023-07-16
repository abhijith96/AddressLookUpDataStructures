//
// Created by Abhijith  K A on 13/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELVEBTREEIMPL_H
#define DHCPALLOCATOR_DSMODELVEBTREEIMPL_H

#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTree/VEBTreeNode.h>
#include <DHCPAllocator/src/Models/DSModelTreeImpl/DSModelTreeImpl.h>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTree/VEBTreeMap.h>
#include <DHCPAllocator/src/Models/IpRange.h>
#include "DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTreeValueObject.h"

constexpr uint32_t universe_size = std::numeric_limits<int32_t>::max();
class DSModelVebTreeImpl : public DSModelmpl {
private:
    IpRange ipRange_;
    VEBTreeMap<VEBTreeValueObject> vebTreeMap_;
    ip_t freeIpStartRange_;

private:
    ip_t ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(ip_t ipInIpRange);

    ip_t ConvertIpAddressFromVebTreeToIpRangeAddressSpace(ip_t ipInVebTree);

public:

   explicit DSModelVebTreeImpl(IpRange ipRange);

    std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, int capacity) override;

    std::pair<bool, ip_t> InsertSubnetHost(MacID hostMacId, ip_t subnetIp) override;

    void DeleteSubnet(ip_t start_ip) override;

    void DeleteHostFromSubnet(ip_t host_ip);

    std::pair<bool, ip_t> GetNetWorkIP(ip_t hostIp);

    std::pair<bool, ip_t> GetIpAddress(MacID macId);

    MacID GetMacAddressOfHost(ip_t hostIpAddress);

    ~DSModelVebTreeImpl();

    void DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) override;

    std::pair<bool, ip_t> GetHostIpAddress(MacID macId, ip_t subnet_ip) override;

    std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) override;


};


#endif //DHCPALLOCATOR_DSMODELVEBTREEIMPL_H
