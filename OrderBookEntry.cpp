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
            price(price),
            amount(amount),
            timestamp(std::move(timestamp)),
            product(std::move(product)),
            orderType(orderType),
            username(_username) {

}
OrderBookType OrderBookEntry::stringToOrderBookType(const std::string& std){
    if(std == "bid") return OrderBookType::bid;
    else if(std == "ask") return OrderBookType::ask;
    else return OrderBookType::unknown;
}

std::string OrderBookEntry::toString() const {
    std::stringstream ss;
    switch(orderType){
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
    ss << product << "," << price << "," << amount;
    return ss.str();
}