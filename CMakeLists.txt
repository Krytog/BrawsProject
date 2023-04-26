cmake_minimum_required(VERSION 3.16)
project(BrawlStars)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio window REQUIRED)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

# targets and sources
add_subdirectory(src)

# external libraries
# add_subdirectory(external)

# future testing
# enable_testing()
# add_subdirectory(tests)
