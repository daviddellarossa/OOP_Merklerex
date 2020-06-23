//
// Created by deedeeaar on 20/06/2020.
//

#include "Bot.h"
#include <numeric>

Bot::Bot(OrderBook &orderBook, Wallet &wallet) : m_orderBook{orderBook}, m_wallet{wallet}, m_logger{"Merklerex_Bot.log", std::ios_base::trunc} {
    m_logger << "Bot instance created" << std::endl;

    auto products = m_orderBook.getKnownProducts();
    for(auto product : products){
        m_avgPrices[product];
    }
}

void Bot::processFrame(const std::string& currentTime) {
    m_currentTime = currentTime;
    m_logger << "Processing frame " << m_currentTime << std::endl;
    //analyze the orderBook for the current time frame, product by product

    m_logger << "What's in my wallet: " << m_wallet.toString() << std::endl;

    for(auto& product : m_avgPrices){
        //get currencies for the current prduct
        std::vector<std::string> currencies = CSVReader::tokenize(product.first, '/');

        //historical average price for the product
        auto sma = product.second.average();


        //get all asks for this product in the current timeframe from the orderbook
        auto asks = m_orderBook.getOrders(OrderBookType::ask, product.first, m_currentTime);
        //get all bids for this product in the current timeframe from the orderbook
        auto bids = m_orderBook.getOrders(OrderBookType::bid, product.first, m_currentTime);

        //Calculate the new average for the current period and add it to the historical for the product
        double sumOfAskPrice = std::accumulate(asks.begin(), asks.end(), 0.0, [](auto x, const auto& y){ return x + y.price; });
        double sumOfBidPrice = std::accumulate(bids.begin(), bids.end(), 0.0, [](auto x, const auto& y){ return x + y.price; });
        product.second.insert((sumOfAskPrice + sumOfBidPrice) / double((asks.size() + bids.size())));

        m_logger << "Product: " << product.first << ": calculated price average: " << sma ;
        if(sma == 0){
            m_logger << " not enough info available. Proceed to next product." << std:: endl;
            continue;
        }
        m_logger << std::endl;


        //I'm going to buy all asks falling below the sma
        //max amount I would buy

        auto maxBidAmount = std::accumulate(asks.begin(), asks.end(), 0.0, [&sma](double sum, const auto& x){ return x.price < sma ? sum + x.amount : sum ; });
        m_logger << "Available amount of asks below sma: " << maxBidAmount << std::endl;

        //max amount of Curr1 I can buy with the amount of Curr2 in my wallet, at the unitary price sma
        double availCurrency = m_wallet.reserveAmount(currencies.at(1), maxBidAmount * sma);
        double availableAmount = availCurrency / sma;
        //max amount I can actually buy
        double bidAmount = std::min(maxBidAmount, availableAmount);
        m_logger    << "Available currency to place bids: " << currencies.at(1) << availCurrency
                    << "; amount of " << currencies.at(0) << " I can purchase: " << bidAmount << std::endl;

        if(bidAmount > 0){
            auto obe = OrderBookEntry{
                    sma,
                    bidAmount,
                    m_currentTime,
                    product.first,
                    OrderBookType::bid,
                    "bot"
            };
            m_logger << obe.toString() << std::endl;
            enterBid_Event(obe);
            m_historicalBids.push_back(obe);
        }

        //I'm going to try to sell to all bids above the sma
        //Max amount requested from the orderbook
        auto maxAskAmount = std::accumulate(bids.begin(), bids.end(), 0.0, [&sma](double sum, const auto& x){ return x.price > sma ? sum + x.amount : sum ; });
        m_logger << "Available amount of bids above sma: " << maxAskAmount << std::endl;

        availCurrency = m_wallet.reserveAmount(currencies.at(0), maxAskAmount * sma);
        //amount actually in my wallet I can sell
        availableAmount = availCurrency / sma;

        m_logger    << "Available currency to place asks: " << currencies.at(1) << availCurrency
                    << "; amount of " << currencies.at(0) << " I can get: " << availableAmount << std::endl;

        //if there are possible bids and I have some currency in my wallet
        if(maxAskAmount > 0 && availableAmount > 0){
            auto obe = OrderBookEntry{
                    sma,
                    availableAmount,
                    m_currentTime,
                    product.first,
                    OrderBookType::ask,
                    "bot"
            };
            m_logger << obe.toString() << std::endl;
            enterAsk_Event(obe);
            m_historicalAsks.push_back(obe);
        }
    }
}

void Bot::logWallet() const {
    m_logger << m_wallet.toString() << std::endl;
}

void Bot::logBids() const {
    std::stringstream ss{"History of Bids: \n"};
    std::for_each(m_historicalBids.begin(), m_historicalBids.end(), [&ss](const OrderBookEntry& x){ ss << x.toString() << std::endl; });
    m_logger << ss.str() << std::endl;
}

void Bot::logAsks() const {
    std::stringstream ss{"History of Asks: \n"};
    std::for_each(m_historicalAsks.begin(), m_historicalAsks.end(), [&ss](const OrderBookEntry& x){ ss << x.toString() << std::endl; });
    m_logger << ss.str() << std::endl;
}

void Bot::logSales() const {
    std::stringstream ss{"History of Sells: \n"};
    std::for_each(m_historicalSells.begin(), m_historicalSells.end(), [&ss](const OrderBookEntry& x){ ss << x.toString() << std::endl; });
    m_logger << ss.str() << std::endl;
}

const BotRemoteControl Bot::GetRemote(){
    BotRemoteControl remote;
    remote.processFrame = [this](const std::string& currentTime){ return this->processFrame(currentTime); };
    remote.logAsks = [this]{ return this->logAsks(); };
    remote.logBids = [this]{ return this->logBids(); };
    remote.logSales = [this]{ return this->logSales(); };
    remote.logWallet = [this]{return this->logWallet(); };
    return remote;
}

void Bot::saleCompleted(const OrderBookEntry &obe) {
    m_logger << "Sale completed: " << obe.toString() << std::endl;
    m_historicalSells.push_back(obe);
}