#include "ParsecInstrument.h"
#include "StepperMotor.h"
#include <Arduino.h>


void stepperMotorSetup(StepperMotors* stepper) {
    stepper->enPins[0] = 31;
    stepper->enPins[1] = 37;
    stepper->enPins[2] = 43;
    stepper->enPins[3] = 53;
    stepper->stepPins[0] = 35;
    stepper->stepPins[1] = 41;
    stepper->stepPins[2] = 47;
    stepper->stepPins[3] = 53;
    stepper->modePins[0] = 33;
    stepper->modePins[1] = 39;
    stepper->modePins[2] = 45;
    stepper->modePins[3] = 51;

    for(int i = 0; i < MAX_STEPPER_MOTORS; i++) {
        pinMode(stepper->enPins[i], OUTPUT);
        pinMode(stepper->stepPins[i], OUTPUT);
        pinMode(stepper->modePins[i], OUTPUT);
    }
    
}

void stepperMotorPlay(StepperMotors* stepper) {
    if(stepper->notePeriod[0] > 0) {
        stepper->cycle[0] += 40;
        if(stepper->cycle[0] >= stepper->notePeriod[0]) {
            stepperMotorPulse(stepper, 0);
            stepper->cycle[0] = 0;
        }
    }

    if(stepper->notePeriod[1] > 0) {
        stepper->cycle[1] += 40;
        if(stepper->cycle[1] >= stepper->notePeriod[1]) {
            stepperMotorPulse(stepper, 1);
            stepper->cycle[1] = 0;
        }
    }

    if(stepper->notePeriod[2] > 0) {
        stepper->cycle[2] += 40;
        if(stepper->cycle[2] >= stepper->notePeriod[2]) {
            stepperMotorPulse(stepper, 2);
            stepper->cycle[2] = 0;
        }
    }

    if(stepper->notePeriod[3] > 0) {
        stepper->cycle[3] += 40;
        if(stepper->cycle[3] >= stepper->notePeriod[3]) {
            stepperMotorPulse(stepper, 3);
            stepper->cycle[3] = 0;
        }
    }
}

void stepperMotorAutoMode(StepperMotors* stepper, int index) {
    if(stepper->notePeriod[index] < 2800 || stepper->notePeriod[index] > 4200){
        digitalWrite(stepper->modePins[index], HIGH);
    }
    else {
        digitalWrite(stepper->modePins[index], LOW);
    }
}

void stepperMotorPulse(StepperMotors* stepper, int index) {
    digitalWrite(stepper->stepPins[index], HIGH);
    digitalWrite(stepper->stepPins[index], LOW);
}

void stepperMotorHandleMessage(StepperMotors* stepper, byte deviceAddress, byte eventCode, byte data) {
    switch(eventCode){
        case 0xA1:
            stepper->notePeriod[deviceAddress] = 0;
            break;

        case 0xA2:
            stepper->notePeriod[deviceAddress] = pitchVals[data];
            stepperMotorAutoMode(stepper, deviceAddress);
            break;

        case 0xAF:
            stepper->notePeriod[deviceAddress] = 0;
            break;
    }
}
