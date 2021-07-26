#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/rec_desc/rec_desc.h"

int LLVMFuzzerTestOneInput(const char *Data, size_t Size) {
    char *real_input = calloc(Size + 1, sizeof(char));
    if (real_input == NULL) {
        return E_MEM_ALLOC;
    }
    memcpy(real_input, Data, Size);
    Expression expr;
    // init_expression(&expr, real_input);
    if (init_expression(&expr, real_input) != 0) return E_MEM_ALLOC;
    double res = 0;
    int flag;
    if ((flag = compute_expression(&expr, &res)) != 0) {
        // err_print(flag);
    } else {
        // printf("res: %lf\n", res);
    }
    finalize_expression(&expr);
    free(real_input);
    return flag;
}