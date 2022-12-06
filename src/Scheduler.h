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

    Job * pendingLowPriorityJob;
    Job * pendingHighPriorityJob;

    cMessage* processingTimerLow_;
    cMessage* processingTimerHigh_;

    bool highIsWaiting;
    bool lowIsWaiting;

    bool logger = false;

    void processLowJob(Job * job);
    void processHighJob(Job * job);
    void removeLowJob();
    void removeHighJob();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void executeLowQueue();
    virtual void executeHighQueue();
    void finish();
};

#endif
