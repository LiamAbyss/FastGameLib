#ifndef INCLUDE_SCENE
#define INCLUDE_SCENE
#include "Game.h"

class Game;

/** 
 * A pure virtual class of a game scene.
 */
class Scene
{
private:

	/** 
	 * \private
	 * A pointer to the game.
	 * \see Game
	 */
	Game* game;

	/** 
	 * \private
	 * A pointer to the resource manager.
	 * \see ResourceManager
	 */
	ResourceManager* resources;

	/** 
	 * \private
	 * A pointer to the main window.
	 */
	sf::RenderWindow* window;

	/** 
	 * \private
	 * Whether the scene has been initialized or not
	 */
	bool initialized = false;

public:

	/**
	 * \public
	 * Used in Game::addScene() to initialize the protected attributes.
	 * \param game A pointer to the game.
	 * \see Game
	 * \note You should not call this method yourself.
	 */
	void setGame(Game& globalGame);

	virtual ~Scene() = default;
	
	/**
	 * \public
	 * \return A reference to the game
	 * \see Game 
	 */
	Game& getGame();

	/**
	 * \public
	 * \return A reference on the resources
	 * \see ResourceManager
	 */
	ResourceManager& getResources();
	
	/**
	 * \public
	 * \return A reference on the game window 
	 */
	sf::RenderWindow& getWindow();

	/**
	 * \public
	 * Choose whether the scene must be used as if it were already initialized or not.
	 * \param init A boolean
	 * \note The game will always call initialize() when changing scenes, it is up to you to use hasBeenInitialized() to choose what you want to do only once inside initialize()
	 * \see Scene.initialize
	 */
	void setInitialized(bool init);

	/**
	 * \public
	 * \return true If the scene has been initialized
	 * \return false Otherwise
	 */
	bool hasBeenInitialized() const;

	/** 
	 * \public
	 * Initializer to load resources, or whatever you want etc...
	 * \note You shouldn't do any rendering inside the initialize method
	 */
	virtual void initialize() = 0;

	/**
	 * \public
	 * Game loop in which all the processing is done.
	 * \param dt The time passed since last update.
	 * \param ev The SFML event for the main window.
	 * \note This function triggers only if there is an event, such as a key press or a mouse click
	 */
	virtual void update(sf::Time dt, sf::Event& ev) = 0;

	/**
	 * \public
	 * Game loop in which all the processing is done.
	 * \param dt The time passed since last update.
	 * \param ev The SFML event for the main window.
	 * \note This method will be called after its overloaded sister when there is an event
	 */
	virtual void update(sf::Time dt) = 0;

	/** 
	 * \public
	 * Game loop in which all the rendering is done.
	 */
	virtual void render() = 0;
};

#endif // !INCLUDE_SCENE

