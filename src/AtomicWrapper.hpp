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

#ifndef ATOMICWRAPPER_HPP_INC
#define ATOMICWRAPPER_HPP_INC

#include <atomic>

template <typename T>
struct AtomicWrapper
{
	AtomicWrapper() : handled{}
	{}
	AtomicWrapper(std::atomic<T> const& atom) : handled{atom.load()}
	{}
	
	AtomicWrapper(AtomicWrapper<T> const& other) : handled{other.handled.load()}
	{}
	AtomicWrapper<T>& operator=(AtomicWrapper<T> const& other)
	{
		handled.store(other.handled.load());
		return *this;
	}

	~AtomicWrapper() = default;

	std::atomic<T> handled;
};

#endif
