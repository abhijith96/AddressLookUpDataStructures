//
// Created by Abhijith  K A on 14/07/23.
//

#ifndef DHCPALLOCATOR_VEBTREEMAP_H
#define DHCPALLOCATOR_VEBTREEMAP_H

#include <memory>
#include "VEBTreeNode.h"


/**
 * C++ Implementation of Van Emde Boas Tree.
 * This version takes O(universeSize) memory upon creation
 * ValueType should have a default constructor and should be copyable and copy assignable
 * @tparam ValueType
 */
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
    [[nodiscard]] veb_key_t GetClusterCount(veb_key_t universe) const;
    [[nodiscard]] veb_key_t GetItemsCount(veb_key_t universe) const;

public:
    /**
     * Creates a van emde boas tree with universe size. ALl the clusters upto universe with count 2 are built.
     * So memory intensive.
     * @param universe
     */
    explicit VEBTreeMap(veb_key_t universe);

    /**
     * Gets the value for the key if key is present in the map.
     * If not present returns pair of False, Default Value for ValueType
     * @param currentKey
     * @return
     */
    std::pair<bool, ValueType> FindKey(veb_key_t currentKey);

    /**
     * Inserts the key and value into the map if not already present
     * Does not overwrite
     * @param key
     * @param value
     */
    void Insert(veb_key_t key, ValueType value);

    /**
     * Deletes the key and associated value from the tree if already present.
     * @param key
     */
    void Delete(veb_key_t key);

    /**
     * Finds the minimum key and value from the map which is >= key
     * Returns VEBTreeNodeKeyType::NORMAL, successor key, value for successor key in normal case.
     * If no successor exists returns VEBTreeNodeKeyType::POSITIVE_INFINITY, default value for key and default value for value type
     * @param key
     * @return
     */
    std::tuple<VEBTreeNodeKeyType, veb_key_t , ValueType> Successor(veb_key_t key);

    /**
     * Fins the maximum key <= key
     * Returns VEBTreeNodeKeyType::NORMAL, successor key, value for successor key in normal case.
     *  If no predecessor exists returns VEBTreeNodeKeyType::NEGATIVE_INFINITY, default value for key and default value for value type
     * @param key
     * @return
     */
    std::tuple<VEBTreeNodeKeyType, veb_key_t , ValueType> Predecessor(veb_key_t key);
};


#endif //DHCPALLOCATOR_VEBTREEMAP_H
