//
// Created by deedeeaar on 11/06/2020.
//

#ifndef MERKLEREX_WALLET_H
#define MERKLEREX_WALLET_H

#include <string>
#include <map>
#include "OrderBookEntry.h"
#include <iostream>

class Wallet
{
public:
    Wallet();
    /** insert currency to the wallet */
    void insertCurrency(const std::string& type, double amount);1
    /** check if the wallet contains this much currency or more */
    bool containsCurrency(const std::string& type, double amount) const;
    /** checks if the wallet can cope with this ask or bid.*/
    bool canFulfillOrder(const OrderBookEntry& order) const;
    /** update the contents of the wallet
     * assumes the order was made by the owner of the wallet
    */
    void processSale(const OrderBookEntry& sale);

    /**
     * Set a reserve on a currency in the wallet.
     * It returns the actual amount reserved based on they currency availability in the wallet
     * @param type - Currency
     * @param amount - Amount to reserve
     * @return - The actual amount reserved
     */
    double reserveAmount(const std::string& type, double amount) const;
    /** Clear all the reserves put on currencies */
    void clearReserves() const ;

    /** generate a string representation of the wallet */
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Wallet& wallet);


private:
    /** Contains the currency type and amount hold in the wallet */
    std::map<std::string,double> m_currencies;
    /** Contains the reserves added to currencies during the time-frame processing, in order not to reuse
     * the same money on different asks/bids
     */
    mutable std::map<std::string,double> m_reserves;
};


#endif //MERKLEREX_WALLET_H
