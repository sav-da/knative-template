# Создаем библиотеку из исходников
add_library(part part.cpp)

# Указываем, где лежат заголовочные файлы
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
