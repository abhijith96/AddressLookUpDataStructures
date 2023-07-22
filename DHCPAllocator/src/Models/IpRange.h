//
// Created by Abhijith  K A on 16/07/23.
//

#ifndef DHCPALLOCATOR_IPRANGE_H
#define DHCPALLOCATOR_IPRANGE_H

#include <DHCPAllocator/src/Models/IPAddress.h>

class IpRange {
private:
    IPAddress startIp_;
    IPAddress endIp_;
public:
    explicit IpRange(IPAddress startIp, IPAddress endIp);
    IpRange(IpRange&& other);
    IpRange & operator= (IpRange&& other) noexcept ;

    IpRange (const IpRange& other);

    IpRange& operator=(const IpRange& other);

    [[nodiscard]] ip_t GetCapacity  () const;

    [[nodiscard]] ip_t GetStartIP() const;

    [[nodiscard]] ip_t GetEndIP() const;
};


#endif //DHCPALLOCATOR_IPRANGE_H
