project(los)

add_executable(los GameLayer.cpp MapLayer.cpp Map.cpp MapParser.cpp main.cpp GameScene.cpp)

install(TARGETS los RUNTIME DESTINATION bin)

target_link_libraries(los z tinyxml SDL2 SDL2_image SDL2_net trutle GL GLEW GLU tmxparser)
add_definitions(-std=c++11)