#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "splash_helper.h"
#include "utils.h"
#include "output.h"


void
helper_init() {
    if_count = 0;
}

void
append_operand(Operand *stack, OpType type, char100 operand) {
    Operand temp;
    *stack = temp;
    (*stack).type = type;

    uuid_gen((*stack).uuid);

    switch (type) {
        case number:        strcpy((*stack).value.value, operand.value); break;
        case variable:      strcpy((*stack).name.value, operand.value); break;
        case magicVariable: strcpy((*stack).name.value, operand.value); break; break;
    }
}

void
operation_optimization(Operand *stack, char operator, Operand op1, Operand op2) {
    fprintf(stderr, "optimizing %s %c %s\n", op1.value.value, operator, op2.value.value);
    Operand new_stack;
    new_stack.type = number;

    uuid_gen(new_stack.uuid);

    double v1 = atof(op1.value.value);
    double v2 = atof(op2.value.value);
    double result;

    fprintf(stderr, "v1 = %lf\n", v1);
    fprintf(stderr, "v2 = %lf\n", v2);

    switch (operator) {
        case '+': result = v1 + v2; break;
        case '-': result = v1 - v2; break;
        case '*': result = v1 * v2; break;
        case '/': result = v1 / v2; break;
        case '^': result = pow(v1, v2); break;
    }
    fprintf(stderr, "ret = %lf\n", result);

    sprintf(new_stack.value.value, "%lf", result);
    (*stack) = new_stack;
}

int
operator_is_commutative(char operator) {
    switch (operator) {
        case '+': return 1;
        case '-': return 0;
        case '*': return 1;
        case '/': return 0;
        case '^': return 0;
        default:  return 0;
    }
}

void
append_operation(Operand *stack, char operator, Operand op1, Operand op2) {
    if (op1.type == number && op2.type == number) { // can optimize
        operation_optimization(stack, operator, op1, op2);
        return;
    }
    if (operator_is_commutative(operator) && strcmp(last_uuid, op2.uuid) == 0) {
        fprintf(stderr, "Switching op's\n");
        Operand tmp = op1;
        op1 = op2;
        op2 = tmp;
    }
    switch (op1.type) {
        case number:        output_number(stdout, op1); break;
        case variable:      output_get_variable(stdout, op1); break;
        case magicVariable: output_get_magic_variable(stdout, op1); break;
    }

    char uuid[37];
    uuid_gen(uuid);
    output_operation(stdout, operator, op2, uuid);

    Operand new_stack;
    new_stack.type = magicVariable;

    char name[] = "Calculation Result";
    strcpy(new_stack.name.value, name);
    strcpy(new_stack.uuid, uuid);

    *stack = new_stack;
}

void
append_minus_op(Operand *stack, Operand op) {
    Operand temp;
    temp.type = number;
    uuid_gen(temp.uuid);
    char minus_one[] = "-1";
    strcpy(temp.value.value, minus_one);

    append_operation(stack, '*', temp, op);
}

void
append_comparison(Comparison *stack, CompOp operator, Operand op1, Operand op2) {
    Comparison comp;
    comp.operator = operator;
    comp.op1 = op1;
    comp.op2 = op2;

    *stack = comp;
}

void
append_conditional(Comparison) {
    Operand number;
    number.type = number;
    strcpy(number.value.value, "0");
    uuid_gen(number.uuid);
    output_number(stdout, number);

    char splash_if_n[100];
    sprintf(splash_if_n, "$splash_if_%d", if_count);
    output_set_variable(stdout, splash_if_n);
}
