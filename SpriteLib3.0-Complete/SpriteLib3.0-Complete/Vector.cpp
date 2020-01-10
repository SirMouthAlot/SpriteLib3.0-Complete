#include "Vector.h"

vec2::vec2()
{
}

vec2::vec2(float _x, float _y)
{
	//Sets x and y
	x = _x;
	y = _y;
}

vec2::vec2(vec3 _xy)
{
	x = _xy.x;
	y = _xy.y;
}

void vec2::Subtract(vec2 v2)
{
	//Subtracts v2 from THIS
	this->x -= v2.x;
	this->y -= v2.y;
}

void vec2::MultScalar(float s)
{
	//multiplies s into THIS
	this->x *= s;
	this->y *= s;
}

void vec2::DivScalar(float s)
{
	//Divides THIS by s
	this->x /= s;
	this->y /= s;
}

float vec2::Dot(vec2 v2)
{
	//Returns the dot product between THIS and v2
	return float(x*v2.x + y * v2.y);
}

float vec2::GetMagnitude()
{
	return float(sqrt((x * x) + (y * y)));
}

float vec2::GetMagnitudeSquared()
{
	//Stores the result of Getmagnitude
	float magnitude = GetMagnitude();

	//Squares the magnitude
	return magnitude * magnitude;
}

vec2 vec2::Normalize()
{
	//Divides this by the magnitude to get a vector in the same
	//direction, but with a magnitude of 1 
	vec2 normalized = *this / GetMagnitude();

	return normalized;
}

vec2 vec2::Project(vec2 b)
{
	//Just for easy readability
	vec2 a = *this;

	//Equation
	//	  A dot B
	//B * ------- 
	//	  ||B||^2


	float numerator = a.Dot(b);
	float denominator = b.GetMagnitudeSquared();

	return (b * (numerator / denominator));
}

float vec2::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = x
	//* 1 = y
	return *hold[i];
}

vec2 vec2::operator-()
{
	return vec2(-x, -y);
}

vec2 vec2::operator+(vec2 v1)
{
	return vec2(this->x + v1.x, this->y + v1.y);
}

vec2 vec2::operator-(vec2 v1)
{
	return vec2(this->x - v1.x, this->y - v1.y);
}

vec2 vec2::operator*(float f)
{
	return vec2(this->x * f, this->y * f);
}

vec2 vec2::operator/(float s)
{
	return vec2(this->x / s, this->y / s);
}

vec3::vec3()
{
}

vec3::vec3(float _x, float _y, float _z)
{
	//Sets x, y, and
	x = _x;
	y = _y;
	z = _z;
}

vec3::vec3(vec2 _xy, float _z)
{
	x = _xy.x;
	y = _xy.y;
	z = _z;
}

vec3::vec3(vec4 _xyz)
{
	x = _xyz.x;
	y = _xyz.y;
	z = _xyz.z;
}

float vec3::GetMagnitude()
{
	return float(sqrt((x * x) + (y * y) + (z * z)));
}

float vec3::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = x
	//* 1 = y
	//* 2 = z
	return *hold[i];
}

vec3 vec3::operator-()
{
	return vec3(-x, -y, -z);
}

vec3 vec3::operator+(vec3 v2)
{
	return vec3(this->x + v2.x, this->y + v2.y, this->z + v2.z);;
}

vec3 vec3::operator-(vec3 v2)
{
	return vec3(this->x - v2.x, this->y - v2.y, this->z - v2.y);
}

vec3 vec3::operator*(float f)
{
	return vec3(this->x * f, this->y * f, this->z * f);
}

vec3 vec3::operator/(float f)
{
	return vec3(this->x / f, this->y / f, this->z / f);
}

vec4::vec4()
{
}

vec4::vec4(float _x, float _y, float _z, float _w)
{
	//Sets x, y, z, and w
	x = _x;
	y = _y;
	z = _z;
	w = _w;

}

vec4::vec4(vec3 _xyz, float _w)
{
	x = _xyz.x;
	y = _xyz.y;
	z = _xyz.z;
	w = _w;
}

vec4::vec4(vec2 _xy, vec2 _zw)
{
	x = _xy.x;
	y = _xy.y;
	z = _zw.x;
	w = _zw.y;
}

vec4::vec4(vec2 _xy, float _z, float _w)
{
	x = _xy.x;
	y = _xy.y;
	z = _z;
	w = _w;
}

vec4::vec4(float _x, float _y, vec2 _zw)
{
	x = _x;
	y = _y;
	z = _zw.x;
	w = _zw.y;
}

float vec4::GetMagnitude()
{
	return float(sqrt((x * x) + (y * y) + (z * z) + (w * w)));
}

float vec4::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = x
	//* 1 = y
	//* 2 = z
	//* 3 = w
	return *hold[i];
}

vec4 vec4::operator-()
{
	return vec4(-x, -y, -z, -w);
}

vec4 vec4::operator+(vec4 v2)
{
	return vec4(this->x + v2.x, this->y + v2.y, this->z + v2.z, this->w + v2.w);
}

vec4 vec4::operator-(vec4 v2)
{
	return vec4(this->x - v2.x, this->y - v2.y, this->z - v2.z, this->w - v2.w);
}

vec4 vec4::operator*(float f)
{
	return vec4(this->x * f, this->y * f, this->z * f, this->w * f);
}

vec4 vec4::operator/(float f)
{
	return vec4(this->x / f, this->y / f, this->z / f, this->w / f);
}

vecN::vecN()
{
	for (int i = 0; i < 4; i++)
	{
		m_components.push_back(0.f);
	}

	m_N = 4;
}

vecN::vecN(int N)
{
	for (int i = 0; i < N; i++)
	{
		m_components.push_back(0.f);
	}

	m_N = N;
}

float vecN::GetComponent(int N)
{
	return m_components[N];
}

void vecN::SetComponents(int N, float value)
{
	m_components[N] = value;
}

int vecN::GetN() const
{
	return m_N;
}

float vecN::Dot(vecN v2)
{
	if (this->m_components.size() != v2.m_components.size())
	{
		//You can't dot vectors of different sizes.
		printf("Invalid operation\n");

		return -1.f;
	}

	float sum = 0.f;

	//This is the same as writing
	//v1.component1 * v2.component1 + v1.component2 * v2.component2.... + v1.componentN * v2.componentN
	for (int i = 0; i < m_components.size(); i++)
	{
		sum += (this->m_components[i] * v2.m_components[i]);
	}

	//returns the value.
	return sum;
}

vecN vecN::operator+(vecN v2)
{
	if (v2.GetN() != this->GetN())
	{
		printf("Invalid operation");
		return vecN();
	}


	vecN sum(v2.GetN());
	
	for (int i = 0; i < v2.GetN(); i++)
	{
		//Adds together the two values and sets the sum vecN value
		sum.SetComponents(i, (this->GetComponent(i) + v2.GetComponent(i)));
	}

	return sum;
}
