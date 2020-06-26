//
// Created by deedeeaar on 19/06/2020.
//

#ifndef MERKLEREX_IACTOR_H
#define MERKLEREX_IACTOR_H

#include <functional>
#include <string>
#include "OrderBookEntry.h"

/** Virtual class that defines the interface of a component, required for the use in conjunction with MerkelMain. */
class IActor{
public:
    /** Start the processing of the current frame */
    virtual void processFrame(const std::string& currentTime) = 0;
    /** Notify that a new sale has been finalized */
    virtual void saleCompleted(const OrderBookEntry& obe) = 0;
    /** EventHandler invoked to notify the observer that a new ask has been created */
    std::function<void(const OrderBookEntry&)> enterAsk_Event;
    /** EventHandler invoked to notify the observer that a new bid has been created */
    std::function<void(const OrderBookEntry&)> enterBid_Event;
    /** EventHandler invoked to notify the observer that a a gotoNextTimeFrame is requested */
    std::function<void(void)> gotoNextTimeFrame_Event;
    /** EventHandler invoked to notify the observer that a quit the program request has been received */
    std::function<void(void)> quitRequest_Event;
};

#endif //MERKLEREX_IACTOR_H
