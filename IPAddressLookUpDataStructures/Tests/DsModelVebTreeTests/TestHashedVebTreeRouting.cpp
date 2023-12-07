//
// Created by Abhijith  K A on 18/07/23.
//


#include <gtest/gtest.h>
#include <map>
#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>


using veb_value_t = VEBTreeValueObject;


class DSModelVebTreeImplTestSubnetReInsertion : public ::testing::Test {
protected:
    void SetUp() override {

//        vebTreeMap = std::make_unique<DSModelVebTreeImpl>(ipRange);
    }


    void TearDown() override {
        dsModelVebTree_.reset();
    }
    std::unique_ptr<DSModelVebTreeImpl> dsModelVebTree_;
//    IpRange ipRange {IPAddress{0}, IPAddress{1023}};


};

TEST_F(DSModelVebTreeImplTestSubnetReInsertion, SimpleTest) {


    IpRange ipRange2Bytes{IPAddress{0}, IPAddress{static_cast<ip_t >(std::pow(2, 16)) - 1}};
    dsModelVebTree_ = std::make_unique<DSModelVebTreeImpl>(ipRange2Bytes);

    ip_t firstNetworkIp, secondNetworkIp;
    {
        ip_t capacity = 10;
        MacID subnetId{1};
        auto [isSubnetInserted, firstNetwork] = dsModelVebTree_->InsertSubnet(subnetId, capacity);
        firstNetworkIp = firstNetwork;
        EXPECT_TRUE(isSubnetInserted);
        EXPECT_EQ(firstNetwork, 0);
    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = dsModelVebTree_->GetNetWorkIP(firstNetworkIp + 5);
        EXPECT_TRUE(isPresent);
        EXPECT_TRUE(networkIp == firstNetworkIp);

    }

    {

        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = dsModelVebTree_->GetNetWorkIP(firstNetworkIp + 15);
        EXPECT_FALSE(isPresent);

    }



}

TEST_F(DSModelVebTreeImplTestSubnetReInsertion, RoutingMultipleSubnets) {


    IpRange ipRange2Bytes{IPAddress{0}, IPAddress{static_cast<ip_t >(std::pow(2, 16)) - 1}};
    dsModelVebTree_ = std::make_unique<DSModelVebTreeImpl>(ipRange2Bytes);
    {
        ip_t startIp = 0;
        ip_t capacity = 10;

        for(size_t i = startIp; i< 1000; i+=capacity){
            MacID macId{i};
            auto [isPresent, networkIp] = dsModelVebTree_->InsertSubnet(macId, capacity);
            EXPECT_TRUE(isPresent);
            EXPECT_EQ(networkIp, i);

            for(ip_t j = i; j < i+capacity; ++j){
                auto [isNetworkPresent, networkIpAddress] = dsModelVebTree_->GetNetWorkIP(j);
                EXPECT_TRUE(isNetworkPresent);
                EXPECT_TRUE(networkIpAddress== i);
            }
        }
    }



}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}