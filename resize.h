// hw3
double **resize_bilinear(double **f, double **new_f, int old_w, int old_h, int new_w, int new_h);
double **resize_bicubic(double **f, double **new_f, int old_w, int old_h, int new_w, int new_h);
double bicubic_weighted(double al, double fa, double fb, double fc, double fd);
double bw_from_lattice(double **f, double al, double bt, int i, int j);
double bicubic_interp(double al, double fa, double fb, double fc, double fd);
double bc_from_lattice(double **f, double al, double bt, int i, int j, int w, int h);
double bnd(double v, double minV, double maxV);