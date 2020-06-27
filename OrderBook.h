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

    void insertOrder(const OrderBookEntry& order);
    /** Method needed exclusively for this simulation.
     * It is found out that when the time-frame rolls over, it finds bids and asks inserted
     * either by the Bot or by the user at the previous rolls. This method basically removes
     * asks and bids not beloging to the initial dataset, that are already processed
     */
    void removeCustomOrders();

    std::vector<OrderBookEntry> matchAsksToBids(const std::string& product, const std::string& timestamp) const;

    static double getHighPrice(const std::vector<OrderBookEntry>& order);
    static double getLowPrice(const std::vector<OrderBookEntry>& order);

private:
    std::vector<OrderBookEntry> m_orders;
};

#endif //MERKLEREX_ORDERBOOK_H
