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
    void insertCurrency(const std::string& type, double amount);
    /** remove currency from the wallet */
//    bool removeCurrency(const std::string& type, double amount);

    /** check if the wallet contains this much currency or more */
    bool containsCurrency(const std::string& type, double amount) const;
    /** checks if the wallet can cope with this ask or bid.*/
    bool canFulfillOrder(const OrderBookEntry& order) const;
    /** update the contents of the wallet
     * assumes the order was made by the owner of the wallet
    */
    void processSale(const OrderBookEntry& sale);

    double currencyAmount(const std::string& type) const;
    double reserveAmount(const std::string& type, double amount) const;
    void clearReserves() const ;

    /** generate a string representation of the wallet */
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Wallet& wallet);


private:
    std::map<std::string,double> currencies;
    mutable std::map<std::string,double> reserves;
};


#endif //MERKLEREX_WALLET_H
