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
    } else {
        arrivalTime = exponential(arrivalMean, AT_RNG);
    }
    if (logger) {
        EV << getName() << ": next job will arrive in " << arrivalTime << " seconds" << endl;
    }
    scheduleAt(simTime() + arrivalTime, timer_);
}

void Producer::sendJob() {
    Job* job = new Job();
    if (serviceDistribution == 0) {
        job->setServiceTime(serviceMean);
    } else {
        job->setServiceTime(exponential(serviceMean, ST_RNG));
    }
    if (getName() == "producerLow") {
        job->setIsHighPriority(false);
    } else {
        job->setIsHighPriority(true);
    }
    send(job, "out");
    if (logger) {
        EV << getName() << ": new job with Service Time = " << job->getServiceTime() << " sent" << endl;
    }
}



