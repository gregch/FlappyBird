#include "Vector3.h"
#include <GL\GLU.h>
#include <sstream>

const Vector3 Vector3::down     = Vector3( 0, -1,  0);
const Vector3 Vector3::up       = Vector3( 0,  1,  0);
const Vector3 Vector3::left     = Vector3(-1,  0,  0);
const Vector3 Vector3::right    = Vector3( 1,  0,  0);
const Vector3 Vector3::forward  = Vector3( 0,  0,  1);
const Vector3 Vector3::backward = Vector3( 0,  0, -1);
const Vector3 Vector3::zero     = Vector3( 0,  0,  0);
const Vector3 Vector3::one      = Vector3( 1,  1,  1);

Vector3::Vector3()
{
	coords[0] = coords[1] = coords[2] = 0;
}

Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}

Vector3 Vector3::fromRGB(unsigned char r, unsigned char g, unsigned char b)
{
	return Vector3(r/256.0f, g/256.0f, b/256.0f);
}
	

Vector3 Vector3::random()
{
	return Vector3(rand() / (GLfloat)RAND_MAX, rand() / (GLfloat)RAND_MAX, rand() / (GLfloat)RAND_MAX);
}

GLfloat Vector3::x() const
{
	return coords[0];
}

GLfloat Vector3::y() const
{
	return coords[1];
}

GLfloat Vector3::z() const
{
	return coords[2];
}

const GLfloat *Vector3::vector() const
{
	return coords;
}

bool Vector3::operator==(const Vector3& v) const
{
	return ((coords[0] == v.coords[0]) &&
		(coords[1] == v.coords[1]) &&
		(coords[2] == v.coords[2]));
}


bool Vector3::operator!=(const Vector3& v) const
{
	return ((coords[0] != v.coords[0]) ||
		(coords[1] != v.coords[1]) ||
		(coords[2] != v.coords[2]));
}


GLfloat Vector3::length(void) const
{
	return sqrt(lengthSquared());
}


GLfloat Vector3::lengthSquared(void) const
{
	return coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2];
}


Vector3 Vector3::normalized() const
{
	return Vector3(*this) / length();
}


GLfloat Vector3::dot(const Vector3& v) const
{
	return coords[0]*v.coords[0] + coords[1]*v.coords[1] + coords[2]*v.coords[2];
}


Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3(coords[1] * v.coords[2] - coords[2] * v.coords[1],
		coords[2] * v.coords[0] - coords[0] * v.coords[2],
		coords[0] * v.coords[1] - coords[1] * v.coords[0]);
}


GLfloat Vector3::distance(const Vector3& v) const
{
	return sqrt(distanceSquared(v));
}


GLfloat Vector3::distanceSquared(const Vector3& v) const
{
	float dx, dy, dz;
	dx = coords[0] - v.coords[0];
	dy = coords[1] - v.coords[1];
	dz = coords[2] - v.coords[2];
	return dx*dx + dy*dy + dz*dz;
}

Vector3 Vector3::scale(GLfloat x, GLfloat y, GLfloat z) const 
{
	return Vector3(coords[0] * x, coords[1] * y, coords[2] * z);
}

SDL_Color Vector3::toSDLColor() const
{
	SDL_Color res = {
		(unsigned char) floorf(coords[0] * 255),
		(unsigned char) floorf(coords[1] * 255),
		(unsigned char) floorf(coords[2] * 255),
		255 };
	return res;
}

Vector3 Vector3::operator -() const
{
	return Vector3(-coords[0], -coords[1], -coords[2]);
}
Vector3 Vector3::operator +() const
{
	return Vector3(coords[0], coords[1], coords[2]);
}

Vector3 Vector3::operator +(const Vector3&v) const
{
	return Vector3(coords[0] + v.coords[0], coords[1]+v.coords[1], coords[2]+v.coords[2]);
}

Vector3& Vector3::operator +=(const Vector3&v)
{
	coords[0] += v.coords[0];
	coords[1] += v.coords[1];
	coords[2] += v.coords[2];
	return *this;
}

Vector3& Vector3::operator -=(const Vector3&v)
{
	coords[0] -= v.coords[0];
	coords[1] -= v.coords[1];
	coords[2] -= v.coords[2];
	return *this;
}

Vector3& Vector3::operator *=(GLfloat k)
{
	coords[0] *= k;
	coords[1] *= k;
	coords[2] *= k;
	return *this;
}

Vector3 Vector3::operator -(const Vector3&v) const
{
	return Vector3(coords[0] - v.coords[0], coords[1] - v.coords[1], coords[2] - v.coords[2]);
}

Vector3 Vector3::operator *(GLfloat k) const
{
	return Vector3(coords[0] * k, coords[1] * k, coords[2] * k);
}

GLfloat Vector3::operator *(const Vector3& v) const
{
	return v.dot(*this);
}

Vector3 Vector3::operator /(GLfloat k) const
{
	return Vector3(coords[0] / k, coords[1] / k, coords[2] / k);
}

Vector3& Vector3::operator /=(GLfloat k)
{
	coords[0] /= k;
	coords[1] /= k;
	coords[2] /= k;
	return *this;
}

Vector3 operator * (GLfloat k, const Vector3& v)
{
	return v * k;
}


std::string Vector3::toString() const
{
	std::stringstream ss;
	ss << "(" << coords[0] << ", " << coords[1] << ", " << coords[2] << ")";
	return ss.str();
}

std::ostream& operator <<(std::ostream& s, const Vector3& v)
{
	return s << v.toString();
}

void glRotate(GLfloat angle, const Vector3& v)
{
	glRotatef(angle, v.x(), v.y(), v.z());
}

void glTranslate(const Vector3& v)
{
	glTranslatef(v.x(), v.y(), v.z());
}

void glClearColor(const Vector3& v, GLclampf alpha)
{
	glClearColor(v.x(), v.y(), v.z(), alpha);
}

void glColor(const Vector3& v, float alpha)
{
	glColor4f(v.x(), v.y(), v.z(), alpha);
}

void glVertex(const Vector3& v)
{
	glVertex3fv(v.vector());
}

void gluLookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	gluLookAt(eye.x(), eye.y(), eye.z(), center.x(), center.y(), center.z(), up.x(), up.y(), up.z());
}

void glFog (GLenum pname, const Vector3& pvalue)
{
	glFogfv(pname, pvalue.vector());
}

void glMaterial(GLenum pname, const Vector3 &params, float alpha)
{
	glMaterial(GL_FRONT_AND_BACK, pname, params, alpha);
}

void glMaterial(GLenum face, GLenum pname, const Vector3 &params, float alpha)
{
	float paramsFloat[4];
	paramsFloat[0] = params.x();
	paramsFloat[1] = params.y();
	paramsFloat[2] = params.z();
	paramsFloat[3] = alpha;
	glMaterialfv(face, pname, paramsFloat);
}

void glLight(GLenum light, GLenum pname, const Vector3 &position, float w)
{
	float paramsFloat[4];
	paramsFloat[0] = position.x();
	paramsFloat[1] = position.y();
	paramsFloat[2] = position.z();
	paramsFloat[3] = w;
	glLightfv(light, pname, paramsFloat);
}