#include "../arithm_func/arithm_func.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define SAFE(call)                           \
    do {                                     \
        if ((flag = call) != 0) return flag; \
    } while (0)

/**
 * add an element(a function/constamt or variable to RPN)
 * \param expression
 * \param size size of data(in bytes) + sizeof(Calculate_elem)
 * \param data data to be written to RPN
 * \param func function to be executed on data
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 */
int put_elem_in_RPN(RPN *expression, Size_elem size, void *data,
                    Calculate_elem func);

/**
 * parse the first layer of operations(+ or -)
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 *
 */
int parse_sum_sub(Expression *expr, RPN *stack_mach);

/**
 * parse the second layer of operations(* or /)
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 *
 */
int parse_mul_div(Expression *expr, RPN *stack_mach);

/**
 * parse the third layer(braces/constants/variables)
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 *
 */
int parse_fact(Expression *expr, RPN *stack_mach);

/**
 * parse the fourth layer - variables
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 */
int parse_variable(Expression *expr, RPN *stack_mach);

/**
 * parse the fourth layer - constants
 * \param expr infex expression needed to be transormed
 * \param stack_mach postfix expression to which expr transforms
 * \exception E_OVERFLOW Thrown in case of RPN overflow
 * \return error code
 */
int parse_literal(Expression *expr, RPN *stack_mach);

int put_elem_in_RPN(RPN *expression, Size_elem size, void *data,
                    Calculate_elem func) {
    if (expression->occupied + sizeof(Size_elem) + size >=
        expression->data_size) {
        return E_OVERFLOW;
    }
    struct input_data {
        Size_elem size;
        Calculate_elem f;
    };
    struct input_data dat;
    dat.size = size;
    if (dat.size % 8 != 0) dat.size += (8 - size % 8);
    dat.f = func;
    memcpy((struct input_data *)&(
               ((char *)expression->data)[expression->occupied]),
           &dat, sizeof(struct input_data));
    expression->occupied += sizeof(struct input_data);
    if (data != NULL) {
        memcpy((char *)expression->data + expression->occupied, data,
               size - sizeof(Calculate_elem));
        expression->occupied += 8;
    }
    return 0;
}

int parse_sum_sub(Expression *expr, RPN *stack_mach) {
    int flag;
    SAFE(parse_mul_div(expr, stack_mach));
    char operation;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    while ((*(expr->curpointer) == '+') || (*(expr->curpointer) == '-')) {
        operation = *(expr->curpointer++);
        SAFE(parse_mul_div(expr, stack_mach));
        switch (operation) {
            case '+':
                SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL,
                                     sum_double));
                break;
            case '-':
                SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL,
                                     sub_double));
                break;
            default:
                break;
        }
    }
    return 0;
}

int parse_mul_div(Expression *expr, RPN *stack_mach) {
    int flag;
    SAFE(parse_fact(expr, stack_mach));
    char operation;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    while ((*(expr->curpointer) == '*') || (*(expr->curpointer) == '/')) {
        operation = *(expr->curpointer++);
        SAFE(parse_fact(expr, stack_mach));
        switch (operation) {
            case '*':
                SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL,
                                     mult_double));
                break;
            case '/':
                SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL,
                                     div_double));
                break;
            default:
                break;
        }
    }
    return 0;
}

int parse_fact(Expression *expr, RPN *stack_mach) {
    int flag;
    short neg_flag = 1;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    if (*(expr->curpointer) == '-' &&
        ((expr->curpointer == expr->string_form) ||
         (*(expr->curpointer - 1) == '('))) {
        neg_flag = -1;
        ++(expr->curpointer);
    }
    if (isdigit(*(expr->curpointer)) || (*(expr->curpointer) == '.')) {
        SAFE(parse_literal(expr, stack_mach));
    } else if (*(expr->curpointer) == '(') {
        ++(expr->curpointer);
        SAFE(parse_sum_sub(expr, stack_mach));
        // FIXME: this should be here, but everything works without it
        while (isspace(*(expr->curpointer))) ++(expr->curpointer);
        if (*(expr->curpointer) != ')') return E_UNBALANCED_LB;
        ++(expr->curpointer);
        // FIXME: i have no idea why this is here but without it everything
        // breaks
        while (isspace(*(expr->curpointer))) ++(expr->curpointer);
    } else if (isalpha(*(expr->curpointer))) {
        SAFE(parse_variable(expr, stack_mach));
    } else {
        // printf("un_s = %c\n", *(expr->curpointer));
        return E_UNEXPECTED_SYMBOL;
    }
    if (neg_flag == -1)
        SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem), NULL, neg));
    return 0;
}

int parse_variable(Expression *expr, RPN *stack_mach) {
    int flag;
    int size = 0;
    // TODO: make variables ANY size
    unsigned char var[7];
    while (isalnum(*(expr->curpointer)) && (size < 6))
        var[size++] = *(expr->curpointer++);
    var[size++] = '\0';
    SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem) + size, var,
                         lookup_var));
    return 0;
}

int add_variable_to_table(Expression *expr, const char *name, double num) {
    // this is here just because this is the only place where Var_data is used
    // directly
    struct Var_data {
        char name[7];
        void *data;
        size_t data_size;
    };
    // TODO: repair to be able to change variables
    // for (int i = 0; i < expr->v_tab->var_num; ++i) {
    //     if (strcmp(name, expr->v_tab->vars[i].name) == 0){
    //         printf("what am i?\n");
    //         memcpy(expr->v_tab->vars[i].data, &num, sizeof(double));
    //     }
    //     return 0;
    // }
    Var_table *cur_table = expr->v_tab;
    ++(cur_table->var_num);
    cur_table->vars =
        realloc(cur_table->vars, cur_table->var_num * sizeof(struct Var_data));
    if (cur_table->vars == NULL) return E_MEM_ALLOC;
    size_t last_var = cur_table->var_num - 1;
    cur_table->vars[last_var].data_size = sizeof(double);
    cur_table->vars[last_var].data = malloc(sizeof(double));
    if (cur_table->vars[last_var].data == NULL) return E_MEM_ALLOC;
    memcpy(cur_table->vars[last_var].data, &num, sizeof(double));
    strcpy(cur_table->vars[last_var].name, name);
    // printf("added to table %s\n", expr->v_tab->vars[expr->v_tab->var_num -
    // 1].name);
    return 0;
}

int parse_literal(Expression *expr, RPN *stack_mach) {
    double ret = 0;
    int flag;
    // expr->curpointer = strtok(expr->curpointer, " +-*/()");
    char *proxy_ptr = NULL;
    ret = strtod(expr->curpointer, &proxy_ptr);
    // printf("%lf\n", ret);
    expr->curpointer = proxy_ptr;
    SAFE(put_elem_in_RPN(stack_mach, sizeof(Calculate_elem) + sizeof(double),
                         &ret, add_elem));
    // SAFE(put_number_in_RPN(stack_mach, ret));
    return 0;
}

int compute_expression(Expression *expr, double *res) {
    RPN stack_machine;
    int flag;
    struct input_data {
        Size_elem size;
        Calculate_elem f;
    };
    size_t rpn_estimate = strlen(expr->string_form) *
                          (sizeof(struct input_data) + sizeof(double));
    SAFE(RPN_init(&stack_machine, rpn_estimate));
    double ret = 0;

    if ((flag = parse_sum_sub(expr, &stack_machine)) != 0) {
        RPN_finalize(&stack_machine);
        return flag;
    }
    // this realloc will never do anything because we demand a smaller chunk of
    // memory so it will always return the same pointer therefore there will
    // never be an error
    stack_machine.data = realloc(stack_machine.data, stack_machine.occupied);
    stack_machine.data_size = stack_machine.occupied;
    while (isspace(*(expr->curpointer))) ++(expr->curpointer);

    if ((*(expr->curpointer) == '\0')) {
        if ((flag = RPN_compute(&stack_machine, &ret, sizeof(double),
                                expr->v_tab)) != 0) {
            RPN_finalize(&stack_machine);
            return flag;
        }
        *res = ret;
        RPN_finalize(&stack_machine);
        return 0;
    } else if (*(expr->curpointer) == ')') {
        RPN_finalize(&stack_machine);
        return E_UNBALANCED_RB;
    } else if (isalnum(*(expr->curpointer))) {
        RPN_finalize(&stack_machine);
        return E_MISSED_OPERATOR;
    }
    RPN_finalize(&stack_machine);
    // printf("un_s = %c(%d)\n", *(expr->curpointer), *(expr->curpointer));
    return E_UNEXPECTED_SYMBOL;
}

int init_expression(Expression *expr, char *input) {
    expr->string_form = strdup(input);
    if (expr->string_form == NULL) return E_MEM_ALLOC;
    expr->curpointer = expr->string_form;
    expr->v_tab = malloc(sizeof(Var_table));
    if (expr->v_tab == NULL) {
        free(expr->string_form);
        return E_MEM_ALLOC;
    }
    expr->v_tab->var_num = 0;
    expr->v_tab->vars = NULL;
    return 0;
}

void finalize_expression(Expression *expr) {
    free(expr->string_form);
    for (int i = 0; i < expr->v_tab->var_num; ++i) {
        free(expr->v_tab->vars[i].data);
    }
    free(expr->v_tab->vars);
    free(expr->v_tab);
}
