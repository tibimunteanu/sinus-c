#ifndef MATH_H
#define MATH_H 

#define PI32 3.14159265358979323846f
#define TAU32 6.28318530717958647692f

union vec2
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

union vec3
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
        vec2 xy;
        r32 _reserved0_;
    };
    struct
    {
        r32 _reserved1_;
        vec2 yz;
    };
    struct
    {
        vec2 uv;
        r32 _reserved2_;
    };
    struct
    {
        r32 _reserved3_;
        vec2 vw;
    };
    r32 elements[3];
};

union vec4
{
    struct
    {
        union
        {
            vec3 xyz;
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
            vec3 rgb;
            struct
            {
                r32 r, g, b;
            };
        };
        r32 a;
    };
    struct
    {
        vec2 xy, zw;
    };
    struct
    {
        r32 _reserved2_;
        vec2 yz;
        r32 _reserved3_;
    };
    struct
    {
        vec2 rg, ba;
    };
    struct
    {
        r32 _reserved8_;
        vec2 gb;
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

union vec2i
{
    struct
    {
        i32 x, y;
    };
    i32 elements[2];
};

union vec3i
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
        vec2i xy;
        i32 _reserved0_;
    };
    struct
    {
        i32 _reserved1_;
        vec2i yz;
    };
    i32 elements[3];
};

union vec4i
{
    struct
    {
        union
        {
            vec3i xyz;
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
            vec3i rgb;
            struct
            {
                i32 r, g, b;
            };
        };
        i32 a;
    };
    struct
    {
        vec2i xy, zw;
    };
    struct
    {
        i32 _reserved2_;
        vec2i yz;
        i32 _reserved3_;
    };
    struct
    {
        vec2i rg, ba;
    };
    struct
    {
        i32 _reserved8_;
        vec2i gb;
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

struct mat4
{

};

union rect2
{
    struct
    {
        r32 x, y, width, height;
    };
    struct
    {
        vec2 min, max;
    };
    r32 elements[4];
};

union rect2i
{
    struct
    {
        i32 x, y, width, height;
    };
    struct
    {
        vec2i min, max;
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


// vec2 operators and functions

inline vec2 perp(vec2 a)
{
    vec2 result = {-a.y, a.x};
    return result;
}

inline vec2 operator*(r32 a, vec2 b)
{
    vec2 result = {a * b.x, a * b.y};
    return result;
}

inline vec2 operator*(vec2 a, r32 b)
{
    vec2 result = b * a;
    return result;
}

inline vec2 &operator*=(vec2 &a, r32 b)
{
    a = b * a;
    return a;
}

inline vec2 operator-(vec2 a)
{
    vec2 result = {-a.x, -a.y};
    return result;
}

inline vec2 operator+(vec2 a, vec2 b)
{
    vec2 result = {a.x + b.x, a.y + b.y};
    return result;
}

inline vec2 &operator+=(vec2 &a, vec2 b)
{
    a = a + b;
    return a;
}

inline vec2 operator-(vec2 a, vec2 b)
{
    vec2 result = {a.x - b.x, a.y - b.y};
    return result;
}

inline vec2 hadamard(vec2 a, vec2 b)
{
    vec2 result = {a.x * b.x, a.y * b.y};
    return result;
}

inline r32 dot(vec2 a, vec2 b)
{
    r32 result = a.x * b.x + a.y * b.y;
    return result;
}

inline r32 lengthSq(vec2 a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(vec2 a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}

inline vec2 normalize(vec2 a)
{
    vec2 result = a * (1.0f / length(a));
    return result;
}

inline vec2 lerp(vec2 a, vec2 b, r32 t)
{
    vec2 result = (1.0f - t) * a + t * b;
    return result;
}


// vec3 operators and functions

inline vec3 operator*(r32 a, vec3 b)
{
    vec3 result = {a * b.x, a * b.y, a * b.z};
    return result;
}

inline vec3 operator*(vec3 a, r32 b)
{
    vec3 result = b * a;
    return result;
}

inline vec3 &operator*=(vec3 &a, r32 b)
{
    a = b * a;
    return a;
}

inline vec3 operator-(vec3 a)
{
    vec3 result = {-a.x, -a.y, -a.z};
    return result;
}

inline vec3 operator+(vec3 a, vec3 b)
{
    vec3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

inline vec3 &operator+=(vec3 &a, vec3 b)
{
    a = a + b;
    return a;
}

inline vec3 operator-(vec3 a, vec3 b)
{
    vec3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

inline vec3 hadamard(vec3 a, vec3 b)
{
    vec3 result = {a.x * b.x, a.y * b.y, a.z * b.z};
    return result;
}

inline r32 dot(vec3 a, vec3 b)
{
    r32 result = a.x * b.x + a.y * b.y + a.z * b.z;
    return result;
}

inline vec3 cross(vec3 a, vec3 b)
{
    vec3 result = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    return result;
}

inline r32 lengthSq(vec3 a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(vec3 a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}

inline vec3 normalize(vec3 a)
{
    vec3 result = a * (1.0f / length(a));
    return result;
}

inline vec3 lerp(vec3 a, vec3 b, r32 t)
{
    vec3 result = (1.0f - t) * a + t * b;
    return result;
}


// vec4 operators and functions

inline vec4 operator*(r32 a, vec4 b)
{
    vec4 result = {a * b.x, a * b.y, a * b.z, a * b.w};
    return result;
}

inline vec4 operator*(vec4 a, r32 b)
{
    vec4 result = b * a;
    return result;
}

inline vec4 &operator*=(vec4 &a, r32 b)
{
    a = b * a;
    return a;
}

inline vec4 operator-(vec4 a)
{
    vec4 result = {-a.x, -a.y, -a.z, -a.w};
    return result;
}

inline vec4 operator+(vec4 a, vec4 b)
{
    vec4 result = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return result;
}

inline vec4 &operator+=(vec4 &a, vec4 b)
{
    a = a + b;
    return a;
}

inline vec4 operator-(vec4 a, vec4 b)
{
    vec4 result = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return result;
}

inline vec4 hadamard(vec4 a, vec4 b)
{
    vec4 result = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    return result;
}

inline r32 dot(vec4 a, vec4 b)
{
    r32 result = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    return result;
}

inline r32 lengthSq(vec4 a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(vec4 a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}

inline vec4 normalize(vec4 a)
{
    vec4 result = a * (1.0f / length(a));
    return result;
}

inline vec4 lerp(vec4 a, vec4 b, r32 t)
{
    vec4 result = (1.0f - t) * a + t * b;
    return result;
}


// rect2 functions

inline vec2 center(rect2 rect)
{
    vec2 result = 0.5f * (rect.min + rect.max);
    return result;
}

inline b32 isInRectangle(rect2 rect, vec2 point)
{
    b32 result = (point.x >= rect.min.x)
        && (point.y >= rect.min.y)
        && (point.x < rect.max.x)
        && (point.y < rect.max.y);
    return result;
}

inline b32 intersect(rect2 a, rect2 b)
{
    b32 result = !((b.max.x <= a.min.x)
            || (b.min.x >= a.max.x)
            || (b.max.y <= a.min.y)
            || (b.min.y >= a.max.y));
    return result;
}


// vec2i operators and functions

inline vec2i perp(vec2i a)
{
    vec2i result = {-a.y, a.x};
    return result;
}

inline vec2i operator*(i32 a, vec2i b)
{
    vec2i result = {a * b.x, a * b.y};
    return result;
}

inline vec2i operator*(vec2i a, i32 b)
{
    vec2i result = b * a;
    return result;
}

inline vec2i &operator*=(vec2i &a, i32 b)
{
    a = b * a;
    return a;
}

inline vec2i operator-(vec2i a)
{
    vec2i result = {-a.x, -a.y};
    return result;
}

inline vec2i operator+(vec2i a, vec2i b)
{
    vec2i result = {a.x + b.x, a.y + b.y};
    return result;
}

inline vec2i &operator+=(vec2i &a, vec2i b)
{
    a = a + b;
    return a;
}

inline vec2i operator-(vec2i a, vec2i b)
{
    vec2i result = {a.x - b.x, a.y - b.y};
    return result;
}

inline vec2i hadamard(vec2i a, vec2i b)
{
    vec2i result = {a.x * b.x, a.y * b.y};
    return result;
}

inline r32 dot(vec2i a, vec2i b)
{
    r32 result = (r32)(a.x * b.x + a.y * b.y);
    return result;
}

inline r32 lengthSq(vec2i a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(vec2i a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}


// vec3i operators and functions

inline vec3i operator*(i32 a, vec3i b)
{
    vec3i result = {a * b.x, a * b.y, a * b.z};
    return result;
}

inline vec3i operator*(vec3i a, i32 b)
{
    vec3i result = b * a;
    return result;
}

inline vec3i &operator*=(vec3i &a, i32 b)
{
    a = b * a;
    return a;
}

inline vec3i operator-(vec3i a)
{
    vec3i result = {-a.x, -a.y, -a.z};
    return result;
}

inline vec3i operator+(vec3i a, vec3i b)
{
    vec3i result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

inline vec3i &operator+=(vec3i &a, vec3i b)
{
    a = a + b;
    return a;
}

inline vec3i operator-(vec3i a, vec3i b)
{
    vec3i result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

inline vec3i hadamard(vec3i a, vec3i b)
{
    vec3i result = {a.x * b.x, a.y * b.y, a.z * b.z};
    return result;
}

inline r32 dot(vec3i a, vec3i b)
{
    r32 result = (r32)(a.x * b.x + a.y * b.y + a.z * b.z);
    return result;
}

inline vec3i cross(vec3i a, vec3i b)
{
    vec3i result = {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    return result;
}

inline r32 lengthSq(vec3i a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(vec3i a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}


// vec4i operators and functions

inline vec4i operator*(i32 a, vec4i b)
{
    vec4i result = {a * b.x, a * b.y, a * b.z, a * b.w};
    return result;
}

inline vec4i operator*(vec4i a, i32 b)
{
    vec4i result = b * a;
    return result;
}

inline vec4i &operator*=(vec4i &a, i32 b)
{
    a = b * a;
    return a;
}

inline vec4i operator-(vec4i a)
{
    vec4i result = {-a.x, -a.y, -a.z, -a.w};
    return result;
}

inline vec4i operator+(vec4i a, vec4i b)
{
    vec4i result = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return result;
}

inline vec4i &operator+=(vec4i &a, vec4i b)
{
    a = a + b;
    return a;
}

inline vec4i operator-(vec4i a, vec4i b)
{
    vec4i result = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return result;
}

inline vec4i hadamard(vec4i a, vec4i b)
{
    vec4i result = {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    return result;
}

inline r32 dot(vec4i a, vec4i b)
{
    r32 result = (r32)(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
    return result;
}

inline r32 lengthSq(vec4i a)
{
    r32 result = dot(a, a);
    return result;
}

inline r32 length(vec4i a)
{
    r32 result = sqrt(lengthSq(a));
    return result;
}


// rect2i functions

inline vec2 center(rect2i rect)
{
    vec2 min = {(r32)rect.min.x, (r32)rect.min.y};
    vec2 max = {(r32)rect.max.x, (r32)rect.max.y};
    vec2 result = 0.5f * (min + max);
    return result;
}

inline b32 isInRectangle(rect2i rect, vec2i point)
{
    b32 result = (point.x >= rect.min.x)
        && (point.y >= rect.min.y)
        && (point.x < rect.max.x)
        && (point.y < rect.max.y);
    return result;
}

inline b32 intersect(rect2i a, rect2i b)
{
    b32 result = !((b.max.x <= a.min.x)
            || (b.min.x >= a.max.x)
            || (b.max.y <= a.min.y)
            || (b.min.y >= a.max.y));
    return result;
}

#endif /* MATH_H */
