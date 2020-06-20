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

class Bot : public IActor, public IBotControl {
    std::string m_currentTime;
    const OrderBook& m_orderBook;
    const Wallet& m_wallet;


    bool m_isEnabled = false;
public:
    Bot(const OrderBook& orderBook, const Wallet& wallet);

    //Interface IActor
    std::function<void()> enterAsk_Event;
    std::function<void()> enterBid_Event;
    std::function<void()> gotoNextTimeFrame_Event;
    std::function<void()> quitRequest_Event;

    void processFrame(std::string currentTime) override;

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
