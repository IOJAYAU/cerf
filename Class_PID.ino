#include <NKP_ONE.h>

int sensorsValue[5];

int meanV = 2048;
int N = 5;
int motorSpeed;
int baseSpeed = 80;
int rightSpeed,leftSpeed;
int maxSpeed = 95;
int sum_error = 0;
bool flag;
int flaga,flagb;

// PID
int error = 0;
int pre_error = 0;
int Kp = 19.5; //18.5, 19 | 80  19.5
int Kd = 15.5; //11, 12         15
int Ki = 0;


bool B(int n){  
  if(n < meanV){ // is black
      return true;
  }else{
      return false;
  } 
}

bool W(int n){  
    if(n >= meanV){ // is white
      return true;
  }else{
      return false;
  } 
}

// void turnrightuntilblack(bool one, bool two, bool three, bool four, bool five){
//   while(one == false && two == false && three == false && four == false && five == false){
//     motor(1, 40);
//     motor(2, -40);
//   }
// }

// void turnleftuntilblack(bool one, bool two, bool three, bool four, bool five){
//   while(one == false && two == false && three == false && four == false && five == false){
//     motor(1, -40);
//     motor(2, 40);
//   }
// }

void LineTracking_PID()
{
    
    sensorsValue[0] = analogRead(A0);
    sensorsValue[1] = analogRead(A1);
    sensorsValue[2] = analogRead(A2);
    sensorsValue[3] = analogRead(A3);
    sensorsValue[4] = analogRead(A4);
   
    if( W(sensorsValue[0]) && W(sensorsValue[1]) && W(sensorsValue[2]) && W(sensorsValue[3]) && B(sensorsValue[4]) ){ 
        error = 4;
    }else if( W(sensorsValue[0]) && W(sensorsValue[1]) && W(sensorsValue[2]) && B(sensorsValue[3]) && B(sensorsValue[4]) ){
      error = 3;
    }else if( W(sensorsValue[0]) && W(sensorsValue[1]) && W(sensorsValue[2]) && B(sensorsValue[3]) && W(sensorsValue[4]) ){
      error = 2;
    }else if( W(sensorsValue[0]) && W(sensorsValue[1]) && B(sensorsValue[2]) && B(sensorsValue[3]) && W(sensorsValue[4]) ){
      error = 1;
    }else if( W(sensorsValue[0]) && W(sensorsValue[1]) && B(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4]) ){
      error = 0;
    }else if( W(sensorsValue[0]) && B(sensorsValue[1]) && B(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4]) ){
      error = -1;
    }else if( W(sensorsValue[0]) && B(sensorsValue[1]) && W(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4]) ){
      error = -2;
    }else if( B(sensorsValue[0]) && B(sensorsValue[1]) && W(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4]) ){
      error = -3;
    }else if( B(sensorsValue[0]) && W(sensorsValue[1]) && W(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4]) ){
        error = -4;
    }
    /// check WWWWW
    else if( W(sensorsValue[0]) && W(sensorsValue[1]) && W(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4]) ){
        error = pre_error;
    }
   
   motorSpeed = Kp*error + Kd*(error - pre_error) + Ki*(sum_error);
   leftSpeed = baseSpeed + motorSpeed;
   rightSpeed = baseSpeed - motorSpeed;
   
   if(leftSpeed > maxSpeed){leftSpeed = maxSpeed;}
   if(rightSpeed > maxSpeed){rightSpeed = maxSpeed;}

   if(leftSpeed < -maxSpeed) {leftSpeed = -maxSpeed;}
   if(rightSpeed < -maxSpeed) {rightSpeed = -maxSpeed;}
   
  //  Serial.print("LS: ");
  //  Serial.println(leftSpeed);
  //  Serial.print("RS: ");
  //  Serial.println(rightSpeed);

   motor(2, rightSpeed);
   motor(1, leftSpeed);
   delay(33);  
   
   pre_error = error;
   sum_error += error;

   Serial.print("0: ");
   Serial.println(sensorsValue[0]);
   Serial.print("1: ");
   Serial.println(sensorsValue[1]);
   Serial.print("2: ");
   Serial.println(sensorsValue[2]);
   Serial.print("3: ");
   Serial.println(sensorsValue[3]);
   Serial.print("4: ");
   Serial.println(sensorsValue[4]);

  // countdoLine(1, 3, 0, B(sensorsValue[0]), B(sensorsValue[1]), B(sensorsValue[2]), B(sensorsValue[3]), B(sensorsValue[4]));
  // a = count of line, b = type of line (3 black left, 3 black right, all black), c = what to do (turn left until black, turn left until right)

  //lcd("e=%d pe=%d|Ls=%d RS=%d|kp=%d kd=%d", error,pre_error, leftSpeed, rightSpeed ,Kp*error, Kd*(error - pre_error) );

  check();
}

void setup()  
{
  IO15();
  Serial.begin(115200);
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  ledcSetup(6, 5000, 8);
  ledcSetup(7, 5000, 8);
  ledcSetup(4, 5000, 8);
  ledcSetup(5, 5000, 8);
  ledcAttachPin(2, 6);
  ledcAttachPin(4, 7);
  ledcAttachPin(16, 4);
  ledcAttachPin(17, 5);
}

void loop()
{
    LineTracking_PID();
}

void check(){
    sensorsValue[0] = analogRead(A0);
    sensorsValue[1] = analogRead(A1);
    sensorsValue[2] = analogRead(A2);
    sensorsValue[3] = analogRead(A3);
    sensorsValue[4] = analogRead(A4);

  if(flagb == 0 && B(sensorsValue[0]) && B(sensorsValue[1]) && B(sensorsValue[2]) && B(sensorsValue[3]) && B(sensorsValue[4])){
    baseSpeed = 30; 
    maxSpeed = 50;
    flagb = 1;    
  }

  if(flag == 0 && B(sensorsValue[0]) && B(sensorsValue[1]) && B(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4])){
    flag = 1;
    ao();
    delay(200);   
    motor(1,30);
    motor(2,30);
    delay(100);     
    motor(1,-21);
    motor(2,21);
    delay(600);
  }

  if(W(sensorsValue[0]) && W(sensorsValue[1]) && B(sensorsValue[2]) && B(sensorsValue[3]) && B(sensorsValue[4])){
    //flaga == 0 && ; flaga = 1;
    int speed = 29;
    motor(1,speed);
    motor(2,-speed);
    delay(160);
    motor(1,30);
    motor(2,30);
    delay(1100);
    motor(1,-speed);
    motor(2,speed);
    delay(160);
    motor(1,60);
    motor(2,60);
    delay(400);
    flag = 0;
    // flaga = 0;
    flagb = 0;
    baseSpeed = 80;
    maxSpeed = 95;
  }

  // if(flaga == 1 && B(sensorsValue[0]) && B(sensorsValue[1]) && B(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4])){
  //   flaga = 2;
  // }

  // if(flaga == 2 && B(sensorsValue[0]) && B(sensorsValue[1]) && B(sensorsValue[2]) && W(sensorsValue[3]) && W(sensorsValue[4])){
  //   baseSpeed = 80;
  //   maxSpeed = 95;
  //   flag = 0;
  //   flaga = 0;
  //   flagb = 0;
  // }
  
}

// int countdoLine(int lineNum, int lineType, int whatDo, bool one, bool two, bool three, bool four, bool five){
//   int count; // a = count of line, b = type of line (3 black left, 3 black right, all black), c = what to do (turn left until black, turn left until right)
//   switch(lineType){
//     case 1:
//       switch(whatDo){
//         case 1:
//             if(one && two && three && four == false && five == false){
//               count++;
//             }
//             if(count == lineNum){
//               turnleftuntilblack(one, two, three, four, five);
//             }
//             break;   
//         case 2:
//             if(one && two && three && four == false && five == false){
//               count++;
//             }
//             if(count == lineNum){
//               turnrightuntilblack(one, two, three, four, five);
//             } 
//             break;
//         case 0:
//             ao();
//             break;
//       }
//     case 2:
//       switch(whatDo){
//         case 1:
//           if(one == false && two == false && three && four && five){
//             count++;
//           }
//           if(count == lineNum){
//             turnleftuntilblack(one, two, three, four, five);
//           } 
//           break;   
//         case 2:
//           if(one == false && two == false && three && four && five){
//             count++;
//           }
//           if(count == lineNum){
//             turnrightuntilblack(one, two, three, four, five);
//           }  
//           break;
//         case 0:
//           ao();
//           break;
//       }
//     case 3:
//       switch(whatDo){
//         case 1:
//           if(one && two && three && four && five){
//             count++;
//           }
//           if(count == lineNum){
//             turnleftuntilblack(one, two, three, four, five);
//           }
//           break;   
//         case 2:
//           if(one && two && three && four && five){
//             count++;
//           }
//           if(count == lineNum){
//             turnrightuntilblack(one, two, three, four, five);
//           } 
//           break;
//         case 0:
//           ao();
//           break;
//     }
//   }
// }


// void check_line(bool a, bool b, bool c, bool d, bool e){
//   int counta, countb;
//   if(a == false && b == false && c && d && e){
//       motor(2,0);
//       motor(1,0);
//       delay(10000);
//     }  
//   if(a && b && c && d && e){
    
//     if(a && b && c && d == false && e == false ){
//       motor(2,0);
//       motor(1,0);
//       delay(1000);
//       motor(1,-30);
//       motor(2,30);
//       delay(300);
//       motor(2,0);
//       motor(1,0);
//       delay(1000);
//     }
//     else{      
//     }
//   }
// }
