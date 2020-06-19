//
// Created by deedeeaar on 19/06/2020.
//

#ifndef MERKLEREX_IACTOR_H
#define MERKLEREX_IACTOR_H

#include <functional>
#include <string>

class IActor{
public:
    virtual void processFrame(std::string currentTime) = 0;
    std::function<void(void)> enterAsk_Event;
    std::function<void(void)> enterBid_Event;
    std::function<void(void)> gotoNextTimeFrame_Event;
    std::function<void(void)> quitRequest_Event;

};

#endif //MERKLEREX_IACTOR_H
