packet Job {
    bool isHighPriority;
    simtime_t queueArrival;		//when the jobs arrives in the queue
    simtime_t serviceTime;		//time needed to process the job 
}
