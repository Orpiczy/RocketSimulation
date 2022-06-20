# RocketSimulation 🚀
Multithreading, multiprocessing and multi window application that uses sfml library in order to bring space flight closer people.
Code executes in six concurrent processes and many of them starts additional threads in order to fullfil realtime requirements:
1. Simulation Core Process -
  * simulationRunningThread
  * sendingDataThread
  * updatingControlThread
2. Main Window Process - 
  * updatingDataInBackgroundThread
  * refreshingWindowContentThread
3. Control Window Process - 
  * publishingUpdateOnThrustersControlThread
  * refreshingWindowContentThread
4. Radar Window Process - 
  * updatingDataInBackgroundThread
  * refreshingWindowContentThread
5. Status Window Process - 
  * updatingDataInBackgroundThread
  * refreshingWindowContentThread
6. Log Publisher/Saver Process - 
  
  
## Installation
Additionaly to standard cpp language installation, application requires sfml library to run visualisation. It can be downloaded simply by running undermentioned command in bash terminal

```bash
sudo apt-get install libsfml-dev
```

## Usage

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
