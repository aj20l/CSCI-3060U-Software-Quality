cmake_minimum_required(VERSION 3.10.0)
project(main VERSION 0.1.0)

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