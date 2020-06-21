//
// Created by deedeeaar on 19/05/2020.
//

#ifndef MERKLEREX_ORDERBOOK_H
#define MERKLEREX_ORDERBOOK_H

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook{
public:
    /** construct, reading a csv datafile */
    explicit OrderBook(const std::string& filename); //Constructor set explicit
    /** return vector of all known products in the dataset */
    std::vector<std::string> getKnownProducts() const;
    /** return vector of Orders according to the sent filters */
    std::vector<OrderBookEntry> getOrders (
            OrderBookType type,
            const std::string& product,
            const std::string& timestamp) const;
    /** returns the earliest time in the orderbook */
    std::string getEarliestTime() const;
    /** returns the next time after the sent time in the orderbook.
     * If there is no timestamp, wraps around to the start.*/
    std::string getNextTime(const std::string& timestamp) const;

    void insertOrder(OrderBookEntry& order);

    std::vector<OrderBookEntry> matchAsksToBids(const std::string& product, const std::string& timestamp) const;

    static double getHighPrice(const std::vector<OrderBookEntry>& order);
    static double getLowPrice(const std::vector<OrderBookEntry>& order);

private:
    std::vector<OrderBookEntry> orders;
};

#endif //MERKLEREX_ORDERBOOK_H
