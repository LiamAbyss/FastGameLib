#include "Scene.h"

void Scene::setGame(Game& globalGame)
{
	game = &globalGame;
	resources = &(game->getResources());
	window = &(game->getWindow());
}

Game& Scene::getGame()
{
	return (Game&)*game;
}

ResourceManager& Scene::getResources()
{
	return (ResourceManager&)*resources;
}

sf::RenderWindow& Scene::getWindow()
{
	return (sf::RenderWindow&)*window;
}

void Scene::setInitialized(bool init)
{
	initialized = init;
}

bool Scene::hasBeenInitialized() const
{
	return initialized;
}
