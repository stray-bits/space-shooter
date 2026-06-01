cmake_minimum_required(VERSION 3.16)

project(SpaceShooter LANGUAGES CXX)

# ---------- C++ STANDARD ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- SFML ----------
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# ---------- SOURCES ----------
add_executable(SpaceShooter
    main.cpp
    Ship.cpp
    Enemy.cpp
    BossEnemy.cpp
    Bullet.cpp
    GameFunctions.cpp
)

# ---------- INCLUDES ----------
target_include_directories(SpaceShooter PRIVATE
    ${SFML_INCLUDE_DIR}
)

# ---------- LINK ----------
target_link_libraries(SpaceShooter
    sfml-graphics
    sfml-window
    sfml-system
)

# ---------- WINDOWS FIX ----------
if (WIN32)
    add_custom_command(TARGET SpaceShooter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/SFML-2.5.1/bin"
        $<TARGET_FILE_DIR:SpaceShooter>
    )
endif()
