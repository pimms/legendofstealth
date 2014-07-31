cmake_minimum_required(VERSION 2.8)

project(los)

file(GLOB game_src "src/*.h" "src/*.cpp")
add_executable(los ${game_src})

install(TARGETS los RUNTIME DESTINATION .)

target_link_libraries(los z tinyxml SDL2 SDL2_image SDL2_net trutle GL GLEW GLU tmxparser Box2D)
add_definitions(-std=c++11)
