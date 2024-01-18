cmake_minimum_required(VERSION 3.26)
project(Yknaa)

set(CMAKE_CXX_STANDARD 23)

add_executable(Yknaa main.cpp
        Database/createDatabase.c
        Database/createDatabase.h
        Controller/deck.c
        Controller/card.c
        Controller/card.h
        Controller/deck.h
        View/front.cpp
        View/front.h)


find_package(PkgConfig REQUIRED)
#find_package(CURL REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(Yknaa PRIVATE Qt6::Core Qt6::Widgets)
#target_include_directories(Yknaa PRIVATE ${CURL_INCLUDE_DIRS})
#target_link_libraries(Yknaa PRIVATE ${CURL_LIBRARIES})
target_link_libraries(Yknaa PRIVATE sqlite3)