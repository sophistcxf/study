/*************************************************************************
  > File Name: test.cpp
  > Author: xiangfei.cui
  > Mail: xiangfei.cui@alibaba-inc.com
  > Created Time: Sat 21 Jul 2018 03:51:18 PM CST
 ************************************************************************/

#include <iostream>
using namespace std;

#include <math.h>
#include <nlopt.h>
#include <stdio.h>
#include <iostream>

/*!
 * @param grad 梯度，基于梯度的优化算法需要使用梯度
 */
double myfunc(unsigned n, const double *x, double *grad, void *my_func_data)
{
    if (grad) {
        grad[0] = 0.0;
        grad[1] = 0.5 / sqrt(x[1]);
    }
    return sqrt(x[1]);
}

typedef struct {
    double a, b;
} my_constraint_data;

double myconstraint(unsigned n, const double *x, double *grad, void *data)
{
    my_constraint_data *d = (my_constraint_data *) data;
    double a = d->a, b = d->b;
    if (grad) {
        grad[0] = 3 * a * (a*x[0] + b) * (a*x[0] + b);
        grad[1] = -1.0;
    }
    return ((a*x[0] + b) * (a*x[0] + b) * (a*x[0] + b) - x[1]);
}

void test1()
{
    double lb[2] = { -HUGE_VAL, 0 }; /* lower bounds */
    nlopt_opt opt;

    opt = nlopt_create(NLOPT_LN_COBYLA, 2); /* algorithm and dimensionality */
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_min_objective(opt, myfunc, NULL);

    my_constraint_data data[2] = { {2,0}, {-1,1} };

    nlopt_add_inequality_constraint(opt, myconstraint, &data[0], 1e-8);
    nlopt_add_inequality_constraint(opt, myconstraint, &data[1], 1e-8);

    nlopt_set_xtol_rel(opt, 1e-4);

    double x[2] = { 1.234, 5.678 };  /* `*`some` `initial` `guess`*` */
    double minf; /* `*`the` `minimum` `objective` `value,` `upon` `return`*` */
    if (nlopt_optimize(opt, x, &minf) < 0) {
        printf("nlopt failed!\n");
    }
    else {
        printf("found minimum at f(%g,%g) = %0.10g\n", x[0], x[1], minf);
    }

    nlopt_destroy(opt);
}

double obj_func2(unsigned n, const double *x, double *grad, void *my_func_data)
{
    return (x[0]-1) * (x[0]-1) + (x[1]-2.5) * (x[1]-2.5);
}

double cons_func21(unsigned n, const double *x, double *grad, void *data)
{
    return - x[0] + 2*x[1] - 2;
}

double cons_func22(unsigned n, const double *x, double *grad, void *data)
{
    return x[0] + 2 * x[1] - 6;
}


double cons_func23(unsigned n, const double *x, double *grad, void *data)
{
    return x[0] - 2 * x[1] - 2;
}

void test2()
{
    double lb[2] = { -HUGE_VAL, 0 }; /* lower bounds */
    nlopt_opt opt;

    opt = nlopt_create(NLOPT_LN_COBYLA, 2); /* algorithm and dimensionality */
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_min_objective(opt, obj_func2, NULL);

    nlopt_add_inequality_constraint(opt, cons_func21, NULL, 1e-8);
    nlopt_add_inequality_constraint(opt, cons_func22, NULL, 1e-8);
    nlopt_add_inequality_constraint(opt, cons_func23, NULL, 1e-8);

    nlopt_set_xtol_rel(opt, 1e-4);

    double x[2] = { 2, 0 };  /* `*`some` `initial` `guess`*` */
    double minf; /* `*`the` `minimum` `objective` `value,` `upon` `return`*` */
    if (nlopt_optimize(opt, x, &minf) < 0) {
        printf("nlopt failed!\n");
    }
    else {
        printf("found minimum at f(%g,%g) = %0.10g\n", x[0], x[1], minf);
    }

    nlopt_destroy(opt);
    
}

void test3()
{
    double lb[2] = {-1000, -10000 }; /* lower bounds */
    double ub[2] = {1000, 1000};
    nlopt_opt opt;

    std::cout << HUGE_VAL;

    //opt = nlopt_create(NLOPT_LN_COBYLA, 2); /* algorithm and dimensionality */
    //opt = nlopt_create(NLOPT_GN_DIRECT, 2);
    opt = nlopt_create(NLOPT_GN_DIRECT_L, 2);
    nlopt_set_lower_bounds(opt, lb);
    nlopt_set_upper_bounds(opt, ub);
    nlopt_set_min_objective(opt, obj_func2, NULL);

    nlopt_add_inequality_constraint(opt, cons_func21, NULL, 1e-8);
    nlopt_add_inequality_constraint(opt, cons_func22, NULL, 1e-8);
    nlopt_add_inequality_constraint(opt, cons_func23, NULL, 1e-8);

    nlopt_set_xtol_rel(opt, 1e-4);
    nlopt_set_maxtime(opt, 10);

    /* 注意初始值不能超出[lb,ub]，否则返回 INVALID_ARGS 错误 */
    double x[2] = { 2, 0 };  /* `*`some` `initial` `guess`*` */
    double minf; /* `*`the` `minimum` `objective` `value,` `upon` `return`*` */
    nlopt_result rlt = nlopt_optimize(opt, x, &minf);
    if (rlt < 0) {
        printf("nlopt failed! %d\n", rlt);
    }
    else {
        printf("found minimum at f(%g,%g) = %0.10g\n", x[0], x[1], minf);
    }

    nlopt_destroy(opt);
    
}

int main(int argc, char* argv[])
{
    // test1()
    //test2();
    test3();
}
