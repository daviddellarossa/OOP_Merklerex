//
// Created by deedeeaar on 20/06/2020.
//

#ifndef MERKLEREX_IBOTCONTROL_H
#define MERKLEREX_IBOTCONTROL_H

class IBotControl {
public:

    virtual void processFrame(const std::string& currentTime) = 0;
    virtual void logWallet() const = 0;
    virtual void logBids() const = 0;
    virtual void logAsks() const = 0;
    virtual void logSales() const = 0;
};

#endif //MERKLEREX_IBOTCONTROL_H
