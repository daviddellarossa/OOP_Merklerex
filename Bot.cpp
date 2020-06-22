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
    if(!m_isEnabled) return;
    m_currentTime = currentTime;
    //analyze the orderBook for the current time frame, product by product
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
        product.second.insert((sumOfAskPrice + sumOfBidPrice) / (asks.size() + bids.size()));

        if(sma == 0)
            continue;

        //I'm going to buy all asks falling below the sma
        //max amount I would buy
        auto maxBidAmount = std::accumulate(asks.begin(), asks.end(), 0.0, [&sma](double sum, const auto& x){ return x.price < sma ? sum + x.amount : sum ; });
        //max amount of Curr1 I can buy with the amount of Curr2 in my wallet, at the unitary price sma
        double availCurrency = m_wallet.reserveAmount(currencies.at(1), maxBidAmount * sma);
        double availableAmount = availCurrency / sma;
        //max amount I can actually buy
        double bidAmount = std::min(maxBidAmount, availableAmount);

        if(bidAmount > 0){
            auto obe = OrderBookEntry{
                    sma,
                    bidAmount,
                    m_currentTime,
                    product.first,
                    OrderBookType::bid,
                    "bot"
            };
            m_logger << obe.toString();
            enterBid_Event(obe);
        }


        //I'm going to try to sell to all bids above the sma
        //Max amount requested from the orderbook
        auto maxAskAmount = std::accumulate(bids.begin(), bids.end(), 0.0, [&sma](double sum, const auto& x){ return x.price > sma ? sum + x.amount : sum ; });

        availCurrency = m_wallet.reserveAmount(currencies.at(0), maxAskAmount * sma);
        //amount actually in my wallet I can sell
        availableAmount = availCurrency / sma;

        //amount actually in my wallet I can sell
//        walletAmount = m_wallet.currencyAmount(currencies.at(0));

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
            m_logger << obe.toString();
            enterAsk_Event(obe);
        }


    }

}

void Bot::logWallet() const {
    m_logger << m_wallet.toString() << std::endl;
}

void Bot::logBids() const {
    m_logger << "logBids not implemented" << std::endl;
}

void Bot::logAsks() const {
    m_logger << "logAsks not implemented" << std::endl;
}

void Bot::logSales() const {
    m_logger << "logSales not implemented" << std::endl;
}

const BotRemoteControl Bot::GetRemote() {
    BotRemoteControl remote;
    remote.disable = [this]{ this->disable(); };
    remote.enable = [this]{ this->enable(); };
    remote.isEnabled = [this]{ return this->m_isEnabled; };
    remote.logAsks = [this]{ return this->logAsks(); };
    remote.logBids = [this]{ return this->logBids(); };
    remote.logSales = [this]{ return this->logSales(); };
    remote.logWallet = [this]{return this->logWallet(); };
    return remote;
}

void Bot::saleCompleted(const OrderBookEntry &obe) const {
    m_logger << obe.toString();
}

//void Bot::enterAsk(const OrderBookEntry& obe) {
//    m_orderBook.insertOrder(obe);
//    m_logger << obe.toString();
//}
//
//void Bot::enterBid(const OrderBookEntry& obe) {
//    m_orderBook.insertOrder(obe);
//    m_logger << obe.toString();
//}
