//
// Created by deedeeaar on 30/04/2020.
//

#ifndef MERKLEREX_MERKELMAIN_H
#define MERKLEREX_MERKELMAIN_H

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain {
public:
    MerkelMain(){

    }
    /** Call this to start the sim */
    void init();

private:
    void printMenu();
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();
    void gotoNextTimeFrame();
    int getUserOption();
    void processUserOption(int userOption);

    std::string currentTime;
    OrderBook orderBook{"20200317.csv"};
    Wallet wallet;
};


#endif //MERKLEREX_MERKELMAIN_H
