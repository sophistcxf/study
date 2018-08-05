from scipy.optimize import minimize, rosen, rosen_der

def optimize_nelder_mead():
    x0 = [1.3, 0.7, 0.8, 1.9, 1.2]
    res = minimize(rosen, x0, method='Nelder-Mead', tol=1e-6)
    print res.x
    print res.message

def optimize_bfgs():
    x0 = [1.3, 0.7, 0.8, 1.9, 1.2]
    res = minimize(rosen, x0, method='BFGS', jac=rosen_der, options={'gtol': 1e-6, 'disp': True})
    print res.x
    print res.message


#optimize_nelder_mead()
optimize_bfgs()
