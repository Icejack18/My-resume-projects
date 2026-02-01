# Minimi vaadittu CMaken versio
cmake_minimum_required(VERSION 3.18)

# Projektin nimi. (Visual studion solutionin nimi) Muuta halutessasi mielekkäämmäksi.
project(JohdantoPelimoottoriohjelmointiin)

# Enabloi C++ 20 tuki
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML-asetukset
# Lisää freetype solutioniin:
# add_subdirectory("ext/freetype-2.13.2")		   # No need yet

# Lisää SFML solutioniin:
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)     # Käännä SFML staattisena kirjastona
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)      # Disabloi SFML:n Audio, koska koulun koneilla ei ole OpenAL asennettu
add_subdirectory("ext/SFML-master")                 # Tämä lisää SFML kirjaston ext kansiosta.


# add_executable:lla voi lisätä uusia executable 'projekteja' solutioniin.
add_executable(hello_world src/main_hello_world.cpp)
add_executable(main_matikkapeli_v1 src/main_matikkapeli_v1.cpp)
add_executable(main_matikkapeli_v2 src/main_matikkapeli_v2.cpp)
# add_executable(main_game_object_test src/main_game_object_test.cpp)

# SFML Testi-ikkuna sovellus
add_executable(sfml_window  src/main_sfml_window.cpp)
# Laita sfml_window executable "riippuvaiseksi" sfml-graphics ja freetype kirjastoista
target_link_libraries(sfml_window sfml-graphics freetype)

# Tämä kopioi sfml_window projektia kännettessä kaikki assetit
# "assets"-kansiosta build -kansioon automaattisesti.
add_custom_command(TARGET sfml_window POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_CURRENT_SOURCE_DIR}/assets"
	"${PROJECT_BINARY_DIR}/assets"
	COMMENT "Copying asset files to binary directory")

# Nämä kaksi seuraavaa riviä määrittää 2 muuttujaa Cmakelle: ENGINE_INC_FILES ja ENGINE_SRC_FILES
# ENGINE_INC_FILES muuttujaan haetaan kaikki .h tiedostot include/engine-kansiosta ja
# ENGINE_SRC_FILES muuttujaan haetaan kaikki .cpp tiedostot src/engine.
file(GLOB_RECURSE ENGINE_INC_FILES "include/engine/*.h")
file(GLOB_RECURSE ENGINE_SRC_FILES "src/engine/*.cpp")

# Määritä "Additional include directory", jotta voidaan incluudata enginen headereitä tyyliin <engine/application.h>
include_directories("include")

# Enginen v1 testisovellus, joka käyttää yllä määritettyjä muuttujia. Tämä siis toisinsanoen lisää
# header ja cpp filet käännökseen mukaan.

### Removed in hopes of fixing pipeline
#add_executable(test_engine_v1  src/main_test_engine_v1.cpp ${ENGINE_INC_FILES} ${ENGINE_SRC_FILES})
#target_link_libraries(test_engine_v1 sfml-graphics)

#add_executable(test_engine_v2  src/main_test_engine_v2.cpp ${ENGINE_INC_FILES} ${ENGINE_SRC_FILES})
#target_link_libraries(test_engine_v2 sfml-graphics)

#add_executable(test_engine_v3  src/main_test_engine_v3.cpp ${ENGINE_INC_FILES} ${ENGINE_SRC_FILES})
#target_link_libraries(test_engine_v3 sfml-graphics)

add_executable(Mygame  src/main_mygame_v1.cpp ${ENGINE_INC_FILES} ${ENGINE_SRC_FILES})
target_link_libraries(Mygame sfml-graphics)
