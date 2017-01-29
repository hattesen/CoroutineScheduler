/**
 * @file SchedulerBlinkMax.ino
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2015-2016, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * @section Description
 * This Arduino sketch uses the Scheduler library.
 * Blink with max number of pins; 64 bytes per task.
 */

#include <Scheduler.h>

#if defined(ARDUINO_ARCH_AVR)
const size_t STACK_SIZE = 64;
#else
const size_t STACK_SIZE = 128;
#endif

int next_pin = 2;
int min_stack = 64;

void blink()
{
  // Allocate a pin and set output mode
  int pin = next_pin++;
  pinMode(pin, OUTPUT);

  // Toggle the pin on and off
  while (1) {
    int bytes = Scheduler.stack();
    if (bytes < min_stack) min_stack = bytes;
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
    delay(500);
  }
}

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBlinkMax: started"));
  Serial.flush();

  // Start a blink task for each pin (use 64 byte stack)
  int tasks = 0;
  const int TASKS_MAX = NUM_DIGITAL_PINS - 2;
  while (tasks < TASKS_MAX && Scheduler.startLoop(blink, STACK_SIZE)) tasks++;
  Serial.print(millis());
  Serial.print(F(":setup::task="));
  Serial.println(tasks);

  Serial.print(millis());
  Serial.print(F(":setup::main:stack="));
  Serial.println(Scheduler.stack());
}

void loop()
{
  unsigned long start = millis();

  // Print the iteration count
  static unsigned int i = 0;
  Serial.print(millis());
  Serial.print(F(":loop::i="));
  Serial.println(i++);

  Serial.print(millis());
  Serial.print(F(":loop::main:stack="));
  Serial.println(Scheduler.stack());

  Serial.print(millis());
  Serial.print(F(":loop::min:stack="));
  Serial.println(min_stack);

  // Delay gracefully
  await((millis() - start) >= 1000);
}
