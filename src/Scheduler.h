#ifndef __PPSCHEDULER_SCHEDULER_H_
#define __PPSCHEDULER_SCHEDULER_H_

#include <omnetpp.h>
#include "Job_m.h"
#include <queue>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Scheduler : public cSimpleModule {

    std::queue <Job*> lowPriorityQueue;
    std::queue <Job*> highPriorityQueue;

    cMessage* processingTimerLow_;
    cMessage* processingTimerHigh_;

    bool logger = false;

    void processLowJob();
    void processHighJob();
    void removeHighJob();
    void removeLowJob();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
