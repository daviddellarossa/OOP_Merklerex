//
// Created by deedeeaar on 12/05/2020.
//

#ifndef MERKLEREX_CSVREADER_H
#define MERKLEREX_CSVREADER_H

#include <vector>
#include <string>
#include "OrderBookEntry.h"

class CSVReader{
public:
    CSVReader();

    static std::vector<OrderBookEntry> readCSV(std::string csvFile);
    static std::vector<std::string> tokenize(std::string csvLine, char separator);
    static OrderBookEntry stringsToOBE(
            std::string price,
            std::string amount,
            std::string timestamp,
            std::string product,
            OrderBookType _orderType
            );
private:
    static OrderBookEntry stringsToOBE(std::vector<std::string> tokens);
};
#endif //MERKLEREX_CSVREADER_H
