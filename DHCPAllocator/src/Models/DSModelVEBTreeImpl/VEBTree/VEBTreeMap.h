//
// Created by Abhijith  K A on 14/07/23.
//

#ifndef DHCPALLOCATOR_VEBTREEMAP_H
#define DHCPALLOCATOR_VEBTREEMAP_H

#include <memory>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTree/VEBTreeNode.h>


template <typename ValueType>
class VEBTreeMap {
private:
    std::unique_ptr<VEBTreeNode<ValueType>> root_node_;

    veb_key_t High(veb_key_t key, veb_key_t universe_size);
    veb_key_t Low(veb_key_t key, veb_key_t universe_size);
    void InsertHelper(VEBTreeNode<ValueType>& currentNode, veb_key_t key, ValueType value);
    void DeleteHelper(VEBTreeNode<ValueType>& currentNode, veb_key_t key);
    [[nodiscard]] veb_key_t Index(veb_key_t high, veb_key_t low, veb_key_t universe_size) const;
    std::tuple<VEBTreeNodeKeyType, veb_key_t , ValueType> SuccessorHelper(VEBTreeNode<ValueType>& currentNode, veb_key_t key);
    std::tuple<VEBTreeNodeKeyType, veb_key_t , ValueType> PredecessorHelper(VEBTreeNode<ValueType>& currentNode, veb_key_t key);

public:
    explicit VEBTreeMap(veb_key_t universe);

    std::pair<bool, ValueType> FindKey(veb_key_t currentKey);

    void Insert(veb_key_t key, ValueType value);

    void Delete(veb_key_t key);

    std::tuple<VEBTreeNodeKeyType, veb_key_t , ValueType> Successor(veb_key_t key);

    std::tuple<VEBTreeNodeKeyType, veb_key_t , ValueType> Predecessor(veb_key_t key);
};


#endif //DHCPALLOCATOR_VEBTREEMAP_H
