packet Job {
    bool isHighPriority;
    simtime_t queueArrival;		//il momento in cui arrivano nella queue
    simtime_t serviceTime;		//per simulare il fatto che ogni job richiede del tempo per essere servito, quando arriva allo scheduler lo facciamo aspettare per questo tot tempo prima di eliminare il job
    simtime_t processingTime; 	//timestamp in cui il job viene servito dallo scheduler
}
