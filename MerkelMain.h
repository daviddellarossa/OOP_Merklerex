//
// Created by deedeeaar on 30/04/2020.
//

#ifndef MERKLEREX_MERKELMAIN_H
#define MERKLEREX_MERKELMAIN_H

#include <vector>
#include <memory>
#include <fstream>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "IActor.h"
#include "CLInterpreter.h"
#include "Bot.h"

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

//    void enterAsk();
//    void enterBid();
    void gotoNextTimeFrame();

    bool keepRunning{true};
    std::string currentTime;
    OrderBook orderBook; //initialization moved into the constructor
    Wallet wallet;
    CLInterpreter clInterpreter;
    Bot bot;
    BotRemoteControl botRemoteControl;
};


#endif //MERKLEREX_MERKELMAIN_H
