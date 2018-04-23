/**
 *
 * file: utils/numeric.hpp
 * defines some helper math functions.
 */
#ifndef _NUMERIC_HPP_
#define _NUMERIC_HPP_

#include <assert.h>
#include <algorithm>

namespace MC {

template <typename T>
T sqr(T const& x) {
	return x * x;
}

template <typename T>
T abs(T const& x) {
	return x > 0 ? x : -x;
}

/*** Numeric integration functions ***/
template <typename ForwardIt>
auto trapz(ForwardIt z0, ForwardIt zN, ForwardIt f0, ForwardIt fN)
    -> decltype(*z0 * *f0) {
	auto Nz = std::distance(z0, zN);
	auto Nf = std::distance(f0, fN);
	assert(Nz == Nf &&
	       "z-vector length != f-vector length. Cannot integrate.");
	assert(Nz > 3 && "grid points too little. Cannot integrate.");

	using T = decltype(*z0 * *f0);
	T integral{0};
	for (auto i = 1; i < Nz; ++i)
		integral += (f0[i] + f0[i - 1]) * (z0[i] - z0[i - 1]) / 2.0;
	return integral;
}

template <typename ForwardIt_z, typename ForwardIt_f>
auto trapz(ForwardIt_z z0, ForwardIt_z zN, ForwardIt_f f0, ForwardIt_f fN)
    -> decltype(*z0 * *f0) {
	auto Nz = std::distance(z0, zN);
	auto Nf = std::distance(f0, fN);
	assert(Nz == Nf &&
	       "z-vector length != f-vector length. Cannot integrate.");
	assert(Nz > 3 && "grid points too little. Cannot integrate.");
	using T = decltype(*z0 * *f0);
	T integral{0};
	for (auto i = 1; i < Nz; ++i)
		integral += (f0[i] + f0[i - 1]) * (z0[i] - z0[i - 1]) / 2.0;
	return integral;
}

};  // namespace MC

#endif
