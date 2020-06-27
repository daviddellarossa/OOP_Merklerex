//
// Created by deedeeaar on 11/06/2020.
//

#include "Wallet.h"
#include <iostream>
#include <sstream>
#include "CSVReader.h"

Wallet::Wallet(){ }

void Wallet::insertCurrency(const std::string& type, double amount)
{
    double balance;
    if (amount < 0)
    {
        throw std::exception{};
    }
    if (m_currencies.count(type) == 0) // not there yet
    {
        balance = 0;
    }
    else { // is there
        balance = m_currencies[type];
    }
    balance += amount;
    m_currencies[type] = balance;
}

//bool Wallet::removeCurrency(const std::string& type, double amount)
//{
//    if (amount < 0)
//    {
//        return false;
//    }
//    if (currencies.count(type) == 0) // not there yet
//    {
//        //std::cout << "No currency for " << type << std::endl;
//        return false;
//    }
//    else { // is there - do  we have enough
//        if (containsCurrency(type, amount))// we have enough
//        {
//            //std::cout << "Removing " << type << ": " << amount << std::endl;
//            currencies[type] -= amount;
//            return true;
//        }
//        else // they have it but not enough.
//            return false;
//    }
//}

bool Wallet::containsCurrency(const std::string& type, double amount) const
{
    if (m_currencies.count(type) == 0) // not there yet
        return false;
    else
        return m_currencies.at(type) >= amount - m_reserves[type];

}

std::string Wallet::toString() const
{
    std::stringstream ss;
    for (std::pair<std::string,double> pair : m_currencies)
    {
        std::string currency = pair.first;
        double amount = pair.second;
        ss << currency << " : " << std::to_string(amount) << "\n";
    }
    return ss.str();
}

bool Wallet::canFulfillOrder(const OrderBookEntry& order) const
{
    std::vector<std::string> currs = CSVReader::tokenize(order.m_product, '/');
    // ask
    if (order.m_orderType == OrderBookType::ask)
    {
        double amount = order.m_amount;
        std::string currency = currs[0];
        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;

        return containsCurrency(currency, amount);
    }
    // bid
    if (order.m_orderType == OrderBookType::bid)
    {
        double amount = order.m_amount * order.m_price;
        std::string currency = currs[1];
        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;
        return containsCurrency(currency, amount);
    }


    return false;
}


void Wallet::processSale(const OrderBookEntry& sale)
{
    std::vector<std::string> currs = CSVReader::tokenize(sale.m_product, '/');
    // ask
    if (sale.m_orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.m_amount;
        std::string outgoingCurrency = currs[0];
        double incomingAmount = sale.m_amount * sale.m_price;
        std::string incomingCurrency = currs[1];

        m_currencies[incomingCurrency] += incomingAmount;
        m_currencies[outgoingCurrency] -= outgoingAmount;

    }
    // bid
    if (sale.m_orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.m_amount;
        std::string incomingCurrency = currs[0];
        double outgoingAmount = sale.m_amount * sale.m_price;
        std::string outgoingCurrency = currs[1];

        m_currencies[incomingCurrency] += incomingAmount;
        m_currencies[outgoingCurrency] -= outgoingAmount;

    }
}
std::ostream& operator<<(std::ostream& os,  const Wallet& wallet)
{
    os << wallet.toString();
    return os;
}
/**
 * Return the available amount of Currency less than or equal to amount
 * The amount is calculated taking into account the amount of currency in the wallet
 * and the reserves set for that currency om the current timespan
 * @param type - Currency
 * @param amount - Desired amount of currency to be reserved
 * @return - Available amount of currency actually reserved.
 */
double Wallet::reserveAmount(const std::string &type, double amount) const {
    if(amount == 0) return 0;
    auto currency = m_currencies.find(type);
    if(currency == m_currencies.end())
        return 0;

    //if there is enough currency available, reserve the asked amount
    if(m_reserves[type] + amount <= currency->second){
        m_reserves[type] += amount;
        return amount;
    }else{ //not enough currency available - reserves already set; return the max quantity available
        auto reserved = currency->second - m_reserves[type];
        m_reserves[type] += reserved;
        return reserved;
    }
}

void Wallet::clearReserves() const {
    for(auto& reserve : m_reserves)
        reserve.second = 0;
}

