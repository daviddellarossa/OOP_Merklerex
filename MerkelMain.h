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

    /** EventHandler invoked to add a new Ask to the Orderbook */
    void enterAsk_EventHandler(const OrderBookEntry&);
    /** EventHandler invoked to add a new Bid to the Orderbook */
    void enterBid_EventHandler(const OrderBookEntry&);
    /** EventHandler invoked to go to next timeframe */
    void gotoNextTimeFrame_EventHandler();
    /** EventHandler invoked to quit the program */
    void quitRequest_EventHandler();

private:
    /** Enter a new OrderBookEntry of type Ask into the OrderBook */
    void enterAsk(const OrderBookEntry& obe);
    /** Enter a new OrderBookEntry of type Bid into the OrderBook */
    void enterBid(const OrderBookEntry& obe);
    /** Move to the next frame */
    void gotoNextTimeFrame();

    /** Flag used to control the main iteration of the program. Set it to false to quit the program */
    bool m_keepRunning{true};
    /** Current time frame processed */
    std::string m_currentTime;
    /** Instance of OrderBook */
    OrderBook m_orderBook; //initialization moved into the constructor
    /** Instance of Wallet */
    Wallet m_wallet;
    /** Instance of CLInterpreter */
    CLInterpreter m_clInterpreter;
    /** Instance of Bot */
    Bot m_bot;
    /** Instance of BotRemoteControl */
    BotRemoteControl m_botRemoteControl;
};


#endif //MERKLEREX_MERKELMAIN_H
