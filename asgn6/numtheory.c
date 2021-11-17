#include "numtheory.h"
#include "randstate.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>

/* gets the gcd of inputs a and b and stores output in d */
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t originalb, originala, temp;
    mpz_inits(originalb, originala, temp, NULL);

    /* store original numbers to avoid changes */
    mpz_set(originala, a);
    mpz_set(originalb, b);

    /* gcd algorithm */
    while ((mpz_cmp_ui(b, 0)) != 0) {
        mpz_set(temp, b);
        mpz_mod(b, a, b);
        mpz_set(a, temp);
    }
    mpz_set(d, a);

    /* return a and b to original values */
    mpz_set(a, originala);
    mpz_set(b, originalb);

    /* free memory of temporary variables */
    mpz_clears(originala, originalb, temp, NULL);
}

void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r, tempr, t, tempt, q, originalr, originalt, timesr, timest;
    mpz_inits(r, tempr, t, tempt, q, originalr, originalt, timesr, timest, NULL);

    mpz_set(r, n);
    mpz_set(tempr, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(tempt, 1);

    /* mod inverse algorithm */
    while ((mpz_cmp_ui(tempr, 0)) != 0) {
        mpz_fdiv_q(q, r, tempr);

        /* store values of r and t so calculations can be done without interference */
        mpz_set(originalr, r);
        mpz_set(originalt, t);

        mpz_set(r, tempr);

        mpz_mul(timesr, q, tempr);
        mpz_sub(tempr, originalr, timesr);

        mpz_set(t, tempt);

        mpz_mul(timest, q, tempt);
        mpz_sub(tempt, originalt, timest);
    }

    /* no inverse */
    if ((mpz_cmp_ui(r, 1)) > 0) {
        mpz_set(i, 0);
    }

    if ((mpz_cmp_ui(t, 0)) < 0) {
        mpz_add(t, t, n);
	mpz_set(i, t);
    }

    /* free memory of temporary variables */
    mpz_clears(r, tempr, t, tempt, q, originalr, originalt, NULL);
}

void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t p, v, originalexponent;
    mpz_inits(p, v, originalexponent, NULL);

    mpz_set_ui(v, 1);
    mpz_set(p, base);

    /* store original exponent value */
    mpz_set(originalexponent, exponent);

    /* power modulo algorithm */
    while ((mpz_cmp_ui(exponent, 0)) > 0) {
        if ((mpz_odd_p(exponent)) != 0) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, modulus);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, modulus);

        mpz_fdiv_q_ui(exponent, exponent, 2);
    }
    mpz_set(out, v);

    /* set exponent back to original value */
    mpz_set(exponent, originalexponent);

    /* free memory of temporary variables */
    mpz_clears(p, v, originalexponent, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t a, r, s, y, tempn, modn, j, temps, consttwo;
    mpz_inits(a, r, s, y, tempn, modn, j, temps, consttwo, NULL);
    mpz_set_ui(consttwo, 2);

    /* special cases */

    /* n is 3 */
    if ((mpz_cmp_ui(n, 3)) == 0) {
        mpz_clears(a, r, s, y, tempn, j, temps, consttwo, modn, NULL);
        return true;
    }

    /* n is 2 */
    if ((mpz_cmp_ui(n, 2)) == 0) {
        mpz_clears(a, r, s, y, tempn, j, temps, consttwo, modn, NULL);
        return true;
    }

    /* n is 1 */
    if ((mpz_cmp_ui(n, 1)) == 0) {
        mpz_clears(a, r, s, y, tempn, j, temps, consttwo, modn, NULL);
        return false;
    }

    /* if number is even */
    if ((mpz_even_p(n)) != 0) {
        mpz_clears(a, r, s, y, tempn, j, temps, consttwo, modn, NULL);
        return false;
    }

    /* miller rabin test */
    mpz_sub_ui(tempn, n, 1);

    mpz_sub_ui(r, n, 1);
    while ((mpz_odd_p(r)) == 0) {
        mpz_fdiv_q_ui(r, r, 2);
        mpz_add_ui(s, s, 1);
    }

    for (uint64_t i = 1; i < iters; i++) {
        while ((mpz_cmp_ui(a, 1)) <= 0) {
            mpz_urandomm(a, state, tempn);
        }

        pow_mod(y, a, r, n);
        if ((mpz_cmp_ui(y, 1)) != 0 && (mpz_cmp(y, tempn)) != 0) {
            mpz_set_ui(j, 1);

            mpz_sub_ui(temps, s, 1);
            while ((mpz_cmp(j, temps)) <= 0 && (mpz_cmp(y, tempn)) != 0) {
                pow_mod(y, y, consttwo, n);
                if ((mpz_cmp_ui(y, 1)) == 0) {
                    mpz_clears(a, r, s, y, tempn, j, temps, consttwo, modn, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if ((mpz_cmp(y, tempn)) != 0) {
                mpz_clears(a, r, s, y, tempn, j, temps, consttwo, modn, NULL);
                return false;
            }
        }
    }
    /* free memory of temporary variables and RNG */
    mpz_clears(a, r, s, y, tempn, j, temps, consttwo, modn, NULL);

    /* return true, number is prime */
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t bitnum;
    mpz_init(bitnum);

    /* maximum value */
    mpz_ui_pow_ui(bitnum, 2, bits);

    /* prime test */
    while (is_prime(p, iters) == false) {
        mpz_urandomm(p, state, bitnum);
    }
}
