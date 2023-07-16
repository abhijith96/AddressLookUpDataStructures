//
// Created by Abhijith  K A on 16/07/23.
//

#include <gtest/gtest.h>
#include <map>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>


using veb_value_t = VEBTreeValueObject;


class DSModelVebTreeImplTest : public ::testing::Test {
protected:
    void SetUp() override {
        
        vebTreeMap = std::make_unique<DSModelVebTreeImpl>(ipRange);
    }

    std::vector<std::pair<ip_t,ip_t >> GenerateSeries(ip_t start, ip_t count, ip_t increment) {
        std::vector<std::pair<ip_t,ip_t >> result;
        result.resize(count);
        ip_t current_val = start;
        for(auto& i : result){
            i = {current_val, current_val + increment};
            current_val += increment + 1;
        }
        return result;
    }

    void InsertElements (const std::vector<ip_t>& keys, const std::vector<ip_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            ip_t  key = keys[i];
            auto value = VEBTreeValueObject{static_cast<ip_t>(i)};
            vebTreeMap->InsertSubnet(MacID{1}, static_cast<ip_t>(values[i]));
        }
    }

    void DeleteElements (const std::vector<ip_t>& keys){
        for(size_t i = 0; i < keys.size(); ++i){
            ip_t  key = keys[i];
            vebTreeMap->DeleteSubnet(key);
        }
    }
//    bool CompareElements(const std::vector<ip_t>& keys, const std::vector<veb_value_t>& values){
//        for(size_t i = 0; i < keys.size(); ++i){
//            ip_t  key = keys[i];
//            ip_t value = values.at(i);
//            bool status;
//            veb_value_t vebValue;
//            std::tie (status, vebValue) = vebTreeMap->FindKey(key);
//            auto iter = map_.find(key);
//            if(iter == map_.end()){
//                return false;
//            }
//            else if(!status) return false;
//            else if(vebValue != iter->second){
//                return false;
//            }
//        }
//        return true;
//    }
//
//    bool CheckElementsAreNotPresent(const std::vector<ip_t>& keys, const std::vector<veb_value_t>& values) {
//        for (size_t i = 0; i < keys.size(); ++i) {
//            ip_t key = keys[i];
//            ip_t value = values.at(i);
//            bool status;
//            veb_value_t vebValue;
//            std::tie(status, vebValue) = vebTreeMap->FindKey(key);
//            auto iter = map_.find(key);
//            if (iter != map_.end() || status) {
//                return false;
//            }
//        }
//        return true;
//    }

    void TearDown() override {
        vebTreeMap.reset();
    }
    std::unique_ptr<DSModelVebTreeImpl> vebTreeMap;
    IpRange ipRange {IPAddress{0}, IPAddress{1024}};
    

};

TEST_F(DSModelVebTreeImplTest, SimpleTest){

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = vebTreeMap->GetNetWorkIP(std::numeric_limits<ip_t>::max() - 1);
        EXPECT_FALSE(isPresent);
    }





    {
        auto [isSubNetInserted, subNetIp ] = vebTreeMap->InsertSubnet(MacID{0}, 10);
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = vebTreeMap->GetNetWorkIP(3);
        EXPECT_EQ(networkIp, 0);

    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = vebTreeMap->GetNetWorkIP(9);
        EXPECT_EQ(networkIp, 0);
    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = vebTreeMap->GetNetWorkIP(10);
        EXPECT_FALSE(isPresent);
    }

    auto [isSubNetInserted, subNetIp ] = vebTreeMap->InsertSubnet(MacID{1}, 10);

    EXPECT_EQ(subNetIp, 10);

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = vebTreeMap->GetNetWorkIP(10);
        EXPECT_EQ(networkIp, 10);
    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = vebTreeMap->GetNetWorkIP(10);
        EXPECT_EQ(networkIp, 10);
    }


}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}