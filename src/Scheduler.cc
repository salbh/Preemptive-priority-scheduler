#include "Scheduler.h"

Define_Module(Scheduler);

void Scheduler::initialize() {
    processingTimerLow_ = new cMessage("processingTimerLow");
    processingTimerHigh_ = new cMessage("processingTimerHigh");

    logger = par("logger");
    genTimeSignal_ = registerSignal("genTimeSignal");
    lastSeen_ = 0;
    pendingLowPriorityJob = null;
    pendingHighPriorityJob = null;
}

void Scheduler::handleMessage(Job *msg) {
    if (msg->isSelfMessage()) {
           if (logger) {
               EV << "scheduler: Job processed at time: " << simTime() << " with name: " << msg->getName() << endl;
           }
           if (strcmp(msg->getName(), "processingTimerHigh") == 0) {
               removeHighJob();
           } else {
               removeLowJob();
           }
           if(highPriorityQueue.size() != 0)
               executeHighQueue();
           executeLowQueue();
    }
    else
    {
        msg->queueArrival = simTime();
            if(msg->priority_)
            {
                cancelEvent(processingTimerLow_);
                highPriorityQueue.push(msg);
            }
            else lowPriorityQueue.push(msg);

            if(highPriorityQueue.size() != 0 || pendingHighPriorityJob != null)
                executeHighQueue();
            executeLowQueue();
    }
}


void Scheduler::processLowJob(Job * job){
    //Job* job = lowPriorityQueue.front();
    scheduleAt(simTime() + job->getServiceTime(), processingTimerLow_);
}

void Scheduler::processHighJob(Job * job){
    //Job* job = highPriorityQueue.front();
    scheduleAt(simTime() + job->getServiceTime(), processingTimerHigh_);
}

void Scheduler::removeHighJob(){
    delete(pendingHighPriorityJob);
    pendingHighPriorityJob = null;
    if (logger) {
        EV << "scheduler: High priority job removed" << endl;
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
    }
}

void Scheduler::removeLowJob(){
    delete(pendingLowPriorityJob);
    pendingLowPriorityJob = null;
    if (logger) {
        EV << "scheduler: Low priority job removed" << endl;
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
    }
}

void Scheduler::executeLowQueue()
{
    //processa i messaggi nella low priority queue finchè la hpq è vuota e basta
    if(pendingLowPriorityJob == null)
    {
        pendingLowPriorityJob = lowPriorityQueue.pop();
        pendingLowPriorityJob->processingTime = simTime();
    }
    processLowJob(pendingLowPriorityJob);
}

void Scheduler::executeHighQueue()
{
    //esaurisce tutta la highpriorityqueue finchè non è vuota
    if(pendingHighPriorityJob == null)
    {
        pendingHighPriorityJob = highPriorityQueue.pop();
        pendingHighPriorityJob->processingTime = simTime();
    }
    processHighJob(pendingHighPriorityJob);
}

void Scheduler::finish()
{
    while(!highPriorityQueue.empty()) highPriorityQueue.pop();
    while(!lowPriorityQueue.empty()) lowPriorityQueue.pop();
    if(pendingHighPriorityJob != null) removeHighJob();
    if(pendingLowPriorityJob != null) removeLowJob();
    delete(processingTimerLow_);
    delete(processingTimerHigh_);
}

