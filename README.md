# RocketSimulation 🚀
Multithreading, multiprocessing and multi window C++ application that uses sfml library in order to bring space flight closer to people and functions as valuable example of multithreading, multiprocessing and multi window app for any one that seeks template like this.

## Quick Preview
<br>
<p align="center">
 <img src="https://github.com/Orpiczy/RocketSimulation/blob/master/assets/gifs/preview1.gif" alt="Gameplay preview"/>
</p>

## Installation
Additionaly to standard cpp language installation, application requires sfml library to run visualisation. It can be downloaded simply by running undermentioned command in bash terminal

```bash
sudo apt-get install libsfml-dev
```

## Usage

Your **Goal** is simple -> make the biggest technological leap in human history and **REACH THE MOON**. <br>
You control rocket's thrusters by clicking one of three switches in command window. To locate moon simply check your radar once in a while and don't forget about your rocket inertia and your **fuel** and **oxygen** status. Without fuel you won't be able to control the rocket and without oxygen ... without it you and your crew is gone ... so do your best to avoid that and good luck. I believe in you 😉 

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
Please make sure to update tests as appropriate.

## Architecture
Code executes in six concurrent processes and many of them starts additional threads in order to fullfil realtime requirements:
1. **Simulation Core Process** - runs physics engine and serves as communication junction between all processe
  * simulationRunningThread
  * sendingDataThread
  * updatingControlThread
2. **Main Window Process** - displays rocket and environment around it
  * updatingDataInBackgroundThread
  * refreshingWindowContentThread*
3. **Control Window Process** - displays available rocket control and sends all modification to simulation core
  * publishingUpdateOnThrustersControlThread 
  * refreshingWindowContentThread*
4. **Radar Window Process** - displays radar info
  * updatingDataInBackgroundThread
  * refreshingWindowContentThread*
5. **Status Window Process** - displays rocket status info
  * updatingDataInBackgroundThread
  * refreshingWindowContentThread*
6. **Log Publisher/Saver Process** - serves as a standalone log collector and publisher for all the classes in the project (required from realtime requirements point of view)

*run as main thread -> additional threads were not seen as profitable in these cases

### Communication between components

Most of the IPC is realized via message queues, however there is one exception to that rule. All classes derived from SimpleLogger class which communicates via FIFO with class functioning as log collector and publisher.

## License
[MIT](https://choosealicense.com/licenses/mit/)
