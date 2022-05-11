# Pose Checker
Communicates with all the Accelerometer connected to the body

Software:
![Screenshot (260)](https://user-images.githubusercontent.com/74640529/167882953-1bdc933f-3c6a-4f02-b95a-51990364a1cd.png)

Emdedded :
![Screenshot (387)](https://user-images.githubusercontent.com/74640529/167881387-6596ce23-4dcf-4b51-8487-026e5ee060de.png)

![Screenshot (378)](https://user-images.githubusercontent.com/74640529/167881358-2e5a85e4-bed6-4fd0-97df-4d715ac52842.png)


Demo Video #1: https://drive.google.com/file/d/1K8rukM-WBUrdklDa2gSfIY9A8WBKTnue/view?usp=sharing                 
Demo Video #2(Yoga Instructor): https://drive.google.com/file/d/18gQjPVl7FKEpDb9RQFc4tmdRfVHFB7mp/view?usp=sharing

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
