#include "Game.h"

Game::Game(const std::string& title, const sf::VideoMode& mode, const sf::Uint32& style) : windowTitle(title), windowMode(mode), ev(sf::Event())
{
	srand(static_cast<unsigned int>(time(nullptr)));
	window.create(mode, title, style);
	window.setFramerateLimit(framerate);
	cam.setCenter(window.getDefaultView().getCenter());
	cam.setSize(window.getDefaultView().getSize());
	cam.setWindow(&window);
}

void Game::addScene(const std::string& label, Scene* scene)
{
	scenes.emplace(label, std::shared_ptr<Scene>(scene));
	scenes[label]->setGame((Game&)*this);
}

std::map<std::string, std::shared_ptr<Scene>>& Game::getScenes()
{
	return scenes;
}

void Game::setCurrentScene(const std::string& label)
{
	currentScene = label;
	scenes[label]->initialize();
	scenes[label]->setInitialized(true);
}

ResourceManager& Game::getResources()
{
	return resources;
}

sf::RenderWindow& Game::getWindow()
{
	return window;
}

sf::VideoMode Game::getVideoMode() const
{
	return windowMode;
}

void Game::setVideoMode(sf::VideoMode mode, sf::Uint32 style)
{
	window.create(mode, windowTitle, style);
	window.setFramerateLimit(framerate);
	windowMode = mode;
	cam.setCenter(window.getDefaultView().getCenter());
	cam.setSize(window.getDefaultView().getSize());
}

unsigned short Game::getFramerate() const
{
	return framerate;
}

void Game::setFramerate(unsigned short rate)
{
	framerate = rate;
	window.setFramerateLimit(framerate);
}

void Game::launch()
{
	if (scenes.empty())
	{
		std::cout << "Unable to launch the game : No scene could be found." << std::endl;
		return;
	}
	window.setFramerateLimit(framerate);
	gameClock.restart();
	framerateClock.restart();
	updateClock.restart();
	while (window.isOpen())
	{
		//sf::Time updateTime = updateClock.restart();
		while (window.pollEvent(ev))
		{
			scenes[currentScene]->update(updateClock.getElapsedTime(), ev);
			if (ev.type == sf::Event::Closed)
				window.close();
		}
		scenes[currentScene]->update(updateClock.getElapsedTime());
		cam.update(updateClock.restart());
		window.setView(cam);
		//if (framerateClock.getElapsedTime().asMilliseconds() >= 1000 / framerate)
		//{
		window.clear();
		scenes[currentScene]->render();
		window.display();
		framerateClock.restart();
		//}
	}
	std::cout << "Game time : " << gameClock.restart().asSeconds() << "s" << std::endl;
}

bool Game::setWShape(const sf::Image& image) const
{
	return ::setWShape(window.getSystemHandle(), image);
}

bool Game::setWTransparency(unsigned char alpha) const
{
	return ::setWTransparency(window.getSystemHandle(), alpha);
}

Camera& Game::getCamera()
{
	return cam;
}

sf::Event& Game::getEvent()
{
	return ev;
}
