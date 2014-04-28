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

#ifndef GRAPHIC_HPP_INC
#define GRAPHIC_HPP_INC

#include <SFML/Graphics.hpp>

class Graphic : public sf::Drawable, public sf::Transformable
{
	public:
	Graphic();

	Graphic(Graphic const&) = delete;
	Graphic& operator=(Graphic const&) = delete;
	
	Graphic(Graphic&&) = default;
	Graphic& operator=(Graphic&&) = default;

	virtual ~Graphic() = default;

	void changeScale(int);

	private:
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	std::array<sf::CircleShape, 3> m_circles;

	sf::Font m_font;
	std::array<sf::Text, 3> m_distances;
};

#endif
