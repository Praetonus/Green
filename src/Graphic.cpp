/************************************************************************
 * Copyright 2014 Benoît Vey                                            *
 *                                                                      *
 * This file is part of Green.                                          *
 *                                                                      *
 * Green is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * Green is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Green.  If not, see <http://www.gnu.org/licenses/>.       *
 ************************************************************************/

#include "Graphic.hpp"

extern int const c_maxDistance;

Graphic::Graphic() : m_circles{sf::CircleShape{300, 60}, sf::CircleShape{200, 60}, sf::CircleShape{100, 60}}, m_font{}, m_distances()
{
	m_circles[0].setPosition(10, 10);
	m_circles[1].setPosition(110, 110);
	m_circles[2].setPosition(210, 210);

	for (auto& elem : m_circles)
	{
		elem.setFillColor(sf::Color::Transparent);
		elem.setOutlineColor(sf::Color::White);
		elem.setOutlineThickness(1);
	}

	m_font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");
	changeScale(c_maxDistance);
}

void Graphic::changeScale(int scl)
{
	static auto intToText(
	[this](int nmbr)->sf::Text
	{
		return (sf::Text(std::to_string(nmbr) + "cm", m_font, 12));
	});

	m_distances = {intToText(scl), intToText(scl * 2 / 3), intToText(scl / 3)};
	m_distances[0].setPosition(300, 10);
	m_distances[1].setPosition(300, 110);
	m_distances[2].setPosition(300, 210);
}

void Graphic::draw(sf::RenderTarget& target, sf::RenderStates /*states*/) const
{
	for (auto& elem : m_circles)
		target.draw(elem);

	for (auto& elem : m_distances)
		target.draw(elem);
}
