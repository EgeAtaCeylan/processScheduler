// Written by : Ege Ata Ceylan
//THIS PROGRAM IS AN IMPLEMENTATION OF A PROCESS SCHEDULER WITH MULTI LEVEL FEEDBACK QUEUE ALGORITHM
//MLFQ is used in operating systems (OS) as a part of the scheduler, which stores processes and determines the order in which the processes need to be executed.
//THESE ARE IMPLEMENTATION DETAILS OF THE PROGRAM
/*
 P1 and P2 are processes:
 1. Rule 1. If Priority(P1) > Priority(P2), P1 runs P2 does not.
 2. Rule 2. If Priority(P1) == Priority(P2), P1 and P2 run in a round-robin fashion.
 3. Rule 3. When a process enters the system, it is placed at the highest priority (the
 topmost queue).
 4. Rule 4a. If a process uses the entire time slice while running, its priority is reduced by
 one.
 5. Rule 4b. If a process gives up the CPU before its time slice is completed, it stays at the same priority level.
 6. Rule 5. After some time period S, all the processes are moved to the topmost queue.
 */

//INPUT FILES
/*
   configuration.txt = has three lines, which indicate the number of queues, the number of processes, and the value of S, respectively. S indicates the number of time slices that need to be passed before all the processes are moved to the top priority queue.
   An example configuration file is given below:
   4 //number of queues
   4 //number of processes
   3 // size of S

   p<process ID>.txt = The process file of a process specifies what the process does in each time slice. More specifically, in a process file, each line corresponds to a time slice and the value at the line depicts what the process does in the time slice. There are three values that can be placed at a line: 0, 1, and ‘-’. 0 indicates that the process gives up the CPU before its time slice is completed, such that Rule 4b applies. 1 indicates that the process uses the entire time slice while running, such that Rule 4a applies. And, ‘-’ depicts that the process has terminated in the respective time slice.
   An example process file is given below:
   1
   0
   1
   1
   1
   -
 */

//OUTPUT FILE
/*
 As the output we output what has happened in each time slice; what type of operation is carried out, which process was involved, and what was the destination (i.e., priority queue) of the process involved, respectively. The output is written to a file, named output.txt.
 In the output, the symbol B refers to the operations done when S is reached. And, the symbol E indicates the termination of a process, in which case the destination queue will be indicated by QX.
 An example output file is given below:
 1, PC1, Q3
 0, PC1, Q3
 1, PC1, Q2
 1, PC1, Q1
 1, PC1, Q1
 1, PC1, Q1
 B, PC1, Q4
 0, PC1, Q4
 E, PC1, QX
 */

//SOME SAMPLE RUNS/TEST CASES ARE PROVIDED AS FILES

#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

 //THIS FUNCTION READS THE PROCESS INFORMATION FOR ALL THE PROCESSES FROM THEIR RESPECTIVE FILE AND STORES THEM INSIDE A VECTOR OF INTEGER QUEUES.STORING EACH OF THE PROCESSES PROCESS FILE INFORMATION IN RESPECTIVE INDEXES ( SUCH AS PROCESSINFOS[i] WILL HAVE THE INFORMATION OF THE PROCESS WITH NUMBER i+1 ),ALSO TO STORE THE INFORMATION IN THE PROCESS FILES OPENING AND READING THEM ONE BY ONE
void storeProcessInfos(vector<queue<int>> & processInfos,const int numOfProcesses,const string processFolderName)
{
    for (int j= 1; j <=numOfProcesses; j++)
    {
        ifstream currentProcessFile;
        currentProcessFile.open((processFolderName+"/p"+to_string(j)+".txt").c_str());
        string temp;
        while (currentProcessFile >> temp)
        {
            if ( temp != "-")//IF THE VALUE IS EITHER 1 OR 0 WE ADD IT TO THE QUEUE
            {
                processInfos[j-1].push(stoi(temp));
            }
        }
        currentProcessFile.close();
    }
}


//THIS FUNCTION PERFORMS THE SCHEDULING AND EXECUTION OPERATIONS ON THE MLFQ.IT WORKS UNTIL ALL THE PROCESSES ARE FINISHED.IF A PROCESS DOESN'T USE ALL OF ITS GIVEN TIME IT STAYS AT THE SAME PRIORITY LEVEL.IF A PROCESS USES ALL OF ITS GIVEN TIME IT'S PRIORITY LEVEL IS REDUCED BY ONE.IF A PROCESS FINISHES ITS REMOVED FROM THE MLFQ STRUCTURE
void scheduleExecuteProcesses(vector<queue<int>> & MLFQ,vector<queue<int>> & processInfos,ofstream & outFile,const int numOfProcesses, const int sizeS)
{
//  INITIALIZNG THE LOOP VARIABLE I,TIMESLICE AND FINISHED PROCESS COUNT FOR THE FIRST ITERATION OF THE LOOP
    int i = MLFQ.size()-1;
    int timeSlice = 0;
    int finishedProcesses = 0;
    int topMostQueueNumber = MLFQ.size();
//  UNTIL ALL THE PROCESSES ARE FINISHED
    while (finishedProcesses != numOfProcesses)
    {
        int temp;
        int tempsProcess;
//      WE KEEP ON PERFORMING OPERATIONS UNTIL OUR CURRENT QUEUE IS EMPTY, IF SO WE GO DOWN ONE QUEUE, OR UNTIL WE REACH THE THE S, IF SO WE MOVE ALL THE PROCESSES TO THE TOPMOST QUEUE
        while (MLFQ[i].empty()== false && timeSlice != sizeS)
        {
//          WE GET THE CURRENT PROCESS AND ITS PROCESS INFORMATION
            temp = MLFQ[i].front();
            MLFQ[i].pop();
            tempsProcess = processInfos[temp-1].front();
            processInfos[temp-1].pop();
            timeSlice++;// WE INCREMENT THE TIMESLICE BY ONE
            
//          AFTER GETTING THE PROCESS INFO IF RESPECTIVE PROCESSINFO QUEUE IS EMPTY THAT MEANS THAT THE PROCESS HAS FINISHED EXECUTION
            if (processInfos[temp-1].empty()==true)
            {
                outFile <<"E, PC"<<temp<<", QX"<<endl;//OUTPUTTING RESPECTIVE INFORMATION TO THE OUTPUT FILE
                finishedProcesses++;
            }
//          IF RESPECTIVE PROCESSINFO QUEUE IS NOT EMPTY THAT MEANS THERE ARE STILL SOME PROCESS INFO AND MORE OPERATIONS NEED TO BE DONE FOR THAT PROCESS
            else
            {
//              IF THE PROCESS VALUE IS EQUAL TO 0 WHICH MEANS THAT THE PROCESS WILL STAY AT THE SAME PRIORITY LEVEL
                if (tempsProcess== 0)
                {
                    MLFQ[i].push(temp);
                    outFile <<tempsProcess<<", PC"<<temp<<", Q"<<i+1<<endl;//OUTPUTTING RESPECTIVE INFORMATION TO THE OUTPUT FILE
                }
//              IF THE PROCESS VALUE IS EQUAL TO 1 WHICH MEANS THAT THE PROCESS WILL BE MOVED TO THE LOWER PRIORITY LEVEL
                else
                {
//                  IF WE ARE AT THE LOWEST PRIORITY QUEUE EVEN IF THE PROCESS VALUE IS EQUAL TO 1 WE CANT REDUCE ITS PRIORITY LEVEL SINCE ITS ALREADY AT THE LOWEST LEVEL, SO WE DONT CHANGE IT'S PRIORITY LEVEL AND PLACE IT BACK AGAIN IN THE SAME PRIORITY LEVEL
                    if (i == 0)
                    {
                        MLFQ[i].push(temp);
                        outFile <<tempsProcess<<", PC"<<temp<<", Q"<<i+1<<endl;//OUTPUTTING RESPECTIVE INFORMATION TO THE OUTPUT FILE
                        
                    }
//                  IF WE ARE NOT AT THE LOWEST PRIORITY AND THE PROCESS VALUE IS EQUAL TO 1 WE REDUCE THE PRIORITY LEVEL OF THE PROCESS
                    else
                    {
                        MLFQ[i-1].push(temp);
                        outFile <<tempsProcess<<", PC"<<temp<<", Q"<<i<<endl;//OUTPUTTING RESPECTIVE INFORMATION TO THE OUTPUT FILE
                    }
                }
            }
        }
//      IF WE STILL HAVE PROCESS WHICH ARE EXECUTING
        if (finishedProcesses != numOfProcesses)
        {
//          IF WE REACHED S WE PLACE ALL THE PROCESSES TO THE TOPMOST QUEUE AND WE RESET OUR TIMESLICE AND I SO THAT WE START FROM THE TOPMOST QUEUE AGAIN
            if (timeSlice== sizeS)
            {
                for (int s = MLFQ.size()-2; s>= 0; s--)
                {
                    int tempToMoved;
                    while (MLFQ[s].empty()==false)
                    {
                        tempToMoved = MLFQ[s].front();
                        MLFQ[s].pop();
                        MLFQ[MLFQ.size()-1].push(tempToMoved);
                        outFile<<"B, PC"<<tempToMoved<<", Q"<<topMostQueueNumber<<endl;//OUTPUTTING RESPECTIVE INFORMATION TO THE OUTPUT FILE
                    }
                }
                timeSlice = 0;
                i = MLFQ.size()-1;
            }
//          IF WE DONT HAVE PROCESS WHICH ARE EXECUTING, THAT MEANS OUR CURRENT QUEUE IS EMPTY SO WE MOVE DOWN ONE QUEUE
            else
            {
                    i--;
            }
        }
    }
}

int main()
{
//  INPUTTING PROCESS FOLDER NAME AND AND PRINTING THE OUTPUT FOLDER NAME TO THE USER
    
    string processFolderName;
    string outputFileName;
    cout<<"Please enter the process folder name: ";
    cin>>processFolderName;
    outputFileName = processFolderName+"/"+"output.txt";
    cout << "When all processes are completed, you can find execution sequence in"<<endl;
    cout<<"\""<<outputFileName<<"\""<<"."<<endl;
    
//  READING THE NUMBER OF QUEUES,PROCESSES AND SIZE OF S INFORMATION FROM THE CONFIGURATION FILE
    
    int numOfQueues, numOfProcesses, sizeS;
    ifstream confFile;
    confFile.open((processFolderName+"/configuration.txt").c_str());
    confFile >> numOfQueues;
    confFile >> numOfProcesses;
    confFile >> sizeS;
    confFile.close();
    
//  CREATING A VECTOR OF INTEGER QUEUES WITH SIZE EQUAL TO NUMBER OF QUEUES INFORMATION READ FROM THE CONFIGURATION FILE AND PUTTING ALL THE PROCESSES TO THE HIGHEST PRIORITY QUEUE
    
    vector<queue<int>> MLFQ(numOfQueues);
    for (int i = 1; i<=numOfProcesses; i++)
    {
        MLFQ[MLFQ.size()-1].push(i);
    }
    
//  CREATING A VECTOR OF INTEGER QUEUES WITH THE NUMBER OF PROCESSES TO STORE EACH OF THE PROCESSES PROCESS FILE INFORMATION IN RESPECTIVE INDEXES ( SUCH AS PROCESSINFO[i] WILL HAVE THE INFORMATION OF THE PROCESS WITH ID i+1 ),ALSO TO STORE THE INFORMATION IN THE PROCESS FILES OPENING AND READING THEM ONE BY ONE WE USE THE STOREPROCESSINFOS FUNCTION.FUNCTION IS EXPLAINED ABOVE
    
    vector<queue<int>> processInfos(numOfProcesses);
    storeProcessInfos(processInfos,numOfProcesses,processFolderName);
    
//  CREATING AN OFSTREAM OBJECT TO WRITE THE INFORMATION OF THE OPERATIONS DONE TO A FILE
    ofstream outFile;
    outFile.open(outputFileName.c_str());
    
//  USING THE SCHEDULEEXECUTEPROCESSES FUNCTION TO PERFORM THE OPERATIONS ON MLFQ.THE FUNCTION IS EXPLAINED ABOVE
    scheduleExecuteProcesses(MLFQ, processInfos,outFile,numOfProcesses, sizeS);
    
    outFile.close();
    
//  TO SEE THE OUTPUT ON THE SCREEN
    cin.get();
    cin.ignore();
    
    return 0;
}
