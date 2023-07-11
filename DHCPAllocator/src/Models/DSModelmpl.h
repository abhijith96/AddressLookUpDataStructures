//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELMPL_H
#define DHCPALLOCATOR_DSMODELMPL_H

#include <DHCPAllocator/src/Models/Subnet.h>
#include <DHCPAllocator/src/Models/SubnetHost.h>
#include <DHCPAllocator/src/Models/IPAddress.h>
#include <DHCPAllocator/src/Models/MacID.h>


class DSModelmpl{
public:
 /**
  * Pure virtual function hence zero
  * @param subnet
  * returns starting ip address of subnet inserted
  */
   virtual ip_t InsertSubnet(MacID subNetMacId, int capacity) = 0;


   /**
    * Give an ip address for a host in the given subnet
    * @param hostMacId
    * @param subnetIp
    * @return
    */

   virtual ip_t InsertSubnetHost(MacID hostMacId, ip_t subnetIp) = 0;


   /**
    * Delete a subnet with ip  starting at start_ip
    * @param start_ip
    */

   /**
    * Delete the subnet with ip starting at start_ip
    * @param start_ip
    */

   virtual void DeleteSubnet(ip_t start_ip) = 0;


   /**
    * Deletes a particular host from a subnet
    * @param host_ip
    */

   virtual void DeleteHostFromSubnet(ip_t host_ip) = 0;

   /**
    * For Router to find network ip of a host
    * @param hostIp
    * @return the network ip address
    */
   virtual ip_t  GetNetWorkIP(ip_t hostIp) = 0;


   /**
    * Get a hosts's ip address when host provides their mac address.
    * @param macId
    * @return
    */

   virtual ip_t  GetIpAddress(MacID macId) = 0;


   /**
    * For network admin to find host mac id using a specific host ip address
    * @param hostIpAddress
    * @return
    */

   virtual MacID GetMacAddressOfHost(ip_t hostIpAddress) = 0;

   virtual ~DSModelmpl()  {}

};


#endif //DHCPALLOCATOR_DSMODELMPL_H
