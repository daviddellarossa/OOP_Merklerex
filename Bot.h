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
#include "RollingAverageManager.h"



class Bot : public IActor, public IBotControl {

private:
    std::string m_currentTime;
    const OrderBook& m_orderBook;
    const Wallet& m_wallet;
    mutable std::ofstream m_logger;

    std::map<std::string, RollingAverageManager> m_avgPrices;
    std::vector<OrderBookEntry> m_historicalSells;
    std::vector<OrderBookEntry> m_historicalAsks;
    std::vector<OrderBookEntry> m_historicalBids;

public:
    Bot(OrderBook& orderBook, Wallet& wallet);

    void processFrame(const std::string& currentTime) override;
    void saleCompleted(const OrderBookEntry& obe) override;

    //Interface IBotControl
    void logWallet() const override ;
    void logBids() const override ;
    void logAsks() const override ;
    void logSales() const override ;

    const BotRemoteControl GetRemote();
};


#endif //MERKLEREX_BOT_H
