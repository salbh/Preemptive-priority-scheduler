package ppscheduler;


simple Scheduler
{
    parameters:
    	@signal[genTimeSignal];
        @statistic[genTime](source="genTimeSignal"; record=mean,vector);
    gates:
        input queuesIn[];
}
