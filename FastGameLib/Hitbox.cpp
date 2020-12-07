#include "Hitbox.h"

namespace json = nlohmann;
using namespace std;

void Hitbox::flip(const sf::Sprite* sprite, bool state)
{
	if (flipped == state) return;
	pos.x = sprite->getGlobalBounds().width - pos.x - size.x;
	flipped = !flipped;
}

std::map<std::string, std::vector<Hitbox>> Hitbox::getHitboxes(const std::string& filename, const std::string& name)
{
	json::json j;
	std::ifstream file(filename);

	file >> j;
	file.close();
	return getHitboxes(j, name);
}

std::map<std::string, std::vector<Hitbox>> Hitbox::getHitboxes(const nlohmann::json& j, const std::string& name)
{
	std::map<std::string, std::vector<Hitbox>> hitboxes;

	for(const auto& f : j[name].items())
	{
		if (f.key() != "active")
		{
			hitboxes.emplace(f.key(), std::vector<Hitbox>());
			for (auto& g : f.value())
			{
				hitboxes[f.key()].push_back(Hitbox());
				auto& hitbox = hitboxes[f.key()][hitboxes[f.key()].size() - 1];
				hitbox.label = g["label"];
				hitbox.pos = sf::Vector2f(stof(g["x"].dump()), stof(g["y"].dump()));
				hitbox.size = sf::Vector2f(stof(g["width"].dump()), stof(g["height"].dump()));
				hitbox.type = g["type"];
				hitbox.start = stoi(g["start"].dump());
				hitbox.end = stoi(g["end"].dump());
				hitbox.anchor = sf::Vector2f(0, 0);
				hitbox.right = hitbox.anchor.x + hitbox.pos.x + hitbox.size.x;
				hitbox.left = hitbox.anchor.x + hitbox.pos.x;
				hitbox.top = hitbox.anchor.y + hitbox.pos.y;
				hitbox.bottom = hitbox.anchor.y + hitbox.pos.y + hitbox.size.y;
				hitbox.flipped = false;
				hitbox.angle = stof(g["angle"].dump());
				hitbox.scale = sf::Vector2f(1, 1);
			}
		}
	}

	return hitboxes;
}

// Given three colinear points p, q, r, the function checks if 
// point q lies on line segment 'pr' 
bool onSegment(const sf::Vector2f& p, const sf::Vector2f& q, const sf::Vector2f& r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(const sf::Vector2f& p, const sf::Vector2f& q, const sf::Vector2f& r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
	// for details of below formula. 
	auto val = static_cast<int>(
		(q.y - p.y) * (r.x - q.x)
		- (q.x - p.x) * (r.y - q.y));

	if (val == 0) return 0;  // colinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(sf::Vector2f p1, sf::Vector2f q1, sf::Vector2f p2, sf::Vector2f q2)
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases 
}

bool Hitbox::isOver(const Hitbox& a, const Hitbox& b)
{
	double c = 2 * M_PI / 360;

	vector<sf::Vector2f> av;
	av.push_back(sf::Vector2f(a.left, a.top));
	av.push_back(sf::Vector2f(
		a.left + a.size.x * static_cast<float>(cos(a.angle*c)), 
		a.top + a.size.x * static_cast<float>(sin(a.angle * c)))
	);
	av.push_back(sf::Vector2f(
		a.left + a.size.x * static_cast<float>(cos(a.angle * c)) - a.size.y * static_cast<float>(sin(a.angle * c)), 
		a.top + a.size.x * static_cast<float>(sin(a.angle * c)) + a.size.y * static_cast<float>(cos(a.angle * c)))
	);
	av.push_back(sf::Vector2f(
		a.left - a.size.y * static_cast<float>(sin(a.angle * c)), 
		a.top + a.size.y * static_cast<float>(cos(a.angle * c)))
	);
	av.push_back(sf::Vector2f(a.left, a.top));

	vector<sf::Vector2f> bv;
	bv.push_back(sf::Vector2f(b.left, b.top));
	bv.push_back(sf::Vector2f(
		b.left + b.size.x * static_cast<float>(cos(b.angle * c)), 
		b.top + b.size.x * static_cast<float>(sin(b.angle * c)))
	);
	bv.push_back(sf::Vector2f(
		b.left + b.size.x * static_cast<float>(cos(b.angle * c)) - b.size.y * static_cast<float>(sin(b.angle * c)), 
		b.top + b.size.x * static_cast<float>(sin(b.angle * c)) + b.size.y * static_cast<float>(cos(b.angle * c)))
	);
	bv.push_back(sf::Vector2f(
		b.left - b.size.y * static_cast<float>(sin(b.angle * c)),
		b.top + b.size.y * static_cast<float>(cos(b.angle * c)))
	);
	bv.push_back(sf::Vector2f(b.left, b.top));

	
	for(size_t i = 0; i < av.size() - 1; i++)
	{
		for (size_t j = 0; j < bv.size() - 1; j++)
		{
			if (doIntersect(av[i], av[i + 1], bv[j], bv[j + 1]))
				return true;
		}
	}

	//Point in polygon algorithm
	for (int k = 0; k < av.size() - 1; k++)
	{
		int nvert = static_cast<int>(bv.size()) - 1;
		bool res = false;
		int j = nvert - 1;
		for (int i = 0; i < nvert; i++) {
			if (((bv[i].y > av[k].y) != (bv[j].y > av[k].y)) &&
				(av[k].x < (bv[j].x - bv[i].x) * (av[k].y - bv[i].y) / (bv[j].y - bv[i].y) + bv[i].x))
				res = !res;
			j = i;
		}
		if(res) return true;
	}
	for (int k = 0; k < bv.size() - 1; k++)
	{
		int nvert = static_cast<int>(av.size()) - 1;
		bool res = false; 
		int j = nvert - 1;
		for (int i = 0; i < nvert; i++) {
			if (((av[i].y > bv[k].y) != (av[j].y > bv[k].y)) &&
				(bv[k].x < (av[j].x - av[i].x) * (bv[k].y - av[i].y) / (av[j].y - av[i].y) + av[i].x))
				res = !res;
			j = i;
		}
		if (res) return true;
	}
	return false;
}

bool Hitbox::doesHit(Hitboxed * a, Hitboxed * b)
{
	for(const Hitbox& hurtbox : a->getActiveHitboxes())
	{
		if (hurtbox.type != "hurtbox") continue;
		
		for(const Hitbox& hitbox : b->getActiveHitboxes())
		{
			if(hitbox.type == "hitbox" && isOver(hurtbox, hitbox))
			{
				return true;
			}
		}
	}
	return false;
}

bool Hitbox::isOver(Hitboxed* a, Hitboxed* b)
{
	for (const Hitbox& hurtbox : a->getActiveHitboxes())
	{
		for (const Hitbox& hitbox : b->getActiveHitboxes())
		{
			if (isOver(hurtbox, hitbox))
			{
				return true;
			}
		}
	}
	return false;
}

bool Hitbox::isPointInHitbox(const sf::Vector2f & point, const Hitbox & hitbox)
{
	double c = 2 * M_PI / 360;

	vector<sf::Vector2f> bv;
	bv.push_back(sf::Vector2f(hitbox.left, hitbox.top));
	bv.push_back(sf::Vector2f(
		hitbox.left + hitbox.size.x * static_cast<float>(cos(hitbox.angle * c)),
		hitbox.top + hitbox.size.x * static_cast<float>(sin(hitbox.angle * c)))
	);
	bv.push_back(sf::Vector2f(
		hitbox.left + hitbox.size.x * static_cast<float>(cos(hitbox.angle * c)) - hitbox.size.y * static_cast<float>(sin(hitbox.angle * c)),
		hitbox.top + hitbox.size.x * static_cast<float>(sin(hitbox.angle * c)) + hitbox.size.y * static_cast<float>(cos(hitbox.angle * c)))
	);
	bv.push_back(sf::Vector2f(
		hitbox.left - hitbox.size.y * static_cast<float>(sin(hitbox.angle * c)),
		hitbox.top + hitbox.size.y * static_cast<float>(cos(hitbox.angle * c)))
	);
	bv.push_back(sf::Vector2f(hitbox.left, hitbox.top));

	int nvert = static_cast<int>(bv.size()) - 1;
	bool res = false;
	int j = nvert - 1;
	for (int i = 0; i < nvert; i++) {
		if (((bv[i].y > point.y) != (bv[j].y > point.y)) &&
			(point.x < (bv[j].x - bv[i].x) * (point.y - bv[i].y) / (bv[j].y - bv[i].y) + bv[i].x))
			res = !res;
		j = i;
	}
	return res;
}

bool Hitbox::isPointInHitbox(const sf::Vector2f & point, Hitboxed * hitboxed)
{
	for (const Hitbox& h : hitboxed->getActiveHitboxes())
	{
		if (isPointInHitbox(point, h))
		{
			return true;
		}
	}
	return false;
}

sf::Vector2f Hitbox::getScale() const
{
	return scale;
}

void Hitbox::setScale(const sf::Vector2f& hitboxScale)
{
	pos.x = pos.x / scale.x * hitboxScale.x;
	pos.y = pos.y / scale.y * hitboxScale.y;
	size.x = size.x / scale.x * hitboxScale.x;
	size.y = size.y / scale.y * hitboxScale.y;
	scale = hitboxScale;
	right = anchor.x + pos.x + size.x;
	left = anchor.x + pos.x;
	top = anchor.y + pos.y;
	bottom = anchor.y + pos.y + size.y;
}

sf::Vector2f Hitbox::getLocalPos() const
{
    return pos;
}

sf::Vector2f Hitbox::getGlobalPos() const
{
	return (anchor + pos);
}

void Hitbox::setPos(const sf::Vector2f& hitboxPos)
{
	setX(hitboxPos.x);
	setY(hitboxPos.y);
}

void Hitbox::setX(const float& x)
{
	anchor.x = x;
	right = anchor.x + pos.x + size.x;
	left = anchor.x + pos.x;
}

void Hitbox::setY(const float& y)
{
	anchor.y = y;
	bottom = anchor.y + pos.y + size.y;
	top = anchor.y + pos.y;
}

std::string Hitbox::getLabel() const
{
    return label;
}

void Hitbox::setType(const std::string& hitboxType)
{
	type = hitboxType;
}

std::string Hitbox::getType() const
{
    return type;
}

int Hitbox::getStart() const
{
    return start;
}

int Hitbox::getEnd() const
{
    return end;
}

void Hitbox::setSize(const sf::Vector2f& hitboxSize)
{
	size = hitboxSize;
	bottom = anchor.y + pos.y + size.y;
	top = anchor.y + pos.y;
	right = anchor.x + pos.x + size.x;
	left = anchor.x + pos.x;
}

sf::Vector2f Hitbox::getSize() const
{
    return size;
}

float Hitbox::getLeft() const
{
    return left;
}

float Hitbox::getRight() const
{
    return right;
}

float Hitbox::getBottom() const
{
    return bottom;
}

float Hitbox::getTop() const
{
    return top;
}

bool Hitbox::getFlipped() const
{
    return flipped;
}

std::map<std::string, std::vector<Hitbox>>& Hitboxed::getHitboxes()
{
	return static_cast<std::map<std::string, std::vector<Hitbox>>&>(hitboxes);
}

void Hitboxed::setHitboxes(const std::map<std::string, std::vector<Hitbox>>& newHitboxes)
{
	hitboxes = newHitboxes;
}

std::string& Hitboxed::getActiveHitboxName()
{
	return static_cast<std::string&>(activeHitbox);
}

std::vector<Hitbox>& Hitboxed::getActiveHitboxes()
{
	return static_cast<std::vector<Hitbox>&>(hitboxes[activeHitbox]);
}

void Hitboxed::moveHitboxes(const sf::Vector2f& v)
{
	for (auto& f : hitboxes)
	{
		for (Hitbox& hitbox : f.second)
		{
			hitbox.setPos(hitbox.getAnchor() + v);
		}
	}
}

void Hitboxed::setPosHitboxes(const sf::Vector2f& pos)
{
	for (auto& f : hitboxes)
	{
		for (Hitbox& hitbox : f.second)
		{
			hitbox.setPos(pos);
		}
	}
}

void Hitboxed::renderHitboxes(sf::RenderTarget& target)
{
	for (const auto& hitbox : hitboxes[activeHitbox])
	{
		sf::RectangleShape rect;
		rect.setFillColor((hitbox.getType() == "hitbox" ? sf::Color(0,0,255,150) : sf::Color(255,0,0,150)));
		rect.setPosition(hitbox.getGlobalPos());
		rect.setSize(hitbox.getSize());
		rect.rotate(static_cast<float>(hitbox.getAngle()));
		target.draw(rect);
	}
}

void Hitboxed::setAngleHitboxes(double degrees)
{
	for (auto& f : hitboxes)
	{
		for (Hitbox& hitbox : f.second)
		{
			hitbox.setAngle(degrees);
		}
	}
}

void Hitboxed::rotateHitboxes(double degrees)
{
	for (auto& f : hitboxes)
	{
		for (Hitbox& hitbox : f.second)
		{
			hitbox.setAngle(hitbox.getAngle() + degrees);
		}
	}
}

void Hitboxed::setActiveHitbox(const std::string& label)
{
	activeHitbox = label;
}

void Hitboxed::setScaleHitboxes(const sf::Vector2f& scale)
{
	for (auto& f : hitboxes)
	{
		for (Hitbox& hitbox : f.second)
		{
			hitbox.setScale(scale);
		}
	}
}

void Hitboxed::flipHitboxes(const sf::Sprite * sprite, const bool state = true)
{
	for (auto& f : hitboxes)
	{
		for (Hitbox& hitbox : f.second)
		{
			if (hitbox.getFlipped() == state) return;
			hitbox.flip(sprite, state);
		}
	}
}

bool Hitboxed::areHitboxesFlipped()
{
	if (hitboxes[activeHitbox].size()) return hitboxes[activeHitbox][0].getFlipped();
	return false;
}

sf::Vector2f Hitbox::getAnchor() const
{
    return anchor;
}


double Hitbox::getAngle() const
{
    return angle;
}

void Hitbox::setAngle(double degrees)
{
    this->angle = degrees;
}

void Hitbox::rotate(double degrees)
{
	angle = getAngle() + degrees;
}
