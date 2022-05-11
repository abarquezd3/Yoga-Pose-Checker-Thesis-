# Pose Checker
Communicates with all the Accelerometer connected to the body

Required Arduino Libraries:
* RF24 by TMRh20

To use PoseGenerator:
1. Download Processing here: https://processing.org/download
2. Open Main.pde

## Progress List:

### BodyReader:
#### Transmitter Class:
- [x] Initialize Transmitter 
- [x] Send Data
#### Reader Class:
- [x] Switch Channel
- [x] Read data from all accelerometers
- [x] Send string data of accelerometers

### Verifier:
#### Receiver Class:
- [x] Receive data
- [x] Write data to char[]
#### SearchTree Class:
- [x] Search from DATA field
- [x] Include Errors in searching

### PoseGenerator:
#### BoxGuide Class:
- [x] Generate box at default orientations
- [ ] Accept new orientation
