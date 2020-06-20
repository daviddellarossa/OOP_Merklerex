//
// Created by deedeeaar on 20/06/2020.
//

#include "Bot.h"

Bot::Bot(const OrderBook &orderBook, const Wallet &wallet) : m_orderBook{orderBook}, m_wallet{wallet} {

}

void Bot::processFrame(std::string currentTime) {
    m_currentTime = currentTime;
}

void Bot::logWallet() const {
    std::cout << m_wallet.toString() << std::endl;
}

void Bot::logBids() const {

}

void Bot::logAsks() const {

}

void Bot::logSales() const {

}