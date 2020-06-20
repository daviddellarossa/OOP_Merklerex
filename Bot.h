//
// Created by deedeeaar on 20/06/2020.
//

#ifndef MERKLEREX_BOT_H
#define MERKLEREX_BOT_H

#include "IActor.h"
#include "Wallet.h"
#include "OrderBook.h"

class Bot : IActor {
    std::string m_currentTime;
    const OrderBook& m_orderBook;
    const Wallet& m_wallet;

    void logWallet() const;
    void logBids() const;
    void logAsks() const;
    void logSales() const;
public:
    Bot(const OrderBook& orderBook, const Wallet& wallet);
    std::function<void()> enterAsk_Event;
    std::function<void()> enterBid_Event;
    std::function<void()> gotoNextTimeFrame_Event;
    std::function<void()> quitRequest_Event;
    void processFrame(std::string currentTime) override;
};


#endif //MERKLEREX_BOT_H
