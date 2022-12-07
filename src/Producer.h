#ifndef __PPSCHEDULER_PRODUCER_H_
#define __PPSCHEDULER_PRODUCER_H_

#include <omnetpp.h>

#define AT_RNG 0
#define ST_RNG 1

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class Producer : public cSimpleModule {
    int arrivalDistribution = 0;
    int serviceDistribution = 0;
    double arrivalMean = 0;
    double serviceMean = 0;

    cMessage* timer_;

    bool logger = false;

    void scheduleNextArrival();
    void sendJob();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

#endif

