//
// Created by mashplant on 18-8-2.
//

#ifndef SPATIALDS_VEC_HPP
#define SPATIALDS_VEC_HPP

#include "Common.hpp"

struct Vec2
{
	Float x, y;

	static Vec2 identity()
	{ return {1, 1}; }

	template<typename RandFunc>
	static Vec2 rand(RandFunc rf)
	{ return {rf(), rf()}; }

	Float &operator[](int pos)
	{ return (&x)[pos]; }

	Float operator[](int pos) const
	{ return (&x)[pos]; }

	Vec2 &operator+=(const Vec2 &rhs)
	{ return x += rhs.x, y += rhs.y, *this; }

	friend Vec2 operator+(Vec2 lhs, const Vec2 &rhs)
	{ return lhs += rhs; }

	Vec2 &operator-=(const Vec2 &rhs)
	{ return x -= rhs.x, y -= rhs.y, *this; }

	friend Vec2 operator-(Vec2 lhs, const Vec2 &rhs)
	{ return lhs -= rhs; }

	Vec2 operator-() const
	{ return {-x, -y}; }

	Vec2 &operator*=(Float rhs)
	{ return x *= rhs, y *= rhs, *this; }

	friend Vec2 operator*(Vec2 lhs, Float rhs)
	{ return lhs *= rhs; }

	friend Vec2 operator*(Float lhs, Vec2 rhs)
	{ return rhs *= lhs; }

	Vec2 &operator/=(Float rhs)
	{ return x /= rhs, y /= rhs, *this; }

	friend Vec2 operator/(Vec2 lhs, Float rhs)
	{ return lhs /= rhs; }

	Float len2() const
	{ return x * x + y * y; }

	Float len() const
	{ return std::sqrt(x * x + y * y); }

	Float dis2(const Vec2 &rhs) const
	{ return (x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y); }

	Float dis(const Vec2 &rhs) const
	{ return std::sqrt(dis2(rhs)); }

	static Float dot(const Vec2 &lhs, const Vec2 &rhs)
	{ return lhs.x * rhs.x + lhs.y * rhs.y; }

	static Float cross(const Vec2 &lhs, const Vec2 &rhs)
	{ return lhs.x * rhs.y - lhs.y * rhs.x; }
};

struct Vec3
{
	Float x, y, z;

	static Vec3 identity()
	{ return {1, 1, 1}; }

	template<typename RandFunc>
	static Vec3 rand(RandFunc rf)
	{ return {rf(), rf(), rf()}; }

	Float &operator[](int pos)
	{ return (&x)[pos]; }

	Float operator[](int pos) const
	{ return (&x)[pos]; }

	Vec3 &operator+=(const Vec3 &rhs)
	{ return x += rhs.x, y += rhs.y, z += rhs.z, *this; }

	friend Vec3 operator+(Vec3 lhs, const Vec3 &rhs)
	{ return lhs += rhs; }

	Vec3 &operator-=(const Vec3 &rhs)
	{ return x -= rhs.x, y -= rhs.y, z -= rhs.z, *this; }

	friend Vec3 operator-(Vec3 lhs, const Vec3 &rhs)
	{ return lhs -= rhs; }

	Vec3 operator-() const
	{ return {-x, -y, -z}; }

	Vec3 &operator*=(Float rhs)
	{ return x *= rhs, y *= rhs, z *= rhs, *this; }

	friend Vec3 operator*(Vec3 lhs, Float rhs)
	{ return lhs *= rhs; }

	friend Vec3 operator*(Float lhs, Vec3 rhs)
	{ return rhs *= lhs; }

	Vec3 &operator/=(Float rhs)
	{ return x /= rhs, y /= rhs, z /= rhs, *this; }

	friend Vec3 operator/(Vec3 lhs, Float rhs)
	{ return lhs /= rhs; }

	Float len2() const
	{ return x * x + y * y + z * z; }

	Float len() const
	{ return std::sqrt(x * x + y * y + z * z); }

	Float dis2(const Vec3 &rhs) const
	{ return (x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y) + (z - rhs.z) * (z - rhs.z); }

	Float dis(const Vec3 &rhs) const
	{ return std::sqrt(dis2(rhs)); }

	static Float dot(const Vec3 &lhs, const Vec3 &rhs)
	{ return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
};

#endif //SPATIALDS_VEC_HPP
