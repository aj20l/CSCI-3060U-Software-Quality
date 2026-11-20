cmake_minimum_required(VERSION 3.10.0)
project(main VERSION 0.1.0)

# Set the C++ standard to C++17 to support inline variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    src/frontend/store.cpp
    src/frontend/frontend.cpp
    src/frontend/game_manager.cpp
    src/frontend/game.cpp
    src/frontend/payment_processor.cpp
    src/frontend/session.cpp
    src/frontend/user_manager.cpp
    src/frontend/user.cpp
)