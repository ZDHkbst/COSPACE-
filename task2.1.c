/////////////////////////////////////////////////////////////////////////
//                                                                     //
//	CoSpace Robot                                                      //
//	Version 2.0.0                                                      //
//	DEC 16 2022                                                        //
//	Copyright (C) 2022 CoSpace Robot. All Rights Reserved              //
//                                                                     //
//  ONLY C Code can be compiled.                                       //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#define CsBot_AI_H//DO NOT delete this line
#ifndef CSBOT_REAL
#include <windows.h>
#include <stdio.h>
#include <math.h>
#define DLL_EXPORT extern __declspec(dllexport)
#define false 0
#define true 1
#endif
int Duration = 0;
int CurAction = -1;
int CurGame = 0;
int MyControl = 0;
int MyControl2 = 0;
int Mycontrol3 = 1;
int US_Front = 0;
int US_Left = 0;
int US_Right = 0;
int PositionX = 0;
int PositionY = 0;
int RotationZ = 0;
int DeltaAngle = 0;
int DeltaDist = 0;
int NavID = 0;
int NavDist = 0;
int CtrID = 0;
int CtrDist = 0;
int StnID = 0;
int StnDist = 0;
int Time = 0;
int MyState = 0;
int WheelLeft = 0;
int WheelRight = 0;
int LED = 0;
int AI_TeamID = 1;   //Robot Team ID.    1:Blue Ream;    2:Red Team.
int AI_SensorNum = 15;


#define CsBot_AI_C//DO NOT delete this line

////////////CoSpace System Functions, Please DON't modify./////////////////////
///////////////////////////////////////////////////////////////////////////////

char TaskString[256];//For Individual Challenge ONLY
struct Task
{
    int TaskID;
    int StationID;
    int ActionID;
} TaskList[10];//For Individual Challenge ONLY

DLL_EXPORT int  GetGameID()
{
    return CurGame;
}
DLL_EXPORT int GetCurAction()
{
    return CurAction;
}

DLL_EXPORT void SetTaskString(char* gameTask)
{
    strcpy(TaskString, gameTask);
    printf(gameTask);
    printf("\r\n");
}

DLL_EXPORT void AddTaskItem(int TaskID, int StationID, int ActionID)
{
    if (TaskID < 0 || TaskID >= 10) return;
    TaskList[TaskID].TaskID = TaskID;
    TaskList[TaskID].StationID = StationID;
    TaskList[TaskID].ActionID = ActionID;
    printf("Task:(TaskID:%d, StID:%d, ActID:%d)\r\n", TaskID, StationID, ActionID);
}
//May not work well in Swarmland. 

int iTurnToGoAhead = 0;
int TurnTo(int curRot, int targetRot)
{
    int angularErrorThreshold = 2;
    int minTuningSpeed = 4;
    int angleDiff = (curRot - targetRot + 360) % 360;
    int turningSpeed;
   
    Duration = 128;
    //SuperDuration = 128; //Uncomment this statement for super Action.

    if (iTurnToGoAhead > 0)
    {
        iTurnToGoAhead = iTurnToGoAhead + 1;
        WheelLeft = 40;
        WheelRight = 40;
        if (iTurnToGoAhead > 8) //Stop this turning action after go ahead 8 time units. 
        {
            Duration = 0;
            //SuperDuration = 0;//Uncomment this statement for super Action.
            iTurnToGoAhead = 0;
            return 0;
        }
        return 1;
    }

    if (angleDiff <= angularErrorThreshold || angleDiff > 360 - angularErrorThreshold)
        turningSpeed = 0;
    else if (angleDiff <= 180)
        turningSpeed = angleDiff / 6 + minTuningSpeed;
    else
        turningSpeed = (angleDiff - 360) / 6 - minTuningSpeed;
    WheelLeft = turningSpeed;
    WheelRight = -WheelLeft;
  
    if (turningSpeed == 0) //Reach the targetRot, continue to go ahead.
        iTurnToGoAhead = 1;

    return 1;
}



struct DeliveryItem
{
    int ItemID;
    int CenterID;
    int CollectionPtID;
    int Deadline;
    int ItemScore;
    int CurStatus; //1:at Center; 2:on Car; 3:at Station;
} DeliveryItemList[100]; //All Items

DLL_EXPORT void AddDeliveryItem(int ItemID, int CenterID, int StationID, int ItemScore, int Deadline, int CurStatus)
{
    if (ItemID < 0 || ItemID>99) {
        printf("ItemID Error! ID = %d", ItemID);
        return;
    }
    DeliveryItemList[ItemID].ItemID = ItemID;
    DeliveryItemList[ItemID].CenterID = CenterID;
    DeliveryItemList[ItemID].CollectionPtID = StationID;
    DeliveryItemList[ItemID].ItemScore = ItemScore;
    DeliveryItemList[ItemID].Deadline = Deadline;
    DeliveryItemList[ItemID].CurStatus = CurStatus;
}
DLL_EXPORT void RequestItems(int* Items)
{
    for (int i = 0; i < 6; i++)
    {
        Items[i] = -1;
    }
}

 
void GameStart()
{
//Add your code here

}

 
void AILoopStart()
{
//Add your code here
printf("\r\nCurAction=%d  MyControl=%d", CurAction, MyControl);
}

 
DLL_EXPORT char* GetTeamName()
{
     return "SFDSA";
}

char info[3000];
DLL_EXPORT char* GetDebugInfo()
{
    sprintf(info, "Duration=%d;CurAction=%d;CurGame=%d;MyControl=%d;MyControl2=%d;Mycontrol3=%d;US_Front=%d;US_Left=%d;US_Right=%d;PositionX=%d;PositionY=%d;RotationZ=%d;DeltaAngle=%d;DeltaDist=%d;NavID=%d;NavDist=%d;CtrID=%d;CtrDist=%d;StnID=%d;StnDist=%d;Time=%d;MyState=%d;WheelLeft=%d;WheelRight=%d;LED=%d;",Duration,CurAction,CurGame,MyControl,MyControl2,Mycontrol3,US_Front,US_Left,US_Right,PositionX,PositionY,RotationZ,DeltaAngle,DeltaDist,NavID,NavDist,CtrID,CtrDist,StnID,StnDist,Time,MyState,WheelLeft,WheelRight,LED);
    return info;
}
DLL_EXPORT void SetGameID(int GameID)
{
    int i = 0;
    if (CurGame != GameID)
    {
        GameStart();
        for (i = 0; i < 100; i++)
        {
            DeliveryItemList[i].ItemID = i;
            DeliveryItemList[i].CenterID = -1;
            DeliveryItemList[i].CollectionPtID = 0;
            DeliveryItemList[i].Deadline = 0;
            DeliveryItemList[i].ItemScore = 0;
            DeliveryItemList[i].CurStatus = 0;
        }
        for (i = 0; i < 10; i++)
        {
            TaskList[i].TaskID = i;
            TaskList[i].StationID = 0;
            TaskList[i].ActionID = 0;
        }
    }
    CurGame = GameID;
}

DLL_EXPORT void SetDataAI(volatile int* packet, volatile int *AI_IN)
{

    int sum = 0;

    US_Front = AI_IN[0]; packet[0] = US_Front; sum += US_Front;
    US_Left = AI_IN[1]; packet[1] = US_Left; sum += US_Left;
    US_Right = AI_IN[2]; packet[2] = US_Right; sum += US_Right;
    PositionX = AI_IN[3]; packet[3] = PositionX; sum += PositionX;
    PositionY = AI_IN[4]; packet[4] = PositionY; sum += PositionY;
    RotationZ = AI_IN[5]; packet[5] = RotationZ; sum += RotationZ;
    DeltaAngle = AI_IN[6]; packet[6] = DeltaAngle; sum += DeltaAngle;
    DeltaDist = AI_IN[7]; packet[7] = DeltaDist; sum += DeltaDist;
    NavID = AI_IN[8]; packet[8] = NavID; sum += NavID;
    NavDist = AI_IN[9]; packet[9] = NavDist; sum += NavDist;
    CtrID = AI_IN[10]; packet[10] = CtrID; sum += CtrID;
    CtrDist = AI_IN[11]; packet[11] = CtrDist; sum += CtrDist;
    StnID = AI_IN[12]; packet[12] = StnID; sum += StnID;
    StnDist = AI_IN[13]; packet[13] = StnDist; sum += StnDist;
    Time = AI_IN[14]; packet[14] = Time; sum += Time;
    MyState = AI_IN[15]; packet[15] = MyState; sum += MyState;
    packet[16] = sum;

}
DLL_EXPORT void GetCommand(int *AI_OUT)
{
    AI_OUT[0] = WheelLeft;
    AI_OUT[1] = WheelRight;
    AI_OUT[2] = LED;
    AI_OUT[3] = MyState;
}
void Game0()
{

    if(Duration>0)
    {
        Duration--;
    }
    else if(NavDist<=6&&(MyControl == 1))
    {
        Duration = 22;
        CurAction =1;
    }
    else if(StnDist>=-2 && StnDist<=2&&(MyControl == 0))
    {
        Duration = 91;
        CurAction =2;
    }
    else if(NavDist<=6&&(NavID == 23 || NavID == 28 || NavID == 12 || NavID == 13 || NavID == 10))
    {
        Duration = 25;
        CurAction =3;
    }
    else if(NavDist<=6&&(NavID == 29 || NavID == 17 || NavID == 7 || NavID == 6 || NavID == 5 || NavID == 16 || NavID == 22 || NavID == 18 
))
    {
        Duration = 25;
        CurAction =4;
    }
    else if(NavDist<=6&&(NavID == 19||NavID == 32||NavID == 4||NavID == 14))
    {
        Duration = 39;
        CurAction =5;
    }
    else if(NavID == 13)
    {
        Duration = 10;
        CurAction =6;
    }
    else if(true)
    {
        Duration = 0;
        CurAction =7;
    }
    switch(CurAction)
    {
        case 1:
            WheelLeft=-38;
            WheelRight=38;
            LED=0;
            MyControl = 0;
            if(Duration<8) // Go Forward to Leave Center.
            {
               WheelLeft = 80;
               WheelRight = 80;
            }
            break;
        case 2:
            WheelLeft=0;
            WheelRight=0;
            LED=1;
            if(Duration < 12)
            {
               if( StnID == 1 || StnID == 11 || StnID == 8 || StnID == 13 || StnID == 15 )
               {
                  MyControl = 1; //Need Turn 180degree;
                  Duration = 0;
               }
               //else go forward to leave Center.
               WheelLeft = 40;
               WheelRight = 40;   
            }
            
            break;
        case 3:
            WheelLeft=38;
            WheelRight=0;
            LED=0;
            break;
        case 4:
            WheelLeft=0;
            WheelRight=38;
            LED=0;
            break;
        case 5:
            WheelLeft=26;
            WheelRight=54;
            LED=0;
            break;
        case 6:
            WheelLeft=10;
            WheelRight=40;
            LED=0;
            break;
        case 7:
            WheelLeft=0;
            WheelRight=0;
            LED=0;
            float kp,kd,P,D,PID;
            	int previousError;
            	kp=0.5;
            	kd=40;
            	previousError = DeltaDist;
            	P = DeltaDist;
                 	D = DeltaDist-previousError;
            	PID= kp*P+kd*D;
            	   if (PID<40)
                 {PID=PID;}
               else
                 {PID=40;}
            	WheelLeft=40+PID;
            	WheelRight=40-PID;
            break;
        default:
            break;
    }

}


DLL_EXPORT void AILoop()
{
    AILoopStart();
    switch (CurGame)
    {
    case 0:
        Game0();
        break;
    default:
        WheelLeft=0;
        WheelRight=0;
        LED=0;
        MyState=0;
        break;
    }
}