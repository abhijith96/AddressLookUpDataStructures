//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_TREEMAPVALUEOBJECTFORUNUSEDOBJECTINARRAY_H
#define DHCPALLOCATOR_TREEMAPVALUEOBJECTFORUNUSEDOBJECTINARRAY_H


#include <DHCPAllocator/src/Models/IPAddress.h>

class TreeMapValueObjectForUnusedObjectInArray {
private:
    ip_t start_ip_;
    ip_t capacity_;

public:
    TreeMapValueObjectForUnusedObjectInArray(ip_t start_ip, ip_t capacity);
    [[nodiscard]] ip_t GetStartIP() const;
    [[nodiscard]] ip_t GetCapacity() const;

};

struct CompareTreeMapValueObjectForUnusedObjectInArray{
    bool operator() (const TreeMapValueObjectForUnusedObjectInArray& arrayItem, const TreeMapValueObjectForUnusedObjectInArray
    & itemWithRequiredCapacity) const{
        return arrayItem.GetCapacity() < itemWithRequiredCapacity.GetCapacity();
    }
};


#endif //DHCPALLOCATOR_TREEMAPVALUEOBJECTFORUNUSEDOBJECTINARRAY_H
