//
// Created by deedeeaar on 20/06/2020.
//

#ifndef MERKLEREX_BOT_H
#define MERKLEREX_BOT_H

#include "IActor.h"
#include "IBotControl.h"
#include "Wallet.h"
#include "OrderBook.h"
#include "BotRemoteControl.h"
#include <fstream>
#include <map>
#include <string>
#include <array>
#include "SimpleMovingAverage.h"



class Bot : public IActor, public IBotControl {

private:
    void enterAsk(const OrderBookEntry& obe);
    void enterBid(const OrderBookEntry& obe);

    constexpr static unsigned int AVGARRSIZE = 4;
    std::string m_currentTime;
    OrderBook& m_orderBook;
    Wallet& m_wallet;
    mutable std::ofstream m_logger;
    bool m_isEnabled = false;

    /**
     * Contains the last AVGARRSIZE average prices for the product.
     * the array contains the average and is accessed with a rollover policy
     * the pair.first integer is the index in the array of the latest average
     */
    std::map<std::string, SimpleMovingAverage> m_avgPrices;

public:
    Bot(OrderBook& orderBook, Wallet& wallet);

    //Interface IActor
//    std::function<void()> enterAsk_Event;
//    std::function<void()> enterBid_Event;
//    std::function<void()> gotoNextTimeFrame_Event;
//    std::function<void()> quitRequest_Event;

    void processFrame(const std::string& currentTime) override;

    //Interface IBotControl
    bool isEnabled() const override { return m_isEnabled; }
    void enable() override{ m_isEnabled = true; }
    void disable() override{ m_isEnabled = false; }
    void logWallet() const override ;
    void logBids() const override ;
    void logAsks() const override ;
    void logSales() const override ;

    const BotRemoteControl GetRemote();
};


#endif //MERKLEREX_BOT_H
