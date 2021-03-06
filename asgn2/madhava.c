#include "mathlib.h"

#include <stdio.h>

static int total_madhava_terms = 1;
/* this function calculates pi using the Madhava series */
double pi_madhava(void) {
    total_madhava_terms = 1;
    double exponent = 1.0;
    double denominator = 1.0;
    double current = 1.0;
    double previous = 0.0;
    double term = 0.0;
    int k = 1;
    while (absolute(current - previous) > EPSILON) {
        exponent *= -3.0;
        denominator = (2.0 * k) + 1.0;
        previous = current;
        current += (1.0 / (exponent * denominator));
        k++;
        total_madhava_terms++;
    }
    term = current * sqrt_newton(12);
    return term;
}
/* this function returns the number of terms computated by the pi_madhava function
 * until epsilon limit was reached */
int pi_madhava_terms() {
    return total_madhava_terms;
}
