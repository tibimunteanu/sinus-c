#ifndef UTILS_H
#define UTILS_H 

inline u32
SafeTruncateToU32(u64 value)
{
    Assert(value <= 0xFFFFFFFF);
    u32 result = (u32)value;
    return result;
}

inline u16
SafeTruncateToU16(u32 value)
{
    Assert(value <= 0xFFFF);
    u16 result = (u16)value;
    return result;
}

inline i32 CompareStrings(char *a, char *b)
{
    while(*a || *b)
    {
        if(*a > *b)
        {
            return 1;
        }
        if(*a < *b)
        {
            return -1;
        }
        a++;
        b++;
    }
    return 0;
}

inline b32 StringsAreEqual(char *a, char *b)
{
    b32 result = (a == b);

    if(a && b)
    {
        while(*a && *b && (*a == *b))
        {
            ++a;
            ++b;
        }
        result = ((*a == 0) && (*b == 0));
    }

    return result;
}

inline b32 StringsAreEqual(memory_index aLength, char  *a, char *b)
{
    char *at = b;
    for(memory_index i = 0; i < aLength; i++, at++)
    {
        if((*at == 0) || (a[i] != *at))
        {
            return false;
        }
    }
    return *at == 0;
}

inline b32 StringsAreEqual(memory_index aLength, char *a, memory_index bLength, char *b)
{
    b32 result = aLength == bLength;
    if(result)
    {
        for(u32 i = 0; i < aLength; i++)
        {
            if(a[i] != b[i])
            {
                result = false;
                break;
            }
        }
    }
    return result;
}

inline b32 IsEndOfLine(char c)
{
    return c == '\n' || c == '\r';
}

inline b32 IsWhitespace(char c)
{
    return c == ' '
        || c == '\t'
        || c == '\v'
        || c == '\f'
        || IsEndOfLine(c);
}

inline char *SkipWhitespace(char *c)
{
    while(IsWhitespace(c[0]))
    {
        c += 1;
    }
    return c;
}

inline u32 StringLength(char *s)
{
    if(s == NULL) return 0;
    u32 length = 0;
    while (*s++)
    {
        length++;
    }    
    return length;
}

inline memory_index StringLengthUntilOneOf(char *s1, char *s2)
{
    return strcspn(s1, s2);
}

inline memory_index StringLengthOfOnly(char *s1, char *s2)
{
    return strspn(s1, s2);
}

inline u8 StringParseU8(char *s, char **adv)
{
    return (u8)strtoul(s, adv, 10);
}

inline u32 StringParseU32(char *s, char **adv)
{
    return (u32)strtoul(s, adv, 10);
}

inline i32 StringParseI32(char *s, char **adv)
{
    return (i32)strtol(s, adv, 10);
}

inline r32 StringParseR32(char *s, char **adv)
{
    return (r32)strtod(s, adv);
}

inline void StringMakeLowercase(char *s)
{
    u32 length = StringLength(s);
    for(u32 i = 0; i < length; i++)
    {
        if(s[i] >= 'A' && s[i] <= 'F')
        {
            s[i] += 'a' - 'A';
        }
    }
}

inline void CopyString(char *dest, char *source, memory_index length)
{
    for(i32 i = 0; i < length; i++)
    {
        *dest++ = *source++;
    }
    *dest++ = 0;
}

inline void CatStrings(char *dest,
                       memory_index sourceACount, char *sourceA,
                       memory_index sourceBCount, char *sourceB)
{
    for(i32 i = 0; i < sourceACount; i++)
    {
        *dest++ = *sourceA++;
    }
    for(i32 i = 0; i < sourceBCount; i++)
    {
        *dest++ = *sourceB++;
    }
    *dest++ = 0;
}

inline void CatStrings(char *dest, char *sourceA, char *sourceB)
{
    CatStrings(dest,
               StringLength(sourceA), sourceA,
               StringLength(sourceB), sourceB);
}

inline const char *SkipTo(const char *source, const char *target)
{
    return strstr(source, target);
}

inline b32 IsPowerOfTwo(u32 value)
{
    b32 result = value != 0 && !(value & (value - 1));
    return result;
}

#endif /* UTILS_H */
