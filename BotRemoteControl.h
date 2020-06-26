//
// Created by deedeeaar on 20/06/2020.
//

#ifndef MERKLEREX_BOTREMOTECONTROL_H
#define MERKLEREX_BOTREMOTECONTROL_H

#include <functional>

/**
 * BotRemoteControl is a sort of Mediator that coordinates the communication between whoever possesses the
 * BotRemoteControl instance and the Bot instance.
 * In this way, the Bot itself and the class controlling the Bot are decoupled.
 * Also, the BotRemoteControl does not store the reference to the bot as a reference variable, but it accesses
 * the Bot method through a closure set up in the methods' definitions.
 */
class BotRemoteControl{
public:
    /**
     * Function object containing a reference to a method in Bot. Invoking the () operator will invoke the
     * corresponding method on the Bot instance.
     * @see Bot::processFrame
     */
    std::function<void(const std::string& currentTime)> processFrame;
    /**
     * Function object containing a reference to a method in Bot. Invoking the () operator will invoke the
     * corresponding method on the Bot instance.
     * @see Bot::logWallet
     */
    std::function<void()> logWallet;
    /**
     * Function object containing a reference to a method in Bot. Invoking the () operator will invoke the
     * corresponding method on the Bot instance.
     * @see Bot::logBids
     */
    std::function<void()> logBids;
    /**
     * Function object containing a reference to a method in Bot. Invoking the () operator will invoke the
     * corresponding method on the Bot instance.
     * @see Bot::logAsks
     */
    std::function<void()> logAsks;
    /**
     * Function object containing a reference to a method in Bot. Invoking the () operator will invoke the
     * corresponding method on the Bot instance.
     * @see Bot::logSales
     */
    std::function<void()> logSales;
};

#endif //MERKLEREX_BOTREMOTECONTROL_H
