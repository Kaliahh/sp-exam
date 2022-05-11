cmake_minimum_required(VERSION 3.22)
project(sp_exam)

set(CMAKE_CXX_STANDARD 20)

file(DOWNLOAD https://raw.githubusercontent.com/onqtam/doctest/v2.4.8/doctest/doctest.h doctest.h)
add_library(doctest INTERFACE doctest.h)
target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_BINARY_DIR})

add_library(stock_trading src/main.cpp src/stock_parser.h src/stock.h src/trade.h include/json.h include/date.h src/candle_stick.h)

add_executable(sp_exam src/main.cpp)
add_executable(test_doc test/test_stock_trading.cpp)

target_link_libraries(test_doc PRIVATE doctest)


enable_testing()
add_test(NAME test_doc COMMAND test_doc)