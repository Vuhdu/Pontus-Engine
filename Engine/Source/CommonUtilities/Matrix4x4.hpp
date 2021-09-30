#pragma once

#include <cassert>
#include <cstring>

#include "Vector4.hpp"
#include "Vector3.hpp"

// Disable warning about unnamed struct
#pragma warning(disable : 4201)

namespace CommonUtilities
{
	template <typename T>
	class Matrix4x4
	{
	public:
		static const Matrix4x4<T> Zeroed;
		static const Matrix4x4<T> Identity;

		Matrix4x4<T>();

		Matrix4x4<T>(const T(&aValues)[4 * 4]);

		Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

		static Matrix4x4<T> CreateRotationAroundX(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundY(const T aAngleInRadians);
		static Matrix4x4<T> CreateRotationAroundZ(const T aAngleInRadians);

		static Matrix4x4<T> CreateTranslation(const Vector3<T> aTranslation);
		static Matrix4x4<T> CreateScale(const float aScale);
		static Matrix4x4<T> CreateScale(const Vector3<T> aScale);

		static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);

		// Assumes aTransform is made up of nothing but rotations and translations.
		static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);

		Matrix4x4<T>& operator=(const Matrix4x4<T>& aRhs);

		bool operator==(const Matrix4x4<T>& aRhs) const;

		// () operator for accessing element (row, column) for read/write or read, respectively.
		T& operator()(const int aRow, const int aColumn);
		const T& operator()(const int aRow, const int aColumn) const;

		const Vector3<T>& GetRight() const;
		const Vector3<T>& GetUp() const;
		const Vector3<T>& GetForward() const;
		const Vector3<T>& GetTranslation() const;

	private:
		union
		{
			struct
			{
				// Instead of keeping in array, i keep them in variables 
				// to avoid the confusion with 0 and 1 index
				// Also avoided the "my" prefix because it would look stoopid
				T m11, m12, m13, m14,
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44;
			};

			// Used for some good memcpy for performance reasons and initializations
			T myValues[4][4];

			// Added for convenience, still unsure if should be public or private, private for now
			struct
			{
				Vector3<T> myX;
				float myXW;
				Vector3<T> myY;
				float myYW;
				Vector3<T> myZ;
				float myZW;
				Vector3<T> myW;
				float myWW;
			};
		};
	};

	template <typename T>
	const Matrix4x4<T> Matrix4x4<T>::Zeroed = Matrix4x4<T>({ 0 });

	template <typename T>
	const Matrix4x4<T> Matrix4x4<T>::Identity = Matrix4x4<T>({
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		});

	using Matrix4x4f = Matrix4x4<float>;
	using Matrix4x4d = Matrix4x4<double>;

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4()
		: m11(1), m12(0), m13(0), m14(0),
		m21(0), m22(1), m23(0), m24(0),
		m31(0), m32(0), m33(1), m34(0),
		m41(0), m42(0), m43(0), m44(1)
	{
	}

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4(const T(&aValues)[4 * 4])
	{
		memcpy(myValues, aValues, sizeof(myValues));
	}

	template<typename T>
	inline Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
	{
		operator=(aMatrix);
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const T aAngleInRadians)
	{
		Matrix4x4<T> matrix = Identity;

		const auto cosTheta = cos(aAngleInRadians);
		const auto sinTheta = sin(aAngleInRadians);

		matrix.m22 = cosTheta;
		matrix.m23 = sinTheta;

		matrix.m32 = -sinTheta;
		matrix.m33 = cosTheta;

		return matrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const T aAngleInRadians)
	{
		Matrix4x4<T> matrix = Identity;

		const auto cosTheta = cos(aAngleInRadians);
		const auto sinTheta = sin(aAngleInRadians);

		matrix.m11 = cosTheta;
		matrix.m13 = -sinTheta;

		matrix.m31 = sinTheta;
		matrix.m33 = cosTheta;

		return matrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const T aAngleInRadians)
	{
		Matrix4x4<T> matrix = Identity;

		const auto cosTheta = cos(aAngleInRadians);
		const auto sinTheta = sin(aAngleInRadians);

		matrix.m11 = cosTheta;
		matrix.m12 = sinTheta;

		matrix.m21 = -sinTheta;
		matrix.m22 = cosTheta;

		return matrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateTranslation(const Vector3<T> aTranslation)
	{
		auto result = Matrix4x4<T>::Identity;
		result.myW = aTranslation;

		return result;
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateScale(const float aScale)
	{
		return Matrix4x4<T>({
			aScale, 0.f, 0.f, 0.f,
			0.f, aScale, 0.f, 0.f,
			0.f, 0.f, aScale, 0.f,
			0.f, 0.f, 0.f, 1.f
			});
	}

	template <typename T>
	inline Matrix4x4<T> Matrix4x4<T>::CreateScale(const CU::Vector3<T> aScale)
	{
		return Matrix4x4<T>({
			aScale.x, 0.f, 0.f, 0.f,
			0.f, aScale.y, 0.f, 0.f,
			0.f, 0.f, aScale.z, 0.f,
			0.f, 0.f, 0.f, 1.f
			});
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
	{
		Matrix4x4<T> matrix;

		// Copy the diagonal
		matrix.m11 = aMatrixToTranspose.m11;
		matrix.m22 = aMatrixToTranspose.m22;
		matrix.m33 = aMatrixToTranspose.m33;
		matrix.m44 = aMatrixToTranspose.m44;

		// Swap the rest
		matrix.m21 = aMatrixToTranspose.m12;
		matrix.m12 = aMatrixToTranspose.m21;

		matrix.m31 = aMatrixToTranspose.m13;
		matrix.m13 = aMatrixToTranspose.m31;

		matrix.m41 = aMatrixToTranspose.m14;
		matrix.m14 = aMatrixToTranspose.m41;

		matrix.m32 = aMatrixToTranspose.m23;
		matrix.m23 = aMatrixToTranspose.m32;

		matrix.m24 = aMatrixToTranspose.m42;
		matrix.m42 = aMatrixToTranspose.m24;

		matrix.m34 = aMatrixToTranspose.m43;
		matrix.m43 = aMatrixToTranspose.m34;

		return matrix;
	}

	template<typename T>
	inline Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
	{
		Matrix4x4<T> result = Matrix4x4<T>::Zeroed;

		// NOTE: Cannot use Matrix3x3.hpp here because of dependency recursion
		// So an ugly solution it is :(

		// Transpose the 3x3 rotation matrix part
		// Copy the diagonal
		result.m11 = aTransform.m11;
		result.m22 = aTransform.m22;
		result.m33 = aTransform.m33;

		// Swap the rest
		result.m21 = aTransform.m12;
		result.m12 = aTransform.m21;

		result.m31 = aTransform.m13;
		result.m13 = aTransform.m31;

		result.m32 = aTransform.m23;
		result.m23 = aTransform.m32;

		// Copy the right column
		result.m14 = aTransform.m14;
		result.m24 = aTransform.m24;
		result.m34 = aTransform.m34;
		result.m44 = aTransform.m44;

		const auto translation = aTransform.GetTranslation() * static_cast<T>(-1);

		const Vector3<T> newTranslationVec;
		result.m41 = (translation.x * result.m11) + (translation.y * result.m21) + (translation.z * result.m31);
		result.m42 = (translation.x * result.m12) + (translation.y * result.m22) + (translation.z * result.m32);
		result.m43 = (translation.x * result.m13) + (translation.y * result.m23) + (translation.z * result.m33);

		return result;
	}

	template<typename T>
	inline Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& aRhs)
	{
		memcpy(myValues, aRhs.myValues, sizeof(myValues));
		return *this;
	}

	template<typename T>
	bool Matrix4x4<T>::operator==(const Matrix4x4<T>& aRhs) const
	{
		// NOTE: Check with epsilon?
		return m11 == aRhs.m11 && m12 == aRhs.m12 && m13 == aRhs.m13 && m14 == aRhs.m14 &&
			m21 == aRhs.m21 && m22 == aRhs.m22 && m23 == aRhs.m23 && m24 == aRhs.m24 &&
			m31 == aRhs.m31 && m32 == aRhs.m32 && m33 == aRhs.m33 && m34 == aRhs.m34 &&
			m41 == aRhs.m41 && m42 == aRhs.m42 && m43 == aRhs.m43 && m44 == aRhs.m44;
	}

	template<typename T>
	T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
	{
		assert(aRow > 0 && aColumn > 0 && "Out of bounds");
		assert(aRow <= 4 && aColumn <= 4 && "Out of bounds");

		return myValues[aRow - 1][aColumn - 1];
	}

	template<typename T>
	const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
	{
		assert(aRow > 0 && aColumn > 0 && "Out of bounds");
		assert(aRow <= 4 && aColumn <= 4 && "Out of bounds");

		return myValues[aRow - 1][aColumn - 1];
	}

	template<typename T>
	inline const Vector3<T>& Matrix4x4<T>::GetRight() const
	{
		return myX;
	}

	template<typename T>
	inline const Vector3<T>& Matrix4x4<T>::GetUp() const
	{
		return myY;
	}

	template<typename T>
	inline const Vector3<T>& Matrix4x4<T>::GetForward() const
	{
		return myZ;
	}

	template<typename T>
	inline const Vector3<T>& Matrix4x4<T>::GetTranslation() const
	{
		return myW;
	}

	template<typename T>
	inline Matrix4x4<T> operator+(Matrix4x4<T> aLhs, const Matrix4x4<T>& aRhs)
	{
		aLhs += aRhs;
		return aLhs;
	}

	template<typename T>
	inline Matrix4x4<T>& operator+=(Matrix4x4<T>& aLhs, const Matrix4x4<T>& aRhs)
	{
		aLhs.m11 += aRhs.m11;
		aLhs.m12 += aRhs.m12;
		aLhs.m13 += aRhs.m13;
		aLhs.m14 += aRhs.m14;

		aLhs.m21 += aRhs.m21;
		aLhs.m22 += aRhs.m22;
		aLhs.m23 += aRhs.m23;
		aLhs.m24 += aRhs.m24;

		aLhs.m31 += aRhs.m31;
		aLhs.m32 += aRhs.m32;
		aLhs.m33 += aRhs.m33;
		aLhs.m34 += aRhs.m34;

		aLhs.m41 += aRhs.m41;
		aLhs.m42 += aRhs.m42;
		aLhs.m43 += aRhs.m43;
		aLhs.m44 += aRhs.m44;

		return aLhs;
	}

	template<typename T>
	inline Matrix4x4<T> operator-(Matrix4x4<T> aLhs, const Matrix4x4<T>& aRhs)
	{
		aLhs -= aRhs;
		return aLhs;
	}

	template<typename T>
	inline Matrix4x4<T>& operator-=(Matrix4x4<T>& aLhs, const Matrix4x4<T>& aRhs)
	{
		aLhs.m11 -= aRhs.m11;
		aLhs.m12 -= aRhs.m12;
		aLhs.m13 -= aRhs.m13;
		aLhs.m14 -= aRhs.m14;

		aLhs.m21 -= aRhs.m21;
		aLhs.m22 -= aRhs.m22;
		aLhs.m23 -= aRhs.m23;
		aLhs.m24 -= aRhs.m24;

		aLhs.m31 -= aRhs.m31;
		aLhs.m32 -= aRhs.m32;
		aLhs.m33 -= aRhs.m33;
		aLhs.m34 -= aRhs.m34;

		aLhs.m41 -= aRhs.m41;
		aLhs.m42 -= aRhs.m42;
		aLhs.m43 -= aRhs.m43;
		aLhs.m44 -= aRhs.m44;

		return aLhs;
	}

	template<typename T>
	inline Matrix4x4<T> operator*(Matrix4x4<T> aLhs, const Matrix4x4<T>& aRhs)
	{
		aLhs *= aRhs;
		return aLhs;
	}

	template<typename T>
	inline Matrix4x4<T>& operator*=(Matrix4x4<T>& aLhs, const Matrix4x4<T>& aRhs)
	{
		Matrix4x4<T> result = Matrix4x4<T>::Zeroed;

		result.m11 = (aLhs.m11 * aRhs.m11) + (aLhs.m12 * aRhs.m21) + (aLhs.m13 * aRhs.m31) + (aLhs.m14 * aRhs.m41);
		result.m12 = (aLhs.m11 * aRhs.m12) + (aLhs.m12 * aRhs.m22) + (aLhs.m13 * aRhs.m32) + (aLhs.m14 * aRhs.m42);
		result.m13 = (aLhs.m11 * aRhs.m13) + (aLhs.m12 * aRhs.m23) + (aLhs.m13 * aRhs.m33) + (aLhs.m14 * aRhs.m43);
		result.m14 = (aLhs.m11 * aRhs.m14) + (aLhs.m12 * aRhs.m24) + (aLhs.m13 * aRhs.m34) + (aLhs.m14 * aRhs.m44);

		result.m21 = (aLhs.m21 * aRhs.m11) + (aLhs.m22 * aRhs.m21) + (aLhs.m23 * aRhs.m31) + (aLhs.m24 * aRhs.m41);
		result.m22 = (aLhs.m21 * aRhs.m12) + (aLhs.m22 * aRhs.m22) + (aLhs.m23 * aRhs.m32) + (aLhs.m24 * aRhs.m42);
		result.m23 = (aLhs.m21 * aRhs.m13) + (aLhs.m22 * aRhs.m23) + (aLhs.m23 * aRhs.m33) + (aLhs.m24 * aRhs.m43);
		result.m24 = (aLhs.m21 * aRhs.m14) + (aLhs.m22 * aRhs.m24) + (aLhs.m23 * aRhs.m34) + (aLhs.m24 * aRhs.m44);

		result.m31 = (aLhs.m31 * aRhs.m11) + (aLhs.m32 * aRhs.m21) + (aLhs.m33 * aRhs.m31) + (aLhs.m34 * aRhs.m41);
		result.m32 = (aLhs.m31 * aRhs.m12) + (aLhs.m32 * aRhs.m22) + (aLhs.m33 * aRhs.m32) + (aLhs.m34 * aRhs.m42);
		result.m33 = (aLhs.m31 * aRhs.m13) + (aLhs.m32 * aRhs.m23) + (aLhs.m33 * aRhs.m33) + (aLhs.m34 * aRhs.m43);
		result.m34 = (aLhs.m31 * aRhs.m14) + (aLhs.m32 * aRhs.m24) + (aLhs.m33 * aRhs.m34) + (aLhs.m34 * aRhs.m44);

		result.m41 = (aLhs.m41 * aRhs.m11) + (aLhs.m42 * aRhs.m21) + (aLhs.m43 * aRhs.m31) + (aLhs.m44 * aRhs.m41);
		result.m42 = (aLhs.m41 * aRhs.m12) + (aLhs.m42 * aRhs.m22) + (aLhs.m43 * aRhs.m32) + (aLhs.m44 * aRhs.m42);
		result.m43 = (aLhs.m41 * aRhs.m13) + (aLhs.m42 * aRhs.m23) + (aLhs.m43 * aRhs.m33) + (aLhs.m44 * aRhs.m43);
		result.m44 = (aLhs.m41 * aRhs.m14) + (aLhs.m42 * aRhs.m24) + (aLhs.m43 * aRhs.m34) + (aLhs.m44 * aRhs.m44);

		aLhs = result;

		return aLhs;
	}

	template<typename T>
	inline Vector4<T> operator*(const Vector4<T>& aLhs, const Matrix4x4<T>& aRhs)
	{
		Vector4<T> result;

		result.x = (aLhs.x * aRhs.m11) + (aLhs.y * aRhs.m21) + (aLhs.z * aRhs.m31) + (aLhs.w * aRhs.m41);
		result.y = (aLhs.x * aRhs.m12) + (aLhs.y * aRhs.m22) + (aLhs.z * aRhs.m32) + (aLhs.w * aRhs.m42);
		result.z = (aLhs.x * aRhs.m13) + (aLhs.y * aRhs.m23) + (aLhs.z * aRhs.m33) + (aLhs.w * aRhs.m43);
		result.w = (aLhs.x * aRhs.m14) + (aLhs.y * aRhs.m24) + (aLhs.z * aRhs.m34) + (aLhs.w * aRhs.m44);

		return result;
	}
}