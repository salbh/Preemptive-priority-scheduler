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
    Job* lowPriorityJob;
    Job* highPriorityJob;

    cMessage* processingTimerLow_;
    cMessage* processingTimerHigh_;

    bool logger;

    bool highWait;
    bool lowWait;

    void handleTimer(cMessage *msg);
    void handleJob(cMessage *msg);
    void processLowJob();
    void processHighJob();
    void removeHighJob();
    void removeLowJob();
    void cleanMemory();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

#endif
