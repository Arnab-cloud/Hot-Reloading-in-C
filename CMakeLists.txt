cmake_minimum_required(VERSION 3.25)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(exmplore_raylib)

add_executable(main)
target_sources(main PRIVATE main.c game.c)

add_subdirectory(vendored/raylib EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE raylib)
target_link_libraries(main PRIVATE libdl)

# Install the executable
install(TARGETS main
    RUNTIME DESTINATION bin
)

# Install raylib DLL automatically
install(TARGETS raylib
    RUNTIME DESTINATION bin
)

# assets
# install(DIRECTORY assets/
#     DESTINATION assets
# )
