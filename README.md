# OldBridgeProblem


## Correctness constraints
  1) At most 3 cars are on the bridge at a time
  
  2) All cars on the bridge go in the same direction

  3) Whenever the bridge is empty and a car is waiting, that car should get on the bridge

  4) Whenever the bridge is not empty or full and a car is waiting to go the same direction as the cars on the bridge, that car should get on the bridge

### Only one thread accesses shared state at a time

### Cars will be waiting to get on the bridge, but in two directions. Use an array of two condition variables, waitingToGo[2].

### It will be necessary to know the number of cars on the bridge (cars, initialized to 0), and the direction of these cars if there are any (call it currentdirection). It will also be useful to know the number of cars waiting to go in each direction; use an array waiters[2].
