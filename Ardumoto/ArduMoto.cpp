// ArduMoto.h - Arduino library to control DC motos using the ArduMoto Shield
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <ArduMoto.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <pins_arduino.h>
#endif

/// Constructor
ArduMoto::ArduMoto()
{
}

/// Destructor
ArduMoto::~ArduMoto()
{
//do something here? (free(), delete()?)
}

/// Begin Function: begin with default values
void ArduMoto::begin()
{
  beginMotoA(DIRECTION_PIN_A, PWM_PIN_A);
  beginMotoB(DIRECTION_PIN_B, PWM_PIN_B);
}

/// Begin Function for moto A
void ArduMoto::beginMotoA(int directionPin, int pwmPin)
{
  pinMode(PWM_PIN_A, OUTPUT);                     //Set control pins to be outputs
  pinMode(DIRECTION_PIN_A, OUTPUT);
  analogWrite(PWM_PIN_A, 100);                    //set moto to run at (100/255 = 39)% duty cycle (slow)
}

/// Begin Function for moto A
void ArduMoto::beginMotoB(int directionPin, int pwmPin)
{
  pinMode(PWM_PIN_B, OUTPUT);
  pinMode(DIRECTION_PIN_B, OUTPUT);
  analogWrite(PWM_PIN_B, 100);                    //set moto to run at (100/255 = 39)% duty cycle (slow)
}

/// setSpeed: from -100 to + 100
void ArduMoto::setSpeed(char moto, int speed)
{
  int throttle = map(abs(speed),0,100,0,255);

  if(moto == 'A')
  {
    _motoSpeedA = speed;
    if(speed >= 0)
    {
      digitalWrite(DIRECTION_PIN_A, HIGH);
    }
    if(speed < 0)
    {
      digitalWrite(DIRECTION_PIN_A, LOW);
    }
#ifdef DEBUG_MOTORS
    Serial.print("Setting Motor A to ");
    Serial.println(speed);
#endif
    analogWrite(PWM_PIN_A, throttle);
  }
  else if (moto == 'B')
  {
    _motoSpeedB = speed;
    if(speed >= 0)
    {
      digitalWrite(DIRECTION_PIN_B, HIGH);
    }
    if(speed < 0)
    {
      digitalWrite(DIRECTION_PIN_B, LOW);
    }
#ifdef DEBUG_MOTORS
    Serial.print("Setting Motor B to ");
    Serial.println(speed);
#endif
    analogWrite(PWM_PIN_B, throttle);
  }
}

/// fadeSpeed
void ArduMoto::fadeSpeed(char moto, int desiredSpeed)
{
  int tempspeed;
  int difference;

  if(moto == 'A')
  {
    difference = _motoSpeedA - desiredSpeed;
#ifdef DEBUG_MOTORS
    Serial.print("Difference: ");
    Serial.println(difference);
#endif
    tempspeed = _motoSpeedA;

    while( tempspeed != desiredSpeed)
    {
      tempspeed = tempspeed - (difference/FADESPEED);
      setSpeed(moto, tempspeed);
#ifdef DEBUG_MOTORS
      Serial.print("Setting Motor A to ");
      Serial.println(tempspeed);
#endif
      delay(10);
    }
  }
  if(moto == 'B')
  {
    tempspeed = _motoSpeedB;
    difference = _motoSpeedB - desiredSpeed;
#ifdef DEBUG_MOTORS
    Serial.print("Difference: ");
    Serial.println(difference);
#endif
    while( tempspeed != desiredSpeed)
    {
      tempspeed = tempspeed - (difference/FADESPEED);
      setSpeed(moto, tempspeed);
#ifdef DEBUG_MOTORS
      Serial.print("Setting Motor B to ");
      Serial.println(tempspeed);
#endif
      delay(10);
    }
  }
}

/// stop
void ArduMoto::stop(char moto)
{
  setSpeed(moto, 0);
#ifdef DEBUG_MOTORS
  if(moto == 'A')
  {
    Serial.println("Stopping Motor A");
  }
  else if (moto == 'B')
  {
    Serial.println("Stopping Motor B");
  }
#endif

}

/// brake
void ArduMoto::brake(char moto)
{
  if(moto == 'A')
  {
#ifdef DEBUG_MOTORS
    Serial.println("Braking Motor A");
    setSpeed(moto, - _motoSpeedA/BRAKESPEED);
    fadeSpeed(moto,0);
#endif
  }
  else if (moto == 'B')
  {
    Serial.println("Braking Motor B");
    setSpeed(moto, - _motoSpeedB/BRAKESPEED);
    fadeSpeed(moto,0);
  }
}
