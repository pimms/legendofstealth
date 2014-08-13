cmake_minimum_required(VERSION 2.8)

project(los)

file(GLOB game_src "client/*.h" "client/*.cpp" "shared/*.h" "shared/*.cpp")
file(GLOB serv_src "server/*.h" "server/*.cpp" "shared/*.h" "shared/*.cpp")

add_executable(los ${game_src})
target_link_libraries(los z tinyxml SDL2 SDL2_image SDL2_net trutle GL GLEW GLU tmxparser Box2D)

add_executable(srv ${serv_src})
target_link_libraries(srv trutle SDL2 SDL2_net)

install(TARGETS los RUNTIME DESTINATION .)
install(TARGETS srv RUNTIME DESTINATION .)

add_definitions(-std=c++11)
add_definitions(-I${CMAKE_CURRENT_SOURCE_DIR}/shared)
