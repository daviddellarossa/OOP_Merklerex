//
// Created by deedeeaar on 19/06/2020.
//

#ifndef MERKLEREX_CLINTERPRETER_H
#define MERKLEREX_CLINTERPRETER_H

#include "IActor.h"
#include "Wallet.h"
#include "OrderBook.h"

class CLInterpreter : public IActor {


private:
    int getUserOption();
    void processUserOption(int userOption) const;
    void printHelp() const;
    void printMenu() const;
    void printWallet() const;
    void printMarketStats() const;

    std::string m_currentTime;
    const OrderBook& m_orderBook;
    const Wallet& m_wallet;
public:
    CLInterpreter(const OrderBook& orderBook, const Wallet& wallet);
    std::function<void()> enterAsk_Event;
    std::function<void()> enterBid_Event;
    std::function<void()> gotoNextTimeFrame_Event;
    std::function<void()> quitRequest_Event;
    void processFrame(std::string currentTime) override;
};


#endif //MERKLEREX_CLINTERPRETER_H
