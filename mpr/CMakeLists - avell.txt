cmake_minimum_required(VERSION 2.8)
set(CMAKE_SUPPRESS_REGENERATION true)
project(estudo-mpr)

set(ITK_DIR "C:/libs/itk/ITK_bin")
set(VTK_DIR "C:/libs/VTK-8.0.0_bin")

# Find ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# Find VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

file(GLOB my_source_files "*.h" "*.cpp" "*.cxx")
add_executable(mpr ${my_source_files} )  
#bota vtk e ITK
target_link_libraries(mpr   ${ITK_LIBRARIES} ${VTK_LIBRARIES})
#Adds OpenGL
find_package(OpenGL)
target_link_libraries(mpr ${OPENGL_LIBRARIES})
#Adds GLEW
include_directories(C:/libs/glew-2.1.0/include)
target_link_libraries(mpr C:/libs/glew-2.1.0/lib/Release/x64/glew32.lib)

#Adds GLFW
include_directories(C:/libs/glfw/include)
target_link_libraries(mpr debug C:/libs/glfw_bin/src/Debug/glfw3.lib)
target_link_libraries(mpr optimized C:/libs/glfw_bin/src/Release/glfw3.lib)

#boost.org
SET (BOOST_INCLUDE_DIR "C:/libs/boost_1_64_0")
include_directories(${BOOST_INCLUDE_DIR})
target_link_libraries(mpr "C:/libs/boost_1_64_0/stage/lib/libboost_signals-vc140-mt-1_64.lib")

#eigen3
set (EIGEN_INCLUDE_DIR "C:\\libs\\eigen3")
include_directories(${EIGEN_INCLUDE_DIR})

#suprime o warning chato dos infernos que a ITK gera por usar funções inseguras da api do windows
add_definitions(-D_SCL_SECURE_NO_WARNINGS)



