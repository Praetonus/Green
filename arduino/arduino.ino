/************************************************************************
 * Copyright 2014 Benoît Vey                                            *
 *                                                                      *
 * This file is part of Green.                                          *
 *                                                                      *
 * Green is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * Green is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Green.  If not, see <http://www.gnu.org/licenses/>.       *
 ************************************************************************/

#include <Ultrasonic.h>
#include <Stepper.h>

int const trig{3};
int const echo{2};

int const steps{3072};

char const begByte{'b'};
char const endByte{'e'};

Ultrasonic sensor(trig, echo);
Stepper motor(steps, 4, 5, 6, 7);

void setup()
{
	Serial.begin(9600);
	motor.setSpeed(9.9);
}

void loop()
{
	static bool active{false};
	static int val{0};
	static int currentStep{0};

	if (active)
	{
		if (Serial.available() > 0)
			active = (Serial.read() != endByte);
		val = sensor.convert(sensor.timing(), Ultrasonic::CM);
		if (val > 500)
			val = 500;
        	Serial.println(val);
        	motor.step(83);
		currentStep += 83;
		if (currentStep == 83 * 36)
		{
			motor.step(-currentStep);
			currentStep = 0;
		}
	}
	else
	{
		motor.step(-currentStep);
		currentStep = 0;
		while (Serial.available() == 0)
			delay(50);
		active = (Serial.read() == begByte);
	}
}
