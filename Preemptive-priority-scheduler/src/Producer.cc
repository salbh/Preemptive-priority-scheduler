#include "Producer.h"
#include "Job_m.h"

Define_Module(Producer);

void Producer::initialize() {
    arrivalDistribution = par("arrivalDistribution");
    serviceDistribution = par("serviceDistribution");
    arrivalMean = par("arrivalMean");
    serviceMean = par("serviceMean");

    timer_ = new cMessage("generationTimer");

    logger = par("logger");

    if (arrivalDistribution < 0 || serviceDistribution < 0 || arrivalMean < 0 || serviceMean < 0) {
        error(": negative distribution parameters!");
    }

    if (logger) {
        if (arrivalDistribution == 0) {
            EV << getName() << ": Constant interarrival times" << endl;
        } else {
            EV << getName() << ": Exponential distribution of the interarrival times" << endl;
        }
        if (serviceDistribution == 0) {
            EV << getName() << ": Constant service times" << endl;
        } else {
            EV << getName() << ": Exponential distribution of the service times" << endl;
        }
    }

    scheduleNextArrival();
}

void Producer::handleMessage(cMessage *msg) {
    sendJob();
    scheduleNextArrival();
}

void Producer::scheduleNextArrival() {
    simtime_t arrivalTime;
    if (arrivalDistribution == 0) {
        arrivalTime = arrivalMean;
    } else if (strcmp(getName(),"producerLow") == 0) {
        arrivalTime = exponential(arrivalMean, AT_LOW_RNG);
    } else {
        arrivalTime = exponential(arrivalMean, AT_HIGH_RNG);
    }
    scheduleAt(simTime() + arrivalTime, timer_);
}

void Producer::sendJob() {
    Job* job = new Job();
    if (serviceDistribution == 0) {
        job->setServiceTime(serviceMean);
    }
    if (strcmp(getName(),"producerLow") == 0) {
        setJob(job, false, ST_LOW_RNG);
    } else {
        setJob(job, true, ST_HIGH_RNG);
    }
    send(job, "out");
    if (logger) {
        EV << getName() << ": new job with Service Time = " << job->getServiceTime() << " sent" << endl;
    }
}

void Producer::setJob(Job *job, bool priority, const int RNG) {
    job->setIsHighPriority(priority);
    if(serviceDistribution != 0) {
        job->setServiceTime(exponential(serviceMean, RNG));
    }
}

void Producer::finish() {
    cancelAndDelete(timer_);
    cSimpleModule::finish();
}



