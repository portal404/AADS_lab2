cmake_minimum_required(VERSION 3.29)
project(LabBLAS)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME LabMatrix) #Создаем обычную локальную переменную с именем проекта
project(${PROJECT_NAME}) # Название проекта

set(application MatrixApp) #Переменная с именем приложения
set(library MatrixLib) #Переменная с именем библиотеки

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE) #Создается переменная CMAKE_CONFIGURATION_TYPES со значением "Debug;Release" и меткой/разделом Configs, переменная сохраниется в специальном файле доступном из всех файлов,
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(lib)
add_subdirectory(main)
add_subdirectory(gtest)
add_subdirectory(maintest)