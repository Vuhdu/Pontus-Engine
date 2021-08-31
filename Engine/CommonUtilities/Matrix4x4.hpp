#pragma once
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <array>
#include <math.h>
#include <assert.h>

namespace CommonUtilities
{
	template<class T>
	class Matrix4x4
	{
	public:
		// Creates the identity matrix.
		Matrix4x4<T>();
		~Matrix4x4<T>() = default;

		// Copy Constructor.
		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		Matrix4x4<T> operator+(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator+=(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T> operator-(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator-=(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator*=(const Matrix4x4<T>& aMatrix);
		Matrix4x4<T>& operator=(const Matrix4x4<T>& aMatrix);

		// Static functions for creating rotation matrices.
		static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);

		// Static function for creating a transpose of a matrix.
		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);

		// Assumes aTransform is made up of nothing but rotations and translations.
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);

		const Vector3<T>	Forward() const;
		const Vector3<T>	Right() const;
		const Vector3<T>	Up() const;

		const Vector3<T>	Position() const;
		// Not correct
		// Taken from stackexchange.com
		// https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati/3554913
		const Vector3<T>	Scale() const;

		const Vector3<float>	EulerAngles() const;

	private:
		std::array<std::array<T, 4>, 4> myMatrix;
	};

	template<class T>
	inline Matrix4x4<T>::Matrix4x4()
	{
		myMatrix[0][0] = 1;
		myMatrix[0][1] = 0;
		myMatrix[0][2] = 0;
		myMatrix[0][3] = 0;

		myMatrix[1][0] = 0;
		myMatrix[1][1] = 1;
		myMatrix[1][2] = 0;
		myMatrix[1][3] = 0;

		myMatrix[2][0] = 0;
		myMatrix[2][1] = 0;
		myMatrix[2][2] = 1;
		myMatrix[2][3] = 0;

		myMatrix[3][0] = 0;
		myMatrix[3][1] = 0;
		myMatrix[3][2] = 0;
		myMatrix[3][3] = 1;
	}

	template<class T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		myMatrix = aMatrix.myMatrix;
	}

	template<class T>
	inline T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow >= 1 && aRow < 4 + 1);
		assert(aColumn >= 1 && aColumn < 4 + 1);
		const int row = aRow - 1;
		const int column = aColumn - 1;
		return myMatrix[row][column];
	}

	template<class T>
	inline const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow >= 1 && aRow < 4 + 1);
		assert(aColumn >= 1 && aColumn < 4 + 1);
		return myMatrix[(aRow - 1)][(aColumn - 1)];
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::operator+(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> temp;
		for (unsigned int rows = 0; rows < 4; rows++)
		{
			for (unsigned int columns = 0; columns < 4; columns++)
			{
				temp.myMatrix[rows][columns] = myMatrix[rows][columns] + aMatrix(rows + 1, columns + 1);
			}
		}
		return temp;
	}

	template<class T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& aMatrix)
	{
		return (*this) = (*this) + aMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::operator-(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> temp;
		for (unsigned int rows = 0; rows < 4; rows++)
		{
			for (unsigned int columns = 0; columns < 4; columns++)
			{
				temp.myMatrix[rows][columns] = myMatrix[rows][columns] - aMatrix(rows + 1, columns + 1);
			}
		}
		return temp;
	}

	template<class T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator-=(const Matrix4x4<T>& aMatrix)
	{
		return (*this) = (*this) - aMatrix;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T>& aMatrix)
	{
		Matrix4x4<T> temp;
		for (unsigned int rows = 0; rows < 4; rows++)
		{
			for (unsigned int columns = 0; columns < 4; columns++)
			{
				T result = 0;
				for (unsigned int index = 0; index < 4; index++)
				{
					result += myMatrix[rows][index] * aMatrix(index + 1, columns + 1);
				}
				temp.myMatrix[rows][columns] = result;
			}
		}
		return temp;
	}

	template<class T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& aMatrix)
	{
		return (*this) = (*this) * aMatrix;
	}

	template<class T>
	inline Vector4<T> operator*(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		Vector4<T> temp;
		temp.x = aVector.x * aMatrix(1, 1) + aVector.y * aMatrix(2, 1) + aVector.z * aMatrix(3, 1) + aVector.w * aMatrix(4, 1);
		temp.y = aVector.x * aMatrix(1, 2) + aVector.y * aMatrix(2, 2) + aVector.z * aMatrix(3, 2) + aVector.w * aMatrix(4, 2);
		temp.z = aVector.x * aMatrix(1, 3) + aVector.y * aMatrix(2, 3) + aVector.z * aMatrix(3, 3) + aVector.w * aMatrix(4, 3);
		temp.w = aVector.x * aMatrix(1, 4) + aVector.y * aMatrix(2, 4) + aVector.z * aMatrix(3, 4) + aVector.w * aMatrix(4, 4);
		return temp;
	}

	template<class T>
	inline Vector4<T> operator*=(const Vector4<T>& aVector, const Matrix4x4<T>& aMatrix)
	{
		return (aVector * aMatrix);
	}

	template<class T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& aMatrix)
	{
		for (unsigned int rows = 0; rows < 4; rows++)
		{
			for (unsigned int columns = 0; columns < 4; columns++)
			{
				myMatrix[rows][columns] = aMatrix(rows + 1, columns + 1);
			}
		}
		return *this;
	}

	template<class T>
	inline bool operator==(const Matrix4x4<T>& aMatrix0, const Matrix4x4<T>& aMatrix1)
	{
		for (unsigned int rows = 0; rows < 4; rows++)
		{
			for (unsigned int columns = 0; columns < 4; columns++)
			{
				if (aMatrix0(rows + 1, columns + 1) != aMatrix1(rows + 1, columns + 1))
				{
					return false;
				}
			}
		}
		return true;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
	{
		Matrix4x4<T> temp;
		temp(1, 1) = 1;
		temp(1, 2) = 0;
		temp(1, 3) = 0;
		temp(1, 4) = 0;

		temp(2, 1) = 0;
		temp(2, 2) = static_cast<T>(cos(aAngleInRadians));
		temp(2, 3) = static_cast<T>(sin(aAngleInRadians));
		temp(2, 4) = 0;

		temp(3, 1) = 0;
		temp(3, 2) = static_cast<T>(-sin(aAngleInRadians));
		temp(3, 3) = static_cast<T>(cos(aAngleInRadians));
		temp(3, 4) = 0;

		temp(4, 1) = 0;
		temp(4, 2) = 0;
		temp(4, 3) = 0;
		temp(4, 4) = 1;
		return temp;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
	{
		Matrix4x4<T> temp;
		temp(1, 1) = static_cast<T>(cos(aAngleInRadians));
		temp(1, 2) = 0;
		temp(1, 3) = static_cast<T>(-sin(aAngleInRadians));
		temp(1, 4) = 0;

		temp(2, 1) = 0;
		temp(2, 2) = 1;
		temp(2, 3) = 0;
		temp(2, 4) = 0;

		temp(3, 1) = static_cast<T>(sin(aAngleInRadians));
		temp(3, 2) = 0;
		temp(3, 3) = static_cast<T>(cos(aAngleInRadians));
		temp(3, 4) = 0;

		temp(4, 1) = 0;
		temp(4, 2) = 0;
		temp(4, 3) = 0;
		temp(4, 4) = 1;
		return temp;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
	{
		Matrix4x4<T> temp;
		temp(1, 1) = static_cast<T>(cos(aAngleInRadians));
		temp(1, 2) = static_cast<T>(sin(aAngleInRadians));
		temp(1, 3) = 0;
		temp(1, 4) = 0;

		temp(2, 1) = static_cast<T>(-sin(aAngleInRadians));
		temp(2, 2) = static_cast<T>(cos(aAngleInRadians));
		temp(2, 3) = 0;
		temp(2, 4) = 0;

		temp(3, 1) = 0;
		temp(3, 2) = 0;
		temp(3, 3) = 1;
		temp(3, 4) = 0;

		temp(4, 1) = 0;
		temp(4, 2) = 0;
		temp(4, 3) = 0;
		temp(4, 4) = 1;
		return temp;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> temp;
		temp(1, 1) = aMatrixToTranspose(1, 1);
		temp(1, 2) = aMatrixToTranspose(2, 1);
		temp(1, 3) = aMatrixToTranspose(3, 1);
		temp(1, 4) = aMatrixToTranspose(4, 1);

		temp(2, 1) = aMatrixToTranspose(1, 2);
		temp(2, 2) = aMatrixToTranspose(2, 2);
		temp(2, 3) = aMatrixToTranspose(3, 2);
		temp(2, 4) = aMatrixToTranspose(4, 2);

		temp(3, 1) = aMatrixToTranspose(1, 3);
		temp(3, 2) = aMatrixToTranspose(2, 3);
		temp(3, 3) = aMatrixToTranspose(3, 3);
		temp(3, 4) = aMatrixToTranspose(4, 3);

		temp(4, 1) = aMatrixToTranspose(1, 4);
		temp(4, 2) = aMatrixToTranspose(2, 4);
		temp(4, 3) = aMatrixToTranspose(3, 4);
		temp(4, 4) = aMatrixToTranspose(4, 4);
		return temp;
	}

	template<class T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> rotationMatrix = Transpose(aTransform);
		rotationMatrix(4, 1) = 0;
		rotationMatrix(4, 2) = 0;
		rotationMatrix(4, 3) = 0;

		rotationMatrix(1, 4) = 0;
		rotationMatrix(2, 4) = 0;
		rotationMatrix(3, 4) = 0;

		rotationMatrix(4, 4) = 1;

		Vector4<T> translation;
		translation.x = -aTransform(4, 1);
		translation.y = -aTransform(4, 2);
		translation.z = -aTransform(4, 3);
		translation.w = 0;
		translation = translation * rotationMatrix;

		Matrix4x4<T> temp;
		temp(1, 1) = rotationMatrix(1, 1);
		temp(1, 2) = rotationMatrix(1, 2);
		temp(1, 3) = rotationMatrix(1, 3);
		temp(1, 4) = aTransform(1, 4);

		temp(2, 1) = rotationMatrix(2, 1);
		temp(2, 2) = rotationMatrix(2, 2);
		temp(2, 3) = rotationMatrix(2, 3);
		temp(2, 4) = aTransform(2, 4);

		temp(3, 1) = rotationMatrix(3, 1);
		temp(3, 2) = rotationMatrix(3, 2);
		temp(3, 3) = rotationMatrix(3, 3);
		temp(3, 4) = aTransform(3, 4);

		temp(4, 1) = translation.x;
		temp(4, 2) = translation.y;
		temp(4, 3) = translation.z;
		temp(4, 4) = aTransform(4, 4);
		return temp;
	}

	template<class T>
	inline const Vector3<T> Matrix4x4<T>::Right() const
	{
		return { myMatrix[0][0], myMatrix[0][1], myMatrix[0][2] };
	}

	template<class T>
	inline const Vector3<T> Matrix4x4<T>::Up() const
	{
		return { myMatrix[1][0], myMatrix[1][1], myMatrix[1][2] };
	}

	template<class T>
	inline const Vector3<T> Matrix4x4<T>::Forward() const
	{
		return { myMatrix[2][0], myMatrix[2][1], myMatrix[2][2] };
	}

	template<class T>
	inline const Vector3<T> Matrix4x4<T>::Position() const
	{
		return { myMatrix[3][0], myMatrix[3][1], myMatrix[3][2] };
	}

	template<class T>
	inline const Vector3<T> Matrix4x4<T>::Scale() const
	{
		Vector3<T> scale;

		scale.x = Vector3<T>(myMatrix[0][0], myMatrix[0][1], myMatrix[0][2]).Length();
		scale.y = Vector3<T>(myMatrix[1][0], myMatrix[1][1], myMatrix[1][2]).Length();
		scale.z = Vector3<T>(myMatrix[2][0], myMatrix[2][1], myMatrix[2][2]).Length();

		return scale;
	}

	template<class T>
	inline const Vector3<float> Matrix4x4<T>::EulerAngles() const
	{
		const float PI = 3.14159265358979323846f;
		const float toDeg = 180 / PI;
		CU::Vector3<float> euler;

		euler.x = asinf(-myMatrix[2][1]) * toDeg;						// Pitch

		if (cosf(euler.x > 0.0001f))
		{
			euler.y = atan2f(myMatrix[2][0], myMatrix[2][2]) * toDeg; // Yaw
			euler.z = atan2f(myMatrix[0][1], myMatrix[1][1]) * toDeg; // Roll
		}
		else
		{
			euler.y = 0.0f;
			euler.z = atan2f(-myMatrix[1][0], myMatrix[0][0]) * toDeg;
		}
		
		return euler;
	}

	using Matrix4x4f = CommonUtilities::Matrix4x4<float>;
}
namespace CU = CommonUtilities;