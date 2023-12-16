#include <Arduino.h>
#include "EloquentTinyML.h"
#include "eloquent.h"
#include "eloquent_tinyml/tensorflow/person_detection.h"
#include "eloquent/vision/camera/aithinker.h"
#include <SPIFFS.h>
#include "def.h"

const uint16_t imageWidth = 160;
const uint16_t imageHeight = 120;

Eloquent::TinyML::TensorFlow::PersonDetection<imageWidth, imageHeight> personDetector;

void setupPersonDetector()
{
    delay(3000);

    // configure camera
    camera.grayscale();
    camera.qqvga();

    while (!camera.begin())
        Serial.println("********** Cannot init camera");

    Serial.println(".......... Camera is good");

    personDetector.setDetectionAbsoluteThreshold(200);
    personDetector.begin();

    // abort if an error occurred on the detector
    while (!personDetector.isOk())
    {
        Serial.print("Detector init error: ");
        Serial.println(personDetector.getErrorMessage());
    }
    Serial.println(".......... Detector is good");
}

void detectPerson()
{
    if (!camera.capture())
    {
        Serial.println("********** Camera capture error");
        delay(1000);
        return;
    }

    bool isPersonInFrame = personDetector.detectPerson(camera.buffer);

    if (!personDetector.isOk())
    {
        Serial.print("********** Person detector detection error: ");
        Serial.println(personDetector.getErrorMessage());
        delay(1000);
        return;
    }

    if (isPersonInFrame)
    {
        Serial.print("\n Person detected");
        Serial.print("\t > score: ");
        Serial.println(personDetector.getPersonScore());

        if (camera.saveTo())
        {
            Serial.println("Saved photo to disk\n");
            postImage();
        }
        else
        {
            Serial.println("********** Failed to save photo to disk\n");
        }
    }
    else
    {
        Serial.println("No person");
    }
}