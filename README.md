# RocketSimulation 🚀
Multithreading, multiprocessing and multi window C++ application that uses sfml library in order to bring space flight closer to people and functions as vaulable example of multithreading, multiprocessing and multi window app for any one that seeks template like this.

## Installation
Additionaly to standard cpp language installation, application requires sfml library to run visualisation. It can be downloaded simply by running undermentioned command in bash terminal

```bash
sudo apt-get install libsfml-dev
```

## Usage

to do : )

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
  * refreshingWindowContentThread
3. **Control Window Process** - displays available rocket control and sends all modification to simulation core
  * publishingUpdateOnThrustersControlThread
  * refreshingWindowContentThread
4. **Radar Window Process** - displays radar info
  * updatingDataInBackgroundThread
  * refreshingWindowContentThread
5. **Status Window Process** - displays rocket status info
  * updatingDataInBackgroundThread
  * refreshingWindowContentThread
6. **Log Publisher/Saver Process** - serves as a standalone log collector and publisher for all the classes in the project (required from realtime requirements point of view)

**Communication between components**
Most of the IPC is realizes via message queues, however there is one exception to that rule. All classes derived from SimpleLogger class which communicates via FIFO with class functioning as log collector and publisher.

## License
[MIT](https://choosealicense.com/licenses/mit/)
