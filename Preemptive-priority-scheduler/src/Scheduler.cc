#include "Scheduler.h"

Define_Module(Scheduler);

void Scheduler::initialize() {
    processingTimerLow_ = new cMessage("processingTimerLow");
    processingTimerHigh_ = new cMessage("processingTimerHigh");

    highWait = false;
    lowWait = false;
    logger = par("logger");

    low_jobs_num_signal = registerSignal("low_jobs_num");
    low_response_time_signal = registerSignal("low_response_time");

    high_jobs_num_signal = registerSignal("high_jobs_num");
    high_response_time_signal = registerSignal("high_response_time");

    emit(low_jobs_num_signal, 0);
    emit(high_jobs_num_signal, 0);
}

void Scheduler::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (logger) {
            EV << "scheduler: Job processed at time: " << simTime() << " with name: " << msg->getName() << endl;
        }
        handleTimer(msg);
    } else {
        Job* job = check_and_cast<Job*>(msg);
        job->setQueueArrival(simTime());
        if (logger) {
            EV << "scheduler: Job arrived at time: " << job->getQueueArrival() << endl;
            EV << "scheduler: Job service time: " << job->getServiceTime() << endl;
        }
        handleJob(job);
    }
}

void Scheduler::handleTimer(cMessage *msg) {
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
}

void Scheduler::handleJob(Job *job) {
    if (job->isHighPriority()) {
        if (!lowPriorityQueue.empty()) {
            cancelEvent(processingTimerLow_);
            lowWait = false;
        }
        highPriorityQueue.push(job);
        emit(high_jobs_num_signal, highPriorityQueue.size());
    } else {
        lowPriorityQueue.push(job);
        emit(low_jobs_num_signal, lowPriorityQueue.size());
    }
    if (logger) {
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
    }
    if (highPriorityQueue.size() == 1 && highWait == false) {
        processHighJob();
    }
    if (highPriorityQueue.empty() && lowPriorityQueue.size() == 1 && lowWait == false) {
        processLowJob();
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
    emit(high_response_time_signal, simTime() - highPriorityJob->getQueueArrival());
    highPriorityQueue.pop();
    emit(high_jobs_num_signal, highPriorityQueue.size());
    if (logger) {
        EV << "scheduler: High priority job removed" << endl;
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
    }
    delete(highPriorityJob);
}

void Scheduler::removeLowJob(){
    emit(low_response_time_signal, simTime() - lowPriorityJob->getQueueArrival());
    lowPriorityQueue.pop();
    emit(low_jobs_num_signal, lowPriorityQueue.size());
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
    if (logger) {
        EV << "Calling destructor" << endl;
    }

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
