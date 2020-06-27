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
        return e1.m_timestamp < e2.m_timestamp;
    }
    static bool compareByPriceAsc(const OrderBookEntry& e1, const OrderBookEntry& e2)
    {
        return e1.m_price < e2.m_price;
    }
    static bool compareByPriceDesc(const OrderBookEntry& e1, const OrderBookEntry& e2)
    {
        return e1.m_price > e2.m_price;
    }

    std::string toString() const;

public:
    double m_price;
    double m_amount;
    std::string m_timestamp;
    std::string m_product;
    OrderBookType m_orderType;
    std::string m_username;
};


#endif //MERKLEREX_ORDERBOOKENTRY_H
