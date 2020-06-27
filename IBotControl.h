//
// Created by deedeeaar on 20/06/2020.
//

#ifndef MERKLEREX_IBOTCONTROL_H
#define MERKLEREX_IBOTCONTROL_H

/**
 * Implement a set of methods offered by the Bot to allow other classes to send commands
 */
class IBotControl {
public:
    /** Log the Wallet */
    virtual void logWallet() const = 0;
    /** Log the list of bids */
    virtual void logBids() const = 0;
    /** Log the list of asks */
    virtual void logAsks() const = 0;
    /** Log the list of sales */
    virtual void logSales() const = 0;
};

#endif //MERKLEREX_IBOTCONTROL_H
