//
// Created by Abhijith  K A on 11/07/23.
//

#include "DSModelTreeImpl.h"
#include "DHCPAllocator/src/Models/DSModelmpl.h"


std::pair<bool, ip_t> DSModelTreeImpl::InsertSubnet(MacID subNetMacId, int capacity) {

    if(free_slots_list_.empty()){

    }
    return {false, 0};
}

std::pair<bool, ip_t> DSModelTreeImpl::InsertSubnetHost(MacID host_mac_id, ip_t subnet_ip) {
    return {false, 0};
}

void DSModelTreeImpl::DeleteSubnet(ip_t start_ip) {

}

void DSModelTreeImpl::DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) {

}

ip_t DSModelTreeImpl::GetNetWorkIP(ip_t hostIp) {
    return 0;
}

ip_t DSModelTreeImpl::GetHostIpAddress(MacID macId, ip_t subnet_ip) {
    return 0;
}

std::pair<bool, MacID> DSModelTreeImpl::GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) {
    return {false, MacID(0)};
};

std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> DSModelTreeImpl::GetBestFitIp(int requiredCapacity) {


    auto compare = [] (const TreeMapValueObjectForUnusedObjectInArray& arrayItem, const TreeMapValueObjectForUnusedObjectInArray requiredItem){
        return arrayItem.GetCapacity() < requiredItem.GetCapacity();
    };

    TreeMapValueObjectForUnusedObjectInArray dummmyForRequiredCapacity{0,requiredCapacity};

    auto iter = std::lower_bound(
            free_slots_list_.begin(), free_slots_list_.end(),dummmyForRequiredCapacity, compare
            );
    if(iter == free_slots_list_.end()){
        if(requiredCapacity > free_slots_list_.back().GetCapacity()){
            return std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator>{};
        }
        else{
            return free_slots_list_.begin();
        }
    }
    else{

        if(iter->GetCapacity() == requiredCapacity){
            return iter;
        }
        else{
            ++iter;
            if(iter != free_slots_list_.end()){
                return iter;
            }
            return std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator>{};
        }
    }

}

void DSModelTreeImpl::UpdateFreeSlotsList(std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator iter,
                                          int requiredCapacity, ip_t newStartIp) {
    if(iter->GetCapacity() == requiredCapacity){
        free_slots_list_.erase(iter);

    }

    else{
        int newCapacity = iter->GetCapacity() - requiredCapacity;
        TreeMapValueObjectForUnusedObjectInArray newIpSlot{newStartIp, newCapacity};
        free_slots_list_.erase(iter);

        auto compare = [] (const TreeMapValueObjectForUnusedObjectInArray& arrayItem, const TreeMapValueObjectForUnusedObjectInArray requiredItem){
            return arrayItem.GetCapacity() < requiredItem.GetCapacity();
        };

        CompareTreeMapValueObjectForUnusedObjectInArray compare2{};
        auto iterToElementBeforeNewItem = std::lower_bound(free_slots_list_.begin(), free_slots_list_.end(),newIpSlot, compare2);
        free_slots_list_.insert(iterToElementBeforeNewItem + 1, newIpSlot);

    }
}
