//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_MACID_H
#define DHCPALLOCATOR_MACID_H
#include <cstdint>
#include <functional>


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

struct HashMacId{

   size_t operator() (const MacID& macId)  const{
       size_t seed = 31;
       std::hash<mac_id_t> hasher;
       seed ^= hasher(macId.GetValue());
       return seed;
   }
};

struct EqualsMacId{

    bool operator() (const MacID& first, const MacID& second) const{
       return first.GetValue() == second.GetValue();
    }
};


#endif //DHCPALLOCATOR_MACID_H
