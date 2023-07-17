//
// Created by Abhijith  K A on 17/07/23.
//

#ifndef DHCPALLOCATOR_VEBTREEFREESLOTSOBJECT_H
#define DHCPALLOCATOR_VEBTREEFREESLOTSOBJECT_H

#include <DHCPAllocator/src/Models/IPAddress.h>
#include <cctype>

class VebTreeFreeSlotsObject {
private:
    ip_t startIp_;
    ip_t capacity_;

public:
    VebTreeFreeSlotsObject(ip_t startIp, ip_t capacity);
    [[nodiscard]] ip_t GetStartIP() const;
    [[nodiscard]] ip_t GetCapacity() const;
};

struct CompareVebTreeFreeSlotsObject{
    bool operator()(const VebTreeFreeSlotsObject& second, const VebTreeFreeSlotsObject& first) const{
        return second.GetCapacity() < first.GetCapacity();
    }
};


#endif //DHCPALLOCATOR_VEBTREEFREESLOTSOBJECT_H
