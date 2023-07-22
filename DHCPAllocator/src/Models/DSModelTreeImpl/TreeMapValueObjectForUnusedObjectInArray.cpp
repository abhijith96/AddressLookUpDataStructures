//
// Created by Abhijith  K A on 11/07/23.
//

#include "TreeMapValueObjectForUnusedObjectInArray.h"

ip_t TreeMapValueObjectForUnusedObjectInArray::GetStartIP() const {
    return start_ip_;
}

TreeMapValueObjectForUnusedObjectInArray::TreeMapValueObjectForUnusedObjectInArray(ip_t start_ip, ip_t capacity): start_ip_(start_ip),
                                                                                                                  capacity_(capacity){

}

ip_t TreeMapValueObjectForUnusedObjectInArray::GetCapacity() const {
    return capacity_;
}
