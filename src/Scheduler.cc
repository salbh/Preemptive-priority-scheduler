#include "Scheduler.h"

Define_Module(Scheduler);

void Scheduler::initialize() {
    genTimeSignal_ = registerSignal("genTimeSignal");
    lastSeen_ = 0;
}

void Scheduler::handleMessage(Job *msg) {
    msg->queueArrival = simTime();
        if(msg->priority_)
            highPriorityQueue.push(msg);
        else lowPriorityQueue.push(msg);

        if(highPriorityQueue.size() != 0)
            executeHighQueue();
        executeLowQueue();
}


void Scheduler::executeLowQueue()
{
    //processa i messaggi nella low priority queue finchè la hpq è vuota e basta
}

void Scheduler::executeHighQueue()
{
    //esaurisce tutta la highpriorityqueue finchè non è vuota
}
