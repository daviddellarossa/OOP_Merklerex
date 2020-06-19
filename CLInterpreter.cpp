//
// Created by deedeeaar on 19/06/2020.
//

#include <iostream>

#include "CLInterpreter.h"

CLInterpreter::CLInterpreter(const OrderBook &orderBook, const Wallet &wallet) : m_orderBook{orderBook}, m_wallet{wallet} {

}


void CLInterpreter::processFrame(std::string currentTime) {
    m_currentTime = currentTime;
    printMenu();
    auto input = getUserOption();
    processUserOption(input);
}

void CLInterpreter::processUserOption(int userOption) const{

    switch(userOption){
        case 0:
            std::cout << "Invalid choice. Choose 1-6" << std::endl;
            break;
        case 1:
            printHelp();
            break;
        case 2:
            printMarketStats();
            break;
        case 3:
            if(enterAsk_Event)
                enterAsk_Event();
            break;
        case 4:
            if(enterBid_Event)
                enterBid_Event();
            break;
        case 5:
            printWallet();
            break;
        case 6:
            if(gotoNextTimeFrame_Event)
                gotoNextTimeFrame_Event();
            break;
        case 7:
            if(quitRequest_Event)
                quitRequest_Event();
            break;
        default:
            break;
    }


}

void CLInterpreter::printHelp() const {
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers" << std::endl;
}

void CLInterpreter::printMenu() const {
    // 1 print help
    std::cout << "1: Print help" << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an ask" << std::endl;
    // 4 make a bid
    std::cout << "4: Make a bid" << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet" << std::endl;
    // 6 continue
    std::cout << "6: Continue" << std::endl;

    std::cout << "============================" << std::endl;

    std::cout << "Current time is:" << m_currentTime << std::endl;
}

void CLInterpreter::printWallet() const {
    std::cout << m_wallet.toString() << std::endl;
}

void CLInterpreter::printMarketStats() const {
    for(auto const& p : m_orderBook.getKnownProducts()){
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = m_orderBook.getOrders(OrderBookType::ask, p, m_currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;


    }
//    std::cout << "OrderBook contains: " << orders.size() << " entries" << std::endl;
//    unsigned int bids{0};
//    unsigned int asks{0};
//    for(OrderBookEntry& e : orders){
//        if(e.orderType == OrderBookType::bid)
//            ++bids;
//        else if(e.orderType == OrderBookType::ask)
//            ++asks;
//    }
//    std::cout << "Order book asks: " << asks << "; bids: " << bids << std::endl;
}

int CLInterpreter::getUserOption() {
    int userOption;
    std::cout << "Type in 1-6" << std::endl;
    std::cin >> userOption;
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
    /*
     *     int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        //
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
     */


}


