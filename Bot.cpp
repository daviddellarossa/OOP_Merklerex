//
// Created by deedeeaar on 20/06/2020.
//

#include "Bot.h"
#include <numeric>

Bot::Bot(const OrderBook &orderBook, const Wallet &wallet) :
        m_orderBook{orderBook},
        m_wallet{wallet},
        m_logger{"Merklerex_Bot.log",
        std::ios_base::trunc} {

    m_logger << "Bot instance created" << std::endl;

    /** Initialize the products vector */
    auto products = m_orderBook.getKnownProducts();

    /** Initialize the map of average prices */
    for(auto product : products){
        m_avgPrices[product];
    }
}

void Bot::processFrame(const std::string& currentTime) {

    this->m_currentTime = currentTime;
    m_logger << "\n======================================================\n";
    m_logger << "Processing frame " << this->m_currentTime << std::endl;

    m_logger << "Averages:" << std::endl;
    for(auto& product : m_avgPrices){
        m_logger << "\t" << product.first << ": " << product.second.average() << std::endl;
    }
    m_logger << std::endl;
    m_logger << "What's in my wallet:\n" << m_wallet.toString() << std::endl;

    //analyze the m_orderBook for the current time frame, product by product
    for(auto& product : m_avgPrices){
        //get currencies for the current product
        std::vector<std::string> currencies = CSVReader::tokenize(product.first, '/');

        //historical average price for the product
        auto sma = product.second.average();

        //get all asks for this product in the current timeframe from the orderbook
        auto asks = m_orderBook.getOrders(OrderBookType::ask, product.first, this->m_currentTime);
        //get all bids for this product in the current timeframe from the orderbook
        auto bids = m_orderBook.getOrders(OrderBookType::bid, product.first, this->m_currentTime);

        //If for the current iteration there are neither asks nor bids, return
        if(asks.size() == 0 && bids.size() == 0){
            m_logger << "No bids or asks for this turn." << std::endl;
            continue;
        }
        //Calculate the new average for the current period and add it to the historical for the product
        double sumOfAskPrice = std::accumulate(asks.begin(), asks.end(), 0.0, [](double x, const OrderBookEntry& y){ return x + y.m_price; });
        double sumOfBidPrice = std::accumulate(bids.begin(), bids.end(), 0.0, [](double x, const OrderBookEntry& y){ return x + y.m_price; });

        /* Insert the newly calculate price average into the average manager. The new average is calculated taking
         * into account asks and bids */
        product.second.insert((sumOfAskPrice + sumOfBidPrice) / double((asks.size() + bids.size())));

        m_logger << "Product: " << product.first << ": calculated price average: " << sma ;
        if(sma == 0){
            m_logger << " not enough info available. Proceed to next product." << std:: endl;
            continue;
        }
        m_logger << std::endl;

        // I'm going to buy all asks falling below the sma
        // The max amount I would buy is
        auto maxBidAmount = std::accumulate(asks.begin(), asks.end(), 0.0, [&sma](double sum, const OrderBookEntry& x){ return x.m_price < sma ? sum + x.m_amount : sum ; });
        m_logger << "Available amount of asks below average: " << maxBidAmount << std::endl;

        //max amount of Currency1 I can buy with the amount of Currency2 in my wallet, at the unitary price sma
        double availCurrency = m_wallet.reserveAmount(currencies.at(1), maxBidAmount * sma);
        double availableAmount = availCurrency / sma;
        //max amount I can actually buy
        double bidAmount = std::min(maxBidAmount, availableAmount);
        m_logger    << "Available currency to place bids: " << currencies.at(1) << " " << availCurrency
                    << "; amount of " << currencies.at(0) << " I can purchase: " << bidAmount << std::endl;

        //Create a new bid
        if(bidAmount > 0){
            auto obe = OrderBookEntry{
                    sma,
                    bidAmount,
                    this->m_currentTime,
                    product.first,
                    OrderBookType::bid,
                    "bot"
            };
            m_logger << obe.toString() << std::endl;
            //send the bid to the observer, invoking its eventHandler function, so it can take care of it
            enterBid_Event(obe);
            //add the bid to the historical
            m_historicalBids.push_back(obe);
        }

        //I'm going to try to sell to all bids above the sma
        //Max amount requested from the orderbook
        auto maxAskAmount = std::accumulate(bids.begin(), bids.end(), 0.0, [&sma](double sum, const OrderBookEntry& x){ return x.m_price > sma ? sum + x.m_amount : sum ; });
        m_logger << "Available amount of bids above average: " << maxAskAmount << std::endl;

        availCurrency = m_wallet.reserveAmount(currencies.at(0), maxAskAmount * sma);
        //amount actually in my wallet I can sell
        availableAmount = availCurrency / sma;

        m_logger    << "Available currency to place asks: " << currencies.at(1) << " " << availCurrency
                    << "; amount of " << currencies.at(0) << " I can get: " << availableAmount << std::endl;

        //if there are possible bids and I have some currency in my wallet
        if(maxAskAmount > 0 && availableAmount > 0){
            //create a new ask
            auto obe = OrderBookEntry{
                    sma,
                    availableAmount,
                    this->m_currentTime,
                    product.first,
                    OrderBookType::ask,
                    "bot"
            };
            m_logger << obe.toString() << std::endl;
            //send the ask to the observer, invoking its eventHandler function, so it can take care of it
            enterAsk_Event(obe);
            //add the ask to the historical
            m_historicalAsks.push_back(obe);
        }
    }
}

void Bot::logWallet() const {
    std::stringstream ss;
    ss << "Wallet content: \n";
    ss << m_wallet.toString();
    m_logger << ss.str();
    m_logger.flush();
}

void Bot::logBids() const {
    std::stringstream ss;
    ss << "History of Bids: \n";
    std::for_each(m_historicalBids.begin(), m_historicalBids.end(), [&ss](const OrderBookEntry& x){ ss << "\t" << x.toString() << std::endl; });
    if(m_historicalBids.empty())
        ss << "None" << std::endl;
    m_logger << ss.str();
    m_logger.flush();
}

void Bot::logAsks() const {
    std::stringstream ss;
    ss << "History of Asks: \n";
    std::for_each(m_historicalAsks.begin(), m_historicalAsks.end(), [&ss](const OrderBookEntry& x){ ss << "\t" << x.toString() << std::endl; });
    if(m_historicalAsks.empty())
        ss << "None" << std::endl;
    m_logger << ss.str();
    m_logger.flush();
}

void Bot::logSales() const {
    std::stringstream ss;
    ss << "History of Sells: \n";
    std::for_each(m_historicalSales.begin(), m_historicalSales.end(), [&ss](const OrderBookEntry& x){ ss << "\t" << x.toString() << std::endl; });
    if(m_historicalSales.empty())
        ss << "None" << std::endl;
    m_logger << ss.str();
    m_logger.flush();
}

const BotRemoteControl Bot::getRemote(){
    BotRemoteControl remote;
    //Bind the BotRemoteControl's properties to the actual methods in Bot they will invoke
    remote.processFrame = [this](const std::string& currentTime){ return this->processFrame(currentTime); };
    remote.logAsks = [this]{ return this->logAsks(); };
    remote.logBids = [this]{ return this->logBids(); };
    remote.logSales = [this]{ return this->logSales(); };
    remote.logWallet = [this]{return this->logWallet(); };
    return remote;
}

void Bot::saleCompleted(const OrderBookEntry &obe) {
    m_logger << "Sale completed: " << obe.toString() << std::endl;
    //add the sell to the historical
    m_historicalSales.push_back(obe);
}