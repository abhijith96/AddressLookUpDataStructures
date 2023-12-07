This is the DHCP Allocator project.

Requirements some C++ compiler, cmake, conan ,python

If u have not configured conan package manager in your system download the conan_config.zip file, then enter the following command
    conan config install conan_config.zip
this command only needs to be run once, ideally right after installing conan.

To create debug build enter the following commands
    mkdir cmake-build-debug
    cd cmake-build-debug
    conan install .. -pr=macos_armv8_debug --output-folder=dependencies --build=missing

To create release build enter the following commands
        mkdir cmake-build-release
        cd cmake-build-release
        conan install .. -pr=macos_armv8 --output-folder=dependencies --build=missing


Create at least a build before loading

To load the project right click on the root CMakeLists.txt file (DHCPAllocator/CMakeLists.txt) and select load cmake project.
