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
    int getUserOption();
    void processUserOption(int userOption);


    void printWallet();

    void enterAsk();
    void enterBid();
    void gotoNextTimeFrame();



    std::string currentTime;
    OrderBook orderBook{"20200317.csv"};
    Wallet wallet;
};


#endif //MERKLEREX_MERKELMAIN_H
