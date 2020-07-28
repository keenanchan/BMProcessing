#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath> // for floor
#include <algorithm> // for min

#include <string>

#include <vector>

using namespace std;

#include "setupfinal.h"
#include "hw3.h"

//MAIN BODY HERE
int main()
{
    int width;
    int height;
    double **fr, **fg, **fb;
    double **nfr, **nfg, **nfb;
 
    string ifname, ofname;
 
    // if reading in picture
    cout << "Enter bmp input filename: ";
    cin >> ifname;
    ifstream infile(ifname, ios::in | ios::binary);
    readcolorpicture(fr,fg,fb,infile,width,height);
    infile.close();
    
    int new_width, new_height;
 
    cout << "Enter desired width (in pixels): ";
    cin >> new_width;
    while (new_width <= 0) 
    {
        cerr << "Invalid width. Please enter a valid value (>0)." << endl;
        cout << "Enter desired width (in pixels): ";
        cin >> new_width;
    }

    cout << "Enter desired height (in pixels): ";
    cin >> new_height;
    while (new_height <= 0) 
    {
        cout << "Invalid height. Please enter a valid value (>0)." << endl;
        cout << "Enter desired height (in pixels): ";
        cin >> new_height;
    }
    nfr = matrix(new_width, new_height);
    nfg = matrix(new_width, new_height);
    nfb = matrix(new_width, new_height);

    string use_bc = "";
    cout << "Enter desired interpolation method (bilinear or bicubic)." << endl;
    cout << "Default is bicubic. Use bicubic? [Y/n] ";
    cin.ignore();
    getline(cin, use_bc);
    while (use_bc != "Y" && use_bc != "y" && use_bc != "N" && use_bc != "n" && !use_bc.empty())
    {
        cout << "Please enter [y/n], or press Enter: ";
        cin >> use_bc; 
    }

    if (use_bc == "n" || use_bc == "N")
    {
        nfr = resize_bilinear(fr, nfr, width, height, new_width, new_height);
        nfg = resize_bilinear(fg, nfg, width, height, new_width, new_height);
        nfb = resize_bilinear(fb, nfb, width, height, new_width, new_height);
    }
    else
    {
        nfr = resize_bicubic(fr, nfr, width, height, new_width, new_height);
        nfg = resize_bicubic(fg, nfg, width, height, new_width, new_height);
        nfb = resize_bicubic(fb, nfb, width, height, new_width, new_height);
    }

    // writing picture
    string tempofname;
    ofname = "result.bmp";
    cout << "Enter bmp output filename. " << endl;
    cout << "Leave blank for default outfile name (\"result.bmp\"): ";
    getline(cin, tempofname);
    if (!tempofname.empty()) ofname = tempofname;
    ofstream outfile(ofname, ios::out | ios::binary);
    writecolorpicture(outfile,nfr,nfg,nfb, new_width, new_height);
    outfile.close();
 
    free_matrix(fr, width, height);
    free_matrix(fg, width, height);
    free_matrix(fb, width, height);

    free_matrix(nfr, new_width, new_height);
    free_matrix(nfg, new_width, new_height);
    free_matrix(nfb, new_width, new_height);
}

/* Bilinear Resizing Algorithm. Uses Bilinear interpolation to
 * resize an image. 
 */
double **resize_bilinear(double **f, double **new_f, int old_w, int old_h, int new_w, int new_h)
{
    int i, j;
    double w_mult, h_mult;
    double ref_x, ref_y;

    w_mult = 1.0*(old_w-1)/(new_w-1);
    h_mult = 1.0*(old_h-1)/(new_h-1);

    for (i = 0; i < new_w; i++) 
    {
        ref_x = 1.0*i*w_mult;
        int ti = static_cast<int>(floor(ref_x));
        double al = ref_x - ti;

        for (j = 0; j < new_h; j++)
        {
            ref_y = 1.0*j*h_mult;
            int tj = static_cast<int>(floor(ref_y));
            double bt = ref_y - tj;

            if (ti < old_w-1 && tj < old_h-1)
            {
                new_f[i][j] = (1-al)*(1-bt)*f[ti][tj] + (1-al)*bt*f[ti][tj+1];
                new_f[i][j] += al*(1-bt)*f[ti+1][tj] + al*bt*f[ti+1][tj+1];
            }
            else if (ti == old_w-1 && tj < old_h-1)
            {
                new_f[i][j] = (1-bt)*f[ti][tj] + bt*f[ti][tj+1];
            }
            else if (ti < old_w-1 && tj == old_h-1)
            {
                new_f[i][j] = (1-al)*f[ti][tj] + al*f[ti+1][tj];
            }
            else
            {
                new_f[i][j] = f[ti][tj];
            }
        }
    }
    return new_f;
}

/* Bicubic Resizing Algorithm. Uses Bicubic interpolation to
 * resize an image. 
 */
double** resize_bicubic(double **f, double **new_f, int old_w, int old_h, int new_w, int new_h)
{
    int i, j;
    int ti, tj;
    double al, bt;
    double w_mult, h_mult;
    double ref_x, ref_y;

    w_mult = 1.0*(old_w-1)/(new_w-1);
    h_mult = 1.0*(old_h-1)/(new_h-1);

    for(i = 0; i < new_w; i++)
    {
        ref_x = 1.0*i*w_mult;
        ti = static_cast<int>(floor(ref_x));
        al = ref_x - ti;

        for(j = 0; j < new_h; j++)
        {

            ref_y = 1.0*j*h_mult;
            tj = static_cast<int>(floor(ref_y));
            bt = ref_y - tj;

            if (ti > 0 && ti < old_w-2 && tj > 0 && tj < old_h-2)
            {
                new_f[i][j] = bw_from_lattice(f, al, bt, ti, tj);
            }
            else
            {
                new_f[i][j] = bc_from_lattice(f, al, bt, ti, tj, old_w, old_h);
            }
        }
    }
    return new_f;
}

/* Weighted average approximation of bicubic interpolation.
 * Sufficiently interior points on the picture are guaranteed to
 * have an interpolation value between 0 and 1.
*/
double bicubic_weighted(double al, double fa, double fb, double fc, double fd)
{
    return fa + (1+al)*(fb-fa) + (1+al)*(al)*(fc-2*fb+fa)/2 + (1+al)*(al)*(al-1)*(fd-3*fc+3*fb-fa)/6;
}

/* Helper function for weighted average approximation of bicubic interpolation.
 * Constructs the necessary 16-pixel lattice to calculate the interpolation value.
 */
double bw_from_lattice(double **f, double al, double bt, int i, int j)
{
    double fa, fb, fc, fd;
    fa = bicubic_weighted(al, f[i-1][j-1], f[i][j-1], f[i+1][j-1], f[i+2][j-1]);
    fb = bicubic_weighted(al, f[i-1][j], f[i][j], f[i+1][j], f[i+2][j]);
    fc = bicubic_weighted(al, f[i-1][j+1], f[i][j+1], f[i+1][j+1], f[i+2][j+1]);
    fd = bicubic_weighted(al, f[i-1][j+2], f[i][j+2], f[i+1][j+2], f[i+2][j+2]);
    return bicubic_weighted(bt, fa, fb, fc, fd);
}

/* Bicubic interpolation algorithm
 */
double bicubic_interp(double al, double fa, double fb, double fc, double fd)
{
    double lz, lo, ltw, lth;
    lz = (al)*(al-1)*(al-2)/((-1)*(-1-1)*(-1-2));
    lo = (al+1)*(al-1)*(al-2)/((1)*(-1)*(-2));
    ltw = (al+1)*(al)*(al-2)/((1+1)*(1)*(1-2));
    lth = (al+1)*(al)*(al-1)/((2+1)*(2)*(2-1));
    return lz*fa + lo*fb + ltw*fc + lth*fd;
}

/* Helper function for bicubic interpolation.
 * Constructs the necessary 16-pixel lattice to calculate the interpolation value.
 * Checks whether the given coords correspond to an edge of the image,
 * and bounds the resulting interpolation value between 0 and 1.
 */
double bc_from_lattice(double **f, double al, double bt, int i, int j, int w, int h)
{
    int im, ip, ipp, jm, jp, jpp;
    im = max(i-1, 0);
    ip = min(i+1, w-1);
    ipp = min(i+2, w-1);
    jm = max(j-1, 0);
    jp = min(j+1, h-1);
    jpp = min(j+2, h-1);

    double fa, fb, fc, fd;
    fa = bicubic_interp(al, f[im][jm], f[i][jm], f[ip][jm], f[ipp][jm]);
    fb = bicubic_interp(al, f[im][j], f[i][j], f[ip][j], f[ipp][j]);
    fc = bicubic_interp(al, f[im][jp], f[i][jp], f[ip][jp], f[ipp][jp]);
    fd = bicubic_interp(al, f[im][jpp], f[i][jpp], f[ip][jpp], f[ipp][jpp]);

    return bnd(bicubic_interp(bt, fa, fb, fc, fd), 0.0, 1.0);
}

// Helper function, bounds between 2 values
double bnd(double v, double minV, double maxV)
{
    return min(max(v, minV), maxV);
}