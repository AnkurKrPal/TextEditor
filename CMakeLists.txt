# cmake_minimum_required(VERSION 3.21)
# project(TextEditor)
# set(CMAKE_PREFIX_PATH "/Users/prajwal/Qt/6.9.3/macos/lib/cmake")

# # find_package(Qt6 REQUIRED COMPONENTS Widgets)
# # set(CMAKE_CXX_STANDARD 17)
# #list(APPEND CMAKE_PREFIX_PATH "/Users/prajwal/Qt/6.9.3/macos/lib/cmake")

# find_package(Qt6 COMPONENTS Widgets REQUIRED)

# qt_standard_project_setup()

# add_executable(TextEditor
#     gui/main.cpp
#     gui/mainwindow.cpp
# )

# target_link_libraries(TextEditor PRIVATE Qt6::Widgets)
# cmake_minimum_required(VERSION 3.5)
# project(TextEditor)

# set(CMAKE_PREFIX_PATH "/Users/prajwal/Qt/6.9.3/macos/lib/cmake")

# find_package(Qt6 REQUIRED COMPONENTS Widgets)

# add_executable(TextEditor main.cpp)
# target_link_libraries(TextEditor PRIVATE Qt6::Widgets)

cmake_minimum_required(VERSION 3.19)
project(TextEditor)

set(CMAKE_PREFIX_PATH "/Users/prajwal/Qt/6.9.3/macos/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets)