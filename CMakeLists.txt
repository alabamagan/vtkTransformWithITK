project(itkMatrixConvert)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(VTK_DIR "/opt/vtk-6.2/")
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES} ${ITK_LIBRARIES})
