//
// Created by Abhijith  K A on 11/07/23.
//

#include <gtest/gtest.h>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <DHCPAllocator/src/Models/IPAddress.h>
#include <DHCPAllocator/src/Models/DSModelArrayImpl/DSModelArrayImpl.h>


class DSModelTestSingletonTest : public ::testing::Test {
protected:
    void SetUp() override {

//        vebTreeMap = std::make_unique<DSModelVebTreeImpl>(ipRange);
    }


    void TearDown() override {

    }

//    IpRange ipRange {IPAddress{0}, IPAddress{1023}};


};

TEST_F(DSModelTestSingletonTest, CreationTest) {

    DSModelType dsModelType = DSModelType::TREE;

    EXPECT_NO_THROW(
    auto & instance = DSModelSingleton::GetSingletonInstance(dsModelType);
            DSModelmpl& dsModelmpl = instance.GetDSModel();
    );

    dsModelType = DSModelType::ARRAY;

    EXPECT_NO_THROW(
            auto & instance = DSModelSingleton::GetSingletonInstance(dsModelType);
            DSModelmpl& dsModelmpl = instance.GetDSModel();
    );


    dsModelType = DSModelType::HASHED_VEB_TREE;

    EXPECT_NO_THROW(
            auto & instance = DSModelSingleton::GetSingletonInstance(dsModelType);
            DSModelmpl& dsModelmpl = instance.GetDSModel();
    );

    dsModelType = DSModelType::VEB_TREE;

    EXPECT_NO_THROW(
            auto & instance = DSModelSingleton::GetSingletonInstance(dsModelType);
            DSModelmpl& dsModelmpl = instance.GetDSModel();
    );


}


TEST_F(DSModelTestSingletonTest, InsertionTest) {

    DSModelType dsModelType = DSModelType::ARRAY;

    auto & instance = DSModelSingleton::GetSingletonInstance(dsModelType);
    DSModelmpl& dsModelmpl = instance.GetDSModel();

    MacID networkIdentifier{10};

    ip_t  capacity = 100;

    ip_t  subNetIpExpected = 0;

   auto[isPresent, subnetIp]  = dsModelmpl.InsertSubnet(networkIdentifier, capacity);

    EXPECT_TRUE(isPresent);
    EXPECT_EQ(subnetIp, subNetIpExpected);



}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}