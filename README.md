# Good Things Come in Threes: The Impact of Robot Responsiveness on Workload and Trust in Multi-User Human-Robot Collaboration

For any questions and clarifications, please contact the developer Mr. Francesco Semeraro at:

francesco.semeraro@manchester.ac.uk

Read full paper <a href="https://ieeexplore.ieee.org/document/10801410">here</a>, Presented at [IROS 2024](http://iros2024-abudhabi.org/)
AAM: <a href="https://research.manchester.ac.uk/en/publications/good-things-come-in-threes-the-impact-of-robot-responsiveness-on-">here</a>

In this paper, we propose a testbed for non-dyadic human-robot collaboration for manufacturing applications. The setup involves two users and one robotic manipulator. The users have to perform different operations on a shared object, which is held by the robot. The robot adjusts it to the needs of the users. The current version of the setup can be manually controlled through a Wizard of Oz to inform the robot about the ongoing state of the interaction wth the user. 
This repository contains the code you can use to replicate the results, related to two different strategies of the robot in handling conflictual situation between the requirements of the two users. The repository also has a luanch file that can be used to store information regarding the 3D skeleton poses of the users during the interaction.

### How to cite our paper
If any of the content of this repository was helpful to you, please cite our paper using:
```
@INPROCEEDINGS{10801410,
  author={Semeraro, Francesco and Carberry, Jon and Leadbetter, James and Cangelosi, Angelo},
  booktitle={2024 IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS)}, 
  title={Good Things Come in Threes: The Impact of Robot Responsiveness on Workload and Trust in Multi-User Human-Robot Collaboration}, 
  year={2024},
  volume={},
  number={},
  pages={2471-2478},
  keywords={Concurrent computing;Statistical analysis;Human-robot interaction;Production;Collaborative robots;Manipulators;Manufacturing;Robots;Intelligent robots},
  doi={10.1109/IROS58592.2024.10801410}}
```

### Requirements
To be able to run all the scripts, you will need to install the following:
<br />
1- Python 3.10 <br />
2- ROS 2 Humble Hawksbill  <br />
3- sqlite3. <br />
<br />

### The repository
The main elements of the repository are the following:
<br />
1- "acquisition_block", ROS 2 Humble package to store data through an Azure Kinect camera in compressed bag files <br />
2- "Universal_Robots_ROS2_Driver", official ROS 2 Humble Universal Robots package to communicate with the UR5e  <br />
3- "Azure_Kinect_ROS_Driver", official ROS 2 Foxy Microsoft package to communicate with the Azure Kinect  <br />
4- "control_block", package used to control the robot during the related user study  <br />
5- "interface_msgs", custom ROS 2 messages for this workspace.  <br />
<br />

