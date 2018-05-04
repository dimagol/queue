//
// Created by dima on 27/01/18.
//

#include "WaitingStrategy.h"

WaitingStrategy * getStrategy(StrategyType strategy){
    switch (strategy){
        case StrategyType::SLEEP:
            return new SleepQueueWaitingStrategy();
        case StrategyType::YIELD:
            return new YeldQueueWaitingStrategy();
        case StrategyType::POLLING:
            return new PollingQueueWaitingStrategy();
    }
}