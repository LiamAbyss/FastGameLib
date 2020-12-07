#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <map>
#include <fstream>
#include <limits>
#include <cmath>
#include <iostream>
#include "D:/CppLibs/nlohmann/json.hpp"
constexpr auto M_PI = 3.14159265358979323846;
class Hitbox;

/** 
 * A virtual class of sf::Sprite with hitboxes
 */
class Hitboxed
{
private:

	/** 
	 * \private
	 * A map containing all the hitboxes of the object 
	 */
	std::map<std::string, std::vector<Hitbox>> hitboxes;

	/** 
	 * \private
	 * The label of the current hitbox
	 */
	std::string activeHitbox;

public:

	/**
	 * \public
	 * \return A reference on the hitboxes of the object
	 */
	std::map<std::string, std::vector<Hitbox>>& getHitboxes();

	/**
	 * \public
	 * Set the hitboxes of the object
	 * \param newHitboxes The hitboxes
	 * \note To load hitboxes, use Hitbox::getHitboxes()
	 */
	void setHitboxes(const std::map<std::string, std::vector<Hitbox>>& newHitboxes);

	/**
	 * \public
	 * \return The current hitbox label
	 */
	std::string& getActiveHitboxName();

	/**
	 * \public
	 * \return A reference on the active Hitbox
	 */
	std::vector<Hitbox>& getActiveHitboxes();

	/**
	 * \public
	 * \param v The translation vector
	 */
	void moveHitboxes(const sf::Vector2f& v);

	/**
	 * \public
	 * \param pos The new position of the hitboxes 
	 */
	void setPosHitboxes(const sf::Vector2f& pos);

	/**
	 * \public
	 * \param target The target where to draw the hitboxes for debug (usually the game window)
	 */
	void renderHitboxes(sf::RenderTarget& target);

	/**
	 * \public
	 * \param degrees The new angle of the hitboxes in degrees
	 */
	void setAngleHitboxes(double degrees);

	/**
	 * \public
	 * \param degrees The rotation angle in degrees
	 */
	void rotateHitboxes(double degrees);

	/**
	 * \public
	 * \param label The label of the wanted current Hitbox
	 */
	void setActiveHitbox(const std::string& label);

	/**
	 * \public
	 * \param scale The new scale of the hitboxes
	 */
	void setScaleHitboxes(const sf::Vector2f& scale);

	/**
	 * \public
	 * \param sprite The sprite which is Hitboxed
	 * \param state Whether the hitboxes must be flipped or not
	 */
	void flipHitboxes(const sf::Sprite * sprite, const bool state);

	/**
	 * \public
	 * \return true If the hitboxes are flipped
	 * \return false Otherwise
	 */
	bool areHitboxesFlipped();
};

/** 
 * Hitboxes to manage collisions
 */
class Hitbox
{
public:

	/**
	 * \public
	 * \param sprite The sprite which is Hitboxed
	 * \param state Whether the hitboxes must be flipped or not
	 */
	void flip(const sf::Sprite* sprite, bool state = true);

	/**
	 * \public
	 * \param filename The name of the json file containing the hitboxes
	 * \param name The name of the entity where to search the hitboxes in the file
	 * \return A map of hitboxes
	 */
	static std::map<std::string, std::vector<Hitbox>> getHitboxes(const std::string& filename, const std::string& name);

	/**
	 * \public
	 * \param j A json object
	 * \param name The name of the entity where to search the hitboxes in the file
	 * \return A map of hitboxes
	 */
	static std::map<std::string, std::vector<Hitbox>> getHitboxes(const nlohmann::json& j, const std::string& name);

	/**
	 * \public
	 * \param a The first hitbox 
	 * \param b The second hitbox
	 * \return true If a is colliding with b
	 * \return false Otherwise
	 */
	static bool isOver(const Hitbox& a, const Hitbox& b);

	/**
	 * \public
	 * \param a The first Hitboxed object
	 * \param b The second Hitboxed object
	 * \return true If a is hitting b
	 * \return false Otherwise
	 * \note This method is only checking for a's hurtboxes and b's hitboxes
	 */
	static bool doesHit(Hitboxed* a, Hitboxed* b);

	/**
	 * \public
	 * \param a The first Hitboxed object
	 * \param b The second Hitboxed object
	 * \return true If any Hitbox of a is colliding with any Hitbox of b
	 * \return false Otherwise
	 */
	static bool isOver(Hitboxed* a, Hitboxed* b);

	/**
	 * \public
	 * \param point The point to check
	 * \param hitbox The Hitbox to check
	 * \return true If point is in Hitbox
	 * \return false Otherwise
	 */
	static bool isPointInHitbox(const sf::Vector2f & point, const Hitbox & hitbox);


	/**
	 * \public
	 * \param point The point to check
	 * \param hitboxed The Hitboxed to check
	 * \return true If point is in Hitboxed
	 * \return false Otherwise
	 */
	static bool isPointInHitbox(const sf::Vector2f & point, Hitboxed * hitboxed);

private:

	/** 
	 * \private
	 * The label of the Hitbox 
	 */
	std::string label;

	/** 
	 * \private
	 * The type of the Hitbox
	 * \note Currently "hitbox" or "hurtbox"
	 */
	std::string type = "hitbox";

	/** 
	 * \private
	 * The frame of the animation where to activate the Hitbox
	 */
	int start = 0;

	/** 
	 * \private
	 * The frame of the animation where to deactivate the Hitbox
	 */
	int end = 0;

	/** 
	 * \private
	 * The position of the Hitbox relatively to the anchor
	 */
	sf::Vector2f pos;

	/** 
	 * \private
	 * The position of the Hitbox relatively to the window
	 */
	sf::Vector2f anchor;

	/** 
	 * \private
	 * The size of the Hitbox
	 */
	sf::Vector2f size;

	/** 
	 * \private
	 * The x coordinate for the left
	 */
	float left = 0.F;

	/**
	 * \private
	 * The x coordinate for the right
	 */
	float right = 0.F;

	/**
	 * \private
	 * The y coordinate for the top
	 */
	float top = 0.F;

	/**
	 * \private
	 * The y coordinate for the bottom
	 */
	float bottom = 0.F;

	/** 
	 * \private
	 * Whether the Hitbox is flipped or not
	 */
	bool flipped = false;

	/** 
	 * \private
	 * The angle of the Hitbox in degrees
	 */
	double angle = 0.;

	/** 
	 * \private
	 * The scale of the Hitbox
	 */
	sf::Vector2f scale = sf::Vector2f(1, 1);

public:

	/**
	 * \public
	 * \return The scale of the Hitbox
	 */
	sf::Vector2f getScale() const;

	/**
	 * \public
	 * \param hitboxScale The new scale of the Hitbox
	 */
	void setScale(const sf::Vector2f& hitboxScale);

	/**
	 * \public
	 * \return The position relatively to the anchor 
	 */
    sf::Vector2f getLocalPos() const;

	/**
	 * \public
	 * \return The position relatively to the window 
	 */
    sf::Vector2f getGlobalPos() const;

	/**
	 * \public
	 * \param hitboxPos The new global position of the Hitbox
	 */
    void setPos(const sf::Vector2f& hitboxPos);

	/**
	 * \public
	 * \param x The new global x coordinate 
	 */
	void setX(const float& x);

	/**
	 * \public
	 * \param y The new global y coordinate
	 */
	void setY(const float& y);

	/**
	 * \public
	 * \return The label of the Hitbox 
	 */
    std::string getLabel() const;

	/**
	 * \public
	 * \param hitboxType The type of the Hitbox
	 * \note Currently "hitbox" or "hurtbox"
	 */
	void setType(const std::string& hitboxType);

	/**
	 * \public
	 * \return The type of the Hitbox
	 * \note Currently "hitbox" or "hurtbox"
	 */
    std::string getType() const;

	/**
	 * \public
	 * \return The start frame
	 */
    int getStart() const;

	/**
	 * \public
	 * \return The end frame
	 */
    int getEnd() const;

	/**
	 * \public
	 * \param hitboxSize The new size of the Hitbox
	 * \note The scale will always be applied
	 */
	void setSize(const sf::Vector2f& hitboxSize);

	/**
	 * \public
	 * \return The size of the Hitbox 
	 */
    sf::Vector2f getSize() const;

	/**
	 * \public
	 * \return The x coordinate for the left 
	 */
    float getLeft() const;

	/**
	 * \public
	 * \return The x coordinate for the right
	 */
    float getRight() const;

	/**
	 * \public
	 * \return The y coordinate for the bottom
	 */
    float getBottom() const;

	/**
	 * \public
	 * \return The y coordinate for the top
	 */
    float getTop() const;

	/**
	 * \public
	 * @return true If the Hitbox is flipped
	 * @return false Otherwise
	 */
    bool getFlipped() const;

	/**
	 * \public
	 * \return The anchor of the Hitbox 
	 */
    sf::Vector2f getAnchor() const;

	/**
	 * \public
	 * \return The angle of the Hitbox in degrees
	 */
	double getAngle() const;

	/**
	 * \public
	 * \param degrees The new angle of the Hitbox in degrees
	 */
    void setAngle(double degrees);

	/**
	 * \public
	 * \param degrees The angle of rotation of the Hitbox in degrees
	 */
	void rotate(double degrees);
};

