#ifndef MATH_H
#define MATH_H 

#define PI32 3.14159265358979323846f
#define TAU32 6.28318530717958647692f

union v2
{
    struct
    {
        r32 x, y;
    };
    struct
    {
        r32 u, v;
    };
    r32 elements[2];
};

union v3
{
    struct
    {
        r32 x, y, z;
    };
    struct
    {
        r32 u, v, w;
    };
    struct
    {
        r32 r, g, b;
    };
    struct
    {
        v2 xy;
        r32 _reserved0_;
    };
    struct
    {
        r32 _reserved1_;
        v2 yz;
    };
    struct
    {
        v2 uv;
        r32 _reserved2_;
    };
    struct
    {
        r32 _reserved3_;
        v2 vw;
    };
    r32 elements[3];
};

union v4
{
    struct
    {
        union
        {
            v3 xyz;
            struct
            {
                r32 x, y, z;
            };
        };
        r32 w;
    };
    struct
    {
        union
        {
            v3 rgb;
            struct
            {
                r32 r, g, b;
            };
        };
        r32 a;
    };
    struct
    {
        v2 xy, zw;
    };
    struct
    {
        r32 _reserved2_;
        v2 yz;
        r32 _reserved3_;
    };
    struct
    {
        v2 rg, ba;
    };
    struct
    {
        r32 _reserved8_;
        v2 gb;
        r32 _reserved9_;
    };
    struct
    {
        r32 top;
        r32 right;
        r32 bottom;
        r32 left;
    };
    r32 elements[4];
};

union v2i
{
    struct
    {
        i32 x, y;
    };
    i32 elements[2];
};

union v3i
{
    struct
    {
        i32 x, y, z;
    };
    struct
    {
        i32 r, g, b;
    };
    struct
    {
        v2i xy;
        i32 _reserved0_;
    };
    struct
    {
        i32 _reserved1_;
        v2i yz;
    };
    i32 elements[3];
};

union v4i
{
    struct
    {
        union
        {
            v3i xyz;
            struct
            {
                i32 x, y, z;
            };
        };
        i32 w;
    };
    struct
    {
        union
        {
            v3i rgb;
            struct
            {
                i32 r, g, b;
            };
        };
        i32 a;
    };
    struct
    {
        v2i xy, zw;
    };
    struct
    {
        i32 _reserved2_;
        v2i yz;
        i32 _reserved3_;
    };
    struct
    {
        v2i rg, ba;
    };
    struct
    {
        i32 _reserved8_;
        v2i gb;
        i32 _reserved9_;
    };
    struct
    {
        i32 top;
        i32 right;
        i32 bottom;
        i32 left;
    };
    i32 elements[4];
};

union rect
{
    struct
    {
        r32 x, y, width, height;
    };
    struct
    {
        v2 min, max;
    };
    r32 elements[4];
};

union recti
{
    struct
    {
        i32 x, y, width, height;
    };
    struct
    {
        v2i min, max;
    };
    i32 elements[4];
};

inline i32 sign(i32 a)
{
    i32 result = a >= 0 ? 1 : -1;
    return result;
}

inline r32 sign(r32 a)
{
    r32 result = a >= 0 ? 1.0f : -1.0f;
    return result;
}

inline r32 sq(r32 a)
{
    r32 result = a * a;
    return result;
}

inline r32 sqrt(r32 a)
{
    r32 result = _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(a)));
    return result;
}

inline r32 abs(r32 a)
{
    r32 result = a < 0 ? -a : a;
    return result;
}

inline i32 roundToI32(r32 a)
{
    i32 result = _mm_cvtss_si32(_mm_set_ss(a));
    return result;
}

inline u32 roundToU32(r32 a)
{
    u32 result = (u32)roundToI32(a);
    return result;
}

inline i32 floorToI32(r32 a)
{
    i32 result = 0;
    if(SSE41)
    {
        result = _mm_cvtss_si32(_mm_floor_ps(_mm_set_ss(a)));
    }
    else
    {
        Assert(!"SSE41 not available. Add a fallback floorToI32 implementation!");
    }
    return result;
}

inline i32 ceilToI32(r32 a)
{
    i32 result = 0;
    if(SSE41)
    {
        result = _mm_cvtss_si32(_mm_ceil_ps(_mm_set_ss(a)));
    }
    else
    {
        Assert(!"SSE41 not available. Add a fallback ceilToI32 implementation!");
    }
    return result;
}

inline i32 truncateToI32(r32 a)
{
    i32 result = _mm_cvttss_si32(_mm_set_ss(a));
    return result;
}

inline r32 sin(r32 angle)
{
    r32 result = 0;
    NotImplemented;
    return result;
}

inline r32 cos(r32 angle)
{
    r32 result = 0;
    NotImplemented;
    return result;
}

inline r32 atan2(r32 angle)
{
    r32 result = 0;
    NotImplemented;
    return result;
}

inline r32 sin01(r32 t)
{
    r32 result = sin(PI32 * t);
    return result;
}

inline r32 tri01(r32 t)
{
    r32 result = 2.0f * t;
    if(result > 1.0f)
    {
        result = 2.0f - result;
    }
    return result;
}

inline r32 lerp(r32 a, r32 b, r32 t)
{
    r32 result = (1.0f - t) * a + t * b;
    return result;
}

inline r32 clamp(r32 value, r32 min, r32 max)
{
    r32 result = value;
    if(result < min)
    {
        result = min;
    }
    else if(result > max)
    {
        result = max;
    }
    return result;
}

inline r32 clamp01(r32 value)
{
    r32 result = clamp(value, 0.0f, 1.0f);
    return result;
}

inline r32 clamp01Range(r32 value, r32 min, r32 max)
{
    r32 result = 0.0f;
    r32 range = max - min;
    if(range != 0.0f)
    {
        result = clamp01((value - min) / range);
    }
    return result;
}


// v2 operators and functions

inline v2 perp(v2 a)
{
    v2 result = {-a.y, a.x};
    return result;
}

inline v2 operator*(r32 a, v2 b)
{
    v2 result = {a * b.x, a * b.y};
    return result;
}

inline v2 operator*(v2 a, r32 b)
{
    v2 result = b * a;
    return result;
}

inline v2 &operator*=(v2 &a, r32 b)
{
    a = b * a;
    return a;
}

inline v2 operator-(v2 a)
{
    v2 result = {-a.x, -a.y};
    return result;
}

inline v2 operator+(v2 a, v2 b)
{
    v2 result = {a.x + b.x, a.y + b.y};
    return result;
}

inline v2 &operator+=(v2 &a, v2 b)
{
    a = a + b;
    return a;
}

inline v2 operator-(v2 a, v2 b)
{
    v2 result = {a.x - b.x, a.y - b.y};
    return result;
}

inline v2 hadamard(v2 a, v2 b)
{
    v2 result = {a.x * b.x, a.y * b.y};
    return result;
}

inline r32 dot(v2 a, v2 b)
{
    r32 result = a.x * b.x + a.y * b.y;
    return result;
}

inline r32 lengthSq(v2 a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(v2 a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}

inline v2 normalize(v2 a)
{
    v2 result = a * (1.0f / length(a));
    return result;
}

inline v2 lerp(v2 a, v2 b, r32 t)
{
    v2 result = (1.0f - t) * a + t * b;
    return result;
}


// v3 operators and functions

inline v3 operator*(r32 a, v3 b)
{
    v3 result = {a * b.x, a * b.y, a * b.z};
    return result;
}

inline v3 operator*(v3 a, r32 b)
{
    v3 result = b * a;
    return result;
}

inline v3 &operator*=(v3 &a, r32 b)
{
    a = b * a;
    return a;
}

inline v3 operator-(v3 a)
{
    v3 result = {-a.x, -a.y, -a.z};
    return result;
}

inline v3 operator+(v3 a, v3 b)
{
    v3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

inline v3 &operator+=(v3 &a, v3 b)
{
    a = a + b;
    return a;
}

inline v3 operator-(v3 a, v3 b)
{
    v3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

inline v3 hadamard(v3 a, v3 b)
{
    v3 result = {a.x * b.x, a.y * b.y, a.z * b.z};
    return result;
}

inline r32 dot(v3 a, v3 b)
{
    r32 result = a.x * b.x + a.y * b.y + a.z * b.z;
    return result;
}

inline v3 cross(v3 a, v3 b)
{
    v3 result = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    return result;
}

inline r32 lengthSq(v3 a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(v3 a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}

inline v3 normalize(v3 a)
{
    v3 result = a * (1.0f / length(a));
    return result;
}

inline v3 lerp(v3 a, v3 b, r32 t)
{
    v3 result = (1.0f - t) * a + t * b;
    return result;
}


// v4 operators and functions

inline v4 operator*(r32 a, v4 b)
{
    v4 result = {a * b.x, a * b.y, a * b.z, a * b.w};
    return result;
}

inline v4 operator*(v4 a, r32 b)
{
    v4 result = b * a;
    return result;
}

inline v4 &operator*=(v4 &a, r32 b)
{
    a = b * a;
    return a;
}

inline v4 operator-(v4 a)
{
    v4 result = {-a.x, -a.y, -a.z, -a.w};
    return result;
}

inline v4 operator+(v4 a, v4 b)
{
    v4 result = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return result;
}

inline v4 &operator+=(v4 &a, v4 b)
{
    a = a + b;
    return a;
}

inline v4 operator-(v4 a, v4 b)
{
    v4 result = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return result;
}

inline v4 hadamard(v4 a, v4 b)
{
    v4 result = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    return result;
}

inline r32 dot(v4 a, v4 b)
{
    r32 result = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    return result;
}

inline r32 lengthSq(v4 a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(v4 a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}

inline v4 normalize(v4 a)
{
    v4 result = a * (1.0f / length(a));
    return result;
}

inline v4 lerp(v4 a, v4 b, r32 t)
{
    v4 result = (1.0f - t) * a + t * b;
    return result;
}


// rect functions

inline v2 center(rect rect)
{
    v2 result = 0.5f * (rect.min + rect.max);
    return result;
}

inline b32 isInRectangle(rect rect, v2 point)
{
    b32 result = (point.x >= rect.min.x)
        && (point.y >= rect.min.y)
        && (point.x < rect.max.x)
        && (point.y < rect.max.y);
    return result;
}

inline b32 intersect(rect a, rect b)
{
    b32 result = !((b.max.x <= a.min.x)
            || (b.min.x >= a.max.x)
            || (b.max.y <= a.min.y)
            || (b.min.y >= a.max.y));
    return result;
}


// v2i operators and functions

inline v2i perp(v2i a)
{
    v2i result = {-a.y, a.x};
    return result;
}

inline v2i operator*(i32 a, v2i b)
{
    v2i result = {a * b.x, a * b.y};
    return result;
}

inline v2i operator*(v2i a, i32 b)
{
    v2i result = b * a;
    return result;
}

inline v2i &operator*=(v2i &a, i32 b)
{
    a = b * a;
    return a;
}

inline v2i operator-(v2i a)
{
    v2i result = {-a.x, -a.y};
    return result;
}

inline v2i operator+(v2i a, v2i b)
{
    v2i result = {a.x + b.x, a.y + b.y};
    return result;
}

inline v2i &operator+=(v2i &a, v2i b)
{
    a = a + b;
    return a;
}

inline v2i operator-(v2i a, v2i b)
{
    v2i result = {a.x - b.x, a.y - b.y};
    return result;
}

inline v2i hadamard(v2i a, v2i b)
{
    v2i result = {a.x * b.x, a.y * b.y};
    return result;
}

inline r32 dot(v2i a, v2i b)
{
    r32 result = (r32)(a.x * b.x + a.y * b.y);
    return result;
}

inline r32 lengthSq(v2i a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(v2i a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}


// v3i operators and functions

inline v3i operator*(i32 a, v3i b)
{
    v3i result = {a * b.x, a * b.y, a * b.z};
    return result;
}

inline v3i operator*(v3i a, i32 b)
{
    v3i result = b * a;
    return result;
}

inline v3i &operator*=(v3i &a, i32 b)
{
    a = b * a;
    return a;
}

inline v3i operator-(v3i a)
{
    v3i result = {-a.x, -a.y, -a.z};
    return result;
}

inline v3i operator+(v3i a, v3i b)
{
    v3i result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

inline v3i &operator+=(v3i &a, v3i b)
{
    a = a + b;
    return a;
}

inline v3i operator-(v3i a, v3i b)
{
    v3i result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

inline v3i hadamard(v3i a, v3i b)
{
    v3i result = {a.x * b.x, a.y * b.y, a.z * b.z};
    return result;
}

inline r32 dot(v3i a, v3i b)
{
    r32 result = (r32)(a.x * b.x + a.y * b.y + a.z * b.z);
    return result;
}

inline v3i cross(v3i a, v3i b)
{
    v3i result = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    return result;
}

inline r32 lengthSq(v3i a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(v3i a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}


// v4i operators and functions

inline v4i operator*(i32 a, v4i b)
{
    v4i result = {a * b.x, a * b.y, a * b.z, a * b.w};
    return result;
}

inline v4i operator*(v4i a, i32 b)
{
    v4i result = b * a;
    return result;
}

inline v4i &operator*=(v4i &a, i32 b)
{
    a = b * a;
    return a;
}

inline v4i operator-(v4i a)
{
    v4i result = {-a.x, -a.y, -a.z, -a.w};
    return result;
}

inline v4i operator+(v4i a, v4i b)
{
    v4i result = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return result;
}

inline v4i &operator+=(v4i &a, v4i b)
{
    a = a + b;
    return a;
}

inline v4i operator-(v4i a, v4i b)
{
    v4i result = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return result;
}

inline v4i hadamard(v4i a, v4i b)
{
    v4i result = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    return result;
}

inline r32 dot(v4i a, v4i b)
{
    r32 result = (r32)(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
    return result;
}

inline r32 lengthSq(v4i a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(v4i a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}


// recti functions

inline v2 center(recti rect)
{
    v2 min = {(r32)rect.min.x, (r32)rect.min.y};
    v2 max = {(r32)rect.max.x, (r32)rect.max.y};
    v2 result = 0.5f * (min + max);
    return result;
}

inline b32 isInRectangle(recti rect, v2i point)
{
    b32 result = (point.x >= rect.min.x)
        && (point.y >= rect.min.y)
        && (point.x < rect.max.x)
        && (point.y < rect.max.y);
    return result;
}

inline b32 intersect(recti a, recti b)
{
    b32 result = !((b.max.x <= a.min.x)
            || (b.min.x >= a.max.x)
            || (b.max.y <= a.min.y)
            || (b.min.y >= a.max.y));
    return result;
}

#endif /* MATH_H */
