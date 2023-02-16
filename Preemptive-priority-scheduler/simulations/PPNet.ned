package ppscheduler.simulations;


import ppscheduler.Producer;
import ppscheduler.Scheduler;

network PPNet
{
    submodules:
        producerLow: Producer {
            @display("p=86,45");
        }
        producerHigh: Producer {
            @display("p=86,175");
        }
        scheduler: Scheduler {
            @display("p=329,104");
        }
    connections:
        producerLow.out --> scheduler.queuesIn++;
        producerHigh.out --> scheduler.queuesIn++;
}
