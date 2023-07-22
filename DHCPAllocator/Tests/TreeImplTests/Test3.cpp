//
// Created by Prabhleen Kaur on 2023-07-16.
//

#include <iostream>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <vector>
#include <DHCPAllocator/src/Models/DSModelmpl.h>

int main(){
    DSModelType dsModelType = DSModelType::TREE;
    DSModelSingleton &dsModelSingleton = DSModelSingleton::GetSingletonInstance(dsModelType);
    DSModelmpl& dsModel = dsModelSingleton.GetDSModel();

    std::cout << "Test cases for InsertSubnet" << std::endl;
    std::cout << "==================================================================\n" << std::endl;
    std::pair<bool, ip_t> start_ip1 = dsModel.InsertSubnet(MacID{100}, 1000);
    std::cout << "Test case 1 - Status is : " << start_ip1.first << std::endl;
    std::cout << "Test case 1 - StartIp is : " << start_ip1.second << std::endl;
    assert(start_ip1.first == true);
    assert(start_ip1.second == 0);
    std::cout << "==================================================================\n" << std::endl;

    std::pair<bool, ip_t> start_ip2 = dsModel.InsertSubnet(MacID{200}, 2147482647);
    std::cout << "Test case 2 - Status is : " << start_ip2.first << std::endl;
    std::cout << "Test case 2 - StartIp is : " << start_ip2.second << std::endl;
    assert(start_ip2.first == true);
    assert(start_ip2.second == 1000);
    std::cout << "==================================================================\n" << std::endl;

    std::pair<bool, ip_t> start_ip3 = dsModel.InsertSubnet(MacID{300}, 10);
    std::cout << "Test case 3 - Status is : " << start_ip3.first << std::endl;
    std::cout << "Test case 3 - StartIp is : " << start_ip3.second << std::endl;
    assert(start_ip3.first == false);
    assert(start_ip3.second == 0);
    std::cout << "==================================================================\n" << std::endl;

    std::cout << "Test cases for GetNetWorkIP when host IP is provided." << std::endl;
    std::cout << "==================================================================" << std::endl;
    std::pair<bool, ip_t> subnet_ip1 = dsModel.GetNetWorkIP(100);
    std::cout << "Test case 4 - Status is : " << subnet_ip1.first << std::endl;
    std::cout << "Test case 4 - Subnet Id is : " << subnet_ip1.second << std::endl;
    assert(subnet_ip1.first == true);
    assert(subnet_ip1.second == 0);
    std::cout << "==================================================================" << std::endl;

    std::pair<bool, ip_t> subnet_ip2 = dsModel.GetNetWorkIP(387423894);
    std::cout << "Test case 5 - Status is : " << subnet_ip2.first << std::endl;
    std::cout << "Test case 5 - Subnet Id is : " << subnet_ip2.second << std::endl;
    assert(subnet_ip2.first == true);
    assert(subnet_ip2.second == 1000);
    std::cout << "==================================================================" << std::endl;

    std::pair<bool, ip_t> subnet_ip3 = dsModel.GetNetWorkIP(2147483648);
    std::cout << "Test case 6 - Status is : " << subnet_ip3.first << std::endl;
    std::cout << "Test case 6 - Subnet Id is : " << subnet_ip3.second << std::endl;
    assert(subnet_ip3.first == false);
    assert(subnet_ip3.second == 0);
    std::cout << "==================================================================\n" << std::endl;

    std::cout << "Test case for Deleting Subnet Id from the tree." << std::endl;
    std::cout << "==================================================================" << std::endl;
    std::cout << "After deleting subnet id - 0, tree is: "<< std::endl;
    dsModel.DeleteSubnet(0);
}
