//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_MACID_H
#define DHCPALLOCATOR_MACID_H
#include <cstdint>

using mac_id_t = uint64_t;

class MacID {
    private:
    mac_id_t value_;

public:

    explicit MacID(mac_id_t value){
        value_ = value;
    }

    [[nodiscard]] mac_id_t GetValue() const{
        return value_;
    }

};


#endif //DHCPALLOCATOR_MACID_H
