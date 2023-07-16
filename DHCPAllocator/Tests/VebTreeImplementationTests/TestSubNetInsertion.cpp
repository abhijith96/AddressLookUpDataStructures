//
// Created by Abhijith  K A on 16/07/23.
//

#include <gtest/gtest.h>
#include <map>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>

constexpr veb_key_t universe = 16;

using veb_value_t = uint32_t;


class VEBTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    std::vector<veb_key_t> GenerateSeries(veb_key_t start, veb_key_t count, veb_key_t increment) {
        std::vector<veb_key_t> result;
        result.resize(count);
        veb_key_t current_val = start;
        for(auto& i : result){
            i = current_val;
            current_val += increment;
        }
        return result;
    }

    void InsertElements (const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values.at(i);
            vebTreeMap->Insert(key, value);
            map_.insert({key, value});
        }
    }

    void DeleteElements (const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values.at(i);
            vebTreeMap->Delete(key);
            map_.erase(key);
        }
    }
    bool CompareElements(const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values.at(i);
            bool status;
            veb_value_t vebValue;
            std::tie (status, vebValue) = vebTreeMap->FindKey(key);
            auto iter = map_.find(key);
            if(iter == map_.end()){
                return false;
            }
            else if(!status) return false;
            else if(vebValue != iter->second){
                return false;
            }
        }
        return true;
    }

    bool CheckElementsAreNotPresent(const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values.at(i);
            bool status;
            veb_value_t vebValue;
            std::tie (status, vebValue) = vebTreeMap->FindKey(key);
            auto iter = map_.find(key);
            if(iter != map_.end() || status){
                return false;
            }
        }
        return true;
    }

    void TearDown() override {
        vebTreeMap.reset();
    }
    std::unique_ptr<DSModelVebTreeImpl> vebTreeMap;
    std::map<veb_key_t , veb_value_t > map_;

};



}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}