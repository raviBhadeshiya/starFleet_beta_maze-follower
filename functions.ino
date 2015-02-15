
void intiallize()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif

  out(PULL_UP);
  digitalWrite(PULL_UP, 1);

  out(LMFW);
  out(LMRV);
  out(RMFW);
  out(RMRV);

  out(led);
  digitalWrite(led, LOW);
  out(ledGnd);
  digitalWrite(ledGnd, LOW);

  in(irLeftFw);
  in(irRightFw);
  in(irLeftRv);
  in(irRightRv);

  in(COLOR_RIGHT);
  in(COLOR_LEFT);

  pwm(LMFW, 255);
  pwm(LMRV, 255);
  pwm(RMFW, 255);
  pwm(RMRV, 255);

  //attachInterrupt(COLOR_RIGHT,ISRColourSensorRight,FALLING);
  //attachInterrupt(COLOR_LEFT,ISRColourSensorLeft,FALLING);

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-SPEED - 255, 255 - SPEED);
  myPID.SetSampleTime(1);

#ifdef DEBUG
  Serial.println("Program Started Sucessfully");
#endif



  delay(250);
}

void botMove(int speedL, int speedR)
{
  pwm(LMFW, (speedL >= 0) ? 255 - speedL : 255);
  pwm(LMRV, (speedL < 0) ? 255 - abs(speedL) : 255);

  pwm(RMFW, (speedR >= 0) ? 255 - speedR : 255);
  pwm(RMRV, (speedR < 0) ? 255 - abs(speedR) : 255);
}

void wallFollow(int CurrentDistance, bool wallSide, int TargetDistance, int Speed)
{
  Input = CurrentDistance;
  Setpoint = TargetDistance;
  myPID.Compute();


  sl = constrain(SPEED + ((wallSide == LEFT) ? Output : -Output), -255, 255);
  sr = constrain(SPEED + ((wallSide == LEFT) ? -Output : Output), -255, 255);

#ifdef DEBUG
  Serial.print("FLW: ");
  Serial.print((wallSide==LEFT)?"LEFT":"RIGHT");
  Serial.print("\t");

  Serial.print(DistanceFrmLeft);
  Serial.print("\t");

  Serial.print(DistanceFrmRight);
  Serial.print("\t");

  Serial.print(sl);
  Serial.print("\t");

  Serial.print(sr);
  Serial.print("\t");

  Serial.println(D_CENT);


#endif
#ifndef MEASURING
   botMove(sl, sr);
#endif

}

float wallDist(bool dir)
{
  if (dir == LEFT)
  {
    return (ultrasonicLeft.convert(ultrasonicLeft.timing(), Ultrasonic::CM) + CORRECTION_LEFT_ULTRA);
  }
  else//RIGHT
  {
    return (ultrasonicRight.convert(ultrasonicRight.timing(), Ultrasonic::CM) + CORRECTION_RIGHT_ULTRA);
  }

}

void getSpeeds(bool dir)
{
  if (dir == RIGHT)
  {
    sl = constrain(SPEED - Output, -255, 255);
    sr = constrain(SPEED + Output, -255, 255);
  }
  else
  {
    sl = constrain(SPEED + Output, -255, 255);
    sr = constrain(SPEED - Output, -255, 255);

  }
}


bool readIr(int irDir)
{
  return !(digitalRead(irDir));
}


void turn(bool dir)
{
  digitalWrite(led, HIGH);
  botMove(0,0);
  delay(1000);
#ifdef DEBUG
  Serial.println("FORWARD");
#endif
  while ((dir == RIGHT) ? readIr(irRightRv) : readIr(irLeftRv))
  {
    botMove(SPEED+turnFwSpeed,SPEED+turnFwSpeed);
  }
  if (dir == RIGHT)
  {
#ifdef DEBUG
    Serial.println("TURNING : RIGHT");
#endif
    botMove(200, -200);
    delay(425);   // Set as per required
    botMove(SPEED, SPEED);
    delay(200);
  }
  else
  {
#ifdef DEBUG
    Serial.println("TURNING : LEFT");
#endif
    botMove(-200, 200);
    delay(425);
    botMove(SPEED, SPEED);
    delay(200);
  }
#ifdef DEBUG
  Serial.println("TURN COMPLETE");
#endif
  digitalWrite(led, LOW);
}


void colorDetect()
{
  if (readIr(irLeftFw)&&readIr(irLeftRv))
  {
#ifdef DEBUG
    Serial.println("LEFT IR DETECTED");
#endif
    if (digitalRead(COLOR_LEFT) == LOW)
    {
#ifdef DEBUG
      Serial.println("COLOR DETECTED:LEFT SIDE...................>");
#endif
      if (wallDist(LEFT) > WALL_CUTOFF) 
      {       
        #ifdef DEBUG
      Serial.println("CONFORM LEFT----------------------->");
#endif
        turn(LEFT);
      }
    }
  }
  if (readIr(irRightFw)&&readIr(irRightRv))
  {
#ifdef DEBUG
    Serial.println("RIGHT IR DETECTED");
#endif
    if (digitalRead(COLOR_RIGHT) == LOW)
    {
#ifdef DEBUG
      Serial.println("COLOR DETECTED: RIGHT SIDE................>");
#endif
      if (wallDist(RIGHT) > WALL_CUTOFF) 
      {
        #ifdef DEBUG
      Serial.println("CONFORM RIGHT----------------------->");
#endif
      turn(RIGHT);
      }
    }
  }
}


void botFwd(void)
{
  DistanceFrmLeft = wallDist(LEFT);
  DistanceFrmRight = wallDist(RIGHT);


 if ((DistanceFrmLeft  <= (DistanceFrmRight + 10)))
    {
      Wallside = LEFT;
    }
    else
    {
      Wallside = RIGHT;
    }
  if ((isInterstellarSpace)?((DistanceFrmLeft < WALL_CUTOFF_MIN) ||(DistanceFrmRight < WALL_CUTOFF_MIN)) :(((Wallside == LEFT) ? DistanceFrmLeft : DistanceFrmRight) > WALL_CUTOFF))//check curent wallside is need to be changed
  {
   


    if ((DistanceFrmLeft < WALL_CUTOFF) || (DistanceFrmRight < WALL_CUTOFF))
    {
      D_CENT = 12.7;
      isInterstellarSpace=false;
      wallFollow(((Wallside == LEFT) ? DistanceFrmLeft : DistanceFrmRight), Wallside, D_CENT, SPEED); //cont with current wallSide
    }
    else//Intersteller Space
    {
      isInterstellarSpace=true;
      if ((DistanceFrmLeft <= DistanceFrmRight+15))
    {
      Wallside = LEFT;
    }
    else
    {
      Wallside = RIGHT;
    }
      D_CENT = 36.3; //D_CENT+((Wallside==LEFT)?DistanceFrmLeft:DistanceFrmRight)-((Wallside==LEFT)?pDistanceFrmLeft:pDistanceFrmRight);
      wallFollow(((Wallside == LEFT) ? DistanceFrmLeft : DistanceFrmRight), Wallside, D_CENT, SPEED); //cont with current wallSide
    }

  }
  else
  {
    wallFollow(((Wallside == LEFT) ? DistanceFrmLeft : DistanceFrmRight), Wallside, D_CENT, SPEED); //cont with current wallSide
  }

  //  pDistanceFrmRight=DistanceFrmRight;
  //  pDistanceFrmLeft=DistanceFrmLeft;
}

//void ISRColourSensorRight()
//{
//  Serial.println("InterruptRight");
//  noInterrupts();
//  turn(RIGHT);
//  interrupts();
//}
//void ISRColourSensorLeft()
//{
//  Serial.println("InterruptLeft");
//  noInterrupts();
//  //  turn(LEFT);
//  interrupts();
//}

//void botFwd(void)
//{
//  DistanceFrmLeft=wallDist(LEFT);
//  DistanceFrmRight=wallDist(RIGHT);
//
//  if(((Wallside==LEFT)?DistanceFrmLeft:DistanceFrmRight)>WALL_CUTOFF)//check curent wallside is need to be changed
//  {
//    if((DistanceFrmLeft<WALL_CUTOFF) || (DistanceFrmRight<WALL_CUTOFF))
//    {
//      D_CENT=12.7;
//    }
//    else//Intersteller Space
//    {
//      D_CENT=38.1;
//    }
//    if((DistanceFrmLeft<=DistanceFrmRight))
//      {
//        wallFollow(DistanceFrmLeft,LEFT,D_CENT,SPEED);
//        Wallside=LEFT;
//      }
//      else
//      {
//        wallFollow(DistanceFrmRight,RIGHT,D_CENT,SPEED);
//        Wallside=RIGHT;
//      }
//  }
//  else
//  {
//    wallFollow(((Wallside==LEFT)?DistanceFrmLeft:DistanceFrmRight),Wallside,D_CENT,SPEED);//cont with current wallSide
//  }
//}










