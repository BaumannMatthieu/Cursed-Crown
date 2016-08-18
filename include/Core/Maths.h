#pragma once

#define PI 3.14

static float norme(const sf::Vector2f& v) {
	return std::sqrt(v.x*v.x + v.y*v.y);
}

static float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
	return norme(p1 - p2);
}

static void normalize(sf::Vector2f& v) {
        v /= norme(v);
}

static sf::Vector2f cart(const sf::Vector2f& vect) {
    return sf::Vector2f(vect.x - vect.y,
                        0.5f*(vect.x + vect.y));
}

static sf::Vector2f iso(const sf::Vector2f& vect, const sf::Vector2f& origin) {
        return sf::Vector2f((vect.x - origin.x)/2.f + vect.y - origin.y,
                            vect.y - origin.y - (vect.x - origin.x)/2.f);
}
static float getAngle(const sf::Vector2f& u, const sf::Vector2f& v) {
	float s = u.x*v.y - u.y*v.x;
	
	float c = (u.x*v.x + u.y*v.y)/(norme(u)*norme(v));

	if(s < 0.f) {
		return -std::acos(c) + 2*PI;
	}

	return std::acos(c);
}



