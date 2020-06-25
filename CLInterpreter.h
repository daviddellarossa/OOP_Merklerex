//
// Created by deedeeaar on 19/06/2020.
//

#ifndef MERKLEREX_CLINTERPRETER_H
#define MERKLEREX_CLINTERPRETER_H

#include "IActor.h"
#include "Wallet.h"
#include "OrderBook.h"
#include "BotRemoteControl.h"

class CLInterpreter : public IActor {


private:
    int getUserOption();
    void processUserOption(int userOption);
    void printHelp() const;
    void printMenu() const;
    void printWallet() const;
    void printMarketStats() const;

    void enterAsk();
    void enterBid();

    std::string m_currentTime;
    OrderBook& m_orderBook;
    Wallet& m_wallet;
    const BotRemoteControl& m_botRemoteControl;
public:
    CLInterpreter(OrderBook& orderBook, Wallet& wallet, const BotRemoteControl& botRemoteControl);

    //IActor interface
    void processFrame(const std::string& currentTime) override;
    void saleCompleted(const OrderBookEntry& obe) override;
};


#endif //MERKLEREX_CLINTERPRETER_H
