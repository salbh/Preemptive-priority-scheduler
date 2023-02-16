package ppscheduler;


simple Producer
{
   	//Initialization of parameters:
    	  //- 0: Constant;
    	  //- 1: Exponential.
    	  
    parameters:
        int arrivalDistribution = default(0);
        int serviceDistribution = default(0);
        
        double arrivalMean = default(1);
        double serviceMean = default(1);
        
        bool logger = default(false);
    
    gates: 
        output out;
}
