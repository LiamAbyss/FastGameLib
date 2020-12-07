#include "Game.h"

class TestClickable : public Clickable
{
public:
	sf::RectangleShape r;

	void onClick()
	{
		std::cout << "ok" << std::endl;
	}
};

class TestScene : public Scene
{
	TestClickable c;

	void initialize() override
	{
		sf::RectangleShape rect;

		// Start with a red square
		rect.setFillColor(sf::Color::Red);
		rect.setSize(sf::Vector2f(100, 100));
		getWindow().clear();
		getWindow().draw(rect);
		getWindow().display();
		sf::sleep(sf::seconds(1));

		getResources().load("music", "../ExampleAssets/Gameplay.ogg");

		// Change the color of our square when our assets are loaded
		rect.setFillColor(sf::Color::Green);
		getWindow().clear();
		getWindow().draw(rect);
		getWindow().display();
		sf::sleep(sf::seconds(1));

		// Don't forget to bind your clickable to the window and the event
		c.setWindow(&getWindow());
		c.setEvent(&getGame().getEvent());

		std::vector<MouseButton> buttonsToReactTo;
		
		// On middle and right click, we will call c.onClick()
		buttonsToReactTo.push_back(MouseButton::MIDDLE);
		buttonsToReactTo.push_back(MouseButton::RIGHT);
		c.getMouseButtons() = buttonsToReactTo;

		// Prepare manually a hitbox but never do it like that
		// You should load it from a json file using Hitbox::getHitboxes()
		c.getHitboxes().emplace("test", std::vector<Hitbox>());
		c.getHitboxes()["test"].push_back(Hitbox());
		c.setActiveHitbox("test");
		Hitbox& cHitbox = c.getHitboxes()["test"][0];
		cHitbox.setSize(sf::Vector2f(300, 100));

		// Scale it because why not
		cHitbox.setScale(sf::Vector2f(0.5, 2));

		// Rotate it for fun
		cHitbox.rotate(-20);

		c.r.setSize(cHitbox.getSize());
		c.r.setFillColor(sf::Color::Red);
		c.r.setRotation(cHitbox.getAngle());
	}

	void update(sf::Time dt, sf::Event& ev) override
	{
		// On space, restart the music
		if(ev.type == sf::Event::KeyReleased && ev.key.code == sf::Keyboard::Space)
		{
			GMusic m = boost::get<GMusic>(getResources().get("music"));
			m->setPlayingOffset(sf::seconds(0));
			m->play();
		}

		// On click
		if (c.isClicked()) c.onClick();
	}

	void update(sf::Time dt) override
	{
		//Does nothing for this example
	}

	void render() override
	{
		getWindow().draw(c.r);

		// Render hitboxes for debug
		c.renderHitboxes(getWindow());
	}
};

int main()
{
	// Create the game instance
	Game game("test", sf::VideoMode(800, 600));

	// Add our scene...
	game.addScene("first", new TestScene());

	// ...and set the game to begin with that scene
	game.setCurrentScene("first");

	// Launch the game
	game.launch();
	return 0;
}