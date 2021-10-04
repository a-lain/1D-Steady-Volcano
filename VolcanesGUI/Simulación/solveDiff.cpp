#include "solveDiff.h"
 
VectorXd EulerExplicito(VectorXd y, double t, std::function<VectorXd(VectorXd,double)> f, double dt)
{
    VectorXd ydot;
    ydot = f(y, t);
    return ydot*dt;
}

VectorXd RungeKuttaOrden4(VectorXd y, double t, std::function<VectorXd(VectorXd, double)> f, double dt)
{
    VectorXd k1, k2, k3, k4;
    k1 = f(y, t);
    k2 = f(y + 1./2*k1*dt, t + dt/2);
    k3 = f(y + 1./2*k2*dt, t + dt/2);
    k4 = f(y + k3*dt, t + dt);
    return y + 1./6*dt*(k1 + 2*k2 + 2*k3 + k4);
}

void RungeKuttaAdaptativo(VectorXd& y, double t, double epsilon, 
    std::function<VectorXd(VectorXd, double)> f, double& dt1, double& dtSig, double dtmin)
{
    VectorXd k1, k2, k3, k4, k5, k6, y5, y4, Delta0, Delta1;
    double temp;
    double S = 0.9; // Factor de seguridad
    double FactMult = 5; // Máximo factor de incremento en dt
    double c51 = 37./378, c52 = 0, c53 = 250./621, c54 = 125./594, c55 = 0, c56 = 512./1771, c41 = 2825./27648,
        c42 = 0, c43 = 18575./ 48384, c44 = 13525./ 55296, c45 = 277./14336, c46 = 1./4, a2 = 1./5, a3 = 3./10,
        a4 = 3./5, a5 = 1, a6 = 7./8, b21 = 1./5, b31 = 3./40, b32 = 9./40, b41 = 3./10, b42 = -9./10, b43 = 6./5,
        b51 = -11./54, b52 = 5./2, b53 = -70./27, b54 = 35./27, b61 = 1631./ 55296, b62 = 175./ 512, b63 = 575./13824,
        b64 = 44275./ 110592, b65 = 253./ 4096;
    k1 = dt1*f(y, t);
    k2 = dt1*f(y + b21*k1, t + a2*dt1);
    k3 = dt1*f(y + b31*k1 + b32*k2, t + a3*dt1);
    k4 = dt1*f(y + b41*k1 + b42*k2 + b43*k3, t + a4*dt1);
    k5 = dt1*f(y + b51*k1 + b52*k2 + b53*k3 + b54*k4, t + a5*dt1);
    k6 = dt1*f(y + b61*k1 + b62*k2 + b63*k3 + b64*k4 + b65*k5, t + a6*dt1);
    y5 = y + c51*k1 + c52*k2 + c53*k3 + c54*k4 + c55*k5 + c56*k6;
    y4 = y + c41*k1 + c42*k2 + c43*k3 + c44*k4 + c45*k5 + c46*k6;
    Delta0 = epsilon*k1;
    Delta1 = y5 - y4;
    double dt0 = fabs(Delta0[0]) >= fabs(Delta1[0]) ? S*dt1*pow(fabs(Delta0[0] / Delta1[0]), 0.2) :
        S*dt1*pow(fabs(Delta0[0] / Delta1[0]), 0.25);
    for (int i = 1; i < k1.size(); i++)
    {
        temp = fabs(Delta0[i]) >= fabs(Delta1[i]) ? S*dt1*pow(fabs(Delta0[i] / Delta1[i]), 0.2) : S*dt1*pow(fabs(Delta0[i] / Delta1[i]), 0.25);
        if (fabs(temp) < fabs(dt0))
        {
            dt0 = temp;
        }
    }
    if (fabs(dt0 / dt1) > FactMult || isinf(dt0))
    {
        dt0 = FactMult*dt1;
    }
    // printf("dt1 = %.32lf,  dt0 = %.32lf\n", dt1, dt0);
    if (fabs(dt0) < fabs(dt1))
    {
        if (fabs(dt0) < fabs(dtmin))
        {
            dt0 = dtmin;
        }
        dt1 = dt0;
        // Repetimos los cálculos con dt0;
        k1 = dt1*f(y, t);
        k2 = dt1*f(y + b21*k1, t + a2*dt1);
        k3 = dt1*f(y + b31*k1 + b32*k2, t + a3*dt1);
        k4 = dt1*f(y + b41*k1 + b42*k2 + b43*k3, t + a4*dt1);
        k5 = dt1*f(y + b51*k1 + b52*k2 + b53*k3 + b54*k4, t + a5*dt1);
        k6 = dt1*f(y + b61*k1 + b62*k2 + b63*k3 + b64*k4 + b65*k5, t + a6*dt1);
        y = y + c51*k1 + c52*k2 + c53*k3 + c54*k4 + c55*k5 + c56*k6;
    }
    else
    {
        y = y5;
    }
    dtSig = dt0;
}

double NewtonRaphson(double x0, std::function<Vector2d(double)> fYdfdx, double tol, int iterMax, int debug, Progreso& progreso)
{
    double x;
    Vector2d v;
    double olderr = 0, err = 0, fmejora = 0, itquedan = 0;
    progreso.AgnadeTarea("Aplicando Newton-Raphson...");
    for (int i = 0; i < iterMax; i++)
    {
        v = fYdfdx(x0);
        x = x0 - v[0] / v[1];
        err = fabs(x - x0);
        if (err/fabs(x) < tol)
        {
            progreso.EliminaTarea();
            return x;
        }
        x0 = x;

        if (debug)
        {
            printf("i = %d,  f = %lf,  dfdx = %lf,  x = %lf,  err = %lf\n", i, v[0], v[1], x, err);
        }

        // Estimación de la velocidad
        fmejora = err / olderr;
        itquedan = log(tol*fabs(x) / err) / log(fmejora);
        progreso.ActualizaTarea((double) i / (i + itquedan));
        olderr = err;
    }
    progreso.EliminaTarea();
    printf("Error. Número máximo de iteraciones alcanzado.\n");
    return 0;
}


double Biseccion(double a, double b, std::function<double(double)> f, double tol, int iterMax, Progreso &progreso)
{
    progreso.AgnadeTarea("Aplicando bisección...");
    int N = ceil(log((b - a) / tol) / log(2)); // Número de iteracciones total.
    double fa = f(a), p, fp;
    for (int i = 0; i < iterMax; i++)
    {
        p = a + (b - a) / 2;
        fp = f(p);
        if (fp == 0 || (b - a) / 2 < tol)
        {
            progreso.EliminaTarea();
            return p;
        }
        else if (fa*fp > 0)
        {
            a = p;
            fa = fp;
        }
        else
        {
            b = p;
        }
        progreso.ActualizaTarea((double)i/N);
    }
    printf("Error. Máximo número de iteraciones alcanzado.\n");
    progreso.EliminaTarea();
    return 0;
}


double Secante(double x1, double x2, std::function<double(double)> f, double tol, int iterMax)
{
    double f1 = f(x1);
    double f2 = f(x2);
    double x;
    for (int i = 0; i < iterMax; i++)
    {
        x = x2 - f1*(x2 - x1) / (f2 - f1);
        if (fabs(x - x2) < tol)
        {
            return x;
        }
        x1 = x2;
        f1 = f2;
        x2 = x;
        f2 = f(x);
    }
    printf("Error. Máximo número de iteraciones alcanzado.\n");
    return 0;
}


double Brent(double x1, double x2, std::function<double(double)> f, double tol, int iterMax)
{
    int iter;
    double EPS = 1e-12;
    double a = x1,b = x2 ,c = x2, d, e, min1, min2;
    double fa = f(a), fb = f(b), fc, p, q, r, s, tol1, xm;
    if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0))
        printf("Root must be bracketed in zbrent");
    fc = fb;
    for (iter=1; iter <= iterMax; iter++) 
    {
        if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0)) {
        c = a; // Rename a, b, c and adjust bounding interval d.
        fc = fa;
        e = d = b - a;
    }
    if (fabs(fc) < fabs(fb)) 
    {
        a = b;
        b = c;
        c = a;
        fa = fb;
        fb = fc;
        fc = fa;
    }
    tol1 = 2.0*EPS*fabs(b) + 0.5*tol; // Convergence check.
    xm = 0.5*(c - b);
    if (fabs(xm) <= tol1 || fb == 0.0) return b;
    if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) 
    {
        s = fb/fa; // Attempt inverse quadratic interpolation.
        if (a == c) {
            p = 2.0*xm*s;
            q = 1.0 - s;
        } 
        else 
        {
            q = fa / fc;
            r = fb / fc;
            p = s*(2.0*xm*q*(q - r) - (b - a)*(r -1.0));
            q = (q - 1.0)*(r - 1.0)*(s - 1.0);
        }
        if (p > 0.0) q = -q; // Check whether in bounds.
        p = fabs(p);
        min1 = 3.0*xm*q - fabs(tol1*q);
        min2 = fabs(e*q);
        if (2.0*p < (min1 < min2 ? min1 : min2)) 
        {
            e = d; // Accept interpolation.
            d = p/q;
        }
        else {
            d = xm; // Interpolation failed, use bisection.
            e = d;
        }
    } 
    else 
    {
        // Bounds decreasing too slowly, use bisection.
        d = xm;
        e = d;
    }
    a = b; // Move last best guess to a.
    fa = fb;
    if (fabs(d) > tol1) // Evaluate new trial root.
        b += d;
    else
        b += xm >=0.0 ? fabs(tol1): -fabs(tol1);
    fb = f(b);
    }
    printf("Maximum number of iterations exceeded in zbrent\n");
    return 0.0;
}



VectorXd Broyden(std::function<VectorXd(VectorXd)> F, VectorXd x0, MatrixXd Jx0, double tol, int iterMax)
{
    MatrixXd A = Jx0.inverse();
    VectorXd v = F(x0);
    VectorXd s = -A*v;
    VectorXd x = x0 + s;
    VectorXd w, y, z, u;
    double p;
    for (int i = 1; i < iterMax; i++)
    {
        w = v;
        v = F(x);
        y = v - w;
        z = -A*y;
        p = -s.dot(z);
        u = s.transpose()*A;
        A = A + 1/p * (s + z)*u.transpose();
        x = x + s;
        if (s.norm() < tol)
        {
            return x;
        }
    }
    printf("Error. Máximo número de iteraciones alcanzado.\n");
    return Vector2d(0,0);
}

std::function<double(double)> InterpolacionCuadratica(double f1, double x1, double f2, double x2, double f3, double x3)
{
    double a, b, c;
    b = (f2 - f1 - (f3 - f1)*(x2*x2 - x1*x1) / (x3*x3 - x1*x1)) / (x2 - x1 - (x2*x2 - x1*x1) / (x3 + x1));
    a = (f3 - f1 - b*(x3 - x1)) / (x3*x3 - x1*x1);
    c = f1 - a*x1*x1 - b*x1;
    auto func = [a,b,c] (double x) -> double
    {
        return a*x*x + b*x +c;
    };
    return func;
}

