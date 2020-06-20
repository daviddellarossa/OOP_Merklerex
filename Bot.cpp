//
// Created by deedeeaar on 20/06/2020.
//

#include "Bot.h"

Bot::Bot(const OrderBook &orderBook, const Wallet &wallet) : m_orderBook{orderBook}, m_wallet{wallet} {

}

void Bot::processFrame(std::string currentTime) {
    if(m_isEnabled == false) return;
    m_currentTime = currentTime;
}

void Bot::logWallet() const {
    std::cout << m_wallet.toString() << std::endl;
}

void Bot::logBids() const {
    std::cout << "logBids not implemented" << std::endl;
}

void Bot::logAsks() const {
    std::cout << "logAsks not implemented" << std::endl;
}

void Bot::logSales() const {
    std::cout << "logSales not implemented" << std::endl;
}

const BotRemoteControl Bot::GetRemote() {
    BotRemoteControl remote;
    remote.disable = [this]{ this->disable(); };
    remote.enable = [this]{ this->enable(); };
    remote.isEnabled = [this]{ return this->m_isEnabled; };
    remote.logAsks = [this]{ return this->logAsks(); };
    remote.logBids = [this]{ return this->logBids(); };
    remote.logSales = [this]{ return this->logSales(); };
    remote.logWallet = [this]{return this->logWallet(); };
    return remote;
}
