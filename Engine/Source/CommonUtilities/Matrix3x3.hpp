#pragma once

#include <cassert>

#include "Vector3.hpp"
#include "Matrix4x4.hpp"

namespace CommonUtilities
{
	template <typename T>
	class Matrix3x3
	{
	public:
		static const Matrix3x3<T> Zeroed;
		static const Matrix3x3<T> Identity;

		Matrix3x3<T>();

		Matrix3x3<T>(const T(&aValues)[3 * 3]);

		Matrix3x3<T>(const Matrix3x3<T>& aMatrix);

		// Copies the top left 3x3 part of the Matrix4x4.
		Matrix3x3<T>(const Matrix4x4<T>& aMatrix);

		static Matrix3x3<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix3x3<T> CreateRotationAroundZ(const T aAngleInRadians);

		static Matrix3x3<T> Transpose(const Matrix3x3<T>& aMatrixToTranspose);

		Matrix3x3<T>& operator=(const Matrix3x3<T>& aRhs);

		bool operator==(const Matrix3x3<T>& aRhs) const;

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		const Vector3<T>& GetRight() const;
		const Vector3<T>& GetUp() const;
		const Vector3<T>& GetForward() const;

	private:
		union
		{
			struct
			{
				// Instead of keeping in array, i keep them in variables 
				// to avoid the confusion with 0 and 1 index
				// Also avoided the "my" prefix because it would look stoopid
				T m11, m12, m13,
					m21, m22, m23,
					m31, m32, m33;
			};

			// Used for some good memcpy for performance reasons and initializations
			T myValues[3][3];

			// Added for convenience, still unsure if should be public or private, private for now
			struct
			{
				// X axis
				Vector3<T> myX;

				// Y axis
				Vector3<T> myY;

				// Z axis
				Vector3<T> myZ;
			};
		};
	};

	template <typename T>
	const Matrix3x3<T> Matrix3x3<T>::Zeroed = Matrix3x3<T>({ 0 });

	template <typename T>
	const Matrix3x3<T> Matrix3x3<T>::Identity = Matrix3x3<T>({
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
		});

	using Matrix3f = Matrix3x3<float>;
	using Matrix3d = Matrix3x3<double>;

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3()
		: m11(1), m12(0), m13(0),
		m21(0), m22(1), m23(0),
		m31(0), m32(0), m33(1)
	{
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const T(&aValues)[3 * 3])
	{
		memcpy(myValues, aValues, sizeof(myValues));
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix3x3<T>& aMatrix)
	{
		operator=(aMatrix);
	}

	template<typename T>
	inline Matrix3x3<T>::Matrix3x3(const Matrix4x4<T>& aMatrix)
	{
		// TODO: Make Matrix4x4 a friend to simply access it directly?

		m11 = aMatrix(1, 1);
		m12 = aMatrix(1, 2);
		m13 = aMatrix(1, 3);

		m21 = aMatrix(2, 1);
		m22 = aMatrix(2, 2);
		m23 = aMatrix(2, 3);

		m31 = aMatrix(3, 1);
		m32 = aMatrix(3, 2);
		m33 = aMatrix(3, 3);
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		Matrix3x3<T> matrix = Identity;

		const auto cosTheta = cos(aAngleInRadians);
		const auto sinTheta = sin(aAngleInRadians);

		matrix.m22 = cosTheta;
		matrix.m23 = sinTheta;

		matrix.m32 = -sinTheta;
		matrix.m33 = cosTheta;

		return matrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		Matrix3x3<T> matrix = Identity;

		const auto cosTheta = cos(aAngleInRadians);
		const auto sinTheta = sin(aAngleInRadians);

		matrix.m11 = cosTheta;
		matrix.m13 = -sinTheta;

		matrix.m31 = sinTheta;
		matrix.m33 = cosTheta;

		return matrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		Matrix3x3<T> matrix = Identity;

		const auto cosTheta = cos(aAngleInRadians);
		const auto sinTheta = sin(aAngleInRadians);

		matrix.m11 = cosTheta;
		matrix.m12 = sinTheta;

		matrix.m21 = -sinTheta;
		matrix.m22 = cosTheta;

		return matrix;
	}

	template<typename T>
	inline Matrix3x3<T> Matrix3x3<T>::Transpose(const Matrix3x3<T>& aMatrixToTranspose)
	{
		Matrix3x3<T> matrix;

		// Copy the diagonal
		matrix.m11 = aMatrixToTranspose.m11;
		matrix.m22 = aMatrixToTranspose.m22;
		matrix.m33 = aMatrixToTranspose.m33;

		// Swap the rest
		matrix.m21 = aMatrixToTranspose.m12;
		matrix.m12 = aMatrixToTranspose.m21;

		matrix.m31 = aMatrixToTranspose.m13;
		matrix.m13 = aMatrixToTranspose.m31;

		matrix.m32 = aMatrixToTranspose.m23;
		matrix.m23 = aMatrixToTranspose.m32;

		return matrix;
	}

	template<typename T>
	inline Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix3x3<T>& aRhs)
	{
		memcpy(myValues, aRhs.myValues, sizeof(myValues));
		return *this;
	}

	template<typename T>
	bool Matrix3x3<T>::operator==(const Matrix3x3<T>& aRhs) const
	{
		// NOTE: Check with epsilon?
		return m11 == aRhs.m11 && m12 == aRhs.m12 && m13 == aRhs.m13 &&
			m21 == aRhs.m21 && m22 == aRhs.m22 && m23 == aRhs.m23 &&
			m31 == aRhs.m31 && m32 == aRhs.m32 && m33 == aRhs.m33;
	}

	template<typename T>
	T& Matrix3x3<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow > 0 && aColumn > 0 && "Out of bounds");
		assert(aRow <= 3 && aColumn <= 3 && "Out of bounds");

		return myValues[aRow - 1][aColumn - 1];
	}

	template<typename T>
	const T& Matrix3x3<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow > 0 && aColumn > 0 && "Out of bounds");
		assert(aRow <= 3 && aColumn <= 3 && "Out of bounds");

		return myValues[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline const Vector3<T>& Matrix3x3<T>::GetRight() const
	{
		return myX;
	}

	template<typename T>
	inline const Vector3<T>& Matrix3x3<T>::GetUp() const
	{
		return myY;
	}

	template<typename T>
	inline const Vector3<T>& Matrix3x3<T>::GetForward() const
	{
		return myZ;
	}

	template<typename T>
	inline Matrix3x3<T> operator+(Matrix3x3<T> aLhs, const Matrix3x3<T>& aRhs)
	{
		aLhs += aRhs;
		return aLhs;
	}

	template<typename T>
	inline Matrix3x3<T>& operator+=(Matrix3x3<T>& aLhs, const Matrix3x3<T>& aRhs)
	{
		aLhs.m11 += aRhs.m11;
		aLhs.m12 += aRhs.m12;
		aLhs.m13 += aRhs.m13;

		aLhs.m21 += aRhs.m21;
		aLhs.m22 += aRhs.m22;
		aLhs.m23 += aRhs.m23;

		aLhs.m31 += aRhs.m31;
		aLhs.m32 += aRhs.m32;
		aLhs.m33 += aRhs.m33;

		return aLhs;
	}

	template<typename T>
	inline Matrix3x3<T> operator-(Matrix3x3<T> aLhs, const Matrix3x3<T>& aRhs)
	{
		aLhs -= aRhs;
		return aLhs;
	}

	template<typename T>
	inline Matrix3x3<T>& operator-=(Matrix3x3<T>& aLhs, const Matrix3x3<T>& aRhs)
	{
		aLhs.m11 -= aRhs.m11;
		aLhs.m12 -= aRhs.m12;
		aLhs.m13 -= aRhs.m13;

		aLhs.m21 -= aRhs.m21;
		aLhs.m22 -= aRhs.m22;
		aLhs.m23 -= aRhs.m23;

		aLhs.m31 -= aRhs.m31;
		aLhs.m32 -= aRhs.m32;
		aLhs.m33 -= aRhs.m33;

		return aLhs;
	}

	template<typename T>
	inline Matrix3x3<T> operator*(Matrix3x3<T> aLhs, const Matrix3x3<T>& aRhs)
	{
		aLhs *= aRhs;
		return aLhs;
	}

	template<typename T>
	inline Matrix3x3<T>& operator*=(Matrix3x3<T>& aLhs, const Matrix3x3<T>& aRhs)
	{
		Matrix3x3<T> result = Matrix3x3<T>::Zeroed;

		result.m11 = (aLhs.m11 * aRhs.m11) + (aLhs.m12 * aRhs.m21) + (aLhs.m13 * aRhs.m31);
		result.m12 = (aLhs.m11 * aRhs.m12) + (aLhs.m12 * aRhs.m22) + (aLhs.m13 * aRhs.m32);
		result.m13 = (aLhs.m11 * aRhs.m13) + (aLhs.m12 * aRhs.m23) + (aLhs.m13 * aRhs.m33);

		result.m21 = (aLhs.m21 * aRhs.m11) + (aLhs.m22 * aRhs.m21) + (aLhs.m23 * aRhs.m31);
		result.m22 = (aLhs.m21 * aRhs.m12) + (aLhs.m22 * aRhs.m22) + (aLhs.m23 * aRhs.m32);
		result.m23 = (aLhs.m21 * aRhs.m13) + (aLhs.m22 * aRhs.m23) + (aLhs.m23 * aRhs.m33);

		result.m31 = (aLhs.m31 * aRhs.m11) + (aLhs.m32 * aRhs.m21) + (aLhs.m33 * aRhs.m31);
		result.m32 = (aLhs.m31 * aRhs.m12) + (aLhs.m32 * aRhs.m22) + (aLhs.m33 * aRhs.m32);
		result.m33 = (aLhs.m31 * aRhs.m13) + (aLhs.m32 * aRhs.m23) + (aLhs.m33 * aRhs.m33);

		aLhs = result;

		return aLhs;
	}

	template<typename T>
	inline Vector3<T> operator*(const Vector3<T>& aLhs, const Matrix3x3<T>& aRhs)
	{
		Vector3<T> result;

		result.x = (aLhs.x * aRhs.m11) + (aLhs.y * aRhs.m21) + (aLhs.z * aRhs.m31);
		result.y = (aLhs.x * aRhs.m12) + (aLhs.y * aRhs.m22) + (aLhs.z * aRhs.m32);
		result.z = (aLhs.x * aRhs.m13) + (aLhs.y * aRhs.m23) + (aLhs.z * aRhs.m33);

		return result;
	}
}