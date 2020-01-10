#include "Matrix.h"

mat2::mat2(vec2 _row1, vec2 _row2)
{
	//Sets row1 and row2
	row1 = _row1;
	row2 = _row2;
}

void mat2::Add(mat2 m)
{
	//Adds the passed parameter to THIS matrix
	this->row1 = this->row1 + m.row1;
	this->row2 = this->row2 + m.row2;
}

void mat2::Subtract(mat2 m)
{
	//Subtracts the passed parameter from THIS matrix
	this->row1 = this->row1 - m.row1;
	this->row2 = this->row2 - m.row2;
}

void mat2::Print()
{
	//Prints out formatted matrix in console
	std::cout << "[ " << this->row1.x << ", " << this->row1.y << " ]\n";
	std::cout << "[ " << this->row2.x << ", " << this->row2.y << " ]\n";
}

mat2 mat2::Transpose()
{
	//Just swaps the rows and columns within the matrix
	mat2 temp;
	temp.row1 = vec2(row1.x, row2.x);
	temp.row2 = vec2(row1.y, row2.y);

	//Returns the transposed matrix
	return temp;
}

float mat2::Determinant()
{
	//Calculates the determinant
	//det(Mat2) = AD - BC
	float temp = (row1.x * row2.y) - (row1.y * row2.x);

	//Returns determinant
	return temp;
}

mat2 mat2::Inverse()
{
	//The inverse is equal to this
	//     -1	=		1
	//[A B]		=	---------[ D -B]
	//[C D]		=	Det(Mat2)[-C  A] 

	//Gets one over determinant
	float oneOverDet =  1.f/Determinant();

	//Creates new matrix to be multiplied.
	mat2 temp = mat2(
		vec2(row2.y, -row1.y),
		vec2(-row2.x, row2.x)
	);

	//Multiplies temp by oneOverDet
	temp = temp * oneOverDet;
	
	//Returns the inverse of our matrix
	return temp;
}

mat2 mat2::operator+(mat2 m)
{
	//Retrns the summed matrix
	return mat2(this->row1 + m.row1, this->row2 + m.row2);
}

mat2 mat2::operator-(mat2 m)
{
	//Returns difference
	return mat2(this->row1 - m.row1, this->row2 - m.row2);
}

mat2 mat2::operator*(float f)
{
	//Returns product
	return mat2(this->row1 * f, this->row2 * f);
}

mat2 mat2::operator*(mat2 m)
{
	vec2 mCol1 = vec2(m.row1.x, m.row2.x);
	vec2 mCol2 = vec2(m.row1.y, m.row2.y);

	mat2 temp = mat2(
		vec2(row1.Dot(mCol1), row1.Dot(mCol2)),
		vec2(row2.Dot(mCol1), row2.Dot(mCol2))
	);
	
	return temp;
}

vec2 mat2::operator*(vec2 v)
{
	//Multiplies 3x3 matrix by 3D vector
	vec2 temp = vec2(
		row1.x * v.x + row1.y * v.y,
		row2.x * v.x + row2.y * v.y
	);

	//returns calculated 3D vector
	return temp;
}

vec2 mat2::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = row1
	//* 1 = row2
	return *hold[i];
}

mat3::mat3(vec3 _row1, vec3 _row2, vec3 _row3)
{
	//Sets row1, row2, and row3
	row1 = _row1;
	row2 = _row2;
	row3 = _row3;
}

mat3::mat3(mat4 m)
{
	//Creates a 3x3 matrix using a 4x4 matrix
	this->row1 = vec3(m.row1.x, m.row1.y, m.row1.z);
	this->row2 = vec3(m.row2.x, m.row2.y, m.row2.z);
	this->row3 = vec3(m.row3.x, m.row3.y, m.row3.z);
}

mat3 mat3::Transpose(mat3 R)
{
	//Just swaps the rows and columns within the matrix
	mat3 temp;
	temp.row1 = vec3(R.row1.x, R.row2.x, R.row3.x);
	temp.row2 = vec3(R.row1.y, R.row2.y, R.row3.y);
	temp.row3 = vec3(R.row1.z, R.row2.z, R.row3.z);

	//Returns the transposed matrix
	return temp;
}

mat3 mat3::operator-()
{
	//Negates all of the rows
	return mat3(-row1, -row2, -row3);
}

vec3 mat3::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = row1
	//* 1 = row2
	//* 2 = row3
	return *hold[i];
}

vec3 mat3::operator*(vec3 vec)
{
	//Multiplies 3x3 matrix by 3D vector
	vec3 temp;
	temp = vec3(
		row1.x * vec.x + row1.y * vec.y + row1.z * vec.z,
		row2.x * vec.x + row2.y * vec.y + row2.z * vec.z,
		row3.x * vec.x + row3.y * vec.y + row3.z * vec.z
	);

	//returns calculated 3D vector
	return temp;
}

mat4::mat4(vec4 _row1, vec4 _row2, vec4 _row3, vec4 _row4)
{
	//Sets row1, row2, row3 and row4
	row1 = _row1;
	row2 = _row2;
	row3 = _row3;
	row4 = _row4;
}

mat4::mat4(mat3 rot, vec3 trans)
{
	//Creates a homogenous transformation matrix using this data
	row1 = vec4(rot.row1.x, rot.row1.y, rot.row1.z, trans.x);
	row2 = vec4(rot.row2.x, rot.row2.y, rot.row2.z, trans.y);
	row3 = vec4(rot.row3.x, rot.row3.y, rot.row3.z, trans.z);
	row4 = vec4(0.f, 0.f, 0.f, 1.f);
}

mat4 mat4::FastInverse(mat4 mat)
{
	//in order to do a faster inverse we assume it's a homogenous transformation matrix which means
	//| R(3x3) | T(1x3) | inverse is equal to |R(3x3)^T|T(1x3)*-R|
	//|--------|--------|					  |--------|---------|
	//| 0(3x1) | 1(1x1) |					  | 0(3x1) |  1(1x1) |

	//Grab the rotation matrix out of mat
	mat3 rotation = mat3(mat);
	//Grab the translation vector out of mat
	vec3 translation = Translation(mat);
	//Transpose the rotation matrix
	rotation = mat3::Transpose(rotation);
	//Multiply negative rotation by translation
	translation = -rotation * translation;

	//Recreate the homogenous transformation matrix using the new 3x3 rotation matrix and 3d translation vector
	return mat4(rotation, translation);
}

vec3 mat4::Translation(mat4 mat)
{
	//Grabs the translation data from the homogenous transformation matrix
	return vec3(mat.row1.z, mat.row2.z, mat.row3.z);
}

vec4 mat4::operator[](int i)
{
	//Indexes the variables at i
	//* 0 = row1
	//* 1 = row2
	//* 2 = row3
	//* 3 = row4
	return *hold[i];
}

matNxM::matNxM()
{
	for (int i = 0; i < 4; i++)
	{
		m_rows.push_back(vecN());
	}

	m_N = 4;
	m_M = 4;
}

matNxM::matNxM(int N, int M)
{
	for (int i = 0; i < N; i++)
	{
		//Pushes back N rows, with each row having M components
		//Aka, it creates an NxM matrix
		m_rows.push_back(vecN(M));
	}

	m_N = N;
	m_M = M;
}

vecN matNxM::GetRow(int row)
{
	return m_rows[row];
}

float matNxM::GetComponent(int row, int column)
{
	return m_rows[row].GetComponent(column);
}

void matNxM::SetRow(int row, vecN value)
{
	m_rows[row] = value;
}

void matNxM::SetComponent(int row, int column, float value)
{
	m_rows[row].SetComponents(column, value);
}

int matNxM::GetN() const
{
	return m_N;
}

int matNxM::GetM() const
{
	return m_M;
}

matNxM matNxM::operator+(matNxM m)
{
	//If the matrices are different sizes
	if (m.GetM() != this->GetM() || m.GetN() != this->GetN())
	{
		//You can't add matrixes of two different sizes.
		printf("Invalid Operation!\n");

		//Returns defaultly constructed matNxM
		return matNxM();
	}

	//Creates new same sized matrix
	matNxM sum(this->GetN(), this->GetM());

	for (int i = 0; i < this->GetN(); i++)
	{
		//Grabs the two rows (this and m), and adds them together
		//Stores them as a new vecN
		vecN row = (this->GetRow(i) + m.GetRow(i));

		//places the newly created row into sum, in the correct row.
		sum.SetRow(i, row);
	}

	//Returns the completed sum
	return sum;
}
