project("MPU-9255")


find_package (Eigen3 REQUIRED NO_MODULE)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE Eigen3::Eigen
)
