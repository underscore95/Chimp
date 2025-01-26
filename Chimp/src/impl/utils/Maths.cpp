#include "api/utils/Maths.h"
#include "api/files/yaml/YAMLSerialiser.h"

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

	bool FloatEqual(float a, float b)
	{
		return abs(a - b) < std::numeric_limits<float>::epsilon();
	}

	void RegisterYAMLSerialisableMathsTypes(YAMLSerialiser& serialiser)
	{
		serialiser.RegisterSerialisable<Vector2f>("Vec2f", Vector2f::Deserialise);
		serialiser.RegisterSerialisable<Vector3f>("Vec3f", Vector3f::Deserialise);
		serialiser.RegisterSerialisable<Vector4f>("Vec4f", Vector4f::Deserialise);

		serialiser.RegisterSerialisable<Vector2i>("Vec2i", Vector2i::Deserialise);
		serialiser.RegisterSerialisable<Vector3i>("Vec3i", Vector3i::Deserialise);
		serialiser.RegisterSerialisable<Vector4i>("Vec4i", Vector4i::Deserialise);

		serialiser.RegisterSerialisable<Rect>("Rect", Rect::Deserialise);
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

	Matrix CreateRotationMatrixYawPitchRoll(Vector3f yawPitchRoll)
	{
		return glm::rotate(CreateIdentityMatrix(), yawPitchRoll.x, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(CreateIdentityMatrix(), yawPitchRoll.y, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(CreateIdentityMatrix(), yawPitchRoll.z, glm::vec3(0.0f, 0.0f, 1.0f));
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
		return glm::lookAt((glm::vec3)position, (glm::vec3)target, (glm::vec3)up);
	}

	Matrix CreateOrthographicProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		return glm::ortho(left, right, bottom, top, zNear, zFar);
	}
}
