//
// Created by Abhijith  K A on 13/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELVEBTREEIMPL_H
#define DHCPALLOCATOR_DSMODELVEBTREEIMPL_H

#include "DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTree/VEBTreeNode.h"
#include "DHCPAllocator/src/Models/DSModelTreeImpl/DSModelTreeImpl.h"

constexpr uint32_t universe_size = std::numeric_limits<int32_t>::max();
class DSModelVebTreeImpl : public DSModelmpl{
private:


public:

   explicit DSModelVebTreeImpl();

    ip_t InsertSubnet(MacID subNetMacId, int capacity) override;

    ip_t InsertSubnetHost(MacID hostMacId, ip_t subnetIp) override;

    void DeleteSubnet(ip_t start_ip) override;

    void DeleteHostFromSubnet(ip_t host_ip) override;

    ip_t GetNetWorkIP(ip_t hostIp) override;

    ip_t GetIpAddress(MacID macId) override;

    MacID GetMacAddressOfHost(ip_t hostIpAddress) override;

    ~DSModelVebTreeImpl() override;


};


#endif //DHCPALLOCATOR_DSMODELVEBTREEIMPL_H
