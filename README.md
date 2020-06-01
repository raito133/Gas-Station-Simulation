# Gas-Station-Simulation
A simulation of a gas station using C++ threads. Compile with makefile. Run with argument which determines how many cars there will be. Exit by pressing "q".
- Cars ride until they run out of gas
- To refuel, cars first have to park on the gas station
- Cars choose one of the two pumps
- Pumps print tickets which determine a car's position in queue to this particular pump
- Cars refuel and ride on

- There is one cistern on the gas station, which has to be refueled by a delivery car.
- The delivery car has to park at the gas station and then refuels the cistern

There is a possibility of a deadlock: if there is no more parking space and cistern is empty, the delivery car cannot park and thus cannot refuel the cistern. The cars cannot refuel because there is no fuel.
