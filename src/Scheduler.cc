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
    msg->queueArrival = simTime();
        if(msg->priority_)
            highPriorityQueue.push(msg);
        else lowPriorityQueue.push(msg);

        if(highPriorityQueue.size() != 0 || pendingHighPriorityJob != null)
            executeHighQueue();
        executeLowQueue();
}


void Scheduler::executeLowQueue()
{
    //processa i messaggi nella low priority queue finch� la hpq � vuota e basta
    if(pendingLowPriorityJob == null)
    {
        pendingLowPriorityJob = lowPriorityQueue.pop();
        pendingLowPriorityJob->processingTime = simTime();
    }
    //TODO: esegue il job (in questo caso penso che serva una funzione che "esegue" il job e se viene interrotta riprende da capo)
    //processLowJob(pendingLowPriorityJob);
    //elimina il messaggio una volta che � eseguito, non so se basta una o ci vogliono tutte e due queste linee di codice, nel dubbio le ho messe entrambe
    delete(pendingLowPriorityJob);
    pendingLowPriorityJob = null;
}

void Scheduler::executeHighQueue()
{
    //esaurisce tutta la highpriorityqueue finch� non � vuota
    if(pendingHighPriorityJob == null)
    {
        pendingHighPriorityJob = highPriorityQueue.pop();
        pendingHighPriorityJob->processingTime = simTime();
    }
    //TODO: esegue il job (in questo caso penso che serva una funzione che "esegue" il job senza interruzioni (sapete mica se handle message blocca le altre funzioni?))

    //elimina il messaggio una volta che � eseguito, non so se basta una o ci vogliono tutte e due queste linee di codice, nel dubbio le ho messe entrambe
    delete(pendingHighPriorityJob);
    pendingHighPriorityJob = null;

}

void Scheduler::processLowJob(){
    Job* job = lowPriorityQueue.front();
    scheduleAt(simTime() + job->getServiceTime(), processingTimerLow_);
}

void Scheduler::processHighJob(){
    Job* job = highPriorityQueue.front();
    scheduleAt(simTime() + job->getServiceTime(), processingTimerHigh_);
}
