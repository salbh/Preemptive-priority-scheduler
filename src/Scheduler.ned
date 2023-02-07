package ppscheduler;


simple Scheduler
{
    parameters:
    	bool logger = default(false);
    	
    	@signal [low_jobs_num];
        @statistic [low_jobs_num_stat] (source="low_jobs_num"; record=mean, vector);
    	
    	@signal [low_response_time];
        @statistic [low_response_time_stat] (source="low_response_time"; record=mean, vector);
        
        @signal [high_jobs_num];
        @statistic [high_jobs_num_stat] (source="high_jobs_num"; record=mean, vector);
    	
    	@signal [high_response_time];
        @statistic [high_response_time_stat] (source="high_response_time"; record=mean, vector);
    	
    gates:
        input queuesIn[];
}
