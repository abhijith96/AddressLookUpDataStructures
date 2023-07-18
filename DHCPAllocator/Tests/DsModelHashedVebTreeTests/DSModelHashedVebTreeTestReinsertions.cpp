//
// Created by Abhijith  K A on 18/07/23.
//



#include <gtest/gtest.h>
#include <map>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelHashedVebTreeImpl.h>





class DSModelHashedVebTreeImplTestSubnetReInsertions : public ::testing::Test {
protected:
    void SetUp() override {

//        vebTreeMap = std::make_unique<DSModelVebTreeImpl>(ipRange);
    }


    void TearDown() override {
        hashedVebTreeMap_.reset();
    }
    std::unique_ptr<DSModelHashedVebTreeImpl> hashedVebTreeMap_;
//    IpRange ipRange {IPAddress{0}, IPAddress{1023}};


};

TEST_F(DSModelHashedVebTreeImplTestSubnetReInsertions, SimpleTest) {



    IpRange ipRange2Bytes{IPAddress{0}, IPAddress{static_cast<ip_t >(std::pow(2,16)) - 1}};
    hashedVebTreeMap_ = std::make_unique<DSModelHashedVebTreeImpl>(ipRange2Bytes);

    ip_t firstNetworkIp, secondNetworkIp;
    {
        ip_t  capacity = static_cast<ip_t >(std::pow(2,15));
        MacID subnetId{1};
        auto [isSubnetInserted, firstNetwork] = hashedVebTreeMap_->InsertSubnet(subnetId, capacity);
        firstNetworkIp = firstNetwork;
        EXPECT_TRUE(isSubnetInserted);
        EXPECT_EQ(firstNetwork, 0);
    }

    {
        ip_t  capacity = static_cast<ip_t >(std::pow(2,15));
        MacID subnetId{2};
        auto [isSubnetInserted, secondNetwork] = hashedVebTreeMap_->InsertSubnet(subnetId, capacity);
        secondNetworkIp = secondNetwork;
        EXPECT_TRUE(isSubnetInserted);
        EXPECT_EQ(secondNetworkIp, static_cast<ip_t >(std::pow(2,15)));
    }

    {
        hashedVebTreeMap_->DeleteSubnet(firstNetworkIp);
    }

    ip_t thirdNetworkIp;

    {
        MacID macIdForThirdSubnet{3};
        ip_t  capacity = 100;
        auto [isSubnetInserted, networkIp] = hashedVebTreeMap_->InsertSubnet(macIdForThirdSubnet, capacity);
        EXPECT_TRUE(isSubnetInserted);
        EXPECT_EQ(networkIp, 0);
        thirdNetworkIp = networkIp;
    }


    {
        MacID macIdForBigSubnet{4};
        ip_t  capacity = static_cast<ip_t >(std::pow(2,15));
        auto [isSubnetInserted, networkIp] = hashedVebTreeMap_->InsertSubnet(macIdForBigSubnet, capacity);
        EXPECT_FALSE(isSubnetInserted);
    }

    {
        MacID macIdForBigSubnetRightFit{4};
        ip_t  capacity = static_cast<ip_t >(std::pow(2,15)) - 100;
        auto [isSubnetInserted, networkIp] = hashedVebTreeMap_->InsertSubnet(macIdForBigSubnetRightFit, capacity);
        EXPECT_TRUE(isSubnetInserted);
        EXPECT_EQ(networkIp, 100);
    }

    {
        MacID macIdForThirdSubnet{3};
        hashedVebTreeMap_->DeleteSubnet(thirdNetworkIp);
    }

    //insert small subnets

    ip_t s1NetworkIp;

    ip_t s2NetworkIp;

    ip_t s3NetworkIp;

    {
        MacID macIdForS1{11};
        ip_t s1Capacity = 25;
        auto [isSubnetInserted, networkIp] = hashedVebTreeMap_->InsertSubnet(macIdForS1, s1Capacity);
        EXPECT_TRUE(isSubnetInserted);
        EXPECT_EQ(networkIp, thirdNetworkIp);
        s1NetworkIp = networkIp;

    }
    {

        MacID macIdForS2{12};
        ip_t s2Capacity = 30;
        auto [isSubnetInserted, networkIp] = hashedVebTreeMap_->InsertSubnet(macIdForS2, s2Capacity);
        EXPECT_TRUE(isSubnetInserted);
        s2NetworkIp = networkIp;

    }

    {

        MacID macIdForS3{13};
        ip_t s3Capacity = 40;
        auto [isSubnetInserted, networkIp] = hashedVebTreeMap_->InsertSubnet(macIdForS3, s3Capacity);
        EXPECT_TRUE(isSubnetInserted);
        s3NetworkIp = networkIp;

    }

    //Delete all small netowrks

    {
        hashedVebTreeMap_->DeleteSubnet(s1NetworkIp);
        hashedVebTreeMap_->DeleteSubnet(s2NetworkIp);
        hashedVebTreeMap_->DeleteSubnet(s3NetworkIp);
    }

    // Insert another small one

    {
        MacID macIdForS10{15};
        ip_t s10Capacity = 28;
        auto [isSubnetInserted, networkIp] = hashedVebTreeMap_->InsertSubnet(macIdForS10, s10Capacity);
        EXPECT_TRUE(isSubnetInserted);
        EXPECT_EQ(s2NetworkIp, networkIp);
    }

    {
        MacID macIdForS11{16};
        ip_t s11Capacity = 25;
        auto [isSubnetInserted, networkIp] = hashedVebTreeMap_->InsertSubnet(macIdForS11, s11Capacity);
        EXPECT_TRUE(isSubnetInserted);
        EXPECT_EQ(s1NetworkIp, networkIp);
    }


}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}