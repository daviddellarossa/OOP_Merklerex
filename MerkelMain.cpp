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
        m_orderBook("20200317.csv"),
        m_bot(m_orderBook, m_wallet),
        m_botRemoteControl(m_bot.getRemote()),
        m_clInterpreter(m_orderBook, m_wallet, m_botRemoteControl) {
    m_clInterpreter.quitRequest_Event = [this]{ this->quitRequest_EventHandler(); };
    m_clInterpreter.gotoNextTimeFrame_Event = [this]{ this->gotoNextTimeFrame_EventHandler(); };
    m_clInterpreter.enterAsk_Event = [this](const OrderBookEntry& obe){ this->enterAsk_EventHandler(obe); };
    m_clInterpreter.enterBid_Event = [this](const OrderBookEntry& obe){ this->enterBid_EventHandler(obe); };

    //bot is not allowed to terminate the application
    //bot.quitRequest_Event = [this]{ this->quitRequest_EventHandler(); };
    //bot.gotoNextTimeFrame_Event = [this]{ this->gotoNextTimeFrame_EventHandler(); };
    m_bot.enterAsk_Event = [this](const OrderBookEntry& obe){ this->enterAsk_EventHandler(obe); };
    m_bot.enterBid_Event = [this](const OrderBookEntry& obe){ this->enterBid_EventHandler(obe); };
}

void MerkelMain::init(){
    m_currentTime = m_orderBook.getEarliestTime();

    m_wallet.insertCurrency("BTC", 10);

    while(m_keepRunning){
        m_clInterpreter.processFrame(m_currentTime);
    }
}

void MerkelMain::enterAsk(const OrderBookEntry& obe){
        try{
            if (m_wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                m_orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch(const std::exception& ex){
            std::cout << "MerkelMain::enterAsk - Bad input" << std::endl;
        }
}

void MerkelMain::enterBid(const OrderBookEntry& obe){
        try {
            if (m_wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                m_orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
}

void MerkelMain::gotoNextTimeFrame(){
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : m_orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  m_orderBook.matchAsksToBids(p, m_currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.m_price << " amount " << sale.m_amount << std::endl;
            if (sale.m_username == "simuser")
            {
                // update the wallet
                m_wallet.processSale(sale);
                m_clInterpreter.saleCompleted(sale);
            }
            if (sale.m_username == "bot")
            {
                // update the wallet
                m_wallet.processSale(sale);
                m_bot.saleCompleted(sale);
            }
        }

    }
    m_wallet.clearReserves();
    m_orderBook.removeCustomOrders();
    m_currentTime = m_orderBook.getNextTime(m_currentTime);
}

void MerkelMain::enterAsk_EventHandler(const OrderBookEntry& obe) {
    enterAsk(obe);
}

void MerkelMain::enterBid_EventHandler(const OrderBookEntry& obe) {
    enterBid(obe);
}

void MerkelMain::gotoNextTimeFrame_EventHandler() {
    gotoNextTimeFrame();
}

void MerkelMain::quitRequest_EventHandler() {
    m_keepRunning = false;
}