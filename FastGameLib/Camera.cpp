#include "Camera.h"

sf::RenderWindow& Camera::getWindow()
{
	return static_cast<sf::RenderWindow&>(*window);
}

void Camera::setWindow(sf::RenderWindow* w)
{
	window = w;
}

void Camera::shake(float shakeIntensity, sf::Time shakeTime)
{
	if (shakeIntensity == 0) return;
	if (this->time.asSeconds() > 0)
	{
		setCenter(lastCenter);
		window->setView(*this);
	}
	intensity = shakeIntensity;
	time = shakeTime;
	/*this->initialTime = time;
	this->angle = rand() % 360;*/
	lastCenter = window->getView().getCenter();
}

void Camera::update(sf::Time dt)
{
	if (time.asSeconds() > 0)
	{
		float shakeFreqX = rand() % 40 + 30;  
		float shakeFreqY = rand() % 40 + 30;
		float shakeFreqY2 = rand() % 40 + 30;
		float shakeSizeX = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(ceil(intensity / 2))) + intensity;
		float shakeSizeY = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(ceil(intensity / 2))) + intensity;
		float shakeSizeY2 = (rand() % 2 ? -1.F : 1.F) * static_cast<float>(rand() % static_cast<int>(ceil(intensity / 2))) + intensity;
		float t = time.asSeconds();  
		float xAdjustment = sin(t * shakeFreqX) * shakeSizeX;  
		float yAdjustment = sin(t * shakeFreqY) * shakeSizeY + cos(t * shakeFreqY2) * shakeSizeY2;  

		//Other method but less elegant
		/*float radius = intensity * time.asSeconds() / initialTime.asSeconds();
		setCenter(lastCenter);
		move(sf::Vector2f(
			radius * sin(angle),
			radius * cos(angle)
		));
		angle += 180 + (rand() % 2 ? rand() % 60 : -rand() % 60);*/
		move(xAdjustment, yAdjustment);
		time = sf::microseconds(time.asMicroseconds() - dt.asMicroseconds());
	}
	else if (time.asSeconds() < 0)
	{
		time = sf::Time::Zero;
		setCenter(lastCenter);
	}
}