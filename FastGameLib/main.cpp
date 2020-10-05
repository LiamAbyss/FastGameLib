#include "Game.h"

/*class TestClickable : public Clickable
{
	// Hérité via Clickable
	virtual void onClick() override
	{
		std::cout << "ok" << std::endl;
	}

public:
	sf::RectangleShape r;
};

class TestScene : public Scene
{
	TestClickable* c = new TestClickable();
	// Hérité via Scene
	virtual void initialize() override
	{
		sf::RectangleShape rect;
		rect.setFillColor(sf::Color::Red);
		rect.setSize(sf::Vector2f(100, 100));
		window->clear();
		window->draw(rect);
		window->display();
		resources->load("music", "../ExampleAssets/Gameplay.ogg");
		rect.setFillColor(sf::Color::Green);
		window->clear();
		window->draw(rect);
		window->display();

		game->addClickable(c);
		c->getMouseButtons().push_back(MouseButton::MIDDLE);
		c->hitboxes.emplace("test", std::vector<Hitbox>());
		c->hitboxes["test"].push_back(Hitbox());
		c->setActiveHitbox("test");
		Hitbox& cHitbox = c->hitboxes["test"][0];
		cHitbox.setSize(sf::Vector2f(300, 100));

		c->r.setSize(cHitbox.getSize());
		c->r.setFillColor(sf::Color::Red);
	}
	virtual void update(sf::Time dt, sf::Event& ev) override
	{
		if(ev.type == sf::Event::KeyReleased && ev.key.code == sf::Keyboard::Space)
		{
			GMusic m = boost::get<GMusic>(resources->get("music"));
			m->setPlayingOffset(sf::seconds(0));
			m->play();
		}
	}
	virtual void render() override
	{
		window->draw(c->r);
	}
};*/

int main()
{
	/*Game game("test", sf::VideoMode(800, 600));
	game.addScene("first", new TestScene());
	game.setCurrentScene("first");
	game.launch();*/
	return 0;
}