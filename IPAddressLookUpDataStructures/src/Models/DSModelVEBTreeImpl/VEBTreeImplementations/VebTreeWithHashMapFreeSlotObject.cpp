//
// Created by Abhijith  K A on 17/07/23.
//

#include "VebTreeWithHashMapFreeSlotObject.h"

VebTreeWithHashMapFreeSlotObject::VebTreeWithHashMapFreeSlotObject(ip_t capacity):capacity_(capacity) {

}

void VebTreeWithHashMapFreeSlotObject::AddIp(ip_t startIp) {
    startIpList_.push_back(startIp);

}

ip_t VebTreeWithHashMapFreeSlotObject::getCapacity() const {
    return capacity_;
}

const std::vector<ip_t> &VebTreeWithHashMapFreeSlotObject::GetIpListHavingCapacity() const {
    return startIpList_;
}

std::vector<ip_t> &VebTreeWithHashMapFreeSlotObject::GetIpListHavingCapacity() {
    return startIpList_;
}

VebTreeWithHashMapFreeSlotObject::VebTreeWithHashMapFreeSlotObject(ip_t capacity, ip_t startIP):capacity_(capacity) {
    startIpList_.push_back(startIP);

}

VebTreeWithHashMapFreeSlotObject::VebTreeWithHashMapFreeSlotObject() = default;
