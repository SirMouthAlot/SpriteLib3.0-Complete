#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <math.h>
#include <vector>

#ifdef VECTOR_EXPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif

class vec2;
class vec3; 
class vec4;

class DLLEXPORT vec2
{
public:
	//Empty constructor
	vec2();
	//Creates vec2 with x and y
	vec2(float _x, float _y);
	vec2(vec3 _xy);

	//Defaults to zero
	float x = 0.f;
	float y = 0.f;
	//Used for indexing using [] operator
	float* hold[2] = { &x, &y };

	void Subtract(vec2 v2);
	void MultScalar(float s);
	void DivScalar(float s);

	float Dot(vec2 v2);

	float GetMagnitude();
	float GetMagnitudeSquared();
	vec2 Normalize();

	vec2 Project(vec2 b);

	//Operator overload for indexing using []
	float operator[](int i);
	vec2 operator-();
	vec2 operator+(vec2 v1);
	vec2 operator-(vec2 v1);

	vec2 operator*(float f);
	vec2 operator/(float s);
};

class vec3
{
public:
	//Empty constructor
	vec3();
	//Creates vec3 with x, y, and z
	vec3(float _x, float _y, float _z);
	vec3(vec2 _xy, float _z);
	vec3(vec4 _xyz);
	
	//Defaults to zero
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	//Used for indexing using [] operator
	float* hold[3] = { &x, &y, &z };

	float GetMagnitude();

	//Operator overload for indexing using []
	float operator[](int i);
	//Negation operator for creating a negative version of the vector
	vec3 operator-();
	vec3 operator+(vec3 v2);
	vec3 operator-(vec3 v2);

	vec3 operator*(float f);
	vec3 operator/(float f);
};

class vec4
{
public:
	//empty constructor
	vec4();
	//Creates vec4 with x,y,z and w
	vec4(float _x, float _y, float _z, float _w);
	vec4(vec3 _xyz, float _w);
	vec4(vec2 _xy, vec2 _zw);
	vec4(vec2 _xy, float _z, float _w);
	vec4(float _x, float _y, vec2 _zw);

	//Defaults to zero
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;
	//Used for indexing using [] operator
	float* hold[4] = { &x, &y, &z, &w };

	float GetMagnitude();

	//Operator overload for indexing using []
	float operator[](int i);
	//Negation operator for creating a negative version of the vector
	vec4 operator-();
	vec4 operator+(vec4 v2);
	vec4 operator-(vec4 v2);

	vec4 operator*(float f);
	vec4 operator/(float f);
};

class vecN
{
public:
	//Empty constructor
	//Defaults to Vec4
	vecN();
	//Creates vector with N components
	vecN(int N);

	float GetComponent(int N);
	void SetComponents(int N, float value);

	int GetN() const;

	float Dot(vecN v2);

	vecN operator+(vecN v2);

	std::vector<float> m_components;

	int m_N = 4;
};

#endif // !__VECTOR_H__
