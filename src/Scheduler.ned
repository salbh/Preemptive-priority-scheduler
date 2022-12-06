package ppscheduler;


simple Scheduler
{
    parameters:
        bool logger = default(false);
    	@signal[genTimeSignal];
        @statistic[genTime](source="genTimeSignal"; record=mean,vector);
    gates:
        input queuesIn[];
}
