/**
 * @file ecs.c
 * @brief Entity Component System implementation.
 */

#include "ecs.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// Proper alignment implementation, using alignas.
#define ECS_TAG_SIZE sizeof(ECSTag)
#define ECSCL_ELEMENT_SIZE(_plist) (alignof(max_align_t) + (_plist)->component_size + ECS_TAG_SIZE)
#define ECSCL_GET(_plist, _i) \
    ((void *)((char *)(_plist)->components + (_i) * ECSCL_ELEMENT_SIZE(_plist) + alignof(max_align_t)))
#define ECS_PTAG(_p) ((ECSTag *)((char *)(_p) - ECS_TAG_SIZE))
#define ECS_TAG(_p) (*ECS_PTAG(_p))

/**
 * @brief Registers a component type with the ECS.
 * @param id Component ID.
 * @param component_size Size of the component.
 * @param ecs ECS instance.
 * @param system Component system.
 */
void _ecs_register_internal(enum ECSComponent id, size_t component_size, struct ECS *ecs, union ECSSystem system) {
    struct ComponentList list = {
        .component_size = component_size,
        .system = system
    };
    list.components = calloc(ecs->capacity, ECSCL_ELEMENT_SIZE(&list));
    assert(list.components != NULL);
    ecs->lists[id] = list;
}

/**
 * @brief Sends an event to all component systems.
 * @param self ECS instance.
 * @param event Event type.
 */
void ecs_event(struct ECS *self, enum ECSEvent event) {
    for (size_t i = 0; i <= ECSCOMPONENT_LAST; i++) {
        struct ComponentList *list = &self->lists[i];
        ECSSubscriber subscriber = list->system.subscribers[event];
        if (subscriber == NULL) {
            continue;
        }
        for (size_t j = 0; j < self->capacity; j++) {
            void *component = ECSCL_GET(list, j);
            if (ECS_TAG(component) & ECS_TAG_USED) {
                struct Entity entity = { .id = self->ids[j], .index = j, .ecs = self };
                subscriber(component, entity);
            }
        }
    }
}

/**
 * @brief Creates a new entity.
 * @param self ECS instance.
 * @return New entity.
 */
struct Entity ecs_new(struct ECS *self) {
    size_t i;
    for (i = 0; i < self->capacity; i += 64) {
        if (((u64 *)self->used)[(i / 64)] != 0xFFFFFFFFFFFFFFFF) {
            break;
        }
    }
    if (i == self->capacity) {
        size_t old_capacity = self->capacity;
        self->capacity *= 2;
        self->used = bitmap_realloc(self->used, self->capacity);
        assert(self->used != NULL);
        memset(((void *)self->used) + (BITMAP_SIZE_TO_BYTES(old_capacity)), 0, BITMAP_SIZE_TO_BYTES(self->capacity) - BITMAP_SIZE_TO_BYTES(old_capacity));
        self->ids = realloc(self->ids, self->capacity * sizeof(u64));
        assert(self->ids != NULL);
        memset(self->ids + old_capacity, 0, (self->capacity - old_capacity) * sizeof(u64));
        for (size_t j = 0; j <= ECSCOMPONENT_LAST; j++) {
            struct ComponentList *list = &self->lists[j];
            list->components = realloc(list->components, self->capacity * ECSCL_ELEMENT_SIZE(list));
            assert(list->components != NULL);
            memset(list->components + (old_capacity * ECSCL_ELEMENT_SIZE(list)), 0, (self->capacity - old_capacity) * ECSCL_ELEMENT_SIZE(list));
        }
    } else {
        for (; i < self->capacity; i++) {
            if (!bitmap_get(self->used, i)) {
                break;
            }
        }
    }
    bitmap_set(self->used, i);
    EntityId id = self->next_entity_id++;
    self->ids[i] = id;
    return (struct Entity) { .id = id, .index = i, .ecs = self };
}

/**
 * @brief Deletes an entity.
 * @param self ECS instance.
 * @param entity Entity to delete.
 */
void ecs_delete(struct ECS *self, struct Entity entity) {
    assert(bitmap_get(self->used, entity.index));
    for (size_t j = 0; j <= ECSCOMPONENT_LAST; j++) {
        struct ComponentList *list = &self->lists[j];
        ECSSubscriber destroy = list->system.destroy;
        void *component = ECSCL_GET(list, entity.index);
        *ECS_PTAG(component) &= ~ECS_TAG_USED;
        if (destroy != NULL) {
            destroy(component, entity);
        }
    }
    bitmap_clr(self->used, entity.index);
    self->ids[entity.index] = ENTITY_NONE;
}

/**
 * @brief Adds a component to an entity.
 * @param entity Entity.
 * @param component_id Component ID.
 * @param value Component value.
 */
void _ecs_add_internal(struct Entity entity, enum ECSComponent component_id, void *value) {
    struct ComponentList *list = &entity.ecs->lists[component_id];
    ECSSubscriber init = list->system.init;
    void *component = ECSCL_GET(list, entity.index);
    assert(!(ECS_TAG(component) & ECS_TAG_USED));
    *ECS_PTAG(component) |= ECS_TAG_USED;
    if (value != NULL) {
        memcpy(component, value, list->component_size);
    }
    if (init != NULL) {
        init(component, entity);
    }
}

/**
 * @brief Removes a component from an entity.
 * @param entity Entity.
 * @param component_id Component ID.
 */
void ecs_remove(struct Entity entity, enum ECSComponent component_id) {
    struct ComponentList *list = &entity.ecs->lists[component_id];
    ECSSubscriber destroy = list->system.destroy;
    void *component = ECSCL_GET(list, entity.index);
    assert(ECS_TAG(component) & ECS_TAG_USED);
    *ECS_PTAG(component) &= ~ECS_TAG_USED;
    if (destroy != NULL) {
        destroy(component, entity);
    }
}

/**
 * @brief Checks if an entity has a component.
 * @param entity Entity.
 * @param component Component ID.
 * @return True if entity has component, false otherwise.
 */
bool ecs_has(struct Entity entity, enum ECSComponent component) {
    return ECS_TAG(ECSCL_GET(&entity.ecs->lists[component], entity.index)) & ECS_TAG_USED;
}

/**
 * @brief Gets a component from an entity.
 * @param entity Entity.
 * @param component Component ID.
 * @return Component pointer.
 */
void *ecs_get(struct Entity entity, enum ECSComponent component) {
    assert(ecs_has(entity, component));
    return ECSCL_GET(&entity.ecs->lists[component], entity.index);
}

/**
 * @brief Initializes the ECS.
 * @param self ECS instance.
 * @param world World instance.
 */
void ecs_init(struct ECS *self, struct World *world) {
    self->capacity = 64;
    self->ids = calloc(self->capacity, sizeof(EntityId));
    assert(self->ids != NULL);
    self->used = bitmap_calloc(self->capacity);
    assert(self->used != NULL);
    self->next_entity_id = 1;
    self->world = world;
    _ecs_init_internal(self);
}

/**
 * @brief Destroys the ECS.
 * @param self ECS instance.
 */
void ecs_destroy(struct ECS *self) {
    free(self->used);
    free(self->ids);
    for (size_t i = 0; i <= ECSCOMPONENT_LAST; i++) {
        free(self->lists[i].components);
    }
}
