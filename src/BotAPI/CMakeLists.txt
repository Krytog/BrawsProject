add_library(bot_api
        BotManagement.cpp
        BotInputSystem.cpp)

# include Core
target_include_directories(bot_api PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)