#ifndef SPLASH_HELPER_H
#define SPLASH_HELPER_H

#include <stdio.h>
#include "structures/structures.h"

typedef struct {
    String *name;
    int position;
} StringToken;

typedef struct {
    String *str;
    List *tokens;
} Interpolated;  // Interpolated String

typedef union {
    char value[100];
} char100;

typedef enum {
    number,
    magicVariable,
    variable,
    ask_number,
    string,
    null
} OpType;

typedef struct {
    OpType type;
    char100 value;
    char100 name;
    char uuid[37];
} Operand;

typedef enum {
    CompOpEQ,
    CompOpLT,
    /* CompOpLE,  not implemented */
    CompOpGT
    /* CompOpGE not implemented */
} CompOp;

typedef struct {
    Operand op1;
    Operand op2;
    CompOp operator;
} Comparison;

typedef struct {
    String *name;
    List *actions;
    String *parent_name;
    char last_uuid[37];
} Scope;

typedef enum {
    WF_conditional,
    WF_get_variable,
    WF_math,
    WF_number,
    WF_text,
    WF_set_variable
} ActionID;

typedef struct {
    ActionID id;
    HashTable *parameters;
    char uuid[37];  /* In case of groups, should be the same for all the actions in the group */
    Scope *sub_scope; /* for groups, like if and loop */
    int cond_control_count;
    int cond_should_close_control;
} Action;

FILE *input_file;
FILE *output_file;
Scope *current_scope;
HashTable *scopes;
int if_count;

FILE *init_parse(int argc, char *argv[]);  /* Must be called before starting parse */
void end_parse();  /* Must be called after ending parse */

void increment_if_count();

Action *action_init();
Action *action_create(ActionID id);

void action_add_subaction(Action *this, Action *other);

void append_operand(Operand *, OpType, char100);
void append_func_call(Operand *, char100);
void append_operation(Operand *, char, Operand, Operand);
void append_minus_op(Operand *, Operand);
void set_variable(char100, Operand);
void place_set_variable(char100 var_name);
void place_operand(Operand op);

void append_comparison(Comparison *, CompOp, Operand, Operand);
void append_cond_control();
void append_conditional(Comparison);
void append_else();
void close_scope();


#endif  /* SPLASH_HELPER_H */
