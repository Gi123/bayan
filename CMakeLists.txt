cmake_minimum_required(VERSION 3.15)

if ($ENV{GITHUB_RUN_NUMBER})
    project (Bayan VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project (Bayan VERSION 0.0.1)
endif()

configure_file(version.h.in version.h)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB bayan_file_cpp
 "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

find_package(Boost 1.81.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(bayan 
${bayan_file_cpp}
)
endif()
            
set_target_properties(bayan 
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bayan
    PRIVATE 
    include
    "${CMAKE_BINARY_DIR}"
)
if (MSVC)
    target_compile_options(bayan 
    PRIVATE
    /EHsc /W4 /WX
        )     
 else ()
    target_compile_options(bayan 
    PRIVATE
    -Wextra -pedantic -Werror
        )
endif()


