cmake_minimum_required(VERSION 2.8)
set(CMAKE_SUPPRESS_REGENERATION true)
project(estudo-mpr)

set(ITK_DIR "C:/itk_nova/ITK_build")

# Find ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

file(GLOB my_source_files "*.h" "*.cpp" "*.cxx")
add_executable(mpr ${my_source_files} )  
#bota vtk e ITK
target_link_libraries(mpr   ${ITK_LIBRARIES})
#Adds OpenGL
find_package(OpenGL)
target_link_libraries(mpr ${OPENGL_LIBRARIES})
#Adds GLEW
include_directories(C:/lib/glew-2.0.0/include)
target_link_libraries(mpr C:/lib/glew-2.0.0/lib/Release/x64/glew32.lib)

#Adds GLFW
include_directories(C:/lib/glfw/include)
target_link_libraries(mpr debug C:/lib/glfw_build/src/Debug/glfw3.lib)
target_link_libraries(mpr optimized C:/lib/glfw_build/src/Release/glfw3.lib)

#boost.org
SET (BOOST_INCLUDE_DIR "C:/boost_1_64_0")
include_directories(${BOOST_INCLUDE_DIR})
target_link_libraries(mpr "C:/boost_1_64_0/stage/win32/lib/libboost_signals-vc120-mt-1_64.lib")

#suprime o warning chato dos infernos que a ITK gera por usar funções inseguras da api do windows
add_definitions(-D_SCL_SECURE_NO_WARNINGS)



