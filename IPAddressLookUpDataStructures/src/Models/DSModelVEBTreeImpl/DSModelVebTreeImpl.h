//
// Created by Abhijith  K A on 13/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELVEBTREEIMPL_H
#define DHCPALLOCATOR_DSMODELVEBTREEIMPL_H

#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTree/VEBTreeNode.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelTreeImpl/DSModelTreeImpl.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTree/VEBTreeMap.h>
#include <IPAddressLookUpDataStructures/src/Models/IpRange.h>
#include <IPAddressLookUpDataStructures/src/Models/SubnetHosts.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTreeValueObject.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/VebTreeFreeSlotsObject.h>
#include <unordered_map>
#include <boost/container/flat_set.hpp>

constexpr uint32_t universe_size = std::numeric_limits<int32_t>::max();
class DSModelVebTreeImpl : public DSModelmpl {
private:
    IpRange ipRange_;
    VEBTreeMap<VEBTreeValueObject> vebTreeMap_;
    std::unordered_map<ip_t,SubnetHosts> subnetToHostsMap_;
    ip_t freeIpStartRange_;
    boost::container::flat_set<VebTreeFreeSlotsObject, CompareVebTreeFreeSlotsObject> freeSubNetSlots_;

private:
    ip_t ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(ip_t ipInIpRange);

    ip_t ConvertIpAddressFromVebTreeToIpRangeAddressSpace(ip_t ipInVebTree);

   std::pair<bool, boost::container::flat_set<VebTreeFreeSlotsObject>::iterator> FindBestFit(ip_t requiredCapacity);


public:

   explicit DSModelVebTreeImpl(IpRange ipRange);

    std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, ip_t capacity) override;

    std::pair<bool, ip_t> InsertSubnetHost(MacID hostMacId, ip_t subnetIp) override;

    bool DeleteSubnet(ip_t start_ip) override;

    std::pair<bool, ip_t> GetNetWorkIP(ip_t hostIp) override;

    const IpRange& GetIpRange() const;





    ~DSModelVebTreeImpl();

    bool DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) override;

    std::pair<bool, ip_t> DeleteHostFromSubnet(MacID host_mac, ip_t subnet_ip);

    std::pair<bool, ip_t> GetHostIpAddress(MacID macId, ip_t subnet_ip) override;

    std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) override;


};


#endif //DHCPALLOCATOR_DSMODELVEBTREEIMPL_H
