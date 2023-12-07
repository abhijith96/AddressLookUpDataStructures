//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_TREEMAPVALUEOBJECT_H
#define DHCPALLOCATOR_TREEMAPVALUEOBJECT_H

#include <DHCPAllocator/src/Models/MacID.h>
#include <DHCPAllocator/src/Models/IPAddress.h>


class TreeMapValueObject {
private:
    ip_t capacity_;
    MacID macId_;
public:
    TreeMapValueObject(ip_t capacity, MacID macId);
    [[nodiscard]] ip_t GetCapacity() const;
    [[nodiscard]] MacID GetMacAddress() const;

};




#endif //DHCPALLOCATOR_TREEMAPVALUEOBJECT_H
