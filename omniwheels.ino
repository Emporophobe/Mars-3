#include <Servo.h>

Servo myServo[4];

//const int servoOffset[3] = {15, 12, 12};
const char vectorForces[6][6] = {{'R','F','L','F','B','B'}, //0
{'L','F','B','B','R','B'}, //1
{'B','B','R','B','L','B'}, //2 // Force vector and its direction pairs
{'R','B','L','B','B','F'}, //3 // (R,F = Right Servo Forward , B,B= Back Servo Backward etc.)
{'L','B','B','F','R','F'}, //4
{'B','F','R','F','L','F'}, //5
};

const int archAreaLimits[7] = {0, 60, 120, 180, 240, 300, 360}; // Split the plane in arches
const int maxBackwardSpeed = 1000; // Pulses for max speed back
const int maxForwardSpeed = 1000; // Pulses for max speed ahead
//const int centerPosition = (maxBackwardSpeed + (maxForwardSpeed - maxBackwardSpeed) / 2); // calculate pulses for stop position
const int centerPosition = 0;
const int speedDif = maxForwardSpeed - centerPosition;


int totalSpeed = 100; // Max rover speed percentanse (future use)
int moveDirection; // The direction to move in degrees (0,360=ahead, 90=right, 180=back, 270=left and all between )
int servoSpeed[3] = {centerPosition, centerPosition, centerPosition}; // Array for Servo movement (init with Stop)
bool start = false; // To check if we just start
bool demo = true; // Demo mode, use some specific directions

void setup()
{
myServo[0].attach(5); //
myServo[1].attach(10); // Attach the servos
myServo[2].attach(9); //
myServo[3].attach(6); //
myServo[4].attach(3); //

moveServo(servoSpeed); // Init the servos to center (stop)
}

void loop()
{
//int moveDirection = 240;
while(demo)
{
for (int specDir = 0; specDir < 360; specDir += 30)
{
moveServo(calculateVectorForces(specDir));
delay(3000);
}
}

while (start)
{
moveDirection = 0; // Just move ahead at start
moveServo(calculateVectorForces(moveDirection));
delay(3000);
}

int moveDirection = (millis()%360) ; // Pseudo-randomise Direction

moveServo(calculateVectorForces(moveDirection));
delay(2000);
}


/*
** Functions Start
*/

/*
** int *calculateVectorForces(int moveDir)
** Does as it says, returns a pointer to an array with the final Servos Speed and rotation direction.
** this is the main function
** DOTO : split the calculations to smaller functions
*/
int *calculateVectorForces(int moveDir)
{
int arch = 0; // Split the plane to archs, keeps the arch our direction is on
char whichForcesToUse[6]; // Finds which vector forces we have to combine
int vectorForceAngle; // at what angles
int returnServoSpeed[3]={centerPosition, centerPosition, centerPosition}; // The final output (as pointer) no global var here
int *returnVal = returnServoSpeed ; // The pointer


for (int i = 0 ; i < 6 ; i++)
{
if (archAreaLimits[i] < moveDir)
{
arch = i; // Find on which arch we are
}
}
for (int n = 0; n < 6; n++)
{
whichForcesToUse[n] = vectorForces[arch][n]; // Read the vectors from array for the specific arch
}

for (int n=0 ; n < 6; n += 2) // Walk through the vector forces to calculate
{
switch(whichForcesToUse[n]) // Check them
{
case 'R': // Servo 0
if (whichForcesToUse[n+1] == 'F') // RF
{
vectorForceAngle = abs(300 - moveDir);
vectorForceAngle = vectorForceAngle == 300 ? 0 : vectorForceAngle;
vectorForceAngle = vectorForceAngle < 90 ? vectorForceAngle : 90;
returnServoSpeed[0] = int(centerPosition + (log(1+(cos(convertDegreesToRad(vectorForceAngle))*1.718)) * speedDif));
}else{ //RB
vectorForceAngle = abs(120 - moveDir);
vectorForceAngle = vectorForceAngle == 120 ? 0 : vectorForceAngle;
vectorForceAngle = vectorForceAngle < 90 ? vectorForceAngle : 90;
returnServoSpeed[0] = int(centerPosition - (log(1+(cos(convertDegreesToRad(vectorForceAngle))*1.718)) * speedDif));
}
break;
case 'L': // Servo 1
if (whichForcesToUse[n+1] == 'F') //LF
{
vectorForceAngle = abs(60 - moveDir);
vectorForceAngle = vectorForceAngle == 60 ? 0 : vectorForceAngle;
vectorForceAngle = vectorForceAngle < 90 ? vectorForceAngle : 90;
returnServoSpeed[1] = int(centerPosition - (log(1+(cos(convertDegreesToRad(vectorForceAngle))*1.718)) * speedDif));
}else{ //LB
vectorForceAngle = abs(240 - moveDir);
vectorForceAngle = vectorForceAngle == 240 ? 0 : vectorForceAngle;
vectorForceAngle = vectorForceAngle < 90 ? vectorForceAngle : 90;
returnServoSpeed[1] = int(centerPosition + (log(1+(cos(convertDegreesToRad(vectorForceAngle))*1.718)) * speedDif));
}
break;
case 'B': // Servo 2
if (whichForcesToUse[n+1] == 'F') //BF
{
vectorForceAngle = abs(270 - moveDir);
vectorForceAngle = vectorForceAngle == 270 ? 0 : vectorForceAngle;
vectorForceAngle = vectorForceAngle < 90 ? vectorForceAngle : 90;
returnServoSpeed[2] = int(centerPosition - (log(1+(cos(convertDegreesToRad(vectorForceAngle))*1.718)) * speedDif));
}else{ //BB
vectorForceAngle = abs(90 - moveDir);
vectorForceAngle = vectorForceAngle == 90 ? 90 : vectorForceAngle;
vectorForceAngle = vectorForceAngle < 90 ? vectorForceAngle : 90;
returnServoSpeed[2] = int(centerPosition + (log(1+(cos(convertDegreesToRad(vectorForceAngle))*1.718)) * speedDif));
}
break;
}
}

return returnVal;
}

/*
** void moveServo(int *servoRotation)
*/
void moveServo(int *servoRotation)
{
int *pos = servoRotation;
for (int i = 0; i < 3; i++)
{
myServo[i].writeMicroseconds(pos[i]);
}
}

/*
** float convertDegreesToRad (int degree)
** Convert Degrees to Rad for COS() maths
*/
float convertDegreesToRad (int degree)
{
return (degree * 3.14)/180;
}
