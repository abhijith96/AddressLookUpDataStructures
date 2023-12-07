//
// Created by Abhijith  K A on 17/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELHASHEDVEBTREEIMPL_H
#define DHCPALLOCATOR_DSMODELHASHEDVEBTREEIMPL_H

#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VebTreeWithHashMap/VEBTreeWithHashMap.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelmpl.h>
#include <IPAddressLookUpDataStructures/src/Models/SubnetHosts.h>
#include <IPAddressLookUpDataStructures/src/Models/IpRange.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTreeValueObject.h>
#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VebTreeWithHashMapFreeSlotObject.h>
#include <unordered_map>


class DSModelHashedVebTreeImpl : public DSModelmpl {
private:
    IpRange ipRange_;
    VEBTreeWithHashMap<VEBTreeValueObject> vebTreeWithHashMap_;
    std::unordered_map<ip_t, SubnetHosts> subnetToHostsMap_;
    VEBTreeWithHashMap<VebTreeWithHashMapFreeSlotObject> freeSlotsList_;
    ip_t freeIpStartRange_;

    ip_t ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(ip_t ipInIpRange);

    ip_t ConvertIpAddressFromVebTreeToIpRangeAddressSpace(ip_t ipInVebTree);

    std::pair<bool,std::reference_wrapper<VebTreeWithHashMapFreeSlotObject>> FindBestFit(ip_t requiredCapacity);

    void UpdateFreeSlotList(ip_t newStartIp, ip_t remainingCapacity);


public:

    [[nodiscard]] const IpRange& GetIpRange() const;

    explicit DSModelHashedVebTreeImpl(IpRange ipRange);
    DSModelHashedVebTreeImpl& operator = (DSModelHashedVebTreeImpl&& other) noexcept ;
    DSModelHashedVebTreeImpl(DSModelHashedVebTreeImpl&& other) noexcept;
    std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, ip_t capacity) override;

    std::pair<bool, ip_t> InsertSubnetHost(MacID hostMacId, ip_t subnetIp) override;

    bool DeleteSubnet(ip_t start_ip) override;




    bool DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) override;


    std::pair<bool, ip_t> DeleteHostFromSubnet(MacID host_mac, ip_t subnet_ip);

    std::pair<bool, ip_t> GetNetWorkIP(ip_t hostIp) override;

    std::pair<bool, ip_t> GetHostIpAddress(MacID macId, ip_t subnet_ip) override;

    std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) override;

    ~DSModelHashedVebTreeImpl() override;

};


#endif //DHCPALLOCATOR_DSMODELHASHEDVEBTREEIMPL_H
