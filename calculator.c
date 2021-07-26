#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/rec_desc/rec_desc.h"

int main(void) {
    char *curline = NULL;
    char *eq_ptr;
    double res;
    size_t what;
    int flag;
    Expression expr;
    init_expression(&expr, "");
    for (;;) {
        printf(">>> ");
        if (getline(&curline, &what, stdin) == -1) {
            printf("error reading\n");
            break;
        }
        if ((eq_ptr = strchr(curline, '=')) != NULL) {
            size_t var_name_size = eq_ptr - curline;
            free(expr.string_form);
            expr.string_form = strdup(eq_ptr + 1);
            expr.curpointer = expr.string_form;
            if ((flag = compute_expression(&expr, &res)) != 0) {
                err_print(flag);
            }
            curline[var_name_size] = '\0';
            add_variable_to_table(&expr, curline, res);
        } else if ((strlen(curline) == 2) && (*curline == 'q')) {
            break;
        } else {
            free(expr.string_form);
            expr.string_form = strdup(curline);
            expr.curpointer = expr.string_form;
            if ((flag = compute_expression(&expr, &res)) != 0) {
                err_print(flag);
            }
            printf("result = %lf\n", res);
        }
        res = 0;
    }
    free(curline);
    finalize_expression(&expr);
    return 0;
}