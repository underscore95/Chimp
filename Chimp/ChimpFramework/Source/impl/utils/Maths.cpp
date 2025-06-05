#include "api/utils/Maths.h"
#include "Loggers.h"
#include <glm/gtx/quaternion.hpp>

namespace Chimp {

#pragma region Types
#pragma region Vectors
	Vector2f::Vector2f(Vector3f vec) : x(vec.x), y(vec.y)
	{
	}

	Vector2f::Vector2f(Vector4f vec) : x(vec.x), y(vec.y)
	{
	}

	Vector2f::Vector2f(Vector2i vec) : x(vec.x), y(vec.y)
	{
	}
	Vector2f::operator Vector2i() const
	{
		return Vector2i(x, y);
	}

	Vector3f::Vector3f(Vector2f vec, float z) : x(vec.x), y(vec.y), z(z)
	{
	}

	Vector3f::Vector3f(Vector4f vec) : x(vec.x), y(vec.y), z(vec.z)
	{
	}

	Vector3f::Vector3f(Vector3i vec) : x(vec.x), y(vec.y), z(vec.z)
	{
	}

	Vector4f::Vector4f(Vector2f vec, float z, float w) : x(vec.x), y(vec.y), z(z), w(w)
	{
	}

	Vector4f::Vector4f(Vector3f vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w)
	{
	}

	Vector4f::Vector4f(Vector4i vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w)
	{
	}

	Vector2i::Vector2i(Vector3i vec) : x(vec.x), y(vec.y)
	{
	}

	Vector2i::Vector2i(Vector4i vec) : x(vec.x), y(vec.y)
	{
	}

	Vector2i::Vector2i(Vector2f vec) : x((int)vec.x), y((int)vec.y)
	{
	}

	Vector2i::Vector2i(Vector3f vec) : x((int)vec.x), y((int)vec.y)
	{
	}

	Vector2i::Vector2i(Vector4f vec) : x((int)vec.x), y((int)vec.y)
	{
	}

	Vector3i::Vector3i(Vector2i vec, int z) : x(vec.x), y(vec.y), z(z)
	{
	}

	Vector3i::Vector3i(Vector4i vec) : x(vec.x), y(vec.y), z(vec.z)
	{
	}

	Vector3i::Vector3i(Vector2f vec, int z) : x((int)vec.x), y((int)vec.y), z(z)
	{
	}

	Vector3i::Vector3i(Vector3f vec) : x((int)vec.x), y((int)vec.y), z((int)vec.z)
	{
	}

	Vector3i::Vector3i(Vector4f vec) : x((int)vec.x), y((int)vec.y), z((int)vec.z)
	{
	}

	Vector4i::Vector4i(Vector2i vec, int z, int w) : x(vec.x), y(vec.y), z(z), w(w)
	{
	}

	Vector4i::Vector4i(Vector3i vec, int w) : x(vec.x), y(vec.y), z(vec.z), w(w)
	{
	}

	Vector4i::Vector4i(Vector2f vec, int z, int w) : x((int)vec.x), y((int)vec.y), z(z), w(w)
	{
	}

	Vector4i::Vector4i(Vector4f vec) : x((int)vec.x), y((int)vec.y), z((int)vec.z), w((int)vec.w)
	{
	}

#pragma endregion
#pragma endregion

	bool FloatEqual(float a, float b, float epsilon)
	{
		return abs(a - b) < epsilon;
	}

	std::string ToString(float v)
	{
		return std::to_string(v);
	}

	std::string ToString(Vector2f v)
	{
		std::stringstream ss;
		ss << "Vector2f(" << v.x << ", " << v.y << ")";
		return ss.str();
	}

	std::string ToString(Vector3f v)
	{
		std::stringstream ss;
		ss << "Vector3f(" << v.x << ", " << v.y << ", " << v.z << ")";
		return ss.str();
	}

	std::string ToString(Vector4f v)
	{
		std::stringstream ss;
		ss << "Vector4f(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return ss.str();
	}

	float ToRadians(float degrees)
	{
		return glm::radians(degrees);
	}

	float ToDegrees(float radians)
	{
		return glm::degrees(radians);
	}

	Vector3f VectorCrossProduct(const Vector3f& a, const Vector3f& b)
	{
		return glm::cross((glm::vec3)a, (glm::vec3)b);
	}

	Vector3f VectorNormalized(const Vector3f& a)
	{
		return glm::normalize((glm::vec3)a);
	}

	Vector2f VectorNormalized(const Vector2f& a)
	{
		return glm::normalize((glm::vec2)a);
	}

	Matrix CreateIdentityMatrix()
	{
		return glm::identity<Matrix>();
	}

	Matrix CreateTranslationMatrix(Vector3f translation)
	{
		return glm::translate(CreateIdentityMatrix(), (glm::vec3)translation);
	}

	Matrix CreateScaleMatrix(Vector3f scale)
	{
		return glm::scale(CreateIdentityMatrix(), (glm::vec3)scale);
	}

	Matrix CreateRotationMatrix(Quaternion rotation)
	{
		return glm::toMat4(rotation);
	}

	std::array<Vector3f, 3> GetForwardUpRightVectors(const Vector3f yawPitchRoll)
	{
		// https://stackoverflow.com/questions/74672991/convert-yaw-pitch-roll-from-gyro-to-forward-up-vector-for-openal-listener-or
		// Extract yaw, pitch, and roll
		const float yaw = yawPitchRoll.x;
		const float pitch = yawPitchRoll.y;
		const float roll = yawPitchRoll.z;

		// Pre-compute trigonometric functions
		const float cosYaw = glm::cos(yaw);
		const float sinYaw = glm::sin(yaw);
		const float cosPitch = glm::cos(pitch);
		const float sinPitch = glm::sin(pitch);
		const float cosRoll = glm::cos(roll);
		const float sinRoll = glm::sin(roll);

		// Calculate forward vector
		Vector3f forward;
		forward.x = cosPitch * sinYaw;
		forward.y = sinPitch;
		forward.z = cosPitch * -cosYaw;

		// Calculate right vector
		Vector3f right;
		right.x = cosYaw * cosRoll - sinYaw * sinPitch * sinRoll;
		right.y = sinPitch;
		right.z = sinYaw * cosRoll + sinPitch * sinRoll * cosYaw;

		Vector3f up = VectorCrossProduct(right, forward);

		// Normalize the vectors
		forward = VectorNormalized(forward);
		up = VectorNormalized(up);
		right = VectorNormalized(right);

		return { forward, up, right };
	}

	Matrix CreateViewMatrix(Vector3f position, Vector3f target, Vector3f up)
	{
		return glm::lookAtRH((glm::vec3)position, (glm::vec3)target, (glm::vec3)up);
	}

	Matrix CreateOrthographicProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return glm::orthoRH(left, right, bottom, top, zNear, zFar);
	}

	Matrix CreatePerspectiveProjectionMatrix(float fov, float aspectRatio, float zNear, float zFar)
	{
		assert(zNear != 0);
		assert(zNear < zFar);
		assert(fov > 2 * PI);
		return glm::perspectiveRH(ToRadians(fov), aspectRatio, zNear, zFar);

		//// https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
		//float f = 1.0f / tan(ToRadians(fov) / 2.0f);
		//return glm::mat4(
		//	f / aspectRatio, 0.0f, 0.0f, 0.0f,
		//	0.0f, f, 0.0f, 0.0f,
		//	0.0f, 0.0f, 0.0f, -1.0f,
		//	0.0f, 0.0f, zNear, 0.0f);
	}

	void MakeUpVectorValid(Reference<Vector3f> up, Vector3f forward)
	{
		assert(IsNormalised(up.Get()));

		// Fix forward and up vectors being collinear which means we can't make a right vector
		if (IsCollinear(*up, forward)) {
			// TODO handle this better?
			if (up->x == 0) up->x += 0.001f;
			else up->y += 0.001f;
			*up = VectorNormalized(*up);
		}
	}

	std::string ToString(const Quaternion& q)
	{
		std::stringstream ss;
		ss << "Quaternion{"
			<< "w: " << q.w
			<< ", x: " << q.x
			<< ", y: " << q.y
			<< ", z: " << q.z
			<< "}";
		return ss.str();
	}

	// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	Quaternion ToQuatRotation(Vector3f degrees)
	{
		degrees.x = ToRadians(degrees.x);
		degrees.y = ToRadians(degrees.y);
		degrees.z = ToRadians(degrees.z);
		Quaternion q = glm::quat(degrees); 
		return q;
	}

	// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code_2
	Vector3f ToEulerRotationDegrees(Quaternion q)
	{
		Vector3f angles = glm::eulerAngles(q);

		// Convert to degrees
		angles.x = ToDegrees(angles.x);
		angles.y = ToDegrees(angles.y);
		angles.z = ToDegrees(angles.z);

		return angles;
	}

	Quaternion CreateIdentityQuaternion()
	{
		Quaternion q = glm::identity<glm::quat>();
		assert(q == ToQuatRotation({ 0, 0, 0 }));
		return q;
	}

	std::string ToString(const Matrix& m)
	{
		std::stringstream ss;
		ss << "Matrix{\n";
		for (int i = 0; i < 4; ++i)
		{
			ss << "  { ";
			for (int j = 0; j < 4; ++j)
			{
				ss << m[i][j];
				if (j < 3) ss << ", ";
			}
			ss << " }";
			if (i < 3) ss << ",";
			ss << "\n";
		}
		ss << "}";
		return ss.str();
	}

	Matrix CreateTransformMatrix(const Vector3f translation, const Quaternion& quat, const Vector3f& scale)
	{
		Matrix transformMatrix = CreateRotationMatrix(quat);
		transformMatrix[0] *= scale.x;
		transformMatrix[1] *= scale.y;
		transformMatrix[2] *= scale.z;
		transformMatrix[3] = glm::vec4(translation.x, translation.y, translation.z, 1.0f);

		return transformMatrix;
	}

	Matrix3x3 To3x3(const Matrix& m)
	{
		return m;
	}

	Matrix3x3 Inverse(const Matrix3x3& m)
	{
		return glm::inverse(m);
	}

	Matrix3x3 Transpose(const Matrix3x3& m)
	{
		return glm::transpose(m);
	}

	Matrix3x3 ToNormalMatrix(const Matrix& m)
	{
		return Transpose(Inverse(To3x3(m)));
	}

	Vector3f MatrixTransform(Vector3f v, const Matrix3x3& m)
	{
		Vector4f transformed = Vector4f(v, 1) * m;
		assert(transformed.w != 0.0f);
		return Vector3f(transformed) / transformed.w;
	}

	Vector3f MatrixTransform(Vector3f v, const Matrix& m)
	{
		Vector4f transformed = Vector4f(v, 1) * m;
		assert(transformed.w != 0.0f);
		return Vector3f(transformed) / transformed.w;
	}

	bool IsIdentityMatrix(const Matrix& m)
	{
		for (int y = 0; y < m.length(); ++y) {
			for (int x = 0; x < m.length(); ++x) {
				if (!FloatEqual(m[x][y], (float)(x == y))) return false;
			}
		}
		return true;
	}
}
