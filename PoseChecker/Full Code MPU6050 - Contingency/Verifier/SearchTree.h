#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include<Arduino.h>

//PLEASE EDIT HERE BEFORE EDITING DATA
#define Rows 9            // # of Poses
#define Columns 8         // # of Float values for each Pose
#define Error 13           // Acceptable distance from registered value

class SearchTree
{
  private:
    //Remember: 1 Line = 1 pose = 48 bytes = 4 bytes from 1 float * 3 parameters * 4 Accelerometers, we only have 25,656 remaining bytes
    const float DATA[Rows][Columns] = {
       {-78.01,-1.39,84.32,-5.29,-14.29,13.42,57.29,-47.16},// mountain pose
       {-7.49,-81.58,67.44,0.46,-22.91,-41.57,56.42,-39.46},//plank pose
       {-12.34,-76.81,53.37,10.58,-15.24,-61.04,56.37,-36.18},// dolphin plank pose
       {-4.51,-42.96,81.38,7.41,7.02,81.01,56.79,-60.75},// side angle pose
       {-75.54,-11.64,52.95,-34.76,-52.64,-0.62,57.17,-34.72},// warrior 1 with prayer right
       {-62.4,-22.73,62.42,-27.32,11.33,74.45,57.04,-43.22}, // warrior 1 with prayer left
        {-77.53,-12.16,19.05,-67.78,-51.33,8.57,57.11,-14.36}, // warrior 2 right 
       {-67.57,-14.93,8.5,-57.05,10.26,69.4,57.08,-7.0}, // warrior 2 left
       {-0.99,-84.52,86.74,-0.7,4.7,0.6,56.52,-45.64} // cow pose 
      };
  public:
    SearchTree();
    int search(float value[Columns]);
};

#endif
