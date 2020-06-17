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
    OrderBook(std::string filename);
    /** return vector of all known products in the dataset */
    std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters */
    std::vector<OrderBookEntry> getOrders(
            OrderBookType type,
            std::string product,
            std::string timestamp);
    /** returns the earliest time in the orderbook */
    std::string getEarliestTime();
    /** returns the next time after the sent time in the orderbook.
     * If there is no timestamp, waraps around to the start.*/
    std::string getNextTime(std::string timestamp);

    void insertOrder(OrderBookEntry& order);

    std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

    static double getHighPrice(std::vector<OrderBookEntry>& order);
    static double getLowPrice(std::vector<OrderBookEntry>& order);

private:
    std::vector<OrderBookEntry> orders;
};

#endif //MERKLEREX_ORDERBOOK_H
