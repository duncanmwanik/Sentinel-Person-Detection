#include <Arduino.h>
#include "def.h"

void setup()
{
    Serial.begin(115200);
    setupFirebase();
    setupTime();
    setupPersonDetector();
}

void loop()
{
    detectPerson();
}
