#pragma once
#include <iostream>
#include "UniformBuffer.h"

#include "GL/glut.h"
#include "gmtk/util.h"

/*
 * timer class
 * - this lets us keep track of the elapsed time
 *   in our program easily so we can use it in things
 *   like physics calculations 
 */
class Timer
{
public:
	Timer()
	{
		previousTimeDub = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
		previousTime = static_cast<float>(previousTimeDub);
		totalTimeDub = 0.0;
		totalTime = static_cast<float>(totalTimeDub);
		m_pUBO.allocateMemory(sizeof(float));
		this->tick();
		m_pUBO.bind(0);
	}

	~Timer();

	// update the timer clock
	float tick()
	{
		currentTimeDub = static_cast<double>(glutGet(GLUT_ELAPSED_TIME));
		elapsedTimeDub = currentTimeDub - previousTimeDub;
		previousTimeDub = currentTimeDub; 
		totalTimeDub += elapsedTimeDub * 0.001;

		currentTime = static_cast<float>(currentTimeDub);
		elapsedTime = static_cast<float>(elapsedTimeDub);
		previousTime = currentTime;
		totalTime += elapsedTime * 0.001f;

		m_pUBO.sendFloat(totalTime, 0);
		return elapsedTime;
	}

	// delta time in milliseconds 
	float getElapsedTimeMS()
	{
		return elapsedTime;
	}

	// delta time in seconds
	float getElapsedTimeSeconds()
	{
		return elapsedTime / 1000.f;
	}

	float getCurrentTime()
	{
		return currentTime;
	}

private:
	double currentTimeDub, previousTimeDub, elapsedTimeDub, totalTimeDub;
	float currentTime, previousTime, elapsedTime, totalTime;
	UniformBuffer m_pUBO;
};
