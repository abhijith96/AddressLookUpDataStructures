//
// Created by Abhijith  K A on 17/07/23.
//

#ifndef DHCPALLOCATOR_VEBTREEFREESLOTSOBJECT_H
#define DHCPALLOCATOR_VEBTREEFREESLOTSOBJECT_H

#include <IPAddressLookUpDataStructures/src/Models/IPAddress.h>
#include <cctype>

class VebTreeFreeSlotsObject {
private:
    ip_t startIp_;
    ip_t capacity_;

public:
    VebTreeFreeSlotsObject() = default;
    VebTreeFreeSlotsObject(ip_t startIp, ip_t capacity);
    VebTreeFreeSlotsObject(const VebTreeFreeSlotsObject& ) = default;
    VebTreeFreeSlotsObject& operator = (VebTreeFreeSlotsObject&& other) = default;

    [[nodiscard]] ip_t GetStartIP() const;
    [[nodiscard]] ip_t GetCapacity() const;
};

struct CompareVebTreeFreeSlotsObject{
    bool operator()(const VebTreeFreeSlotsObject& second, const VebTreeFreeSlotsObject& first) const{
        if(second.GetCapacity() == first.GetCapacity()){
            return second.GetStartIP() < first.GetStartIP();
        }
        return second.GetCapacity() < first.GetCapacity();
    }
};


#endif //DHCPALLOCATOR_VEBTREEFREESLOTSOBJECT_H
