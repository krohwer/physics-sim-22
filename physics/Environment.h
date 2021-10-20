#pragma once

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

struct Environment {

	/// the vertical size of the environment, measured in meters (m)
	float height;
	/// the horizontal size of the environment, measured in meters (m)
	float width;
	/// the gravitational acceleration of the environment, measured downward in meters/second squared (m/s^2)
	float gravity;
	/// the ratio of windowHeight / environmentHeight, used for converting meters to pixels before rendering
	float pixelRatio;
};

#endif