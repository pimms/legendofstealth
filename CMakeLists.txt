cmake_minimum_required(VERSION 2.8)

project(los)

# Build the Google Test library
exec_program("bash gtestbuild.sh")

# Assert that the library exists
set(gtest_a ${CMAKE_CURRENT_LIST_DIR}/gtest/build/libgtest.a)
if (NOT EXISTS ${gtest_a})
	message(FATAL_ERROR "Google Test library does not exist.")
endif()


file(GLOB game_src "client/*.h" "client/*.cpp")
file(GLOB serv_src "server/*.h" "server/*.cpp")
file(GLOB shared_src "shared/*.h" "shared/*.cpp")
file(GLOB test_src "test/*.h" "test/*.cpp")

set(game_lib z tinyxml SDL2 SDL2_image SDL2_net trutle GL GLEW GLU tmxparser Box2D)
set(serv_lib trutle SDL2 SDL2_net)
set(test_lib ${game_lib} ${gtest_a} ${game_lib} ${serv_lib} pthread)

 
add_executable(los ${game_src} ${shared_src})
target_link_libraries(los ${game_lib})

add_executable(srv ${serv_src} ${shared_src})
target_link_libraries(srv ${serv_lib})

add_executable(utest ${test_src} ${game_src} ${serv_src} ${shared_src})
target_link_libraries(utest ${test_lib})


install(TARGETS los RUNTIME DESTINATION .)
install(TARGETS srv RUNTIME DESTINATION .)
install(TARGETS utest RUNTIME DESTINATION .)


# Definitions used for all files
add_definitions(-std=c++11)
add_definitions(-I${CMAKE_CURRENT_SOURCE_DIR}/shared)
add_definitions(-I${CMAKE_CURRENT_SOURCE_DIR}/client)
add_definitions(-I${CMAKE_CURRENT_SOURCE_DIR}/server)

# Definitions used specifically for testing
set_target_properties(utest PROPERTIES COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/gtest/include -DTEST")

