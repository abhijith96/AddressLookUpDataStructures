//
// Created by Abhijith  K A on 14/07/23.
//

#ifndef DHCPALLOCATOR_VEBTREENODE_H
#define DHCPALLOCATOR_VEBTREENODE_H

#include <cstdint>
#include <memory>
#include <cmath>
#include <vector>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTreeNodeKeyType.h>

using veb_key_t = uint32_t;


template <typename ValueType>
class VEBTreeNode {
private:
    veb_key_t universe_;
    veb_key_t min_;
    veb_key_t max_;
    bool is_set_;


    std::unique_ptr<ValueType> min_value_;
    std::unique_ptr<ValueType> max_value_;

    std::unique_ptr<VEBTreeNode> summary_;
    std::vector<std::unique_ptr<VEBTreeNode>> sub_clusters_;

public:
   explicit VEBTreeNode(veb_key_t universe);
   void SetMinKey(veb_key_t min);
   void SetMaxKey(veb_key_t max);
   void SetMinValue(ValueType value);
   void SetMaxValue(ValueType value);
   void UnSetMin();
   void UnSetMax();
   void UnSet();
   [[nodiscard]] veb_key_t GetMinKey() const;
   [[nodiscard]] veb_key_t GetMaxKey() const;
   [[nodiscard]] bool IsSet() const;
   const ValueType & GetMinValue();
   const ValueType & GetMaxValue();
   [[nodiscard]] veb_key_t GetUniverseSize() const;
    VEBTreeNode<ValueType>& GetCluster(veb_key_t clusterIndex);
   VEBTreeNode<ValueType>& GetSummary();

};



#endif //DHCPALLOCATOR_VEBTREENODE_H
