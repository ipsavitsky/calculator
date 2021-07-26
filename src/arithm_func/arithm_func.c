#include "arithm_func.h"

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define SAFE(call)                           \
    do {                                     \
        if ((flag = call) != 0) return flag; \
    } while (0)

int add_elem(Calculation_data *data) { return stack_push(data->stack, data->elem, data->size); }

int sum_double(Calculation_data *data) {
    double elem1, elem2;
    int flag;
    assert(data->stack != NULL);
    SAFE(stack_pop(data->stack, &elem2, sizeof(elem2)));
    SAFE(stack_pop(data->stack, &elem1, sizeof(elem1)));
    double res = elem1 + elem2;
    printf("%lf + %lf = %lf\n", elem1, elem2, res);
    // printf("res: %d\n", res);
    SAFE(stack_push(data->stack, &res, sizeof(res)));
    return 0;
}

int sub_double(Calculation_data *data) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(data->stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(data->stack, &elem1, sizeof(elem2)));
    double res = elem1 - elem2;
    printf("%lf - %lf = %lf\n", elem1, elem2, res);
    // printf("res: %d\n", res);
    SAFE(stack_push(data->stack, &res, sizeof(res)));
    return 0;
}

int mult_double(Calculation_data *data) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(data->stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(data->stack, &elem1, sizeof(elem2)));
    double res = elem1 * elem2;
    printf("%lf * %lf = %lf\n", elem1, elem2, res);
    // printf("res: %d\n", res);
    SAFE(stack_push(data->stack, &res, sizeof(res)));
    return 0;
}

int div_double(Calculation_data *data) {
    int flag;
    double elem1, elem2;
    SAFE(stack_pop(data->stack, &elem2, sizeof(elem1)));
    SAFE(stack_pop(data->stack, &elem1, sizeof(elem2)));
    if (fabs(elem2) < DBL_EPSILON) return E_ZERO_DIVISION;
    double res = elem1 / elem2;
    printf("%lf / %lf = %lf\n", elem1, elem2, res);
    SAFE(stack_push(data->stack, &res, sizeof(res)));
    return 0;
}

int neg(Calculation_data *data) {
    double num;
    int flag;
    SAFE(stack_pop(data->stack, &num, sizeof(num)));
    num = -num;
    SAFE(stack_push(data->stack, &num, sizeof(num)));
    return 0;
}

int lookup_var(Calculation_data *data) {
    size_t lookup_name_size = strlen(data->elem);
    // printf("looking up: %s\n", (char *)(data->elem));
    for (int i = 0; i < data->v_tab->var_num; ++i) {
        size_t name_size = strlen(data->v_tab->vars[i].name);
        // printf("%s and %s", data->v_tab->vars[i].name, (char *)(data->elem));
        if (name_size != lookup_name_size) continue;
        if (strncmp(data->elem, data->v_tab->vars[i].name, name_size) == 0) {
            printf("%s = %lf\n", data->v_tab->vars[i].name,
                   *((double *)(data->v_tab->vars[i].data)));
            return stack_push(data->stack, data->v_tab->vars[i].data, sizeof(double));
        }
    }
    return E_UNKNOWN_VAR;
}