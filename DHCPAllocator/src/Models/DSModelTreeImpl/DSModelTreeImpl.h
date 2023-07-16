//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELTREEIMPL_H
#define DHCPALLOCATOR_DSMODELTREEIMPL_H

#include "DHCPAllocator/src/Models/Subnet.h"
#include "DHCPAllocator/src/Models/SubnetHost.h"
#include "DHCPAllocator/src/Models/DSModelmpl.h"
#include "DHCPAllocator/src/Models/MacID.h"
#include "DHCPAllocator/src/Models/IPAddress.h"

#include <DHCPAllocator/src/Models/DSModelTreeImpl/TreeMapValueObject.h>
#include <DHCPAllocator/src/Models/DSModelTreeImpl/TreeMapValueObjectForUnusedObjectInArray.h>

#include <boost/container/flat_map.hpp>

#include <map>
#include <vector>
#include <limits>


class DSModelTreeImpl : public DSModelmpl {
//std::map<ip_t,TreeMapValueObject> treemap_;
//std::vector<TreeMapValueObjectForUnusedObjectInArray> free_slots_list_;

    boost::container::flat_map <ip_t , TreeMapValueObject> subnet_routing_map_;
    std::unordered_map<ip_t , Subnet> subnets_;
    std::unordered_map<MacID , ip_t , HashMacId, EqualsMacId> subnet_mac_ip_map_;

    std::vector<TreeMapValueObjectForUnusedObjectInArray> free_slots_list_ {TreeMapValueObjectForUnusedObjectInArray(0, std::numeric_limits<int32_t>::max())};

public:

    DSModelTreeImpl() = default;
    /**
    * Pure virtual function hence zero
    * @param subnet
    * returns starting ip address of subnet inserted
    */
    std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, int capacity) override;


    /**
     * Give an ip address for a host in the given subnet
     * @param hostMacId
     * @param subnetIp
     * @return
     */

    std::pair<bool, ip_t> InsertSubnetHost(MacID hostMacId, ip_t subnetIp) override;


    /**
     * Delete a subnet with ip  starting at start_ip
     * @param start_ip
     */

    /**
     * Delete the subnet with ip starting at start_ip
     * @param start_ip
     */

     void DeleteSubnet(ip_t start_ip) override;


    /**
     * Deletes a particular host from a subnet
     * @param host_ip
     */

    void DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) override;

    /**
     * For Router to find network ip of a host
     * @param hostIp
     * @return the network ip address
     */
    std::pair<bool, ip_t>  GetNetWorkIP(ip_t hostIp) override;


    /**
     * Get a hosts's ip address when host provides their mac address.
     * @param macId
     * @return
     */

    std::pair<bool, ip_t> GetHostIpAddress(MacID macId, ip_t subnet_ip) override;


    /**
     * For network admin to find host mac id using a specific host ip address
     * @param hostIpAddress
     * @return
     */

    std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) override;

    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> GetBestFitIp(int requiredCapacity);
    void UpdateFreeSlotsList(std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator iter, int requiredCapacity);

    [[maybe_unused]] void SetFreeSlots(ip_t startIp, int freeCapacity);

    std::vector<TreeMapValueObjectForUnusedObjectInArray> GetFreeSlotsList();
};


#endif //DHCPALLOCATOR_DSMODELTREEIMPL_H
