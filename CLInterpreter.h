//
// Created by deedeeaar on 19/06/2020.
//

#ifndef MERKLEREX_CLINTERPRETER_H
#define MERKLEREX_CLINTERPRETER_H

#include "IActor.h"
#include "Wallet.h"
#include "OrderBook.h"
#include "BotRemoteControl.h"

/**
 * CLInterpreter, is a Command Line Interpreter. It manages the communication with the user interface, displays
 * messages to the User and reads input messages from the console.
 * It also sends commands to the Bot instance through the BotRemoteControl instance reference it owns.
 * @extends IActor
 */
class CLInterpreter : public IActor {
private:
    /**
     * Read a user selection from the console
     * @return Integer indicating the selected option
     */
    int getUserOption();
    /**
     * Decides which action to follow based on the user selection
     * @param userOption - parameter indicating the user selection
     */
    void processUserOption(int userOption);
    /** Print Help on the console */
    void printHelp() const;
    /** Print the Menu on the console */
    void printMenu() const;
    /** Print the Wallet on the console */
    void printWallet() const;
    /** Print the Market stats on the console */
    void printMarketStats() const;

    /** Create a new OrderBookEntry of type ask, based on the user input, and send it to the observer to be processed */
    void enterAsk();
    /** Create a new OrderBookEntry of type bid, based on the user input, and send it to the observer to be processed */
    void enterBid();

    /** Current  Time frame */
    std::string m_currentTime;
    /** Reference to the OrderBook instance. This class only creates OrderBookEntry instances and sends them to the
     * observer, invoking an event, letting it manage the insertion. Therefore it only needs a constant reference.*/
    const OrderBook& m_orderBook;
    /** Reference to the Wallet instance. This class only invokes const functions upon wallet, therefore it only
     * needs a const reference.*/
    const Wallet& m_wallet;
    /** Reference to the BotRemoteControl. Used to send commands to the Bot instance */
    const BotRemoteControl& m_botRemoteControl;
public:
    /**
     * Create a new instance of CLInterpreter
     * @param orderBook - Constant reference to an instance of OrderBook
     * @param wallet - Constant reference to an instance of Wallet
     * @param botRemoteControl - Constant reference to an instance of BotRemoteControl
     */
    CLInterpreter(const OrderBook& orderBook, const Wallet& wallet, const BotRemoteControl& botRemoteControl);

    //IActor interface
    /** Start the processing of the current frame. Listen for user commands and act accordingly.
     * The interaction with the MerkelMain class is done invoking the events of the IActor interface.
     * @param currentTime - time frame executed
     */
    void processFrame(const std::string& currentTime) override;
    /**
     * Action executed when a new sale is finalized
     * @param obe - OrderBookEntry containing the details of the new sale
     */
    void saleCompleted(const OrderBookEntry& obe) override;
};


#endif //MERKLEREX_CLINTERPRETER_H
