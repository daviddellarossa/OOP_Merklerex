//
// Created by deedeeaar on 19/06/2020.
//

#include <iostream>

#include "CLInterpreter.h"

CLInterpreter::CLInterpreter(
        OrderBook &orderBook,
        Wallet &wallet,
        const BotRemoteControl& botRemoteControl) :
        m_orderBook{orderBook},
        m_wallet{wallet},
        m_botRemoteControl{botRemoteControl}{

}


void CLInterpreter::processFrame(const std::string& currentTime) {
    m_currentTime = currentTime;
    printMenu();
    auto input = getUserOption();
    processUserOption(input);
}

void CLInterpreter::processUserOption(int userOption){

    switch(userOption){
        case 0:
            std::cout << "Invalid choice. Choose 1-10" << std::endl;
            break;
        case 1:
            printHelp();
            break;
        case 2:
            printMarketStats();
            break;
        case 3:
            enterAsk();
//            if(enterAsk_Event)
//                enterAsk_Event();
            break;
        case 4:
            enterBid();
//            if(enterBid_Event)
//                enterBid_Event();
            break;
        case 5:
            printWallet();
            break;
        case 6:
            if(gotoNextTimeFrame_Event)
                gotoNextTimeFrame_Event();
            break;
        case 7:
            if(m_botRemoteControl.processFrame)
                m_botRemoteControl.processFrame(m_currentTime);
            break;
//        case 8:
//            if(m_botRemoteControl.enable)
//                m_botRemoteControl.enable();
//            break;
//        case 9:
//            if(m_botRemoteControl.disable)
//                m_botRemoteControl.disable();
//            break;
        case 8:
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
    std::cout << "============================" << std::endl;
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
    // 7 get bot status
    std::cout << "7: Let Bot process current frame" << std::endl;
    // 8 quit
    std::cout << "8: Quit" << std::endl;
    std::cout << "============================" << std::endl;

    std::cout << "Current time is:" << m_currentTime << std::endl;
}

void CLInterpreter::printWallet() const {
    std::cout << "Wallet content:" << std::endl;
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
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-10" << std::endl;
    std::getline(std::cin, line);
    std::cout << "You chose: " << line << std::endl;
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        //
    }
//    std::cout << "You chose: " << userOption << std::endl;
    return userOption;

}


void CLInterpreter::enterAsk(){
    std::cout << "Make an ask - enter the amount: product,price,amount, eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenize(input, ',');
    if(tokens.size() != 3){
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }else{
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[1],
                    tokens[2],
                    m_currentTime,
                    tokens[0],
                    OrderBookType::ask
            );
            obe.username = "simuser";
            enterAsk_Event(obe);
        }catch(const std::exception& ex){
            std::cout << "MerkelMain::enterAsk - Bad input" << std::endl;
        }
    }
//    std::cout << "You typed:" << input << std::endl;
}

void CLInterpreter::enterBid(){
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenize(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[1],
                    tokens[2],
                    m_currentTime,
                    tokens[0],
                    OrderBookType::bid
            );
            obe.username = "simuser";
            enterBid_Event(obe);
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }
}

void CLInterpreter::saleCompleted(const OrderBookEntry &obe) {
    std::cout << obe.toString() << std::endl;
}
