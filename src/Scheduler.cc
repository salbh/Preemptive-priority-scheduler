#include "Scheduler.h"

Define_Module(Scheduler);

void Scheduler::initialize() {
    processingTimerLow_ = new cMessage("processingTimerLow");
    processingTimerHigh_ = new cMessage("processingTimerHigh");

    highWait = false;
    lowWait = false;
    logger = par("logger");
}

void Scheduler::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (logger) {
            EV << "scheduler: Job processed at time: " << simTime() << " with name: " << msg->getName() << endl;
        }
        if (strcmp(msg->getName(), "processingTimerHigh") == 0) {
            highWait = false;
            removeHighJob();
        } else {
            lowWait = false;
            removeLowJob();
        }
        if (!highPriorityQueue.empty() && highWait == false) {
            processHighJob();
        } else if (!lowPriorityQueue.empty() && lowWait == false) {
            processLowJob();
        }
    } else {
        Job* job = check_and_cast<Job*>(msg);
        job->setQueueArrival(simTime());
        if (logger) {
            EV << "scheduler: Job arrived at time: " << job->getQueueArrival() << endl;
            EV << "scheduler: Job service time: " << job->getServiceTime() << endl;
        }
        if (job->getIsHighPriority()) {
            if (!lowPriorityQueue.empty()) {
                cancelEvent(processingTimerLow_);
            }
            highPriorityQueue.push(job);
        } else {
            lowPriorityQueue.push(job);
        }
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
        if (highPriorityQueue.size() == 1 && highWait == false) {
            processHighJob();
        }
        if (highPriorityQueue.empty() && lowPriorityQueue.size() == 1 && lowWait == false) {
            processLowJob();
        }
    }
}


void Scheduler::processLowJob(){
    lowPriorityJob = lowPriorityQueue.front();
    lowWait = true;
    scheduleAt(simTime() + lowPriorityJob->getServiceTime(), processingTimerLow_);
}

void Scheduler::processHighJob(){
    highPriorityJob = highPriorityQueue.front();
    highWait = true;
    scheduleAt(simTime() + highPriorityJob->getServiceTime(), processingTimerHigh_);
}

void Scheduler::removeHighJob(){
    highPriorityQueue.pop();
    if (logger) {
        EV << "scheduler: High priority job removed" << endl;
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
    }
    delete(highPriorityJob);
}

void Scheduler::removeLowJob(){
    lowPriorityQueue.pop();
    if (logger) {
        EV << "scheduler: Low priority job removed" << endl;
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
    }
    delete(lowPriorityJob);
}


void Scheduler::finish() {
    cleanMemory();
    if (logger) {
        EV << "scheduler: " << "High Priority Queue size: " << highPriorityQueue.size() << endl;
        EV << "scheduler: " << "Low Priority Queue size: " << lowPriorityQueue.size() << endl;
    }
    cSimpleModule::finish();
}

void Scheduler::cleanMemory(){
    EV << "Calling destructor" << endl;

    cancelAndDelete(processingTimerLow_);
    cancelAndDelete(processingTimerHigh_);

    while(!highPriorityQueue.empty()) {
        Job* jobHigh = highPriorityQueue.front();
        highPriorityQueue.pop();
        delete(jobHigh);
    }

    while(!lowPriorityQueue.empty()) {
        Job* jobLow = lowPriorityQueue.front();
        lowPriorityQueue.pop();
        delete(jobLow);
    }
}
