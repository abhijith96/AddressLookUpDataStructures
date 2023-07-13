//
// Created by Abhijith  K A on 11/07/23.
//

#include <iostream>
#include "DSModelTreeImpl.h"
#include "DHCPAllocator/src/Models/DSModelmpl.h"


ip_t DSModelTreeImpl::InsertSubnet(MacID subNetMacId, int capacity) {

    if(free_slots_list_.empty()){

    }else{
        std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> requiredSlotItr = GetBestFitIp(capacity);
        if(requiredSlotItr.has_value()){
            UpdateFreeSlotsList(*requiredSlotItr, capacity);
            treemap_.insert({(*requiredSlotItr)->GetStartIP(), TreeMapValueObject(capacity, subNetMacId)});
        }else{
        }
    }
}

ip_t DSModelTreeImpl::InsertSubnetHost(MacID hostMacId, ip_t subnetIp) {
    return 0;
}

void DSModelTreeImpl::DeleteSubnet(ip_t start_ip) {

}

void DSModelTreeImpl::DeleteHostFromSubnet(ip_t host_ip) {

}

ip_t DSModelTreeImpl::GetNetWorkIP(ip_t hostIp) {
    return 0;
}

ip_t DSModelTreeImpl::GetIpAddress(MacID macId) {
    return 0;
}

MacID DSModelTreeImpl::GetMacAddressOfHost(ip_t hostIpAddress) {
    return MacID(0);
}

std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> DSModelTreeImpl::GetBestFitIp(int requiredCapacity) {
    auto compare = [](const TreeMapValueObjectForUnusedObjectInArray& arrayItem, const TreeMapValueObjectForUnusedObjectInArray& requiredItem) {
        return arrayItem.GetCapacity() < requiredItem.GetCapacity();
    };

    TreeMapValueObjectForUnusedObjectInArray dummmyForRequiredCapacity{0,requiredCapacity};
    auto iter = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(), dummmyForRequiredCapacity, compare);

    if (iter != free_slots_list_.end() && iter->GetCapacity() >= requiredCapacity) {
        return iter;
    } else {
        return std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator>{};
    }
}


void DSModelTreeImpl::UpdateFreeSlotsList(std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator iter,
                                          int requiredCapacity) {
    if(iter->GetCapacity() == requiredCapacity){
        free_slots_list_.erase(iter);
    }

    else{
        int newCapacity = iter->GetCapacity() - requiredCapacity;
        ip_t newStartIp = iter->GetStartIP() + requiredCapacity;
        TreeMapValueObjectForUnusedObjectInArray newIpSlot{newStartIp, newCapacity};
        free_slots_list_.erase(iter);
        SetFreeSlots(newStartIp, newCapacity);
    }
}

[[maybe_unused]] void DSModelTreeImpl::SetFreeSlots(ip_t startIp, int freeCapacity) {
    TreeMapValueObjectForUnusedObjectInArray newObj(startIp, freeCapacity);

    auto insertPos = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(), newObj,
                                      [](const TreeMapValueObjectForUnusedObjectInArray& lhs,
                                         const TreeMapValueObjectForUnusedObjectInArray& rhs) {
                                          return lhs.GetCapacity() < rhs.GetCapacity();
                                      });

    free_slots_list_.insert(insertPos, newObj);
}

std::vector<TreeMapValueObjectForUnusedObjectInArray> DSModelTreeImpl::GetFreeSlotsList() {
    return free_slots_list_;
}
