cmake_minimum_required(VERSION 3.5)

if ($ENV{GITHUB_RUN_NUMBER})
    project (bayan VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project (bayan VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS program_options regex REQUIRED)

file(GLOB bayan_file_cpp
"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(bayan ${bayan_file_cpp})

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 17    
    CXX_STANDARD_REQUIRED ON    
)
target_link_libraries(bayan Boost::program_options
                            Boost::regex)
                            
target_include_directories(bayan
    PRIVATE
    include
    "${CMAKE_BINARY_DIR}")

install(TARGETS bayan RUNTIME DESTINATION bin)