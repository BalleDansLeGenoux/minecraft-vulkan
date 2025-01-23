cmake_minimum_required(VERSION 3.10)
project(VulkanApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")

# Dossiers
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(BUILD_DIR bin)

# Récupération des fichiers source
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.h")

# Création de l'exécutable
add_executable(app ${SOURCES})

# Inclure les headers
target_include_directories(app PRIVATE ${INCLUDE_DIR})

# Ajout des bibliothèques nécessaires
target_link_libraries(app PRIVATE glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi)

# Définir le dossier de sortie
set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
