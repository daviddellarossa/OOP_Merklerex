//
// Created by deedeeaar on 19/05/2020.
//

#include "OrderBook.h"
#include "CSVReader.h"
#include <set>
#include <algorithm>
#include <iostream>

//input parameter set to const
//initialization of orders moved to initialization list
OrderBook::OrderBook(const std::string& filename) : m_orders(CSVReader::readCSV(filename)) {
//    orders = CSVReader::readCSV(filename);
}

/** Return unique products out of orders collection */
//function set to const
std::vector<std::string> OrderBook::getKnownProducts() const{
    /** map replaced with a set */
    std::set<std::string> prodSet;
//    std::set<std::string> distinctTimestamp;
    for(const OrderBookEntry& e : m_orders){
        prodSet.insert(e.m_product);
//        distinctTimestamp.insert(e.timestamp);
    }
    std::vector<std::string> products(prodSet.begin(), prodSet.end());

    return products;
}

//function set to const
//input parameters set to const
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, const std::string& product, const std::string& timestamp) const{
    std::vector<OrderBookEntry> orders_sub;
    for(const OrderBookEntry& e : m_orders){
        if(e.m_orderType == type &&
           e.m_product == product &&
           e.m_timestamp == timestamp){
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}
//input parameter set to const
double OrderBook::getHighPrice(const std::vector<OrderBookEntry> &orders) {
    double max = orders[0].m_price;
    for(auto& e: orders){
        if(e.m_price > max) max = e.m_price;
    }
    //auto maxEntry = std::max_element(orders.cbegin(), orders.cend(), [](const auto& x, const auto& y){ return x.price<y.price;});
    return max;
}

//input parameter set to const
double OrderBook::getLowPrice(const std::vector<OrderBookEntry> &orders) {
    double min = orders[0].m_price;
    for(auto& e: orders){
        if(e.m_price < min) min = e.m_price;
    }
    return min;
}

//input parameter set to const
std::string OrderBook::getEarliestTime() const {
    return m_orders[0].m_timestamp;
}

//function made const; input parameters made const&
std::string OrderBook::getNextTime(const std::string& timestamp) const {
    std::string next_timestamp = "";
    for(auto& e : m_orders){
        if(e.m_timestamp > timestamp){
            next_timestamp = e.m_timestamp;
            break;
        }
    }
    if(next_timestamp == ""){
        next_timestamp = m_orders[0].m_timestamp;
    }
    return next_timestamp;
}

void OrderBook::insertOrder(const OrderBookEntry& order)
{
    m_orders.push_back(order);
    std::sort(m_orders.begin(), m_orders.end(), OrderBookEntry::compareByTimestamp);
}

//function made const; input parameters made const&
std::vector<OrderBookEntry> OrderBook::matchAsksToBids(const std::string& product, const std::string& timestamp) const
{
// asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                 product,
                                                 timestamp);
// bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                 product,
                                                 timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales;

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size()-1].m_price << std::endl;
    std::cout << "min ask " << asks[0].m_price << std::endl;
    std::cout << "max bid " << bids[0].m_price << std::endl;
    std::cout << "min bid " << bids[bids.size()-1].m_price << std::endl;

    for (OrderBookEntry& ask : asks)
    {
        //     for bid in bids:
        for (OrderBookEntry& bid : bids)
        {
            //         if bid.price >= ask.price # we have a match
            if (bid.m_price >= ask.m_price)
            {
                //             sale = new order()
                //             sale.price = ask.price
                OrderBookEntry sale{ask.m_price, 0, timestamp,
                                    product,
                                    OrderBookType::asksale};

                if (bid.m_username != "dataset")
                {
                    sale.m_username = bid.m_username;
                    sale.m_orderType = OrderBookType::bidsale;
                }
                if (ask.m_username != "dataset")
                {
                    sale.m_username = ask.m_username;
                    sale.m_orderType =  OrderBookType::asksale;
                }

                //             # now work out how much was sold and
                //             # create new bids and asks covering
                //             # anything that was not sold
                //             if bid.amount == ask.amount: # bid completely clears ask
                if (bid.m_amount == ask.m_amount)
                {
                    //                 sale.amount = ask.amount
                    sale.m_amount = ask.m_amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.m_amount = 0;
                    //                 # can do no more with this ask
                    //                 # go onto the next ask
                    //                 break
                    break;
                }
                //           if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.m_amount > ask.m_amount)
                {
                    //                 sale.amount = ask.amount
                    sale.m_amount = ask.m_amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # we adjust the bid in place
                    //                 # so it can be used to process the next ask
                    //                 bid.amount = bid.amount - ask.amount
                    bid.m_amount = bid.m_amount - ask.m_amount;
                    //                 # ask is completely gone, so go to next ask
                    //                 break
                    break;
                }


                //             if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.m_amount < ask.m_amount &&
                    bid.m_amount > 0)
                {
                    //                 sale.amount = bid.amount
                    sale.m_amount = bid.m_amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # update the ask
                    //                 # and allow further bids to process the remaining amount
                    //                 ask.amount = ask.amount - bid.amount
                    ask.m_amount = ask.m_amount - bid.m_amount;
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.m_amount = 0;
                    //                 # some ask remains so go to the next bid
                    //                 continue
                    continue;
                }
            }
        }
    }

    return sales;
}

/**
 * Remove from the Orderbook the orders not in the original dataset
 * This is necessary because the orderbook rolls over when reaching the end of the dataset
 * and further processing would find the orders inserted at the previous run.
 * But this would mess up with the wallet amount check, as the matchAsksToBids method would find more orders
 * than the currency amount currently contained in the wallet.
 */
void OrderBook::removeCustomOrders() {
    for(int i = m_orders.size() - 1; i >= 0; i--){
        if(m_orders[i].m_username != "dataset")
            m_orders.erase(m_orders.begin() + i);
    }
}
