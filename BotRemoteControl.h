//
// Created by deedeeaar on 20/06/2020.
//

#ifndef MERKLEREX_BOTREMOTECONTROL_H
#define MERKLEREX_BOTREMOTECONTROL_H

#include <functional>

class BotRemoteControl{
public:
    std::function<bool()> isEnabled;
    std::function<void()> enable;
    std::function<void()> disable;
    std::function<void()> logWallet;
    std::function<void()> logBids;
    std::function<void()> logAsks;
    std::function<void()> logSales;
};

#endif //MERKLEREX_BOTREMOTECONTROL_H
