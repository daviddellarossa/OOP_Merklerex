//
// Created by deedeeaar on 30/04/2020.
//

#ifndef MERKLEREX_ORDERBOOKENTRY_H
#define MERKLEREX_ORDERBOOKENTRY_H

#include <string>

enum class OrderBookType{bid, ask, unknown, asksale, bidsale};

class OrderBookEntry {

public:
    OrderBookEntry(
            double price,
            double amount,
            std::string timestamp,
            std::string product,
            OrderBookType orderType,
            std::string username = "dataset");
    static OrderBookType stringToOrderBookType(const std::string std);

    static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.timestamp < e2.timestamp;
    }
    static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.price < e2.price;
    }
    static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2)
    {
        return e1.price > e2.price;
    }

public:
    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
    std::string username;
};


#endif //MERKLEREX_ORDERBOOKENTRY_H
