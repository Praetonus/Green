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

#include "AtomicWrapper.hpp"
#include "Graphic.hpp"

#include <boost/asio.hpp>
#include <SFML/Window.hpp>

#include <chrono>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

using lockGuard = std::lock_guard<std::mutex>;

extern int constexpr c_maxDistance{500};
int constexpr c_distanceStep{10};

namespace
{
	std::array<AtomicWrapper<int>, 36> g_distances{std::atomic<int>{0}};
	std::atomic<bool> g_stop{false};
	std::atomic<bool> g_pause{false};

	std::exception_ptr g_exPtr{};
	std::mutex g_lock{};
}

void arduinoInterface(boost::asio::serial_port&);
void uiHandling();

int main()
{
	try
	{
		std::cout << "Path to the serial port : " << std::endl;
		std::string path;
		std::cin >> path;

		boost::asio::io_service ioserv{};
		boost::asio::serial_port ard{ioserv, path};
		ard.set_option(boost::asio::serial_port::baud_rate(9600));
		
		std::thread arduinoThread{arduinoInterface, std::ref(ard)};
		arduinoThread.detach();
		uiHandling();
	}
	catch (std::exception const& e)
	{
		std::cerr << "Exception caught, terminating." << std::endl << e.what() << std::endl;
		return 1;
	}
	return 0;
}

void arduinoInterface(boost::asio::serial_port& ard)
{
	try
	{
		boost::asio::streambuf buf{};
		std::string strDistance{};
		std::size_t i{0};
		boost::asio::write(ard, boost::asio::buffer(std::string{"b"}));
		while (!g_stop.load())
		{
			if (g_pause.load())
			{
				boost::asio::write(ard, boost::asio::buffer(std::string{"e"}));
				while (g_pause.load())
					std::this_thread::sleep_for(std::chrono::milliseconds{16});
				boost::asio::write(ard, boost::asio::buffer(std::string{"b"}));
			}
			boost::asio::read_until(ard, buf, '\n');
			std::istream is{&buf};
			std::getline(is, strDistance);
			g_distances[i].handled.store(std::stoi(strDistance));
			++i;
			if (i == 36)
				i = 0;
		}
		boost::asio::write(ard, boost::asio::buffer(std::string{"e"}));
	}
	catch (std::exception const& e)
	{
		lockGuard lock{g_lock};
		g_exPtr = std::current_exception();
	}
}

void uiHandling()
{
	using std::chrono::milliseconds;
	using std::chrono::steady_clock;
	using std::chrono::duration_cast;
	
	sf::RenderWindow win{sf::VideoMode{620, 620}, "Projet SI"};
	sf::RectangleShape graphDist{sf::Vector2f{1, 1}};
	graphDist.setFillColor(sf::Color::Green);
	graphDist.setPosition(310, 310);
	Graphic graphic{};
	int scale{c_maxDistance};
	while (1)
	{
		steady_clock::time_point beg{steady_clock::now()};
		{
			lockGuard lock{g_lock};
			if (g_exPtr != std::current_exception())
				std::rethrow_exception(g_exPtr);
		}
		sf::Event event;
		if (win.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					g_stop.store(true);
					return;
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
						case sf::Keyboard::Up:
							if (scale < c_maxDistance)
							{
								scale += c_distanceStep;
								graphic.changeScale(scale);
							}
							break;
						case sf::Keyboard::Down:
							if (scale > 10)
							{
								scale -= c_distanceStep;
								graphic.changeScale(scale);
							}
							break;
						case sf::Keyboard::Space:
							g_pause.store(!g_pause.load());
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
		win.clear();
		for (std::size_t i{0} ; i < 36 ; ++i)
		{
			int distance{g_distances[i].handled.load()};
			graphDist.setSize(sf::Vector2f(distance < scale ? distance * 300 / scale : 300, 1));
			graphDist.setRotation(i * 10);
			win.draw(graphDist);
		}
		win.draw(graphic);
		win.display();
		std::this_thread::sleep_for(milliseconds{16 - duration_cast<milliseconds>(steady_clock::now() - beg).count()});
	}
}
