//
// Created by deedeeaar on 21/06/2020.
//

#ifndef MERKLEREX_ROLLINGAVERAGEMANAGER_H
#define MERKLEREX_ROLLINGAVERAGEMANAGER_H

#include <string>
#include <vector>
#include <map>

class RollingAverageManager {
    const size_t m_size = 4;
    unsigned int m_index{0};
    std::vector<double> m_avgs;
public:
    RollingAverageManager();
    void insert(double avgPrice);
    double average() const;
};


#endif //MERKLEREX_ROLLINGAVERAGEMANAGER_H
