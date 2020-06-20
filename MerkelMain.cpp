//
// Created by deedeeaar on 30/04/2020.
//

#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include <limits>
#include <functional>
#include "CSVReader.h"



MerkelMain::MerkelMain() :
            bot(orderBook, wallet),
            botRemoteControl(bot.GetRemote()),
            clInterpreter(orderBook, wallet, botRemoteControl) {
    clInterpreter.quitRequest_Event = [this]{ this->quitRequest_EventHandler(); };
    clInterpreter.gotoNextTimeFrame_Event = [this]{ this->gotoNextTimeFrame_EventHandler(); };
    clInterpreter.enterAsk_Event = [this]{ this->enterAsk_EventHandler(); };
    clInterpreter.enterBid_Event = [this]{ this->enterBid_EventHandler(); };

    //bot is not allowed to terminate the application
//    bot.quitRequest_Event = [this]{ this->quitRequest_EventHandler(); };
    bot.gotoNextTimeFrame_Event = [this]{ this->gotoNextTimeFrame_EventHandler(); };
    bot.enterAsk_Event = [this]{ this->enterAsk_EventHandler(); };
    bot.enterBid_Event = [this]{ this->enterBid_EventHandler(); };
}

void MerkelMain::init(){
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);
    auto a = botRemoteControl.isEnabled();
    while(keepRunning){
        bot.processFrame(currentTime);
        clInterpreter.processFrame(currentTime);
    }
}

void MerkelMain::enterAsk(){
    std::cout << "Make an ask - enter the amount: product,price,amount, eg: ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenize(input, ',');
    if(tokens.size() != 3){
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }else{
        try{
            OrderBookEntry obe = CSVReader::stringsToOBE(
                    tokens[1],
                    tokens[2],
                    currentTime,
                    tokens[0],
                    OrderBookType::ask
            );
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch(const std::exception& ex){
            std::cout << "MerkelMain::enterAsk - Bad input" << std::endl;
        }
    }
    std::cout << "You typed:" << input << std::endl;
}

void MerkelMain::enterBid(){
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
                    currentTime,
                    tokens[0],
                    OrderBookType::bid
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }

}

void MerkelMain::gotoNextTimeFrame(){
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }

    }
    currentTime = orderBook.getNextTime(currentTime);
}

void MerkelMain::enterAsk_EventHandler() {
    enterAsk();
}

void MerkelMain::enterBid_EventHandler() {
    enterBid();
}

void MerkelMain::gotoNextTimeFrame_EventHandler() {
    gotoNextTimeFrame();
}

void MerkelMain::quitRequest_EventHandler() {
    keepRunning = false;
}