package ppscheduler;


simple Scheduler
{
    parameters:
    	bool logger = default(false);
    	
    	@signal [jobs_num];
        @statistic [jobs_num_stat] (source="jobs_num"; record=vector);
    	
    	@signal [response_time];
        @statistic [response_time_stat] (source="response_time"; record=mean, vector);
    	
    gates:
        input queuesIn[];
}
