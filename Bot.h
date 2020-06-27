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


/**
 * The Bot class implements an automaton able to perform bid and ask operations on the wallet
 * @extends: IActor
 * @extends: IBotControl
 */
class Bot : public IActor, public IBotControl {

private:
    /** Current time-frame */
    std::string m_currentTime;
    /** Reference to the OrderBook instance. This class only creates OrderBookEntry instances and sends them to the
     * observer, invoking an event, letting it manage the insertion. Therefore it only needs a constant reference.*/
    const OrderBook& m_orderBook;
    /** Reference to the Wallet instance. This class only invokes const functions upon wallet, therefore it only
     * needs a const reference.*/
    const Wallet& m_wallet;
    /** Logger */
    mutable std::ofstream m_logger;

    /** Maps each product to an instance of RollingAverageManager */
    std::map<std::string, RollingAverageManager> m_avgPrices;
    /** Historical collection of all Sells made by the bot */
    std::vector<OrderBookEntry> m_historicalSales;
    /** Historical collection of all Asks made by the bot */
    std::vector<OrderBookEntry> m_historicalAsks;
    /** Historical collection of all Bids made by the bot */
    std::vector<OrderBookEntry> m_historicalBids;

public:
    /**
     * Create a new instance of Bot
     * @param orderBook - Constant reference to the OrderBook instance
     * @param wallet - Constant reference to the Wallet instance
     */
    Bot(const OrderBook& orderBook, const Wallet& wallet);

    /** Start the processing during which the bot analyses the orderbook and the wallet and
     * generates new asks and bids. These new OrderBookEntries are sent to the class' observer
     * through the events exposed by the interface IActor and let to be managed by the observer
     * @param currentTime - time frame executed
     */
    void processFrame(const std::string& currentTime) override;
    /**
     * Action executed when a new sale is finalized
     * @param obe - OrderBookEntry containing the details of the new sale
     */
    void saleCompleted(const OrderBookEntry& obe) override;

    // IBotControl interface
    /** Log the Wallet */
    void logWallet() const override ;
    /** Log the bids done by the Bot */
    void logBids() const override ;
    /** Log the asks done by the Bot */
    void logAsks() const override ;
    /** Log the sales done by the Bot */
    void logSales() const override ;

    /** Generate an instance of a BotRemoteControl class, which is a Mediator that can be used to control the bot */
    const BotRemoteControl getRemote();
};


#endif //MERKLEREX_BOT_H
