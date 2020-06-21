cmake_minimum_required(VERSION 3.16)
project(Merklerex)

set(CMAKE_CXX_STANDARD 17)

add_executable(Merklerex
        main.cpp
        main2.cpp
        OrderBookEntry.cpp
        OrderBookEntry.h
        MerkelMain.cpp
        MerkelMain.h
        CSVReader.h
        CSVReader.cpp OrderBook.h OrderBook.cpp Wallet.cpp Wallet.h IActor.h CLInterpreter.cpp CLInterpreter.h Bot.cpp Bot.h IBotControl.h BotRemoteControl.h SimpleMovingAverage.cpp SimpleMovingAverage.h)

file(COPY ${CMAKE_SOURCE_DIR}/20200317.csv DESTINATION ${CMAKE_BINARY_DIR})