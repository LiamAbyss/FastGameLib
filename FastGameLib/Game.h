#ifndef INCLUDE_GAME
#define INCLUDE_GAME

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <ctime>
#include <cstdlib>
#include "Clickable.h"
#include "ResourceManager.h"
#include "TransparentWindow.h"
#include "Camera.h"
#include "Scene.h"

class Scene;

/** 
 * Global container for the game.
 */
class Game
{
private:
	/** 
	 * \private
	 * Contains all the scenes of the game.
	 * \see Scene
	 */
	std::map<std::string, std::shared_ptr<Scene>> scenes;

	/** 
	 * \private
	 * Manages all the resources.
	 */
	ResourceManager resources;

	/** 
	 * \private
	 * Time since launch() call.
	 */
	sf::Clock gameClock;

	/** 
	 * \private
	 * Timer used to keep a stable maximum framerate.
	 */
	sf::Clock framerateClock;

	/** 
	 * \private
	 * Time passed to Scene::update() since the last update.
	 */
	sf::Clock updateClock;

	/** 
	 * \private
	 * Title of the main window.
	 */
	std::string windowTitle;

	/** 
	 * \private
	 * Main window.
	 */
	sf::RenderWindow window;

	/** 
	 * \private
	 * Dimensions of the main window.
	 */
	sf::VideoMode windowMode;

	/** 
	 * \private
	 */
	unsigned short framerate = 60;

	/** 
	 * \private
	 * Label of the current scene.
	 * \see Scene
	 */
	std::string currentScene;

	/**
	 * \private
	 * Camera managing the view of the game.
	 */
	Camera cam;

	/**
	 * \private
	 * SFML event passed to Scene::update().
	 */
	sf::Event ev;

public:

	/**
	 * \public
	 * Game constructor
	 * \param title Title of the main window.
	 * \param mode Dimensions of the main window.
	 * \param style Style of the main window.
	 */
	Game(const std::string& title, const sf::VideoMode& mode, const sf::Uint32& style = 7U);

	/**
	 * \public
	 * Adds a scene to be loaded afterwards.
	 * \param label Label of the scene.
	 * \param scene A pointer on the scene allocated with \c new.
	 * \see Scene
	 */
	void addScene(const std::string& label, Scene* scene);

	/**
	 * \public
	 * \return A reference to the map Game::scenes.
	 * \see Scene
	 */
	std::map<std::string, std::shared_ptr<Scene>>& getScenes();

	/**
	 * \public
	 * Sets the current scene.
	 * \param label Label of the scene.
	 * \see Scene 
	 */
	void setCurrentScene(const std::string& label);

	/**
	 * \public
	 * \return A reference to the Game::resources.
	 * \see ResourceManager
	 */
	ResourceManager& getResources();

	/**
	 * \public
	 * \return A reference to the main window.
	 */
	sf::RenderWindow& getWindow();

	/**
	 * \public
	 * \return A reference to the dimensions of the main window.
	 */
	sf::VideoMode getVideoMode() const;

	/**
	 * \public
	 * \param mode New video mode. 
	 */
	void setVideoMode(sf::VideoMode mode, sf::Uint32 style = 7U);

	/**
	 * \public
	 * \return The maximum framerate of the game.
	 */
	unsigned short getFramerate() const;

	/**
	 * \public
	 * \param rate The maximum framerate of the game.
	 */
	void setFramerate(unsigned short rate);

	/** 
	 * \public
	 * Launches the game.
	 */
	void launch();

	/**
	 * \public
	 * Sets the shape of the window according to an image with transparent parts.
	 * \param image The image to shape the window.
	 */
	bool setWShape(const sf::Image& image) const;

	/**
	 * \public
	 * Sets the transparency of the window.
	 * \param alpha the opacity (0-255).
	 */
	bool setWTransparency(unsigned char alpha) const;

	/**
	 * \public
	 * \return A reference on the game camera.
	 * \see Camera 
	 */
	Camera& getCamera();

	/**
	 * \public
	 * \return A reference on the game event. 
	 */
	sf::Event& getEvent();
};

#endif // !INCLUDE_GAME

