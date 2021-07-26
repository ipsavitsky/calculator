#include "rec_desc.h"

/**
 * add a new element to the stack machine(may be a double/string/struct of any kind)
 * \param data data needed for calculation
 * \exception E_OVERFLOW Thrown in case of RPN/Stack overflow
 * \exception E_UNDERFLOW Thrown in case of RPN/Stack underflow
 * \return error code
 */
int add_elem(Calculation_data *data);

/**
 * sum two doubles from the stack machine and put the result back to the stack
 * \param data data needed for calculation
 * \exception E_OVERFLOW Thrown in case of RPN/Stack overflow
 * \exception E_UNDERFLOW Thrown in case of RPN/Stack underflow
 * \return error code
 */
int sum_double(Calculation_data *data);

/**
 * substract two doubles from the stack machine and put the result back to the stack
 * \param data data needed for calculation
 * \exception E_OVERFLOW Thrown in case of RPN/Stack overflow
 * \exception E_UNDERFLOW Thrown in case of RPN/Stack underflow
 * \return error code
 */
int sub_double(Calculation_data *data);

/**
 * multiply two doubles from the stack machine and put the result back to the stack
 * \param data data needed for calculation
 * \exception E_OVERFLOW Thrown in case of RPN/Stack overflow
 * \exception E_UNDERFLOW Thrown in case of RPN/Stack underflow
 * \return error code
 */
int mult_double(Calculation_data *data);

/**
 * divide two doubles from the stack machine and put the result back to the stack
 * \param data data needed for calculation
 * \exception E_OVERFLOW Thrown in case of RPN/Stack overflow
 * \exception E_UNDERFLOW Thrown in case of RPN/Stack underflow
 * \exception E_ZERO_DIVISION Thrown in case there is a division by zero
 * \return error code
 */
int div_double(Calculation_data *data);

/**
 * take a double from the stack and put its negative back to the stack
 * \param data data needed for calculation
 * \exception E_OVERFLOW Thrown in case of RPN/Stack overflow
 * \exception E_UNDERFLOW Thrown in case of RPN/Stack underflow
 * \return error code
 */
int neg(Calculation_data *data);

/**
 * search the name of a variable and put its value to the stack
 * \param data data needed for calculation
 * \exception E_OVERFLOW Thrown in case of RPN/Stack overflow
 * \exception E_UNDERFLOW Thrown in case of RPN/Stack underflow
 * \exception E_UNKNOWN_VAR Thrown in case the variable is not found in the var table
 * \return error code
 */
int lookup_var(Calculation_data *data);