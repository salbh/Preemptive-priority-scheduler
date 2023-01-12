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

    simsignal_t low_jobs_num_signal = 0;
    simsignal_t low_response_time_signal = 0;

    simsignal_t high_jobs_num_signal = 0;
    simsignal_t high_response_time_signal = 0;

    cMessage* processingTimerLow_;
    cMessage* processingTimerHigh_;

    bool logger;

    bool highWait;
    bool lowWait;

    void handleTimer(cMessage *msg);
    void handleJob(Job *job);
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
