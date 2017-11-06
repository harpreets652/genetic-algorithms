PROJECT(main)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules")

SET(CXX11_FLAGS -std=gnu++11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")

FILE(GLOB_RECURSE SOURCES "src/*.cpp")
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

add_custom_target("${PROJECT_NAME}_SUCCESSFUL" ALL
                  DEPENDS ${PROJECT_NAME}
                  COMMAND ${CMAKE_COMMAND} -E echo ""
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "  Compile complete!"
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_CURRENT_BINARY_DIR}"
                 )

# TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARY} ${SDL2_LIBRARY} ${ASSIMP_LIBRARY} ${ImageMagick_LIBRARIES})
