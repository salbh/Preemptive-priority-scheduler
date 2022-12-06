#include "Scheduler.h"

Define_Module(Scheduler);

void Scheduler::initialize() {
    processingTimerLow_ = new cMessage("processingTimerLow");
    processingTimerHigh_ = new cMessage("processingTimerHigh");

    logger = par("logger");
    //genTimeSignal_ = registerSignal("genTimeSignal");
    //lastSeen_ = 0;
    pendingLowPriorityJob = NULL;
    pendingHighPriorityJob = NULL;

    lowIsWaiting = false;
    highIsWaiting = false;
    EV << "initialization done" << endl;
}

void Scheduler::handleMessage(cMessage *mess) {
    if (mess->isSelfMessage()) {
        EV << "self message" << endl;
           if (logger) {
               EV << "scheduler: Job processed at time: " << simTime() << " with name: " << mess->getName() << endl;
           }
           if (strcmp(mess->getName(), "processingTimerHigh") == 0) {
               EV << "i remove a high job" << endl;
               highIsWaiting = false;
               removeHighJob();
           } else {
               EV << "i remove a low job" << endl;
               lowIsWaiting = false;
               removeLowJob();
           }
           if((highPriorityQueue.size() != 0 || pendingHighPriorityJob != NULL) && !highIsWaiting)
           {
               EV << "after i cancel mesage i execute high queue which is of size " << highPriorityQueue.size() << endl;
               executeHighQueue();
           }
           else if((lowPriorityQueue.size() != 0 || pendingLowPriorityJob != NULL)/* && !lowIsWaiting*/)
               EV << "after i cancel mesage i execute low queue which is of size " << lowPriorityQueue.size()  << endl;
               executeLowQueue();
    }
    else
    {
        EV << "it is a new message" << endl;
        Job* msg = check_and_cast<Job*>(mess);
        msg->setQueueArrival(simTime());
            if(msg->isHighPriority())
            {
                cancelEvent(processingTimerLow_);
                EV << "the preemption is done" << endl;
                highPriorityQueue.push(msg);
                EV << "push message in high queue" << endl;
            }
            else lowPriorityQueue.push(msg);


            if((highPriorityQueue.size() != 0 || pendingHighPriorityJob != NULL) && !highIsWaiting)
            {
                EV << "after i put new message in queue i execute high queue of size " << highPriorityQueue.size()  << endl;
                executeHighQueue();
            }
            else if((lowPriorityQueue.size() != 0 || pendingLowPriorityJob != NULL) && !lowIsWaiting)
            {
                EV << "after i put new message in queue i execute low queue of size " << lowPriorityQueue.size()  << endl;
                executeLowQueue();
            }
    }
}


void Scheduler::processLowJob(Job * job){
    //Job* job = lowPriorityQueue.front();
    lowIsWaiting = true;
    EV << "i process the low job" << endl;

    scheduleAt(simTime() + job->getServiceTime(), processingTimerLow_);
}

void Scheduler::processHighJob(Job * job){
    //Job* job = highPriorityQueue.front();
    highIsWaiting = true;
    EV << "i proces the high jobe" << endl;

    scheduleAt(simTime() + job->getServiceTime(), processingTimerHigh_);
    EV << "sent the high jobe" << endl;
}

void Scheduler::removeHighJob(){
    EV << "i remove high job" << endl;

    delete(pendingHighPriorityJob);
    pendingHighPriorityJob = NULL;
    EV << "rmeove high job pointer null" << endl;

    if (logger) {
        EV << "scheduler: High priority job removed" << endl;
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
    }
}

void Scheduler::removeLowJob(){
    EV << "i remove low job" << endl;

    delete(pendingLowPriorityJob);
    pendingLowPriorityJob = NULL;
    EV << "rmeove low job pointer null" << endl;

    if (logger) {
        EV << "scheduler: Low priority job removed" << endl;
        EV << "scheduler: Low Priority Queue Size: " << lowPriorityQueue.size() << endl;
        EV << "scheduler: High Priority Queue Size: " << highPriorityQueue.size() << endl;
    }
}

void Scheduler::executeLowQueue()
{
    //processa i messaggi nella low priority queue finchè la hpq è vuota e basta
    EV << "i will execute low queue" << endl;

    if(pendingLowPriorityJob == NULL)
    {
        EV << "the low queue pointer was null whilst queue is of size " << lowPriorityQueue.size()  << endl;

        pendingLowPriorityJob = lowPriorityQueue.front();
        pendingLowPriorityJob->setProcessingTime(simTime());
    }
    processLowJob(pendingLowPriorityJob);
}

void Scheduler::executeHighQueue()
{
    EV << "i will execute high queue" << endl;

    //esaurisce tutta la highpriorityqueue finchè non è vuota
    if(pendingHighPriorityJob == NULL)
    {
        EV << "the high queue pointer was null whilst queue is of size " << lowPriorityQueue.size()  << endl;

        pendingHighPriorityJob = highPriorityQueue.front();
        pendingHighPriorityJob->setProcessingTime(simTime());
    }
    processHighJob(pendingHighPriorityJob);
}

void Scheduler::finish()
{
    while(!highPriorityQueue.empty()) highPriorityQueue.pop();
    while(!lowPriorityQueue.empty()) lowPriorityQueue.pop();
    if(pendingHighPriorityJob != NULL) removeHighJob();
    if(pendingLowPriorityJob != NULL) removeLowJob();
    delete(processingTimerLow_);
    delete(processingTimerHigh_);
}

