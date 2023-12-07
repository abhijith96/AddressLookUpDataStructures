//
// Created by Abhijith  K A on 17/07/23.
//

#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VebTreeFreeSlotsObject.h>


VebTreeFreeSlotsObject::VebTreeFreeSlotsObject(ip_t startIp, ip_t capacity) :startIp_(startIp), capacity_(capacity) {

}

ip_t VebTreeFreeSlotsObject::GetStartIP() const {
    return startIp_;
}

ip_t VebTreeFreeSlotsObject::GetCapacity() const {
    return capacity_;
}
