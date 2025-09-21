#include <math.h>
#include <DxLib.h>
#include "../Utility/Utility.h"
#include "Quaternion.h"

Quaternion::Quaternion(void)
{
    w = 1;
    x = y = z = 0;
}

Quaternion::Quaternion(const VECTOR& _rad)
{
    Quaternion q = Euler(_rad.x, _rad.y, _rad.z);
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
}

Quaternion::Quaternion(double _ww, double _wx, double _wy, double _wz)
{
    w = _ww;
    x = _wx;
    y = _wy;
    z = _wz;
}

Quaternion::~Quaternion(void)
{
}

Quaternion Quaternion::Euler(const VECTOR& _rad)
{
    return Euler(_rad.x, _rad.y, _rad.z);
}

Quaternion Quaternion::Euler(double _radX, double _radY, double _radZ)
{

    Quaternion ret = Quaternion();

    _radX = Utility::RadIn2PI(_radX);
    _radY = Utility::RadIn2PI(_radY);
    _radZ = Utility::RadIn2PI(_radZ);

    double cosZ = cos(_radZ / 2.0f);
    double sinZ = sin(_radZ / 2.0f);
    double cosX = cos(_radX / 2.0f);
    double sinX = sin(_radX / 2.0f);
    double cosY = cos(_radY / 2.0f);
    double sinY = sin(_radY / 2.0f);

    //ret.w = cosZ * cosX * cosY + sinZ * sinX * sinY;
    //ret.x = sinZ * cosX * cosY - cosZ * sinX * sinY;
    //ret.y = cosZ * sinX * cosY + sinZ * cosX * sinY;
    //ret.z = cosZ * cosX * sinY - sinZ * sinX * cosY;

    ret.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
    ret.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
    ret.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
    ret.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

    return ret;

}

Quaternion Quaternion::Mult(const Quaternion& _q1, const Quaternion& _q2)
{

    Quaternion ret = Quaternion();
    double d1, d2, d3, d4;

    // wÇÃåvéZ 
    d1 = _q1.w * _q2.w;
    d2 = -_q1.x * _q2.x;
    d3 = -_q1.y * _q2.y;
    d4 = -_q1.z * _q2.z;
    ret.w = d1 + d2 + d3 + d4;

    // xÇÃåvéZ 
    d1 = _q1.w * _q2.x;
    d2 = _q2.w * _q1.x;
    d3 = _q1.y * _q2.z;
    d4 = -_q1.z * _q2.y;
    ret.x = d1 + d2 + d3 + d4;

    // yÇÃåvéZ
    d1 = _q1.w * _q2.y;
    d2 = _q2.w * _q1.y;
    d3 = _q1.z * _q2.x;
    d4 = -_q1.x * _q2.z;
    ret.y = d1 + d2 + d3 + d4;

    // zÇÃåvéZ
    d1 = _q1.w * _q2.z;
    d2 = _q2.w * _q1.z;
    d3 = _q1.x * _q2.y;
    d4 = -_q1.y * _q2.x;
    ret.z = d1 + d2 + d3 + d4;

    return ret;

}

Quaternion Quaternion::Mult(const Quaternion& _q) const
{
    return Mult(Quaternion(w, x, y, z), _q);
}

Quaternion Quaternion::AngleAxis(double _rad, VECTOR _axis)
{

    Quaternion ret = Quaternion();

    double norm;
    double c, s;

    // UnityÇ…çáÇÌÇπÇÈ
    //ret.w = ret.x = ret.y = ret.z = 0.0;
    ret.w = 1.0;
    ret.x = ret.y = ret.z = 0.0;

    norm = (double)_axis.x * (double)_axis.x + (double)_axis.y * (double)_axis.y + (double)_axis.z * (double)_axis.z;
    if (norm <= 0.0f)
    {
        return ret;
    }

    norm = 1.0 / sqrt(norm);
    _axis.x = (float)(_axis.x * norm);
    _axis.y = (float)(_axis.y * norm);
    _axis.z = (float)(_axis.z * norm);

    c = cos(0.5f * _rad);
    s = sin(0.5f * _rad);

    ret.w = c;
    ret.x = s * _axis.x;
    ret.y = s * _axis.y;
    ret.z = s * _axis.z;

    return ret;

}

VECTOR Quaternion::PosAxis(const Quaternion& _q, VECTOR _pos)
{
    // à íuèÓïÒÇ…âÒì]èÓïÒÇîΩâfÇ≥ÇπÇÈ
    // pos' = qÅEposÅEq(-1)
    Quaternion tmp = Quaternion();
    tmp = tmp.Mult(_q);
    tmp = tmp.Mult(Quaternion(0.0f, _pos.x, _pos.y, _pos.z));
    tmp = tmp.Mult(_q.Inverse());
    return { (float)tmp.x, (float)tmp.y, (float)tmp.z };
}

VECTOR Quaternion::PosAxis(VECTOR _pos) const
{
    return PosAxis(Quaternion(w, x, y, z), _pos);
}

VECTOR Quaternion::ToEuler(const Quaternion& _q)
{

    VECTOR ret;

    double r11 = 2 * (_q.x * _q.z + _q.w * _q.y);
    double r12 = _q.w * _q.w - _q.x * _q.x - _q.y * _q.y + _q.z * _q.z;
    double r21 = -2 * (_q.y * _q.z - _q.w * _q.x);
    double r31 = 2 * (_q.x * _q.y + _q.w * _q.z);
    double r32 = _q.w * _q.w - _q.x * _q.x + _q.y * _q.y - _q.z * _q.z;

    ret.x = static_cast<float>(asin(r21));
    ret.y = static_cast<float>(atan2(r11, r12));
    ret.z = static_cast<float>(atan2(r31, r32));

    return ret;

}

VECTOR Quaternion::ToEuler(void) const
{
    return ToEuler(Quaternion(w, x, y, z));
}

MATRIX Quaternion::ToMatrix(const Quaternion& _q)
{

    MATRIX mat;

    FLOAT4 fq = { (float)_q.x, (float)_q.y, (float)_q.z, (float)_q.w };

    float sx = fq.x * fq.x * 2.0f;
    float sy = fq.y * fq.y * 2.0f;
    float sz = fq.z * fq.z * 2.0f;
    float cx = fq.y * fq.z * 2.0f;
    float cy = fq.x * fq.z * 2.0f;
    float cz = fq.x * fq.y * 2.0f;
    float wx = fq.w * fq.x * 2.0f;
    float wy = fq.w * fq.y * 2.0f;
    float wz = fq.w * fq.z * 2.0f;

    mat.m[0][0] = 1.0f - (sy + sz);	mat.m[0][1] = cz + wz;			mat.m[0][2] = cy - wy;			mat.m[0][3] = 0.0f;
    mat.m[1][0] = cz - wz;			mat.m[1][1] = 1.0f - (sx + sz);	mat.m[1][2] = cx + wx;			mat.m[1][3] = 0.0f;
    mat.m[2][0] = cy + wy;			mat.m[2][1] = cx - wx;			mat.m[2][2] = 1.0f - (sx + sy);	mat.m[2][3] = 0.0f;
    mat.m[3][0] = 0.0f;				mat.m[3][1] = 0.0f;				mat.m[3][2] = 0.0f;				mat.m[3][3] = 1.0f;
    //mat.m[3][0] = trans.x;				mat.m[3][1] = trans.y;				mat.m[3][2] = trans.z;				mat.m[3][3] = 1.0f;

    return mat;

    //double sqw = q.w * q.w;
    //double sqx = q.x * q.x;
    //double sqy = q.y * q.y;
    //double sqz = q.z * q.z;
    //double invs = 1.0 / (sqx + sqy + sqz + sqw);

    //MATRIX matrix = MGetIdent();

    //matrix.m[0][0] = static_cast<float>((sqx - sqy - sqz + sqw) * invs);
    //matrix.m[1][1] = static_cast<float>((-sqx + sqy - sqz + sqw) * invs);
    //matrix.m[2][2] = static_cast<float>((-sqx - sqy + sqz + sqw) * invs);

    //double tmp1 = q.x * q.y;
    //double tmp2 = q.z * q.w;
    ////matrix.m[0][1] = static_cast<float>(2.0 * (tmp1 + tmp2) * invs);
    ////matrix.m[1][0] = static_cast<float>(2.0 * (tmp1 - tmp2) * invs);
    //matrix.m[0][1] = static_cast<float>(2.0 * (tmp1 - tmp2) * invs);
    //matrix.m[1][0] = static_cast<float>(2.0 * (tmp1 + tmp2) * invs);

    //tmp1 = q.x * q.z;
    //tmp2 = q.y * q.w;
    //matrix.m[0][2] = static_cast<float>(2.0 * (tmp1 - tmp2) * invs);
    //matrix.m[2][0] = static_cast<float>(2.0 * (tmp1 + tmp2) * invs);

    //tmp1 = q.y * q.z;
    //tmp2 = q.x * q.w;
    ////matrix.m[1][2] = static_cast<float>(2.0 * (tmp1 + tmp2) * invs);
    ////matrix.m[2][1] = static_cast<float>(2.0 * (tmp1 - tmp2) * invs);
    //matrix.m[1][2] = static_cast<float>(2.0 * (tmp1 - tmp2) * invs);
    //matrix.m[2][1] = static_cast<float>(2.0 * (tmp1 + tmp2) * invs);

    //return matrix;

}

MATRIX Quaternion::ToMatrix(void) const
{
    return ToMatrix(Quaternion(w, x, y, z));
}

Quaternion Quaternion::LookRotation(VECTOR _dir)
{
    VECTOR up = { 0.0f, 1.0f, 0.0f };
    return LookRotation(_dir, up);
}

Quaternion Quaternion::LookRotation(VECTOR _dir, VECTOR _up)
{

    _dir = Utility::VNormalize(_dir);
    VECTOR right = Utility::VNormalize(VCross(_up, _dir));
    _up = VCross(_dir, right);
    auto m00 = right.x;
    auto m01 = right.y;
    auto m02 = right.z;
    auto m10 = _up.x;
    auto m11 = _up.y;
    auto m12 = _up.z;
    auto m20 = _dir.x;
    auto m21 = _dir.y;
    auto m22 = _dir.z;


    float num8 = (m00 + m11) + m22;
    auto quaternion = Quaternion();
    if (num8 > 0.0f)
    {
        double num = sqrt(num8 + 1.0);
        quaternion.w = num * 0.5;
        num = 0.5 / num;
        quaternion.x = ((double)m12 - m21) * num;
        quaternion.y = ((double)m20 - m02) * num;
        quaternion.z = ((double)m01 - m10) * num;
        return quaternion.Normalized();
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        // xÇ∆wÇ™ãtÅH
        //auto num7 = sqrt(((1.0f + m00) - m11) - m22);
        //auto num4 = 0.5f / num7;
        //quaternion.x = 0.5f * num7;
        //quaternion.y = (m01 + m10) * num4;
        //quaternion.z = (m02 + m20) * num4;
        //quaternion.w = (m12 - m21) * num4;
        //return quaternion.Normalized();
        auto num7 = sqrt(((1.0f + m00) - m11) - m22);
        auto num4 = 0.5f / num7;
        quaternion.x = ((double)m12 - m21) * num4;
        quaternion.y = ((double)m01 + m10) * num4;
        quaternion.z = ((double)m02 + m20) * num4;
        quaternion.w = 0.5 * num7;
        return quaternion.Normalized();
    }
    if (m11 > m22)
    {
        auto num6 = sqrt(((1.0f + m11) - m00) - m22);
        auto num3 = 0.5f / num6;
        quaternion.x = ((double)m10 + m01) * num3;
        quaternion.y = 0.5 * num6;
        quaternion.z = ((double)m21 + m12) * num3;
        quaternion.w = ((double)m20 - m02) * num3;
        return quaternion.Normalized();
    }
    auto num5 = sqrt(((1.0f + m22) - m00) - m11);
    auto num2 = 0.5f / num5;
    quaternion.x = ((double)m20 + m02) * num2;
    quaternion.y = ((double)m21 + m12) * num2;
    quaternion.z = 0.5 * num5;
    quaternion.w = ((double)m01 - m10) * num2;
    return quaternion.Normalized();

}

Quaternion Quaternion::GetRotation(MATRIX _mat)
{

    Quaternion ret;

    float s;
    float tr = _mat.m[0][0] + _mat.m[1][1] + _mat.m[2][2] + 1.0f;
    if (tr >= 1.0f)
    {
        s = 0.5f / sqrtf(tr);
        ret.w = 0.25f / s;
        ret.x = (_mat.m[1][2] - _mat.m[2][1]) * s;
        ret.y = (_mat.m[2][0] - _mat.m[0][2]) * s;
        ret.z = (_mat.m[0][1] - _mat.m[1][0]) * s;
    }
    else
    {
        float max;
        max = _mat.m[1][1] > _mat.m[2][2] ? _mat.m[1][1] : _mat.m[2][2];

        if (max < _mat.m[0][0])
        {
            s = sqrtf(_mat.m[0][0] - (_mat.m[1][1] + _mat.m[2][2]) + 1.0f);

            float x = s * 0.5f;
            s = 0.5f / s;
            ret.x = x;
            ret.y = (_mat.m[0][1] + _mat.m[1][0]) * s;
            ret.z = (_mat.m[2][0] + _mat.m[0][2]) * s;
            ret.w = (_mat.m[1][2] - _mat.m[2][1]) * s;
        }
        else
            if (max == _mat.m[1][1])
            {
                s = sqrtf(_mat.m[1][1] - (_mat.m[2][2] + _mat.m[0][0]) + 1.0f);

                float y = s * 0.5f;
                s = 0.5f / s;
                ret.x = (_mat.m[0][1] + _mat.m[1][0]) * s;
                ret.y = y;
                ret.z = (_mat.m[1][2] + _mat.m[2][1]) * s;
                ret.w = (_mat.m[2][0] - _mat.m[0][2]) * s;
            }
            else
            {
                s = sqrtf(_mat.m[2][2] - (_mat.m[0][0] + _mat.m[1][1]) + 1.0f);

                float z = s * 0.5f;
                s = 0.5f / s;
                ret.x = (_mat.m[2][0] + _mat.m[0][2]) * s;
                ret.y = (_mat.m[1][2] + _mat.m[2][1]) * s;
                ret.z = z;
                ret.w = (_mat.m[0][1] - _mat.m[1][0]) * s;
            }
    }

    return ret;


    //float elem[4];
    //elem[0] = mat.m[0][0] - mat.m[1][1] - mat.m[2][2] + 1.0f;
    //elem[1] = -mat.m[0][0] + mat.m[1][1] - mat.m[2][2] + 1.0f;
    //elem[2] = -mat.m[0][0] - mat.m[1][1] + mat.m[2][2] + 1.0f;
    //elem[3] = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1.0f;

    //int biggestIdx = 0;
    //for (int i = 0; i < 4; i++)
    //{
    //    if (elem[i] > elem[biggestIdx])
    //    {
    //        biggestIdx = i;
    //    }
    //}

    //if (elem[biggestIdx] < 0)
    //{
    //    return Quaternion();
    //}

    //float q[4];
    //float v = sqrt(elem[biggestIdx]) * 0.5f;
    //q[biggestIdx] = v;
    //float mult = 0.25f / v;

    //switch (biggestIdx)
    //{
    //case 0:
    //    q[1] = (mat.m[1][0] + mat.m[0][1]) * mult;
    //    q[2] = (mat.m[0][2] + mat.m[2][0]) * mult;
    //    q[3] = (mat.m[2][1] - mat.m[1][2]) * mult;
    //    break;
    //case 1:
    //    q[0] = (mat.m[1][0] + mat.m[0][1]) * mult;
    //    q[2] = (mat.m[2][1] + mat.m[1][2]) * mult;
    //    q[3] = (mat.m[0][2] - mat.m[2][0]) * mult;
    //    break;
    //case 2:
    //    q[0] = (mat.m[0][2] + mat.m[2][0]) * mult;
    //    q[1] = (mat.m[2][1] + mat.m[1][2]) * mult;
    //    q[3] = (mat.m[1][0] - mat.m[0][1]) * mult;
    //    break;
    //case 3:
    //    q[0] = (mat.m[2][1] - mat.m[1][2]) * mult;
    //    q[1] = (mat.m[0][2] - mat.m[2][0]) * mult;
    //    q[2] = (mat.m[1][0] - mat.m[0][1]) * mult;
    //    break;
    //}

    //return Quaternion(q[3], q[0], q[1], q[2]);

}

VECTOR Quaternion::GetDir(VECTOR _dir) const
{
    return PosAxis(_dir);
}

VECTOR Quaternion::GetForward(void) const
{
    return GetDir(Utility::DIR_F);
}

VECTOR Quaternion::GetBack(void) const
{
    return GetDir(Utility::DIR_B);
}

VECTOR Quaternion::GetRight(void) const
{
    return GetDir(Utility::DIR_R);
}

VECTOR Quaternion::GetLeft(void) const
{
    return GetDir(Utility::DIR_L);
}

VECTOR Quaternion::GetUp(void) const
{
    return GetDir(Utility::DIR_U);
}

VECTOR Quaternion::GetDown(void) const
{
    return GetDir(Utility::DIR_D);
}

double Quaternion::Dot(const Quaternion& _q1, const Quaternion& _q2)
{
    return (_q1.w * _q2.w + _q1.x * _q2.x + _q1.y * _q2.y + _q1.z * _q2.z);
}

double Quaternion::Dot(const Quaternion& _q) const
{
    return (w * _q.w + x * _q.x + y * _q.y + z * _q.z);
}

Quaternion Quaternion::Normalize(const Quaternion& _q)
{
    float scale = 1.0f / static_cast<float>(_q.Length());
    VECTOR v = VScale(_q.xyz(), scale);
    Quaternion ret = Quaternion(_q.w * scale, v.x, v.y, v.z);
    return ret;
}

Quaternion Quaternion::Normalized(void) const
{

    double mag = sqrt(w * w + x * x + y * y + z * z);
    return Quaternion(w / mag, x / mag, y / mag, z / mag);

}

void Quaternion::Normalize(void)
{

    double mag = sqrt(w * w + x * x + y * y + z * z);

    w /= mag;
    x /= mag;
    y /= mag;
    z /= mag;

}

Quaternion Quaternion::Inverse(void) const
{

    double n = 1.0f / (w * w + x * x + y * y + z * z);
    Quaternion tmp = Quaternion(w, -x, -y, -z);
    return Quaternion(tmp.w * n, tmp.x * n, tmp.y * n, tmp.z * n);;

}

Quaternion Quaternion::Slerp(Quaternion _from, Quaternion _to, double _t)
{
    if (_t > 1) _t = 1;
    if (_t < 0) _t = 0;
    return SlerpUnclamped(_from, _to, (float)_t);

}

inline float SIGN(float _x) {
    return (_x >= 0.0f) ? +1.0f : -1.0f;
}

inline float NORM(float _a, float _b, float _c, float _d) {
    return sqrt(_a * _a + _b * _b + _c * _c + _d * _d);
}

Quaternion Quaternion::FromToRotation(VECTOR _fromDir, VECTOR _toDir)
{

	VECTOR axis = VCross(_fromDir, _toDir);
	double angle = Utility::AngleDeg(_fromDir, _toDir);
	if (angle >= 179.9196)
	{
		auto r = VCross(_fromDir, Utility::DIR_R);
		axis = VCross(r, _fromDir);
		//if (axis.sqrMagnitude < 0.000001f)
		float len = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
		if (len < 0.000001f)
		{
			axis = Utility::DIR_U;
		}
	}

	axis = Utility::VNormalize(axis);
	return Quaternion::AngleAxis(Utility::Deg2RadD(angle), axis);

}

Quaternion Quaternion::RotateTowards(const Quaternion& _from, const Quaternion& _to, float _maxDegreesDelta)
{
    double num = Quaternion::Angle(_from, _to);
    if (num == 0.0)
    {
        return _to;
    }
    float t = std::min(1.0f, _maxDegreesDelta / (float)num);
    return Quaternion::SlerpUnclamped(_from, _to, t);
}

double Quaternion::Angle(const Quaternion& _q1, const Quaternion& _q2)
{
	double cos = Quaternion::Dot(_q1, _q2);
    double ac = acos(cos);
	return ac * (180.0 / DX_PI);
}

Quaternion Quaternion::SlerpUnclamped(Quaternion _a, Quaternion _b, float _t)
{

    // if either input is zero, return the other.
    if (_a.LengthSquared() == 0.0f)
    {
        if (_b.LengthSquared() == 0.0f)
        {
            return Identity();
        }
        return _b;
    }
    else if (_b.LengthSquared() == 0.0f)
    {
        return _a;
    }


    float cosHalfAngle = (float)(_a.w * _b.w) + VDot(_a.xyz(), _b.xyz());

    if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
    {
        // angle = 0.0f, so just return one input.
        return _a;
    }
    else if (cosHalfAngle < 0.0f)
    {
        //b.xyz() = -b.xyz();
		_b.x = _b.x * -1.0f;
		_b.y = _b.y * -1.0f;
		_b.z = _b.z * -1.0f;
		_b.w = -_b.w;
        cosHalfAngle = -cosHalfAngle;
    }

    float blendA;
    float blendB;
    if (cosHalfAngle < 0.99f)
    {
        // do proper slerp for big angles
        float halfAngle = acosf(cosHalfAngle);
        float sinHalfAngle = sinf(halfAngle);
        float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
        blendA = sinf(halfAngle * (1.0f - _t)) * oneOverSinHalfAngle;
        blendB = sinf(halfAngle * _t) * oneOverSinHalfAngle;
    }
    else
    {
        // do lerp if angle is really small.
        blendA = 1.0f - _t;
        blendB = _t;
    }

    //Quaternion result = Quaternion(blendA * a.xyz() + blendB * b.xyz(), blendA * a.w + blendB * b.w);
    VECTOR v = VAdd(VScale(_a.xyz(), blendA), VScale(_b.xyz(), blendB));
    //Quaternion result = Quaternion(v.x, v.y, v.z, blendA * a.w + blendB * b.w);
	Quaternion result = Quaternion(blendA * _a.w + blendB * _b.w, v.x, v.y, v.z);
    if (result.LengthSquared() > 0.0f)
    {
        return Normalize(result);
    }
    else
    {
        return Identity();
    }

}

Quaternion Quaternion::Identity(void)
{
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

double Quaternion::Length(void) const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

double Quaternion::LengthSquared(void) const
{
    return x * x + y * y + z * z + w * w;
}

VECTOR Quaternion::xyz(void) const
{
    return { (float)x, (float)y, (float)z };
}

void Quaternion::ToAngleAxis(float* _angle, VECTOR* _axis)
{

	if (abs(this->w) > 1.0f)
	{
		this->Normalize();
	}
	*_angle = 2.0f * acosf((float)this->w); // angle

    if (x == 0 && y == 0 && z == 0)
    {
        *_angle = 0.0f;
    }

	float den = sqrtf(1.0f - (float)(this->w * this->w));
	if (den > 0.0001f)
	{
		//axis = q->xyz / den;
		auto v = this->xyz();
		_axis->x = v.x / den;
		_axis->y = v.y / den;
		_axis->z = v.z / den;
	}
	else
	{
		// This occurs when the angle is zero. 
		// Not a problem: just set an arbitrary normalized axis.
		*_axis = { 1.0f, 0.0f, 0.0f };
	}

}

Quaternion Quaternion::operator*(float& _f) {
    return Quaternion(w * _f, x * _f, y * _f, z * _f);
}

const Quaternion Quaternion::operator*(const float& _f) {
    return Quaternion(w * _f, x * _f, y * _f, z * _f);
}

Quaternion Quaternion::operator+(Quaternion& _rhs) {
    return Quaternion(w + _rhs.w, x + _rhs.x, y + _rhs.y, z + _rhs.z);
}

const Quaternion Quaternion::operator+(const Quaternion& _rhs) {
    return Quaternion(w + _rhs.w, x + _rhs.x, y + _rhs.y, z + _rhs.z);
}
