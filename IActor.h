//
// Created by deedeeaar on 19/06/2020.
//

#ifndef MERKLEREX_IACTOR_H
#define MERKLEREX_IACTOR_H

#include <functional>
#include <string>
#include "OrderBookEntry.h"

class IActor{
public:
    virtual void processFrame(const std::string& currentTime) = 0;
    virtual void saleCompleted(const OrderBookEntry& obe) = 0;
    std::function<void(const OrderBookEntry&)> enterAsk_Event;
    std::function<void(const OrderBookEntry&)> enterBid_Event;
    std::function<void(void)> gotoNextTimeFrame_Event;
    std::function<void(void)> quitRequest_Event;
};

#endif //MERKLEREX_IACTOR_H
