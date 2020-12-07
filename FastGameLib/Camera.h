#pragma once
#ifndef INCLUDE_CAMERA
#define INCLUDE_CAMERA

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class Camera : public sf::View
{
private:
	sf::Time time = sf::Time::Zero;
	/*sf::Time initialTime = sf::Time::Zero;
	float angle = 0;*/
	float intensity = 0;
	sf::Vector2f lastCenter;
	sf::Vector2f newCenter;
	sf::RenderWindow* window = nullptr;

public:

	/**
	 * \public
	 * \return A reference on the game window
	 */
	sf::RenderWindow& getWindow();

	/**
	 * \public
	 * \param w The game window 
	 */
	void setWindow(sf::RenderWindow* w);

	/**
	 * \public
	 * Shakes the camera according to the given parameters
	 * \param shakeIntensity The intensity of the shaking
	 * \param shakeTime The duration of the shaking
	 */
	void shake(float shakeIntensity, sf::Time shakeTime);

	/**
	 * \public
	 * \param dt The time since the last frame
	 */
	void update(sf::Time dt);
};

#endif // !INCLUDE_CAMERA

