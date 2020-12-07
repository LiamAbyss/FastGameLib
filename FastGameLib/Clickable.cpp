#include "Clickable.h"

bool Clickable::isClicked()
{
	if (!isHovered())  return false;
	bool res = false;
	if ((std::find(mouseButtons.begin(), mouseButtons.end(), MouseButton::LEFT) != mouseButtons.end()
		&& ev->mouseButton.button == sf::Mouse::Left)
		|| (std::find(mouseButtons.begin(), mouseButtons.end(), MouseButton::RIGHT) != mouseButtons.end()
			&& ev->mouseButton.button == sf::Mouse::Right)
		|| (std::find(mouseButtons.begin(), mouseButtons.end(), MouseButton::MIDDLE) != mouseButtons.end()
			&& ev->mouseButton.button == sf::Mouse::Middle))
	{
		if (ev->type == sf::Event::MouseButtonReleased
			&& std::find(clickTypes.begin(), clickTypes.end(), ClickType::ONRELEASE) != clickTypes.end())
		{
			res = true;
		}
		else if (ev->type == sf::Event::MouseButtonPressed
			&& std::find(clickTypes.begin(), clickTypes.end(), ClickType::ONDOWN) != clickTypes.end())
		{
			res = true;
		}
	}
	return res;
}

bool Clickable::isHovered()
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	/*CursorHitbox tmp;
	tmp.hitboxes.emplace("tmp", std::vector<Hitbox>());
	tmp.hitboxes["tmp"].push_back(Hitbox());
	tmp.setActiveHitbox("tmp");
	Hitbox& tmpHitbox = tmp.hitboxes["tmp"][0];
	tmpHitbox.setPos(sf::Vector2f(static_cast<float>(mousePos.x - 1), static_cast<float>(mousePos.y - 1)));
	tmpHitbox.setSize(sf::Vector2f(2.F, 2.F));
	tmpHitbox.setType("hurtbox");*/

	return Hitbox::isPointInHitbox(static_cast<sf::Vector2f>(mousePos), this);
}

void Clickable::setWindow(sf::RenderWindow * gameWindow)
{
	window = gameWindow;
}

sf::RenderWindow& Clickable::getWindow()
{
	return static_cast<sf::RenderWindow&>(*window);
}

void Clickable::setEvent(sf::Event * gameEvent)
{
	ev = gameEvent;
}

std::vector<ClickType>& Clickable::getClickTypes()
{
	return clickTypes;
}

std::vector<MouseButton>& Clickable::getMouseButtons()
{
	return mouseButtons;
}
