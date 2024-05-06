cmake_minimum_required(VERSION 3.24)
project(strata_sol_qualification LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)


set(QT_VERSION 5 CACHE STRING "Used qt version")


set(Qt5Libs Core Widgets)
#set(Qt6Libs ...)
find_package(Qt${QT_VERSION} COMPONENTS ${Qt${QT_VERSION}Libs} REQUIRED)

find_package(Boost COMPONENTS headers REQUIRED)

add_subdirectory(external/qcustomplot)
add_subdirectory(external/range-v3)


add_subdirectory(strata_sol_qualification)
