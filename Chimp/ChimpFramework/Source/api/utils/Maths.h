#pragma once

#include <vector>
#include <array>
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <imgui.h>
#include "api/utils/preprocessor/Casting.h"
#include <memory>
#include "OptionalReference.h"
#include "api/utils/Json.h"
#include <string>

namespace Chimp {
	struct Vector2f;
	struct Vector3f;
	struct Vector4f;
	struct Vector2i;
	struct Vector3i;
	struct Vector4i;

	using Quaternion = glm::quat;

	// Compare two floats for equality, supports floating point error
	[[nodiscard]] bool FloatEqual(float a, float b, float epsilon = 1e-6);

#pragma region Types
#pragma region Vectors
	struct Vector2f {
		Vector2f() : x(0), y(0) {}
		Vector2f(float x, float y) : x(x), y(y) {}
		Vector2f(int x, int y) : x((float)x), y((float)y) {}
		Vector2f(double x, double y) : x((float)x), y((float)y) {}
		Vector2f(Vector2i vec);
		Vector2f(glm::vec<2, float> vec) : x(vec.x), y(vec.y) {}
		Vector2f(ImVec2 vec) : x(vec.x), y(vec.y) {}
		explicit Vector2f(Vector3f vec);
		explicit Vector2f(Vector3i vec);
		explicit Vector2f(Vector4f vec);

		operator glm::vec2() const { return glm::vec2(x, y); }
		operator ImVec2() const { return ImVec2(x, y); }
		operator Vector2i() const;

		Vector2f operator*(float scale) const {
			return Vector2f(x * scale, y * scale);
		}
		void operator*=(float scale) {
			x *= scale;
			y *= scale;
		}
		Vector2f operator/(float scale) const {
			return Vector2f(x / scale, y / scale);
		}
		void operator/=(float scale) {
			x /= scale;
			y /= scale;
		}
		Vector2f operator+(const Vector2f& other) const {
			return Vector2f(x + other.x, y + other.y);
		}
		void operator+=(const Vector2f& other) {
			x += other.x;
			y += other.y;
		}
		Vector2f operator-(const Vector2f& other) const {
			return Vector2f(x - other.x, y - other.y);
		}
		void operator-=(const Vector2f& other) {
			x -= other.x;
			y -= other.y;
		}
		bool operator==(const Vector2f& other) const {
			return FloatEqual(x, other.x) && FloatEqual(y, other.y);
		}
		bool operator!=(const Vector2f& other) const {
			return !(*this == other);
		}
		bool operator<(const Vector2f& other) const {
			return x < other.x && y < other.y;
		}
		bool operator<=(const Vector2f& other) const {
			return x <= other.x && y <= other.y;
		}
		bool operator>(const Vector2f& other) const {
			return x > other.x && y > other.y;
		}
		bool operator>=(const Vector2f& other) const {
			return x >= other.x && y >= other.y;
		}

		float x;
		float y;
	};

	struct Vector3f {
		Vector3f() : x(0), y(0), z(0) {}
		Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3f(int x, int y, int z) : x((float)x), y((float)y), z((float)z) {}
		Vector3f(double x, double y, double z) : x((float)x), y((float)y), z((float)z) {}
		Vector3f(Vector3i vec);
		explicit Vector3f(Vector2f vec, float z = 0.0f);
		explicit Vector3f(Vector4f vec);
		Vector3f(glm::vec<3, float> vec) : x(vec.x), y(vec.y), z(vec.z) {}
		explicit Vector3f(ImVec2 vec, float z = 0.0f) : x(vec.x), y(vec.y), z(z) {}

		operator glm::vec3() const { return glm::vec3(x, y, z); }

		void operator=(const Vector3f& other) {
			x = other.x;
			y = other.y;
			z = other.z;
		}

		Vector3f operator*(float scale) const {
			return Vector3f(x * scale, y * scale, z * scale);
		}
		void operator*=(float scale) {
			x *= scale;
			y *= scale;
			z *= scale;
		}
		Vector3f operator/(float scale) const {
			return Vector3f(x / scale, y / scale, z / scale);
		}
		void operator/=(float scale) {
			x /= scale;
			y /= scale;
			z /= scale;
		}
		Vector3f operator+(const Vector3f& other) const {
			return Vector3f(x + other.x, y + other.y, z + other.z);
		}
		void operator+=(const Vector3f& other) {
			x += other.x;
			y += other.y;
			z += other.z;
		}
		Vector3f operator-(const Vector3f& other) const {
			return Vector3f(x - other.x, y - other.y, z - other.z);
		}
		void operator-=(const Vector3f& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}
		bool operator==(const Vector3f& other) const {
			return FloatEqual(x, other.x) && FloatEqual(y, other.y) && FloatEqual(z, other.z);
		}
		bool operator!=(const Vector3f& other) const {
			return !(*this == other);
		}
		bool operator<(const Vector3f& other) const {
			return x < other.x && y < other.y && z < other.z;
		}
		bool operator<=(const Vector3f& other) const {
			return x <= other.x && y <= other.y && z <= other.z;
		}
		bool operator>(const Vector3f& other) const {
			return x > other.x && y > other.y && z > other.z;
		}
		bool operator>=(const Vector3f& other) const {
			return x >= other.x && y >= other.y && z >= other.z;
		}

		Vector3f operator*(const glm::quat& q) const {
			glm::vec3 rotated = q * glm::vec3(x, y, z);
			return Vector3f(rotated.x, rotated.y, rotated.z);
		}
		void operator*=(const glm::quat& q) {
			glm::vec3 rotated = q * glm::vec3(x, y, z);
			x = rotated.x;
			y = rotated.y;
			z = rotated.z;
		}

		Vector3f operator*(const glm::mat3x3& m) const {
			return m * glm::vec3(x, y, z);
		}
		void operator*=(const glm::mat3x3& m) {
			(*this) = (*this) * m;
		}

		float* Data() {
			return &x;
		}

		float x;
		float y;
		float z;
	};

	struct Vector4f {
		Vector4f() : x(0), y(0), z(0), w(0) {}
		Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vector4f(int x, int y, int z, int w) : x((float)x), y((float)y), z((float)z), w((float)w) {}
		Vector4f(double x, double y, double z, double w) : x((float)x), y((float)y), z((float)z), w((float)w) {}
		Vector4f(Vector4i vec);
		explicit Vector4f(Vector2f vec, float z = 0.0f, float w = 0.0f);
		explicit Vector4f(Vector3f vec, float w = 0.0f);
		Vector4f(glm::vec<4, float> vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
		Vector4f(ImVec2 vec, float z = 0.0f, float w = 0.0f)
			: x(vec.x), y(vec.y), z(z), w(w) {
		}

		operator glm::vec4() const { return glm::vec4(x, y, z, w); }

		void operator=(const Vector4f& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}

		Vector4f operator*(float scale) const {
			return Vector4f(x * scale, y * scale, z * scale, w * scale);
		}
		void operator*=(float scale) {
			x *= scale;
			y *= scale;
			z *= scale;
			w *= scale;
		}
		Vector4f operator/(float scale) const {
			return Vector4f(x / scale, y / scale, z / scale, w / scale);
		}
		void operator/=(float scale) {
			x /= scale;
			y /= scale;
			z /= scale;
			w /= scale;
		}
		Vector4f operator+(const Vector4f& other) const {
			return Vector4f(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		void operator+=(const Vector4f& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}
		Vector4f operator-(const Vector4f& other) const {
			return Vector4f(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		void operator-=(const Vector4f& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}
		bool operator==(const Vector4f& other) const {
			return FloatEqual(x, other.x) && FloatEqual(y, other.y) && FloatEqual(z, other.z) && FloatEqual(w, other.w);
		}
		bool operator!=(const Vector4f& other) const {
			return !(*this == other);
		}
		bool operator<(const Vector4f& other) const {
			return x < other.x && y < other.y && z < other.z && w < other.w;
		}
		bool operator<=(const Vector4f& other) const {
			return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
		}
		bool operator>(const Vector4f& other) const {
			return x > other.x && y > other.y && z > other.z && w > other.w;
		}
		bool operator>=(const Vector4f& other) const {
			return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
		}

		Vector4f operator*(const glm::mat4x4& m) const {
			return m * glm::vec4(x, y, z, w);
		}
		void operator*=(const glm::mat4x4& m) {
			(*this) = (*this) * m;
		}

		float x;
		float y;
		float z;
		float w;
	};

	struct Vector2i {
		Vector2i() : x(0), y(0) {}
		Vector2i(int x, int y) : x(x), y(y) {}
		explicit Vector2i(Vector3i vec);
		explicit Vector2i(Vector4i vec);
		explicit Vector2i(Vector2f vec);
		explicit Vector2i(Vector3f vec);
		explicit Vector2i(Vector4f vec);
		Vector2i(glm::vec<2, int> vec) : x(vec.x), y(vec.y) {}
		Vector2i(ImVec2 vec) : x((int)vec.x), y((int)vec.y) {}

		operator glm::ivec2() const { return glm::ivec2(x, y); }

		Vector2i operator*(int scale) const {
			return Vector2i(x * scale, y * scale);
		}
		void operator*=(int scale) {
			x *= scale;
			y *= scale;
		}
		Vector2i operator/(int scale) const {
			return Vector2i(x / scale, y / scale);
		}
		void operator/=(int scale) {
			x /= scale;
			y /= scale;
		}
		Vector2i operator+(const Vector2i& other) const {
			return Vector2i(x + other.x, y + other.y);
		}
		void operator+=(const Vector2i& other) {
			x += other.x;
			y += other.y;
		}
		Vector2i operator-(const Vector2i& other) const {
			return Vector2i(x - other.x, y - other.y);
		}
		void operator-=(const Vector2i& other) {
			x -= other.x;
			y -= other.y;
		}
		bool operator==(const Vector2i& other) const {
			return x == other.x && y == other.y;
		}
		bool operator!=(const Vector2i& other) const {
			return !(*this == other);
		}

		bool operator<(const Vector2i& other) const {
			return x < other.x && y < other.y;
		}
		bool operator<=(const Vector2i& other) const {
			return x <= other.x && y <= other.y;
		}
		bool operator>(const Vector2i& other) const {
			return x > other.x && y > other.y;
		}
		bool operator>=(const Vector2i& other) const {
			return x >= other.x && y >= other.y;
		}

		int x;
		int y;
	};

	struct Vector3i {
		Vector3i() : x(0), y(0), z(0) {}
		Vector3i(int x, int y, int z) : x(x), y(y), z(z) {}
		explicit Vector3i(Vector4i vec);
		explicit Vector3i(Vector2i vec, int z = 0);
		explicit Vector3i(Vector2f vec, int z = 0);
		explicit Vector3i(Vector3f vec);
		explicit Vector3i(Vector4f vec);
		Vector3i(glm::vec<3, int> vec) : x(vec.x), y(vec.y), z(vec.z) {}
		Vector3i(ImVec2 vec, int z = 0) : x((int)vec.x), y((int)vec.y), z(z) {}

		operator glm::ivec3() const { return glm::ivec3(x, y, z); }

		Vector3i operator*(int scale) const {
			return Vector3i(x * scale, y * scale, z * scale);
		}
		void operator*=(int scale) {
			x *= scale;
			y *= scale;
			z *= scale;
		}
		Vector3i operator/(int scale) const {
			return Vector3i(x / scale, y / scale, z / scale);
		}
		void operator/=(int scale) {
			x /= scale;
			y /= scale;
			z /= scale;
		}
		Vector3i operator+(const Vector3i& other) const {
			return Vector3i(x + other.x, y + other.y, z + other.z);
		}
		void operator+=(const Vector3i& other) {
			x += other.x;
			y += other.y;
			z += other.z;
		}
		Vector3i operator-(const Vector3i& other) const {
			return Vector3i(x - other.x, y - other.y, z - other.z);
		}
		void operator-=(const Vector3i& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}
		bool operator==(const Vector3i& other) const {
			return x == other.x && y == other.y && z == other.z;
		}
		bool operator!=(const Vector3i& other) const {
			return !(*this == other);
		}

		bool operator<(const Vector3i& other) const {
			return x < other.x && y < other.y && z < other.z;
		}
		bool operator<=(const Vector3i& other) const {
			return x <= other.x && y <= other.y && z <= other.z;
		}
		bool operator>(const Vector3i& other) const {
			return x > other.x && y > other.y && z > other.z;
		}
		bool operator>=(const Vector3i& other) const {
			return x >= other.x && y >= other.y && z >= other.z;
		}

		int x;
		int y;
		int z;
	};

	struct Vector4i {
		Vector4i() : x(0), y(0), z(0), w(0) {}
		Vector4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
		explicit Vector4i(Vector3i vec, int w = 0);
		explicit Vector4i(Vector2i vec, int z = 0, int w = 0);
		explicit Vector4i(Vector2f vec, int z = 0, int w = 0);
		explicit Vector4i(Vector3f vec);
		explicit Vector4i(Vector4f vec);
		Vector4i(glm::vec<4, int> vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
		Vector4i(ImVec2 vec, int z = 0, int w = 0) : x((int)vec.x), y((int)vec.y), z(z), w(w) {}

		operator glm::ivec4() const { return glm::ivec4(x, y, z, w); }

		Vector4i operator*(int scale) const {
			return Vector4i(x * scale, y * scale, z * scale, w * scale);
		}
		void operator*=(int scale) {
			x *= scale;
			y *= scale;
			z *= scale;
			w *= scale;
		}
		Vector4i operator/(int scale) const {
			return Vector4i(x / scale, y / scale, z / scale, w / scale);
		}
		void operator/=(int scale) {
			x /= scale;
			y /= scale;
			z /= scale;
			w /= scale;
		}
		Vector4i operator+(const Vector4i& other) const {
			return Vector4i(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		void operator+=(const Vector4i& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}
		Vector4i operator-(const Vector4i& other) const {
			return Vector4i(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		void operator-=(const Vector4i& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}
		bool operator==(const Vector4i& other) const {
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}
		bool operator!=(const Vector4i& other) const {
			return !(*this == other);
		}

		bool operator<(const Vector4i& other) const {
			return x < other.x && y < other.y && z < other.z && w < other.w;
		}
		bool operator<=(const Vector4i& other) const {
			return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
		}
		bool operator>(const Vector4i& other) const {
			return x > other.x && y > other.y && z > other.z && w > other.w;
		}
		bool operator>=(const Vector4i& other) const {
			return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
		}

		int x;
		int y;
		int z;
		int w;
	};

#pragma endregion

	typedef glm::mat4x4 Matrix;
	typedef glm::mat3x3 Matrix3x3;
#pragma endregion

	constexpr float PI = glm::pi<float>();
	constexpr float HALF_PI = glm::pi<float>() / 2.0f;

	struct Rect {
		Vector2f Position;
		Vector2f Size;

		Rect(Vector2f position, Vector2f size) :
			Position{ position }, Size{ size } {
			assert(size.x >= 0 && size.y >= 0);
		}

		Rect(float x, float y, float width, float height) : Rect({ x, y }, { width,height }) {

		}

		Rect(std::unique_ptr<Vector2f> position, std::unique_ptr<Vector2f> size) : Rect(*position, *size) {}

		// Check if a point is inside the rectangle
		[[nodiscard]] bool Contains(Vector2f point) const {
			return point.x >= Position.x && point.x <= Position.x + Size.x &&
				point.y >= Position.y && point.y <= Position.y + Size.y;
		}
		[[nodiscard]] bool Contains(Vector3f point) const {
			return point.x >= Position.x && point.x <= Position.x + Size.x &&
				point.y >= Position.y && point.y <= Position.y + Size.y;
		}

		// Get a position
		[[nodiscard]] Vector2f GetCenter() const {
			return Position + Size / 2.0f;
		}
		[[nodiscard]] Vector2f GetTopLeft() const {
			return Position;
		}
		[[nodiscard]] Vector2f GetTopRight() const {
			return Position + Vector2f(Size.x, 0.0f);
		}
		[[nodiscard]] Vector2f GetBottomLeft() const {
			return Position + Vector2f(0.0f, Size.y);
		}
		[[nodiscard]] Vector2f GetBottomRight() const {
			return Position + Size;
		}
		[[nodiscard]] float GetLeft() const {
			return GetTopLeft().x;
		}
		[[nodiscard]] float GetRight() const {
			return GetTopRight().x;
		}
		[[nodiscard]] float GetTop() const {
			return GetTopLeft().y;
		}
		[[nodiscard]] float GetBottom() const {
			return GetBottomLeft().y;
		}

		// Scale the rectangle around its center
		void Scale(float scale) {
			Position -= Size * (scale - 1.0f) / 2.0f;
			Size *= scale;
		}

		bool operator==(const Rect& other) const {
			return Position == other.Position && Size == other.Size;
		}
	};

	// Serialisation
	// Vector2f
	inline void to_json(Json& j, const Vector2f& e) {
		j = Json::object();
		j["x"] = e.x;
		j["y"] = e.y;
	}

	inline void from_json(const Json& j, Vector2f& e) {
		e = {
			j.at("x").get<float>(),
			j.at("y").get<float>()
		};
	}

	// Vector3f
	inline void to_json(Json& j, const Vector3f& e) {
		j = Json::object();
		j["x"] = e.x;
		j["y"] = e.y;
		j["z"] = e.z;
	}

	inline void from_json(const Json& j, Vector3f& e) {
		e = {
			j.at("x").get<float>(),
			j.at("y").get<float>(),
			j.at("z").get<float>()
		};
	}

	// Vector4f
	inline void to_json(Json& j, const Vector4f& e) {
		j = Json::object();
		j["x"] = e.x;
		j["y"] = e.y;
		j["z"] = e.z;
		j["w"] = e.w;
	}

	inline void from_json(const Json& j, Vector4f& e) {
		e = {
			j.at("x").get<float>(),
			j.at("y").get<float>(),
			j.at("z").get<float>(),
			j.at("w").get<float>()
		};
	}
}
namespace glm {
	// Quaternion
	inline void to_json(Chimp::Json& j, const glm::quat& q) {
		j = Chimp::Json::object();
		j["x"] = q.x;
		j["y"] = q.y;
		j["z"] = q.z;
		j["w"] = q.w;
	}

	inline void from_json(const Chimp::Json& j, glm::quat& q) {
		q = glm::quat{
			j.at("x").get<float>(),
			j.at("y").get<float>(),
			j.at("z").get<float>(),
			j.at("w").get<float>()
		};
	}
}

namespace Chimp {
	// Vector to string
	[[nodiscard]] std::string ToString(float v);
	[[nodiscard]] std::string ToString(Vector2f v);
	[[nodiscard]] std::string ToString(Vector3f v);
	[[nodiscard]] std::string ToString(Vector4f v);

	// Unit conversation
	[[nodiscard]] float ToRadians(float degrees);
	[[nodiscard]] float ToDegrees(float radians);

	// Cross product of two vectors
	[[nodiscard]] Vector3f VectorCrossProduct(const Vector3f& a, const Vector3f& b);

	// Normalize a vector
	[[nodiscard]] Vector3f VectorNormalized(const Vector3f& a);
	[[nodiscard]] Vector2f VectorNormalized(const Vector2f& a);

	// Create an identity matrix
	[[nodiscard]] Matrix CreateIdentityMatrix();

	// Create a translation matrix
	// translation - The translation vector
	[[nodiscard]] Matrix CreateTranslationMatrix(Vector3f translation);

	// Create a scale matrix
	// scale - The scale vector
	[[nodiscard]] Matrix CreateScaleMatrix(Vector3f scale);

	// Create rotation matrix from quaternion
	[[nodiscard]] Matrix CreateRotationMatrix(Quaternion rotation);

	// Get the forward, up and right vectors from a yaw, pitch and roll (in radians)
	[[nodiscard]] std::array<Vector3f, 3> GetForwardUpRightVectors(const Vector3f yawPitchRoll);

	// Create a view matrix
	// position - The position of the camera
	// target - The point the camera is looking at
	// up - The up vector of the camera
	[[nodiscard]] Matrix CreateViewMatrix(Vector3f position, Vector3f target, Vector3f up = Vector3f(0.0f, 1.0f, 0.0f));

	// Create an orthographic projection matrix
	// left - The coordinate of the left vertical clipping plane. This defines the minimum x-coordinate visible in the projection.
	// right - The coordinate of the right vertical clipping plane. This defines the maximum x-coordinate visible in the projection.
	// bottom - The coordinate of the bottom horizontal clipping plane. This defines the minimum y-coordinate visible in the projection.
	// top - The coordinate of the top horizontal clipping plane. This defines the maximum y-coordinate visible in the projection.
	// zNear - The Z value of the near clipping plane. This defines the minimum Z-coordinate visible in the projection.
	// zFar - The Z value of the far clipping plane. This defines the maximum Z-coordinate visible in the projection.
	[[nodiscard]] Matrix CreateOrthographicProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar);

	// Create a perspective projection matrix
	// Reversed projection matrix required for https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
	// fov - The FOV in degrees
	// aspectRatio - The aspect ratio (e.g 16/9)
	// zNear - The Z value of the near clipping plane. This defines the minimum distance visible in the projection.
	// zNear - The Z value of the far clipping plane. This defines the maximum distance visible in the projection.
	[[nodiscard]] Matrix CreatePerspectiveProjectionMatrix(float fov, float aspectRatio, float zNear, float zFar);

	// Get distance between two points
	inline float GetDistanceBetween(float a, float b) {
		return std::abs(a - b);
	}
	inline float GetDistanceBetween(Vector2f a, Vector2f b) {
		return glm::distance((glm::vec2)a, (glm::vec2)b);
	}
	inline float GetDistanceBetween(Vector3f a, Vector3f b) {
		return glm::distance((glm::vec3)a, (glm::vec3)b);
	}
	inline float GetDistanceBetween(Vector4f a, Vector4f b) {
		return glm::distance((glm::vec4)a, (glm::vec4)b);
	}

	// Power
	inline float Power(float base, float exponent) {
		return std::pow(base, exponent);
	}

	// Rounding
	inline float Floor(float value) {
		return std::floor(value);
	}

	inline float Ceil(float value) {
		return std::ceil(value);
	}

	inline float Round(float value) {
		return std::round(value);
	}

	// Log
	inline float Log(float value, float base) {
		return log(value) / log(base);
	}

	// Get squared distance between two points
	inline float GetSquaredDistanceBetween(float a, float b) {
		return (a - b) * (a - b);
	}
	inline float GetSquaredDistanceBetween(Vector2f a, Vector2f b) {
		const auto diff = a - b;
		return glm::dot((glm::vec2)diff, (glm::vec2)diff);
	}
	inline float GetSquaredDistanceBetween(Vector3f a, Vector3f b) {
		const auto diff = a - b;
		return glm::dot((glm::vec3)diff, (glm::vec3)diff);
	}
	inline float GetSquaredDistanceBetween(Vector4f a, Vector4f b) {
		const auto diff = a - b;
		return glm::dot((glm::vec4)diff, (glm::vec4)diff);
	}

	// Get length of a vector
	inline float Length(float a) {
		return std::abs(a);
	}
	inline float Length(Vector2f a) {
		return glm::length((glm::vec2)a);
	}
	inline float Length(Vector3f a) {
		return glm::length((glm::vec3)a);
	}
	inline float Length(Vector4f a) {
		return glm::length((glm::vec4)a);
	}

	// Squared length of a vector
	inline float SquaredLength(float a) {
		return a * a;
	}
	inline float SquaredLength(Vector2f a) {
		return glm::dot((glm::vec2)a, (glm::vec2)a);
	}
	inline float SquaredLength(Vector3f a) {
		return glm::dot((glm::vec3)a, (glm::vec3)a);
	}
	inline float SquaredLength(Vector4f a) {
		return glm::dot((glm::vec4)a, (glm::vec4)a);
	}

	// Is normalised
	inline bool IsNormalised(float a) {
		return FloatEqual(SquaredLength(a), 1);
	}
	inline float IsNormalised(Vector2f a) {
		return FloatEqual(SquaredLength(a), 1);
	}
	inline float IsNormalised(Vector3f a) {
		return FloatEqual(SquaredLength(a), 1);
	}
	inline float IsNormalised(Vector4f a) {
		return FloatEqual(SquaredLength(a), 1);
	}

	// Returns minimum components of two values (e.g (2,3) and (1,4) would return (1,3))
	inline float Min(float a, float b) {
		return std::min(a, b);
	}
	inline float ComponentMin(float a, float b) {
		return Min(a, b);
	}
	inline Vector2f ComponentMin(Vector2f a, Vector2f b) {
		return Vector2f(
			ComponentMin(a.x, b.x),
			ComponentMin(a.y, b.y)
		);
	}
	inline Vector3f ComponentMin(Vector3f a, Vector3f b) {
		return Vector3f(
			ComponentMin(a.x, b.x),
			ComponentMin(a.y, b.y),
			ComponentMin(a.z, b.z)
		);
	}
	inline Vector4f ComponentMin(Vector4f a, Vector4f b) {
		return Vector4f(
			ComponentMin(a.x, b.x),
			ComponentMin(a.y, b.y),
			ComponentMin(a.z, b.z),
			ComponentMin(a.w, b.w)
		);
	}

	// Returns maximum components of two values
	inline float Max(float a, float b) {
		return std::max(a, b);
	}
	inline float ComponentMax(float a, float b) {
		return Max(a, b);
	}
	inline Vector2f ComponentMax(Vector2f a, Vector2f b) {
		return Vector2f(
			ComponentMax(a.x, b.x),
			ComponentMax(a.y, b.y)
		);
	}
	inline Vector3f ComponentMax(Vector3f a, Vector3f b) {
		return Vector3f(
			ComponentMax(a.x, b.x),
			ComponentMax(a.y, b.y),
			ComponentMax(a.z, b.z)
		);
	}
	inline Vector4f ComponentMax(Vector4f a, Vector4f b) {
		return Vector4f(
			ComponentMax(a.x, b.x),
			ComponentMax(a.y, b.y),
			ComponentMax(a.z, b.z),
			ComponentMax(a.w, b.w)
		);
	}

	// Multiply components of two values
	// e.g (2,3) and (1,4) would return (2,12)
	inline float ComponentMultiply(float a, float b) {
		return a * b;
	}
	inline Vector2f ComponentMultiply(Vector2f a, Vector2f b) {
		return Vector2f(
			ComponentMultiply(a.x, b.x),
			ComponentMultiply(a.y, b.y)
		);
	}
	inline Vector3f ComponentMultiply(Vector3f a, Vector3f b) {
		return Vector3f(
			ComponentMultiply(a.x, b.x),
			ComponentMultiply(a.y, b.y),
			ComponentMultiply(a.z, b.z)
		);
	}
	inline Vector4f ComponentMultiply(Vector4f a, Vector4f b) {
		return Vector4f(
			ComponentMultiply(a.x, b.x),
			ComponentMultiply(a.y, b.y),
			ComponentMultiply(a.z, b.z),
			ComponentMultiply(a.w, b.w)
		);
	}

	// Clamp components of a value between a minimum and maximum
	inline float Clamp(float value, float min, float max) {
		return value < min ? min : value > max ? max : value;
	}
	inline float ComponentClamp(float value, float min, float max) {
		return Clamp(value, min, max);
	}
	inline Vector2f ComponentClamp(Vector2f value, Vector2f min, Vector2f max) {
		return Vector2f(
			ComponentClamp(value.x, min.x, max.x),
			ComponentClamp(value.y, min.y, max.y)
		);
	}
	inline Vector3f ComponentClamp(Vector3f value, Vector3f min, Vector3f max) {
		return Vector3f(
			ComponentClamp(value.x, min.x, max.x),
			ComponentClamp(value.y, min.y, max.y),
			ComponentClamp(value.z, min.z, max.z)
		);
	}
	inline Vector4f ComponentClamp(Vector4f value, Vector4f min, Vector4f max) {
		return Vector4f(
			ComponentClamp(value.x, min.x, max.x),
			ComponentClamp(value.y, min.y, max.y),
			ComponentClamp(value.z, min.z, max.z),
			ComponentClamp(value.w, min.w, max.w)
		);
	}

	// Lerp between two values
	// a - The start value
	// b - The end value
	// t - The interpolation value
	// Clamps t to 0 and 1
	inline float Lerp(float a, float b, float t) {
		return a + t * (b - a);
	}
	inline Vector2f Lerp(Vector2f a, Vector2f b, float t) {
		return Vector2f(
			Lerp(a.x, b.x, t),
			Lerp(a.y, b.y, t)
		);
	}
	inline Vector3f Lerp(Vector3f a, Vector3f b, float t) {
		return Vector3f(
			Lerp(a.x, b.x, t),
			Lerp(a.y, b.y, t),
			Lerp(a.z, b.z, t)
		);
	}
	inline Vector4f Lerp(Vector4f a, Vector4f b, float t) {
		return Vector4f(
			Lerp(a.x, b.x, t),
			Lerp(a.y, b.y, t),
			Lerp(a.z, b.z, t),
			Lerp(a.w, b.w, t)
		);
	}

	// Inverse lerp between two values
	// a - The start value
	// b - The end value
	// value - The value to find the interpolation value of (%, ranged 0 to 1)
	inline float InverseLerp(float a, float b, float value) {
		return (b - a) == 0.0f ? 1.0f : Clamp((value - a) / (b - a), 0.0f, 1.0f);
	}
	inline float InverseLerp(Vector2f a, Vector2f b, Vector2f value) {
		return Min(InverseLerp(a.x, b.x, value.x), InverseLerp(a.y, b.y, value.y));
	}
	inline float InverseLerp(Vector3f a, Vector3f b, Vector3f value) {
		return Min(InverseLerp(a.x, b.x, value.x), Min(InverseLerp(a.y, b.y, value.y), InverseLerp(a.z, b.z, value.z)));
	}
	inline float InverseLerp(Vector4f a, Vector4f b, Vector4f value) {
		return Min(InverseLerp(a.x, b.x, value.x), Min(InverseLerp(a.y, b.y, value.y), Min(InverseLerp(a.z, b.z, value.z), InverseLerp(a.w, b.w, value.w))));
	}

	// Find closest position to a target
	// target - The target position
	// positions - The positions to compare, cannot be empty
	// Returns the index of the closest position
	[[nodiscard]] inline size_t FindClosest(Vector2f target, const std::vector<Vector2f>& positions) {
		assert(!positions.empty());
		size_t closestIndex = 0;
		float closestDistance = GetSquaredDistanceBetween(target, positions[closestIndex]);
		for (size_t i = closestIndex + 1; i < positions.size(); ++i) {
			float distance = GetSquaredDistanceBetween(target, positions[i]);
			if (distance < closestDistance) {
				closestIndex = i;
				closestDistance = distance;
			}
		}
		return closestIndex;
	}

	// Arc tangent of two values
	// returns angle in radians
	[[nodiscard]] inline float Atan2(float y, float x) {
		return glm::atan(y, x);
	}
	[[nodiscard]] inline float Atan2(Vector2f direction) {
		return glm::atan(direction.y, direction.x);
	}

	// Dot product
	[[nodiscard]] inline float Dot(Vector2f a, Vector2f b) {
		return glm::dot((glm::vec2)a, (glm::vec2)b);
	}
	[[nodiscard]] inline float Dot(Vector3f a, Vector3f b) {
		return glm::dot((glm::vec3)a, (glm::vec3)b);
	}
	[[nodiscard]] inline float Dot(Vector4f a, Vector4f b) {
		return glm::dot((glm::vec4)a, (glm::vec4)b);
	}

	// Are two vectors collinear?
	// Returns true if a and b are parallel directions
	[[nodiscard]] inline bool IsCollinear(Vector3f a, Vector3f b) {
		float dot = Dot(a, b);
		return FloatEqual(dot * dot, SquaredLength(a) * SquaredLength(b));
	}

	[[nodiscard]] void MakeUpVectorValid(Reference<Vector3f> up, Vector3f forward);

	// Quaternions
	[[nodiscard]] std::string ToString(const Quaternion& q);

	[[nodiscard]] Quaternion ToQuatRotation(Vector3f degrees);
	[[nodiscard]] Vector3f ToEulerRotationDegrees(Quaternion rotation);

	[[nodiscard]] Quaternion CreateIdentityQuaternion();

	// Matrices
	[[nodiscard]] std::string ToString(const Matrix& m);

	[[nodiscard]] Matrix CreateTransformMatrix(const Vector3f translation, const Quaternion& quat, const Vector3f& scale);

	[[nodiscard]] Matrix3x3 To3x3(const Matrix& m);

	[[nodiscard]] Matrix3x3 Inverse(const Matrix3x3& m);

	[[nodiscard]] Matrix3x3 Transpose(const Matrix3x3& m);

	// Convert matrix to a normal matrix (for normal vectors)
	// Chimp uses world space (so only passing in model matrix) in shaders
	[[nodiscard]] Matrix3x3 ToNormalMatrix(const Matrix& m);

	[[nodiscard]] Vector3f MatrixTransform(Vector3f v, const Matrix3x3& m);
	[[nodiscard]] Vector3f MatrixTransform(Vector3f v, const Matrix& m);

	[[nodiscard]] bool IsIdentityMatrix(const Matrix& m);

	// Trig
	[[nodiscard]] inline float Cos(float degrees) {
		assert(FloatEqual(glm::cos(ToRadians(90)), 0));
		return glm::cos(ToRadians(degrees));
	}

	[[nodiscard]] inline float Sin(float degrees) {
		assert(FloatEqual(glm::sin(ToRadians(90)), 1));
		return glm::sin(ToRadians(degrees));
	}
}