#include "scope.h"
#include "action.h"
#include "structures/structures.h"

#include <string.h>

void _scope_release(void *obj);

Scope *
scope_create(char *name) {
    Scope *scope = (Scope *)alloc(sizeof(Scope), _scope_release);
    scope->actions = list_init();
    scope->parent_name = NULL;

    htable_set(scopes, name, scope);

    scope_clear_last_uuid(scope);

    return scope;
}

void
scope_output(FILE *output, Scope *scope) {
    char last_uuid_output[37];
    *last_uuid_output = 0;
    LIST_LOOP(scope->actions) {
        Action *action = (Action *)node->object;
        if (strcmp(last_uuid_output, action->uuid) == 0) {
            fprintf(stderr, "Skipping action\n");
            continue;
        }

        action_output(output, action);
        strcpy(last_uuid_output, action->uuid);
    }
}

void
scope_clear_last_uuid(Scope *scope) {
    *(scope->last_uuid) = 0;
}

void
scope_add_action(Scope *scope, Action *action) {
    list_append(scope->actions, action);
    strcpy(scope->last_uuid, action->uuid);
}

void
_scope_release(void *obj) {
    Scope *scope = (Scope *)obj;

    if (scope->parent_name) {
        release(scope->parent_name);
    }
    release(scope->actions);
}
