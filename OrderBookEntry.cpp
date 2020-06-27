//
// Created by deedeeaar on 30/04/2020.
//

#include "OrderBookEntry.h"

#include <utility>

OrderBookEntry::OrderBookEntry(
        double price,
        double amount,
        std::string timestamp,
        std::string product,
        OrderBookType orderType,
        std::string _username) :
        m_price(price),
        m_amount(amount),
        m_timestamp(std::move(timestamp)),
        m_product(std::move(product)),
        m_orderType(orderType),
        m_username(_username) {

}
OrderBookType OrderBookEntry::stringToOrderBookType(const std::string& std){
    if(std == "bid") return OrderBookType::bid;
    else if(std == "ask") return OrderBookType::ask;
    else return OrderBookType::unknown;
}

std::string OrderBookEntry::toString() const {
    std::stringstream ss;
    switch(m_orderType){
        case OrderBookType::ask:
            ss << "Ask: ";
            break;
        case OrderBookType::bid:
            ss << "Bid: ";
            break;
        case OrderBookType::asksale:
            ss << "Asksale: ";
            break;
        case OrderBookType::bidsale:
            ss << "Bidsale: ";
            break;
        default:
            ss << "Unknown: ";
            break;
    }
    ss << m_product << "," << m_price << "," << m_amount;
    return ss.str();
}