//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELMPL_H
#define DHCPALLOCATOR_DSMODELMPL_H

#include <IPAddressLookUpDataStructures/src/Models/Subnet.h>
#include <IPAddressLookUpDataStructures//src/Models/SubnetHosts.h>
#include <IPAddressLookUpDataStructures//src/Models/IPAddress.h>
#include <IPAddressLookUpDataStructures//src/Models/MacID.h>
#include <utility>


class DSModelmpl{
public:
 /**
  * Pure virtual function hence zero
  * @param subnet
  * returns starting ip address of subnet inserted
  *
  */

    DSModelmpl(){

    }
   virtual std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, ip_t capacity) = 0;


   /**
    * Give an ip address for a host in the given subnet
    * @param hostMacId
    * @param subnetIp
    * @return
    */

   virtual std::pair<bool, ip_t> InsertSubnetHost(MacID hostMacId, ip_t subnetIp) = 0;


   /**
    * Delete a subnet with ip  starting at start_ip
    * @param start_ip
    */

   /**
    * Delete the subnet with ip starting at start_ip
    * @param start_ip
    */

   virtual bool DeleteSubnet(ip_t start_ip) = 0;


   /**
    * Deletes a particular host from a subnet
    * @param host_ip
    */

   virtual bool DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) = 0;

   /**
    * For Router to find network ip of a host
    * @param hostIp
    * @return the network ip address
    */
   virtual std::pair<bool, ip_t> GetNetWorkIP(ip_t hostIp) = 0;


   /**
    * Get a hosts's ip address when host provides their mac address.
    * @param macId
    * @return
    */

   virtual std::pair<bool, ip_t> GetHostIpAddress(MacID macId, ip_t subnet_ip) = 0;


   /**
    * For network admin to find host mac id using a specific host ip address
    * @param hostIpAddress
    * @return
    */

   virtual std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) = 0;



   virtual ~DSModelmpl()  {}

};


#endif //DHCPALLOCATOR_DSMODELMPL_H
