//
// Created by Sreedevi R Warrier on 2023-07-16.
//


#include <iostream>
#include <DHCPAllocator/src/HTTPServer/HttpServer.h>

#include <boost/property_tree/json_parser.hpp>
#include <DHCPAllocator/src/Models/DSModelArrayImpl/DSModelArrayImpl.h>
#include <DHCPAllocator/src/Models/DSModelTreeImpl/DSModelTreeImpl.h>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelHashedVebTreeImpl.h>
#include <DHCPAllocator/src/Models/IPAddress.h>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <boost/json.hpp>


DSModelType globalDSModelType;
ip_t getIPAddressNumber(const std::string& ipString) {
    std::vector<unsigned int> octets;
    std::istringstream ss(ipString);
    std::string octetString;

    while (std::getline(ss, octetString, '.')) {
        unsigned int octet = std::stoi(octetString);
        if (octet > 255) {
            throw std::invalid_argument("Invalid IP address: " + ipString);
        }
        octets.push_back(octet);
    }

    if (octets.size() != 4) {
        throw std::invalid_argument("Invalid IP address: " + ipString);
    }

    ip_t ipAddress = (octets[0] << 24) | (octets[1] << 16) | (octets[2] << 8) | octets[3];
    return ipAddress;
}


std::string getIPAddressString(ip_t ip) {
    std::string ipString;
    for (int i = 3; i >= 0; --i) {
        int octet = (ip >> (i * 8)) & 0xFF;
        ipString += std::to_string(octet);
        if (i > 0) {
            ipString += ".";
        }
    }
    return ipString;
}

boost::property_tree::ptree requestSubnet(boost::property_tree::ptree pt, DSModelmpl& dsModelImpl) {
    boost::property_tree::ptree response;

    auto network_identifier_optional = pt.get_optional<std::int64_t>("network_identifier");
    auto capacity_optional = pt.get_optional<ip_t>("capacity");

    if(network_identifier_optional.has_value() && capacity_optional.has_value()) {
        int64_t network_identifier = network_identifier_optional.get();
        ip_t capacity = capacity_optional.get();

        std::cout << "network_identifier: " << network_identifier << std::endl;
        std::cout << "capacity: " << capacity << std::endl;

        MacID macId = MacID(network_identifier);
        std::pair<bool, ip_t> insert_subnet_response = dsModelImpl.InsertSubnet(macId, capacity);

        if (insert_subnet_response.first) {
            ip_t start_ip = insert_subnet_response.second;
            response.add("status", true);
            response.add("start_ip", getIPAddressString(start_ip));
        } else {
            response.add("status", false);
            response.add("reason", "Error in assigning subnet");
        }
    } else {
        response.add("status", false);
        response.add("reason", "Error in given input");
    }
    return response;
}

boost::property_tree::ptree requestSubnetHost(boost::property_tree::ptree pt, DSModelmpl& dsModelImpl) {
    boost::property_tree::ptree response;

    auto mac_id_optional = pt.get_optional<std::int64_t>("mac_id");
    auto subnet_ip_optional = pt.get_optional<std::string>("subnet_ip");

    if(mac_id_optional.has_value() && subnet_ip_optional.has_value()) {
        int64_t mac_id = mac_id_optional.get();
        ip_t subnet_ip = getIPAddressNumber(subnet_ip_optional.get());

        std::cout << "mac_id: " << mac_id << std::endl;
        std::cout << "subnet_ip: " << subnet_ip << std::endl;

        MacID macId = MacID(mac_id);
        std::pair<bool, ip_t> insert_host_response = dsModelImpl.InsertSubnetHost(macId, subnet_ip);

        if (insert_host_response.first) {
            ip_t host_ip = insert_host_response.second;
            response.add("status", true);
            response.add("host_ip", getIPAddressString(host_ip));
        } else {
            response.add("status", false);
            response.add("reason", "Error in assigning host IP");
        }
    } else {
        response.add("status", false);
        response.add("reason", "Error in given input");
    }
    return response;
}

boost::property_tree::ptree deleteSubnet(boost::property_tree::ptree pt, DSModelmpl& dsModelImpl) {
    boost::property_tree::ptree response;

    auto subnet_ip_optional = pt.get_optional<std::string>("subnet_ip");

    if (subnet_ip_optional.has_value()) {

        ip_t subnet_ip = getIPAddressNumber(subnet_ip_optional.get());
        std::cout << "subnet_ip: " << subnet_ip << std::endl;

        bool delete_host_response = dsModelImpl.DeleteSubnet(subnet_ip);

        if (delete_host_response) {
            response.add("status", true);
        } else {
            response.add("status", false);
            response.add("reason", "Error in deleting the subnet");
        }
    } else {
        response.add("status", false);
        response.add("reason", "Error in given input");
    }
    return response;
}

boost::property_tree::ptree deleteSubnetHost(boost::property_tree::ptree pt, DSModelmpl& dsModelImpl) {
    boost::property_tree::ptree response;

    auto host_ip_optional = pt.get_optional<std::string>("host_ip");
//    auto host_mac_id_optional = pt.get_optional<int64_t>("host_mac_id");
    auto subnet_ip_optional = pt.get_optional<std::string>("subnet_ip");

    if(host_ip_optional.has_value() && subnet_ip_optional.has_value()){ // && host_mac_id_optional.has_value()) {
        ip_t host_ip = getIPAddressNumber(host_ip_optional.get());
        ip_t subnet_ip = getIPAddressNumber(subnet_ip_optional.get());
//        int64_t host_mac_id = host_mac_id_optional.get();

        std::cout << "host_ip: " << host_ip << std::endl;
//        std::cout << "host_mac_id: " << host_mac_id << std::endl;
        std::cout << "subnet_ip: " << subnet_ip << std::endl;

        bool delete_host_response = dsModelImpl.DeleteHostFromSubnet(host_ip, subnet_ip);

        if (delete_host_response) {
            response.add("status", true);
        } else {
            response.add("status", false);
            response.add("reason", "Error in deleting the host IP");
        }
    } else {
        response.add("status", false);
        response.add("reason", "Error in given input");
    }
    return response;
}

boost::property_tree::ptree getNetworkIP(boost::property_tree::ptree pt, DSModelmpl& dsModelImpl) {
    boost::property_tree::ptree response;

    auto host_ip_optional = pt.get_optional<std::string>("host_ip");

    if (host_ip_optional.has_value()) {

        ip_t host_ip = getIPAddressNumber(host_ip_optional.get());
        std::cout << "host_ip: " << host_ip << std::endl;

        auto get_network_ip_response = dsModelImpl.GetNetWorkIP(host_ip);

        if (get_network_ip_response.first) {
            response.add("status", true);
            response.add("network_ip", getIPAddressString(get_network_ip_response.second));
        } else {
            response.add("status", false);
            response.add("reason", "Error in getting network address");
        }
    } else {
        response.add("status", false);
        response.add("reason", "Error in given input");
    }
    return response;
}

boost::property_tree::ptree getHostIpAddress(boost::property_tree::ptree pt, DSModelmpl& dsModelImpl) {
    boost::property_tree::ptree response;

    auto host_mac_id_optional = pt.get_optional<int64_t>("host_mac_id");
    auto subnet_ip_optional = pt.get_optional<std::string>("subnet_ip");

    if (host_mac_id_optional.has_value() && subnet_ip_optional.has_value()) {

        ip_t subnet_ip = getIPAddressNumber(subnet_ip_optional.get());
        int64_t host_mac_id = host_mac_id_optional.get();

        std::cout << "host_mac_id: " << host_mac_id << std::endl;
        std::cout << "subnet_ip: " << subnet_ip << std::endl;


        auto get_host_ip_response = dsModelImpl.GetHostIpAddress(MacID(host_mac_id), subnet_ip);

        if (get_host_ip_response.first) {
            response.add("status", true);
            response.add("host_ip", getIPAddressString(get_host_ip_response.second));
        } else {
            response.add("status", false);
            response.add("reason", "Error in getting host IP address");
        }
    } else {
        response.add("status", false);
        response.add("reason", "Error in given input");
    }
    return response;
}

boost::property_tree::ptree getMacAddressOfHost(boost::property_tree::ptree pt, DSModelmpl& dsModelImpl) {
    boost::property_tree::ptree response;

    auto host_ip_optional = pt.get_optional<std::string>("host_ip");
    auto subnet_ip_optional = pt.get_optional<std::string>("subnet_ip");

    if (host_ip_optional.has_value() && subnet_ip_optional.has_value()) {

        ip_t subnet_ip = getIPAddressNumber(subnet_ip_optional.get());
        ip_t host_ip = getIPAddressNumber(host_ip_optional.get());

        std::cout << "host_ip: " << host_ip << std::endl;
        std::cout << "subnet_ip: " << subnet_ip << std::endl;

        auto get_host_mac_response = dsModelImpl.GetMacAddressOfHost(host_ip, subnet_ip);

        if (get_host_mac_response.first) {
            response.add("status", true);
            response.add("mac_id", get_host_mac_response.second.GetValue());
        } else {
            response.add("status", false);
            response.add("reason", "Error in getting host mac address");
        }
    } else {
        response.add("status", false);
        response.add("reason", "Error in given input");
    }
    return response;
}

boost::property_tree::ptree optimizeSubnetAllocationSpace(DSModelmpl& dsModelImpl){
    boost::property_tree::ptree subnets;

    if(globalDSModelType == DSModelType::ARRAY) {
        DSModelArrayImpl & dsModelArrayImpl =  dynamic_cast<DSModelArrayImpl&>(dsModelImpl);
        auto new_assignments = dsModelArrayImpl.optimizeSubnetAllocationSpace();


        for (const auto &subnet_pair: new_assignments) {
            MacID subnet_mac_id = subnet_pair.first;
            auto subnet = subnet_pair.second;
            ip_t subnet_start_ip = subnet.first;

            subnets.add("network_identifier", subnet_mac_id.GetValue());
            subnets.add("new_subnet_ip", getIPAddressString(subnet_start_ip));

            boost::property_tree::ptree hosts;
            for (const auto &host: subnet.second) {
                MacID host_mac_id = host.first;
                ip_t host_ip = host.second;
                hosts.add("host_mac_id", host_mac_id.GetValue());
                hosts.add("new_host_ip", getIPAddressString(host_ip));
            }
            subnets.add_child("hosts", hosts);
        }
    }
    return subnets;
}

boost::property_tree::ptree renewHost(boost::property_tree::ptree pt, DSModelmpl& dsModelImpl) {
    boost::property_tree::ptree response;

    if(globalDSModelType == DSModelType::ARRAY) {

        DSModelArrayImpl &dsModelArrayImpl = dynamic_cast<DSModelArrayImpl &>(dsModelImpl);
        auto new_assignments = dsModelArrayImpl.optimizeSubnetAllocationSpace();

        auto host_ip_optional = pt.get_optional<std::string>("host_ip");
        auto subnet_ip_optional = pt.get_optional<std::string>("subnet_ip");

        if (host_ip_optional.has_value() && subnet_ip_optional.has_value()) {

            ip_t subnet_ip = getIPAddressNumber(subnet_ip_optional.get());
            ip_t host_ip = getIPAddressNumber(host_ip_optional.get());

            std::cout << "host_ip: " << host_ip << std::endl;
            std::cout << "subnet_ip: " << subnet_ip << std::endl;

            auto host_renewal_request = dsModelArrayImpl.RequestHostRenewal(host_ip, subnet_ip);

            if (host_renewal_request) {
                response.add("status", true);
            } else {
                response.add("status", false);
                response.add("reason", "Error in renewing host");
            }
        } else {
            response.add("status", false);
            response.add("reason", "Error in given input");
        }
    }
    return response;
}

boost::property_tree::ptree deleteNonRenewedHosts(DSModelmpl& dsModelImpl){
    boost::property_tree::ptree response;

    if(globalDSModelType == DSModelType::ARRAY) {
        DSModelArrayImpl & dsModelArrayImpl =  dynamic_cast<DSModelArrayImpl&>(dsModelImpl);
        auto delete_non_renewed_host_response = dsModelArrayImpl.DeleteNonRenewedHosts();

        if (delete_non_renewed_host_response) {
            response.add("status", true);
        } else {
            response.add("status", false);
            response.add("reason", "Error in deleting non-renewed host");
        }

    }
    return response;
}

std::string HttpServer::generateResponse(const http::request<http::string_body>& request, DSModelmpl& dsModelImpl) {
    std::string response;

    if (request.method() == http::verb::post) {

        if (request.target() == "/requestSubnet") {
            // Extract input parameters from the request body
            std::string requestBody = request.body();
            std::stringstream requestBodyStream;
            requestBodyStream << requestBody;
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(requestBodyStream, pt);

            auto request_subnet_response = requestSubnet(pt, dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, request_subnet_response);
            response = out_stream.str();
        } else if (request.target() == "/requestSubnetHost") {
            // Extract input parameters from the request body
            std::string requestBody = request.body();
            std::stringstream requestBodyStream;
            requestBodyStream << requestBody;
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(requestBodyStream, pt);

            auto request_subnet_host_response = requestSubnetHost(pt, dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, request_subnet_host_response);
            response = out_stream.str();
        } else if (request.target() == "/deleteSubnet") {
            // Extract input parameters from the request body
            std::string requestBody = request.body();
            std::stringstream requestBodyStream;
            requestBodyStream << requestBody;
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(requestBodyStream, pt);

            auto delete_host_response = deleteSubnet(pt, dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, delete_host_response);
            response = out_stream.str();
        } else if (request.target() == "/deleteSubnetHost") {
            // Extract input parameters from the request body
            std::string requestBody = request.body();
            std::stringstream requestBodyStream;
            requestBodyStream << requestBody;
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(requestBodyStream, pt);

            auto delete_host_response = deleteSubnetHost(pt, dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, delete_host_response);
            response = out_stream.str();
        } else if (request.target() == "/getNetworkIP") {
            // Extract input parameters from the request body
            std::string requestBody = request.body();
            std::stringstream requestBodyStream;
            requestBodyStream << requestBody;
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(requestBodyStream, pt);

            auto get_network_ip_response = getNetworkIP(pt, dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, get_network_ip_response);
            response = out_stream.str();
        } else if (request.target() == "/getHostIpAddress") {
            // Extract input parameters from the request body
            std::string requestBody = request.body();
            std::stringstream requestBodyStream;
            requestBodyStream << requestBody;
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(requestBodyStream, pt);

            auto get_host_ip_response = getHostIpAddress(pt, dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, get_host_ip_response);
            response = out_stream.str();
        } else if (request.target() == "/getMacAddressOfHost") {
            // Extract input parameters from the request body
            std::string requestBody = request.body();
            std::stringstream requestBodyStream;
            requestBodyStream << requestBody;
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(requestBodyStream, pt);

            auto get_mac_addrress_response = getMacAddressOfHost(pt, dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, get_mac_addrress_response);
            response = out_stream.str();
        } else if (request.target() == "/optimizeSubnetAllocationSpace") {

            auto optimize_free_slot_reponse = optimizeSubnetAllocationSpace(dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, optimize_free_slot_reponse);
            response = out_stream.str();
        }  else if (request.target() == "/renewHost") {
            // Extract input parameters from the request body
            std::string requestBody = request.body();
            std::stringstream requestBodyStream;
            requestBodyStream << requestBody;
            boost::property_tree::ptree pt;
            boost::property_tree::read_json(requestBodyStream, pt);

            auto optimize_free_slot_reponse = renewHost(pt, dsModelImpl);

            // Convert output to string
            std::stringstream out_stream;
            boost::property_tree::write_json(out_stream, optimize_free_slot_reponse);
            response = out_stream.str();
        } else {
            response = "404 Not Found";
        }
    } else {
        response = "400 Bad Request";
    }

    return response;
}

int main(int argc, char* argv[]){


    DSModelType dsModelType;

    if (argc > 1) {
        std::string implementation = argv[1];
       

        if (implementation == "array") {
            dsModelType = DSModelType::ARRAY;
            std::cout << "Using array implementation" << std::endl;
        } else if (implementation == "bst") {
            dsModelType = DSModelType::TREE;
            std::cout << "Using Binary search tree implementation" << std::endl;
        } else if (implementation == "veb") {
            dsModelType = DSModelType::VEB_TREE;
            std::cout << "Using Van Emde Boas Tree implementation" << std::endl;
        } else if (implementation == "map") {
            dsModelType = DSModelType::HASHED_VEB_TREE;
            std::cout << "Using Van Emde Boas Tree with Hash Map implementation" << std::endl;
        } else {
            std::cout << "Please enter valid argument to start with." << std::endl;
            exit(0);
        }

    } else {
        std::cout << "Please enter valid argument to start with." << std::endl;
        exit(0);
    }
    
    DSModelSingleton& dsModelSingleton = DSModelSingleton::GetSingletonInstance(dsModelType);
    globalDSModelType = dsModelType;
    DSModelmpl& dsModelImpl = dsModelSingleton.GetDSModel();

    net::io_context ioContext;

    net::ip::tcp::acceptor acceptor(ioContext, net::ip::tcp::endpoint(net::ip::tcp::v4(), PORT));

    std::cout << "HTTP Server listening on port " << PORT << std::endl;

    while (true) {

        net::ip::tcp::socket socket(ioContext);
        acceptor.accept(socket);

        // read request
        beast::flat_buffer buffer;
        http::request<http::string_body> request;
        http::read(socket, buffer, request);

        HttpServer httpServer;

        // process and generate response
        std::string response =  httpServer.generateResponse(request, dsModelImpl);

        // sending response
        http::response<http::string_body> httpResponse{http::status::ok, request.version()};
        httpResponse.set(http::field::server, "HTTP Server mimicking DHCP server");
        httpResponse.set(http::field::content_type, "text/html");
        httpResponse.body() = response;
        httpResponse.prepare_payload();

        http::write(socket, httpResponse);

        // close socket
        beast::error_code ec;
        socket.shutdown(net::socket_base::shutdown_both, ec);
        socket.close(ec);
    }

    return 0;

}