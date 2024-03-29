#ifndef __PPSCHEDULER_PRODUCER_H_
#define __PPSCHEDULER_PRODUCER_H_

#include <omnetpp.h>
#include "Job_m.h"

#define AT_LOW_RNG 0
#define ST_LOW_RNG 1
#define AT_HIGH_RNG 2
#define ST_HIGH_RNG 3

using namespace omnetpp;

class Producer : public cSimpleModule {
    int arrivalDistribution = 0;
    int serviceDistribution = 0;
    double arrivalMean = 0;
    double serviceMean = 0;

    cMessage* timer_;

    bool logger = false;

    void scheduleNextArrival();
    void sendJob();
    void setJob(Job *job, bool priority, const int RNG);

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

#endif

