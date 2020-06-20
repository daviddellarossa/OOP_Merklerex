//
// Created by deedeeaar on 30/04/2020.
//

#ifndef MERKLEREX_MERKELMAIN_H
#define MERKLEREX_MERKELMAIN_H

#include <vector>
#include <memory>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "IActor.h"
#include "CLInterpreter.h"

class MerkelMain {
public:
    MerkelMain();
    /** Call this to start the sim */
    void init();

    void enterAsk_EventHandler();
    void enterBid_EventHandler();
    void gotoNextTimeFrame_EventHandler();
    void quitRequest_EventHandler();

private:
//    void printMenu();
//    void printHelp();
//    void printMarketStats();
//    int getUserOption();
//    void processUserOption(int userOption);
//    void printWallet();



    void enterAsk();
    void enterBid();
    void gotoNextTimeFrame();


    bool keepRunning = true;
    std::string currentTime;
    OrderBook orderBook{"20200317.csv"};
    Wallet wallet;
    CLInterpreter clInterpreter;
};


#endif //MERKLEREX_MERKELMAIN_H
