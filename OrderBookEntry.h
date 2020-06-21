//
// Created by deedeeaar on 30/04/2020.
//

#ifndef MERKLEREX_ORDERBOOKENTRY_H
#define MERKLEREX_ORDERBOOKENTRY_H

#include <string>
#include <sstream>

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
    static OrderBookType stringToOrderBookType(const std::string& std);

    static bool compareByTimestamp(const OrderBookEntry& e1, const OrderBookEntry& e2)
    {
        return e1.timestamp < e2.timestamp;
    }
    static bool compareByPriceAsc(const OrderBookEntry& e1, const OrderBookEntry& e2)
    {
        return e1.price < e2.price;
    }
    static bool compareByPriceDesc(const OrderBookEntry& e1, const OrderBookEntry& e2)
    {
        return e1.price > e2.price;
    }

    std::string toString() const;

public:
    double price;
    double amount;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
    std::string username;
};


#endif //MERKLEREX_ORDERBOOKENTRY_H
