#include "mathlib.h"
#include <stdio.h>

static int total_madhava_terms = 1;

double pi_madhava(void) {
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
                fprintf(stdout, "%f, %f, %f, %d\n",  current, denominator, exponent, k);
                k++;
                total_madhava_terms++;
        }
        term = current * sqrt_newton(12);
        return term;
}
int pi_euler_terms() {
        return total_madhava_terms;
}

