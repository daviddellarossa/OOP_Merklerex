//
// Created by deedeeaar on 19/06/2020.
//

#include <iostream>

#include "CLInterpreter.h"

CLInterpreter::CLInterpreter(
        const OrderBook &orderBook,
        const Wallet &wallet,
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
            break;
        case 4:
            enterBid();
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
        case 8:
            if(m_botRemoteControl.logWallet)
                m_botRemoteControl.logWallet();
            break;
        case 9:
            if(m_botRemoteControl.logBids)
                m_botRemoteControl.logBids();
            break;
        case 10:
            if(m_botRemoteControl.logAsks)
                m_botRemoteControl.logAsks();
            break;
        case 11:
            if(m_botRemoteControl.logSales)
                m_botRemoteControl.logSales();
            break;
        case 12:
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
    //8 let bot log wallet
    std::cout << "8: Let Bot log the wallet" << std::endl;
    //9 let bot log bids
    std::cout << "9: Let Bot log its bids" << std::endl;
    //10 let bot log asks
    std::cout << "10: Let Bot log its asks" << std::endl;
    //11 let bot log sales
    std::cout << "11: Let Bot log its sales" << std::endl;
    // 12 quit
    std::cout << "12: Quit" << std::endl;
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
        // No action
    }
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
            obe.m_username = "simuser";
            enterAsk_Event(obe);
        }catch(const std::exception& ex){
            std::cout << "MerkelMain::enterAsk - Bad input" << std::endl;
        }
    }
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
            obe.m_username = "simuser";
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
