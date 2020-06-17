//
// Created by deedeeaar on 12/05/2020.
//

#include <iostream>
#include <fstream>
#include "CSVReader.h"

CSVReader::CSVReader(){

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFileName){
    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile(csvFileName);
    std::string line;
    if(csvFile.is_open()){
        while(std::getline(csvFile, line)){
            try{
                OrderBookEntry obe = stringsToOBE(tokenize(line, ','));
                entries.push_back(obe);}
            catch(const std::exception& e){
                std::cerr << "CSVReader::readCSV exception: " << e.what() << std::endl;
            }
        }
    }
    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
    return entries;
}


std::vector<std::string> CSVReader::tokenize(std::string csvLine, char separator){
    std::vector<std::string> tokens;
    signed int  start, end;
    std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do{
        end = csvLine.find_first_of(separator, start);
        if(start == csvLine.length() || start == end) break;
        if(end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end+1;
    }while(end > 0);
    return  tokens;
}
OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string>tokens){
    double price, amount;

    if(tokens.size() != 5){
        std::cerr << "Bad line " << std::endl;
        throw std::exception();
    }
    try{
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }catch(const std::exception& e){
        std::cerr << "CSVReader::stringsToOBE Bad float! " << tokens[3]  << std::endl;
        std::cerr << "CSVReader::stringsToOBE Bad float! " << tokens[4]  << std::endl;
        throw;
    }

    OrderBookEntry obe{
    price,
    amount,
    tokens[0],
    tokens[1],
    OrderBookEntry::stringToOrderBookType(tokens[2])};
    return obe;
}

OrderBookEntry CSVReader::stringsToOBE(
        std::string priceString,
        std::string amountString,
        std::string timestamp,
        std::string product,
        OrderBookType _orderType
){
    double price, amount;

    try{
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }catch(const std::exception& e){
        std::cerr << "CSVReader::stringsToOBE: Bad float! " << priceString  << std::endl;
        std::cerr << "CSVReader::stringsToOBE: Bad float! " << amountString  << std::endl;
        throw;
    }

    OrderBookEntry obe{
            price,
            amount,
            timestamp,
            product,
            _orderType};
    return obe;
}