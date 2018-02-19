#ifndef ENTITIES_H
#define ENTITIES_H 

#include "../shared/handles.h"

#define MAX_ENTITIES 4096
#define MAX_COMPONENTS 4096

enum component_types
{
    ComponentType_Transform,
    ComponentType_SpriteRenderer,
    
    ComponentType_Count
};

struct transform
{
    u32 id;
    vec3 position;
    r32 rotation;
    vec3 localPosition;
    r32 localRotation;
    mat4 localTransform;
    mat4 worldTransform;
    b32 isDirty;
};

struct spriteRenderer
{
    u32 id;
    b32 isActive;
    u32 spriteId;
    vec4 tintColor;
};

struct entity
{
    u32 id;
    handle handle;
    u32 components[ComponentType_Count];
};

struct entity_manager
{
    handle_manager *handles;

    u32 entitiesLength;
    entity entities[MAX_ENTITIES];

    u32 transformsLength;
    transform transforms[MAX_COMPONENTS];
};

#endif /* ENTITIES_H */
