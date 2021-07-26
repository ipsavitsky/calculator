#include "RPN.h"
/**
 * structure of an infex expression
 */
typedef struct {
    unsigned char *string_form;  ///< string with the expression
    unsigned char
        *curpointer;   ///< pointer to the current symbol(used internally)
    Var_table *v_tab;  ///< variable table of a current expression
} Expression;

/**
 * compute an expression
 * \param expr expression to compute
 * \param res result of the computation
 * \exception E_UNEXPECTED_SYMBOL Thrown in case of error in parsing a string
 * \exception E_MEM_ALLOC Thrown in case of memory allocation error
 * \exception E_UNBALANCED_RB Thrown in case of an unbalanced right bracket
 * \exception E_MISSED_OPERATOR Thrown in case two operands presented with
 * nothing between \exception may throw an exception thrown to it by a
 * Calculate_elem function \return error code
 */
int compute_expression(Expression *expr, double *res);

/**
 * initialize an expression
 * \param expr which expression to initialize
 * \param input string with which to initialize the expression
 * \exception E_MEM_ALLOC Thrown in case of memory allocation error
 * \return error code
 */
int init_expression(Expression *expr, char *input);

/**
 * finalize an expression
 * \param expr expression to finalize
 */
void finalize_expression(Expression *expr);

/**
 * add a viriable to the variable table of `expr` or change its value
 * \param expr expression to which add the variable
 * \param name name of the variable to add
 * \param num number to put in a variable
 * \exception E_MEM_ALLOC Thrown in case of memory allocation error
 * \return error code
 */
int add_variable_to_table(Expression *expr, const char *name, double num);
