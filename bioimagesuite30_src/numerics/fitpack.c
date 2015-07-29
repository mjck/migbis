//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------

/* fitpack.f -- translated by f2c (version 20000704).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "bio_f2c.h"

#define min(a,b) ((a) <= (b) ? (a) : (b))
#define max(a,b) ((a) >= (b) ? (a) : (b))


/* Table of constant values */

static integer c__3 = 3;
static integer c__1 = 1;
static integer c__8 = 8;
static integer c__2 = 2;
static integer c__0 = 0;
static integer c__5 = 5;
static integer c__4 = 4;

/* Subroutine */ int bispev_(tx, nx, ty, ny, c__, kx, ky, x, mx, y, my, z__, 
	wrk, lwrk, iwrk, kwrk, ier)
real *tx;
integer *nx;
real *ty;
integer *ny;
real *c__;
integer *kx, *ky;
real *x;
integer *mx;
real *y;
integer *my;
real *z__, *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, lwest, iw;
    extern /* Subroutine */ int fpbisp_();

/*  subroutine bispev evaluates on a grid (x(i),y(j)),i=1,...,mx; j=1,... */
/*  ,my a bivariate spline s(x,y) of degrees kx and ky, given in the */
/*  b-spline representation. */

/*  calling sequence: */
/*     call bispev(tx,nx,ty,ny,c,kx,ky,x,mx,y,my,z,wrk,lwrk, */
/*    * iwrk,kwrk,ier) */

/*  input parameters: */
/*   tx    : real array, length nx, which contains the position of the */
/*           knots in the x-direction. */
/*   nx    : integer, giving the total number of knots in the x-direction */
/*   ty    : real array, length ny, which contains the position of the */
/*           knots in the y-direction. */
/*   ny    : integer, giving the total number of knots in the y-direction */
/*   c     : real array, length (nx-kx-1)*(ny-ky-1), which contains the */
/*           b-spline coefficients. */
/*   kx,ky : integer values, giving the degrees of the spline. */
/*   x     : real array of dimension (mx). */
/*           before entry x(i) must be set to the x co-ordinate of the */
/*           i-th grid point along the x-axis. */
/*           tx(kx+1)<=x(i-1)<=x(i)<=tx(nx-kx), i=2,...,mx. */
/*   mx    : on entry mx must specify the number of grid points along */
/*           the x-axis. mx >=1. */
/*   y     : real array of dimension (my). */
/*           before entry y(j) must be set to the y co-ordinate of the */
/*           j-th grid point along the y-axis. */
/*           ty(ky+1)<=y(j-1)<=y(j)<=ty(ny-ky), j=2,...,my. */
/*   my    : on entry my must specify the number of grid points along */
/*           the y-axis. my >=1. */
/*   wrk   : real array of dimension lwrk. used as workspace. */
/*   lwrk  : integer, specifying the dimension of wrk. */
/*           lwrk >= mx*(kx+1)+my*(ky+1) */
/*   iwrk  : integer array of dimension kwrk. used as workspace. */
/*   kwrk  : integer, specifying the dimension of iwrk. kwrk >= mx+my. */

/*  output parameters: */
/*   z     : real array of dimension (mx*my). */
/*           on succesful exit z(my*(i-1)+j) contains the value of s(x,y) */
/*           at the point (x(i),y(j)),i=1,...,mx;j=1,...,my. */
/*   ier   : integer error flag */
/*    ier=0 : normal return */
/*    ier=10: invalid input data (see restrictions) */

/*  restrictions: */
/*   mx >=1, my >=1, lwrk>=mx*(kx+1)+my*(ky+1), kwrk>=mx+my */
/*   tx(kx+1) <= x(i-1) <= x(i) <= tx(nx-kx), i=2,...,mx */
/*   ty(ky+1) <= y(j-1) <= y(j) <= ty(ny-ky), j=2,...,my */

/*  other subroutines required: */
/*    fpbisp,fpbspl */

/*  references : */
/*    de boor c : on calculating with b-splines, j. approximation theory */
/*                6 (1972) 50-62. */
/*    cox m.g.  : the numerical evaluation of b-splines, j. inst. maths */
/*                applics 10 (1972) 134-149. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --tx;
    --ty;
    --c__;
    --x;
    --z__;
    --y;
    --wrk;
    --iwrk;

    /* Function Body */
    *ier = 10;
    lwest = (*kx + 1) * *mx + (*ky + 1) * *my;
    if (*lwrk < lwest) {
	goto L100;
    }
    if (*kwrk < *mx + *my) {
	goto L100;
    }
    if ((i__1 = *mx - 1) < 0) {
	goto L100;
    } else if (i__1 == 0) {
	goto L30;
    } else {
	goto L10;
    }
L10:
    i__1 = *mx;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (x[i__] < x[i__ - 1]) {
	    goto L100;
	}
/* L20: */
    }
L30:
    if ((i__1 = *my - 1) < 0) {
	goto L100;
    } else if (i__1 == 0) {
	goto L60;
    } else {
	goto L40;
    }
L40:
    i__1 = *my;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (y[i__] < y[i__ - 1]) {
	    goto L100;
	}
/* L50: */
    }
L60:
    *ier = 0;
    iw = *mx * (*kx + 1) + 1;
    fpbisp_(&tx[1], nx, &ty[1], ny, &c__[1], kx, ky, &x[1], mx, &y[1], my, &
	    z__[1], &wrk[1], &wrk[iw], &iwrk[1], &iwrk[*mx + 1]);
L100:
    return 0;
} /* bispev_ */

/* Subroutine */ int clocur_(iopt, ipar, idim, m, u, mx, x, w, k, s, nest, n, 
	t, nc, c__, fp, wrk, lwrk, iwrk, ier)
integer *iopt, *ipar, *idim, *m;
real *u;
integer *mx;
real *x, *w;
integer *k;
real *s;
integer *nest, *n;
real *t;
integer *nc;
real *c__, *fp, *wrk;
integer *lwrk, *iwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    static integer nmin;
    static real dist;
    static integer i__, j, maxit, i1, i2, j1, j2, k1, k2, lwest, m1, ib, iq, 
	    iz;
    extern /* Subroutine */ int fpchep_(), fpclos_();
    static integer ia1, ia2, ig1, ig2, ncc, ifp;
    static real per, tol;

/*  given the ordered set of m points x(i) in the idim-dimensional space */
/*  with x(1)=x(m), and given also a corresponding set of strictly in- */
/*  creasing values u(i) and the set of positive numbers w(i),i=1,2,...,m */
/*  subroutine clocur determines a smooth approximating closed spline */
/*  curve s(u), i.e. */
/*      x1 = s1(u) */
/*      x2 = s2(u)       u(1) <= u <= u(m) */
/*      ......... */
/*      xidim = sidim(u) */
/*  with sj(u),j=1,2,...,idim periodic spline functions of degree k with */
/*  common knots t(j),j=1,2,...,n. */
/*  if ipar=1 the values u(i),i=1,2,...,m must be supplied by the user. */
/*  if ipar=0 these values are chosen automatically by clocur as */
/*      v(1) = 0 */
/*      v(i) = v(i-1) + dist(x(i),x(i-1)) ,i=2,3,...,m */
/*      u(i) = v(i)/v(m) ,i=1,2,...,m */
/*  if iopt=-1 clocur calculates the weighted least-squares closed spline */
/*  curve according to a given set of knots. */
/*  if iopt>=0 the number of knots of the splines sj(u) and the position */
/*  t(j),j=1,2,...,n is chosen automatically by the routine. the smooth- */
/*  ness of s(u) is then achieved by minimalizing the discontinuity */
/*  jumps of the k-th derivative of s(u) at the knots t(j),j=k+2,k+3,..., */
/*  n-k-1. the amount of smoothness is determined by the condition that */
/*  f(p)=sum((w(i)*dist(x(i),s(u(i))))**2) be <= s, with s a given non- */
/*  negative constant, called the smoothing factor. */
/*  the fit s(u) is given in the b-spline representation and can be */
/*  evaluated by means of subroutine curev. */

/*  calling sequence: */
/*     call clocur(iopt,ipar,idim,m,u,mx,x,w,k,s,nest,n,t,nc,c, */
/*    * fp,wrk,lwrk,iwrk,ier) */

/*  parameters: */
/*   iopt  : integer flag. on entry iopt must specify whether a weighted */
/*           least-squares closed spline curve (iopt=-1) or a smoothing */
/*           closed spline curve (iopt=0 or 1) must be determined. if */
/*           iopt=0 the routine will start with an initial set of knots */
/*           t(i)=u(1)+(u(m)-u(1))*(i-k-1),i=1,2,...,2*k+2. if iopt=1 the */
/*           routine will continue with the knots found at the last call. */
/*           attention: a call with iopt=1 must always be immediately */
/*           preceded by another call with iopt=1 or iopt=0. */
/*           unchanged on exit. */
/*   ipar  : integer flag. on entry ipar must specify whether (ipar=1) */
/*           the user will supply the parameter values u(i),or whether */
/*           (ipar=0) these values are to be calculated by clocur. */
/*           unchanged on exit. */
/*   idim  : integer. on entry idim must specify the dimension of the */
/*           curve. 0 < idim < 11. */
/*           unchanged on exit. */
/*   m     : integer. on entry m must specify the number of data points. */
/*           m > 1. unchanged on exit. */
/*   u     : real array of dimension at least (m). in case ipar=1,before */
/*           entry, u(i) must be set to the i-th value of the parameter */
/*           variable u for i=1,2,...,m. these values must then be */
/*           supplied in strictly ascending order and will be unchanged */
/*           on exit. in case ipar=0, on exit,the array will contain the */
/*           values u(i) as determined by clocur. */
/*   mx    : integer. on entry mx must specify the actual dimension of */
/*           the array x as declared in the calling (sub)program. mx must */
/*           not be too small (see x). unchanged on exit. */
/*   x     : real array of dimension at least idim*m. */
/*           before entry, x(idim*(i-1)+j) must contain the j-th coord- */
/*           inate of the i-th data point for i=1,2,...,m and j=1,2,..., */
/*           idim. since first and last data point must coincide it */
/*           means that x(j)=x(idim*(m-1)+j),j=1,2,...,idim. */
/*           unchanged on exit. */
/*   w     : real array of dimension at least (m). before entry, w(i) */
/*           must be set to the i-th value in the set of weights. the */
/*           w(i) must be strictly positive. w(m) is not used. */
/*           unchanged on exit. see also further comments. */
/*   k     : integer. on entry k must specify the degree of the splines. */
/*           1<=k<=5. it is recommended to use cubic splines (k=3). */
/*           the user is strongly dissuaded from choosing k even,together */
/*           with a small s-value. unchanged on exit. */
/*   s     : real.on entry (in case iopt>=0) s must specify the smoothing */
/*           factor. s >=0. unchanged on exit. */
/*           for advice on the choice of s see further comments. */
/*   nest  : integer. on entry nest must contain an over-estimate of the */
/*           total number of knots of the splines returned, to indicate */
/*           the storage space available to the routine. nest >=2*k+2. */
/*           in most practical situation nest=m/2 will be sufficient. */
/*           always large enough is nest=m+2*k, the number of knots */
/*           needed for interpolation (s=0). unchanged on exit. */
/*   n     : integer. */
/*           unless ier = 10 (in case iopt >=0), n will contain the */
/*           total number of knots of the smoothing spline curve returned */
/*           if the computation mode iopt=1 is used this value of n */
/*           should be left unchanged between subsequent calls. */
/*           in case iopt=-1, the value of n must be specified on entry. */
/*   t     : real array of dimension at least (nest). */
/*           on succesful exit, this array will contain the knots of the */
/*           spline curve,i.e. the position of the interior knots t(k+2), */
/*           t(k+3),..,t(n-k-1) as well as the position of the additional */
/*           t(1),t(2),..,t(k+1)=u(1) and u(m)=t(n-k),...,t(n) needed for */
/*           the b-spline representation. */
/*           if the computation mode iopt=1 is used, the values of t(1), */
/*           t(2),...,t(n) should be left unchanged between subsequent */
/*           calls. if the computation mode iopt=-1 is used, the values */
/*           t(k+2),...,t(n-k-1) must be supplied by the user, before */
/*           entry. see also the restrictions (ier=10). */
/*   nc    : integer. on entry nc must specify the actual dimension of */
/*           the array c as declared in the calling (sub)program. nc */
/*           must not be too small (see c). unchanged on exit. */
/*   c     : real array of dimension at least (nest*idim). */
/*           on succesful exit, this array will contain the coefficients */
/*           in the b-spline representation of the spline curve s(u),i.e. */
/*           the b-spline coefficients of the spline sj(u) will be given */
/*           in c(n*(j-1)+i),i=1,2,...,n-k-1 for j=1,2,...,idim. */
/*   fp    : real. unless ier = 10, fp contains the weighted sum of */
/*           squared residuals of the spline curve returned. */
/*   wrk   : real array of dimension at least m*(k+1)+nest*(7+idim+5*k). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used, the values wrk(1),...,wrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   lwrk  : integer. on entry,lwrk must specify the actual dimension of */
/*           the array wrk as declared in the calling (sub)program. lwrk */
/*           must not be too small (see wrk). unchanged on exit. */
/*   iwrk  : integer array of dimension at least (nest). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used,the values iwrk(1),...,iwrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   ier   : integer. unless the routine detects an error, ier contains a */
/*           non-positive value on exit, i.e. */
/*    ier=0  : normal return. the close curve returned has a residual */
/*             sum of squares fp such that abs(fp-s)/s <= tol with tol a */
/*             relative tolerance set to 0.001 by the program. */
/*    ier=-1 : normal return. the curve returned is an interpolating */
/*             spline curve (fp=0). */
/*    ier=-2 : normal return. the curve returned is the weighted least- */
/*             squares point,i.e. each spline sj(u) is a constant. in */
/*             this extreme case fp gives the upper bound fp0 for the */
/*             smoothing factor s. */
/*    ier=1  : error. the required storage space exceeds the available */
/*             storage space, as specified by the parameter nest. */
/*             probably causes : nest too small. if nest is already */
/*             large (say nest > m/2), it may also indicate that s is */
/*             too small */
/*             the approximation returned is the least-squares closed */
/*             curve according to the knots t(1),t(2),...,t(n). (n=nest) */
/*             the parameter fp gives the corresponding weighted sum of */
/*             squared residuals (fp>s). */
/*    ier=2  : error. a theoretically impossible result was found during */
/*             the iteration proces for finding a smoothing curve with */
/*             fp = s. probably causes : s too small. */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*             by the program) allowed for finding a smoothing curve */
/*             with fp=s has been reached. probably causes : s too small */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=10 : error. on entry, the input data are controlled on validity */
/*             the following restrictions must be satisfied. */
/*             -1<=iopt<=1, 1<=k<=5, m>1, nest>2*k+2, w(i)>0,i=1,2,...,m */
/*             0<=ipar<=1, 0<idim<=10, lwrk>=(k+1)*m+nest*(7+idim+5*k), */
/*             nc>=nest*idim, x(j)=x(idim*(m-1)+j), j=1,2,...,idim */
/*             if ipar=0: sum j=1,idim (x(i*idim+j)-x((i-1)*idim+j))**2>0 */
/*                        i=1,2,...,m-1. */
/*             if ipar=1: u(1)<u(2)<...<u(m) */
/*             if iopt=-1: 2*k+2<=n<=min(nest,m+2*k) */
/*                         u(1)<t(k+2)<t(k+3)<...<t(n-k-1)<u(m) */
/*                            (u(1)=0 and u(m)=1 in case ipar=0) */
/*                       the schoenberg-whitney conditions, i.e. there */
/*                       must be a subset of data points uu(j) with */
/*                       uu(j) = u(i) or u(i)+(u(m)-u(1)) such that */
/*                         t(j) < uu(j) < t(j+k+1), j=k+1,...,n-k-1 */
/*             if iopt>=0: s>=0 */
/*                         if s=0 : nest >= m+2*k */
/*             if one of these conditions is found to be violated,control */
/*             is immediately repassed to the calling program. in that */
/*             case there is no approximation returned. */

/*  further comments: */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the curve will be too smooth and signal will be */
/*   lost ; if s is too small the curve will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating curve if */
/*   s=0 and the weighted least-squares point if s is very large. */
/*   between these extremes, a properly chosen s will result in a good */
/*   compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the weights w(i). if these are */
/*   taken as 1/d(i) with d(i) an estimate of the standard deviation of */
/*   x(i), a good s-value should be found in the range (m-sqrt(2*m),m+ */
/*   sqrt(2*m)). if nothing is known about the statistical error in x(i) */
/*   each w(i) can be set equal to one and s determined by trial and */
/*   error, taking account of the comments above. the best is then to */
/*   start with a very large value of s ( to determine the weighted */
/*   least-squares point and the upper bound fp0 for s) and then to */
/*   progressively decrease the value of s ( say by a factor 10 in the */
/*   beginning, i.e. s=fp0/10, fp0/100,...and more carefully as the */
/*   approximating curve shows more detail) to obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt=1 the program will continue with the set of knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if clocur is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   curve underlying the data. but, if the computation mode iopt=1 is */
/*   used, the knots returned may also depend on the s-values at previous */
/*   calls (if these were smaller). therefore, if after a number of */
/*   trials with different s-values and iopt=1, the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   clocur once more with the selected value for s but now with iopt=0. */
/*   indeed, clocur may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */

/*   the form of the approximating curve can strongly be affected  by */
/*   the choice of the parameter values u(i). if there is no physical */
/*   reason for choosing a particular parameter u, often good results */
/*   will be obtained with the choice of clocur(in case ipar=0), i.e. */
/*        v(1)=0, v(i)=v(i-1)+q(i), i=2,...,m, u(i)=v(i)/v(m), i=1,..,m */
/*   where */
/*        q(i)= sqrt(sum j=1,idim (xj(i)-xj(i-1))**2 ) */
/*   other possibilities for q(i) are */
/*        q(i)= sum j=1,idim (xj(i)-xj(i-1))**2 */
/*        q(i)= sum j=1,idim abs(xj(i)-xj(i-1)) */
/*        q(i)= max j=1,idim abs(xj(i)-xj(i-1)) */
/*        q(i)= 1 */


/*  other subroutines required: */
/*    fpbacp,fpbspl,fpchep,fpclos,fpdisc,fpgivs,fpknot,fprati,fprota */

/*  references: */
/*   dierckx p. : algorithms for smoothing data with periodic and */
/*                parametric splines, computer graphics and image */
/*                processing 20 (1982) 171-184. */
/*   dierckx p. : algorithms for smoothing data with periodic and param- */
/*                etric splines, report tw55, dept. computer science, */
/*                k.u.leuven, 1981. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : may 1979 */
/*  latest update : march 1987 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  we set up the parameters tol and maxit */
    /* Parameter adjustments */
    --w;
    --u;
    --x;
    --iwrk;
    --t;
    --c__;
    --wrk;

    /* Function Body */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*iopt < -1 || *iopt > 1) {
	goto L90;
    }
    if (*ipar < 0 || *ipar > 1) {
	goto L90;
    }
    if (*idim <= 0 || *idim > 10) {
	goto L90;
    }
    if (*k <= 0 || *k > 5) {
	goto L90;
    }
    k1 = *k + 1;
    k2 = k1 + 1;
    nmin = k1 << 1;
    if (*m < 2 || *nest < nmin) {
	goto L90;
    }
    ncc = *nest * *idim;
    if (*mx < *m * *idim || *nc < ncc) {
	goto L90;
    }
    lwest = *m * k1 + *nest * (*idim + 7 + *k * 5);
    if (*lwrk < lwest) {
	goto L90;
    }
    i1 = *idim;
    i2 = *m * *idim;
    i__1 = *idim;
    for (j = 1; j <= i__1; ++j) {
	if (x[i1] != x[i2]) {
	    goto L90;
	}
	--i1;
	--i2;
/* L5: */
    }
    if (*ipar != 0 || *iopt > 0) {
	goto L40;
    }
    i1 = 0;
    i2 = *idim;
    u[1] = (float)0.;
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	dist = (float)0.;
	i__2 = *idim;
	for (j1 = 1; j1 <= i__2; ++j1) {
	    ++i1;
	    ++i2;
/* Computing 2nd power */
	    r__1 = x[i2] - x[i1];
	    dist += r__1 * r__1;
/* L10: */
	}
	u[i__] = u[i__ - 1] + sqrt(dist);
/* L20: */
    }
    if (u[*m] <= (float)0.) {
	goto L90;
    }
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	u[i__] /= u[*m];
/* L30: */
    }
    u[*m] = (float)1.;
L40:
    if (w[1] <= (float)0.) {
	goto L90;
    }
    m1 = *m - 1;
    i__1 = m1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (u[i__] >= u[i__ + 1] || w[i__] <= (float)0.) {
	    goto L90;
	}
/* L50: */
    }
    if (*iopt >= 0) {
	goto L70;
    }
    if (*n <= nmin || *n > *nest) {
	goto L90;
    }
    per = u[*m] - u[1];
    j1 = k1;
    t[j1] = u[1];
    i1 = *n - *k;
    t[i1] = u[*m];
    j2 = j1;
    i2 = i1;
    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	++i1;
	--i2;
	++j1;
	--j2;
	t[j2] = t[i2] - per;
	t[i1] = t[j1] + per;
/* L60: */
    }
    fpchep_(&u[1], m, &t[1], n, k, ier);
    if (*ier != 0) {
	goto L90;
    } else {
	goto L80;
    }
L70:
    if (*s < (float)0.) {
	goto L90;
    }
    if (*s == (float)0. && *nest < *m + (*k << 1)) {
	goto L90;
    }
    *ier = 0;
/* we partition the working space and determine the spline approximation. */
L80:
    ifp = 1;
    iz = ifp + *nest;
    ia1 = iz + ncc;
    ia2 = ia1 + *nest * k1;
    ib = ia2 + *nest * *k;
    ig1 = ib + *nest * k2;
    ig2 = ig1 + *nest * k2;
    iq = ig2 + *nest * k1;
    fpclos_(iopt, idim, m, &u[1], mx, &x[1], &w[1], k, s, nest, &tol, &maxit, 
	    &k1, &k2, n, &t[1], &ncc, &c__[1], fp, &wrk[ifp], &wrk[iz], &wrk[
	    ia1], &wrk[ia2], &wrk[ib], &wrk[ig1], &wrk[ig2], &wrk[iq], &iwrk[
	    1], ier);
L90:
    return 0;
} /* clocur_ */

/* Subroutine */ int cocosp_(m, x, y, w, n, t, e, maxtr, maxbin, c__, sq, sx, 
	bind, wrk, lwrk, iwrk, kwrk, ier)
integer *m;
real *x, *y, *w;
integer *n;
real *t, *e;
integer *maxtr, *maxbin;
real *c__, *sq, *sx;
logical *bind;
real *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, kwest, lwest, n6, ia, ib, ic, mb, ji, jl;
    extern /* Subroutine */ int fpchec_();
    static integer iq, nm, jr, iu, ju, iz;
    extern /* Subroutine */ int fpcosp_();
    static integer jib, jjb;
    static real one;
    static integer izz;

/*  given the set of data points (x(i),y(i)) and the set of positive */
/*  numbers w(i),i=1,2,...,m, subroutine cocosp determines the weighted */
/*  least-squares cubic spline s(x) with given knots t(j),j=1,2,...,n */
/*  which satisfies the following concavity/convexity conditions */
/*      s''(t(j+3))*e(j) <= 0, j=1,2,...n-6 */
/*  the fit is given in the b-spline representation( b-spline coef- */
/*  ficients c(j),j=1,2,...n-4) and can be evaluated by means of */
/*  subroutine splev. */

/*  calling sequence: */
/*     call cocosp(m,x,y,w,n,t,e,maxtr,maxbin,c,sq,sx,bind,wrk, */
/*    * lwrk,iwrk,kwrk,ier) */

/*  parameters: */
/*    m   : integer. on entry m must specify the number of data points. */
/*          m > 3. unchanged on exit. */
/*    x   : real array of dimension at least (m). before entry, x(i) */
/*          must be set to the i-th value of the independent variable x, */
/*          for i=1,2,...,m. these values must be supplied in strictly */
/*          ascending order. unchanged on exit. */
/*    y   : real array of dimension at least (m). before entry, y(i) */
/*          must be set to the i-th value of the dependent variable y, */
/*          for i=1,2,...,m. unchanged on exit. */
/*    w   : real array of dimension at least (m). before entry, w(i) */
/*          must be set to the i-th value in the set of weights. the */
/*          w(i) must be strictly positive. unchanged on exit. */
/*    n   : integer. on entry n must contain the total number of knots */
/*          of the cubic spline. m+4>=n>=8. unchanged on exit. */
/*    t   : real array of dimension at least (n). before entry, this */
/*          array must contain the knots of the spline, i.e. the position */
/*          of the interior knots t(5),t(6),...,t(n-4) as well as the */
/*          position of the boundary knots t(1),t(2),t(3),t(4) and t(n-3) */
/*          t(n-2),t(n-1),t(n) needed for the b-spline representation. */
/*          unchanged on exit. see also the restrictions (ier=10). */
/*    e   : real array of dimension at least (n). before entry, e(j) */
/*          must be set to 1 if s(x) must be locally concave at t(j+3), */
/*          to (-1) if s(x) must be locally convex at t(j+3) and to 0 */
/*          if no convexity constraint is imposed at t(j+3),j=1,2,..,n-6. */
/*          e(n-5),...,e(n) are not used. unchanged on exit. */
/*  maxtr : integer. on entry maxtr must contain an over-estimate of the */
/*          total number of records in the used tree structure, to indic- */
/*          ate the storage space available to the routine. maxtr >=1 */
/*          in most practical situation maxtr=100 will be sufficient. */
/*          always large enough is */
/*                         n-5       n-6 */
/*              maxtr =  (     ) + (     )  with l the greatest */
/*                          l        l+1 */
/*          integer <= (n-6)/2 . unchanged on exit. */
/*  maxbin: integer. on entry maxbin must contain an over-estimate of the */
/*          number of knots where s(x) will have a zero second derivative */
/*          maxbin >=1. in most practical situation maxbin = 10 will be */
/*          sufficient. always large enough is maxbin=n-6. */
/*          unchanged on exit. */
/*    c   : real array of dimension at least (n). */
/*          on succesful exit, this array will contain the coefficients */
/*          c(1),c(2),..,c(n-4) in the b-spline representation of s(x) */
/*    sq  : real. on succesful exit, sq contains the weighted sum of */
/*          squared residuals of the spline approximation returned. */
/*    sx  : real array of dimension at least m. on succesful exit */
/*          this array will contain the spline values s(x(i)),i=1,...,m */
/*   bind : logical array of dimension at least (n). on succesful exit */
/*          this array will indicate the knots where s''(x)=0, i.e. */
/*                s''(t(j+3)) .eq. 0 if  bind(j) = .true. */
/*                s''(t(j+3)) .ne. 0 if  bind(j) = .false., j=1,2,...,n-6 */
/*   wrk  : real array of dimension at least  m*4+n*7+maxbin*(maxbin+n+1) */
/*          used as working space. */
/*   lwrk : integer. on entry,lwrk must specify the actual dimension of */
/*          the array wrk as declared in the calling (sub)program.lwrk */
/*          must not be too small (see wrk). unchanged on exit. */
/*   iwrk : integer array of dimension at least (maxtr*4+2*(maxbin+1)) */
/*          used as working space. */
/*   kwrk : integer. on entry,kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. kwrk */
/*          must not be too small (see iwrk). unchanged on exit. */
/*   ier   : integer. error flag */
/*      ier=0 : succesful exit. */
/*      ier>0 : abnormal termination: no approximation is returned */
/*        ier=1  : the number of knots where s''(x)=0 exceeds maxbin. */
/*                 probably causes : maxbin too small. */
/*        ier=2  : the number of records in the tree structure exceeds */
/*                 maxtr. */
/*                 probably causes : maxtr too small. */
/*        ier=3  : the algoritm finds no solution to the posed quadratic */
/*                 programming problem. */
/*                 probably causes : rounding errors. */
/*        ier=10 : on entry, the input data are controlled on validity. */
/*                 the following restrictions must be satisfied */
/*                   m>3, maxtr>=1, maxbin>=1, 8<=n<=m+4,w(i) > 0, */
/*                   x(1)<x(2)<...<x(m), t(1)<=t(2)<=t(3)<=t(4)<=x(1), */
/*                   x(1)<t(5)<t(6)<...<t(n-4)<x(m)<=t(n-3)<=...<=t(n), */
/*                   kwrk>=maxtr*4+2*(maxbin+1), */
/*                   lwrk>=m*4+n*7+maxbin*(maxbin+n+1), */
/*                   the schoenberg-whitney conditions, i.e. there must */
/*                   be a subset of data points xx(j) such that */
/*                     t(j) < xx(j) < t(j+4), j=1,2,...,n-4 */
/*                 if one of these restrictions is found to be violated */
/*                 control is immediately repassed to the calling program */


/*  other subroutines required: */
/*    fpcosp,fpbspl,fpadno,fpdeno,fpseno,fpfrno,fpchec */

/*  references: */
/*   dierckx p. : an algorithm for cubic spline fitting with convexity */
/*                constraints, computing 24 (1980) 349-371. */
/*   dierckx p. : an algorithm for least-squares cubic spline fitting */
/*                with convexity and concavity constraints, report tw39, */
/*                dept. computer science, k.u.leuven, 1978. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*   p. dierckx */
/*   dept. computer science, k.u.leuven */
/*   celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*   e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : march 1978 */
/*  latest update : march 1987. */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  set constant */
    /* Parameter adjustments */
    --sx;
    --w;
    --y;
    --x;
    --bind;
    --c__;
    --e;
    --t;
    --wrk;
    --iwrk;

    /* Function Body */
    one = (float)1.;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*m < 4 || *n < 8) {
	goto L40;
    }
    if (*maxtr < 1 || *maxbin < 1) {
	goto L40;
    }
    lwest = *n * 7 + (*m << 2) + *maxbin * (*n + 1 + *maxbin);
    kwest = (*maxtr << 2) + (*maxbin + 1 << 1);
    if (*lwrk < lwest || *kwrk < kwest) {
	goto L40;
    }
    if (w[1] <= (float)0.) {
	goto L40;
    }
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (x[i__ - 1] >= x[i__] || w[i__] <= (float)0.) {
	    goto L40;
	}
/* L10: */
    }
    fpchec_(&x[1], m, &t[1], n, &c__3, ier);
    if (*ier != 0) {
	goto L40;
    } else {
	goto L20;
    }
/*  set numbers e(i) */
L20:
    n6 = *n - 6;
    i__1 = n6;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (e[i__] > (float)0.) {
	    e[i__] = one;
	}
	if (e[i__] < (float)0.) {
	    e[i__] = -one;
	}
/* L30: */
    }
/*  we partition the working space and determine the spline approximation */
    nm = *n + *maxbin;
    mb = *maxbin + 1;
    ia = 1;
    ib = ia + (*n << 2);
    ic = ib + nm * *maxbin;
    iz = ic + *n;
    izz = iz + *n;
    iu = izz + *n;
    iq = iu + *maxbin;
    ji = 1;
    ju = ji + *maxtr;
    jl = ju + *maxtr;
    jr = jl + *maxtr;
    jjb = jr + *maxtr;
    jib = jjb + mb;
    fpcosp_(m, &x[1], &y[1], &w[1], n, &t[1], &e[1], maxtr, maxbin, &c__[1], 
	    sq, &sx[1], &bind[1], &nm, &mb, &wrk[ia], &wrk[ib], &wrk[ic], &
	    wrk[iz], &wrk[izz], &wrk[iu], &wrk[iq], &iwrk[ji], &iwrk[ju], &
	    iwrk[jl], &iwrk[jr], &iwrk[jjb], &iwrk[jib], ier);
L40:
    return 0;
} /* cocosp_ */

/* Subroutine */ int concon_(iopt, m, x, y, w, v, s, nest, maxtr, maxbin, n, 
	t, c__, sq, sx, bind, wrk, lwrk, iwrk, kwrk, ier)
integer *iopt, *m;
real *x, *y, *w, *v, *s;
integer *nest, *maxtr, *maxbin, *n;
real *t, *c__, *sq, *sx;
logical *bind;
real *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, kwest, lwest, ie, iw;
    extern /* Subroutine */ int fpcoco_();
    static real one;
    static integer lww;

/*  given the set of data points (x(i),y(i)) and the set of positive */
/*  numbers w(i), i=1,2,...,m,subroutine concon determines a cubic spline */
/*  approximation s(x) which satisfies the following local convexity */
/*  constraints  s''(x(i))*v(i) <= 0, i=1,2,...,m. */
/*  the number of knots n and the position t(j),j=1,2,...n is chosen */
/*  automatically by the routine in a way that */
/*       sq = sum((w(i)*(y(i)-s(x(i))))**2) be <= s. */
/*  the fit is given in the b-spline representation (b-spline coef- */
/*  ficients c(j),j=1,2,...n-4) and can be evaluated by means of */
/*  subroutine splev. */

/*  calling sequence: */

/*     call concon(iopt,m,x,y,w,v,s,nest,maxtr,maxbin,n,t,c,sq, */
/*    * sx,bind,wrk,lwrk,iwrk,kwrk,ier) */

/*  parameters: */
/*    iopt: integer flag. */
/*          if iopt=0, the routine will start with the minimal number of */
/*          knots to guarantee that the convexity conditions will be */
/*          satisfied. if iopt=1, the routine will continue with the set */
/*          of knots found at the last call of the routine. */
/*          attention: a call with iopt=1 must always be immediately */
/*          preceded by another call with iopt=1 or iopt=0. */
/*          unchanged on exit. */
/*    m   : integer. on entry m must specify the number of data points. */
/*          m > 3. unchanged on exit. */
/*    x   : real array of dimension at least (m). before entry, x(i) */
/*          must be set to the i-th value of the independent variable x, */
/*          for i=1,2,...,m. these values must be supplied in strictly */
/*          ascending order. unchanged on exit. */
/*    y   : real array of dimension at least (m). before entry, y(i) */
/*          must be set to the i-th value of the dependent variable y, */
/*          for i=1,2,...,m. unchanged on exit. */
/*    w   : real array of dimension at least (m). before entry, w(i) */
/*          must be set to the i-th value in the set of weights. the */
/*          w(i) must be strictly positive. unchanged on exit. */
/*    v   : real array of dimension at least (m). before entry, v(i) */
/*          must be set to 1 if s(x) must be locally concave at x(i), */
/*          to (-1) if s(x) must be locally convex at x(i) and to 0 */
/*          if no convexity constraint is imposed at x(i). */
/*    s   : real. on entry s must specify an over-estimate for the */
/*          the weighted sum of squared residuals sq of the requested */
/*          spline. s >=0. unchanged on exit. */
/*   nest : integer. on entry nest must contain an over-estimate of the */
/*          total number of knots of the spline returned, to indicate */
/*          the storage space available to the routine. nest >=8. */
/*          in most practical situation nest=m/2 will be sufficient. */
/*          always large enough is  nest=m+4. unchanged on exit. */
/*  maxtr : integer. on entry maxtr must contain an over-estimate of the */
/*          total number of records in the used tree structure, to indic- */
/*          ate the storage space available to the routine. maxtr >=1 */
/*          in most practical situation maxtr=100 will be sufficient. */
/*          always large enough is */
/*                         nest-5      nest-6 */
/*              maxtr =  (       ) + (        )  with l the greatest */
/*                           l          l+1 */
/*          integer <= (nest-6)/2 . unchanged on exit. */
/*  maxbin: integer. on entry maxbin must contain an over-estimate of the */
/*          number of knots where s(x) will have a zero second derivative */
/*          maxbin >=1. in most practical situation maxbin = 10 will be */
/*          sufficient. always large enough is maxbin=nest-6. */
/*          unchanged on exit. */
/*    n   : integer. */
/*          on exit with ier <=0, n will contain the total number of */
/*          knots of the spline approximation returned. if the comput- */
/*          ation mode iopt=1 is used this value of n should be left */
/*          unchanged between subsequent calls. */
/*    t   : real array of dimension at least (nest). */
/*          on exit with ier<=0, this array will contain the knots of the */
/*          spline,i.e. the position of the interior knots t(5),t(6),..., */
/*          t(n-4) as well as the position of the additional knots */
/*          t(1)=t(2)=t(3)=t(4)=x(1) and t(n-3)=t(n-2)=t(n-1)=t(n)=x(m) */
/*          needed for the the b-spline representation. */
/*          if the computation mode iopt=1 is used, the values of t(1), */
/*          t(2),...,t(n) should be left unchanged between subsequent */
/*          calls. */
/*    c   : real array of dimension at least (nest). */
/*          on succesful exit, this array will contain the coefficients */
/*          c(1),c(2),..,c(n-4) in the b-spline representation of s(x) */
/*    sq  : real. unless ier>0 , sq contains the weighted sum of */
/*          squared residuals of the spline approximation returned. */
/*    sx  : real array of dimension at least m. on exit with ier<=0 */
/*          this array will contain the spline values s(x(i)),i=1,...,m */
/*          if the computation mode iopt=1 is used, the values of sx(1), */
/*          sx(2),...,sx(m) should be left unchanged between subsequent */
/*          calls. */
/*    bind: logical array of dimension at least nest. on exit with ier<=0 */
/*          this array will indicate the knots where s''(x)=0, i.e. */
/*                s''(t(j+3)) .eq. 0 if  bind(j) = .true. */
/*                s''(t(j+3)) .ne. 0 if  bind(j) = .false., j=1,2,...,n-6 */
/*          if the computation mode iopt=1 is used, the values of bind(1) */
/*          ,...,bind(n-6) should be left unchanged between subsequent */
/*          calls. */
/*   wrk  : real array of dimension at least (m*4+nest*8+maxbin*(maxbin+ */
/*          nest+1)). used as working space. */
/*   lwrk : integer. on entry,lwrk must specify the actual dimension of */
/*          the array wrk as declared in the calling (sub)program.lwrk */
/*          must not be too small (see wrk). unchanged on exit. */
/*   iwrk : integer array of dimension at least (maxtr*4+2*(maxbin+1)) */
/*          used as working space. */
/*   kwrk : integer. on entry,kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. kwrk */
/*          must not be too small (see iwrk). unchanged on exit. */
/*   ier   : integer. error flag */
/*      ier=0 : normal return, s(x) satisfies the concavity/convexity */
/*              constraints and sq <= s. */
/*      ier<0 : abnormal termination: s(x) satisfies the concavity/ */
/*              convexity constraints but sq > s. */
/*        ier=-3 : the requested storage space exceeds the available */
/*                 storage space as specified by the parameter nest. */
/*                 probably causes: nest too small. if nest is already */
/*                 large (say nest > m/2), it may also indicate that s */
/*                 is too small. */
/*                 the approximation returned is the least-squares cubic */
/*                 spline according to the knots t(1),...,t(n) (n=nest) */
/*                 which satisfies the convexity constraints. */
/*        ier=-2 : the maximal number of knots n=m+4 has been reached. */
/*                 probably causes: s too small. */
/*        ier=-1 : the number of knots n is less than the maximal number */
/*                 m+4 but concon finds that adding one or more knots */
/*                 will not further reduce the value of sq. */
/*                 probably causes : s too small. */
/*      ier>0 : abnormal termination: no approximation is returned */
/*        ier=1  : the number of knots where s''(x)=0 exceeds maxbin. */
/*                 probably causes : maxbin too small. */
/*        ier=2  : the number of records in the tree structure exceeds */
/*                 maxtr. */
/*                 probably causes : maxtr too small. */
/*        ier=3  : the algoritm finds no solution to the posed quadratic */
/*                 programming problem. */
/*                 probably causes : rounding errors. */
/*        ier=4  : the minimum number of knots (given by n) to guarantee */
/*                 that the concavity/convexity conditions will be */
/*                 satisfied is greater than nest. */
/*                 probably causes: nest too small. */
/*        ier=5  : the minimum number of knots (given by n) to guarantee */
/*                 that the concavity/convexity conditions will be */
/*                 satisfied is greater than m+4. */
/*                 probably causes: strongly alternating convexity and */
/*                 concavity conditions. normally the situation can be */
/*                 coped with by adding n-m-4 extra data points (found */
/*                 by linear interpolation e.g.) with a small weight w(i) */
/*                 and a v(i) number equal to zero. */
/*        ier=10 : on entry, the input data are controlled on validity. */
/*                 the following restrictions must be satisfied */
/*                   0<=iopt<=1, m>3, nest>=8, s>=0, maxtr>=1, maxbin>=1, */
/*                   kwrk>=maxtr*4+2*(maxbin+1), w(i)>0, x(i) < x(i+1), */
/*                   lwrk>=m*4+nest*8+maxbin*(maxbin+nest+1) */
/*                 if one of these restrictions is found to be violated */
/*                 control is immediately repassed to the calling program */

/*  further comments: */
/*    as an example of the use of the computation mode iopt=1, the */
/*    following program segment will cause concon to return control */
/*    each time a spline with a new set of knots has been computed. */
/*     ............. */
/*     iopt = 0 */
/*     s = 0.1e+60  (s very large) */
/*     do 10 i=1,m */
/*       call concon(iopt,m,x,y,w,v,s,nest,maxtr,maxbin,n,t,c,sq,sx, */
/*    *  bind,wrk,lwrk,iwrk,kwrk,ier) */
/*       ...... */
/*       s = sq */
/*       iopt=1 */
/* 10  continue */
/*     ............. */

/*  other subroutines required: */
/*    fpcoco,fpcosp,fpbspl,fpadno,fpdeno,fpseno,fpfrno */

/*  references: */
/*   dierckx p. : an algorithm for cubic spline fitting with convexity */
/*                constraints, computing 24 (1980) 349-371. */
/*   dierckx p. : an algorithm for least-squares cubic spline fitting */
/*                with convexity and concavity constraints, report tw39, */
/*                dept. computer science, k.u.leuven, 1978. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*   p. dierckx */
/*   dept. computer science, k.u.leuven */
/*   celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*   e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : march 1978 */
/*  latest update : march 1987. */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  set constant */
    /* Parameter adjustments */
    --sx;
    --v;
    --w;
    --y;
    --x;
    --bind;
    --c__;
    --t;
    --wrk;
    --iwrk;

    /* Function Body */
    one = (float)1.;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*iopt < 0 || *iopt > 1) {
	goto L30;
    }
    if (*m < 4 || *nest < 8) {
	goto L30;
    }
    if (*s < (float)0.) {
	goto L30;
    }
    if (*maxtr < 1 || *maxbin < 1) {
	goto L30;
    }
    lwest = (*nest << 3) + (*m << 2) + *maxbin * (*nest + 1 + *maxbin);
    kwest = (*maxtr << 2) + (*maxbin + 1 << 1);
    if (*lwrk < lwest || *kwrk < kwest) {
	goto L30;
    }
    if (*iopt > 0) {
	goto L20;
    }
    if (w[1] <= (float)0.) {
	goto L30;
    }
    if (v[1] > (float)0.) {
	v[1] = one;
    }
    if (v[1] < (float)0.) {
	v[1] = -one;
    }
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (x[i__ - 1] >= x[i__] || w[i__] <= (float)0.) {
	    goto L30;
	}
	if (v[i__] > (float)0.) {
	    v[i__] = one;
	}
	if (v[i__] < (float)0.) {
	    v[i__] = -one;
	}
/* L10: */
    }
L20:
    *ier = 0;
/*  we partition the working space and determine the spline approximation */
    ie = 1;
    iw = ie + *nest;
    lww = *lwrk - *nest;
    fpcoco_(iopt, m, &x[1], &y[1], &w[1], &v[1], s, nest, maxtr, maxbin, n, &
	    t[1], &c__[1], sq, &sx[1], &bind[1], &wrk[ie], &wrk[iw], &lww, &
	    iwrk[1], kwrk, ier);
L30:
    return 0;
} /* concon_ */

/* Subroutine */ int concur_(iopt, idim, m, u, mx, x, xx, w, ib, db, nb, ie, 
	de, ne, k, s, nest, n, t, nc, c__, np, cp, fp, wrk, lwrk, iwrk, ier)
integer *iopt, *idim, *m;
real *u;
integer *mx;
real *x, *xx, *w;
integer *ib;
real *db;
integer *nb, *ie;
real *de;
integer *ne, *k;
real *s;
integer *nest, *n;
real *t;
integer *nc;
real *c__;
integer *np;
real *cp, *fp, *wrk;
integer *lwrk, *iwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer mmin, nmin, nmax, i__, j, maxit;
    extern /* Subroutine */ int curev_();
    static integer k1, k2, lwest, ja, jb, jg, kk;
    extern /* Subroutine */ int fpched_();
    static integer jq, jz;
    extern /* Subroutine */ int fpadpo_(), fpcons_();
    static integer ib1;
    extern /* Subroutine */ int fppocu_();
    static integer ie1, ncc, jfp;
    static real tol;
    static integer mxx;

/*  given the ordered set of m points x(i) in the idim-dimensional space */
/*  and given also a corresponding set of strictly increasing values u(i) */
/*  and the set of positive numbers w(i),i=1,2,...,m, subroutine concur */
/*  determines a smooth approximating spline curve s(u), i.e. */
/*      x1 = s1(u) */
/*      x2 = s2(u)      ub = u(1) <= u <= u(m) = ue */
/*      ......... */
/*      xidim = sidim(u) */
/*  with sj(u),j=1,2,...,idim spline functions of odd degree k with */
/*  common knots t(j),j=1,2,...,n. */
/*  in addition these splines will satisfy the following boundary */
/*  constraints        (l) */
/*      if ib > 0 :  sj   (u(1)) = db(idim*l+j) ,l=0,1,...,ib-1 */
/*  and                (l) */
/*      if ie > 0 :  sj   (u(m)) = de(idim*l+j) ,l=0,1,...,ie-1. */
/*  if iopt=-1 concur calculates the weighted least-squares spline curve */
/*  according to a given set of knots. */
/*  if iopt>=0 the number of knots of the splines sj(u) and the position */
/*  t(j),j=1,2,...,n is chosen automatically by the routine. the smooth- */
/*  ness of s(u) is then achieved by minimalizing the discontinuity */
/*  jumps of the k-th derivative of s(u) at the knots t(j),j=k+2,k+3,..., */
/*  n-k-1. the amount of smoothness is determined by the condition that */
/*  f(p)=sum((w(i)*dist(x(i),s(u(i))))**2) be <= s, with s a given non- */
/*  negative constant, called the smoothing factor. */
/*  the fit s(u) is given in the b-spline representation and can be */
/*  evaluated by means of subroutine curev. */

/*  calling sequence: */
/*     call concur(iopt,idim,m,u,mx,x,xx,w,ib,db,nb,ie,de,ne,k,s,nest,n, */
/*    * t,nc,c,np,cp,fp,wrk,lwrk,iwrk,ier) */

/*  parameters: */
/*   iopt  : integer flag. on entry iopt must specify whether a weighted */
/*           least-squares spline curve (iopt=-1) or a smoothing spline */
/*           curve (iopt=0 or 1) must be determined.if iopt=0 the routine */
/*           will start with an initial set of knots t(i)=ub,t(i+k+1)=ue, */
/*           i=1,2,...,k+1. if iopt=1 the routine will continue with the */
/*           knots found at the last call of the routine. */
/*           attention: a call with iopt=1 must always be immediately */
/*           preceded by another call with iopt=1 or iopt=0. */
/*           unchanged on exit. */
/*   idim  : integer. on entry idim must specify the dimension of the */
/*           curve. 0 < idim < 11. */
/*           unchanged on exit. */
/*   m     : integer. on entry m must specify the number of data points. */
/*           m > k-max(ib-1,0)-max(ie-1,0). unchanged on exit. */
/*   u     : real array of dimension at least (m). before entry, */
/*           u(i) must be set to the i-th value of the parameter variable */
/*           u for i=1,2,...,m. these values must be supplied in */
/*           strictly ascending order and will be unchanged on exit. */
/*   mx    : integer. on entry mx must specify the actual dimension of */
/*           the arrays x and xx as declared in the calling (sub)program */
/*           mx must not be too small (see x). unchanged on exit. */
/*   x     : real array of dimension at least idim*m. */
/*           before entry, x(idim*(i-1)+j) must contain the j-th coord- */
/*           inate of the i-th data point for i=1,2,...,m and j=1,2,..., */
/*           idim. unchanged on exit. */
/*   xx    : real array of dimension at least idim*m. */
/*           used as working space. on exit xx contains the coordinates */
/*           of the data points to which a spline curve with zero deriv- */
/*           ative constraints has been determined. */
/*           if the computation mode iopt =1 is used xx should be left */
/*           unchanged between calls. */
/*   w     : real array of dimension at least (m). before entry, w(i) */
/*           must be set to the i-th value in the set of weights. the */
/*           w(i) must be strictly positive. unchanged on exit. */
/*           see also further comments. */
/*   ib    : integer. on entry ib must specify the number of derivative */
/*           constraints for the curve at the begin point. 0<=ib<=(k+1)/2 */
/*           unchanged on exit. */
/*   db    : real array of dimension nb. before entry db(idim*l+j) must */
/*           contain the l-th order derivative of sj(u) at u=u(1) for */
/*           j=1,2,...,idim and l=0,1,...,ib-1 (if ib>0). */
/*           unchanged on exit. */
/*   nb    : integer, specifying the dimension of db. nb>=max(1,idim*ib) */
/*           unchanged on exit. */
/*   ie    : integer. on entry ie must specify the number of derivative */
/*           constraints for the curve at the end point. 0<=ie<=(k+1)/2 */
/*           unchanged on exit. */
/*   de    : real array of dimension ne. before entry de(idim*l+j) must */
/*           contain the l-th order derivative of sj(u) at u=u(m) for */
/*           j=1,2,...,idim and l=0,1,...,ie-1 (if ie>0). */
/*           unchanged on exit. */
/*   ne    : integer, specifying the dimension of de. ne>=max(1,idim*ie) */
/*           unchanged on exit. */
/*   k     : integer. on entry k must specify the degree of the splines. */
/*           k=1,3 or 5. */
/*           unchanged on exit. */
/*   s     : real.on entry (in case iopt>=0) s must specify the smoothing */
/*           factor. s >=0. unchanged on exit. */
/*           for advice on the choice of s see further comments. */
/*   nest  : integer. on entry nest must contain an over-estimate of the */
/*           total number of knots of the splines returned, to indicate */
/*           the storage space available to the routine. nest >=2*k+2. */
/*           in most practical situation nest=m/2 will be sufficient. */
/*           always large enough is nest=m+k+1+max(0,ib-1)+max(0,ie-1), */
/*           the number of knots needed for interpolation (s=0). */
/*           unchanged on exit. */
/*   n     : integer. */
/*           unless ier = 10 (in case iopt >=0), n will contain the */
/*           total number of knots of the smoothing spline curve returned */
/*           if the computation mode iopt=1 is used this value of n */
/*           should be left unchanged between subsequent calls. */
/*           in case iopt=-1, the value of n must be specified on entry. */
/*   t     : real array of dimension at least (nest). */
/*           on succesful exit, this array will contain the knots of the */
/*           spline curve,i.e. the position of the interior knots t(k+2), */
/*           t(k+3),..,t(n-k-1) as well as the position of the additional */
/*           t(1)=t(2)=...=t(k+1)=ub and t(n-k)=...=t(n)=ue needed for */
/*           the b-spline representation. */
/*           if the computation mode iopt=1 is used, the values of t(1), */
/*           t(2),...,t(n) should be left unchanged between subsequent */
/*           calls. if the computation mode iopt=-1 is used, the values */
/*           t(k+2),...,t(n-k-1) must be supplied by the user, before */
/*           entry. see also the restrictions (ier=10). */
/*   nc    : integer. on entry nc must specify the actual dimension of */
/*           the array c as declared in the calling (sub)program. nc */
/*           must not be too small (see c). unchanged on exit. */
/*   c     : real array of dimension at least (nest*idim). */
/*           on succesful exit, this array will contain the coefficients */
/*           in the b-spline representation of the spline curve s(u),i.e. */
/*           the b-spline coefficients of the spline sj(u) will be given */
/*           in c(n*(j-1)+i),i=1,2,...,n-k-1 for j=1,2,...,idim. */
/*   cp    : real array of dimension at least 2*(k+1)*idim. */
/*           on exit cp will contain the b-spline coefficients of a */
/*           polynomial curve which satisfies the boundary constraints. */
/*           if the computation mode iopt =1 is used cp should be left */
/*           unchanged between calls. */
/*   np    : integer. on entry np must specify the actual dimension of */
/*           the array cp as declared in the calling (sub)program. np */
/*           must not be too small (see cp). unchanged on exit. */
/*   fp    : real. unless ier = 10, fp contains the weighted sum of */
/*           squared residuals of the spline curve returned. */
/*   wrk   : real array of dimension at least m*(k+1)+nest*(6+idim+3*k). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used, the values wrk(1),...,wrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   lwrk  : integer. on entry,lwrk must specify the actual dimension of */
/*           the array wrk as declared in the calling (sub)program. lwrk */
/*           must not be too small (see wrk). unchanged on exit. */
/*   iwrk  : integer array of dimension at least (nest). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used,the values iwrk(1),...,iwrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   ier   : integer. unless the routine detects an error, ier contains a */
/*           non-positive value on exit, i.e. */
/*    ier=0  : normal return. the curve returned has a residual sum of */
/*             squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*             ive tolerance set to 0.001 by the program. */
/*    ier=-1 : normal return. the curve returned is an interpolating */
/*             spline curve, satisfying the constraints (fp=0). */
/*    ier=-2 : normal return. the curve returned is the weighted least- */
/*             squares polynomial curve of degree k, satisfying the */
/*             constraints. in this extreme case fp gives the upper */
/*             bound fp0 for the smoothing factor s. */
/*    ier=1  : error. the required storage space exceeds the available */
/*             storage space, as specified by the parameter nest. */
/*             probably causes : nest too small. if nest is already */
/*             large (say nest > m/2), it may also indicate that s is */
/*             too small */
/*             the approximation returned is the least-squares spline */
/*             curve according to the knots t(1),t(2),...,t(n). (n=nest) */
/*             the parameter fp gives the corresponding weighted sum of */
/*             squared residuals (fp>s). */
/*    ier=2  : error. a theoretically impossible result was found during */
/*             the iteration proces for finding a smoothing spline curve */
/*             with fp = s. probably causes : s too small. */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*             by the program) allowed for finding a smoothing curve */
/*             with fp=s has been reached. probably causes : s too small */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=10 : error. on entry, the input data are controlled on validity */
/*             the following restrictions must be satisfied. */
/*             -1<=iopt<=1, k = 1,3 or 5, m>k-max(0,ib-1)-max(0,ie-1), */
/*             nest>=2k+2, 0<idim<=10, lwrk>=(k+1)*m+nest*(6+idim+3*k), */
/*             nc >=nest*idim ,u(1)<u(2)<...<u(m),w(i)>0 i=1,2,...,m, */
/*             mx>=idim*m,0<=ib<=(k+1)/2,0<=ie<=(k+1)/2,nb>=1,ne>=1, */
/*             nb>=ib*idim,ne>=ib*idim,np>=2*(k+1)*idim, */
/*             if iopt=-1:2*k+2<=n<=min(nest,mmax) with mmax = m+k+1+ */
/*                        max(0,ib-1)+max(0,ie-1) */
/*                        u(1)<t(k+2)<t(k+3)<...<t(n-k-1)<u(m) */
/*                       the schoenberg-whitney conditions, i.e. there */
/*                       must be a subset of data points uu(j) such that */
/*                         t(j) < uu(j) < t(j+k+1), j=1+max(0,ib-1),... */
/*                                                   ,n+k-1-max(0,ie-1) */
/*             if iopt>=0: s>=0 */
/*                         if s=0 : nest >=mmax (see above) */
/*             if one of these conditions is found to be violated,control */
/*             is immediately repassed to the calling program. in that */
/*             case there is no approximation returned. */

/*  further comments: */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the curve will be too smooth and signal will be */
/*   lost ; if s is too small the curve will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating curve if */
/*   s=0 and the least-squares polynomial curve of degree k if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the weights w(i). if these are */
/*   taken as 1/d(i) with d(i) an estimate of the standard deviation of */
/*   x(i), a good s-value should be found in the range (m-sqrt(2*m),m+ */
/*   sqrt(2*m)). if nothing is known about the statistical error in x(i) */
/*   each w(i) can be set equal to one and s determined by trial and */
/*   error, taking account of the comments above. the best is then to */
/*   start with a very large value of s ( to determine the least-squares */
/*   polynomial curve and the upper bound fp0 for s) and then to */
/*   progressively decrease the value of s ( say by a factor 10 in the */
/*   beginning, i.e. s=fp0/10, fp0/100,...and more carefully as the */
/*   approximating curve shows more detail) to obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt=1 the program will continue with the set of knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if concur is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   curve underlying the data. but, if the computation mode iopt=1 is */
/*   used, the knots returned may also depend on the s-values at previous */
/*   calls (if these were smaller). therefore, if after a number of */
/*   trials with different s-values and iopt=1, the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   concur once more with the selected value for s but now with iopt=0. */
/*   indeed, concur may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */

/*   the form of the approximating curve can strongly be affected by */
/*   the choice of the parameter values u(i). if there is no physical */
/*   reason for choosing a particular parameter u, often good results */
/*   will be obtained with the choice */
/*        v(1)=0, v(i)=v(i-1)+q(i), i=2,...,m, u(i)=v(i)/v(m), i=1,..,m */
/*   where */
/*        q(i)= sqrt(sum j=1,idim (xj(i)-xj(i-1))**2 ) */
/*   other possibilities for q(i) are */
/*        q(i)= sum j=1,idim (xj(i)-xj(i-1))**2 */
/*        q(i)= sum j=1,idim abs(xj(i)-xj(i-1)) */
/*        q(i)= max j=1,idim abs(xj(i)-xj(i-1)) */
/*        q(i)= 1 */

/*  other subroutines required: */
/*    fpback,fpbspl,fpched,fpcons,fpdisc,fpgivs,fpknot,fprati,fprota */
/*    curev,fppocu,fpadpo,fpinst */

/*  references: */
/*   dierckx p. : algorithms for smoothing data with periodic and */
/*                parametric splines, computer graphics and image */
/*                processing 20 (1982) 171-184. */
/*   dierckx p. : algorithms for smoothing data with periodic and param- */
/*                etric splines, report tw55, dept. computer science, */
/*                k.u.leuven, 1981. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : may 1979 */
/*  latest update : march 1987 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/* ..function references */
/*  .. */
/*  we set up the parameters tol and maxit */
    /* Parameter adjustments */
    --w;
    --u;
    --xx;
    --x;
    --db;
    --de;
    --iwrk;
    --t;
    --c__;
    --cp;
    --wrk;

    /* Function Body */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*iopt < -1 || *iopt > 1) {
	goto L90;
    }
    if (*idim <= 0 || *idim > 10) {
	goto L90;
    }
    if (*k <= 0 || *k > 5) {
	goto L90;
    }
    k1 = *k + 1;
    kk = k1 / 2;
    if (kk << 1 != k1) {
	goto L90;
    }
    k2 = k1 + 1;
    if (*ib < 0 || *ib > kk) {
	goto L90;
    }
    if (*ie < 0 || *ie > kk) {
	goto L90;
    }
    nmin = k1 << 1;
/* Computing MAX */
    i__1 = 0, i__2 = *ib - 1;
    ib1 = max(i__1,i__2);
/* Computing MAX */
    i__1 = 0, i__2 = *ie - 1;
    ie1 = max(i__1,i__2);
    mmin = k1 - ib1 - ie1;
    if (*m < mmin || *nest < nmin) {
	goto L90;
    }
    if (*nb < *idim * *ib || *ne < *idim * *ie) {
	goto L90;
    }
    if (*np < (k1 << 1) * *idim) {
	goto L90;
    }
    mxx = *m * *idim;
    ncc = *nest * *idim;
    if (*mx < mxx || *nc < ncc) {
	goto L90;
    }
    lwest = *m * k1 + *nest * (*idim + 6 + *k * 3);
    if (*lwrk < lwest) {
	goto L90;
    }
    if (w[1] <= (float)0.) {
	goto L90;
    }
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (u[i__ - 1] >= u[i__] || w[i__] <= (float)0.) {
	    goto L90;
	}
/* L10: */
    }
    if (*iopt >= 0) {
	goto L30;
    }
    if (*n < nmin || *n > *nest) {
	goto L90;
    }
    j = *n;
    i__1 = k1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t[i__] = u[1];
	t[j] = u[*m];
	--j;
/* L20: */
    }
    fpched_(&u[1], m, &t[1], n, k, ib, ie, ier);
    if (*ier != 0) {
	goto L90;
    } else {
	goto L40;
    }
L30:
    if (*s < (float)0.) {
	goto L90;
    }
    nmax = *m + k1 + ib1 + ie1;
    if (*s == (float)0. && *nest < nmax) {
	goto L90;
    }
    *ier = 0;
    if (*iopt > 0) {
	goto L70;
    }
/*  we determine a polynomial curve satisfying the boundary constraints. */
L40:
    fppocu_(idim, k, &u[1], &u[*m], ib, &db[1], nb, ie, &de[1], ne, &cp[1], 
	    np);
/*  we generate new data points which will be approximated by a spline */
/*  with zero derivative constraints. */
    j = nmin;
    i__1 = k1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wrk[i__] = u[1];
	wrk[j] = u[*m];
	--j;
/* L50: */
    }
/*  evaluate the polynomial curve */
    curev_(idim, &wrk[1], &nmin, &cp[1], np, k, &u[1], m, &xx[1], &mxx, ier);
/*  substract from the old data, the values of the polynomial curve */
    i__1 = mxx;
    for (i__ = 1; i__ <= i__1; ++i__) {
	xx[i__] = x[i__] - xx[i__];
/* L60: */
    }
/* we partition the working space and determine the spline curve. */
L70:
    jfp = 1;
    jz = jfp + *nest;
    ja = jz + ncc;
    jb = ja + *nest * k1;
    jg = jb + *nest * k2;
    jq = jg + *nest * k2;
    fpcons_(iopt, idim, m, &u[1], &mxx, &xx[1], &w[1], ib, ie, k, s, nest, &
	    tol, &maxit, &k1, &k2, n, &t[1], &ncc, &c__[1], fp, &wrk[jfp], &
	    wrk[jz], &wrk[ja], &wrk[jb], &wrk[jg], &wrk[jq], &iwrk[1], ier);
/*  add the polynomial curve to the calculated spline. */
    fpadpo_(idim, &t[1], n, &c__[1], &ncc, k, &cp[1], np, &wrk[jz], &wrk[ja], 
	    &wrk[jb]);
L90:
    return 0;
} /* concur_ */

/* Subroutine */ int cualde_(idim, t, n, c__, nc, k1, u, d__, nd, ier)
integer *idim;
real *t;
integer *n;
real *c__;
integer *nc, *k1;
real *u, *d__;
integer *nd, *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static real h__[6];
    static integer i__, j, l, m, kk;
    extern /* Subroutine */ int fpader_();
    static integer nk1;

/*  subroutine cualde evaluates at the point u all the derivatives */
/*                     (l) */
/*     d(idim*l+j) = sj   (u) ,l=0,1,...,k, j=1,2,...,idim */
/*  of a spline curve s(u) of order k1 (degree k=k1-1) and dimension idim */
/*  given in its b-spline representation. */

/*  calling sequence: */
/*     call cualde(idim,t,n,c,nc,k1,u,d,nd,ier) */

/*  input parameters: */
/*    idim : integer, giving the dimension of the spline curve. */
/*    t    : array,length n, which contains the position of the knots. */
/*    n    : integer, giving the total number of knots of s(u). */
/*    c    : array,length nc, which contains the b-spline coefficients. */
/*    nc   : integer, giving the total number of coefficients of s(u). */
/*    k1   : integer, giving the order of s(u) (order=degree+1). */
/*    u    : real, which contains the point where the derivatives must */
/*           be evaluated. */
/*    nd   : integer, giving the dimension of the array d. nd >= k1*idim */

/*  output parameters: */
/*    d    : array,length nd,giving the different curve derivatives. */
/*           d(idim*l+j) will contain the j-th coordinate of the l-th */
/*           derivative of the curve at the point u. */
/*    ier  : error flag */
/*      ier = 0 : normal return */
/*      ier =10 : invalid input data (see restrictions) */

/*  restrictions: */
/*    nd >= k1*idim */
/*    t(k1) <= u <= t(n-k1+1) */

/*  further comments: */
/*    if u coincides with a knot, right derivatives are computed */
/*    ( left derivatives if u = t(n-k1+1) ). */

/*  other subroutines required: fpader. */

/*  references : */
/*    de boor c : on calculating with b-splines, j. approximation theory */
/*                6 (1972) 50-62. */
/*    cox m.g.  : the numerical evaluation of b-splines, j. inst. maths */
/*                applics 10 (1972) 134-149. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --t;
    --c__;
    --d__;

    /* Function Body */
    *ier = 10;
    if (*nd < *k1 * *idim) {
	goto L500;
    }
    nk1 = *n - *k1;
    if (*u < t[*k1] || *u > t[nk1 + 1]) {
	goto L500;
    }
/*  search for knot interval t(l) <= u < t(l+1) */
    l = *k1;
L100:
    if (*u < t[l + 1] || l == nk1) {
	goto L200;
    }
    ++l;
    goto L100;
L200:
    if (t[l] >= t[l + 1]) {
	goto L500;
    }
    *ier = 0;
/*  calculate the derivatives. */
    j = 1;
    i__1 = *idim;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpader_(&t[1], n, &c__[j], k1, u, &l, h__);
	m = i__;
	i__2 = *k1;
	for (kk = 1; kk <= i__2; ++kk) {
	    d__[m] = h__[kk - 1];
	    m += *idim;
/* L300: */
	}
	j += *n;
/* L400: */
    }
L500:
    return 0;
} /* cualde_ */

/* Subroutine */ int curev_(idim, t, n, c__, nc, k, u, m, x, mx, ier)
integer *idim;
real *t;
integer *n;
real *c__;
integer *nc, *k;
real *u;
integer *m;
real *x;
integer *mx, *ier;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static real h__[6];
    static integer i__, j, l, j1, k1, l1, jj;
    static real tb;
    static integer ll;
    static real te;
    static integer mm;
    static real sp;
    extern /* Subroutine */ int fpbspl_();
    static integer nk1;
    static real arg;

/*  subroutine curev evaluates in a number of points u(i),i=1,2,...,m */
/*  a spline curve s(u) of degree k and dimension idim, given in its */
/*  b-spline representation. */

/*  calling sequence: */
/*     call curev(idim,t,n,c,nc,k,u,m,x,mx,ier) */

/*  input parameters: */
/*    idim : integer, giving the dimension of the spline curve. */
/*    t    : array,length n, which contains the position of the knots. */
/*    n    : integer, giving the total number of knots of s(u). */
/*    c    : array,length nc, which contains the b-spline coefficients. */
/*    nc   : integer, giving the total number of coefficients of s(u). */
/*    k    : integer, giving the degree of s(u). */
/*    u    : array,length m, which contains the points where s(u) must */
/*           be evaluated. */
/*    m    : integer, giving the number of points where s(u) must be */
/*           evaluated. */
/*    mx   : integer, giving the dimension of the array x. mx >= m*idim */

/*  output parameters: */
/*    x    : array,length mx,giving the value of s(u) at the different */
/*           points. x(idim*(i-1)+j) will contain the j-th coordinate */
/*           of the i-th point on the curve. */
/*    ier  : error flag */
/*      ier = 0 : normal return */
/*      ier =10 : invalid input data (see restrictions) */

/*  restrictions: */
/*    m >= 1 */
/*    mx >= m*idim */
/*    t(k+1) <= u(i) <= u(i+1) <= t(n-k) , i=1,2,...,m-1. */

/*  other subroutines required: fpbspl. */

/*  references : */
/*    de boor c : on calculating with b-splines, j. approximation theory */
/*                6 (1972) 50-62. */
/*    cox m.g.  : the numerical evaluation of b-splines, j. inst. maths */
/*                applics 10 (1972) 134-149. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --t;
    --c__;
    --u;
    --x;

    /* Function Body */
    *ier = 10;
    if ((i__1 = *m - 1) < 0) {
	goto L100;
    } else if (i__1 == 0) {
	goto L30;
    } else {
	goto L10;
    }
L10:
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (u[i__] < u[i__ - 1]) {
	    goto L100;
	}
/* L20: */
    }
L30:
    if (*mx < *m * *idim) {
	goto L100;
    }
    *ier = 0;
/*  fetch tb and te, the boundaries of the approximation interval. */
    k1 = *k + 1;
    nk1 = *n - k1;
    tb = t[k1];
    te = t[nk1 + 1];
    l = k1;
    l1 = l + 1;
/*  main loop for the different points. */
    mm = 0;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*  fetch a new u-value arg. */
	arg = u[i__];
	if (arg < tb) {
	    arg = tb;
	}
	if (arg > te) {
	    arg = te;
	}
/*  search for knot interval t(l) <= arg < t(l+1) */
L40:
	if (arg < t[l1] || l == nk1) {
	    goto L50;
	}
	l = l1;
	l1 = l + 1;
	goto L40;
/*  evaluate the non-zero b-splines at arg. */
L50:
	fpbspl_(&t[1], n, k, &arg, &l, h__);
/*  find the value of s(u) at u=arg. */
	ll = l - k1;
	i__2 = *idim;
	for (j1 = 1; j1 <= i__2; ++j1) {
	    jj = ll;
	    sp = (float)0.;
	    i__3 = k1;
	    for (j = 1; j <= i__3; ++j) {
		++jj;
		sp += c__[jj] * h__[j - 1];
/* L60: */
	    }
	    ++mm;
	    x[mm] = sp;
	    ll += *n;
/* L70: */
	}
/* L80: */
    }
L100:
    return 0;
} /* curev_ */

/* Subroutine */ int curfit_(iopt, m, x, y, w, xb, xe, k, s, nest, n, t, c__, 
	fp, wrk, lwrk, iwrk, ier)
integer *iopt, *m;
real *x, *y, *w, *xb, *xe;
integer *k;
real *s;
integer *nest, *n;
real *t, *c__, *fp, *wrk;
integer *lwrk, *iwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer nmin, i__, j, maxit, k1, k2, lwest, ia, ib, ig;
    extern /* Subroutine */ int fpchec_();
    static integer iq, iz;
    extern /* Subroutine */ int fpcurf_();
    static integer ifp;
    static real tol;

/*  given the set of data points (x(i),y(i)) and the set of positive */
/*  numbers w(i),i=1,2,...,m,subroutine curfit determines a smooth spline */
/*  approximation of degree k on the interval xb <= x <= xe. */
/*  if iopt=-1 curfit calculates the weighted least-squares spline */
/*  according to a given set of knots. */
/*  if iopt>=0 the number of knots of the spline s(x) and the position */
/*  t(j),j=1,2,...,n is chosen automatically by the routine. the smooth- */
/*  ness of s(x) is then achieved by minimalizing the discontinuity */
/*  jumps of the k-th derivative of s(x) at the knots t(j),j=k+2,k+3,..., */
/*  n-k-1. the amount of smoothness is determined by the condition that */
/*  f(p)=sum((w(i)*(y(i)-s(x(i))))**2) be <= s, with s a given non- */
/*  negative constant, called the smoothing factor. */
/*  the fit s(x) is given in the b-spline representation (b-spline coef- */
/*  ficients c(j),j=1,2,...,n-k-1) and can be evaluated by means of */
/*  subroutine splev. */

/*  calling sequence: */
/*     call curfit(iopt,m,x,y,w,xb,xe,k,s,nest,n,t,c,fp,wrk, */
/*    * lwrk,iwrk,ier) */

/*  parameters: */
/*   iopt  : integer flag. on entry iopt must specify whether a weighted */
/*           least-squares spline (iopt=-1) or a smoothing spline (iopt= */
/*           0 or 1) must be determined. if iopt=0 the routine will start */
/*           with an initial set of knots t(i)=xb, t(i+k+1)=xe, i=1,2,... */
/*           k+1. if iopt=1 the routine will continue with the knots */
/*           found at the last call of the routine. */
/*           attention: a call with iopt=1 must always be immediately */
/*           preceded by another call with iopt=1 or iopt=0. */
/*           unchanged on exit. */
/*   m     : integer. on entry m must specify the number of data points. */
/*           m > k. unchanged on exit. */
/*   x     : real array of dimension at least (m). before entry, x(i) */
/*           must be set to the i-th value of the independent variable x, */
/*           for i=1,2,...,m. these values must be supplied in strictly */
/*           ascending order. unchanged on exit. */
/*   y     : real array of dimension at least (m). before entry, y(i) */
/*           must be set to the i-th value of the dependent variable y, */
/*           for i=1,2,...,m. unchanged on exit. */
/*   w     : real array of dimension at least (m). before entry, w(i) */
/*           must be set to the i-th value in the set of weights. the */
/*           w(i) must be strictly positive. unchanged on exit. */
/*           see also further comments. */
/*   xb,xe : real values. on entry xb and xe must specify the boundaries */
/*           of the approximation interval. xb<=x(1), xe>=x(m). */
/*           unchanged on exit. */
/*   k     : integer. on entry k must specify the degree of the spline. */
/*           1<=k<=5. it is recommended to use cubic splines (k=3). */
/*           the user is strongly dissuaded from choosing k even,together */
/*           with a small s-value. unchanged on exit. */
/*   s     : real.on entry (in case iopt>=0) s must specify the smoothing */
/*           factor. s >=0. unchanged on exit. */
/*           for advice on the choice of s see further comments. */
/*   nest  : integer. on entry nest must contain an over-estimate of the */
/*           total number of knots of the spline returned, to indicate */
/*           the storage space available to the routine. nest >=2*k+2. */
/*           in most practical situation nest=m/2 will be sufficient. */
/*           always large enough is  nest=m+k+1, the number of knots */
/*           needed for interpolation (s=0). unchanged on exit. */
/*   n     : integer. */
/*           unless ier =10 (in case iopt >=0), n will contain the */
/*           total number of knots of the spline approximation returned. */
/*           if the computation mode iopt=1 is used this value of n */
/*           should be left unchanged between subsequent calls. */
/*           in case iopt=-1, the value of n must be specified on entry. */
/*   t     : real array of dimension at least (nest). */
/*           on succesful exit, this array will contain the knots of the */
/*           spline,i.e. the position of the interior knots t(k+2),t(k+3) */
/*           ...,t(n-k-1) as well as the position of the additional knots */
/*           t(1)=t(2)=...=t(k+1)=xb and t(n-k)=...=t(n)=xe needed for */
/*           the b-spline representation. */
/*           if the computation mode iopt=1 is used, the values of t(1), */
/*           t(2),...,t(n) should be left unchanged between subsequent */
/*           calls. if the computation mode iopt=-1 is used, the values */
/*           t(k+2),...,t(n-k-1) must be supplied by the user, before */
/*           entry. see also the restrictions (ier=10). */
/*   c     : real array of dimension at least (nest). */
/*           on succesful exit, this array will contain the coefficients */
/*           c(1),c(2),..,c(n-k-1) in the b-spline representation of s(x) */
/*   fp    : real. unless ier=10, fp contains the weighted sum of */
/*           squared residuals of the spline approximation returned. */
/*   wrk   : real array of dimension at least (m*(k+1)+nest*(7+3*k)). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used, the values wrk(1),...,wrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   lwrk  : integer. on entry,lwrk must specify the actual dimension of */
/*           the array wrk as declared in the calling (sub)program.lwrk */
/*           must not be too small (see wrk). unchanged on exit. */
/*   iwrk  : integer array of dimension at least (nest). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used,the values iwrk(1),...,iwrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   ier   : integer. unless the routine detects an error, ier contains a */
/*           non-positive value on exit, i.e. */
/*    ier=0  : normal return. the spline returned has a residual sum of */
/*             squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*             ive tolerance set to 0.001 by the program. */
/*    ier=-1 : normal return. the spline returned is an interpolating */
/*             spline (fp=0). */
/*    ier=-2 : normal return. the spline returned is the weighted least- */
/*             squares polynomial of degree k. in this extreme case fp */
/*             gives the upper bound fp0 for the smoothing factor s. */
/*    ier=1  : error. the required storage space exceeds the available */
/*             storage space, as specified by the parameter nest. */
/*             probably causes : nest too small. if nest is already */
/*             large (say nest > m/2), it may also indicate that s is */
/*             too small */
/*             the approximation returned is the weighted least-squares */
/*             spline according to the knots t(1),t(2),...,t(n). (n=nest) */
/*             the parameter fp gives the corresponding weighted sum of */
/*             squared residuals (fp>s). */
/*    ier=2  : error. a theoretically impossible result was found during */
/*             the iteration proces for finding a smoothing spline with */
/*             fp = s. probably causes : s too small. */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*             by the program) allowed for finding a smoothing spline */
/*             with fp=s has been reached. probably causes : s too small */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=10 : error. on entry, the input data are controlled on validity */
/*             the following restrictions must be satisfied. */
/*             -1<=iopt<=1, 1<=k<=5, m>k, nest>2*k+2, w(i)>0,i=1,2,...,m */
/*             xb<=x(1)<x(2)<...<x(m)<=xe, lwrk>=(k+1)*m+nest*(7+3*k) */
/*             if iopt=-1: 2*k+2<=n<=min(nest,m+k+1) */
/*                         xb<t(k+2)<t(k+3)<...<t(n-k-1)<xe */
/*                       the schoenberg-whitney conditions, i.e. there */
/*                       must be a subset of data points xx(j) such that */
/*                         t(j) < xx(j) < t(j+k+1), j=1,2,...,n-k-1 */
/*             if iopt>=0: s>=0 */
/*                         if s=0 : nest >= m+k+1 */
/*             if one of these conditions is found to be violated,control */
/*             is immediately repassed to the calling program. in that */
/*             case there is no approximation returned. */

/*  further comments: */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the spline will be too smooth and signal will be */
/*   lost ; if s is too small the spline will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating spline if */
/*   s=0 and the weighted least-squares polynomial of degree k if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the weights w(i). if these are */
/*   taken as 1/d(i) with d(i) an estimate of the standard deviation of */
/*   y(i), a good s-value should be found in the range (m-sqrt(2*m),m+ */
/*   sqrt(2*m)). if nothing is known about the statistical error in y(i) */
/*   each w(i) can be set equal to one and s determined by trial and */
/*   error, taking account of the comments above. the best is then to */
/*   start with a very large value of s ( to determine the least-squares */
/*   polynomial and the corresponding upper bound fp0 for s) and then to */
/*   progressively decrease the value of s ( say by a factor 10 in the */
/*   beginning, i.e. s=fp0/10, fp0/100,...and more carefully as the */
/*   approximation shows more detail) to obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt=1 the program will continue with the set of knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if curfit is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   function underlying the data. but, if the computation mode iopt=1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt=1, the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   curfit once more with the selected value for s but now with iopt=0. */
/*   indeed, curfit may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */

/*  other subroutines required: */
/*    fpback,fpbspl,fpchec,fpcurf,fpdisc,fpgivs,fpknot,fprati,fprota */

/*  references: */
/*   dierckx p. : an algorithm for smoothing, differentiation and integ- */
/*                ration of experimental data using spline functions, */
/*                j.comp.appl.maths 1 (1975) 165-184. */
/*   dierckx p. : a fast algorithm for smoothing data on a rectangular */
/*                grid while using spline functions, siam j.numer.anal. */
/*                19 (1982) 1286-1304. */
/*   dierckx p. : an improved algorithm for curve fitting with spline */
/*                functions, report tw54, dept. computer science,k.u. */
/*                leuven, 1981. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : may 1979 */
/*  latest update : march 1987 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  we set up the parameters tol and maxit */
    /* Parameter adjustments */
    --w;
    --y;
    --x;
    --iwrk;
    --c__;
    --t;
    --wrk;

    /* Function Body */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*k <= 0 || *k > 5) {
	goto L50;
    }
    k1 = *k + 1;
    k2 = k1 + 1;
    if (*iopt < -1 || *iopt > 1) {
	goto L50;
    }
    nmin = k1 << 1;
    if (*m < k1 || *nest < nmin) {
	goto L50;
    }
    lwest = *m * k1 + *nest * (*k * 3 + 7);
    if (*lwrk < lwest) {
	goto L50;
    }
    if (*xb > x[1] || *xe < x[*m] || w[1] <= (float)0.) {
	goto L50;
    }
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (x[i__ - 1] >= x[i__] || w[i__] <= (float)0.) {
	    goto L50;
	}
/* L10: */
    }
    if (*iopt >= 0) {
	goto L30;
    }
    if (*n < nmin || *n > *nest) {
	goto L50;
    }
    j = *n;
    i__1 = k1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t[i__] = *xb;
	t[j] = *xe;
	--j;
/* L20: */
    }
    fpchec_(&x[1], m, &t[1], n, k, ier);
    if (*ier != 0) {
	goto L50;
    } else {
	goto L40;
    }
L30:
    if (*s < (float)0.) {
	goto L50;
    }
    if (*s == (float)0. && *nest < *m + k1) {
	goto L50;
    }
    *ier = 0;
/* we partition the working space and determine the spline approximation. */
L40:
    ifp = 1;
    iz = ifp + *nest;
    ia = iz + *nest;
    ib = ia + *nest * k1;
    ig = ib + *nest * k2;
    iq = ig + *nest * k2;
    fpcurf_(iopt, &x[1], &y[1], &w[1], m, xb, xe, k, s, nest, &tol, &maxit, &
	    k1, &k2, n, &t[1], &c__[1], fp, &wrk[ifp], &wrk[iz], &wrk[ia], &
	    wrk[ib], &wrk[ig], &wrk[iq], &iwrk[1], ier);
L50:
    return 0;
} /* curfit_ */

doublereal dblint_(tx, nx, ty, ny, c__, kx, ky, xb, xe, yb, ye, wrk)
real *tx;
integer *nx;
real *ty;
integer *ny;
real *c__;
integer *kx, *ky;
real *xb, *xe, *yb, *ye, *wrk;
{
    /* System generated locals */
    integer i__1, i__2;
    real ret_val;

    /* Local variables */
    static integer i__, j, l, m;
    extern /* Subroutine */ int fpintb_();
    static real res;
    static integer nkx1, nky1;

/*  function dblint calculates the double integral */
/*         / xe  / ye */
/*        |     |      s(x,y) dx dy */
/*    xb /  yb / */
/*  with s(x,y) a bivariate spline of degrees kx and ky, given in the */
/*  b-spline representation. */

/*  calling sequence: */
/*     aint = dblint(tx,nx,ty,ny,c,kx,ky,xb,xe,yb,ye,wrk) */

/*  input parameters: */
/*   tx    : real array, length nx, which contains the position of the */
/*           knots in the x-direction. */
/*   nx    : integer, giving the total number of knots in the x-direction */
/*   ty    : real array, length ny, which contains the position of the */
/*           knots in the y-direction. */
/*   ny    : integer, giving the total number of knots in the y-direction */
/*   c     : real array, length (nx-kx-1)*(ny-ky-1), which contains the */
/*           b-spline coefficients. */
/*   kx,ky : integer values, giving the degrees of the spline. */
/*   xb,xe : real values, containing the boundaries of the integration */
/*   yb,ye   domain. s(x,y) is considered to be identically zero out- */
/*           side the rectangle (tx(kx+1),tx(nx-kx))*(ty(ky+1),ty(ny-ky)) */

/*  output parameters: */
/*   aint  : real , containing the double integral of s(x,y). */
/*   wrk   : real array of dimension at least (nx+ny-kx-ky-2). */
/*           used as working space. */
/*           on exit, wrk(i) will contain the integral */
/*                / xe */
/*               | ni,kx+1(x) dx , i=1,2,...,nx-kx-1 */
/*           xb / */
/*           with ni,kx+1(x) the normalized b-spline defined on */
/*           the knots tx(i),...,tx(i+kx+1) */
/*           wrk(j+nx-kx-1) will contain the integral */
/*                / ye */
/*               | nj,ky+1(y) dy , j=1,2,...,ny-ky-1 */
/*           yb / */
/*           with nj,ky+1(y) the normalized b-spline defined on */
/*           the knots ty(j),...,ty(j+ky+1) */

/*  other subroutines required: fpintb */

/*  references : */
/*    gaffney p.w. : the calculation of indefinite integrals of b-splines */
/*                   j. inst. maths applics 17 (1976) 37-41. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1989 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --tx;
    --ty;
    --wrk;
    --c__;

    /* Function Body */
    nkx1 = *nx - *kx - 1;
    nky1 = *ny - *ky - 1;
/*  we calculate the integrals of the normalized b-splines ni,kx+1(x) */
    fpintb_(&tx[1], nx, &wrk[1], &nkx1, xb, xe);
/*  we calculate the integrals of the normalized b-splines nj,ky+1(y) */
    fpintb_(&ty[1], ny, &wrk[nkx1 + 1], &nky1, yb, ye);
/*  calculate the integral of s(x,y) */
    ret_val = (float)0.;
    i__1 = nkx1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	res = wrk[i__];
	if (res == (float)0.) {
	    goto L200;
	}
	m = (i__ - 1) * nky1;
	l = nkx1;
	i__2 = nky1;
	for (j = 1; j <= i__2; ++j) {
	    ++m;
	    ++l;
	    ret_val += res * wrk[l] * c__[m];
/* L100: */
	}
L200:
	;
    }
    return ret_val;
} /* dblint_ */

doublereal evapol_(tu, nu, tv, nv, c__, rad, x, y)
real *tu;
integer *nu;
real *tv;
integer *nv;
real *c__;
doublereal (*rad) ();
real *x, *y;
{
    /* System generated locals */
    real ret_val, r__1, r__2;

    /* Builtin functions */
    double atan2(), sqrt();

    /* Local variables */
    static real dist;
    static integer iwrk[2];
    static real f, r__, u, v;
    extern /* Subroutine */ int bispev_();
    static integer ier;
    static real one, wrk[8];

/*  function program evacir evaluates the function f(x,y) = s(u,v), */
/*  defined through the transformation */
/*      x = u*rad(v)*cos(v)    y = u*rad(v)*sin(v) */
/*  and where s(u,v) is a bicubic spline ( 0<=u<=1 , -pi<=v<=pi ), given */
/*  in its standard b-spline representation. */

/*  calling sequence: */
/*     f = evapol(tu,nu,tv,nv,c,rad,x,y) */

/*  input parameters: */
/*   tu    : real array, length nu, which contains the position of the */
/*           knots in the u-direction. */
/*   nu    : integer, giving the total number of knots in the u-direction */
/*   tv    : real array, length nv, which contains the position of the */
/*           knots in the v-direction. */
/*   nv    : integer, giving the total number of knots in the v-direction */
/*   c     : real array, length (nu-4)*(nv-4), which contains the */
/*           b-spline coefficients. */
/*   rad   : real function subprogram, defining the boundary of the */
/*           approximation domain. must be declared external in the */
/*           calling (sub)-program */
/*   x,y   : real values. */
/*           before entry x and y must be set to the co-ordinates of */
/*           the point where f(x,y) must be evaluated. */

/*  output parameter: */
/*   f     : real */
/*           on exit f contains the value of f(x,y) */

/*  other subroutines required: */
/*    bispev,fpbisp,fpbspl */

/*  references : */
/*    de boor c : on calculating with b-splines, j. approximation theory */
/*                6 (1972) 50-62. */
/*    cox m.g.  : the numerical evaluation of b-splines, j. inst. maths */
/*                applics 10 (1972) 134-149. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1989 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..user specified function */
/*  ..local scalars.. */
/*  ..local arrays */
/*  ..function references */
/*  .. */
/*  calculate the (u,v)-coordinates of the given point. */
    /* Parameter adjustments */
    --tu;
    --c__;
    --tv;

    /* Function Body */
    one = (float)1.;
    u = (float)0.;
    v = (float)0.;
/* Computing 2nd power */
    r__1 = *x;
/* Computing 2nd power */
    r__2 = *y;
    dist = r__1 * r__1 + r__2 * r__2;
    if (dist <= (float)0.) {
	goto L10;
    }
    v = atan2(*y, *x);
    r__ = (*rad)(&v);
    if (r__ <= (float)0.) {
	goto L10;
    }
    u = sqrt(dist) / r__;
    if (u > one) {
	u = one;
    }
/*  evaluate s(u,v) */
L10:
    bispev_(&tu[1], nu, &tv[1], nv, &c__[1], &c__3, &c__3, &u, &c__1, &v, &
	    c__1, &f, wrk, &c__8, iwrk, &c__2, &ier);
    ret_val = f;
    return ret_val;
} /* evapol_ */

/* Subroutine */ int fourco_(t, n, c__, alfa, m, ress, resc, wrk1, wrk2, ier)
real *t;
integer *n;
real *c__, *alfa;
integer *m;
real *ress, *resc, *wrk1, *wrk2;
integer *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, n4;
    static real rc, rs;
    extern /* Subroutine */ int fpbfou_();

/*  subroutine fourco calculates the integrals */
/*                    /t(n-3) */
/*    ress(i) =      !        s(x)*sin(alfa(i)*x) dx    and */
/*              t(4)/ */
/*                    /t(n-3) */
/*    resc(i) =      !        s(x)*cos(alfa(i)*x) dx, i=1,...,m, */
/*              t(4)/ */
/*  where s(x) denotes a cubic spline which is given in its */
/*  b-spline representation. */

/*  calling sequence: */
/*     call fourco(t,n,c,alfa,m,ress,resc,wrk1,wrk2,ier) */

/*  input parameters: */
/*    t    : real array,length n, containing the knots of s(x). */
/*    n    : integer, containing the total number of knots. n>=10. */
/*    c    : real array,length n, containing the b-spline coefficients. */
/*    alfa : real array,length m, containing the parameters alfa(i). */
/*    m    : integer, specifying the number of integrals to be computed. */
/*    wrk1 : real array,length n. used as working space */
/*    wrk2 : real array,length n. used as working space */

/*  output parameters: */
/*    ress : real array,length m, containing the integrals ress(i). */
/*    resc : real array,length m, containing the integrals resc(i). */
/*    ier  : error flag: */
/*      ier=0 : normal return. */
/*      ier=10: invalid input data (see restrictions). */

/*  restrictions: */
/*    n >= 10 */
/*    t(4) < t(5) < ... < t(n-4) < t(n-3). */
/*    t(1) <= t(2) <= t(3) <= t(4). */
/*    t(n-3) <= t(n-2) <= t(n-1) <= t(n). */

/*  other subroutines required: fpbfou,fpcsin */

/*  references : */
/*    dierckx p. : calculation of fouriercoefficients of discrete */
/*                 functions using cubic splines. j. computational */
/*                 and applied mathematics 3 (1977) 207-209. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --wrk2;
    --wrk1;
    --c__;
    --t;
    --resc;
    --ress;
    --alfa;

    /* Function Body */
    n4 = *n - 4;
/*  before starting computations a data check is made. in the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*n < 10) {
	goto L50;
    }
    j = *n;
    for (i__ = 1; i__ <= 3; ++i__) {
	if (t[i__] > t[i__ + 1]) {
	    goto L50;
	}
	if (t[j] < t[j - 1]) {
	    goto L50;
	}
	--j;
/* L10: */
    }
    i__1 = n4;
    for (i__ = 4; i__ <= i__1; ++i__) {
	if (t[i__] >= t[i__ + 1]) {
	    goto L50;
	}
/* L20: */
    }
    *ier = 0;
/*  main loop for the different alfa(i). */
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*  calculate the integrals */
/*    wrk1(j) = integral(nj,4(x)*sin(alfa*x))    and */
/*    wrk2(j) = integral(nj,4(x)*cos(alfa*x)),  j=1,2,...,n-4, */
/*  where nj,4(x) denotes the normalised cubic b-spline defined on the */
/*  knots t(j),t(j+1),...,t(j+4). */
	fpbfou_(&t[1], n, &alfa[i__], &wrk1[1], &wrk2[1]);
/*  calculate the integrals ress(i) and resc(i). */
	rs = (float)0.;
	rc = (float)0.;
	i__2 = n4;
	for (j = 1; j <= i__2; ++j) {
	    rs += c__[j] * wrk1[j];
	    rc += c__[j] * wrk2[j];
/* L30: */
	}
	ress[i__] = rs;
	resc[i__] = rc;
/* L40: */
    }
L50:
    return 0;
} /* fourco_ */

/* Subroutine */ int fpader_(t, n, c__, k1, x, l, d__)
real *t;
integer *n;
real *c__;
integer *k1;
real *x;
integer *l;
real *d__;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static real h__[6];
    static integer i__, j, j1, j2;
    static real ak;
    static integer ik, jj, ki, kj, li, lj, lk;
    static real fac, one;

/*  subroutine fpader calculates the derivatives */
/*             (j-1) */
/*     d(j) = s     (x) , j=1,2,...,k1 */
/*  of a spline of order k1 at the point t(l)<=x<t(l+1), using the */
/*  stable recurrence scheme of de boor */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  .. */
    /* Parameter adjustments */
    --c__;
    --t;
    --d__;

    /* Function Body */
    one = (float)1.;
    lk = *l - *k1;
    i__1 = *k1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ik = i__ + lk;
	h__[i__ - 1] = c__[ik];
/* L100: */
    }
    kj = *k1;
    fac = one;
    i__1 = *k1;
    for (j = 1; j <= i__1; ++j) {
	ki = kj;
	j1 = j + 1;
	if (j == 1) {
	    goto L300;
	}
	i__ = *k1;
	i__2 = *k1;
	for (jj = j; jj <= i__2; ++jj) {
	    li = i__ + lk;
	    lj = li + kj;
	    h__[i__ - 1] = (h__[i__ - 1] - h__[i__ - 2]) / (t[lj] - t[li]);
	    --i__;
/* L200: */
	}
L300:
	i__2 = *k1;
	for (i__ = j; i__ <= i__2; ++i__) {
	    d__[i__] = h__[i__ - 1];
/* L400: */
	}
	if (j == *k1) {
	    goto L600;
	}
	i__2 = *k1;
	for (jj = j1; jj <= i__2; ++jj) {
	    --ki;
	    i__ = *k1;
	    i__3 = *k1;
	    for (j2 = jj; j2 <= i__3; ++j2) {
		li = i__ + lk;
		lj = li + ki;
		d__[i__] = ((*x - t[li]) * d__[i__] + (t[lj] - *x) * d__[i__ 
			- 1]) / (t[lj] - t[li]);
		--i__;
/* L500: */
	    }
	}
L600:
	d__[j] = d__[*k1] * fac;
	ak = (real) (*k1 - j);
	fac *= ak;
	--kj;
/* L700: */
    }
    return 0;
} /* fpader_ */

/* Subroutine */ int fpadno_(maxtr, up, left, right, info, count, merk, jbind,
	 n1, ier)
integer *maxtr, *up, *left, *right, *info, *count, *merk, *jbind, *n1, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static logical bool;
    static integer k, point, niveau;
    extern /* Subroutine */ int fpfrno_();

/*  subroutine fpadno adds a branch of length n1 to the triply linked */
/*  tree,the information of which is kept in the arrays up,left,right */
/*  and info. the information field of the nodes of this new branch is */
/*  given in the array jbind. in linking the new branch fpadno takes */
/*  account of the property of the tree that */
/*    info(k) < info(right(k)) ; info(k) < info(left(k)) */
/*  if necessary the subroutine calls subroutine fpfrno to collect the */
/*  free nodes of the tree. if no computer words are available at that */
/*  moment, the error parameter ier is set to 1. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..subroutine references.. */
/*    fpfrno */
/*  .. */
    /* Parameter adjustments */
    --info;
    --right;
    --left;
    --up;
    --jbind;

    /* Function Body */
    point = 1;
    niveau = 1;
L10:
    k = left[point];
    bool = TRUE_;
L20:
    if (k == 0) {
	goto L50;
    }
    if ((i__1 = info[k] - jbind[niveau]) < 0) {
	goto L30;
    } else if (i__1 == 0) {
	goto L40;
    } else {
	goto L50;
    }
L30:
    point = k;
    k = right[point];
    bool = FALSE_;
    goto L20;
L40:
    point = k;
    ++niveau;
    goto L10;
L50:
    if (niveau > *n1) {
	goto L90;
    }
    ++(*count);
    if (*count <= *maxtr) {
	goto L60;
    }
    fpfrno_(maxtr, &up[1], &left[1], &right[1], &info[1], &point, merk, n1, 
	    count, ier);
    if (*ier != 0) {
	goto L100;
    }
L60:
    info[*count] = jbind[niveau];
    left[*count] = 0;
    right[*count] = k;
    if (bool) {
	goto L70;
    }
    bool = TRUE_;
    right[point] = *count;
    up[*count] = up[point];
    goto L80;
L70:
    up[*count] = point;
    left[point] = *count;
L80:
    point = *count;
    ++niveau;
    k = 0;
    goto L50;
L90:
    *ier = 0;
L100:
    return 0;
} /* fpadno_ */

/* Subroutine */ int fpadpo_(idim, t, n, c__, nc, k, cp, np, cc, t1, t2)
integer *idim;
real *t;
integer *n;
real *c__;
integer *nc, *k;
real *cp;
integer *np;
real *cc, *t1, *t2;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, l, k1, l1, n1, n2, ii, jj;
    extern /* Subroutine */ int fpinst_();
    static integer nk1, nk2;

/*  given a idim-dimensional spline curve of degree k, in its b-spline */
/*  representation ( knots t(j),j=1,...,n , b-spline coefficients c(j), */
/*  j=1,...,nc) and given also a polynomial curve in its b-spline */
/*  representation ( coefficients cp(j), j=1,...,np), subroutine fpadpo */
/*  calculates the b-spline representation (coefficients c(j),j=1,...,nc) */
/*  of the sum of the two curves. */

/*  other subroutine required : fpinst */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --t2;
    --t1;
    --t;
    --cc;
    --c__;
    --cp;

    /* Function Body */
    k1 = *k + 1;
    nk1 = *n - k1;
/*  initialization */
    j = 1;
    l = 1;
    i__1 = *idim;
    for (jj = 1; jj <= i__1; ++jj) {
	l1 = j;
	i__2 = k1;
	for (ii = 1; ii <= i__2; ++ii) {
	    cc[l1] = cp[l];
	    ++l1;
	    ++l;
/* L10: */
	}
	j += *n;
	l += k1;
/* L20: */
    }
    if (nk1 == k1) {
	goto L70;
    }
    n1 = k1 << 1;
    j = *n;
    l = n1;
    i__1 = k1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t1[i__] = t[i__];
	t1[l] = t[j];
	--l;
	--j;
/* L30: */
    }
/*  find the b-spline representation of the given polynomial curve */
/*  according to the given set of knots. */
    nk2 = nk1 - 1;
    i__1 = nk2;
    for (l = k1; l <= i__1; ++l) {
	l1 = l + 1;
	j = 1;
	i__2 = *idim;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    fpinst_(&c__0, &t1[1], &n1, &cc[j], k, &t[l1], &l, &t2[1], &n2, &
		    cc[j], n);
	    j += *n;
/* L40: */
	}
	i__2 = n2;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    t1[i__] = t2[i__];
/* L50: */
	}
	n1 = n2;
/* L60: */
    }
/*  find the b-spline representation of the resulting curve. */
L70:
    j = 1;
    i__1 = *idim;
    for (jj = 1; jj <= i__1; ++jj) {
	l = j;
	i__2 = nk1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    c__[l] = cc[l] + c__[l];
	    ++l;
/* L80: */
	}
	j += *n;
/* L90: */
    }
    return 0;
} /* fpadpo_ */

/* Subroutine */ int fpback_(a, z__, n, k, c__, nest)
real *a, *z__;
integer *n, *k;
real *c__;
integer *nest;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;

    /* Local variables */
    static integer i__, j, l, m, i1;
    static real store;
    static integer k1;

/*  subroutine fpback calculates the solution of the system of */
/*  equations a*c = z with a a n x n upper triangular matrix */
/*  of bandwidth k. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --c__;
    --z__;
    a_dim1 = *nest;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;

    /* Function Body */
    k1 = *k - 1;
    c__[*n] = z__[*n] / a[*n + a_dim1];
    i__ = *n - 1;
    if (i__ == 0) {
	goto L30;
    }
    i__1 = *n;
    for (j = 2; j <= i__1; ++j) {
	store = z__[i__];
	i1 = k1;
	if (j <= k1) {
	    i1 = j - 1;
	}
	m = i__;
	i__2 = i1;
	for (l = 1; l <= i__2; ++l) {
	    ++m;
	    store -= c__[m] * a[i__ + (l + 1) * a_dim1];
/* L10: */
	}
	c__[i__] = store / a[i__ + a_dim1];
	--i__;
/* L20: */
    }
L30:
    return 0;
} /* fpback_ */

/* Subroutine */ int fpbacp_(a, b, z__, n, k, c__, k1, nest)
real *a, *b, *z__;
integer *n, *k;
real *c__;
integer *k1, *nest;
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1, i__2;

    /* Local variables */
    static integer i__, j, l, i1, l0, l1;
    static real store;
    static integer n2;

/*  subroutine fpbacp calculates the solution of the system of equations */
/*  g * c = z  with g  a n x n upper triangular matrix of the form */
/*            ! a '   ! */
/*        g = !   ' b ! */
/*            ! 0 '   ! */
/*  with b a n x k matrix and a a (n-k) x (n-k) upper triangular */
/*  matrix of bandwidth k1. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --c__;
    --z__;
    b_dim1 = *nest;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    a_dim1 = *nest;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;

    /* Function Body */
    n2 = *n - *k;
    l = *n;
    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	store = z__[l];
	j = *k + 2 - i__;
	if (i__ == 1) {
	    goto L20;
	}
	l0 = l;
	i__2 = *k;
	for (l1 = j; l1 <= i__2; ++l1) {
	    ++l0;
	    store -= c__[l0] * b[l + l1 * b_dim1];
/* L10: */
	}
L20:
	c__[l] = store / b[l + (j - 1) * b_dim1];
	--l;
	if (l == 0) {
	    goto L80;
	}
/* L30: */
    }
    i__1 = n2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	store = z__[i__];
	l = n2;
	i__2 = *k;
	for (j = 1; j <= i__2; ++j) {
	    ++l;
	    store -= c__[l] * b[i__ + j * b_dim1];
/* L40: */
	}
	c__[i__] = store;
/* L50: */
    }
    i__ = n2;
    c__[i__] /= a[i__ + a_dim1];
    if (i__ == 1) {
	goto L80;
    }
    i__1 = n2;
    for (j = 2; j <= i__1; ++j) {
	--i__;
	store = c__[i__];
	i1 = *k;
	if (j <= *k) {
	    i1 = j - 1;
	}
	l = i__;
	i__2 = i1;
	for (l0 = 1; l0 <= i__2; ++l0) {
	    ++l;
	    store -= c__[l] * a[i__ + (l0 + 1) * a_dim1];
/* L60: */
	}
	c__[i__] = store / a[i__ + a_dim1];
/* L70: */
    }
L80:
    return 0;
} /* fpbacp_ */

/* Subroutine */ int fpbfou_(t, n, par, ress, resc)
real *t;
integer *n;
real *par, *ress, *resc;
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double cos(), sin();

    /* Local variables */
    static real beta, sign, term;
    static integer i__, j, k;
    static real delta, c1, c2, f1, f2, f3, quart, s1, s2, hc[5];
    static integer ic;
    static real ak, co[5];
    static integer jj, li, lj;
    static real rc[3];
    static integer ll;
    static real hs[5];
    static integer is;
    static real si[5], rs[3];
    extern /* Subroutine */ int fpcsin_();
    static integer jp1, jp4, nm3, nm7;
    static real fac, one;
    static integer ipj, nmj;
    static real eps, six, con1, con2;

/*  subroutine fpbfou calculates the integrals */
/*                    /t(n-3) */
/*    ress(j) =      !        nj,4(x)*sin(par*x) dx    and */
/*              t(4)/ */
/*                    /t(n-3) */
/*    resc(j) =      !        nj,4(x)*cos(par*x) dx ,  j=1,2,...n-4 */
/*              t(4)/ */
/*  where nj,4(x) denotes the cubic b-spline defined on the knots */
/*  t(j),t(j+1),...,t(j+4). */

/*  calling sequence: */
/*     call fpbfou(t,n,par,ress,resc) */

/*  input parameters: */
/*    t    : real array,length n, containing the knots. */
/*    n    : integer, containing the number of knots. */
/*    par  : real, containing the value of the parameter par. */

/*  output parameters: */
/*    ress  : real array,length n, containing the integrals ress(j). */
/*    resc  : real array,length n, containing the integrals resc(j). */

/*  restrictions: */
/*    n >= 10, t(4) < t(5) < ... < t(n-4) < t(n-3). */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  .. */
/*  initialization. */
    /* Parameter adjustments */
    --resc;
    --ress;
    --t;

    /* Function Body */
    one = (float)1.;
    six = (float)6.;
    eps = (float)1e-8;
    quart = (float).25;
    con1 = (float).05;
    con2 = (float)120.;
    nm3 = *n - 3;
    nm7 = *n - 7;
    if (*par != (float)0.) {
	term = six / *par;
    }
    beta = *par * t[4];
    co[0] = cos(beta);
    si[0] = sin(beta);
/*  calculate the integrals ress(j) and resc(j), j=1,2,3 by setting up */
/*  a divided difference table. */
    for (j = 1; j <= 3; ++j) {
	jp1 = j + 1;
	jp4 = j + 4;
	beta = *par * t[jp4];
	co[jp1 - 1] = cos(beta);
	si[jp1 - 1] = sin(beta);
	fpcsin_(&t[4], &t[jp4], par, si, co, &si[jp1 - 1], &co[jp1 - 1], &rs[
		j - 1], &rc[j - 1]);
	i__ = 5 - j;
	hs[i__ - 1] = (float)0.;
	hc[i__ - 1] = (float)0.;
	i__1 = j;
	for (jj = 1; jj <= i__1; ++jj) {
	    ipj = i__ + jj;
	    hs[ipj - 1] = rs[jj - 1];
	    hc[ipj - 1] = rc[jj - 1];
/* L10: */
	}
	for (jj = 1; jj <= 3; ++jj) {
	    if (i__ < jj) {
		i__ = jj;
	    }
	    k = 5;
	    li = jp4;
	    for (ll = i__; ll <= 4; ++ll) {
		lj = li - jj;
		fac = t[li] - t[lj];
		hs[k - 1] = (hs[k - 1] - hs[k - 2]) / fac;
		hc[k - 1] = (hc[k - 1] - hc[k - 2]) / fac;
		--k;
		--li;
/* L20: */
	    }
	}
	ress[j] = hs[4] - hs[3];
	resc[j] = hc[4] - hc[3];
/* L30: */
    }
    if (nm7 < 4) {
	goto L160;
    }
/*  calculate the integrals ress(j) and resc(j),j=4,5,...,n-7. */
    i__1 = nm7;
    for (j = 4; j <= i__1; ++j) {
	jp4 = j + 4;
	beta = *par * t[jp4];
	co[4] = cos(beta);
	si[4] = sin(beta);
	delta = t[jp4] - t[j];
/*  the way of computing ress(j) and resc(j) depends on the value of */
/*  beta = par*(t(j+4)-t(j)). */
	beta = delta * *par;
	if (dabs(beta) <= one) {
	    goto L60;
	}
/*  if !beta! > 1 the integrals are calculated by setting up a divided */
/*  difference table. */
	for (k = 1; k <= 5; ++k) {
	    hs[k - 1] = si[k - 1];
	    hc[k - 1] = co[k - 1];
/* L40: */
	}
	for (jj = 1; jj <= 3; ++jj) {
	    k = 5;
	    li = jp4;
	    for (ll = jj; ll <= 4; ++ll) {
		lj = li - jj;
		fac = *par * (t[li] - t[lj]);
		hs[k - 1] = (hs[k - 1] - hs[k - 2]) / fac;
		hc[k - 1] = (hc[k - 1] - hc[k - 2]) / fac;
		--k;
		--li;
/* L50: */
	    }
	}
	s2 = (hs[4] - hs[3]) * term;
	c2 = (hc[4] - hc[3]) * term;
	goto L130;
/*  if !beta! <= 1 the integrals are calculated by evaluating a series */
/*  expansion. */
L60:
	f3 = (float)0.;
	for (i__ = 1; i__ <= 4; ++i__) {
	    ipj = i__ + j;
	    hs[i__ - 1] = *par * (t[ipj] - t[j]);
	    hc[i__ - 1] = hs[i__ - 1];
	    f3 += hs[i__ - 1];
/* L70: */
	}
	f3 *= con1;
	c1 = quart;
	s1 = f3;
	if (dabs(f3) <= eps) {
	    goto L120;
	}
	sign = one;
	fac = con2;
	k = 5;
	is = 0;
	for (ic = 1; ic <= 20; ++ic) {
	    ++k;
	    ak = (real) k;
	    fac *= ak;
	    f1 = (float)0.;
	    f3 = (float)0.;
	    for (i__ = 1; i__ <= 4; ++i__) {
		f1 += hc[i__ - 1];
		f2 = f1 * hs[i__ - 1];
		hc[i__ - 1] = f2;
		f3 += f2;
/* L80: */
	    }
	    f3 = f3 * six / fac;
	    if (is == 0) {
		goto L90;
	    }
	    is = 0;
	    s1 += f3 * sign;
	    goto L100;
L90:
	    sign = -sign;
	    is = 1;
	    c1 += f3 * sign;
L100:
	    if (dabs(f3) <= eps) {
		goto L120;
	    }
/* L110: */
	}
L120:
	s2 = delta * (co[0] * s1 + si[0] * c1);
	c2 = delta * (co[0] * c1 - si[0] * s1);
L130:
	ress[j] = s2;
	resc[j] = c2;
	for (i__ = 1; i__ <= 4; ++i__) {
	    co[i__ - 1] = co[i__];
	    si[i__ - 1] = si[i__];
/* L140: */
	}
/* L150: */
    }
/*  calculate the integrals ress(j) and resc(j),j=n-6,n-5,n-4 by setting */
/*  up a divided difference table. */
L160:
    for (j = 1; j <= 3; ++j) {
	nmj = nm3 - j;
	i__ = 5 - j;
	fpcsin_(&t[nm3], &t[nmj], par, &si[3], &co[3], &si[i__ - 2], &co[i__ 
		- 2], &rs[j - 1], &rc[j - 1]);
	hs[i__ - 1] = (float)0.;
	hc[i__ - 1] = (float)0.;
	i__1 = j;
	for (jj = 1; jj <= i__1; ++jj) {
	    ipj = i__ + jj;
	    hc[ipj - 1] = rc[jj - 1];
	    hs[ipj - 1] = rs[jj - 1];
/* L170: */
	}
	for (jj = 1; jj <= 3; ++jj) {
	    if (i__ < jj) {
		i__ = jj;
	    }
	    k = 5;
	    li = nmj;
	    for (ll = i__; ll <= 4; ++ll) {
		lj = li + jj;
		fac = t[lj] - t[li];
		hs[k - 1] = (hs[k - 2] - hs[k - 1]) / fac;
		hc[k - 1] = (hc[k - 2] - hc[k - 1]) / fac;
		--k;
		++li;
/* L180: */
	    }
	}
	ress[nmj] = hs[3] - hs[4];
	resc[nmj] = hc[3] - hc[4];
/* L190: */
    }
    return 0;
} /* fpbfou_ */

/* Subroutine */ int fpbisp_(tx, nx, ty, ny, c__, kx, ky, x, mx, y, my, z__, 
	wx, wy, lx, ly)
real *tx;
integer *nx;
real *ty;
integer *ny;
real *c__;
integer *kx, *ky;
real *x;
integer *mx;
real *y;
integer *my;
real *z__, *wx, *wy;
integer *lx, *ly;
{
    /* System generated locals */
    integer wx_dim1, wx_offset, wy_dim1, wy_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static real h__[6];
    static integer i__, j, l, m, i1, j1, l1, l2;
    static real tb, te, sp;
    extern /* Subroutine */ int fpbspl_();
    static integer kx1, ky1;
    static real arg;
    static integer nkx1, nky1;

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..subroutine references.. */
/*    fpbspl */
/*  .. */
    /* Parameter adjustments */
    --tx;
    --ty;
    --c__;
    --lx;
    wx_dim1 = *mx;
    wx_offset = 1 + wx_dim1 * 1;
    wx -= wx_offset;
    --x;
    --ly;
    wy_dim1 = *my;
    wy_offset = 1 + wy_dim1 * 1;
    wy -= wy_offset;
    --z__;
    --y;

    /* Function Body */
    kx1 = *kx + 1;
    nkx1 = *nx - kx1;
    tb = tx[kx1];
    te = tx[nkx1 + 1];
    l = kx1;
    l1 = l + 1;
    i__1 = *mx;
    for (i__ = 1; i__ <= i__1; ++i__) {
	arg = x[i__];
	if (arg < tb) {
	    arg = tb;
	}
	if (arg > te) {
	    arg = te;
	}
L10:
	if (arg < tx[l1] || l == nkx1) {
	    goto L20;
	}
	l = l1;
	l1 = l + 1;
	goto L10;
L20:
	fpbspl_(&tx[1], nx, kx, &arg, &l, h__);
	lx[i__] = l - kx1;
	i__2 = kx1;
	for (j = 1; j <= i__2; ++j) {
	    wx[i__ + j * wx_dim1] = h__[j - 1];
/* L30: */
	}
/* L40: */
    }
    ky1 = *ky + 1;
    nky1 = *ny - ky1;
    tb = ty[ky1];
    te = ty[nky1 + 1];
    l = ky1;
    l1 = l + 1;
    i__1 = *my;
    for (i__ = 1; i__ <= i__1; ++i__) {
	arg = y[i__];
	if (arg < tb) {
	    arg = tb;
	}
	if (arg > te) {
	    arg = te;
	}
L50:
	if (arg < ty[l1] || l == nky1) {
	    goto L60;
	}
	l = l1;
	l1 = l + 1;
	goto L50;
L60:
	fpbspl_(&ty[1], ny, ky, &arg, &l, h__);
	ly[i__] = l - ky1;
	i__2 = ky1;
	for (j = 1; j <= i__2; ++j) {
	    wy[i__ + j * wy_dim1] = h__[j - 1];
/* L70: */
	}
/* L80: */
    }
    m = 0;
    i__1 = *mx;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l = lx[i__] * nky1;
	i__2 = kx1;
	for (i1 = 1; i1 <= i__2; ++i1) {
	    h__[i1 - 1] = wx[i__ + i1 * wx_dim1];
/* L90: */
	}
	i__2 = *my;
	for (j = 1; j <= i__2; ++j) {
	    l1 = l + ly[j];
	    sp = (float)0.;
	    i__3 = kx1;
	    for (i1 = 1; i1 <= i__3; ++i1) {
		l2 = l1;
		i__4 = ky1;
		for (j1 = 1; j1 <= i__4; ++j1) {
		    ++l2;
		    sp += c__[l2] * h__[i1 - 1] * wy[j + j1 * wy_dim1];
/* L100: */
		}
		l1 += nky1;
/* L110: */
	    }
	    ++m;
	    z__[m] = sp;
/* L120: */
	}
/* L130: */
    }
    return 0;
} /* fpbisp_ */

/* Subroutine */ int fpbspl_(t, n, k, x, l, h__)
real *t;
integer *n, *k;
real *x;
integer *l;
real *h__;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static real f;
    static integer i__, j;
    static real hh[5];
    static integer li, lj;
    static real one;

/*  subroutine fpbspl evaluates the (k+1) non-zero b-splines of */
/*  degree k at t(l) <= x < t(l+1) using the stable recurrence */
/*  relation of de boor and cox. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  .. */
    /* Parameter adjustments */
    --t;
    --h__;

    /* Function Body */
    one = (float)1.;
    h__[1] = one;
    i__1 = *k;
    for (j = 1; j <= i__1; ++j) {
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    hh[i__ - 1] = h__[i__];
/* L10: */
	}
	h__[1] = (float)0.;
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    li = *l + i__;
	    lj = li - j;
	    f = hh[i__ - 1] / (t[li] - t[lj]);
	    h__[i__] += f * (t[li] - *x);
	    h__[i__ + 1] = f * (*x - t[lj]);
/* L20: */
	}
    }
    return 0;
} /* fpbspl_ */

/* Subroutine */ int fpchec_(x, m, t, n, k, ier)
real *x;
integer *m;
real *t;
integer *n, *k, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, l, k1, k2;
    static real tj, tl;
    static integer nk1, nk2, nk3;

/*  subroutine fpchec verifies the number and the position of the knots */
/*  t(j),j=1,2,...,n of a spline of degree k, in relation to the number */
/*  and the position of the data points x(i),i=1,2,...,m. if all of the */
/*  following conditions are fulfilled, the error parameter ier is set */
/*  to zero. if one of the conditions is violated ier is set to ten. */
/*      1) k+1 <= n-k-1 <= m */
/*      2) t(1) <= t(2) <= ... <= t(k+1) */
/*         t(n-k) <= t(n-k+1) <= ... <= t(n) */
/*      3) t(k+1) < t(k+2) < ... < t(n-k) */
/*      4) t(k+1) <= x(i) <= t(n-k) */
/*      5) the conditions specified by schoenberg and whitney must hold */
/*         for at least one subset of data points, i.e. there must be a */
/*         subset of data points y(j) such that */
/*             t(j) < y(j) < t(j+k+1), j=1,2,...,n-k-1 */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --x;
    --t;

    /* Function Body */
    k1 = *k + 1;
    k2 = k1 + 1;
    nk1 = *n - k1;
    nk2 = nk1 + 1;
    *ier = 10;
/*  check condition no 1 */
    if (nk1 < k1 || nk1 > *m) {
	goto L80;
    }
/*  check condition no 2 */
    j = *n;
    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (t[i__] > t[i__ + 1]) {
	    goto L80;
	}
	if (t[j] < t[j - 1]) {
	    goto L80;
	}
	--j;
/* L20: */
    }
/*  check condition no 3 */
    i__1 = nk2;
    for (i__ = k2; i__ <= i__1; ++i__) {
	if (t[i__] <= t[i__ - 1]) {
	    goto L80;
	}
/* L30: */
    }
/*  check condition no 4 */
    if (x[1] < t[k1] || x[*m] > t[nk2]) {
	goto L80;
    }
/*  check condition no 5 */
    if (x[1] >= t[k2] || x[*m] <= t[nk1]) {
	goto L80;
    }
    i__ = 1;
    l = k2;
    nk3 = nk1 - 1;
    if (nk3 < 2) {
	goto L70;
    }
    i__1 = nk3;
    for (j = 2; j <= i__1; ++j) {
	tj = t[j];
	++l;
	tl = t[l];
L40:
	++i__;
	if (i__ >= *m) {
	    goto L80;
	}
	if (x[i__] <= tj) {
	    goto L40;
	}
	if (x[i__] >= tl) {
	    goto L80;
	}
/* L60: */
    }
L70:
    *ier = 0;
L80:
    return 0;
} /* fpchec_ */

/* Subroutine */ int fpched_(x, m, t, n, k, ib, ie, ier)
real *x;
integer *m;
real *t;
integer *n, *k, *ib, *ie, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, l, k1, k2, jj;
    static real tj, tl;
    static integer ib1, ie1, nk1, nk2, nk3;

/*  subroutine fpched verifies the number and the position of the knots */
/*  t(j),j=1,2,...,n of a spline of degree k,with ib derative constraints */
/*  at x(1) and ie constraints at x(m), in relation to the number and */
/*  the position of the data points x(i),i=1,2,...,m. if all of the */
/*  following conditions are fulfilled, the error parameter ier is set */
/*  to zero. if one of the conditions is violated ier is set to ten. */
/*      1) k+1 <= n-k-1 <= m + max(0,ib-1) + max(0,ie-1) */
/*      2) t(1) <= t(2) <= ... <= t(k+1) */
/*         t(n-k) <= t(n-k+1) <= ... <= t(n) */
/*      3) t(k+1) < t(k+2) < ... < t(n-k) */
/*      4) t(k+1) <= x(i) <= t(n-k) */
/*      5) the conditions specified by schoenberg and whitney must hold */
/*         for at least one subset of data points, i.e. there must be a */
/*         subset of data points y(j) such that */
/*             t(j) < y(j) < t(j+k+1), j=1+ib1,2+ib1,...,n-k-1-ie1 */
/*               with ib1 = max(0,ib-1), ie1 = max(0,ie-1) */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --x;
    --t;

    /* Function Body */
    k1 = *k + 1;
    k2 = k1 + 1;
    nk1 = *n - k1;
    nk2 = nk1 + 1;
    ib1 = *ib - 1;
    if (ib1 < 0) {
	ib1 = 0;
    }
    ie1 = *ie - 1;
    if (ie1 < 0) {
	ie1 = 0;
    }
    *ier = 10;
/*  check condition no 1 */
    if (nk1 < k1 || nk1 > *m + ib1 + ie1) {
	goto L80;
    }
/*  check condition no 2 */
    j = *n;
    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (t[i__] > t[i__ + 1]) {
	    goto L80;
	}
	if (t[j] < t[j - 1]) {
	    goto L80;
	}
	--j;
/* L20: */
    }
/*  check condition no 3 */
    i__1 = nk2;
    for (i__ = k2; i__ <= i__1; ++i__) {
	if (t[i__] <= t[i__ - 1]) {
	    goto L80;
	}
/* L30: */
    }
/*  check condition no 4 */
    if (x[1] < t[k1] || x[*m] > t[nk2]) {
	goto L80;
    }
/*  check condition no 5 */
    if (x[1] >= t[k2] || x[*m] <= t[nk1]) {
	goto L80;
    }
    i__ = 1;
    jj = ib1 + 2;
    l = jj + *k;
    nk3 = nk1 - 1 - ie1;
    if (nk3 < jj) {
	goto L70;
    }
    i__1 = nk3;
    for (j = jj; j <= i__1; ++j) {
	tj = t[j];
	++l;
	tl = t[l];
L40:
	++i__;
	if (i__ >= *m) {
	    goto L80;
	}
	if (x[i__] <= tj) {
	    goto L40;
	}
	if (x[i__] >= tl) {
	    goto L80;
	}
/* L60: */
    }
L70:
    *ier = 0;
L80:
    return 0;
} /* fpched_ */

/* Subroutine */ int fpchep_(x, m, t, n, k, ier)
real *x;
integer *m;
real *t;
integer *n, *k, *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, l, i1, i2, j1, k1, k2, l1, l2, m1, mm;
    static real tj, tl, xi;
    static integer nk1, nk2;
    static real per;

/*  subroutine fpchep verifies the number and the position of the knots */
/*  t(j),j=1,2,...,n of a periodic spline of degree k, in relation to */
/*  the number and the position of the data points x(i),i=1,2,...,m. */
/*  if all of the following conditions are fulfilled, ier is set */
/*  to zero. if one of the conditions is violated ier is set to ten. */
/*      1) k+1 <= n-k-1 <= m+k-1 */
/*      2) t(1) <= t(2) <= ... <= t(k+1) */
/*         t(n-k) <= t(n-k+1) <= ... <= t(n) */
/*      3) t(k+1) < t(k+2) < ... < t(n-k) */
/*      4) t(k+1) <= x(i) <= t(n-k) */
/*      5) the conditions specified by schoenberg and whitney must hold */
/*         for at least one subset of data points, i.e. there must be a */
/*         subset of data points y(j) such that */
/*             t(j) < y(j) < t(j+k+1), j=k+1,...,n-k-1 */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --x;
    --t;

    /* Function Body */
    k1 = *k + 1;
    k2 = k1 + 1;
    nk1 = *n - k1;
    nk2 = nk1 + 1;
    m1 = *m - 1;
    *ier = 10;
/*  check condition no 1 */
    if (nk1 < k1 || *n > *m + (*k << 1)) {
	goto L130;
    }
/*  check condition no 2 */
    j = *n;
    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (t[i__] > t[i__ + 1]) {
	    goto L130;
	}
	if (t[j] < t[j - 1]) {
	    goto L130;
	}
	--j;
/* L20: */
    }
/*  check condition no 3 */
    i__1 = nk2;
    for (i__ = k2; i__ <= i__1; ++i__) {
	if (t[i__] <= t[i__ - 1]) {
	    goto L130;
	}
/* L30: */
    }
/*  check condition no 4 */
    if (x[1] < t[k1] || x[*m] > t[nk2]) {
	goto L130;
    }
/*  check condition no 5 */
    l1 = k1;
    l2 = 1;
    i__1 = *m;
    for (l = 1; l <= i__1; ++l) {
	xi = x[l];
L40:
	if (xi < t[l1 + 1] || l == nk1) {
	    goto L50;
	}
	++l1;
	++l2;
	if (l2 > k1) {
	    goto L60;
	}
	goto L40;
L50:
	;
    }
    l = *m;
L60:
    per = t[nk2] - t[k1];
    i__1 = l;
    for (i1 = 2; i1 <= i__1; ++i1) {
	i__ = i1 - 1;
	mm = i__ + m1;
	i__2 = nk1;
	for (j = k1; j <= i__2; ++j) {
	    tj = t[j];
	    j1 = j + k1;
	    tl = t[j1];
L70:
	    ++i__;
	    if (i__ > mm) {
		goto L120;
	    }
	    i2 = i__ - m1;
	    if (i2 <= 0) {
		goto L80;
	    } else {
		goto L90;
	    }
L80:
	    xi = x[i__];
	    goto L100;
L90:
	    xi = x[i2] + per;
L100:
	    if (xi <= tj) {
		goto L70;
	    }
	    if (xi >= tl) {
		goto L120;
	    }
/* L110: */
	}
	*ier = 0;
	goto L130;
L120:
	;
    }
L130:
    return 0;
} /* fpchep_ */

/* Subroutine */ int fpclos_(iopt, idim, m, u, mx, x, w, k, s, nest, tol, 
	maxit, k1, k2, n, t, nc, c__, fp, fpint, z__, a1, a2, b, g1, g2, q, 
	nrdata, ier)
integer *iopt, *idim, *m;
real *u;
integer *mx;
real *x, *w;
integer *k;
real *s;
integer *nest;
real *tol;
integer *maxit, *k1, *k2, *n;
real *t;
integer *nc;
real *c__, *fp, *fpint, *z__, *a1, *a2, *b, *g1, *g2, *q;
integer *nrdata, *ier;
{
    /* System generated locals */
    integer a1_dim1, a1_offset, a2_dim1, a2_offset, b_dim1, b_offset, g1_dim1,
	     g1_offset, g2_dim1, g2_offset, q_dim1, q_offset, i__1, i__2, 
	    i__3, i__4, i__5;
    real r__1;

    /* Local variables */
    static real half;
    static integer jper, nmin, iter, nmax;
    static real fpms, term, pinv, h__[6];
    static integer i__, j, l;
    static real p, fpold, fpart, d1, f1, f2, f3, store;
    static integer i1, i2;
    static real p1, p2, p3;
    static integer i3, j1, j2, k3, l0, l1, l5, m1, nplus, nrint, n7, n8;
    static real h1[7], h2[6];
    static integer n10, n11, ij, ik, jj, jk, kk, mm;
    extern /* Subroutine */ int fpbacp_();
    static integer it;
    static real ui, wi, rn, xi[10];
    extern /* Subroutine */ int fpdisc_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpbspl_(), fprota_(), fpgivs_(), fpknot_();
    static real fp0;
    static integer kk1, nk1, nk2;
    static real acc, fac, one, cos__, per, sin__;
    static integer new__;
    static real piv;
    static integer ich1, ich3;
    static real con1, con4, con9;
    static integer npl1;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpbacp,fpbspl,fpgivs,fpdisc,fpknot,fprota */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --w;
    --u;
    --x;
    --nrdata;
    a2_dim1 = *nest;
    a2_offset = 1 + a2_dim1 * 1;
    a2 -= a2_offset;
    --fpint;
    --t;
    q_dim1 = *m;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    g2_dim1 = *nest;
    g2_offset = 1 + g2_dim1 * 1;
    g2 -= g2_offset;
    a1_dim1 = *nest;
    a1_offset = 1 + a1_dim1 * 1;
    a1 -= a1_offset;
    g1_dim1 = *nest;
    g1_offset = 1 + g1_dim1 * 1;
    g1 -= g1_offset;
    b_dim1 = *nest;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    --z__;
    --c__;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
    half = (float).5;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 1: determination of the number of knots and their position     c */
/*  **************************************************************      c */
/*  given a set of knots we compute the least-squares closed curve      c */
/*  sinf(u). if the sum f(p=inf) <= s we accept the choice of knots.    c */
/*  if iopt=-1 sinf(u) is the requested curve                           c */
/*  if iopt=0 or iopt=1 we check whether we can accept the knots:       c */
/*    if fp <=s we will continue with the current set of knots.         c */
/*    if fp > s we will increase the number of knots and compute the    c */
/*       corresponding least-squares curve until finally fp<=s.         c */
/*  the initial choice of knots depends on the value of s and iopt.     c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*    knots equals nmax = m+2*k.                                        c */
/*    if s > 0 and                                                      c */
/*      iopt=0 we first compute the least-squares polynomial curve of   c */
/*      degree k; n = nmin = 2*k+2. since s(u) must be periodic we      c */
/*      find that s(u) reduces to a fixed point.                        c */
/*      iopt=1 we start with the set of knots found at the last         c */
/*      call of the routine, except for the case that s > fp0; then     c */
/*      we compute directly the least-squares polynomial curve.         c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
    m1 = *m - 1;
    kk = *k;
    kk1 = *k1;
    k3 = *k * 3 + 1;
    nmin = *k1 << 1;
/*  determine the length of the period of the splines. */
    per = u[*m] - u[1];
    if (*iopt < 0) {
	goto L50;
    }
/*  calculation of acc, the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  determine nmax, the number of knots for periodic spline interpolation */
    nmax = *m + (*k << 1);
    if (*s > (float)0. || nmax == nmin) {
	goto L30;
    }
/*  if s=0, s(u) is an interpolating curve. */
    *n = nmax;
/*  test whether the required storage space exceeds the available one. */
    if (*n > *nest) {
	goto L620;
    }
/*  find the position of the interior knots in case of interpolation. */
L5:
    if (*k / 2 << 1 == *k) {
	goto L20;
    }
    i__1 = m1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	j = i__ + *k;
	t[j] = u[i__];
/* L10: */
    }
    if (*s > (float)0.) {
	goto L50;
    }
    kk = *k - 1;
    kk1 = *k;
    if (kk > 0) {
	goto L50;
    }
    t[1] = t[*m] - per;
    t[2] = u[1];
    t[*m + 1] = u[*m];
    t[*m + 2] = t[3] + per;
    jj = 0;
    i__1 = m1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = i__;
	i__2 = *idim;
	for (j1 = 1; j1 <= i__2; ++j1) {
	    ++jj;
	    c__[j] = x[jj];
	    j += *n;
/* L12: */
	}
/* L15: */
    }
    jj = 1;
    j = *m;
    i__1 = *idim;
    for (j1 = 1; j1 <= i__1; ++j1) {
	c__[j] = c__[jj];
	j += *n;
	jj += *n;
/* L17: */
    }
    *fp = (float)0.;
    fpint[*n] = fp0;
    fpint[*n - 1] = (float)0.;
    nrdata[*n] = 0;
    goto L630;
L20:
    i__1 = m1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	j = i__ + *k;
	t[j] = (u[i__] + u[i__ - 1]) * half;
/* L25: */
    }
    goto L50;
/*  if s > 0 our initial choice depends on the value of iopt. */
/*  if iopt=0 or iopt=1 and s>=fp0, we start computing the least-squares */
/*  polynomial curve. (i.e. a constant point). */
/*  if iopt=1 and fp0>s we start computing the least-squares closed */
/*  curve according the set of knots found at the last call of the */
/*  routine. */
L30:
    if (*iopt == 0) {
	goto L35;
    }
    if (*n == nmin) {
	goto L35;
    }
    fp0 = fpint[*n];
    fpold = fpint[*n - 1];
    nplus = nrdata[*n];
    if (fp0 > *s) {
	goto L50;
    }
/*  the case that s(u) is a fixed point is treated separetely. */
/*  fp0 denotes the corresponding sum of squared residuals. */
L35:
    fp0 = (float)0.;
    d1 = (float)0.;
    i__1 = *idim;
    for (j = 1; j <= i__1; ++j) {
	z__[j] = (float)0.;
/* L37: */
    }
    jj = 0;
    i__1 = m1;
    for (it = 1; it <= i__1; ++it) {
	wi = w[it];
	fpgivs_(&wi, &d1, &cos__, &sin__);
	i__2 = *idim;
	for (j = 1; j <= i__2; ++j) {
	    ++jj;
	    fac = wi * x[jj];
	    fprota_(&cos__, &sin__, &fac, &z__[j]);
/* Computing 2nd power */
	    r__1 = fac;
	    fp0 += r__1 * r__1;
/* L40: */
	}
/* L45: */
    }
    i__1 = *idim;
    for (j = 1; j <= i__1; ++j) {
	z__[j] /= d1;
/* L47: */
    }
/*  test whether that fixed point is a solution of our problem. */
    fpms = fp0 - *s;
    if (fpms < acc || nmax == nmin) {
	goto L640;
    }
    fpold = fp0;
/*  test whether the required storage space exceeds the available one. */
    if (*n >= *nest) {
	goto L620;
    }
/*  start computing the least-squares closed curve with one */
/*  interior knot. */
    nplus = 1;
    *n = nmin + 1;
    mm = (*m + 1) / 2;
    t[*k2] = u[mm];
    nrdata[1] = mm - 2;
    nrdata[2] = m1 - mm;
/*  main loop for the different sets of knots. m is a save upper */
/*  bound for the number of trials. */
L50:
    i__1 = *m;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find nrint, the number of knot intervals. */
	nrint = *n - nmin + 1;
/*  find the position of the additional knots which are needed for */
/*  the b-spline representation of s(u). if we take */
/*      t(k+1) = u(1), t(n-k) = u(m) */
/*      t(k+1-j) = t(n-k-j) - per, j=1,2,...k */
/*      t(n-k+j) = t(k+1+j) + per, j=1,2,...k */
/*  then s(u) will be a smooth closed curve if the b-spline */
/*  coefficients satisfy the following conditions */
/*      c((i-1)*n+n7+j) = c((i-1)*n+j), j=1,...k,i=1,2,...,idim (**) */
/*  with n7=n-2*k-1. */
	t[*k1] = u[1];
	nk1 = *n - *k1;
	nk2 = nk1 + 1;
	t[nk2] = u[*m];
	i__2 = *k;
	for (j = 1; j <= i__2; ++j) {
	    i1 = nk2 + j;
	    i2 = nk2 - j;
	    j1 = *k1 + j;
	    j2 = *k1 - j;
	    t[i1] = t[j1] + per;
	    t[j2] = t[i2] - per;
/* L60: */
	}
/*  compute the b-spline coefficients of the least-squares closed curve */
/*  sinf(u). the observation matrix a is built up row by row while */
/*  taking into account condition (**) and is reduced to triangular */
/*  form by givens transformations . */
/*  at the same time fp=f(p=inf) is computed. */
/*  the n7 x n7 triangularised upper matrix a has the form */
/*            ! a1 '    ! */
/*        a = !    ' a2 ! */
/*            ! 0  '    ! */
/*  with a2 a n7 x k matrix and a1 a n10 x n10 upper triangular */
/*  matrix of bandwith k+1 ( n10 = n7-k). */
/*  initialization. */
	i__2 = *nc;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    z__[i__] = (float)0.;
/* L65: */
	}
	i__2 = nk1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = kk1;
	    for (j = 1; j <= i__3; ++j) {
		a1[i__ + j * a1_dim1] = (float)0.;
/* L70: */
	    }
	}
	n7 = nk1 - *k;
	n10 = n7 - kk;
	jper = 0;
	*fp = (float)0.;
	l = *k1;
	jj = 0;
	i__3 = m1;
	for (it = 1; it <= i__3; ++it) {
/*  fetch the current data point u(it),x(it) */
	    ui = u[it];
	    wi = w[it];
	    i__2 = *idim;
	    for (j = 1; j <= i__2; ++j) {
		++jj;
		xi[j - 1] = x[jj] * wi;
/* L75: */
	    }
/*  search for knot interval t(l) <= ui < t(l+1). */
L80:
	    if (ui < t[l + 1]) {
		goto L85;
	    }
	    ++l;
	    goto L80;
/*  evaluate the (k+1) non-zero b-splines at ui and store them in q. */
L85:
	    fpbspl_(&t[1], n, k, &ui, &l, h__);
	    i__2 = *k1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		q[it + i__ * q_dim1] = h__[i__ - 1];
		h__[i__ - 1] *= wi;
/* L90: */
	    }
	    l5 = l - *k1;
/*  test whether the b-splines nj,k+1(u),j=1+n7,...nk1 are all zero at ui */
	    if (l5 < n10) {
		goto L285;
	    }
	    if (jper != 0) {
		goto L160;
	    }
/*  initialize the matrix a2. */
	    i__2 = n7;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		i__4 = kk;
		for (j = 1; j <= i__4; ++j) {
		    a2[i__ + j * a2_dim1] = (float)0.;
/* L95: */
		}
	    }
	    jk = n10 + 1;
	    i__4 = kk;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		ik = jk;
		i__2 = kk1;
		for (j = 1; j <= i__2; ++j) {
		    if (ik <= 0) {
			goto L105;
		    }
		    a2[ik + i__ * a2_dim1] = a1[ik + j * a1_dim1];
		    --ik;
/* L100: */
		}
L105:
		++jk;
/* L110: */
	    }
	    jper = 1;
/*  if one of the b-splines nj,k+1(u),j=n7+1,...nk1 is not zero at ui */
/*  we take account of condition (**) for setting up the new row */
/*  of the observation matrix a. this row is stored in the arrays h1 */
/*  (the part with respect to a1) and h2 (the part with */
/*  respect to a2). */
L160:
	    i__4 = kk;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		h1[i__ - 1] = (float)0.;
		h2[i__ - 1] = (float)0.;
/* L170: */
	    }
	    h1[kk1 - 1] = (float)0.;
	    j = l5 - n10;
	    i__4 = kk1;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		++j;
		l0 = j;
L180:
		l1 = l0 - kk;
		if (l1 <= 0) {
		    goto L200;
		}
		if (l1 <= n10) {
		    goto L190;
		}
		l0 = l1 - n10;
		goto L180;
L190:
		h1[l1 - 1] = h__[i__ - 1];
		goto L210;
L200:
		h2[l0 - 1] += h__[i__ - 1];
L210:
		;
	    }
/*  rotate the new row of the observation matrix into triangle */
/*  by givens transformations. */
	    if (n10 <= 0) {
		goto L250;
	    }
/*  rotation with the rows 1,2,...n10 of matrix a. */
	    i__4 = n10;
	    for (j = 1; j <= i__4; ++j) {
		piv = h1[0];
		if (piv != (float)0.) {
		    goto L214;
		}
		i__2 = kk;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    h1[i__ - 1] = h1[i__];
/* L212: */
		}
		h1[kk1 - 1] = (float)0.;
		goto L240;
/*  calculate the parameters of the givens transformation. */
L214:
		fpgivs_(&piv, &a1[j + a1_dim1], &cos__, &sin__);
/*  transformation to the right hand side. */
		j1 = j;
		i__2 = *idim;
		for (j2 = 1; j2 <= i__2; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &z__[j1]);
		    j1 += *n;
/* L217: */
		}
/*  transformations to the left hand side with respect to a2. */
		i__2 = kk;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    fprota_(&cos__, &sin__, &h2[i__ - 1], &a2[j + i__ * 
			    a2_dim1]);
/* L220: */
		}
		if (j == n10) {
		    goto L250;
		}
/* Computing MIN */
		i__2 = n10 - j;
		i2 = min(i__2,kk);
/*  transformations to the left hand side with respect to a1. */
		i__2 = i2;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i1 = i__ + 1;
		    fprota_(&cos__, &sin__, &h1[i1 - 1], &a1[j + i1 * a1_dim1]
			    );
		    h1[i__ - 1] = h1[i1 - 1];
/* L230: */
		}
		h1[i1 - 1] = (float)0.;
L240:
		;
	    }
/*  rotation with the rows n10+1,...n7 of matrix a. */
L250:
	    i__4 = kk;
	    for (j = 1; j <= i__4; ++j) {
		ij = n10 + j;
		if (ij <= 0) {
		    goto L270;
		}
		piv = h2[j - 1];
		if (piv == (float)0.) {
		    goto L270;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a2[ij + j * a2_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		j1 = ij;
		i__2 = *idim;
		for (j2 = 1; j2 <= i__2; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &z__[j1]);
		    j1 += *n;
/* L255: */
		}
		if (j == kk) {
		    goto L280;
		}
		j1 = j + 1;
/*  transformations to left hand side. */
		i__2 = kk;
		for (i__ = j1; i__ <= i__2; ++i__) {
		    fprota_(&cos__, &sin__, &h2[i__ - 1], &a2[ij + i__ * 
			    a2_dim1]);
/* L260: */
		}
L270:
		;
	    }
/*  add contribution of this row to the sum of squares of residual */
/*  right hand sides. */
L280:
	    i__4 = *idim;
	    for (j2 = 1; j2 <= i__4; ++j2) {
/* Computing 2nd power */
		r__1 = xi[j2 - 1];
		*fp += r__1 * r__1;
/* L282: */
	    }
	    goto L290;
/*  rotation of the new row of the observation matrix into */
/*  triangle in case the b-splines nj,k+1(u),j=n7+1,...n-k-1 are all zero */
/*  at ui. */
L285:
	    j = l5;
	    i__4 = kk1;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		++j;
		piv = h__[i__ - 1];
		if (piv == (float)0.) {
		    goto L140;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a1[j + a1_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		j1 = j;
		i__2 = *idim;
		for (j2 = 1; j2 <= i__2; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &z__[j1]);
		    j1 += *n;
/* L125: */
		}
		if (i__ == kk1) {
		    goto L150;
		}
		i2 = 1;
		i3 = i__ + 1;
/*  transformations to left hand side. */
		i__2 = kk1;
		for (i1 = i3; i1 <= i__2; ++i1) {
		    ++i2;
		    fprota_(&cos__, &sin__, &h__[i1 - 1], &a1[j + i2 * 
			    a1_dim1]);
/* L130: */
		}
L140:
		;
	    }
/*  add contribution of this row to the sum of squares of residual */
/*  right hand sides. */
L150:
	    i__4 = *idim;
	    for (j2 = 1; j2 <= i__4; ++j2) {
/* Computing 2nd power */
		r__1 = xi[j2 - 1];
		*fp += r__1 * r__1;
/* L155: */
	    }
L290:
	    ;
	}
	fpint[*n] = fp0;
	fpint[*n - 1] = fpold;
	nrdata[*n] = nplus;
/*  backward substitution to obtain the b-spline coefficients . */
	j1 = 1;
	i__3 = *idim;
	for (j2 = 1; j2 <= i__3; ++j2) {
	    fpbacp_(&a1[a1_offset], &a2[a2_offset], &z__[j1], &n7, &kk, &c__[
		    j1], &kk1, nest);
	    j1 += *n;
/* L292: */
	}
/*  calculate from condition (**) the remaining coefficients. */
	i__3 = *k;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    j1 = i__;
	    i__4 = *idim;
	    for (j = 1; j <= i__4; ++j) {
		j2 = j1 + n7;
		c__[j2] = c__[j1];
		j1 += *n;
/* L295: */
	    }
/* L297: */
	}
	if (*iopt < 0) {
	    goto L660;
	}
/*  test whether the approximation sinf(u) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L660;
	}
/*  if f(p=inf) < s accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L350;
	}
/*  if n=nmax, sinf(u) is an interpolating curve. */
	if (*n == nmax) {
	    goto L630;
	}
/*  increase the number of knots. */
/*  if n=nest we cannot increase the number of knots because of the */
/*  storage capacity limitation. */
	if (*n == *nest) {
	    goto L620;
	}
/*  determine the number of knots nplus we are going to add. */
	npl1 = nplus << 1;
	rn = (real) nplus;
	if (fpold - *fp > acc) {
	    npl1 = rn * fpms / (fpold - *fp);
	}
/* Computing MIN */
/* Computing MAX */
	i__2 = npl1, i__5 = nplus / 2, i__2 = max(i__2,i__5);
	i__3 = nplus << 1, i__4 = max(i__2,1);
	nplus = min(i__3,i__4);
	fpold = *fp;
/*  compute the sum of squared residuals for each knot interval */
/*  t(j+k) <= ui <= t(j+k+1) and store it in fpint(j),j=1,2,...nrint. */
	fpart = (float)0.;
	i__ = 1;
	l = *k1;
	jj = 0;
	i__3 = m1;
	for (it = 1; it <= i__3; ++it) {
	    if (u[it] < t[l]) {
		goto L300;
	    }
	    new__ = 1;
	    ++l;
L300:
	    term = (float)0.;
	    l0 = l - *k2;
	    i__4 = *idim;
	    for (j2 = 1; j2 <= i__4; ++j2) {
		fac = (float)0.;
		j1 = l0;
		i__2 = *k1;
		for (j = 1; j <= i__2; ++j) {
		    ++j1;
		    fac += c__[j1] * q[it + j * q_dim1];
/* L305: */
		}
		++jj;
/* Computing 2nd power */
		r__1 = w[it] * (fac - x[jj]);
		term += r__1 * r__1;
		l0 += *n;
/* L310: */
	    }
	    fpart += term;
	    if (new__ == 0) {
		goto L320;
	    }
	    if (l > *k2) {
		goto L315;
	    }
	    fpint[nrint] = term;
	    new__ = 0;
	    goto L320;
L315:
	    store = term * half;
	    fpint[i__] = fpart - store;
	    ++i__;
	    fpart = store;
	    new__ = 0;
L320:
	    ;
	}
	fpint[nrint] += fpart;
	i__3 = nplus;
	for (l = 1; l <= i__3; ++l) {
/*  add a new knot */
	    fpknot_(&u[1], m, &t[1], n, &fpint[1], &nrdata[1], &nrint, nest, &
		    c__1);
/*  if n=nmax we locate the knots as for interpolation */
	    if (*n == nmax) {
		goto L5;
	    }
/*  test whether we cannot further increase the number of knots. */
	    if (*n == *nest) {
		goto L340;
	    }
/* L330: */
	}
/*  restart the computations with the new set of knots. */
L340:
	;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 2: determination of the smoothing closed curve sp(u).          c */
/*  **********************************************************          c */
/*  we have determined the number of knots and their position.          c */
/*  we now compute the b-spline coefficients of the smoothing curve     c */
/*  sp(u). the observation matrix a is extended by the rows of matrix   c */
/*  b expressing that the kth derivative discontinuities of sp(u) at    c */
/*  the interior knots t(k+2),...t(n-k-1) must be zero. the corres-     c */
/*  ponding weights of these additional rows are set to 1/p.            c */
/*  iteratively we then have to determine the value of p such that f(p),c */
/*  the sum of squared residuals be = s. we already know that the least-c */
/*  squares polynomial curve corresponds to p=0, and that the least-    c */
/*  squares periodic spline curve corresponds to p=infinity. the        c */
/*  iteration process which is proposed here, makes use of rational     c */
/*  interpolation. since f(p) is a convex and strictly decreasing       c */
/*  function of p, it can be approximated by a rational function        c */
/*  r(p) = (u*p+v)/(p+w). three values of p(p1,p2,p3) with correspond-  c */
/*  ing values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s) are used      c */
/*  to calculate the new value of p such that r(p)=s. convergence is    c */
/*  guaranteed by taking f1>0 and f3<0.                                 c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  evaluate the discontinuity jump of the kth derivative of the */
/*  b-splines at the knots t(l),l=k+2,...n-k-1 and store in b. */
L350:
    fpdisc_(&t[1], n, k2, &b[b_offset], nest);
/*  initial value for p. */
    p1 = (float)0.;
    f1 = fp0 - *s;
    p3 = -one;
    f3 = fpms;
    n11 = n10 - 1;
    n8 = n7 - 1;
    p = (float)0.;
    l = n7;
    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = *k + 1 - i__;
	p += a2[l + j * a2_dim1];
	--l;
	if (l == 0) {
	    goto L356;
	}
/* L352: */
    }
    i__1 = n10;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p += a1[i__ + a1_dim1];
/* L354: */
    }
L356:
    rn = (real) n7;
    p = rn / p;
    ich1 = 0;
    ich3 = 0;
/*  iteration process to find the root of f(p) = s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  form the matrix g  as the matrix a extended by the rows of matrix b. */
/*  the rows of matrix b with weight 1/p are rotated into */
/*  the triangularised observation matrix a. */
/*  after triangularisation our n7 x n7 matrix g takes the form */
/*            ! g1 '    ! */
/*        g = !    ' g2 ! */
/*            ! 0  '    ! */
/*  with g2 a n7 x (k+1) matrix and g1 a n11 x n11 upper triangular */
/*  matrix of bandwidth k+2. ( n11 = n7-k-1) */
	pinv = one / p;
/*  store matrix a into g */
	i__3 = *nc;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    c__[i__] = z__[i__];
/* L358: */
	}
	i__3 = n7;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    g1[i__ + *k1 * g1_dim1] = a1[i__ + *k1 * a1_dim1];
	    g1[i__ + *k2 * g1_dim1] = (float)0.;
	    g2[i__ + g2_dim1] = (float)0.;
	    i__4 = *k;
	    for (j = 1; j <= i__4; ++j) {
		g1[i__ + j * g1_dim1] = a1[i__ + j * a1_dim1];
		g2[i__ + (j + 1) * g2_dim1] = a2[i__ + j * a2_dim1];
/* L360: */
	    }
	}
	l = n10;
	i__4 = *k1;
	for (j = 1; j <= i__4; ++j) {
	    if (l <= 0) {
		goto L375;
	    }
	    g2[l + g2_dim1] = a1[l + j * a1_dim1];
	    --l;
/* L370: */
	}
L375:
	i__4 = n8;
	for (it = 1; it <= i__4; ++it) {
/*  fetch a new row of matrix b and store it in the arrays h1 (the part */
/*  with respect to g1) and h2 (the part with respect to g2). */
	    i__3 = *idim;
	    for (j = 1; j <= i__3; ++j) {
		xi[j - 1] = (float)0.;
/* L380: */
	    }
	    i__3 = *k1;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h1[i__ - 1] = (float)0.;
		h2[i__ - 1] = (float)0.;
/* L385: */
	    }
	    h1[*k2 - 1] = (float)0.;
	    if (it > n11) {
		goto L420;
	    }
	    l = it;
	    l0 = it;
	    i__3 = *k2;
	    for (j = 1; j <= i__3; ++j) {
		if (l0 == n10) {
		    goto L400;
		}
		h1[j - 1] = b[it + j * b_dim1] * pinv;
		++l0;
/* L390: */
	    }
	    goto L470;
L400:
	    l0 = 1;
	    i__3 = *k2;
	    for (l1 = j; l1 <= i__3; ++l1) {
		h2[l0 - 1] = b[it + l1 * b_dim1] * pinv;
		++l0;
/* L410: */
	    }
	    goto L470;
L420:
	    l = 1;
	    i__ = it - n10;
	    i__3 = *k2;
	    for (j = 1; j <= i__3; ++j) {
		++i__;
		l0 = i__;
L430:
		l1 = l0 - *k1;
		if (l1 <= 0) {
		    goto L450;
		}
		if (l1 <= n11) {
		    goto L440;
		}
		l0 = l1 - n11;
		goto L430;
L440:
		h1[l1 - 1] = b[it + j * b_dim1] * pinv;
		goto L460;
L450:
		h2[l0 - 1] += b[it + j * b_dim1] * pinv;
L460:
		;
	    }
	    if (n11 <= 0) {
		goto L510;
	    }
/*  rotate this row into triangle by givens transformations */
/*  rotation with the rows l,l+1,...n11. */
L470:
	    i__3 = n11;
	    for (j = l; j <= i__3; ++j) {
		piv = h1[0];
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &g1[j + g1_dim1], &cos__, &sin__);
/*  transformation to right hand side. */
		j1 = j;
		i__2 = *idim;
		for (j2 = 1; j2 <= i__2; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &c__[j1]);
		    j1 += *n;
/* L475: */
		}
/*  transformation to the left hand side with respect to g2. */
		i__2 = *k1;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    fprota_(&cos__, &sin__, &h2[i__ - 1], &g2[j + i__ * 
			    g2_dim1]);
/* L480: */
		}
		if (j == n11) {
		    goto L510;
		}
/* Computing MIN */
		i__2 = n11 - j;
		i2 = min(i__2,*k1);
/*  transformation to the left hand side with respect to g1. */
		i__2 = i2;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i1 = i__ + 1;
		    fprota_(&cos__, &sin__, &h1[i1 - 1], &g1[j + i1 * g1_dim1]
			    );
		    h1[i__ - 1] = h1[i1 - 1];
/* L490: */
		}
		h1[i1 - 1] = (float)0.;
/* L500: */
	    }
/*  rotation with the rows n11+1,...n7 */
L510:
	    i__3 = *k1;
	    for (j = 1; j <= i__3; ++j) {
		ij = n11 + j;
		if (ij <= 0) {
		    goto L530;
		}
		piv = h2[j - 1];
/*  calculate the parameters of the givens transformation */
		fpgivs_(&piv, &g2[ij + j * g2_dim1], &cos__, &sin__);
/*  transformation to the right hand side. */
		j1 = ij;
		i__2 = *idim;
		for (j2 = 1; j2 <= i__2; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &c__[j1]);
		    j1 += *n;
/* L515: */
		}
		if (j == *k1) {
		    goto L540;
		}
		j1 = j + 1;
/*  transformation to the left hand side. */
		i__2 = *k1;
		for (i__ = j1; i__ <= i__2; ++i__) {
		    fprota_(&cos__, &sin__, &h2[i__ - 1], &g2[ij + i__ * 
			    g2_dim1]);
/* L520: */
		}
L530:
		;
	    }
L540:
	    ;
	}
/*  backward substitution to obtain the b-spline coefficients */
	j1 = 1;
	i__4 = *idim;
	for (j2 = 1; j2 <= i__4; ++j2) {
	    fpbacp_(&g1[g1_offset], &g2[g2_offset], &c__[j1], &n7, k1, &c__[
		    j1], k2, nest);
	    j1 += *n;
/* L542: */
	}
/*  calculate from condition (**) the remaining b-spline coefficients. */
	i__4 = *k;
	for (i__ = 1; i__ <= i__4; ++i__) {
	    j1 = i__;
	    i__3 = *idim;
	    for (j = 1; j <= i__3; ++j) {
		j2 = j1 + n7;
		c__[j2] = c__[j1];
		j1 += *n;
/* L545: */
	    }
/* L547: */
	}
/*  computation of f(p). */
	*fp = (float)0.;
	l = *k1;
	jj = 0;
	i__4 = m1;
	for (it = 1; it <= i__4; ++it) {
	    if (u[it] < t[l]) {
		goto L550;
	    }
	    ++l;
L550:
	    l0 = l - *k2;
	    term = (float)0.;
	    i__3 = *idim;
	    for (j2 = 1; j2 <= i__3; ++j2) {
		fac = (float)0.;
		j1 = l0;
		i__2 = *k1;
		for (j = 1; j <= i__2; ++j) {
		    ++j1;
		    fac += c__[j1] * q[it + j * q_dim1];
/* L560: */
		}
		++jj;
/* Computing 2nd power */
		r__1 = fac - x[jj];
		term += r__1 * r__1;
		l0 += *n;
/* L565: */
	    }
/* Computing 2nd power */
	    r__1 = w[it];
	    *fp += term * (r__1 * r__1);
/* L570: */
	}
/*  test whether the approximation sp(u) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L660;
	}
/*  test whether the maximal number of iterations is reached. */
	if (iter == *maxit) {
	    goto L600;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L580;
	}
	if (f2 - f3 > acc) {
	    goto L575;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L595;
L575:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L580:
	if (ich1 != 0) {
	    goto L590;
	}
	if (f1 - f2 > acc) {
	    goto L585;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L595;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L595;
L585:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L590:
	if (f2 >= f1 || f2 <= f3) {
	    goto L610;
	}
/*  find the new value for p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L595:
	;
    }
/*  error codes and messages. */
L600:
    *ier = 3;
    goto L660;
L610:
    *ier = 2;
    goto L660;
L620:
    *ier = 1;
    goto L660;
L630:
    *ier = -1;
    goto L660;
L640:
    *ier = -2;
/*  the point (z(1),z(2),...,z(idim)) is a solution of our problem. */
/*  a constant function is a spline of degree k with all b-spline */
/*  coefficients equal to that constant. */
    i__1 = *k1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rn = (real) (*k1 - i__);
	t[i__] = u[1] - rn * per;
	j = i__ + *k1;
	rn = (real) (i__ - 1);
	t[j] = u[*m] + rn * per;
/* L650: */
    }
    *n = nmin;
    j1 = 0;
    i__1 = *idim;
    for (j = 1; j <= i__1; ++j) {
	fac = z__[j];
	j2 = j1;
	i__4 = *k1;
	for (i__ = 1; i__ <= i__4; ++i__) {
	    ++j2;
	    c__[j2] = fac;
/* L654: */
	}
	j1 += *n;
/* L658: */
    }
    *fp = fp0;
    fpint[*n] = fp0;
    fpint[*n - 1] = (float)0.;
    nrdata[*n] = 0;
L660:
    return 0;
} /* fpclos_ */

/* Subroutine */ int fpcoco_(iopt, m, x, y, w, v, s, nest, maxtr, maxbin, n, 
	t, c__, sq, sx, bind, e, wrk, lwrk, iwrk, kwrk, ier)
integer *iopt, *m;
real *x, *y, *w, *v, *s;
integer *nest, *maxtr, *maxbin, *n;
real *t, *c__, *sq, *sx;
logical *bind;
real *e, *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Local variables */
    static real half;
    static integer nmax;
    static real term;
    static integer i__, j, k, l;
    static real sqmax;
    static integer i1, l1, m1, n4, n6, n8, ia, ib, ic, mb, ji, jl, iq, nm, jr,
	     iu, ju, nr;
    static real tj, xi;
    static integer iz;
    extern /* Subroutine */ int fpcosp_();
    static integer jib, jjb;
    static real sql;
    static integer izz;

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..subroutine references.. */
/*    fpcosp,fpbspl,fpadno,fpdeno,fpseno,fpfrno */
/*  .. */
/*  set constant */
    /* Parameter adjustments */
    --sx;
    --v;
    --w;
    --y;
    --x;
    --e;
    --bind;
    --c__;
    --t;
    --wrk;
    --iwrk;

    /* Function Body */
    half = (float).5;
/*  determine the maximal admissible number of knots. */
    nmax = *m + 4;
/*  the initial choice of knots depends on the value of iopt. */
/*    if iopt=0 the program starts with the minimal number of knots */
/*    so that can be guarantied that the concavity/convexity constraints */
/*    will be satisfied. */
/*    if iopt = 1 the program will continue from the point on where she */
/*    left at the foregoing call. */
    if (*iopt > 0) {
	goto L80;
    }
/*  find the minimal number of knots. */
/*  a knot is located at the data point x(i), i=2,3,...m-1 if */
/*    1) v(i) ^= 0    and */
/*    2) v(i)*v(i-1) <= 0  or  v(i)*v(i+1) <= 0. */
    m1 = *m - 1;
    *n = 4;
    i__1 = m1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (v[i__] == (float)0. || v[i__] * v[i__ - 1] > (float)0. && v[i__] *
		 v[i__ + 1] > (float)0.) {
	    goto L20;
	}
	++(*n);
/*  test whether the required storage space exceeds the available one. */
	if (*n + 4 > *nest) {
	    goto L200;
	}
	t[*n] = x[i__];
L20:
	;
    }
/*  find the position of the knots t(1),...t(4) and t(n-3),...t(n) which */
/*  are needed for the b-spline representation of s(x). */
    for (i__ = 1; i__ <= 4; ++i__) {
	t[i__] = x[1];
	++(*n);
	t[*n] = x[*m];
/* L30: */
    }
/*  test whether the minimum number of knots exceeds the maximum number. */
    if (*n > nmax) {
	goto L210;
    }
/*  main loop for the different sets of knots. */
/*  find corresponding values e(j) to the knots t(j+3),j=1,2,...n-6 */
/*    e(j) will take the value -1,1, or 0 according to the requirement */
/*    that s(x) must be locally convex or concave at t(j+3) or that the */
/*    sign of s''(x) is unrestricted at that point. */
L40:
    i__ = 1;
    xi = x[1];
    j = 4;
    tj = t[4];
    n6 = *n - 6;
    i__1 = n6;
    for (l = 1; l <= i__1; ++l) {
L50:
	if (xi == tj) {
	    goto L60;
	}
	++i__;
	xi = x[i__];
	goto L50;
L60:
	e[l] = v[i__];
	++j;
	tj = t[j];
/* L70: */
    }
/*  we partition the working space */
    nm = *n + *maxbin;
    mb = *maxbin + 1;
    ia = 1;
    ib = ia + (*n << 2);
    ic = ib + nm * *maxbin;
    iz = ic + *n;
    izz = iz + *n;
    iu = izz + *n;
    iq = iu + *maxbin;
    ji = 1;
    ju = ji + *maxtr;
    jl = ju + *maxtr;
    jr = jl + *maxtr;
    jjb = jr + *maxtr;
    jib = jjb + mb;
/*  given the set of knots t(j),j=1,2,...n, find the least-squares cubic */
/*  spline which satisfies the imposed concavity/convexity constraints. */
    fpcosp_(m, &x[1], &y[1], &w[1], n, &t[1], &e[1], maxtr, maxbin, &c__[1], 
	    sq, &sx[1], &bind[1], &nm, &mb, &wrk[ia], &wrk[ib], &wrk[ic], &
	    wrk[iz], &wrk[izz], &wrk[iu], &wrk[iq], &iwrk[ji], &iwrk[ju], &
	    iwrk[jl], &iwrk[jr], &iwrk[jjb], &iwrk[jib], ier);
/*  if sq <= s or in case of abnormal exit from fpcosp, control is */
/*  repassed to the driver program. */
    if (*sq <= *s || *ier > 0) {
	goto L300;
    }
/*  calculate for each knot interval t(l-1) <= xi <= t(l) the */
/*  sum((wi*(yi-s(xi)))**2). */
/*  find the interval t(k-1) <= x <= t(k) for which this sum is maximal */
/*  on the condition that this interval contains at least one interior */
/*  data point x(nr) and that s(x) is not given there by a straight line. */
L80:
    sqmax = (float)0.;
    sql = (float)0.;
    l = 5;
    nr = 0;
    i1 = 1;
    n4 = *n - 4;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	r__1 = w[i__] * (sx[i__] - y[i__]);
	term = r__1 * r__1;
	if (x[i__] < t[l] || l > n4) {
	    goto L100;
	}
	term *= half;
	sql += term;
	if (i__ - i1 <= 1 || bind[l - 4] && bind[l - 3]) {
	    goto L90;
	}
	if (sql <= sqmax) {
	    goto L90;
	}
	k = l;
	sqmax = sql;
	nr = i1 + (i__ - i1) / 2;
L90:
	++l;
	i1 = i__;
	sql = (float)0.;
L100:
	sql += term;
/* L110: */
    }
    if (*m - i1 <= 1 || bind[l - 4] && bind[l - 3]) {
	goto L120;
    }
    if (sql <= sqmax) {
	goto L120;
    }
    k = l;
    nr = i1 + (*m - i1) / 2;
/*  if no such interval is found, control is repassed to the driver */
/*  program (ier = -1). */
L120:
    if (nr == 0) {
	goto L190;
    }
/*  if s(x) is given by the same straight line in two succeeding knot */
/*  intervals t(l-1) <= x <= t(l) and t(l) <= x <= t(l+1),delete t(l) */
    n8 = *n - 8;
    l1 = 0;
    if (n8 <= 0) {
	goto L150;
    }
    i__1 = n8;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (! (bind[i__] && bind[i__ + 1] && bind[i__ + 2])) {
	    goto L140;
	}
	l = i__ + 4 - l1;
	if (k > l) {
	    --k;
	}
	--(*n);
	++l1;
	i__2 = *n;
	for (j = l; j <= i__2; ++j) {
	    t[j] = t[j + 1];
/* L130: */
	}
L140:
	;
    }
/*  test whether we cannot further increase the number of knots. */
L150:
    if (*n == nmax) {
	goto L180;
    }
    if (*n == *nest) {
	goto L170;
    }
/*  locate an additional knot at the point x(nr). */
    j = *n;
    i__1 = *n;
    for (i__ = k; i__ <= i__1; ++i__) {
	t[j + 1] = t[j];
	--j;
/* L160: */
    }
    t[k] = x[nr];
    ++(*n);
/*  restart the computations with the new set of knots. */
    goto L40;
/*  error codes and messages. */
L170:
    *ier = -3;
    goto L300;
L180:
    *ier = -2;
    goto L300;
L190:
    *ier = -1;
    goto L300;
L200:
    *ier = 4;
    goto L300;
L210:
    *ier = 5;
L300:
    return 0;
} /* fpcoco_ */

/* Subroutine */ int fpcons_(iopt, idim, m, u, mx, x, w, ib, ie, k, s, nest, 
	tol, maxit, k1, k2, n, t, nc, c__, fp, fpint, z__, a, b, g, q, nrdata,
	 ier)
integer *iopt, *idim, *m;
real *u;
integer *mx;
real *x, *w;
integer *ib, *ie, *k;
real *s;
integer *nest;
real *tol;
integer *maxit, *k1, *k2, *n;
real *t;
integer *nc;
real *c__, *fp, *fpint, *z__, *a, *b, *g, *q;
integer *nrdata, *ier;
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, g_dim1, g_offset, q_dim1, 
	    q_offset, i__1, i__2, i__3, i__4, i__5;
    real r__1;

    /* Local variables */
    static real half;
    static integer mmin, nmin, iter, nmax;
    static real fpms, term, pinv, h__[7];
    static integer i__, j, l;
    static real p, fpold, fpart, f1, f2, f3;
    static integer i1, i2;
    static real store;
    static integer i3, j1;
    static real p1, p2, p3;
    static integer j2, j3, l0, nplus, nrint, n8, jb, je, mb, me, jj, li, lj;
    extern /* Subroutine */ int fpback_();
    static integer mm, nn, it;
    static real ui, rn, wi, xi[10];
    extern /* Subroutine */ int fpdisc_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpbspl_(), fprota_(), fpgivs_(), fpknot_();
    static real fp0;
    static integer nk1;
    static real acc, fac;
    static integer kbe;
    static real one, cos__, sin__;
    static integer new__;
    static real piv;
    static integer ich1, ich3;
    static real con1, con4, con9;
    static integer npl1;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references */
/*  ..subroutine references.. */
/*    fpbacp,fpbspl,fpgivs,fpdisc,fpknot,fprota */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --w;
    --u;
    --x;
    --nrdata;
    --fpint;
    --t;
    q_dim1 = *m;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    a_dim1 = *nest;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    g_dim1 = *nest;
    g_offset = 1 + g_dim1 * 1;
    g -= g_offset;
    b_dim1 = *nest;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    --z__;
    --c__;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
    half = (float).5;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 1: determination of the number of knots and their position     c */
/*  **************************************************************      c */
/*  given a set of knots we compute the least-squares curve sinf(u),    c */
/*  and the corresponding sum of squared residuals fp=f(p=inf).         c */
/*  if iopt=-1 sinf(u) is the requested curve.                          c */
/*  if iopt=0 or iopt=1 we check whether we can accept the knots:       c */
/*    if fp <=s we will continue with the current set of knots.         c */
/*    if fp > s we will increase the number of knots and compute the    c */
/*       corresponding least-squares curve until finally fp<=s.         c */
/*    the initial choice of knots depends on the value of s and iopt.   c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*    knots equals nmax = m+k+1-max(0,ib-1)-max(0,ie-1)                 c */
/*    if s > 0 and                                                      c */
/*      iopt=0 we first compute the least-squares polynomial curve of   c */
/*      degree k; n = nmin = 2*k+2                                      c */
/*      iopt=1 we start with the set of knots found at the last         c */
/*      call of the routine, except for the case that s > fp0; then     c */
/*      we compute directly the polynomial curve of degree k.           c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  determine nmin, the number of knots for polynomial approximation. */
    nmin = *k1 << 1;
/*  find which data points are to be concidered. */
    mb = 2;
    jb = *ib;
    if (*ib > 0) {
	goto L10;
    }
    mb = 1;
    jb = 1;
L10:
    me = *m - 1;
    je = *ie;
    if (*ie > 0) {
	goto L20;
    }
    me = *m;
    je = 1;
L20:
    if (*iopt < 0) {
	goto L60;
    }
/*  calculation of acc, the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  determine nmax, the number of knots for spline interpolation. */
    kbe = *k1 - jb - je;
    mmin = kbe + 2;
    mm = *m - mmin;
    nmax = nmin + mm;
    if (*s > (float)0.) {
	goto L40;
    }
/*  if s=0, s(u) is an interpolating curve. */
/*  test whether the required storage space exceeds the available one. */
    *n = nmax;
    if (nmax > *nest) {
	goto L420;
    }
/*  find the position of the interior knots in case of interpolation. */
    if (mm == 0) {
	goto L60;
    }
L25:
    i__ = *k2;
    j = 3 - jb + *k / 2;
    i__1 = mm;
    for (l = 1; l <= i__1; ++l) {
	t[i__] = u[j];
	++i__;
	++j;
/* L30: */
    }
    goto L60;
/*  if s>0 our initial choice of knots depends on the value of iopt. */
/*  if iopt=0 or iopt=1 and s>=fp0, we start computing the least-squares */
/*  polynomial curve which is a spline curve without interior knots. */
/*  if iopt=1 and fp0>s we start computing the least squares spline curve */
/*  according to the set of knots found at the last call of the routine. */
L40:
    if (*iopt == 0) {
	goto L50;
    }
    if (*n == nmin) {
	goto L50;
    }
    fp0 = fpint[*n];
    fpold = fpint[*n - 1];
    nplus = nrdata[*n];
    if (fp0 > *s) {
	goto L60;
    }
L50:
    *n = nmin;
    fpold = (float)0.;
    nplus = 0;
    nrdata[1] = *m - 2;
/*  main loop for the different sets of knots. m is a save upper bound */
/*  for the number of trials. */
L60:
    i__1 = *m;
    for (iter = 1; iter <= i__1; ++iter) {
	if (*n == nmin) {
	    *ier = -2;
	}
/*  find nrint, tne number of knot intervals. */
	nrint = *n - nmin + 1;
/*  find the position of the additional knots which are needed for */
/*  the b-spline representation of s(u). */
	nk1 = *n - *k1;
	i__ = *n;
	i__2 = *k1;
	for (j = 1; j <= i__2; ++j) {
	    t[j] = u[1];
	    t[i__] = u[*m];
	    --i__;
/* L70: */
	}
/*  compute the b-spline coefficients of the least-squares spline curve */
/*  sinf(u). the observation matrix a is built up row by row and */
/*  reduced to upper triangular form by givens transformations. */
/*  at the same time fp=f(p=inf) is computed. */
	*fp = (float)0.;
/*  nn denotes the dimension of the splines */
	nn = nk1 - *ib - *ie;
/*  initialize the b-spline coefficients and the observation matrix a. */
	i__2 = *nc;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    z__[i__] = (float)0.;
	    c__[i__] = (float)0.;
/* L75: */
	}
	if (me < mb) {
	    goto L134;
	}
	if (nn == 0) {
	    goto L82;
	}
	i__2 = nn;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = *k1;
	    for (j = 1; j <= i__3; ++j) {
		a[i__ + j * a_dim1] = (float)0.;
/* L80: */
	    }
	}
L82:
	l = *k1;
	jj = (mb - 1) * *idim;
	i__3 = me;
	for (it = mb; it <= i__3; ++it) {
/*  fetch the current data point u(it),x(it). */
	    ui = u[it];
	    wi = w[it];
	    i__2 = *idim;
	    for (j = 1; j <= i__2; ++j) {
		++jj;
		xi[j - 1] = x[jj] * wi;
/* L84: */
	    }
/*  search for knot interval t(l) <= ui < t(l+1). */
L86:
	    if (ui < t[l + 1] || l == nk1) {
		goto L90;
	    }
	    ++l;
	    goto L86;
/*  evaluate the (k+1) non-zero b-splines at ui and store them in q. */
L90:
	    fpbspl_(&t[1], n, k, &ui, &l, h__);
	    i__2 = *k1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		q[it + i__ * q_dim1] = h__[i__ - 1];
		h__[i__ - 1] *= wi;
/* L92: */
	    }
/*  take into account that certain b-spline coefficients must be zero. */
	    lj = *k1;
	    j = nk1 - l - *ie;
	    if (j >= 0) {
		goto L94;
	    }
	    lj += j;
L94:
	    li = 1;
	    j = l - *k1 - *ib;
	    if (j >= 0) {
		goto L96;
	    }
	    li -= j;
	    j = 0;
L96:
	    if (li > lj) {
		goto L120;
	    }
/*  rotate the new row of the observation matrix into triangle. */
	    i__2 = lj;
	    for (i__ = li; i__ <= i__2; ++i__) {
		++j;
		piv = h__[i__ - 1];
		if (piv == (float)0.) {
		    goto L110;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a[j + a_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		j1 = j;
		i__4 = *idim;
		for (j2 = 1; j2 <= i__4; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &z__[j1]);
		    j1 += *n;
/* L98: */
		}
		if (i__ == lj) {
		    goto L120;
		}
		i2 = 1;
		i3 = i__ + 1;
		i__4 = lj;
		for (i1 = i3; i1 <= i__4; ++i1) {
		    ++i2;
/*  transformations to left hand side. */
		    fprota_(&cos__, &sin__, &h__[i1 - 1], &a[j + i2 * a_dim1])
			    ;
/* L100: */
		}
L110:
		;
	    }
/*  add contribution of this row to the sum of squares of residual */
/*  right hand sides. */
L120:
	    i__2 = *idim;
	    for (j2 = 1; j2 <= i__2; ++j2) {
/* Computing 2nd power */
		r__1 = xi[j2 - 1];
		*fp += r__1 * r__1;
/* L125: */
	    }
/* L130: */
	}
	if (*ier == -2) {
	    fp0 = *fp;
	}
	fpint[*n] = fp0;
	fpint[*n - 1] = fpold;
	nrdata[*n] = nplus;
/*  backward substitution to obtain the b-spline coefficients. */
	if (nn == 0) {
	    goto L134;
	}
	j1 = 1;
	i__3 = *idim;
	for (j2 = 1; j2 <= i__3; ++j2) {
	    j3 = j1 + *ib;
	    fpback_(&a[a_offset], &z__[j1], &nn, k1, &c__[j3], nest);
	    j1 += *n;
/* L132: */
	}
/*  test whether the approximation sinf(u) is an acceptable solution. */
L134:
	if (*iopt < 0) {
	    goto L440;
	}
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  if f(p=inf) < s accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L250;
	}
/*  if n = nmax, sinf(u) is an interpolating spline curve. */
	if (*n == nmax) {
	    goto L430;
	}
/*  increase the number of knots. */
/*  if n=nest we cannot increase the number of knots because of */
/*  the storage capacity limitation. */
	if (*n == *nest) {
	    goto L420;
	}
/*  determine the number of knots nplus we are going to add. */
	if (*ier == 0) {
	    goto L140;
	}
	nplus = 1;
	*ier = 0;
	goto L150;
L140:
	npl1 = nplus << 1;
	rn = (real) nplus;
	if (fpold - *fp > acc) {
	    npl1 = rn * fpms / (fpold - *fp);
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = nplus / 2, i__4 = max(i__4,i__5);
	i__3 = nplus << 1, i__2 = max(i__4,1);
	nplus = min(i__3,i__2);
L150:
	fpold = *fp;
/*  compute the sum of squared residuals for each knot interval */
/*  t(j+k) <= u(i) <= t(j+k+1) and store it in fpint(j),j=1,2,...nrint. */
	fpart = (float)0.;
	i__ = 1;
	l = *k2;
	new__ = 0;
	jj = (mb - 1) * *idim;
	i__3 = me;
	for (it = mb; it <= i__3; ++it) {
	    if (u[it] < t[l] || l > nk1) {
		goto L160;
	    }
	    new__ = 1;
	    ++l;
L160:
	    term = (float)0.;
	    l0 = l - *k2;
	    i__2 = *idim;
	    for (j2 = 1; j2 <= i__2; ++j2) {
		fac = (float)0.;
		j1 = l0;
		i__4 = *k1;
		for (j = 1; j <= i__4; ++j) {
		    ++j1;
		    fac += c__[j1] * q[it + j * q_dim1];
/* L170: */
		}
		++jj;
/* Computing 2nd power */
		r__1 = w[it] * (fac - x[jj]);
		term += r__1 * r__1;
		l0 += *n;
/* L175: */
	    }
	    fpart += term;
	    if (new__ == 0) {
		goto L180;
	    }
	    store = term * half;
	    fpint[i__] = fpart - store;
	    ++i__;
	    fpart = store;
	    new__ = 0;
L180:
	    ;
	}
	fpint[nrint] = fpart;
	i__3 = nplus;
	for (l = 1; l <= i__3; ++l) {
/*  add a new knot. */
	    fpknot_(&u[1], m, &t[1], n, &fpint[1], &nrdata[1], &nrint, nest, &
		    c__1);
/*  if n=nmax we locate the knots as for interpolation */
	    if (*n == nmax) {
		goto L25;
	    }
/*  test whether we cannot further increase the number of knots. */
	    if (*n == *nest) {
		goto L200;
	    }
/* L190: */
	}
/*  restart the computations with the new set of knots. */
L200:
	;
    }
/*  test whether the least-squares kth degree polynomial curve is a */
/*  solution of our approximation problem. */
L250:
    if (*ier == -2) {
	goto L440;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 2: determination of the smoothing spline curve sp(u).          c */
/*  **********************************************************          c */
/*  we have determined the number of knots and their position.          c */
/*  we now compute the b-spline coefficients of the smoothing curve     c */
/*  sp(u). the observation matrix a is extended by the rows of matrix   c */
/*  b expressing that the kth derivative discontinuities of sp(u) at    c */
/*  the interior knots t(k+2),...t(n-k-1) must be zero. the corres-     c */
/*  ponding weights of these additional rows are set to 1/p.            c */
/*  iteratively we then have to determine the value of p such that f(p),c */
/*  the sum of squared residuals be = s. we already know that the least c */
/*  squares kth degree polynomial curve corresponds to p=0, and that    c */
/*  the least-squares spline curve corresponds to p=infinity. the       c */
/*  iteration process which is proposed here, makes use of rational     c */
/*  interpolation. since f(p) is a convex and strictly decreasing       c */
/*  function of p, it can be approximated by a rational function        c */
/*  r(p) = (u*p+v)/(p+w). three values of p(p1,p2,p3) with correspond-  c */
/*  ing values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s) are used      c */
/*  to calculate the new value of p such that r(p)=s. convergence is    c */
/*  guaranteed by taking f1>0 and f3<0.                                 c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  evaluate the discontinuity jump of the kth derivative of the */
/*  b-splines at the knots t(l),l=k+2,...n-k-1 and store in b. */
    fpdisc_(&t[1], n, k2, &b[b_offset], nest);
/*  initial value for p. */
    p1 = (float)0.;
    f1 = fp0 - *s;
    p3 = -one;
    f3 = fpms;
    p = (float)0.;
    i__1 = nn;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p += a[i__ + a_dim1];
/* L252: */
    }
    rn = (real) nn;
    p = rn / p;
    ich1 = 0;
    ich3 = 0;
    n8 = *n - nmin;
/*  iteration process to find the root of f(p) = s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  the rows of matrix b with weight 1/p are rotated into the */
/*  triangularised observation matrix a which is stored in g. */
	pinv = one / p;
	i__3 = *nc;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    c__[i__] = z__[i__];
/* L255: */
	}
	i__3 = nn;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    g[i__ + *k2 * g_dim1] = (float)0.;
	    i__2 = *k1;
	    for (j = 1; j <= i__2; ++j) {
		g[i__ + j * g_dim1] = a[i__ + j * a_dim1];
/* L260: */
	    }
	}
	i__2 = n8;
	for (it = 1; it <= i__2; ++it) {
/*  the row of matrix b is rotated into triangle by givens transformation */
	    i__3 = *k2;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h__[i__ - 1] = b[it + i__ * b_dim1] * pinv;
/* L264: */
	    }
	    i__3 = *idim;
	    for (j = 1; j <= i__3; ++j) {
		xi[j - 1] = (float)0.;
/* L268: */
	    }
/*  take into account that certain b-spline coefficients must be zero. */
	    if (it > *ib) {
		goto L274;
	    }
	    j1 = *ib - it + 2;
	    j2 = 1;
	    i__3 = *k2;
	    for (i__ = j1; i__ <= i__3; ++i__) {
		h__[j2 - 1] = h__[i__ - 1];
		++j2;
/* L270: */
	    }
	    i__3 = *k2;
	    for (i__ = j2; i__ <= i__3; ++i__) {
		h__[i__ - 1] = (float)0.;
/* L272: */
	    }
L274:
/* Computing MAX */
	    i__3 = 1, i__4 = it - *ib;
	    jj = max(i__3,i__4);
	    i__3 = nn;
	    for (j = jj; j <= i__3; ++j) {
		piv = h__[0];
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &g[j + g_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		j1 = j;
		i__4 = *idim;
		for (j2 = 1; j2 <= i__4; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &c__[j1]);
		    j1 += *n;
/* L277: */
		}
		if (j == nn) {
		    goto L300;
		}
/* Computing MIN */
		i__4 = nn - j;
		i2 = min(i__4,*k1);
		i__4 = i2;
		for (i__ = 1; i__ <= i__4; ++i__) {
/*  transformations to left hand side. */
		    i1 = i__ + 1;
		    fprota_(&cos__, &sin__, &h__[i1 - 1], &g[j + i1 * g_dim1])
			    ;
		    h__[i__ - 1] = h__[i1 - 1];
/* L280: */
		}
		h__[i2] = (float)0.;
/* L290: */
	    }
L300:
	    ;
	}
/*  backward substitution to obtain the b-spline coefficients. */
	j1 = 1;
	i__2 = *idim;
	for (j2 = 1; j2 <= i__2; ++j2) {
	    j3 = j1 + *ib;
	    fpback_(&g[g_offset], &c__[j1], &nn, k2, &c__[j3], nest);
	    if (*ib == 0) {
		goto L306;
	    }
	    j3 = j1;
	    i__3 = *ib;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		c__[j3] = (float)0.;
		++j3;
/* L304: */
	    }
L306:
	    j1 += *n;
/* L308: */
	}
/*  computation of f(p). */
	*fp = (float)0.;
	l = *k2;
	jj = (mb - 1) * *idim;
	i__2 = me;
	for (it = mb; it <= i__2; ++it) {
	    if (u[it] < t[l] || l > nk1) {
		goto L310;
	    }
	    ++l;
L310:
	    l0 = l - *k2;
	    term = (float)0.;
	    i__3 = *idim;
	    for (j2 = 1; j2 <= i__3; ++j2) {
		fac = (float)0.;
		j1 = l0;
		i__4 = *k1;
		for (j = 1; j <= i__4; ++j) {
		    ++j1;
		    fac += c__[j1] * q[it + j * q_dim1];
/* L320: */
		}
		++jj;
/* Computing 2nd power */
		r__1 = fac - x[jj];
		term += r__1 * r__1;
		l0 += *n;
/* L325: */
	    }
/* Computing 2nd power */
	    r__1 = w[it];
	    *fp += term * (r__1 * r__1);
/* L330: */
	}
/*  test whether the approximation sp(u) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  test whether the maximal number of iterations is reached. */
	if (iter == *maxit) {
	    goto L400;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L340;
	}
	if (f2 - f3 > acc) {
	    goto L335;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L360;
L335:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L340:
	if (ich1 != 0) {
	    goto L350;
	}
	if (f1 - f2 > acc) {
	    goto L345;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L360;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L360;
L345:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L350:
	if (f2 >= f1 || f2 <= f3) {
	    goto L410;
	}
/*  find the new value for p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L360:
	;
    }
/*  error codes and messages. */
L400:
    *ier = 3;
    goto L440;
L410:
    *ier = 2;
    goto L440;
L420:
    *ier = 1;
    goto L440;
L430:
    *ier = -1;
L440:
    return 0;
} /* fpcons_ */

/* Subroutine */ int fpcosp_(m, x, y, w, n, t, e, maxtr, maxbin, c__, sq, sx, 
	bind, nm, mb, a, b, const__, z__, zz, u, q, info, up, left, right, 
	jbind, ibind, ier)
integer *m;
real *x, *y, *w;
integer *n;
real *t, *e;
integer *maxtr, *maxbin;
real *c__, *sq, *sx;
logical *bind;
integer *nm, *mb;
real *a, *b, *const__, *z__, *zz, *u, *q;
integer *info, *up, *left, *right, *jbind, *ibind, *ier;
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, q_dim1, q_offset, i__1, i__2, 
	    i__3;
    real r__1;

    /* Local variables */
    static integer kdim, merk;
    static real f, h__[4];
    static integer i__, j, k, l, nbind, count, i1, j1, j2, j3, k1, k2, k3, k4,
	     k5, k6, l1, l2, l3, n1, n4, n6;
    static real wi, xi;
    extern /* Subroutine */ int fpadno_(), fpdeno_(), fpbspl_();
    static integer number;
    extern /* Subroutine */ int fpseno_();
    static integer lp1;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  ..subroutine references.. */
/*    fpbspl,fpadno,fpdeno,fpfrno,fpseno */
/*  .. */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  if we use the b-spline representation of s(x) our approximation     c */
/*  problem results in a quadratic programming problem:                 c */
/*    find the b-spline coefficients c(j),j=1,2,...n-4 such that        c */
/*        (1) sumi((wi*(yi-sumj(cj*nj(xi))))**2),i=1,2,...m is minimal  c */
/*        (2) sumj(cj*n''j(t(l+3)))*e(l) <= 0, l=1,2,...n-6.            c */
/*  to solve this problem we use the theil-van de panne procedure.      c */
/*  if the inequality constraints (2) are numbered from 1 to n-6,       c */
/*  this algorithm finds a subset of constraints ibind(1)..ibind(nbind) c */
/*  such that the solution of the minimization problem (1) with these   c */
/*  constraints in equality form, satisfies all constraints. such a     c */
/*  feasible solution is optimal if the lagrange parameters associated  c */
/*  with that problem with equality constraints, are all positive.      c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  determine n6, the number of inequality constraints. */
    /* Parameter adjustments */
    q_dim1 = *m;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    --sx;
    --w;
    --y;
    --x;
    --zz;
    --z__;
    --const__;
    a_dim1 = *n;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --bind;
    --c__;
    --e;
    --t;
    --right;
    --left;
    --up;
    --info;
    --u;
    b_dim1 = *nm;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    --ibind;
    --jbind;

    /* Function Body */
    n6 = *n - 6;
/*  fix the parameters which determine these constraints. */
    i__1 = n6;
    for (i__ = 1; i__ <= i__1; ++i__) {
	const__[i__] = e[i__] * (t[i__ + 4] - t[i__ + 1]) / (t[i__ + 5] - t[
		i__ + 2]);
/* L10: */
    }
/*  initialize the triply linked tree which is used to find the subset */
/*  of constraints ibind(1),...ibind(nbind). */
    count = 1;
    info[1] = 0;
    left[1] = 0;
    right[1] = 0;
    up[1] = 1;
    merk = 1;
/*  set up the normal equations  n'nc=n'y  where n denotes the m x (n-4) */
/*  observation matrix with elements ni,j = wi*nj(xi)  and y is the */
/*  column vector with elements yi*wi. */
/*  from the properties of the b-splines nj(x),j=1,2,...n-4, it follows */
/*  that  n'n  is a (n-4) x (n-4)  positive definit bandmatrix of */
/*  bandwidth 7. the matrices n'n and n'y are built up in a and z. */
    n4 = *n - 4;
/*  initialization */
    i__1 = n4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	z__[i__] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    a[i__ + j * a_dim1] = (float)0.;
/* L20: */
	}
    }
    l = 4;
    lp1 = l + 1;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*  fetch the current row of the observation matrix. */
	xi = x[i__];
/* Computing 2nd power */
	r__1 = w[i__];
	wi = r__1 * r__1;
/*  search for knot interval  t(l) <= xi < t(l+1) */
L30:
	if (xi < t[lp1] || l == n4) {
	    goto L40;
	}
	l = lp1;
	lp1 = l + 1;
	goto L30;
/*  evaluate the four non-zero cubic b-splines nj(xi),j=l-3,...l. */
L40:
	fpbspl_(&t[1], n, &c__3, &xi, &l, h__);
/*  store in q these values h(1),h(2),...h(4). */
	for (j = 1; j <= 4; ++j) {
	    q[i__ + j * q_dim1] = h__[j - 1];
/* L50: */
	}
/*  add the contribution of the current row of the observation matrix */
/*  n to the normal equations. */
	l3 = l - 3;
	k1 = 0;
	i__2 = l;
	for (j1 = l3; j1 <= i__2; ++j1) {
	    ++k1;
	    f = h__[k1 - 1];
	    z__[j1] += f * wi * y[i__];
	    k2 = k1;
	    j2 = 4;
	    i__3 = l;
	    for (j3 = j1; j3 <= i__3; ++j3) {
		a[j3 + j2 * a_dim1] += f * wi * h__[k2 - 1];
		++k2;
		--j2;
/* L60: */
	    }
	}
/* L70: */
    }
/*  since n'n is a symmetric matrix it can be factorized as */
/*        (3)  n'n = (r1)'(d1)(r1) */
/*  with d1 a diagonal matrix and r1 an (n-4) x (n-4)  unit upper */
/*  triangular matrix of bandwidth 4. the matrices r1 and d1 are built */
/*  up in a. at the same time we solve the systems of equations */
/*        (4)  (r1)'(z2) = n'y */
/*        (5)  (d1) (z1) = (z2) */
/*  the vectors z2 and z1 are kept in zz and z. */
    i__1 = n4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k1 = 1;
	if (i__ < 4) {
	    k1 = 5 - i__;
	}
	k2 = i__ - 4 + k1;
	k3 = k2;
	for (j = k1; j <= 4; ++j) {
	    k4 = j - 1;
	    k5 = 4 - j + k1;
	    f = a[i__ + j * a_dim1];
	    if (k1 > k4) {
		goto L90;
	    }
	    k6 = k2;
	    i__3 = k4;
	    for (k = k1; k <= i__3; ++k) {
		f -= a[i__ + k * a_dim1] * a[k3 + k5 * a_dim1] * a[k6 + (
			a_dim1 << 2)];
		++k5;
		++k6;
/* L80: */
	    }
L90:
	    if (j == 4) {
		goto L110;
	    }
	    a[i__ + j * a_dim1] = f / a[k3 + (a_dim1 << 2)];
	    ++k3;
/* L100: */
	}
L110:
	a[i__ + (a_dim1 << 2)] = f;
	f = z__[i__];
	if (i__ == 1) {
	    goto L130;
	}
	k4 = i__;
	for (j = k1; j <= 3; ++j) {
	    k = k1 + 3 - j;
	    --k4;
	    f -= a[i__ + k * a_dim1] * z__[k4] * a[k4 + (a_dim1 << 2)];
/* L120: */
	}
L130:
	z__[i__] = f / a[i__ + (a_dim1 << 2)];
	zz[i__] = f;
/* L140: */
    }
/*  start computing the least-squares cubic spline without taking account */
/*  of any constraint. */
    nbind = 0;
    n1 = 1;
    ibind[1] = 0;
/*  main loop for the least-squares problems with different subsets of */
/*  the constraints (2) in equality form. the resulting b-spline coeff. */
/*  c and lagrange parameters u are the solution of the system */
/*            ! n'n  b' ! ! c !   ! n'y ! */
/*        (6) !         ! !   ! = !     ! */
/*            !  b   0  ! ! u !   !  0  ! */
/*  z1 is stored into array c. */
L150:
    i__1 = n4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = z__[i__];
/* L160: */
    }
/*  if there are no equality constraints, compute the coeff. c directly. */
    if (nbind == 0) {
	goto L370;
    }
/*  initialization */
    kdim = n4 + nbind;
    i__1 = nbind;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__3 = kdim;
	for (j = 1; j <= i__3; ++j) {
	    b[j + i__ * b_dim1] = (float)0.;
/* L170: */
	}
    }
/*  matrix b is built up,expressing that the constraints nrs ibind(1),... */
/*  ibind(nbind) must be satisfied in equality form. */
    i__3 = nbind;
    for (i__ = 1; i__ <= i__3; ++i__) {
	l = ibind[i__];
	b[l + i__ * b_dim1] = e[l];
	b[l + 1 + i__ * b_dim1] = -(e[l] + const__[l]);
	b[l + 2 + i__ * b_dim1] = const__[l];
/* L180: */
    }
/*  find the matrix (b1) as the solution of the system of equations */
/*        (7)  (r1)'(d1)(b1) = b' */
/*  (b1) is built up in the upper part of the array b(rows 1,...n-4). */
    i__3 = nbind;
    for (k1 = 1; k1 <= i__3; ++k1) {
	l = ibind[k1];
	i__1 = n4;
	for (i__ = l; i__ <= i__1; ++i__) {
	    f = b[i__ + k1 * b_dim1];
	    if (i__ == 1) {
		goto L200;
	    }
	    k2 = 3;
	    if (i__ < 4) {
		k2 = i__ - 1;
	    }
	    i__2 = k2;
	    for (k3 = 1; k3 <= i__2; ++k3) {
		l1 = i__ - k3;
		l2 = 4 - k3;
		f -= b[l1 + k1 * b_dim1] * a[i__ + l2 * a_dim1] * a[l1 + (
			a_dim1 << 2)];
/* L190: */
	    }
L200:
	    b[i__ + k1 * b_dim1] = f / a[i__ + (a_dim1 << 2)];
/* L210: */
	}
/* L220: */
    }
/*  factorization of the symmetric matrix  -(b1)'(d1)(b1) */
/*        (8)  -(b1)'(d1)(b1) = (r2)'(d2)(r2) */
/*  with (d2) a diagonal matrix and (r2) an nbind x nbind unit upper */
/*  triangular matrix. the matrices r2 and d2 are built up in the lower */
/*  part of the array b (rows n-3,n-2,...n-4+nbind). */
    i__3 = nbind;
    for (i__ = 1; i__ <= i__3; ++i__) {
	i1 = i__ - 1;
	i__1 = nbind;
	for (j = i__; j <= i__1; ++j) {
	    f = (float)0.;
	    i__2 = n4;
	    for (k = 1; k <= i__2; ++k) {
		f += b[k + i__ * b_dim1] * b[k + j * b_dim1] * a[k + (a_dim1 
			<< 2)];
/* L230: */
	    }
	    k1 = n4 + 1;
	    if (i1 == 0) {
		goto L250;
	    }
	    i__2 = i1;
	    for (k = 1; k <= i__2; ++k) {
		f += b[k1 + i__ * b_dim1] * b[k1 + j * b_dim1] * b[k1 + k * 
			b_dim1];
		++k1;
/* L240: */
	    }
L250:
	    b[k1 + j * b_dim1] = -f;
	    if (j == i__) {
		goto L260;
	    }
	    b[k1 + j * b_dim1] /= b[k1 + i__ * b_dim1];
L260:
	    ;
	}
/* L270: */
    }
/*  according to (3),(7) and (8) the system of equations (6) becomes */
/*         ! (r1)'    0  ! ! (d1)    0  ! ! (r1)  (b1) ! ! c !   ! n'y ! */
/*    (9)  !             ! !            ! !            ! !   ! = !     ! */
/*         ! (b1)'  (r2)'! !   0   (d2) ! !   0   (r2) ! ! u !   !  0  ! */
/*  backward substitution to obtain the b-spline coefficients c(j),j=1,.. */
/*  n-4 and the lagrange parameters u(j),j=1,2,...nbind. */
/*  first step of the backward substitution: solve the system */
/*             ! (r1)'(d1)      0     ! ! (c1) !   ! n'y ! */
/*        (10) !                      ! !      ! = !     ! */
/*             ! (b1)'(d1)  (r2)'(d2) ! ! (u1) !   !  0  ! */
/*  from (4) and (5) we know that this is equivalent to */
/*        (11)  (c1) = (z1) */
/*        (12)  (r2)'(d2)(u1) = -(b1)'(z2) */
    i__3 = nbind;
    for (i__ = 1; i__ <= i__3; ++i__) {
	f = (float)0.;
	i__1 = n4;
	for (j = 1; j <= i__1; ++j) {
	    f += b[j + i__ * b_dim1] * zz[j];
/* L280: */
	}
	i1 = i__ - 1;
	k1 = n4 + 1;
	if (i1 == 0) {
	    goto L300;
	}
	i__1 = i1;
	for (j = 1; j <= i__1; ++j) {
	    f += u[j] * b[k1 + i__ * b_dim1] * b[k1 + j * b_dim1];
	    ++k1;
/* L290: */
	}
L300:
	u[i__] = -f / b[k1 + i__ * b_dim1];
/* L310: */
    }
/*  second step of the backward substitution: solve the system */
/*             ! (r1)  (b1) ! ! c !   ! c1 ! */
/*        (13) !            ! !   ! = !    ! */
/*             !   0   (r2) ! ! u !   ! u1 ! */
    k1 = nbind;
    k2 = kdim;
/*  find the lagrange parameters u. */
    i__3 = nbind;
    for (i__ = 1; i__ <= i__3; ++i__) {
	f = u[k1];
	if (i__ == 1) {
	    goto L330;
	}
	k3 = k1 + 1;
	i__1 = nbind;
	for (j = k3; j <= i__1; ++j) {
	    f -= u[j] * b[k2 + j * b_dim1];
/* L320: */
	}
L330:
	u[k1] = f;
	--k1;
	--k2;
/* L340: */
    }
/*  find the b-spline coefficients c. */
    i__3 = n4;
    for (i__ = 1; i__ <= i__3; ++i__) {
	f = c__[i__];
	i__1 = nbind;
	for (j = 1; j <= i__1; ++j) {
	    f -= u[j] * b[i__ + j * b_dim1];
/* L350: */
	}
	c__[i__] = f;
/* L360: */
    }
L370:
    k1 = n4;
    i__3 = n4;
    for (i__ = 2; i__ <= i__3; ++i__) {
	--k1;
	f = c__[k1];
	k2 = 1;
	if (i__ < 5) {
	    k2 = 5 - i__;
	}
	k3 = k1;
	l = 3;
	for (j = k2; j <= 3; ++j) {
	    ++k3;
	    f -= a[k3 + l * a_dim1] * c__[k3];
	    --l;
/* L380: */
	}
	c__[k1] = f;
/* L390: */
    }
/*  test whether the solution of the least-squares problem with the */
/*  constraints ibind(1),...ibind(nbind) in equality form, satisfies */
/*  all of the constraints (2). */
    k = 1;
/*  number counts the number of violated inequality constraints. */
    number = 0;
    i__3 = n6;
    for (j = 1; j <= i__3; ++j) {
	l = ibind[k];
	++k;
	if (j == l) {
	    goto L440;
	}
	--k;
/*  test whether constraint j is satisfied */
	f = e[j] * (c__[j] - c__[j + 1]) + const__[j] * (c__[j + 2] - c__[j + 
		1]);
	if (f <= (float)0.) {
	    goto L440;
	}
/*  if constraint j is not satisfied, add a branch of length nbind+1 */
/*  to the tree. the nodes of this branch contain in their information */
/*  field the number of the constraints ibind(1),...ibind(nbind) and j, */
/*  arranged in increasing order. */
	++number;
	k1 = k - 1;
	if (k1 == 0) {
	    goto L410;
	}
	i__1 = k1;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    jbind[i__] = ibind[i__];
/* L400: */
	}
L410:
	jbind[k] = j;
	if (l == 0) {
	    goto L430;
	}
	i__1 = nbind;
	for (i__ = k; i__ <= i__1; ++i__) {
	    jbind[i__ + 1] = ibind[i__];
/* L420: */
	}
L430:
	fpadno_(maxtr, &up[1], &left[1], &right[1], &info[1], &count, &merk, &
		jbind[1], &n1, ier);
/*  test whether the storage space which is required for the tree,exceeds */
/*  the available storage space. */
	if (*ier != 0) {
	    goto L560;
	}
L440:
	;
    }
/*  test whether the solution of the least-squares problem with equality */
/*  constraints is a feasible solution. */
    if (number == 0) {
	goto L470;
    }
/*  test whether there are still cases with nbind constraints in */
/*  equality form to be considered. */
L450:
    if (merk > 1) {
	goto L460;
    }
    nbind = n1;
/*  test whether the number of knots where s''(x)=0 exceeds maxbin. */
    if (nbind > *maxbin) {
	goto L550;
    }
    ++n1;
    ibind[n1] = 0;
/*  search which cases with nbind constraints in equality form */
/*  are going to be considered. */
    fpdeno_(maxtr, &up[1], &left[1], &right[1], &nbind, &merk);
/*  test whether the quadratic programming problem has a solution. */
    if (merk == 1) {
	goto L570;
    }
/*  find a new case with nbind constraints in equality form. */
L460:
    fpseno_(maxtr, &up[1], &left[1], &right[1], &info[1], &merk, &ibind[1], &
	    nbind);
    goto L150;
/*  test whether the feasible solution is optimal. */
L470:
    *ier = 0;
    i__3 = n6;
    for (i__ = 1; i__ <= i__3; ++i__) {
	bind[i__] = FALSE_;
/* L480: */
    }
    if (nbind == 0) {
	goto L500;
    }
    i__3 = nbind;
    for (i__ = 1; i__ <= i__3; ++i__) {
	if (u[i__] <= (float)0.) {
	    goto L450;
	}
	j = ibind[i__];
	bind[j] = TRUE_;
/* L490: */
    }
/*  evaluate s(x) at the data points x(i) and calculate the weighted */
/*  sum of squared residual right hand sides sq. */
L500:
    *sq = (float)0.;
    l = 4;
    lp1 = 5;
    i__3 = *m;
    for (i__ = 1; i__ <= i__3; ++i__) {
L510:
	if (x[i__] < t[lp1] || l == n4) {
	    goto L520;
	}
	l = lp1;
	lp1 = l + 1;
	goto L510;
L520:
	sx[i__] = c__[l - 3] * q[i__ + q_dim1] + c__[l - 2] * q[i__ + (q_dim1 
		<< 1)] + c__[l - 1] * q[i__ + q_dim1 * 3] + c__[l] * q[i__ + (
		q_dim1 << 2)];
/* Computing 2nd power */
	r__1 = w[i__] * (y[i__] - sx[i__]);
	*sq += r__1 * r__1;
/* L530: */
    }
    goto L600;
/*  error codes and messages. */
L550:
    *ier = 1;
    goto L600;
L560:
    *ier = 2;
    goto L600;
L570:
    *ier = 3;
L600:
    return 0;
} /* fpcosp_ */

/* Subroutine */ int fpcsin_(a, b, par, sia, coa, sib, cob, ress, resc)
real *a, *b, *par, *sia, *coa, *sib, *cob, *ress, *resc;
{
    /* System generated locals */
    real r__1;

    /* Local variables */
    static real alfa, beta;
    static integer i__, j;
    static real three, b2, b4, f1, f2, quart, ab, ai, ab4, fac, one, eps, six,
	     two;

/*  fpcsin calculates the integrals ress=integral((b-x)**3*sin(par*x)) */
/*  and resc=integral((b-x)**3*cos(par*x)) over the interval (a,b), */
/*  given sia=sin(par*a),coa=cos(par*a),sib=sin(par*b) and cob=cos(par*b) */
/*  .. */
/*  ..scalar arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  .. */
    one = (float)1.;
    two = (float)2.;
    three = (float)3.;
    six = (float)6.;
    quart = (float).25;
    eps = (float)1e-10;
    ab = *b - *a;
/* Computing 4th power */
    r__1 = ab, r__1 *= r__1;
    ab4 = r__1 * r__1;
    alfa = ab * *par;
/* the way of calculating the integrals ress and resc depends on */
/* the value of alfa = (b-a)*par. */
    if (dabs(alfa) <= one) {
	goto L100;
    }
/* integration by parts. */
    beta = one / alfa;
/* Computing 2nd power */
    r__1 = beta;
    b2 = r__1 * r__1;
/* Computing 2nd power */
    r__1 = b2;
    b4 = six * (r__1 * r__1);
    f1 = three * b2 * (one - two * b2);
    f2 = beta * (one - six * b2);
    *ress = ab4 * (*coa * f2 + *sia * f1 + *sib * b4);
    *resc = ab4 * (*coa * f1 - *sia * f2 + *cob * b4);
    goto L400;
/* ress and resc are found by evaluating a series expansion. */
L100:
    fac = quart;
    f1 = fac;
    f2 = (float)0.;
    i__ = 4;
    for (j = 1; j <= 5; ++j) {
	++i__;
	ai = (real) i__;
	fac = fac * alfa / ai;
	f2 += fac;
	if (dabs(fac) <= eps) {
	    goto L300;
	}
	++i__;
	ai = (real) i__;
	fac = -fac * alfa / ai;
	f1 += fac;
	if (dabs(fac) <= eps) {
	    goto L300;
	}
/* L200: */
    }
L300:
    *ress = ab4 * (*coa * f2 + *sia * f1);
    *resc = ab4 * (*coa * f1 - *sia * f2);
L400:
    return 0;
} /* fpcsin_ */

/* Subroutine */ int fpcurf_(iopt, x, y, w, m, xb, xe, k, s, nest, tol, maxit,
	 k1, k2, n, t, c__, fp, fpint, z__, a, b, g, q, nrdata, ier)
integer *iopt;
real *x, *y, *w;
integer *m;
real *xb, *xe;
integer *k;
real *s;
integer *nest;
real *tol;
integer *maxit, *k1, *k2, *n;
real *t, *c__, *fp, *fpint, *z__, *a, *b, *g, *q;
integer *nrdata, *ier;
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, g_dim1, g_offset, q_dim1, 
	    q_offset, i__1, i__2, i__3, i__4, i__5;
    real r__1;

    /* Local variables */
    static real half;
    static integer nmin, iter, nmax;
    static real fpms, term, pinv, h__[7];
    static integer i__, j, l;
    static real p, fpold, fpart, f1, f2, f3;
    static integer i1, i2;
    static real store;
    static integer i3, k3;
    static real p1, p2, p3;
    static integer l0, nplus, nrint, n8;
    extern /* Subroutine */ int fpback_();
    static integer it;
    static real rn, wi, xi, yi;
    extern /* Subroutine */ int fpdisc_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpbspl_(), fprota_(), fpgivs_(), fpknot_();
    static real fp0;
    static integer mk1, nk1;
    static real acc, one, cos__, sin__;
    static integer new__;
    static real piv;
    static integer ich1, ich3;
    static real con1, con4, con9;
    static integer npl1;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references */
/*  ..subroutine references.. */
/*    fpback,fpbspl,fpgivs,fpdisc,fpknot,fprota */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --w;
    --y;
    --x;
    --nrdata;
    --z__;
    --fpint;
    --c__;
    --t;
    q_dim1 = *m;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    a_dim1 = *nest;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    g_dim1 = *nest;
    g_offset = 1 + g_dim1 * 1;
    g -= g_offset;
    b_dim1 = *nest;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
    half = (float).5;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 1: determination of the number of knots and their position     c */
/*  **************************************************************      c */
/*  given a set of knots we compute the least-squares spline sinf(x),   c */
/*  and the corresponding sum of squared residuals fp=f(p=inf).         c */
/*  if iopt=-1 sinf(x) is the requested approximation.                  c */
/*  if iopt=0 or iopt=1 we check whether we can accept the knots:       c */
/*    if fp <=s we will continue with the current set of knots.         c */
/*    if fp > s we will increase the number of knots and compute the    c */
/*       corresponding least-squares spline until finally fp<=s.        c */
/*    the initial choice of knots depends on the value of s and iopt.   c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*    knots equals nmax = m+k+1.                                        c */
/*    if s > 0 and                                                      c */
/*      iopt=0 we first compute the least-squares polynomial of         c */
/*      degree k; n = nmin = 2*k+2                                      c */
/*      iopt=1 we start with the set of knots found at the last         c */
/*      call of the routine, except for the case that s > fp0; then     c */
/*      we compute directly the least-squares polynomial of degree k.   c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  determine nmin, the number of knots for polynomial approximation. */
    nmin = *k1 << 1;
    if (*iopt < 0) {
	goto L60;
    }
/*  calculation of acc, the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  determine nmax, the number of knots for spline interpolation. */
    nmax = *m + *k1;
    if (*s > (float)0.) {
	goto L45;
    }
/*  if s=0, s(x) is an interpolating spline. */
/*  test whether the required storage space exceeds the available one. */
    *n = nmax;
    if (nmax > *nest) {
	goto L420;
    }
/*  find the position of the interior knots in case of interpolation. */
L10:
    mk1 = *m - *k1;
    if (mk1 == 0) {
	goto L60;
    }
    k3 = *k / 2;
    i__ = *k2;
    j = k3 + 2;
    if (k3 << 1 == *k) {
	goto L30;
    }
    i__1 = mk1;
    for (l = 1; l <= i__1; ++l) {
	t[i__] = x[j];
	++i__;
	++j;
/* L20: */
    }
    goto L60;
L30:
    i__1 = mk1;
    for (l = 1; l <= i__1; ++l) {
	t[i__] = (x[j] + x[j - 1]) * half;
	++i__;
	++j;
/* L40: */
    }
    goto L60;
/*  if s>0 our initial choice of knots depends on the value of iopt. */
/*  if iopt=0 or iopt=1 and s>=fp0, we start computing the least-squares */
/*  polynomial of degree k which is a spline without interior knots. */
/*  if iopt=1 and fp0>s we start computing the least squares spline */
/*  according to the set of knots found at the last call of the routine. */
L45:
    if (*iopt == 0) {
	goto L50;
    }
    if (*n == nmin) {
	goto L50;
    }
    fp0 = fpint[*n];
    fpold = fpint[*n - 1];
    nplus = nrdata[*n];
    if (fp0 > *s) {
	goto L60;
    }
L50:
    *n = nmin;
    fpold = (float)0.;
    nplus = 0;
    nrdata[1] = *m - 2;
/*  main loop for the different sets of knots. m is a save upper bound */
/*  for the number of trials. */
L60:
    i__1 = *m;
    for (iter = 1; iter <= i__1; ++iter) {
	if (*n == nmin) {
	    *ier = -2;
	}
/*  find nrint, tne number of knot intervals. */
	nrint = *n - nmin + 1;
/*  find the position of the additional knots which are needed for */
/*  the b-spline representation of s(x). */
	nk1 = *n - *k1;
	i__ = *n;
	i__2 = *k1;
	for (j = 1; j <= i__2; ++j) {
	    t[j] = *xb;
	    t[i__] = *xe;
	    --i__;
/* L70: */
	}
/*  compute the b-spline coefficients of the least-squares spline */
/*  sinf(x). the observation matrix a is built up row by row and */
/*  reduced to upper triangular form by givens transformations. */
/*  at the same time fp=f(p=inf) is computed. */
	*fp = (float)0.;
/*  initialize the observation matrix a. */
	i__2 = nk1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    z__[i__] = (float)0.;
	    i__3 = *k1;
	    for (j = 1; j <= i__3; ++j) {
		a[i__ + j * a_dim1] = (float)0.;
/* L80: */
	    }
	}
	l = *k1;
	i__3 = *m;
	for (it = 1; it <= i__3; ++it) {
/*  fetch the current data point x(it),y(it). */
	    xi = x[it];
	    wi = w[it];
	    yi = y[it] * wi;
/*  search for knot interval t(l) <= xi < t(l+1). */
L85:
	    if (xi < t[l + 1] || l == nk1) {
		goto L90;
	    }
	    ++l;
	    goto L85;
/*  evaluate the (k+1) non-zero b-splines at xi and store them in q. */
L90:
	    fpbspl_(&t[1], n, k, &xi, &l, h__);
	    i__2 = *k1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		q[it + i__ * q_dim1] = h__[i__ - 1];
		h__[i__ - 1] *= wi;
/* L95: */
	    }
/*  rotate the new row of the observation matrix into triangle. */
	    j = l - *k1;
	    i__2 = *k1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		++j;
		piv = h__[i__ - 1];
		if (piv == (float)0.) {
		    goto L110;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a[j + a_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		fprota_(&cos__, &sin__, &yi, &z__[j]);
		if (i__ == *k1) {
		    goto L120;
		}
		i2 = 1;
		i3 = i__ + 1;
		i__4 = *k1;
		for (i1 = i3; i1 <= i__4; ++i1) {
		    ++i2;
/*  transformations to left hand side. */
		    fprota_(&cos__, &sin__, &h__[i1 - 1], &a[j + i2 * a_dim1])
			    ;
/* L100: */
		}
L110:
		;
	    }
/*  add contribution of this row to the sum of squares of residual */
/*  right hand sides. */
L120:
/* Computing 2nd power */
	    r__1 = yi;
	    *fp += r__1 * r__1;
/* L130: */
	}
	if (*ier == -2) {
	    fp0 = *fp;
	}
	fpint[*n] = fp0;
	fpint[*n - 1] = fpold;
	nrdata[*n] = nplus;
/*  backward substitution to obtain the b-spline coefficients. */
	fpback_(&a[a_offset], &z__[1], &nk1, k1, &c__[1], nest);
/*  test whether the approximation sinf(x) is an acceptable solution. */
	if (*iopt < 0) {
	    goto L440;
	}
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  if f(p=inf) < s accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L250;
	}
/*  if n = nmax, sinf(x) is an interpolating spline. */
	if (*n == nmax) {
	    goto L430;
	}
/*  increase the number of knots. */
/*  if n=nest we cannot increase the number of knots because of */
/*  the storage capacity limitation. */
	if (*n == *nest) {
	    goto L420;
	}
/*  determine the number of knots nplus we are going to add. */
	if (*ier == 0) {
	    goto L140;
	}
	nplus = 1;
	*ier = 0;
	goto L150;
L140:
	npl1 = nplus << 1;
	rn = (real) nplus;
	if (fpold - *fp > acc) {
	    npl1 = rn * fpms / (fpold - *fp);
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = nplus / 2, i__4 = max(i__4,i__5);
	i__3 = nplus << 1, i__2 = max(i__4,1);
	nplus = min(i__3,i__2);
L150:
	fpold = *fp;
/*  compute the sum((w(i)*(y(i)-s(x(i))))**2) for each knot interval */
/*  t(j+k) <= x(i) <= t(j+k+1) and store it in fpint(j),j=1,2,...nrint. */
	fpart = (float)0.;
	i__ = 1;
	l = *k2;
	new__ = 0;
	i__3 = *m;
	for (it = 1; it <= i__3; ++it) {
	    if (x[it] < t[l] || l > nk1) {
		goto L160;
	    }
	    new__ = 1;
	    ++l;
L160:
	    term = (float)0.;
	    l0 = l - *k2;
	    i__2 = *k1;
	    for (j = 1; j <= i__2; ++j) {
		++l0;
		term += c__[l0] * q[it + j * q_dim1];
/* L170: */
	    }
/* Computing 2nd power */
	    r__1 = w[it] * (term - y[it]);
	    term = r__1 * r__1;
	    fpart += term;
	    if (new__ == 0) {
		goto L180;
	    }
	    store = term * half;
	    fpint[i__] = fpart - store;
	    ++i__;
	    fpart = store;
	    new__ = 0;
L180:
	    ;
	}
	fpint[nrint] = fpart;
	i__3 = nplus;
	for (l = 1; l <= i__3; ++l) {
/*  add a new knot. */
	    fpknot_(&x[1], m, &t[1], n, &fpint[1], &nrdata[1], &nrint, nest, &
		    c__1);
/*  if n=nmax we locate the knots as for interpolation. */
	    if (*n == nmax) {
		goto L10;
	    }
/*  test whether we cannot further increase the number of knots. */
	    if (*n == *nest) {
		goto L200;
	    }
/* L190: */
	}
/*  restart the computations with the new set of knots. */
L200:
	;
    }
/*  test whether the least-squares kth degree polynomial is a solution */
/*  of our approximation problem. */
L250:
    if (*ier == -2) {
	goto L440;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 2: determination of the smoothing spline sp(x).                c */
/*  ***************************************************                 c */
/*  we have determined the number of knots and their position.          c */
/*  we now compute the b-spline coefficients of the smoothing spline    c */
/*  sp(x). the observation matrix a is extended by the rows of matrix   c */
/*  b expressing that the kth derivative discontinuities of sp(x) at    c */
/*  the interior knots t(k+2),...t(n-k-1) must be zero. the corres-     c */
/*  ponding weights of these additional rows are set to 1/p.            c */
/*  iteratively we then have to determine the value of p such that      c */
/*  f(p)=sum((w(i)*(y(i)-sp(x(i))))**2) be = s. we already know that    c */
/*  the least-squares kth degree polynomial corresponds to p=0, and     c */
/*  that the least-squares spline corresponds to p=infinity. the        c */
/*  iteration process which is proposed here, makes use of rational     c */
/*  interpolation. since f(p) is a convex and strictly decreasing       c */
/*  function of p, it can be approximated by a rational function        c */
/*  r(p) = (u*p+v)/(p+w). three values of p(p1,p2,p3) with correspond-  c */
/*  ing values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s) are used      c */
/*  to calculate the new value of p such that r(p)=s. convergence is    c */
/*  guaranteed by taking f1>0 and f3<0.                                 c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  evaluate the discontinuity jump of the kth derivative of the */
/*  b-splines at the knots t(l),l=k+2,...n-k-1 and store in b. */
    fpdisc_(&t[1], n, k2, &b[b_offset], nest);
/*  initial value for p. */
    p1 = (float)0.;
    f1 = fp0 - *s;
    p3 = -one;
    f3 = fpms;
    p = (float)0.;
    i__1 = nk1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p += a[i__ + a_dim1];
/* L255: */
    }
    rn = (real) nk1;
    p = rn / p;
    ich1 = 0;
    ich3 = 0;
    n8 = *n - nmin;
/*  iteration process to find the root of f(p) = s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  the rows of matrix b with weight 1/p are rotated into the */
/*  triangularised observation matrix a which is stored in g. */
	pinv = one / p;
	i__3 = nk1;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    c__[i__] = z__[i__];
	    g[i__ + *k2 * g_dim1] = (float)0.;
	    i__2 = *k1;
	    for (j = 1; j <= i__2; ++j) {
		g[i__ + j * g_dim1] = a[i__ + j * a_dim1];
/* L260: */
	    }
	}
	i__2 = n8;
	for (it = 1; it <= i__2; ++it) {
/*  the row of matrix b is rotated into triangle by givens transformation */
	    i__3 = *k2;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h__[i__ - 1] = b[it + i__ * b_dim1] * pinv;
/* L270: */
	    }
	    yi = (float)0.;
	    i__3 = nk1;
	    for (j = it; j <= i__3; ++j) {
		piv = h__[0];
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &g[j + g_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		fprota_(&cos__, &sin__, &yi, &c__[j]);
		if (j == nk1) {
		    goto L300;
		}
		i2 = *k1;
		if (j > n8) {
		    i2 = nk1 - j;
		}
		i__4 = i2;
		for (i__ = 1; i__ <= i__4; ++i__) {
/*  transformations to left hand side. */
		    i1 = i__ + 1;
		    fprota_(&cos__, &sin__, &h__[i1 - 1], &g[j + i1 * g_dim1])
			    ;
		    h__[i__ - 1] = h__[i1 - 1];
/* L280: */
		}
		h__[i2] = (float)0.;
/* L290: */
	    }
L300:
	    ;
	}
/*  backward substitution to obtain the b-spline coefficients. */
	fpback_(&g[g_offset], &c__[1], &nk1, k2, &c__[1], nest);
/*  computation of f(p). */
	*fp = (float)0.;
	l = *k2;
	i__2 = *m;
	for (it = 1; it <= i__2; ++it) {
	    if (x[it] < t[l] || l > nk1) {
		goto L310;
	    }
	    ++l;
L310:
	    l0 = l - *k2;
	    term = (float)0.;
	    i__3 = *k1;
	    for (j = 1; j <= i__3; ++j) {
		++l0;
		term += c__[l0] * q[it + j * q_dim1];
/* L320: */
	    }
/* Computing 2nd power */
	    r__1 = w[it] * (term - y[it]);
	    *fp += r__1 * r__1;
/* L330: */
	}
/*  test whether the approximation sp(x) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  test whether the maximal number of iterations is reached. */
	if (iter == *maxit) {
	    goto L400;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L340;
	}
	if (f2 - f3 > acc) {
	    goto L335;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L360;
L335:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L340:
	if (ich1 != 0) {
	    goto L350;
	}
	if (f1 - f2 > acc) {
	    goto L345;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L360;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L360;
L345:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L350:
	if (f2 >= f1 || f2 <= f3) {
	    goto L410;
	}
/*  find the new value for p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L360:
	;
    }
/*  error codes and messages. */
L400:
    *ier = 3;
    goto L440;
L410:
    *ier = 2;
    goto L440;
L420:
    *ier = 1;
    goto L440;
L430:
    *ier = -1;
L440:
    return 0;
} /* fpcurf_ */

/* Subroutine */ int fpcuro_(a, b, c__, d__, x, n)
real *a, *b, *c__, *d__, *x;
integer *n;
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double atan(), sqrt(), atan2(), cos(), pow_dd(), r_sign();

    /* Local variables */
    static real half, disc, ovfl, tent, four, step, f;
    static integer i__;
    static real q, r__, u, y, three, a1, b1, c1, d1, e3, p3, u1, u2, df, pi3, 
	    two;

/*  subroutine fpcuro finds the real zeros of a cubic polynomial */
/*  p(x) = a*x**3+b*x**2+c*x+d. */

/*  calling sequence: */
/*     call fpcuro(a,b,c,d,x,n) */

/*  input parameters: */
/*    a,b,c,d: real values, containing the coefficients of p(x). */

/*  output parameters: */
/*    x      : real array,length 3, which contains the real zeros of p(x) */
/*    n      : integer, giving the number of real zeros of p(x). */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array argument.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  set constants */
    /* Parameter adjustments */
    --x;

    /* Function Body */
    two = (float)2.;
    three = (float)3.;
    four = (float)4.;
    ovfl = (float)1e4;
    half = (float).5;
    tent = (float).1;
    e3 = tent / (float).3;
    pi3 = atan((float)1.) / (float).75;
    a1 = dabs(*a);
    b1 = dabs(*b);
    c1 = dabs(*c__);
    d1 = dabs(*d__);
/*  test whether p(x) is a third degree polynomial. */
/* Computing MAX */
    r__1 = max(b1,c1);
    if (dmax(r__1,d1) < a1 * ovfl) {
	goto L300;
    }
/*  test whether p(x) is a second degree polynomial. */
    if (dmax(c1,d1) < b1 * ovfl) {
	goto L200;
    }
/*  test whether p(x) is a first degree polynomial. */
    if (d1 < c1 * ovfl) {
	goto L100;
    }
/*  p(x) is a constant function. */
    *n = 0;
    goto L800;
/*  p(x) is a first degree polynomial. */
L100:
    *n = 1;
    x[1] = -(*d__) / *c__;
    goto L500;
/*  p(x) is a second degree polynomial. */
L200:
    disc = *c__ * *c__ - four * *b * *d__;
    *n = 0;
    if (disc < (float)0.) {
	goto L800;
    }
    *n = 2;
    u = sqrt(disc);
    b1 = *b + *b;
    x[1] = (-(*c__) + u) / b1;
    x[2] = (-(*c__) - u) / b1;
    goto L500;
/*  p(x) is a third degree polynomial. */
L300:
    b1 = *b / *a * e3;
    c1 = *c__ / *a;
    d1 = *d__ / *a;
    q = c1 * e3 - b1 * b1;
    r__ = b1 * b1 * b1 + (d1 - b1 * c1) * half;
    disc = q * q * q + r__ * r__;
    if (disc > (float)0.) {
	goto L400;
    }
    u = sqrt((dabs(q)));
    if (r__ < (float)0.) {
	u = -u;
    }
    p3 = atan2(sqrt(-disc), (dabs(r__))) * e3;
    u2 = u + u;
    *n = 3;
    x[1] = -u2 * cos(p3) - b1;
    x[2] = u2 * cos(pi3 - p3) - b1;
    x[3] = u2 * cos(pi3 + p3) - b1;
    goto L500;
L400:
    u = sqrt(disc);
    u1 = -r__ + u;
    u2 = -r__ - u;
    *n = 1;
    d__1 = (doublereal) dabs(u1);
    d__2 = (doublereal) e3;
    r__1 = pow_dd(&d__1, &d__2);
    d__3 = (doublereal) dabs(u2);
    d__4 = (doublereal) e3;
    r__2 = pow_dd(&d__3, &d__4);
    x[1] = r_sign(&r__1, &u1) + r_sign(&r__2, &u2) - b1;
/*  apply a newton iteration to improve the accuracy of the roots. */
L500:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	y = x[i__];
	f = ((*a * y + *b) * y + *c__) * y + *d__;
	df = (three * *a * y + two * *b) * y + *c__;
	step = (float)0.;
	if (dabs(f) < dabs(df) * tent) {
	    step = f / df;
	}
	x[i__] = y - step;
/* L700: */
    }
L800:
    return 0;
} /* fpcuro_ */

/* Subroutine */ int fpcyt1_(a, n, nn)
real *a;
integer *n, *nn;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
    static real beta, teta;
    static integer i__;
    static real gamma, v;
    static integer n1, n2;
    static real aa, one, sum;

/* (l u)-decomposition of a cyclic tridiagonal matrix with the non-zero */
/* elements stored as follows */

/*    | a(1,2) a(1,3)                                    a(1,1)  | */
/*    | a(2,1) a(2,2) a(2,3)                                     | */
/*    |        a(3,1) a(3,2) a(3,3)                              | */
/*    |               ...............                            | */
/*    |                               a(n-1,1) a(n-1,2) a(n-1,3) | */
/*    | a(n,3)                                  a(n,1)   a(n,2)  | */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  set constant */
    /* Parameter adjustments */
    a_dim1 = *nn;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;

    /* Function Body */
    one = (float)1.;
    n2 = *n - 2;
    beta = one / a[(a_dim1 << 1) + 1];
    gamma = a[*n + a_dim1 * 3];
    teta = a[a_dim1 + 1] * beta;
    a[(a_dim1 << 2) + 1] = beta;
    a[a_dim1 * 5 + 1] = gamma;
    a[a_dim1 * 6 + 1] = teta;
    sum = gamma * teta;
    i__1 = n2;
    for (i__ = 2; i__ <= i__1; ++i__) {
	v = a[i__ - 1 + a_dim1 * 3] * beta;
	aa = a[i__ + a_dim1];
	beta = one / (a[i__ + (a_dim1 << 1)] - aa * v);
	gamma = -gamma * v;
	teta = -teta * aa * beta;
	a[i__ + (a_dim1 << 2)] = beta;
	a[i__ + a_dim1 * 5] = gamma;
	a[i__ + a_dim1 * 6] = teta;
	sum += gamma * teta;
/* L10: */
    }
    n1 = *n - 1;
    v = a[n2 + a_dim1 * 3] * beta;
    aa = a[n1 + a_dim1];
    beta = one / (a[n1 + (a_dim1 << 1)] - aa * v);
    gamma = a[*n + a_dim1] - gamma * v;
    teta = (a[n1 + a_dim1 * 3] - teta * aa) * beta;
    a[n1 + (a_dim1 << 2)] = beta;
    a[n1 + a_dim1 * 5] = gamma;
    a[n1 + a_dim1 * 6] = teta;
    a[*n + (a_dim1 << 2)] = one / (a[*n + (a_dim1 << 1)] - (sum + gamma * 
	    teta));
    return 0;
} /* fpcyt1_ */

/* Subroutine */ int fpcyt2_(a, n, b, c__, nn)
real *a;
integer *n;
real *b, *c__;
integer *nn;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
    static integer i__, j, j1, n1;
    static real cc, sum;

/* subroutine fpcyt2 solves a linear n x n system */
/*         a * c = b */
/* where matrix a is a cyclic tridiagonal matrix, decomposed */
/* using subroutine fpsyt1. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --c__;
    --b;
    a_dim1 = *nn;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;

    /* Function Body */
    c__[1] = b[1] * a[(a_dim1 << 2) + 1];
    sum = c__[1] * a[a_dim1 * 5 + 1];
    n1 = *n - 1;
    i__1 = n1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	c__[i__] = (b[i__] - a[i__ + a_dim1] * c__[i__ - 1]) * a[i__ + (
		a_dim1 << 2)];
	sum += c__[i__] * a[i__ + a_dim1 * 5];
/* L10: */
    }
    cc = (b[*n] - sum) * a[*n + (a_dim1 << 2)];
    c__[*n] = cc;
    c__[n1] -= cc * a[n1 + a_dim1 * 6];
    j = n1;
    i__1 = *n;
    for (i__ = 3; i__ <= i__1; ++i__) {
	j1 = j - 1;
	c__[j1] = c__[j1] - c__[j] * a[j1 + a_dim1 * 3] * a[j1 + (a_dim1 << 2)
		] - cc * a[j1 + a_dim1 * 6];
	j = j1;
/* L20: */
    }
    return 0;
} /* fpcyt2_ */

/* Subroutine */ int fpdeno_(maxtr, up, left, right, nbind, merk)
integer *maxtr, *up, *left, *right, *nbind, *merk;
{
    static integer i__, j, k, l, point, niveau;

/*  subroutine fpdeno frees the nodes of all branches of a triply linked */
/*  tree with length < nbind by putting to zero their up field. */
/*  on exit the parameter merk points to the terminal node of the */
/*  most left branch of length nbind or takes the value 1 if there */
/*  is no such branch. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars .. */
/*  .. */
    /* Parameter adjustments */
    --right;
    --left;
    --up;

    /* Function Body */
    i__ = 1;
    niveau = 0;
L10:
    point = i__;
    i__ = left[point];
    if (i__ == 0) {
	goto L20;
    }
    ++niveau;
    goto L10;
L20:
    if (niveau == *nbind) {
	goto L70;
    }
L30:
    i__ = right[point];
    j = up[point];
    up[point] = 0;
    k = left[j];
    if (point != k) {
	goto L50;
    }
    if (i__ != 0) {
	goto L40;
    }
    --niveau;
    if (niveau == 0) {
	goto L80;
    }
    point = j;
    goto L30;
L40:
    left[j] = i__;
    goto L10;
L50:
    l = right[k];
    if (point == l) {
	goto L60;
    }
    k = l;
    goto L50;
L60:
    right[k] = i__;
    point = k;
L70:
    i__ = right[point];
    if (i__ != 0) {
	goto L10;
    }
    i__ = up[point];
    --niveau;
    if (niveau == 0) {
	goto L80;
    }
    point = i__;
    goto L70;
L80:
    k = 1;
    l = left[k];
    if (up[l] == 0) {
	return 0;
    }
L90:
    *merk = k;
    k = left[k];
    if (k != 0) {
	goto L90;
    }
    return 0;
} /* fpdeno_ */

/* Subroutine */ int fpdisc_(t, n, k2, b, nest)
real *t;
integer *n, *k2;
real *b;
integer *nest;
{
    /* System generated locals */
    integer b_dim1, b_offset, i__1, i__2, i__3;

    /* Local variables */
    static real prod, h__[12];
    static integer i__, j, k, l, nrint, k1;
    static real an;
    static integer ik, jk, lj, lk, lp, nk1;
    static real fac;
    static integer lmk;

/*  subroutine fpdisc calculates the discontinuity jumps of the kth */
/*  derivative of the b-splines of degree k at the knots t(k+2)..t(n-k-1) */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  .. */
    /* Parameter adjustments */
    --t;
    b_dim1 = *nest;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    k1 = *k2 - 1;
    k = k1 - 1;
    nk1 = *n - k1;
    nrint = nk1 - k;
    an = (real) nrint;
    fac = an / (t[nk1 + 1] - t[k1]);
    i__1 = nk1;
    for (l = *k2; l <= i__1; ++l) {
	lmk = l - k1;
	i__2 = k1;
	for (j = 1; j <= i__2; ++j) {
	    ik = j + k1;
	    lj = l + j;
	    lk = lj - *k2;
	    h__[j - 1] = t[l] - t[lk];
	    h__[ik - 1] = t[l] - t[lj];
/* L10: */
	}
	lp = lmk;
	i__2 = *k2;
	for (j = 1; j <= i__2; ++j) {
	    jk = j;
	    prod = h__[j - 1];
	    i__3 = k;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		++jk;
		prod = prod * h__[jk - 1] * fac;
/* L20: */
	    }
	    lk = lp + k1;
	    b[lmk + j * b_dim1] = (t[lk] - t[lp]) / prod;
	    ++lp;
/* L30: */
	}
/* L40: */
    }
    return 0;
} /* fpdisc_ */

/* Subroutine */ int fpfrno_(maxtr, up, left, right, info, point, merk, n1, 
	count, ier)
integer *maxtr, *up, *left, *right, *info, *point, *merk, *n1, *count, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, n, niveau;

/*  subroutine fpfrno collects the free nodes (up field zero) of the */
/*  triply linked tree the information of which is kept in the arrays */
/*  up,left,right and info. the maximal length of the branches of the */
/*  tree is given by n1. if no free nodes are found, the error flag */
/*  ier is set to 1. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars */
/*  .. */
    /* Parameter adjustments */
    --info;
    --right;
    --left;
    --up;

    /* Function Body */
    *ier = 1;
    if (*n1 == 2) {
	goto L140;
    }
    niveau = 1;
    *count = 2;
L10:
    j = 0;
    i__ = 1;
L20:
    if (j == niveau) {
	goto L30;
    }
    k = 0;
    l = left[i__];
    if (l == 0) {
	goto L110;
    }
    i__ = l;
    ++j;
    goto L20;
L30:
    if ((i__1 = i__ - *count) < 0) {
	goto L110;
    } else if (i__1 == 0) {
	goto L100;
    } else {
	goto L40;
    }
L40:
    if (up[*count] == 0) {
	goto L50;
    }
    ++(*count);
    goto L30;
L50:
    up[*count] = up[i__];
    left[*count] = left[i__];
    right[*count] = right[i__];
    info[*count] = info[i__];
    if (*merk == i__) {
	*merk = *count;
    }
    if (*point == i__) {
	*point = *count;
    }
    if (k == 0) {
	goto L60;
    }
    right[k] = *count;
    goto L70;
L60:
    n = up[i__];
    left[n] = *count;
L70:
    l = left[i__];
L80:
    if (l == 0) {
	goto L90;
    }
    up[l] = *count;
    l = right[l];
    goto L80;
L90:
    up[i__] = 0;
    i__ = *count;
L100:
    ++(*count);
L110:
    l = right[i__];
    k = i__;
    if (l == 0) {
	goto L120;
    }
    i__ = l;
    goto L20;
L120:
    l = up[i__];
    --j;
    if (j == 0) {
	goto L130;
    }
    i__ = l;
    goto L110;
L130:
    ++niveau;
    if (niveau <= *n1) {
	goto L10;
    }
    if (*count > *maxtr) {
	goto L140;
    }
    *ier = 0;
L140:
    return 0;
} /* fpfrno_ */

/* Subroutine */ int fpgivs_(piv, ww, cos__, sin__)
real *piv, *ww, *cos__, *sin__;
{
    /* System generated locals */
    real r__1;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    static real store, dd, one;

/*  subroutine fpgivs calculates the parameters of a givens */
/*  transformation . */
/*  .. */
/*  ..scalar arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  .. */
    one = (float)1.;
    store = dabs(*piv);
    if (store >= *ww) {
/* Computing 2nd power */
	r__1 = *ww / *piv;
	dd = store * sqrt(one + r__1 * r__1);
    }
    if (store < *ww) {
/* Computing 2nd power */
	r__1 = *piv / *ww;
	dd = *ww * sqrt(one + r__1 * r__1);
    }
    *cos__ = *ww / dd;
    *sin__ = *piv / dd;
    *ww = dd;
    return 0;
} /* fpgivs_ */

/* Subroutine */ int fpgrdi_(ifsu, ifsv, ifbu, ifbv, iback, u, mu, v, mv, z__,
	 mz, dz, iop0, iop1, tu, nu, tv, nv, p, c__, nc, sq, fp, fpu, fpv, mm,
	 mvnu, spu, spv, right, q, au, av1, av2, bu, bv, aa, bb, cc, cosi, 
	nru, nrv)
integer *ifsu, *ifsv, *ifbu, *ifbv, *iback;
real *u;
integer *mu;
real *v;
integer *mv;
real *z__;
integer *mz;
real *dz;
integer *iop0, *iop1;
real *tu;
integer *nu;
real *tv;
integer *nv;
real *p, *c__;
integer *nc;
real *sq, *fp, *fpu, *fpv;
integer *mm, *mvnu;
real *spu, *spv, *right, *q, *au, *av1, *av2, *bu, *bv, *aa, *bb, *cc, *cosi;
integer *nru, *nrv;
{
    /* System generated locals */
    integer spu_dim1, spu_offset, spv_dim1, spv_offset, au_dim1, au_offset, 
	    av1_dim1, av1_offset, av2_dim1, av2_offset, bu_dim1, bu_offset, 
	    bv_dim1, bv_offset, i__1, i__2, i__3;
    real r__1;

    /* Builtin functions */
    double cos(), sin();

    /* Local variables */
    static real half;
    static integer ncof, jper;
    static real term, pinv;
    static integer irot, numu, numv, numu1, numv1;
    static real h__[5];
    static integer i__, j, k, l;
    static real three;
    static integer nrold, i0, i1, i2, i3, j0, j1, k1, k2, l0, l1, l2, n1;
    static real h1[5], h2[4];
    extern /* Subroutine */ int fpcyt1_(), fpcyt2_();
    static integer ic;
    static real co;
    static integer ii, ij, ik, iq;
    static real si;
    static integer it, jj, jk;
    extern /* Subroutine */ int fpbacp_();
    static integer iz;
    extern /* Subroutine */ int fpdisc_(), fpback_(), fpbspl_();
    static integer number;
    extern /* Subroutine */ int fprota_(), fpgivs_();
    static integer nroldu, nroldv;
    static real dz1, dz2, dz3;
    static integer nu4, nv4, nu7, nu8, nu9, nv7, nv8;
    static real fac, arg, one;
    static integer nv11;
    static real piv, fac0;
    static integer mvv, nuu;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpback,fpbspl,fpgivs,fpcyt1,fpcyt2,fpdisc,fpbacp,fprota */
/*  .. */
/*  let */
/*               |   (spu)    |            |   (spv)    | */
/*        (au) = | ---------- |     (av) = | ---------- | */
/*               | (1/p) (bu) |            | (1/p) (bv) | */

/*                                | z  ' 0 | */
/*                            q = | ------ | */
/*                                | 0  ' 0 | */

/*  with c      : the (nu-4) x (nv-4) matrix which contains the b-spline */
/*                coefficients. */
/*       z      : the mu x mv matrix which contains the function values. */
/*       spu,spv: the mu x (nu-4), resp. mv x (nv-4) observation matrices */
/*                according to the least-squares problems in the u-,resp. */
/*                v-direction. */
/*       bu,bv  : the (nu-7) x (nu-4),resp. (nv-7) x (nv-4) matrices */
/*                containing the discontinuity jumps of the derivatives */
/*                of the b-splines in the u-,resp.v-variable at the knots */
/*  the b-spline coefficients of the smoothing spline are then calculated */
/*  as the least-squares solution of the following over-determined linear */
/*  system of equations */

/*    (1)  (av) c (au)' = q */

/*  subject to the constraints */

/*    (2)  c(i,nv-3+j) = c(i,j), j=1,2,3 ; i=1,2,...,nu-4 */

/*    (3)  if iop0 = 0  c(1,j) = dz(1) */
/*            iop0 = 1  c(1,j) = dz(1) */
/*                      c(2,j) = dz(1)+(dz(2)*cosi(1,j)+dz(3)*cosi(2,j))* */
/*                               tu(5)/3. = cc(j) , j=1,2,...nv-4 */

/*    (4)  if iop1 = 1  c(nu-4,j) = 0, j=1,2,...,nv-4. */

/*  set constants */
    /* Parameter adjustments */
    --nru;
    spu_dim1 = *mu;
    spu_offset = 1 + spu_dim1 * 1;
    spu -= spu_offset;
    --u;
    --nrv;
    aa -= 3;
    spv_dim1 = *mv;
    spv_offset = 1 + spv_dim1 * 1;
    spv -= spv_offset;
    --v;
    --z__;
    --dz;
    bu_dim1 = *nu;
    bu_offset = 1 + bu_dim1 * 1;
    bu -= bu_offset;
    au_dim1 = *nu;
    au_offset = 1 + au_dim1 * 1;
    au -= au_offset;
    --fpu;
    --tu;
    cosi -= 3;
    --cc;
    bb -= 3;
    bv_dim1 = *nv;
    bv_offset = 1 + bv_dim1 * 1;
    bv -= bv_offset;
    av2_dim1 = *nv;
    av2_offset = 1 + av2_dim1 * 1;
    av2 -= av2_offset;
    av1_dim1 = *nv;
    av1_offset = 1 + av1_dim1 * 1;
    av1 -= av1_offset;
    --fpv;
    --tv;
    --c__;
    --right;
    --q;

    /* Function Body */
    one = (float)1.;
    three = (float)3.;
    half = (float).5;
/*  initialization */
    nu4 = *nu - 4;
    nu7 = *nu - 7;
    nu8 = *nu - 8;
    nu9 = *nu - 9;
    nv4 = *nv - 4;
    nv7 = *nv - 7;
    nv8 = *nv - 8;
    nv11 = *nv - 11;
    nuu = nu4 - *iop0 - *iop1 - 1;
    if (*p > (float)0.) {
	pinv = one / *p;
    }
/*  it depends on the value of the flags ifsu,ifsv,ifbu,ifbv and iop0 and */
/*  on the value of p whether the matrices (spu), (spv), (bu), (bv) and */
/*  (cosi) still must be determined. */
    if (*ifsu != 0) {
	goto L30;
    }
/*  calculate the non-zero elements of the matrix (spu) which is the ob- */
/*  servation matrix according to the least-squares spline approximation */
/*  problem in the u-direction. */
    l = 4;
    l1 = 5;
    number = 0;
    i__1 = *mu;
    for (it = 1; it <= i__1; ++it) {
	arg = u[it];
L10:
	if (arg < tu[l1] || l == nu4) {
	    goto L15;
	}
	l = l1;
	l1 = l + 1;
	++number;
	goto L10;
L15:
	fpbspl_(&tu[1], nu, &c__3, &arg, &l, h__);
	for (i__ = 1; i__ <= 4; ++i__) {
	    spu[it + i__ * spu_dim1] = h__[i__ - 1];
/* L20: */
	}
	nru[it] = number;
/* L25: */
    }
    *ifsu = 1;
/*  calculate the non-zero elements of the matrix (spv) which is the ob- */
/*  servation matrix according to the least-squares spline approximation */
/*  problem in the v-direction. */
L30:
    if (*ifsv != 0) {
	goto L85;
    }
    l = 4;
    l1 = 5;
    number = 0;
    i__1 = *mv;
    for (it = 1; it <= i__1; ++it) {
	arg = v[it];
L35:
	if (arg < tv[l1] || l == nv4) {
	    goto L40;
	}
	l = l1;
	l1 = l + 1;
	++number;
	goto L35;
L40:
	fpbspl_(&tv[1], nv, &c__3, &arg, &l, h__);
	for (i__ = 1; i__ <= 4; ++i__) {
	    spv[it + i__ * spv_dim1] = h__[i__ - 1];
/* L45: */
	}
	nrv[it] = number;
/* L50: */
    }
    *ifsv = 1;
    if (*iop0 == 0) {
	goto L85;
    }
/*  calculate the coefficients of the interpolating splines for cos(v) */
/*  and sin(v). */
    i__1 = nv4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	cosi[(i__ << 1) + 1] = (float)0.;
	cosi[(i__ << 1) + 2] = (float)0.;
/* L55: */
    }
    if (nv7 < 4) {
	goto L85;
    }
    i__1 = nv7;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l = i__ + 3;
	arg = tv[l];
	fpbspl_(&tv[1], nv, &c__3, &arg, &l, h__);
	for (j = 1; j <= 3; ++j) {
	    av1[i__ + j * av1_dim1] = h__[j - 1];
/* L60: */
	}
	cosi[(i__ << 1) + 1] = cos(arg);
	cosi[(i__ << 1) + 2] = sin(arg);
/* L65: */
    }
    fpcyt1_(&av1[av1_offset], &nv7, nv);
    for (j = 1; j <= 2; ++j) {
	i__1 = nv7;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    right[i__] = cosi[j + (i__ << 1)];
/* L70: */
	}
	fpcyt2_(&av1[av1_offset], &nv7, &right[1], &right[1], nv);
	i__1 = nv7;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    cosi[j + (i__ + 1 << 1)] = right[i__];
/* L75: */
	}
	cosi[j + 2] = cosi[j + (nv7 + 1 << 1)];
	cosi[j + (nv7 + 2 << 1)] = cosi[j + 4];
	cosi[j + (nv4 << 1)] = cosi[j + 6];
/* L80: */
    }
L85:
    if (*p <= (float)0.) {
	goto L150;
    }
/*  calculate the non-zero elements of the matrix (bu). */
    if (*ifbu != 0 || nu8 == 0) {
	goto L90;
    }
    fpdisc_(&tu[1], nu, &c__5, &bu[bu_offset], nu);
    *ifbu = 1;
/*  calculate the non-zero elements of the matrix (bv). */
L90:
    if (*ifbv != 0 || nv8 == 0) {
	goto L150;
    }
    fpdisc_(&tv[1], nv, &c__5, &bv[bv_offset], nv);
    *ifbv = 1;
/*  substituting (2),(3) and (4) into (1), we obtain the overdetermined */
/*  system */
/*         (5)  (avv) (cr) (auu)' = (qq) */
/*  from which the nuu*nv7 remaining coefficients */
/*         c(i,j) , i=2+iop0,3+iop0,...,nu-4-iop1 ; j=1,2,...,nv-7 , */
/*  the elements of (cr), are then determined in the least-squares sense. */
/*  simultaneously, we compute the resulting sum of squared residuals sq. */
L150:
    dz1 = dz[1];
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	aa[(i__ << 1) + 1] = dz1;
/* L155: */
    }
    if (nv8 == 0 || *p <= (float)0.) {
	goto L165;
    }
    i__1 = nv8;
    for (i__ = 1; i__ <= i__1; ++i__) {
	bb[(i__ << 1) + 1] = (float)0.;
/* L160: */
    }
L165:
    mvv = *mv;
    if (*iop0 == 0) {
	goto L220;
    }
    fac = tu[5] / three;
    dz2 = dz[2] * fac;
    dz3 = dz[3] * fac;
    i__1 = nv4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	cc[i__] = dz1 + dz2 * cosi[(i__ << 1) + 1] + dz3 * cosi[(i__ << 1) + 
		2];
/* L170: */
    }
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	number = nrv[i__];
	fac = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    ++number;
	    fac += cc[number] * spv[i__ + j * spv_dim1];
/* L180: */
	}
	aa[(i__ << 1) + 2] = fac;
/* L190: */
    }
    if (nv8 == 0 || *p <= (float)0.) {
	goto L220;
    }
    i__1 = nv8;
    for (i__ = 1; i__ <= i__1; ++i__) {
	number = i__;
	fac = (float)0.;
	for (j = 1; j <= 5; ++j) {
	    fac += cc[number] * bv[i__ + j * bv_dim1];
	    ++number;
/* L200: */
	}
	bb[(i__ << 1) + 2] = fac * pinv;
/* L210: */
    }
    mvv += nv8;
/*  we first determine the matrices (auu) and (qq). then we reduce the */
/*  matrix (auu) to upper triangular form (ru) using givens rotations. */
/*  we apply the same transformations to the rows of matrix qq to obtain */
/*  the (mv+nv8) x nuu matrix g. */
/*  we store matrix (ru) into au and g into q. */
L220:
    l = mvv * nuu;
/*  initialization. */
    *sq = (float)0.;
    i__1 = l;
    for (i__ = 1; i__ <= i__1; ++i__) {
	q[i__] = (float)0.;
/* L230: */
    }
    i__1 = nuu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 5; ++j) {
	    au[i__ + j * au_dim1] = (float)0.;
/* L240: */
	}
    }
    l = 0;
    nrold = 0;
    n1 = nrold + 1;
    i__1 = *mu;
    for (it = 1; it <= i__1; ++it) {
	number = nru[it];
/*  find the appropriate column of q. */
L250:
	i__2 = mvv;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = (float)0.;
/* L260: */
	}
	if (nrold == number) {
	    goto L280;
	}
	if (*p <= (float)0.) {
	    goto L410;
	}
/*  fetch a new row of matrix (bu). */
	for (j = 1; j <= 5; ++j) {
	    h__[j - 1] = bu[n1 + j * bu_dim1] * pinv;
/* L270: */
	}
	i0 = 1;
	i1 = 5;
	goto L310;
/*  fetch a new row of matrix (spu). */
L280:
	for (j = 1; j <= 4; ++j) {
	    h__[j - 1] = spu[it + j * spu_dim1];
/* L290: */
	}
/*  find the appropriate column of q. */
	i__2 = *mv;
	for (j = 1; j <= i__2; ++j) {
	    ++l;
	    right[j] = z__[l];
/* L300: */
	}
	i0 = 1;
	i1 = 4;
L310:
	if (nu7 - number == *iop1) {
	    --i1;
	}
	j0 = n1;
/*  take into account that we eliminate the constraints (3) */
L320:
	if (j0 - 1 > *iop0) {
	    goto L360;
	}
	fac0 = h__[i0 - 1];
	i__2 = *mv;
	for (j = 1; j <= i__2; ++j) {
	    right[j] -= fac0 * aa[j0 + (j << 1)];
/* L330: */
	}
	if (*mv == mvv) {
	    goto L350;
	}
	j = *mv;
	i__2 = nv8;
	for (jj = 1; jj <= i__2; ++jj) {
	    ++j;
	    right[j] -= fac0 * bb[j0 + (jj << 1)];
/* L340: */
	}
L350:
	++j0;
	++i0;
	goto L320;
L360:
	irot = nrold - *iop0 - 1;
	if (irot < 0) {
	    irot = 0;
	}
/*  rotate the new row of matrix (auu) into triangle. */
	i__2 = i1;
	for (i__ = i0; i__ <= i__2; ++i__) {
	    ++irot;
	    piv = h__[i__ - 1];
	    if (piv == (float)0.) {
		goto L390;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &au[irot + au_dim1], &co, &si);
/*  apply that transformation to the rows of matrix (qq). */
	    iq = (irot - 1) * mvv;
	    i__3 = mvv;
	    for (j = 1; j <= i__3; ++j) {
		++iq;
		fprota_(&co, &si, &right[j], &q[iq]);
/* L370: */
	    }
/*  apply that transformation to the columns of (auu). */
	    if (i__ == i1) {
		goto L390;
	    }
	    i2 = 1;
	    i3 = i__ + 1;
	    i__3 = i1;
	    for (j = i3; j <= i__3; ++j) {
		++i2;
		fprota_(&co, &si, &h__[j - 1], &au[irot + i2 * au_dim1]);
/* L380: */
	    }
L390:
	    ;
	}
/* we update the sum of squared residuals */
	i__2 = mvv;
	for (j = 1; j <= i__2; ++j) {
/* Computing 2nd power */
	    r__1 = right[j];
	    *sq += r__1 * r__1;
/* L395: */
	}
/* L400: */
	if (nrold == number) {
	    goto L420;
	}
L410:
	nrold = n1;
	++n1;
	goto L250;
L420:
	;
    }
/*  we determine the matrix (avv) and then we reduce her to */
/*  upper triangular form (rv) using givens rotations. */
/*  we apply the same transformations to the columns of matrix */
/*  g to obtain the (nv-7) x (nu-5-iop0-iop1) matrix h. */
/*  we store matrix (rv) into av1 and av2, h into c. */
/*  the nv7 x nv7 upper triangular matrix (rv) has the form */
/*              | av1 '     | */
/*       (rv) = |     ' av2 | */
/*              |  0  '     | */
/*  with (av2) a nv7 x 4 matrix and (av1) a nv11 x nv11 upper */
/*  triangular matrix of bandwidth 5. */
    ncof = nuu * nv7;
/*  initialization. */
    i__1 = ncof;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = (float)0.;
/* L430: */
    }
    i__1 = nv4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	av1[i__ + av1_dim1 * 5] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    av1[i__ + j * av1_dim1] = (float)0.;
	    av2[i__ + j * av2_dim1] = (float)0.;
/* L440: */
	}
    }
    jper = 0;
    nrold = 0;
    i__1 = *mv;
    for (it = 1; it <= i__1; ++it) {
	number = nrv[it];
L450:
	if (nrold == number) {
	    goto L480;
	}
	if (*p <= (float)0.) {
	    goto L760;
	}
/*  fetch a new row of matrix (bv). */
	n1 = nrold + 1;
	for (j = 1; j <= 5; ++j) {
	    h__[j - 1] = bv[n1 + j * bv_dim1] * pinv;
/* L460: */
	}
/*  find the appropiate row of g. */
	i__2 = nuu;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = (float)0.;
/* L465: */
	}
	if (*mv == mvv) {
	    goto L510;
	}
	l = *mv + n1;
	i__2 = nuu;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = q[l];
	    l += mvv;
/* L470: */
	}
	goto L510;
/*  fetch a new row of matrix (spv) */
L480:
	h__[4] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    h__[j - 1] = spv[it + j * spv_dim1];
/* L490: */
	}
/*  find the appropiate row of g. */
	l = it;
	i__2 = nuu;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = q[l];
	    l += mvv;
/* L500: */
	}
/*  test whether there are non-zero values in the new row of (avv) */
/*  corresponding to the b-splines n(j,v),j=nv7+1,...,nv4. */
L510:
	if (nrold < nv11) {
	    goto L710;
	}
	if (jper != 0) {
	    goto L550;
	}
/*  initialize the matrix (av2). */
	jk = nv11 + 1;
	for (i__ = 1; i__ <= 4; ++i__) {
	    ik = jk;
	    for (j = 1; j <= 5; ++j) {
		if (ik <= 0) {
		    goto L530;
		}
		av2[ik + i__ * av2_dim1] = av1[ik + j * av1_dim1];
		--ik;
/* L520: */
	    }
L530:
	    ++jk;
/* L540: */
	}
	jper = 1;
/*  if one of the non-zero elements of the new row corresponds to one of */
/*  the b-splines n(j;v),j=nv7+1,...,nv4, we take account of condition */
/*  (2) for setting up this row of (avv). the row is stored in h1( the */
/*  part with respect to av1) and h2 (the part with respect to av2). */
L550:
	for (i__ = 1; i__ <= 4; ++i__) {
	    h1[i__ - 1] = (float)0.;
	    h2[i__ - 1] = (float)0.;
/* L560: */
	}
	h1[4] = (float)0.;
	j = nrold - nv11;
	for (i__ = 1; i__ <= 5; ++i__) {
	    ++j;
	    l0 = j;
L570:
	    l1 = l0 - 4;
	    if (l1 <= 0) {
		goto L590;
	    }
	    if (l1 <= nv11) {
		goto L580;
	    }
	    l0 = l1 - nv11;
	    goto L570;
L580:
	    h1[l1 - 1] = h__[i__ - 1];
	    goto L600;
L590:
	    h2[l0 - 1] += h__[i__ - 1];
L600:
	    ;
	}
/*  rotate the new row of (avv) into triangle. */
	if (nv11 <= 0) {
	    goto L670;
	}
/*  rotations with the rows 1,2,...,nv11 of (avv). */
	i__2 = nv11;
	for (j = 1; j <= i__2; ++j) {
	    piv = h1[0];
/* Computing MIN */
	    i__3 = nv11 - j;
	    i2 = min(i__3,4);
	    if (piv == (float)0.) {
		goto L640;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &av1[j + av1_dim1], &co, &si);
/*  apply that transformation to the columns of matrix g. */
	    ic = j;
	    i__3 = nuu;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		fprota_(&co, &si, &right[i__], &c__[ic]);
		ic += nv7;
/* L610: */
	    }
/*  apply that transformation to the rows of (avv) with respect to av2. */
	    for (i__ = 1; i__ <= 4; ++i__) {
		fprota_(&co, &si, &h2[i__ - 1], &av2[j + i__ * av2_dim1]);
/* L620: */
	    }
/*  apply that transformation to the rows of (avv) with respect to av1. */
	    if (i2 == 0) {
		goto L670;
	    }
	    i__3 = i2;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		i1 = i__ + 1;
		fprota_(&co, &si, &h1[i1 - 1], &av1[j + i1 * av1_dim1]);
/* L630: */
	    }
L640:
	    i__3 = i2;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h1[i__ - 1] = h1[i__];
/* L650: */
	    }
	    h1[i2] = (float)0.;
/* L660: */
	}
/*  rotations with the rows nv11+1,...,nv7 of avv. */
L670:
	for (j = 1; j <= 4; ++j) {
	    ij = nv11 + j;
	    if (ij <= 0) {
		goto L700;
	    }
	    piv = h2[j - 1];
	    if (piv == (float)0.) {
		goto L700;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &av2[ij + j * av2_dim1], &co, &si);
/*  apply that transformation to the columns of matrix g. */
	    ic = ij;
	    i__2 = nuu;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		fprota_(&co, &si, &right[i__], &c__[ic]);
		ic += nv7;
/* L680: */
	    }
	    if (j == 4) {
		goto L700;
	    }
/*  apply that transformation to the rows of (avv) with respect to av2. */
	    j1 = j + 1;
	    for (i__ = j1; i__ <= 4; ++i__) {
		fprota_(&co, &si, &h2[i__ - 1], &av2[ij + i__ * av2_dim1]);
/* L690: */
	    }
L700:
	    ;
	}
/* we update the sum of squared residuals */
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing 2nd power */
	    r__1 = right[i__];
	    *sq += r__1 * r__1;
/* L705: */
	}
	goto L750;
/*  rotation into triangle of the new row of (avv), in case the elements */
/*  corresponding to the b-splines n(j;v),j=nv7+1,...,nv4 are all zero. */
L710:
	irot = nrold;
	for (i__ = 1; i__ <= 5; ++i__) {
	    ++irot;
	    piv = h__[i__ - 1];
	    if (piv == (float)0.) {
		goto L740;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &av1[irot + av1_dim1], &co, &si);
/*  apply that transformation to the columns of matrix g. */
	    ic = irot;
	    i__2 = nuu;
	    for (j = 1; j <= i__2; ++j) {
		fprota_(&co, &si, &right[j], &c__[ic]);
		ic += nv7;
/* L720: */
	    }
/*  apply that transformation to the rows of (avv). */
	    if (i__ == 5) {
		goto L740;
	    }
	    i2 = 1;
	    i3 = i__ + 1;
	    for (j = i3; j <= 5; ++j) {
		++i2;
		fprota_(&co, &si, &h__[j - 1], &av1[irot + i2 * av1_dim1]);
/* L730: */
	    }
L740:
	    ;
	}
/* we update the sum of squared residuals */
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing 2nd power */
	    r__1 = right[i__];
	    *sq += r__1 * r__1;
/* L745: */
	}
L750:
	if (nrold == number) {
	    goto L770;
	}
L760:
	++nrold;
	goto L450;
L770:
	;
    }
/*  test whether the b-spline coefficients must be determined. */
    if (*iback != 0) {
	return 0;
    }
/*  backward substitution to obtain the b-spline coefficients as the */
/*  solution of the linear system    (rv) (cr) (ru)' = h. */
/*  first step: solve the system  (rv) (c1) = h. */
    k = 1;
    i__1 = nuu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpbacp_(&av1[av1_offset], &av2[av2_offset], &c__[k], &nv7, &c__4, &
		c__[k], &c__5, nv);
	k += nv7;
/* L780: */
    }
/*  second step: solve the system  (cr) (ru)' = (c1). */
    k = 0;
    i__1 = nv7;
    for (j = 1; j <= i__1; ++j) {
	++k;
	l = k;
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    right[i__] = c__[l];
	    l += nv7;
/* L790: */
	}
	fpback_(&au[au_offset], &right[1], &nuu, &c__5, &right[1], nu);
	l = k;
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    c__[l] = right[i__];
	    l += nv7;
/* L795: */
	}
/* L800: */
    }
/*  calculate from the conditions (2)-(3)-(4), the remaining b-spline */
/*  coefficients. */
    ncof = nu4 * nv4;
    i__ = nv4;
    j = 0;
    i__1 = nv4;
    for (l = 1; l <= i__1; ++l) {
	q[l] = dz1;
/* L805: */
    }
    if (*iop0 == 0) {
	goto L815;
    }
    i__1 = nv4;
    for (l = 1; l <= i__1; ++l) {
	++i__;
	q[i__] = cc[l];
/* L810: */
    }
L815:
    if (nuu == 0) {
	goto L850;
    }
    i__1 = nuu;
    for (l = 1; l <= i__1; ++l) {
	ii = i__;
	i__2 = nv7;
	for (k = 1; k <= i__2; ++k) {
	    ++i__;
	    ++j;
	    q[i__] = c__[j];
/* L820: */
	}
	for (k = 1; k <= 3; ++k) {
	    ++ii;
	    ++i__;
	    q[i__] = q[ii];
/* L830: */
	}
/* L840: */
    }
L850:
    if (*iop1 == 0) {
	goto L870;
    }
    i__1 = nv4;
    for (l = 1; l <= i__1; ++l) {
	++i__;
	q[i__] = (float)0.;
/* L860: */
    }
L870:
    i__1 = ncof;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = q[i__];
/* L880: */
    }
/*  calculate the quantities */
/*    res(i,j) = (z(i,j) - s(u(i),v(j)))**2 , i=1,2,..,mu;j=1,2,..,mv */
/*    fp = sumi=1,mu(sumj=1,mv(res(i,j))) */
/*    fpu(r) = sum''i(sumj=1,mv(res(i,j))) , r=1,2,...,nu-7 */
/*                  tu(r+3) <= u(i) <= tu(r+4) */
/*    fpv(r) = sumi=1,mu(sum''j(res(i,j))) , r=1,2,...,nv-7 */
/*                  tv(r+3) <= v(j) <= tv(r+4) */
    *fp = (float)0.;
    i__1 = *nu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpu[i__] = (float)0.;
/* L890: */
    }
    i__1 = *nv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpv[i__] = (float)0.;
/* L900: */
    }
    iz = 0;
    nroldu = 0;
/*  main loop for the different grid points. */
    i__1 = *mu;
    for (i1 = 1; i1 <= i__1; ++i1) {
	numu = nru[i1];
	numu1 = numu + 1;
	nroldv = 0;
	i__2 = *mv;
	for (i2 = 1; i2 <= i__2; ++i2) {
	    numv = nrv[i2];
	    numv1 = numv + 1;
	    ++iz;
/*  evaluate s(u,v) at the current grid point by making the sum of the */
/*  cross products of the non-zero b-splines at (u,v), multiplied with */
/*  the appropiate b-spline coefficients. */
	    term = (float)0.;
	    k1 = numu * nv4 + numv;
	    for (l1 = 1; l1 <= 4; ++l1) {
		k2 = k1;
		fac = spu[i1 + l1 * spu_dim1];
		for (l2 = 1; l2 <= 4; ++l2) {
		    ++k2;
		    term += fac * spv[i2 + l2 * spv_dim1] * c__[k2];
/* L910: */
		}
		k1 += nv4;
/* L920: */
	    }
/*  calculate the squared residual at the current grid point. */
/* Computing 2nd power */
	    r__1 = z__[iz] - term;
	    term = r__1 * r__1;
/*  adjust the different parameters. */
	    *fp += term;
	    fpu[numu1] += term;
	    fpv[numv1] += term;
	    fac = term * half;
	    if (numv == nroldv) {
		goto L930;
	    }
	    fpv[numv1] -= fac;
	    fpv[numv] += fac;
L930:
	    nroldv = numv;
	    if (numu == nroldu) {
		goto L940;
	    }
	    fpu[numu1] -= fac;
	    fpu[numu] += fac;
L940:
	    ;
	}
	nroldu = numu;
/* L950: */
    }
    return 0;
} /* fpgrdi_ */

/* Subroutine */ int fpgrpa_(ifsu, ifsv, ifbu, ifbv, idim, ipar, u, mu, v, mv,
	 z__, mz, tu, nu, tv, nv, p, c__, nc, fp, fpu, fpv, mm, mvnu, spu, 
	spv, right, q, au, au1, av, av1, bu, bv, nru, nrv)
integer *ifsu, *ifsv, *ifbu, *ifbv, *idim, *ipar;
real *u;
integer *mu;
real *v;
integer *mv;
real *z__;
integer *mz;
real *tu;
integer *nu;
real *tv;
integer *nv;
real *p, *c__;
integer *nc;
real *fp, *fpu, *fpv;
integer *mm, *mvnu;
real *spu, *spv, *right, *q, *au, *au1, *av, *av1, *bu, *bv;
integer *nru, *nrv;
{
    /* System generated locals */
    integer spu_dim1, spu_offset, spv_dim1, spv_offset, au_dim1, au_offset, 
	    au1_dim1, au1_offset, av_dim1, av_offset, av1_dim1, av1_offset, 
	    bu_dim1, bu_offset, bv_dim1, bv_offset, i__1, i__2, i__3;
    real r__1;

    /* Local variables */
    static real half;
    static integer ncof;
    static real term;
    static integer numu, numv, numu1, numv1;
    static real h__[5];
    static integer i__, j, k, l;
    static real value;
    static integer i1, i2, k1, k2, l1, l2, k0, id, ii, n33;
    extern /* Subroutine */ int fpback_();
    static integer it;
    extern /* Subroutine */ int fpbacp_();
    static integer iz, jz;
    extern /* Subroutine */ int fpdisc_(), fpbspl_();
    static integer number;
    extern /* Subroutine */ int fptrpe_();
    static integer nroldu, nroldv;
    extern /* Subroutine */ int fptrnp_();
    static integer nu4, nv4, nu7, nu8, nv7, nv8;
    static real fac, arg;
    static integer nmd;
    static real one;
    static integer muu, mvv, nuu, nvv;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..subroutine references.. */
/*    fpback,fpbspl,fpdisc,fpbacp,fptrnp,fptrpe */
/*  .. */
/*  let */
/*               |   (spu)    |            |   (spv)    | */
/*        (au) = | ---------- |     (av) = | ---------- | */
/*               | (1/p) (bu) |            | (1/p) (bv) | */

/*                                | z  ' 0 | */
/*                            q = | ------ | */
/*                                | 0  ' 0 | */

/*  with c      : the (nu-4) x (nv-4) matrix which contains the b-spline */
/*                coefficients. */
/*       z      : the mu x mv matrix which contains the function values. */
/*       spu,spv: the mu x (nu-4), resp. mv x (nv-4) observation matrices */
/*                according to the least-squares problems in the u-,resp. */
/*                v-direction. */
/*       bu,bv  : the (nu-7) x (nu-4),resp. (nv-7) x (nv-4) matrices */
/*                containing the discontinuity jumps of the derivatives */
/*                of the b-splines in the u-,resp.v-variable at the knots */
/*  the b-spline coefficients of the smoothing spline are then calculated */
/*  as the least-squares solution of the following over-determined linear */
/*  system of equations */

/*    (1)  (av) c (au)' = q */

/*  subject to the constraints */

/*    (2)  c(nu-3+i,j) = c(i,j), i=1,2,3 ; j=1,2,...,nv-4 */
/*            if(ipar(1).ne.0) */

/*    (3)  c(i,nv-3+j) = c(i,j), j=1,2,3 ; i=1,2,...,nu-4 */
/*            if(ipar(2).ne.0) */

/*  set constants */
    /* Parameter adjustments */
    --ipar;
    --nru;
    spu_dim1 = *mu;
    spu_offset = 1 + spu_dim1 * 1;
    spu -= spu_offset;
    --u;
    --nrv;
    spv_dim1 = *mv;
    spv_offset = 1 + spv_dim1 * 1;
    spv -= spv_offset;
    --v;
    --z__;
    bu_dim1 = *nu;
    bu_offset = 1 + bu_dim1 * 1;
    bu -= bu_offset;
    au1_dim1 = *nu;
    au1_offset = 1 + au1_dim1 * 1;
    au1 -= au1_offset;
    au_dim1 = *nu;
    au_offset = 1 + au_dim1 * 1;
    au -= au_offset;
    --fpu;
    --tu;
    bv_dim1 = *nv;
    bv_offset = 1 + bv_dim1 * 1;
    bv -= bv_offset;
    av1_dim1 = *nv;
    av1_offset = 1 + av1_dim1 * 1;
    av1 -= av1_offset;
    av_dim1 = *nv;
    av_offset = 1 + av_dim1 * 1;
    av -= av_offset;
    --fpv;
    --tv;
    --c__;
    --right;
    --q;

    /* Function Body */
    one = (float)1.;
    half = (float).5;
/*  initialization */
    nu4 = *nu - 4;
    nu7 = *nu - 7;
    nu8 = *nu - 8;
    nv4 = *nv - 4;
    nv7 = *nv - 7;
    nv8 = *nv - 8;
    muu = *mu;
    if (ipar[1] != 0) {
	muu = *mu - 1;
    }
    mvv = *mv;
    if (ipar[2] != 0) {
	mvv = *mv - 1;
    }
/*  it depends on the value of the flags ifsu,ifsv,ifbu  and ibvand */
/*  on the value of p whether the matrices (spu), (spv), (bu) and (bv) */
/*  still must be determined. */
    if (*ifsu != 0) {
	goto L50;
    }
/*  calculate the non-zero elements of the matrix (spu) which is the ob- */
/*  servation matrix according to the least-squares spline approximation */
/*  problem in the u-direction. */
    l = 4;
    l1 = 5;
    number = 0;
    i__1 = muu;
    for (it = 1; it <= i__1; ++it) {
	arg = u[it];
L10:
	if (arg < tu[l1] || l == nu4) {
	    goto L20;
	}
	l = l1;
	l1 = l + 1;
	++number;
	goto L10;
L20:
	fpbspl_(&tu[1], nu, &c__3, &arg, &l, h__);
	for (i__ = 1; i__ <= 4; ++i__) {
	    spu[it + i__ * spu_dim1] = h__[i__ - 1];
/* L30: */
	}
	nru[it] = number;
/* L40: */
    }
    *ifsu = 1;
/*  calculate the non-zero elements of the matrix (spv) which is the ob- */
/*  servation matrix according to the least-squares spline approximation */
/*  problem in the v-direction. */
L50:
    if (*ifsv != 0) {
	goto L100;
    }
    l = 4;
    l1 = 5;
    number = 0;
    i__1 = mvv;
    for (it = 1; it <= i__1; ++it) {
	arg = v[it];
L60:
	if (arg < tv[l1] || l == nv4) {
	    goto L70;
	}
	l = l1;
	l1 = l + 1;
	++number;
	goto L60;
L70:
	fpbspl_(&tv[1], nv, &c__3, &arg, &l, h__);
	for (i__ = 1; i__ <= 4; ++i__) {
	    spv[it + i__ * spv_dim1] = h__[i__ - 1];
/* L80: */
	}
	nrv[it] = number;
/* L90: */
    }
    *ifsv = 1;
L100:
    if (*p <= (float)0.) {
	goto L150;
    }
/*  calculate the non-zero elements of the matrix (bu). */
    if (*ifbu != 0 || nu8 == 0) {
	goto L110;
    }
    fpdisc_(&tu[1], nu, &c__5, &bu[bu_offset], nu);
    *ifbu = 1;
/*  calculate the non-zero elements of the matrix (bv). */
L110:
    if (*ifbv != 0 || nv8 == 0) {
	goto L150;
    }
    fpdisc_(&tv[1], nv, &c__5, &bv[bv_offset], nv);
    *ifbv = 1;
/*  substituting (2)  and (3) into (1), we obtain the overdetermined */
/*  system */
/*         (4)  (avv) (cr) (auu)' = (qq) */
/*  from which the nuu*nvv remaining coefficients */
/*         c(i,j) , i=1,...,nu-4-3*ipar(1) ; j=1,...,nv-4-3*ipar(2) , */
/*  the elements of (cr), are then determined in the least-squares sense. */
/*  we first determine the matrices (auu) and (qq). then we reduce the */
/*  matrix (auu) to upper triangular form (ru) using givens rotations. */
/*  we apply the same transformations to the rows of matrix qq to obtain */
/*  the (mv) x nuu matrix g. */
/*  we store matrix (ru) into au (and au1 if ipar(1)=1) and g into q. */
L150:
    if (ipar[1] != 0) {
	goto L160;
    }
    nuu = nu4;
    fptrnp_(mu, mv, idim, nu, &nru[1], &spu[spu_offset], p, &bu[bu_offset], &
	    z__[1], &au[au_offset], &q[1], &right[1]);
    goto L180;
L160:
    nuu = nu7;
    fptrpe_(mu, mv, idim, nu, &nru[1], &spu[spu_offset], p, &bu[bu_offset], &
	    z__[1], &au[au_offset], &au1[au1_offset], &q[1], &right[1]);
/*  we determine the matrix (avv) and then we reduce this matrix to */
/*  upper triangular form (rv) using givens rotations. */
/*  we apply the same transformations to the columns of matrix */
/*  g to obtain the (nvv) x (nuu) matrix h. */
/*  we store matrix (rv) into av (and av1 if ipar(2)=1) and h into c. */
L180:
    if (ipar[2] != 0) {
	goto L190;
    }
    nvv = nv4;
    fptrnp_(mv, &nuu, idim, nv, &nrv[1], &spv[spv_offset], p, &bv[bv_offset], 
	    &q[1], &av[av_offset], &c__[1], &right[1]);
    goto L200;
L190:
    nvv = nv7;
    fptrpe_(mv, &nuu, idim, nv, &nrv[1], &spv[spv_offset], p, &bv[bv_offset], 
	    &q[1], &av[av_offset], &av1[av1_offset], &c__[1], &right[1]);
/*  backward substitution to obtain the b-spline coefficients as the */
/*  solution of the linear system    (rv) (cr) (ru)' = h. */
/*  first step: solve the system  (rv) (c1) = h. */
L200:
    ncof = nuu * nvv;
    k = 1;
    if (ipar[2] != 0) {
	goto L240;
    }
    i__1 = *idim;
    for (ii = 1; ii <= i__1; ++ii) {
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    fpback_(&av[av_offset], &c__[k], &nvv, &c__5, &c__[k], nv);
	    k += nvv;
/* L220: */
	}
    }
    goto L300;
L240:
    i__2 = *idim;
    for (ii = 1; ii <= i__2; ++ii) {
	i__1 = nuu;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    fpbacp_(&av[av_offset], &av1[av1_offset], &c__[k], &nvv, &c__4, &
		    c__[k], &c__5, nv);
	    k += nvv;
/* L260: */
	}
    }
/*  second step: solve the system  (cr) (ru)' = (c1). */
L300:
    if (ipar[1] != 0) {
	goto L400;
    }
    i__1 = *idim;
    for (ii = 1; ii <= i__1; ++ii) {
	k = (ii - 1) * ncof;
	i__2 = nvv;
	for (j = 1; j <= i__2; ++j) {
	    ++k;
	    l = k;
	    i__3 = nuu;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		right[i__] = c__[l];
		l += nvv;
/* L320: */
	    }
	    fpback_(&au[au_offset], &right[1], &nuu, &c__5, &right[1], nu);
	    l = k;
	    i__3 = nuu;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		c__[l] = right[i__];
		l += nvv;
/* L340: */
	    }
/* L360: */
	}
    }
    goto L500;
L400:
    i__2 = *idim;
    for (ii = 1; ii <= i__2; ++ii) {
	k = (ii - 1) * ncof;
	i__1 = nvv;
	for (j = 1; j <= i__1; ++j) {
	    ++k;
	    l = k;
	    i__3 = nuu;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		right[i__] = c__[l];
		l += nvv;
/* L420: */
	    }
	    fpbacp_(&au[au_offset], &au1[au1_offset], &right[1], &nuu, &c__4, 
		    &right[1], &c__5, nu);
	    l = k;
	    i__3 = nuu;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		c__[l] = right[i__];
		l += nvv;
/* L440: */
	    }
/* L460: */
	}
    }
/*  calculate from the conditions (2)-(3), the remaining b-spline */
/*  coefficients. */
L500:
    if (ipar[2] == 0) {
	goto L600;
    }
    i__ = 0;
    j = 0;
    i__1 = *idim;
    for (id = 1; id <= i__1; ++id) {
	i__2 = nuu;
	for (l = 1; l <= i__2; ++l) {
	    ii = i__;
	    i__3 = nvv;
	    for (k = 1; k <= i__3; ++k) {
		++i__;
		++j;
		q[i__] = c__[j];
/* L520: */
	    }
	    for (k = 1; k <= 3; ++k) {
		++ii;
		++i__;
		q[i__] = q[ii];
/* L540: */
	    }
/* L560: */
	}
    }
    ncof = nv4 * nuu;
    nmd = ncof * *idim;
    i__2 = nmd;
    for (i__ = 1; i__ <= i__2; ++i__) {
	c__[i__] = q[i__];
/* L580: */
    }
L600:
    if (ipar[1] == 0) {
	goto L700;
    }
    i__ = 0;
    j = 0;
    n33 = nv4 * 3;
    i__2 = *idim;
    for (id = 1; id <= i__2; ++id) {
	ii = i__;
	i__1 = ncof;
	for (k = 1; k <= i__1; ++k) {
	    ++i__;
	    ++j;
	    q[i__] = c__[j];
/* L620: */
	}
	i__1 = n33;
	for (k = 1; k <= i__1; ++k) {
	    ++ii;
	    ++i__;
	    q[i__] = q[ii];
/* L640: */
	}
/* L660: */
    }
    ncof = nv4 * nu4;
    nmd = ncof * *idim;
    i__2 = nmd;
    for (i__ = 1; i__ <= i__2; ++i__) {
	c__[i__] = q[i__];
/* L680: */
    }
/*  calculate the quantities */
/*    res(i,j) = (z(i,j) - s(u(i),v(j)))**2 , i=1,2,..,mu;j=1,2,..,mv */
/*    fp = sumi=1,mu(sumj=1,mv(res(i,j))) */
/*    fpu(r) = sum''i(sumj=1,mv(res(i,j))) , r=1,2,...,nu-7 */
/*                  tu(r+3) <= u(i) <= tu(r+4) */
/*    fpv(r) = sumi=1,mu(sum''j(res(i,j))) , r=1,2,...,nv-7 */
/*                  tv(r+3) <= v(j) <= tv(r+4) */
L700:
    *fp = (float)0.;
    i__2 = *nu;
    for (i__ = 1; i__ <= i__2; ++i__) {
	fpu[i__] = (float)0.;
/* L720: */
    }
    i__2 = *nv;
    for (i__ = 1; i__ <= i__2; ++i__) {
	fpv[i__] = (float)0.;
/* L740: */
    }
    nroldu = 0;
/*  main loop for the different grid points. */
    i__2 = muu;
    for (i1 = 1; i1 <= i__2; ++i1) {
	numu = nru[i1];
	numu1 = numu + 1;
	nroldv = 0;
	iz = (i1 - 1) * *mv;
	i__1 = mvv;
	for (i2 = 1; i2 <= i__1; ++i2) {
	    numv = nrv[i2];
	    numv1 = numv + 1;
	    ++iz;
/*  evaluate s(u,v) at the current grid point by making the sum of the */
/*  cross products of the non-zero b-splines at (u,v), multiplied with */
/*  the appropiate b-spline coefficients. */
	    term = (float)0.;
	    k0 = numu * nv4 + numv;
	    jz = iz;
	    i__3 = *idim;
	    for (id = 1; id <= i__3; ++id) {
		k1 = k0;
		value = (float)0.;
		for (l1 = 1; l1 <= 4; ++l1) {
		    k2 = k1;
		    fac = spu[i1 + l1 * spu_dim1];
		    for (l2 = 1; l2 <= 4; ++l2) {
			++k2;
			value += fac * spv[i2 + l2 * spv_dim1] * c__[k2];
/* L760: */
		    }
		    k1 += nv4;
/* L780: */
		}
/*  calculate the squared residual at the current grid point. */
/* Computing 2nd power */
		r__1 = z__[jz] - value;
		term += r__1 * r__1;
		jz += *mz;
		k0 += ncof;
/* L800: */
	    }
/*  adjust the different parameters. */
	    *fp += term;
	    fpu[numu1] += term;
	    fpv[numv1] += term;
	    fac = term * half;
	    if (numv == nroldv) {
		goto L820;
	    }
	    fpv[numv1] -= fac;
	    fpv[numv] += fac;
L820:
	    nroldv = numv;
	    if (numu == nroldu) {
		goto L840;
	    }
	    fpu[numu1] -= fac;
	    fpu[numu] += fac;
L840:
	    ;
	}
	nroldu = numu;
/* L860: */
    }
    return 0;
} /* fpgrpa_ */

/* Subroutine */ int fpgrre_(ifsx, ifsy, ifbx, ifby, x, mx, y, my, z__, mz, 
	kx, ky, tx, nx, ty, ny, p, c__, nc, fp, fpx, fpy, mm, mynx, kx1, kx2, 
	ky1, ky2, spx, spy, right, q, ax, ay, bx, by, nrx, nry)
integer *ifsx, *ifsy, *ifbx, *ifby;
real *x;
integer *mx;
real *y;
integer *my;
real *z__;
integer *mz, *kx, *ky;
real *tx;
integer *nx;
real *ty;
integer *ny;
real *p, *c__;
integer *nc;
real *fp, *fpx, *fpy;
integer *mm, *mynx, *kx1, *kx2, *ky1, *ky2;
real *spx, *spy, *right, *q, *ax, *ay, *bx, *by;
integer *nrx, *nry;
{
    /* System generated locals */
    integer spx_dim1, spx_offset, spy_dim1, spy_offset, ax_dim1, ax_offset, 
	    bx_dim1, bx_offset, ay_dim1, ay_offset, by_dim1, by_offset, i__1, 
	    i__2, i__3, i__4;
    real r__1;

    /* Local variables */
    static real half;
    static integer ncof;
    static real term, pinv;
    static integer irot, numx, numy, numx1, i__, j, k, l, numy1;
    static real h__[7];
    static integer nrold, i1, i2, i3, k1, k2, l1, l2, n1, ic;
    extern /* Subroutine */ int fpback_();
    static integer iq, it, ibandx, ibandy, iz;
    extern /* Subroutine */ int fpdisc_(), fpbspl_();
    static integer number;
    extern /* Subroutine */ int fprota_(), fpgivs_();
    static integer nroldx, nroldy;
    static real fac, arg, one, cos__, sin__, piv;
    static integer nk1x, nk1y;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..subroutine references.. */
/*    fpback,fpbspl,fpgivs,fpdisc,fprota */
/*  .. */
/*  the b-spline coefficients of the smoothing spline are calculated as */
/*  the least-squares solution of the over-determined linear system of */
/*  equations  (ay) c (ax)' = q       where */

/*               |   (spx)    |            |   (spy)    | */
/*        (ax) = | ---------- |     (ay) = | ---------- | */
/*               | (1/p) (bx) |            | (1/p) (by) | */

/*                                | z  ' 0 | */
/*                            q = | ------ | */
/*                                | 0  ' 0 | */

/*  with c      : the (ny-ky-1) x (nx-kx-1) matrix which contains the */
/*                b-spline coefficients. */
/*       z      : the my x mx matrix which contains the function values. */
/*       spx,spy: the mx x (nx-kx-1) and  my x (ny-ky-1) observation */
/*                matrices according to the least-squares problems in */
/*                the x- and y-direction. */
/*       bx,by  : the (nx-2*kx-1) x (nx-kx-1) and (ny-2*ky-1) x (ny-ky-1) */
/*                matrices which contain the discontinuity jumps of the */
/*                derivatives of the b-splines in the x- and y-direction. */
    /* Parameter adjustments */
    --nrx;
    --x;
    --nry;
    --y;
    --z__;
    --fpx;
    --tx;
    --fpy;
    --ty;
    --c__;
    --right;
    --q;
    spx_dim1 = *mx;
    spx_offset = 1 + spx_dim1 * 1;
    spx -= spx_offset;
    bx_dim1 = *nx;
    bx_offset = 1 + bx_dim1 * 1;
    bx -= bx_offset;
    ax_dim1 = *nx;
    ax_offset = 1 + ax_dim1 * 1;
    ax -= ax_offset;
    spy_dim1 = *my;
    spy_offset = 1 + spy_dim1 * 1;
    spy -= spy_offset;
    by_dim1 = *ny;
    by_offset = 1 + by_dim1 * 1;
    by -= by_offset;
    ay_dim1 = *ny;
    ay_offset = 1 + ay_dim1 * 1;
    ay -= ay_offset;

    /* Function Body */
    one = (float)1.;
    half = (float).5;
    nk1x = *nx - *kx1;
    nk1y = *ny - *ky1;
    if (*p > (float)0.) {
	pinv = one / *p;
    }
/*  it depends on the value of the flags ifsx,ifsy,ifbx and ifby and on */
/*  the value of p whether the matrices (spx),(spy),(bx) and (by) still */
/*  must be determined. */
    if (*ifsx != 0) {
	goto L50;
    }
/*  calculate the non-zero elements of the matrix (spx) which is the */
/*  observation matrix according to the least-squares spline approximat- */
/*  ion problem in the x-direction. */
    l = *kx1;
    l1 = *kx2;
    number = 0;
    i__1 = *mx;
    for (it = 1; it <= i__1; ++it) {
	arg = x[it];
L10:
	if (arg < tx[l1] || l == nk1x) {
	    goto L20;
	}
	l = l1;
	l1 = l + 1;
	++number;
	goto L10;
L20:
	fpbspl_(&tx[1], nx, kx, &arg, &l, h__);
	i__2 = *kx1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    spx[it + i__ * spx_dim1] = h__[i__ - 1];
/* L30: */
	}
	nrx[it] = number;
/* L40: */
    }
    *ifsx = 1;
L50:
    if (*ifsy != 0) {
	goto L100;
    }
/*  calculate the non-zero elements of the matrix (spy) which is the */
/*  observation matrix according to the least-squares spline approximat- */
/*  ion problem in the y-direction. */
    l = *ky1;
    l1 = *ky2;
    number = 0;
    i__1 = *my;
    for (it = 1; it <= i__1; ++it) {
	arg = y[it];
L60:
	if (arg < ty[l1] || l == nk1y) {
	    goto L70;
	}
	l = l1;
	l1 = l + 1;
	++number;
	goto L60;
L70:
	fpbspl_(&ty[1], ny, ky, &arg, &l, h__);
	i__2 = *ky1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    spy[it + i__ * spy_dim1] = h__[i__ - 1];
/* L80: */
	}
	nry[it] = number;
/* L90: */
    }
    *ifsy = 1;
L100:
    if (*p <= (float)0.) {
	goto L120;
    }
/*  calculate the non-zero elements of the matrix (bx). */
    if (*ifbx != 0 || *nx == *kx1 << 1) {
	goto L110;
    }
    fpdisc_(&tx[1], nx, kx2, &bx[bx_offset], nx);
    *ifbx = 1;
/*  calculate the non-zero elements of the matrix (by). */
L110:
    if (*ifby != 0 || *ny == *ky1 << 1) {
	goto L120;
    }
    fpdisc_(&ty[1], ny, ky2, &by[by_offset], ny);
    *ifby = 1;
/*  reduce the matrix (ax) to upper triangular form (rx) using givens */
/*  rotations. apply the same transformations to the rows of matrix q */
/*  to obtain the my x (nx-kx-1) matrix g. */
/*  store matrix (rx) into (ax) and g into q. */
L120:
    l = *my * nk1x;
/*  initialization. */
    i__1 = l;
    for (i__ = 1; i__ <= i__1; ++i__) {
	q[i__] = (float)0.;
/* L130: */
    }
    i__1 = nk1x;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *kx2;
	for (j = 1; j <= i__2; ++j) {
	    ax[i__ + j * ax_dim1] = (float)0.;
/* L140: */
	}
    }
    l = 0;
    nrold = 0;
/*  ibandx denotes the bandwidth of the matrices (ax) and (rx). */
    ibandx = *kx1;
    i__2 = *mx;
    for (it = 1; it <= i__2; ++it) {
	number = nrx[it];
L150:
	if (nrold == number) {
	    goto L180;
	}
	if (*p <= (float)0.) {
	    goto L260;
	}
	ibandx = *kx2;
/*  fetch a new row of matrix (bx). */
	n1 = nrold + 1;
	i__1 = *kx2;
	for (j = 1; j <= i__1; ++j) {
	    h__[j - 1] = bx[n1 + j * bx_dim1] * pinv;
/* L160: */
	}
/*  find the appropriate column of q. */
	i__1 = *my;
	for (j = 1; j <= i__1; ++j) {
	    right[j] = (float)0.;
/* L170: */
	}
	irot = nrold;
	goto L210;
/*  fetch a new row of matrix (spx). */
L180:
	h__[ibandx - 1] = (float)0.;
	i__1 = *kx1;
	for (j = 1; j <= i__1; ++j) {
	    h__[j - 1] = spx[it + j * spx_dim1];
/* L190: */
	}
/*  find the appropriate column of q. */
	i__1 = *my;
	for (j = 1; j <= i__1; ++j) {
	    ++l;
	    right[j] = z__[l];
/* L200: */
	}
	irot = number;
/*  rotate the new row of matrix (ax) into triangle. */
L210:
	i__1 = ibandx;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    ++irot;
	    piv = h__[i__ - 1];
	    if (piv == (float)0.) {
		goto L240;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &ax[irot + ax_dim1], &cos__, &sin__);
/*  apply that transformation to the rows of matrix q. */
	    iq = (irot - 1) * *my;
	    i__3 = *my;
	    for (j = 1; j <= i__3; ++j) {
		++iq;
		fprota_(&cos__, &sin__, &right[j], &q[iq]);
/* L220: */
	    }
/*  apply that transformation to the columns of (ax). */
	    if (i__ == ibandx) {
		goto L250;
	    }
	    i2 = 1;
	    i3 = i__ + 1;
	    i__3 = ibandx;
	    for (j = i3; j <= i__3; ++j) {
		++i2;
		fprota_(&cos__, &sin__, &h__[j - 1], &ax[irot + i2 * ax_dim1])
			;
/* L230: */
	    }
L240:
	    ;
	}
L250:
	if (nrold == number) {
	    goto L270;
	}
L260:
	++nrold;
	goto L150;
L270:
	;
    }
/*  reduce the matrix (ay) to upper triangular form (ry) using givens */
/*  rotations. apply the same transformations to the columns of matrix g */
/*  to obtain the (ny-ky-1) x (nx-kx-1) matrix h. */
/*  store matrix (ry) into (ay) and h into c. */
    ncof = nk1x * nk1y;
/*  initialization. */
    i__2 = ncof;
    for (i__ = 1; i__ <= i__2; ++i__) {
	c__[i__] = (float)0.;
/* L280: */
    }
    i__2 = nk1y;
    for (i__ = 1; i__ <= i__2; ++i__) {
	i__1 = *ky2;
	for (j = 1; j <= i__1; ++j) {
	    ay[i__ + j * ay_dim1] = (float)0.;
/* L290: */
	}
    }
    nrold = 0;
/*  ibandy denotes the bandwidth of the matrices (ay) and (ry). */
    ibandy = *ky1;
    i__1 = *my;
    for (it = 1; it <= i__1; ++it) {
	number = nry[it];
L300:
	if (nrold == number) {
	    goto L330;
	}
	if (*p <= (float)0.) {
	    goto L410;
	}
	ibandy = *ky2;
/*  fetch a new row of matrix (by). */
	n1 = nrold + 1;
	i__2 = *ky2;
	for (j = 1; j <= i__2; ++j) {
	    h__[j - 1] = by[n1 + j * by_dim1] * pinv;
/* L310: */
	}
/*  find the appropiate row of g. */
	i__2 = nk1x;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = (float)0.;
/* L320: */
	}
	irot = nrold;
	goto L360;
/*  fetch a new row of matrix (spy) */
L330:
	h__[ibandy - 1] = (float)0.;
	i__2 = *ky1;
	for (j = 1; j <= i__2; ++j) {
	    h__[j - 1] = spy[it + j * spy_dim1];
/* L340: */
	}
/*  find the appropiate row of g. */
	l = it;
	i__2 = nk1x;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = q[l];
	    l += *my;
/* L350: */
	}
	irot = number;
/*  rotate the new row of matrix (ay) into triangle. */
L360:
	i__2 = ibandy;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ++irot;
	    piv = h__[i__ - 1];
	    if (piv == (float)0.) {
		goto L390;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &ay[irot + ay_dim1], &cos__, &sin__);
/*  apply that transformation to the colums of matrix g. */
	    ic = irot;
	    i__3 = nk1x;
	    for (j = 1; j <= i__3; ++j) {
		fprota_(&cos__, &sin__, &right[j], &c__[ic]);
		ic += nk1y;
/* L370: */
	    }
/*  apply that transformation to the columns of matrix (ay). */
	    if (i__ == ibandy) {
		goto L400;
	    }
	    i2 = 1;
	    i3 = i__ + 1;
	    i__3 = ibandy;
	    for (j = i3; j <= i__3; ++j) {
		++i2;
		fprota_(&cos__, &sin__, &h__[j - 1], &ay[irot + i2 * ay_dim1])
			;
/* L380: */
	    }
L390:
	    ;
	}
L400:
	if (nrold == number) {
	    goto L420;
	}
L410:
	++nrold;
	goto L300;
L420:
	;
    }
/*  backward substitution to obtain the b-spline coefficients as the */
/*  solution of the linear system    (ry) c (rx)' = h. */
/*  first step: solve the system  (ry) (c1) = h. */
    k = 1;
    i__1 = nk1x;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpback_(&ay[ay_offset], &c__[k], &nk1y, &ibandy, &c__[k], ny);
	k += nk1y;
/* L450: */
    }
/*  second step: solve the system  c (rx)' = (c1). */
    k = 0;
    i__1 = nk1y;
    for (j = 1; j <= i__1; ++j) {
	++k;
	l = k;
	i__2 = nk1x;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    right[i__] = c__[l];
	    l += nk1y;
/* L460: */
	}
	fpback_(&ax[ax_offset], &right[1], &nk1x, &ibandx, &right[1], nx);
	l = k;
	i__2 = nk1x;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    c__[l] = right[i__];
	    l += nk1y;
/* L470: */
	}
/* L480: */
    }
/*  calculate the quantities */
/*    res(i,j) = (z(i,j) - s(x(i),y(j)))**2 , i=1,2,..,mx;j=1,2,..,my */
/*    fp = sumi=1,mx(sumj=1,my(res(i,j))) */
/*    fpx(r) = sum''i(sumj=1,my(res(i,j))) , r=1,2,...,nx-2*kx-1 */
/*                  tx(r+kx) <= x(i) <= tx(r+kx+1) */
/*    fpy(r) = sumi=1,mx(sum''j(res(i,j))) , r=1,2,...,ny-2*ky-1 */
/*                  ty(r+ky) <= y(j) <= ty(r+ky+1) */
    *fp = (float)0.;
    i__1 = *nx;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpx[i__] = (float)0.;
/* L490: */
    }
    i__1 = *ny;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpy[i__] = (float)0.;
/* L500: */
    }
    nk1y = *ny - *ky1;
    iz = 0;
    nroldx = 0;
/*  main loop for the different grid points. */
    i__1 = *mx;
    for (i1 = 1; i1 <= i__1; ++i1) {
	numx = nrx[i1];
	numx1 = numx + 1;
	nroldy = 0;
	i__2 = *my;
	for (i2 = 1; i2 <= i__2; ++i2) {
	    numy = nry[i2];
	    numy1 = numy + 1;
	    ++iz;
/*  evaluate s(x,y) at the current grid point by making the sum of the */
/*  cross products of the non-zero b-splines at (x,y), multiplied with */
/*  the appropiate b-spline coefficients. */
	    term = (float)0.;
	    k1 = numx * nk1y + numy;
	    i__3 = *kx1;
	    for (l1 = 1; l1 <= i__3; ++l1) {
		k2 = k1;
		fac = spx[i1 + l1 * spx_dim1];
		i__4 = *ky1;
		for (l2 = 1; l2 <= i__4; ++l2) {
		    ++k2;
		    term += fac * spy[i2 + l2 * spy_dim1] * c__[k2];
/* L510: */
		}
		k1 += nk1y;
/* L520: */
	    }
/*  calculate the squared residual at the current grid point. */
/* Computing 2nd power */
	    r__1 = z__[iz] - term;
	    term = r__1 * r__1;
/*  adjust the different parameters. */
	    *fp += term;
	    fpx[numx1] += term;
	    fpy[numy1] += term;
	    fac = term * half;
	    if (numy == nroldy) {
		goto L530;
	    }
	    fpy[numy1] -= fac;
	    fpy[numy] += fac;
L530:
	    nroldy = numy;
	    if (numx == nroldx) {
		goto L540;
	    }
	    fpx[numx1] -= fac;
	    fpx[numx] += fac;
L540:
	    ;
	}
	nroldx = numx;
/* L550: */
    }
    return 0;
} /* fpgrre_ */

/* Subroutine */ int fpgrsp_(ifsu, ifsv, ifbu, ifbv, iback, u, mu, v, mv, r__,
	 mr, dr, iop0, iop1, tu, nu, tv, nv, p, c__, nc, sq, fp, fpu, fpv, mm,
	 mvnu, spu, spv, right, q, au, av1, av2, bu, bv, a0, a1, b0, b1, c0, 
	c1, cosi, nru, nrv)
integer *ifsu, *ifsv, *ifbu, *ifbv, *iback;
real *u;
integer *mu;
real *v;
integer *mv;
real *r__;
integer *mr;
real *dr;
integer *iop0, *iop1;
real *tu;
integer *nu;
real *tv;
integer *nv;
real *p, *c__;
integer *nc;
real *sq, *fp, *fpu, *fpv;
integer *mm, *mvnu;
real *spu, *spv, *right, *q, *au, *av1, *av2, *bu, *bv, *a0, *a1, *b0, *b1, *
	c0, *c1, *cosi;
integer *nru, *nrv;
{
    /* System generated locals */
    integer spu_dim1, spu_offset, spv_dim1, spv_offset, au_dim1, au_offset, 
	    av1_dim1, av1_offset, av2_dim1, av2_offset, bu_dim1, bu_offset, 
	    bv_dim1, bv_offset, i__1, i__2, i__3;
    real r__1;

    /* Builtin functions */
    double cos(), sin();

    /* Local variables */
    static real half;
    static integer ncof, jper;
    static real term, pinv;
    static integer irot, numu, numv, numu1, numv1;
    static real h__[5];
    static integer i__, j, k, l;
    static real three;
    static integer nrold, i0, i1, i2, i3, j0, j1, k1, k2, l0, l1, l2, n1;
    static real h1[5], h2[4];
    extern /* Subroutine */ int fpcyt1_(), fpcyt2_();
    static integer ic;
    static real co;
    static integer ii, ij, ik;
    static real si;
    static integer iq, it, ir, jj, jk;
    extern /* Subroutine */ int fpdisc_(), fpbacp_(), fpback_(), fpbspl_();
    static integer number;
    extern /* Subroutine */ int fprota_(), fpgivs_();
    static integer nroldu, nroldv, nu4, nv4, nu7, nu8, nu9, nv7, nv8;
    static real fac, dr01, dr02, dr03, arg, dr11, dr12, dr13, one;
    static integer nv11;
    static real piv, fac0, fac1;
    static integer mvv, nuu;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpback,fpbspl,fpgivs,fpcyt1,fpcyt2,fpdisc,fpbacp,fprota */
/*  .. */
/*  let */
/*               |     (spu)      |            |     (spv)      | */
/*        (au) = | -------------- |     (av) = | -------------- | */
/*               | sqrt(1/p) (bu) |            | sqrt(1/p) (bv) | */

/*                                | r  ' 0 | */
/*                            q = | ------ | */
/*                                | 0  ' 0 | */

/*  with c      : the (nu-4) x (nv-4) matrix which contains the b-spline */
/*                coefficients. */
/*       r      : the mu x mv matrix which contains the function values. */
/*       spu,spv: the mu x (nu-4), resp. mv x (nv-4) observation matrices */
/*                according to the least-squares problems in the u-,resp. */
/*                v-direction. */
/*       bu,bv  : the (nu-7) x (nu-4),resp. (nv-7) x (nv-4) matrices */
/*                containing the discontinuity jumps of the derivatives */
/*                of the b-splines in the u-,resp.v-variable at the knots */
/*  the b-spline coefficients of the smoothing spline are then calculated */
/*  as the least-squares solution of the following over-determined linear */
/*  system of equations */

/*  (1)  (av) c (au)' = q */

/*  subject to the constraints */

/*  (2)  c(i,nv-3+j) = c(i,j), j=1,2,3 ; i=1,2,...,nu-4 */

/*  (3)  if iop0 = 0  c(1,j) = dr(1) */
/*          iop0 = 1  c(1,j) = dr(1) */
/*                    c(2,j) = dr(1)+(dr(2)*cosi(1,j)+dr(3)*cosi(2,j))* */
/*                            tu(5)/3. = c0(j) , j=1,2,...nv-4 */

/*  (4)  if iop1 = 0  c(nu-4,j) = dr(4) */
/*          iop1 = 1  c(nu-4,j) = dr(4) */
/*                    c(nu-5,j) = dr(4)+(dr(5)*cosi(1,j)+dr(6)*cosi(2,j)) */
/*                                *(tu(nu-4)-tu(nu-3))/3. = c1(j) */

/*  set constants */
    /* Parameter adjustments */
    --nru;
    spu_dim1 = *mu;
    spu_offset = 1 + spu_dim1 * 1;
    spu -= spu_offset;
    --u;
    --nrv;
    a1 -= 3;
    a0 -= 3;
    spv_dim1 = *mv;
    spv_offset = 1 + spv_dim1 * 1;
    spv -= spv_offset;
    --v;
    --r__;
    --dr;
    bu_dim1 = *nu;
    bu_offset = 1 + bu_dim1 * 1;
    bu -= bu_offset;
    au_dim1 = *nu;
    au_offset = 1 + au_dim1 * 1;
    au -= au_offset;
    --fpu;
    --tu;
    cosi -= 3;
    --c1;
    --c0;
    b1 -= 3;
    b0 -= 3;
    bv_dim1 = *nv;
    bv_offset = 1 + bv_dim1 * 1;
    bv -= bv_offset;
    av2_dim1 = *nv;
    av2_offset = 1 + av2_dim1 * 1;
    av2 -= av2_offset;
    av1_dim1 = *nv;
    av1_offset = 1 + av1_dim1 * 1;
    av1 -= av1_offset;
    --fpv;
    --tv;
    --c__;
    --right;
    --q;

    /* Function Body */
    one = (float)1.;
    three = (float)3.;
    half = (float).5;
/*  initialization */
    nu4 = *nu - 4;
    nu7 = *nu - 7;
    nu8 = *nu - 8;
    nu9 = *nu - 9;
    nv4 = *nv - 4;
    nv7 = *nv - 7;
    nv8 = *nv - 8;
    nv11 = *nv - 11;
    nuu = nu4 - *iop0 - *iop1 - 2;
    if (*p > (float)0.) {
	pinv = one / *p;
    }
/*  it depends on the value of the flags ifsu,ifsv,ifbu,ifbv,iop0,iop1 */
/*  and on the value of p whether the matrices (spu), (spv), (bu), (bv), */
/*  (cosi) still must be determined. */
    if (*ifsu != 0) {
	goto L30;
    }
/*  calculate the non-zero elements of the matrix (spu) which is the ob- */
/*  servation matrix according to the least-squares spline approximation */
/*  problem in the u-direction. */
    l = 4;
    l1 = 5;
    number = 0;
    i__1 = *mu;
    for (it = 1; it <= i__1; ++it) {
	arg = u[it];
L10:
	if (arg < tu[l1] || l == nu4) {
	    goto L15;
	}
	l = l1;
	l1 = l + 1;
	++number;
	goto L10;
L15:
	fpbspl_(&tu[1], nu, &c__3, &arg, &l, h__);
	for (i__ = 1; i__ <= 4; ++i__) {
	    spu[it + i__ * spu_dim1] = h__[i__ - 1];
/* L20: */
	}
	nru[it] = number;
/* L25: */
    }
    *ifsu = 1;
/*  calculate the non-zero elements of the matrix (spv) which is the ob- */
/*  servation matrix according to the least-squares spline approximation */
/*  problem in the v-direction. */
L30:
    if (*ifsv != 0) {
	goto L85;
    }
    l = 4;
    l1 = 5;
    number = 0;
    i__1 = *mv;
    for (it = 1; it <= i__1; ++it) {
	arg = v[it];
L35:
	if (arg < tv[l1] || l == nv4) {
	    goto L40;
	}
	l = l1;
	l1 = l + 1;
	++number;
	goto L35;
L40:
	fpbspl_(&tv[1], nv, &c__3, &arg, &l, h__);
	for (i__ = 1; i__ <= 4; ++i__) {
	    spv[it + i__ * spv_dim1] = h__[i__ - 1];
/* L45: */
	}
	nrv[it] = number;
/* L50: */
    }
    *ifsv = 1;
    if (*iop0 == 0 && *iop1 == 0) {
	goto L85;
    }
/*  calculate the coefficients of the interpolating splines for cos(v) */
/*  and sin(v). */
    i__1 = nv4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	cosi[(i__ << 1) + 1] = (float)0.;
	cosi[(i__ << 1) + 2] = (float)0.;
/* L55: */
    }
    if (nv7 < 4) {
	goto L85;
    }
    i__1 = nv7;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l = i__ + 3;
	arg = tv[l];
	fpbspl_(&tv[1], nv, &c__3, &arg, &l, h__);
	for (j = 1; j <= 3; ++j) {
	    av1[i__ + j * av1_dim1] = h__[j - 1];
/* L60: */
	}
	cosi[(i__ << 1) + 1] = cos(arg);
	cosi[(i__ << 1) + 2] = sin(arg);
/* L65: */
    }
    fpcyt1_(&av1[av1_offset], &nv7, nv);
    for (j = 1; j <= 2; ++j) {
	i__1 = nv7;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    right[i__] = cosi[j + (i__ << 1)];
/* L70: */
	}
	fpcyt2_(&av1[av1_offset], &nv7, &right[1], &right[1], nv);
	i__1 = nv7;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    cosi[j + (i__ + 1 << 1)] = right[i__];
/* L75: */
	}
	cosi[j + 2] = cosi[j + (nv7 + 1 << 1)];
	cosi[j + (nv7 + 2 << 1)] = cosi[j + 4];
	cosi[j + (nv4 << 1)] = cosi[j + 6];
/* L80: */
    }
L85:
    if (*p <= (float)0.) {
	goto L150;
    }
/*  calculate the non-zero elements of the matrix (bu). */
    if (*ifbu != 0 || nu8 == 0) {
	goto L90;
    }
    fpdisc_(&tu[1], nu, &c__5, &bu[bu_offset], nu);
    *ifbu = 1;
/*  calculate the non-zero elements of the matrix (bv). */
L90:
    if (*ifbv != 0 || nv8 == 0) {
	goto L150;
    }
    fpdisc_(&tv[1], nv, &c__5, &bv[bv_offset], nv);
    *ifbv = 1;
/*  substituting (2),(3) and (4) into (1), we obtain the overdetermined */
/*  system */
/*         (5)  (avv) (cc) (auu)' = (qq) */
/*  from which the nuu*nv7 remaining coefficients */
/*         c(i,j) , i=2+iop0,3+iop0,...,nu-5-iop1,j=1,2,...,nv-7. */
/*  the elements of (cc), are then determined in the least-squares sense. */
/*  simultaneously, we compute the resulting sum of squared residuals sq. */
L150:
    dr01 = dr[1];
    dr11 = dr[4];
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	a0[(i__ << 1) + 1] = dr01;
	a1[(i__ << 1) + 1] = dr11;
/* L155: */
    }
    if (nv8 == 0 || *p <= (float)0.) {
	goto L165;
    }
    i__1 = nv8;
    for (i__ = 1; i__ <= i__1; ++i__) {
	b0[(i__ << 1) + 1] = (float)0.;
	b1[(i__ << 1) + 1] = (float)0.;
/* L160: */
    }
L165:
    mvv = *mv;
    if (*iop0 == 0) {
	goto L195;
    }
    fac = (tu[5] - tu[4]) / three;
    dr02 = dr[2] * fac;
    dr03 = dr[3] * fac;
    i__1 = nv4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c0[i__] = dr01 + dr02 * cosi[(i__ << 1) + 1] + dr03 * cosi[(i__ << 1) 
		+ 2];
/* L170: */
    }
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	number = nrv[i__];
	fac = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    ++number;
	    fac += c0[number] * spv[i__ + j * spv_dim1];
/* L175: */
	}
	a0[(i__ << 1) + 2] = fac;
/* L180: */
    }
    if (nv8 == 0 || *p <= (float)0.) {
	goto L195;
    }
    i__1 = nv8;
    for (i__ = 1; i__ <= i__1; ++i__) {
	number = i__;
	fac = (float)0.;
	for (j = 1; j <= 5; ++j) {
	    fac += c0[number] * bv[i__ + j * bv_dim1];
	    ++number;
/* L185: */
	}
	b0[(i__ << 1) + 2] = fac * pinv;
/* L190: */
    }
    mvv = *mv + nv8;
L195:
    if (*iop1 == 0) {
	goto L225;
    }
    fac = (tu[nu4] - tu[nu4 + 1]) / three;
    dr12 = dr[5] * fac;
    dr13 = dr[6] * fac;
    i__1 = nv4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c1[i__] = dr11 + dr12 * cosi[(i__ << 1) + 1] + dr13 * cosi[(i__ << 1) 
		+ 2];
/* L200: */
    }
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	number = nrv[i__];
	fac = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    ++number;
	    fac += c1[number] * spv[i__ + j * spv_dim1];
/* L205: */
	}
	a1[(i__ << 1) + 2] = fac;
/* L210: */
    }
    if (nv8 == 0 || *p <= (float)0.) {
	goto L225;
    }
    i__1 = nv8;
    for (i__ = 1; i__ <= i__1; ++i__) {
	number = i__;
	fac = (float)0.;
	for (j = 1; j <= 5; ++j) {
	    fac += c1[number] * bv[i__ + j * bv_dim1];
	    ++number;
/* L215: */
	}
	b1[(i__ << 1) + 2] = fac * pinv;
/* L220: */
    }
    mvv = *mv + nv8;
/*  we first determine the matrices (auu) and (qq). then we reduce the */
/*  matrix (auu) to an unit upper triangular form (ru) using givens */
/*  rotations without square roots. we apply the same transformations to */
/*  the rows of matrix qq to obtain the mv x nuu matrix g. */
/*  we store matrix (ru) into au and g into q. */
L225:
    l = mvv * nuu;
/*  initialization. */
    *sq = (float)0.;
    if (l == 0) {
	goto L245;
    }
    i__1 = l;
    for (i__ = 1; i__ <= i__1; ++i__) {
	q[i__] = (float)0.;
/* L230: */
    }
    i__1 = nuu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 5; ++j) {
	    au[i__ + j * au_dim1] = (float)0.;
/* L240: */
	}
    }
    l = 0;
L245:
    nrold = 0;
    n1 = nrold + 1;
    i__1 = *mu;
    for (it = 1; it <= i__1; ++it) {
	number = nru[it];
/*  find the appropriate column of q. */
L250:
	i__2 = mvv;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = (float)0.;
/* L260: */
	}
	if (nrold == number) {
	    goto L280;
	}
	if (*p <= (float)0.) {
	    goto L410;
	}
/*  fetch a new row of matrix (bu). */
	for (j = 1; j <= 5; ++j) {
	    h__[j - 1] = bu[n1 + j * bu_dim1] * pinv;
/* L270: */
	}
	i0 = 1;
	i1 = 5;
	goto L310;
/*  fetch a new row of matrix (spu). */
L280:
	for (j = 1; j <= 4; ++j) {
	    h__[j - 1] = spu[it + j * spu_dim1];
/* L290: */
	}
/*  find the appropriate column of q. */
	i__2 = *mv;
	for (j = 1; j <= i__2; ++j) {
	    ++l;
	    right[j] = r__[l];
/* L300: */
	}
	i0 = 1;
	i1 = 4;
L310:
	j0 = n1;
	j1 = nu7 - number;
/*  take into account that we eliminate the constraints (3) */
L315:
	if (j0 - 1 > *iop0) {
	    goto L335;
	}
	fac0 = h__[i0 - 1];
	i__2 = *mv;
	for (j = 1; j <= i__2; ++j) {
	    right[j] -= fac0 * a0[j0 + (j << 1)];
/* L320: */
	}
	if (*mv == mvv) {
	    goto L330;
	}
	j = *mv;
	i__2 = nv8;
	for (jj = 1; jj <= i__2; ++jj) {
	    ++j;
	    right[j] -= fac0 * b0[j0 + (jj << 1)];
/* L325: */
	}
L330:
	++j0;
	++i0;
	goto L315;
/*  take into account that we eliminate the constraints (4) */
L335:
	if (j1 - 1 > *iop1) {
	    goto L360;
	}
	fac1 = h__[i1 - 1];
	i__2 = *mv;
	for (j = 1; j <= i__2; ++j) {
	    right[j] -= fac1 * a1[j1 + (j << 1)];
/* L340: */
	}
	if (*mv == mvv) {
	    goto L350;
	}
	j = *mv;
	i__2 = nv8;
	for (jj = 1; jj <= i__2; ++jj) {
	    ++j;
	    right[j] -= fac1 * b1[j1 + (jj << 1)];
/* L345: */
	}
L350:
	++j1;
	--i1;
	goto L335;
L360:
	irot = nrold - *iop0 - 1;
	if (irot < 0) {
	    irot = 0;
	}
/*  rotate the new row of matrix (auu) into triangle. */
	if (i0 > i1) {
	    goto L390;
	}
	i__2 = i1;
	for (i__ = i0; i__ <= i__2; ++i__) {
	    ++irot;
	    piv = h__[i__ - 1];
	    if (piv == (float)0.) {
		goto L385;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &au[irot + au_dim1], &co, &si);
/*  apply that transformation to the rows of matrix (qq). */
	    iq = (irot - 1) * mvv;
	    i__3 = mvv;
	    for (j = 1; j <= i__3; ++j) {
		++iq;
		fprota_(&co, &si, &right[j], &q[iq]);
/* L370: */
	    }
/*  apply that transformation to the columns of (auu). */
	    if (i__ == i1) {
		goto L385;
	    }
	    i2 = 1;
	    i3 = i__ + 1;
	    i__3 = i1;
	    for (j = i3; j <= i__3; ++j) {
		++i2;
		fprota_(&co, &si, &h__[j - 1], &au[irot + i2 * au_dim1]);
/* L380: */
	    }
L385:
	    ;
	}
/*  we update the sum of squared residuals. */
L390:
	i__2 = mvv;
	for (j = 1; j <= i__2; ++j) {
/* Computing 2nd power */
	    r__1 = right[j];
	    *sq += r__1 * r__1;
/* L395: */
	}
/* L400: */
	if (nrold == number) {
	    goto L420;
	}
L410:
	nrold = n1;
	++n1;
	goto L250;
L420:
	;
    }
    if (nuu == 0) {
	goto L800;
    }
/*  we determine the matrix (avv) and then we reduce her to an unit */
/*  upper triangular form (rv) using givens rotations without square */
/*  roots. we apply the same transformations to the columns of matrix */
/*  g to obtain the (nv-7) x (nu-6-iop0-iop1) matrix h. */
/*  we store matrix (rv) into av1 and av2, h into c. */
/*  the nv7 x nv7 triangular unit upper matrix (rv) has the form */
/*              | av1 '     | */
/*       (rv) = |     ' av2 | */
/*              |  0  '     | */
/*  with (av2) a nv7 x 4 matrix and (av1) a nv11 x nv11 unit upper */
/*  triangular matrix of bandwidth 5. */
    ncof = nuu * nv7;
/*  initialization. */
    i__1 = ncof;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = (float)0.;
/* L430: */
    }
    i__1 = nv4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	av1[i__ + av1_dim1 * 5] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    av1[i__ + j * av1_dim1] = (float)0.;
	    av2[i__ + j * av2_dim1] = (float)0.;
/* L440: */
	}
    }
    jper = 0;
    nrold = 0;
    i__1 = *mv;
    for (it = 1; it <= i__1; ++it) {
	number = nrv[it];
L450:
	if (nrold == number) {
	    goto L480;
	}
	if (*p <= (float)0.) {
	    goto L760;
	}
/*  fetch a new row of matrix (bv). */
	n1 = nrold + 1;
	for (j = 1; j <= 5; ++j) {
	    h__[j - 1] = bv[n1 + j * bv_dim1] * pinv;
/* L460: */
	}
/*  find the appropiate row of g. */
	i__2 = nuu;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = (float)0.;
/* L465: */
	}
	if (*mv == mvv) {
	    goto L510;
	}
	l = *mv + n1;
	i__2 = nuu;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = q[l];
	    l += mvv;
/* L470: */
	}
	goto L510;
/*  fetch a new row of matrix (spv) */
L480:
	h__[4] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    h__[j - 1] = spv[it + j * spv_dim1];
/* L490: */
	}
/*  find the appropiate row of g. */
	l = it;
	i__2 = nuu;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = q[l];
	    l += mvv;
/* L500: */
	}
/*  test whether there are non-zero values in the new row of (avv) */
/*  corresponding to the b-splines n(j;v),j=nv7+1,...,nv4. */
L510:
	if (nrold < nv11) {
	    goto L710;
	}
	if (jper != 0) {
	    goto L550;
	}
/*  initialize the matrix (av2). */
	jk = nv11 + 1;
	for (i__ = 1; i__ <= 4; ++i__) {
	    ik = jk;
	    for (j = 1; j <= 5; ++j) {
		if (ik <= 0) {
		    goto L530;
		}
		av2[ik + i__ * av2_dim1] = av1[ik + j * av1_dim1];
		--ik;
/* L520: */
	    }
L530:
	    ++jk;
/* L540: */
	}
	jper = 1;
/*  if one of the non-zero elements of the new row corresponds to one of */
/*  the b-splines n(j;v),j=nv7+1,...,nv4, we take account of condition */
/*  (2) for setting up this row of (avv). the row is stored in h1( the */
/*  part with respect to av1) and h2 (the part with respect to av2). */
L550:
	for (i__ = 1; i__ <= 4; ++i__) {
	    h1[i__ - 1] = (float)0.;
	    h2[i__ - 1] = (float)0.;
/* L560: */
	}
	h1[4] = (float)0.;
	j = nrold - nv11;
	for (i__ = 1; i__ <= 5; ++i__) {
	    ++j;
	    l0 = j;
L570:
	    l1 = l0 - 4;
	    if (l1 <= 0) {
		goto L590;
	    }
	    if (l1 <= nv11) {
		goto L580;
	    }
	    l0 = l1 - nv11;
	    goto L570;
L580:
	    h1[l1 - 1] = h__[i__ - 1];
	    goto L600;
L590:
	    h2[l0 - 1] += h__[i__ - 1];
L600:
	    ;
	}
/*  rotate the new row of (avv) into triangle. */
	if (nv11 <= 0) {
	    goto L670;
	}
/*  rotations with the rows 1,2,...,nv11 of (avv). */
	i__2 = nv11;
	for (j = 1; j <= i__2; ++j) {
	    piv = h1[0];
/* Computing MIN */
	    i__3 = nv11 - j;
	    i2 = min(i__3,4);
	    if (piv == (float)0.) {
		goto L640;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &av1[j + av1_dim1], &co, &si);
/*  apply that transformation to the columns of matrix g. */
	    ic = j;
	    i__3 = nuu;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		fprota_(&co, &si, &right[i__], &c__[ic]);
		ic += nv7;
/* L610: */
	    }
/*  apply that transformation to the rows of (avv) with respect to av2. */
	    for (i__ = 1; i__ <= 4; ++i__) {
		fprota_(&co, &si, &h2[i__ - 1], &av2[j + i__ * av2_dim1]);
/* L620: */
	    }
/*  apply that transformation to the rows of (avv) with respect to av1. */
	    if (i2 == 0) {
		goto L670;
	    }
	    i__3 = i2;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		i1 = i__ + 1;
		fprota_(&co, &si, &h1[i1 - 1], &av1[j + i1 * av1_dim1]);
/* L630: */
	    }
L640:
	    i__3 = i2;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h1[i__ - 1] = h1[i__];
/* L650: */
	    }
	    h1[i2] = (float)0.;
/* L660: */
	}
/*  rotations with the rows nv11+1,...,nv7 of avv. */
L670:
	for (j = 1; j <= 4; ++j) {
	    ij = nv11 + j;
	    if (ij <= 0) {
		goto L700;
	    }
	    piv = h2[j - 1];
	    if (piv == (float)0.) {
		goto L700;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &av2[ij + j * av2_dim1], &co, &si);
/*  apply that transformation to the columns of matrix g. */
	    ic = ij;
	    i__2 = nuu;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		fprota_(&co, &si, &right[i__], &c__[ic]);
		ic += nv7;
/* L680: */
	    }
	    if (j == 4) {
		goto L700;
	    }
/*  apply that transformation to the rows of (avv) with respect to av2. */
	    j1 = j + 1;
	    for (i__ = j1; i__ <= 4; ++i__) {
		fprota_(&co, &si, &h2[i__ - 1], &av2[ij + i__ * av2_dim1]);
/* L690: */
	    }
L700:
	    ;
	}
/*  we update the sum of squared residuals. */
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing 2nd power */
	    r__1 = right[i__];
	    *sq += r__1 * r__1;
/* L705: */
	}
	goto L750;
/*  rotation into triangle of the new row of (avv), in case the elements */
/*  corresponding to the b-splines n(j;v),j=nv7+1,...,nv4 are all zero. */
L710:
	irot = nrold;
	for (i__ = 1; i__ <= 5; ++i__) {
	    ++irot;
	    piv = h__[i__ - 1];
	    if (piv == (float)0.) {
		goto L740;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &av1[irot + av1_dim1], &co, &si);
/*  apply that transformation to the columns of matrix g. */
	    ic = irot;
	    i__2 = nuu;
	    for (j = 1; j <= i__2; ++j) {
		fprota_(&co, &si, &right[j], &c__[ic]);
		ic += nv7;
/* L720: */
	    }
/*  apply that transformation to the rows of (avv). */
	    if (i__ == 5) {
		goto L740;
	    }
	    i2 = 1;
	    i3 = i__ + 1;
	    for (j = i3; j <= 5; ++j) {
		++i2;
		fprota_(&co, &si, &h__[j - 1], &av1[irot + i2 * av1_dim1]);
/* L730: */
	    }
L740:
	    ;
	}
/*  we update the sum of squared residuals. */
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing 2nd power */
	    r__1 = right[i__];
	    *sq += r__1 * r__1;
/* L745: */
	}
L750:
	if (nrold == number) {
	    goto L770;
	}
L760:
	++nrold;
	goto L450;
L770:
	;
    }
/*  test whether the b-spline coefficients must be determined. */
    if (*iback != 0) {
	return 0;
    }
/*  backward substitution to obtain the b-spline coefficients as the */
/*  solution of the linear system    (rv) (cr) (ru)' = h. */
/*  first step: solve the system  (rv) (c1) = h. */
    k = 1;
    i__1 = nuu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpbacp_(&av1[av1_offset], &av2[av2_offset], &c__[k], &nv7, &c__4, &
		c__[k], &c__5, nv);
	k += nv7;
/* L780: */
    }
/*  second step: solve the system  (cr) (ru)' = (c1). */
    k = 0;
    i__1 = nv7;
    for (j = 1; j <= i__1; ++j) {
	++k;
	l = k;
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    right[i__] = c__[l];
	    l += nv7;
/* L785: */
	}
	fpback_(&au[au_offset], &right[1], &nuu, &c__5, &right[1], nu);
	l = k;
	i__2 = nuu;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    c__[l] = right[i__];
	    l += nv7;
/* L790: */
	}
/* L795: */
    }
/*  calculate from the conditions (2)-(3)-(4), the remaining b-spline */
/*  coefficients. */
L800:
    ncof = nu4 * nv4;
    j = ncof;
    i__1 = nv4;
    for (l = 1; l <= i__1; ++l) {
	q[l] = dr01;
	q[j] = dr11;
	--j;
/* L805: */
    }
    i__ = nv4;
    j = 0;
    if (*iop0 == 0) {
	goto L815;
    }
    i__1 = nv4;
    for (l = 1; l <= i__1; ++l) {
	++i__;
	q[i__] = c0[l];
/* L810: */
    }
L815:
    if (nuu == 0) {
	goto L835;
    }
    i__1 = nuu;
    for (l = 1; l <= i__1; ++l) {
	ii = i__;
	i__2 = nv7;
	for (k = 1; k <= i__2; ++k) {
	    ++i__;
	    ++j;
	    q[i__] = c__[j];
/* L820: */
	}
	for (k = 1; k <= 3; ++k) {
	    ++ii;
	    ++i__;
	    q[i__] = q[ii];
/* L825: */
	}
/* L830: */
    }
L835:
    if (*iop1 == 0) {
	goto L845;
    }
    i__1 = nv4;
    for (l = 1; l <= i__1; ++l) {
	++i__;
	q[i__] = c1[l];
/* L840: */
    }
L845:
    i__1 = ncof;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = q[i__];
/* L850: */
    }
/*  calculate the quantities */
/*    res(i,j) = (r(i,j) - s(u(i),v(j)))**2 , i=1,2,..,mu;j=1,2,..,mv */
/*    fp = sumi=1,mu(sumj=1,mv(res(i,j))) */
/*    fpu(r) = sum''i(sumj=1,mv(res(i,j))) , r=1,2,...,nu-7 */
/*                  tu(r+3) <= u(i) <= tu(r+4) */
/*    fpv(r) = sumi=1,mu(sum''j(res(i,j))) , r=1,2,...,nv-7 */
/*                  tv(r+3) <= v(j) <= tv(r+4) */
    *fp = (float)0.;
    i__1 = *nu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpu[i__] = (float)0.;
/* L890: */
    }
    i__1 = *nv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fpv[i__] = (float)0.;
/* L900: */
    }
    ir = 0;
    nroldu = 0;
/*  main loop for the different grid points. */
    i__1 = *mu;
    for (i1 = 1; i1 <= i__1; ++i1) {
	numu = nru[i1];
	numu1 = numu + 1;
	nroldv = 0;
	i__2 = *mv;
	for (i2 = 1; i2 <= i__2; ++i2) {
	    numv = nrv[i2];
	    numv1 = numv + 1;
	    ++ir;
/*  evaluate s(u,v) at the current grid point by making the sum of the */
/*  cross products of the non-zero b-splines at (u,v), multiplied with */
/*  the appropiate b-spline coefficients. */
	    term = (float)0.;
	    k1 = numu * nv4 + numv;
	    for (l1 = 1; l1 <= 4; ++l1) {
		k2 = k1;
		fac = spu[i1 + l1 * spu_dim1];
		for (l2 = 1; l2 <= 4; ++l2) {
		    ++k2;
		    term += fac * spv[i2 + l2 * spv_dim1] * c__[k2];
/* L910: */
		}
		k1 += nv4;
/* L920: */
	    }
/*  calculate the squared residual at the current grid point. */
/* Computing 2nd power */
	    r__1 = r__[ir] - term;
	    term = r__1 * r__1;
/*  adjust the different parameters. */
	    *fp += term;
	    fpu[numu1] += term;
	    fpv[numv1] += term;
	    fac = term * half;
	    if (numv == nroldv) {
		goto L930;
	    }
	    fpv[numv1] -= fac;
	    fpv[numv] += fac;
L930:
	    nroldv = numv;
	    if (numu == nroldu) {
		goto L940;
	    }
	    fpu[numu1] -= fac;
	    fpu[numu] += fac;
L940:
	    ;
	}
	nroldu = numu;
/* L950: */
    }
    return 0;
} /* fpgrsp_ */

/* Subroutine */ int fpinst_(iopt, t, n, c__, k, x, l, tt, nn, cc, nest)
integer *iopt;
real *t;
integer *n;
real *c__;
integer *k;
real *x;
integer *l;
real *tt;
integer *nn;
real *cc;
integer *nest;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, m, i1, k1, mk, nk, nl, ll, nk1;
    static real fac, one, per;

/*  given the b-spline representation (knots t(j),j=1,2,...,n, b-spline */
/*  coefficients c(j),j=1,2,...,n-k-1) of a spline of degree k, fpinst */
/*  calculates the b-spline representation (knots tt(j),j=1,2,...,nn, */
/*  b-spline coefficients cc(j),j=1,2,...,nn-k-1) of the same spline if */
/*  an additional knot is inserted at the point x situated in the inter- */
/*  val t(l)<=x<t(l+1). iopt denotes whether (iopt.ne.0) or not (iopt=0) */
/*  the given spline is periodic. in case of a periodic spline at least */
/*  one of the following conditions must be fulfilled: l>2*k or l<n-2*k. */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --cc;
    --tt;
    --c__;
    --t;

    /* Function Body */
    one = (float)1.;
    k1 = *k + 1;
    nk1 = *n - k1;
/*  the new knots */
    ll = *l + 1;
    i__ = *n;
    i__1 = *n;
    for (j = ll; j <= i__1; ++j) {
	tt[i__ + 1] = t[i__];
	--i__;
/* L10: */
    }
    tt[ll] = *x;
    i__1 = *l;
    for (j = 1; j <= i__1; ++j) {
	tt[j] = t[j];
/* L20: */
    }
/*  the new b-spline coefficients */
    i__ = nk1;
    i__1 = nk1;
    for (j = *l; j <= i__1; ++j) {
	cc[i__ + 1] = c__[i__];
	--i__;
/* L30: */
    }
    i__ = *l;
    i__1 = *k;
    for (j = 1; j <= i__1; ++j) {
	m = i__ + k1;
	fac = (*x - tt[i__]) / (tt[m] - tt[i__]);
	i1 = i__ - 1;
	cc[i__] = fac * c__[i__] + (one - fac) * c__[i1];
	i__ = i1;
/* L40: */
    }
    i__1 = i__;
    for (j = 1; j <= i__1; ++j) {
	cc[j] = c__[j];
/* L50: */
    }
    *nn = *n + 1;
    if (*iopt == 0) {
	return 0;
    }
/*   incorporate the boundary conditions for a periodic spline. */
    nk = *nn - *k;
    nl = nk - k1;
    per = tt[nk] - tt[k1];
    i__ = k1;
    j = nk;
    if (ll <= nl) {
	goto L70;
    }
    i__1 = *k;
    for (m = 1; m <= i__1; ++m) {
	mk = m + nl;
	cc[m] = cc[mk];
	--i__;
	--j;
	tt[i__] = tt[j] - per;
/* L60: */
    }
    return 0;
L70:
    if (ll > k1 + *k) {
	return 0;
    }
    i__1 = *k;
    for (m = 1; m <= i__1; ++m) {
	mk = m + nl;
	cc[mk] = cc[m];
	++i__;
	++j;
	tt[j] = tt[i__] + per;
/* L80: */
    }
    return 0;
} /* fpinst_ */

/* Subroutine */ int fpintb_(t, n, bint, nk1, x, y)
real *t;
integer *n;
real *bint;
integer *nk1;
real *x, *y;
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Local variables */
    static real aint[6], a, b, f, h__[6];
    static integer i__, j, k, l;
    static real h1[6];
    static integer j1, k1, l0, ia, ib;
    static real ak;
    static integer li, lj, lk, it;
    static real arg, one;
    static integer min__;

/*  subroutine fpintb calculates integrals of the normalized b-splines */
/*  nj,k+1(x) of degree k, defined on the set of knots t(j),j=1,2,...n. */
/*  it makes use of the formulae of gaffney for the calculation of */
/*  indefinite integrals of b-splines. */

/*  calling sequence: */
/*     call fpintb(t,n,bint,nk1,x,y) */

/*  input parameters: */
/*    t    : real array,length n, containing the position of the knots. */
/*    n    : integer value, giving the number of knots. */
/*    nk1  : integer value, giving the number of b-splines of degree k, */
/*           defined on the set of knots ,i.e. nk1 = n-k-1. */
/*    x,y  : real values, containing the end points of the integration */
/*           interval. */
/*  output parameter: */
/*    bint : array,length nk1, containing the integrals of the b-splines. */
/*  .. */
/*  ..scalars arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  initialization. */
    /* Parameter adjustments */
    --t;
    --bint;

    /* Function Body */
    one = (float)1.;
    k1 = *n - *nk1;
    ak = (real) k1;
    k = k1 - 1;
    i__1 = *nk1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	bint[i__] = (float)0.;
/* L10: */
    }
/*  the integration limits are arranged in increasing order. */
    a = *x;
    b = *y;
    min__ = 0;
    if ((r__1 = a - b) < (float)0.) {
	goto L30;
    } else if (r__1 == 0) {
	goto L160;
    } else {
	goto L20;
    }
L20:
    a = *y;
    b = *x;
    min__ = 1;
L30:
    if (a < t[k1]) {
	a = t[k1];
    }
    if (b > t[*nk1 + 1]) {
	b = t[*nk1 + 1];
    }
/*  using the expression of gaffney for the indefinite integral of a */
/*  b-spline we find that */
/*  bint(j) = (t(j+k+1)-t(j))*(res(j,b)-res(j,a))/(k+1) */
/*    where for t(l) <= x < t(l+1) */
/*    res(j,x) = 0, j=1,2,...,l-k-1 */
/*             = 1, j=l+1,l+2,...,nk1 */
/*             = aint(j+k-l+1), j=l-k,l-k+1,...,l */
/*               = sumi((x-t(j+i))*nj+i,k+1-i(x)/(t(j+k+1)-t(j+i))) */
/*                 i=0,1,...,k */
    l = k1;
    l0 = l + 1;
/*  set arg = a. */
    arg = a;
    for (it = 1; it <= 2; ++it) {
/*  search for the knot interval t(l) <= arg < t(l+1). */
L40:
	if (arg < t[l0] || l == *nk1) {
	    goto L50;
	}
	l = l0;
	l0 = l + 1;
	goto L40;
/*  calculation of aint(j), j=1,2,...,k+1. */
/*  initialization. */
L50:
	i__1 = k1;
	for (j = 1; j <= i__1; ++j) {
	    aint[j - 1] = (float)0.;
/* L55: */
	}
	aint[0] = (arg - t[l]) / (t[l + 1] - t[l]);
	h1[0] = one;
	i__1 = k;
	for (j = 1; j <= i__1; ++j) {
/*  evaluation of the non-zero b-splines of degree j at arg,i.e. */
/*    h(i+1) = nl-j+i,j(arg), i=0,1,...,j. */
	    h__[0] = (float)0.;
	    i__2 = j;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		li = l + i__;
		lj = li - j;
		f = h1[i__ - 1] / (t[li] - t[lj]);
		h__[i__ - 1] += f * (t[li] - arg);
		h__[i__] = f * (arg - t[lj]);
/* L60: */
	    }
/*  updating of the integrals aint. */
	    j1 = j + 1;
	    i__2 = j1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		li = l + i__;
		lj = li - j1;
		aint[i__ - 1] += h__[i__ - 1] * (arg - t[lj]) / (t[li] - t[lj]
			);
		h1[i__ - 1] = h__[i__ - 1];
/* L70: */
	    }
	}
	if (it == 2) {
	    goto L100;
	}
/*  updating of the integrals bint */
	lk = l - k;
	ia = lk;
	i__2 = k1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    bint[lk] = -aint[i__ - 1];
	    ++lk;
/* L80: */
	}
/*  set arg = b. */
	arg = b;
/* L90: */
    }
/*  updating of the integrals bint. */
L100:
    lk = l - k;
    ib = lk - 1;
    i__2 = k1;
    for (i__ = 1; i__ <= i__2; ++i__) {
	bint[lk] += aint[i__ - 1];
	++lk;
/* L110: */
    }
    if (ib < ia) {
	goto L130;
    }
    i__2 = ib;
    for (i__ = ia; i__ <= i__2; ++i__) {
	bint[i__] += one;
/* L120: */
    }
/*  the scaling factors are taken into account. */
L130:
    f = one / ak;
    i__2 = *nk1;
    for (i__ = 1; i__ <= i__2; ++i__) {
	j = i__ + k1;
	bint[i__] = bint[i__] * (t[j] - t[i__]) * f;
/* L140: */
    }
/*  the order of the integration limits is taken into account. */
    if (min__ == 0) {
	goto L160;
    }
    i__2 = *nk1;
    for (i__ = 1; i__ <= i__2; ++i__) {
	bint[i__] = -bint[i__];
/* L150: */
    }
L160:
    return 0;
} /* fpintb_ */

/* Subroutine */ int fpknot_(x, m, t, n, fpint, nrdata, nrint, nest, istart)
real *x;
integer *m;
real *t;
integer *n;
real *fpint;
integer *nrdata, *nrint, *nest, *istart;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer next, j, k, ihalf;
    static real fpmax;
    static integer maxpt;
    static real am, an;
    static integer jj, jk, jbegin, maxbeg, number, jpoint, nrx;

/*  subroutine fpknot locates an additional knot for a spline of degree */
/*  k and adjusts the corresponding parameters,i.e. */
/*    t     : the position of the knots. */
/*    n     : the number of knots. */
/*    nrint : the number of knotintervals. */
/*    fpint : the sum of squares of residual right hand sides */
/*            for each knot interval. */
/*    nrdata: the number of data points inside each knot interval. */
/*  istart indicates that the smallest data point at which the new knot */
/*  may be added is x(istart+1) */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --x;
    --nrdata;
    --fpint;
    --t;

    /* Function Body */
    k = (*n - *nrint - 1) / 2;
/*  search for knot interval t(number+k) <= x <= t(number+k+1) where */
/*  fpint(number) is maximal on the condition that nrdata(number) */
/*  not equals zero. */
    fpmax = (float)0.;
    jbegin = *istart;
    i__1 = *nrint;
    for (j = 1; j <= i__1; ++j) {
	jpoint = nrdata[j];
	if (fpmax >= fpint[j] || jpoint == 0) {
	    goto L10;
	}
	fpmax = fpint[j];
	number = j;
	maxpt = jpoint;
	maxbeg = jbegin;
L10:
	jbegin = jbegin + jpoint + 1;
/* L20: */
    }
/*  let coincide the new knot t(number+k+1) with a data point x(nrx) */
/*  inside the old knot interval t(number+k) <= x <= t(number+k+1). */
    ihalf = maxpt / 2 + 1;
    nrx = maxbeg + ihalf;
    next = number + 1;
    if (next > *nrint) {
	goto L40;
    }
/*  adjust the different parameters. */
    i__1 = *nrint;
    for (j = next; j <= i__1; ++j) {
	jj = next + *nrint - j;
	fpint[jj + 1] = fpint[jj];
	nrdata[jj + 1] = nrdata[jj];
	jk = jj + k;
	t[jk + 1] = t[jk];
/* L30: */
    }
L40:
    nrdata[number] = ihalf - 1;
    nrdata[next] = maxpt - ihalf;
    am = (real) maxpt;
    an = (real) nrdata[number];
    fpint[number] = fpmax * an / am;
    an = (real) nrdata[next];
    fpint[next] = fpmax * an / am;
    jk = next + k;
    t[jk] = x[nrx];
    ++(*n);
    ++(*nrint);
    return 0;
} /* fpknot_ */

/* Subroutine */ int fpopdi_(ifsu, ifsv, ifbu, ifbv, u, mu, v, mv, z__, mz, 
	z0, dz, iopt, ider, tu, nu, tv, nv, nuest, nvest, p, step, c__, nc, 
	fp, fpu, fpv, nru, nrv, wrk, lwrk)
integer *ifsu, *ifsv, *ifbu, *ifbv;
real *u;
integer *mu;
real *v;
integer *mv;
real *z__;
integer *mz;
real *z0, *dz;
integer *iopt, *ider;
real *tu;
integer *nu;
real *tv;
integer *nv, *nuest, *nvest;
real *p, *step, *c__;
integer *nc;
real *fp, *fpu, *fpv;
integer *nru, *nrv;
real *wrk;
integer *lwrk;
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Local variables */
    static integer mvnu;
    static real step1, step2, a[36]	/* was [6][6] */, g[6];
    static integer i__, j, l;
    static real delta[3], three;
    static integer i1, l1, l2, mm, lq, nr[3];
    static real sq;
    extern /* Subroutine */ int fpgrdi_();
    static integer number, id0;
    extern /* Subroutine */ int fpsysy_();
    static integer laa, lbb, lcc, lau, lbu, lbv, lcs, lri;
    static real res, sqq;
    static integer lsu, lsv;
    static real dzz[3], sum[3];
    static integer lav1, lav2, iop0, iop1;

/*  given the set of function values z(i,j) defined on the rectangular */
/*  grid (u(i),v(j)),i=1,2,...,mu;j=1,2,...,mv, fpopdi determines a */
/*  smooth bicubic spline approximation with given knots tu(i),i=1,..,nu */
/*  in the u-direction and tv(j),j=1,2,...,nv in the v-direction. this */
/*  spline sp(u,v) will be periodic in the variable v and will satisfy */
/*  the following constraints */

/*     s(tu(1),v) = dz(1) , tv(4) <=v<= tv(nv-3) */

/*  and (if iopt(2) = 1) */

/*     d s(tu(1),v) */
/*     ------------ =  dz(2)*cos(v)+dz(3)*sin(v) , tv(4) <=v<= tv(nv-3) */
/*     d u */

/*  and (if iopt(3) = 1) */

/*     s(tu(nu),v)  =  0   tv(4) <=v<= tv(nv-3) */

/*  where the parameters dz(i) correspond to the derivative values g(i,j) */
/*  as defined in subroutine pogrid. */

/*  the b-spline coefficients of sp(u,v) are determined as the least- */
/*  squares solution  of an overdetermined linear system which depends */
/*  on the value of p and on the values dz(i),i=1,2,3. the correspond- */
/*  ing sum of squared residuals sq is a simple quadratic function in */
/*  the variables dz(i). these may or may not be provided. the values */
/*  dz(i) which are not given will be determined so as to minimize the */
/*  resulting sum of squared residuals sq. in that case the user must */
/*  provide some initial guess dz(i) and some estimate (dz(i)-step, */
/*  dz(i)+step) of the range of possible values for these latter. */

/*  sp(u,v) also depends on the parameter p (p>0) in such a way that */
/*    - if p tends to infinity, sp(u,v) becomes the least-squares spline */
/*      with given knots, satisfying the constraints. */
/*    - if p tends to zero, sp(u,v) becomes the least-squares polynomial, */
/*      satisfying the constraints. */
/*    - the function  f(p)=sumi=1,mu(sumj=1,mv((z(i,j)-sp(u(i),v(j)))**2) */
/*      is continuous and strictly decreasing for p>0. */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpgrdi,fpsysy */
/*  .. */
/*  set constant */
    /* Parameter adjustments */
    --nru;
    --u;
    --nrv;
    --v;
    --z__;
    --dz;
    --iopt;
    --ider;
    --fpu;
    --tu;
    --fpv;
    --tv;
    --c__;
    --wrk;

    /* Function Body */
    three = (float)3.;
/*  we partition the working space */
    lsu = 1;
    lsv = lsu + (*mu << 2);
    lri = lsv + (*mv << 2);
/* Computing MAX */
    i__1 = *nuest, i__2 = *mv + *nvest;
    mm = max(i__1,i__2);
    lq = lri + mm;
    mvnu = *nuest * (*mv + *nvest - 8);
    lau = lq + mvnu;
    lav1 = lau + *nuest * 5;
    lav2 = lav1 + *nvest * 6;
    lbu = lav2 + (*nvest << 2);
    lbv = lbu + *nuest * 5;
    laa = lbv + *nvest * 5;
    lbb = laa + (*mv << 1);
    lcc = lbb + (*nvest << 1);
    lcs = lcc + *nvest;
/*  we calculate the smoothing spline sp(u,v) according to the input */
/*  values dz(i),i=1,2,3. */
    iop0 = iopt[2];
    iop1 = iopt[3];
    fpgrdi_(ifsu, ifsv, ifbu, ifbv, &c__0, &u[1], mu, &v[1], mv, &z__[1], mz, 
	    &dz[1], &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &c__[1], nc, &sq,
	     fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[lsu], &wrk[lsv], &wrk[lri]
	    , &wrk[lq], &wrk[lau], &wrk[lav1], &wrk[lav2], &wrk[lbu], &wrk[
	    lbv], &wrk[laa], &wrk[lbb], &wrk[lcc], &wrk[lcs], &nru[1], &nrv[1]
	    );
    id0 = ider[1];
    if (id0 != 0) {
	goto L5;
    }
/* Computing 2nd power */
    r__1 = *z0 - dz[1];
    res = r__1 * r__1;
    *fp += res;
    sq += res;
/* in case all derivative values dz(i) are given (step<=0) or in case */
/* we have spline interpolation, we accept this spline as a solution. */
L5:
    if (*step <= (float)0. || sq <= (float)0.) {
	return 0;
    }
    dzz[0] = dz[1];
    dzz[1] = dz[2];
    dzz[2] = dz[3];
/* number denotes the number of derivative values dz(i) that still must */
/* be optimized. let us denote these parameters by g(j),j=1,...,number. */
    number = 0;
    if (id0 > 0) {
	goto L10;
    }
    number = 1;
    nr[0] = 1;
    delta[0] = *step;
L10:
    if (iop0 == 0) {
	goto L20;
    }
    if (ider[2] != 0) {
	goto L20;
    }
    step2 = *step * three / tu[5];
    nr[number] = 2;
    nr[number + 1] = 3;
    delta[number] = step2;
    delta[number + 1] = step2;
    number += 2;
L20:
    if (number == 0) {
	return 0;
    }
/* the sum of squared residuals sq is a quadratic polynomial in the */
/* parameters g(j). we determine the unknown coefficients of this */
/* polymomial by calculating (number+1)*(number+2)/2 different splines */
/* according to specific values for g(j). */
    i__1 = number;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l = nr[i__ - 1];
	step1 = delta[i__ - 1];
	dzz[l - 1] = dz[l] + step1;
	fpgrdi_(ifsu, ifsv, ifbu, ifbv, &c__1, &u[1], mu, &v[1], mv, &z__[1], 
		mz, dzz, &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &c__[1], nc,
		 &sum[i__ - 1], fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[lsu], &
		wrk[lsv], &wrk[lri], &wrk[lq], &wrk[lau], &wrk[lav1], &wrk[
		lav2], &wrk[lbu], &wrk[lbv], &wrk[laa], &wrk[lbb], &wrk[lcc], 
		&wrk[lcs], &nru[1], &nrv[1]);
	if (id0 == 0) {
/* Computing 2nd power */
	    r__1 = *z0 - dzz[0];
	    sum[i__ - 1] += r__1 * r__1;
	}
	dzz[l - 1] = dz[l] - step1;
	fpgrdi_(ifsu, ifsv, ifbu, ifbv, &c__1, &u[1], mu, &v[1], mv, &z__[1], 
		mz, dzz, &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &c__[1], nc,
		 &sqq, fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[lsu], &wrk[lsv],
		 &wrk[lri], &wrk[lq], &wrk[lau], &wrk[lav1], &wrk[lav2], &wrk[
		lbu], &wrk[lbv], &wrk[laa], &wrk[lbb], &wrk[lcc], &wrk[lcs], &
		nru[1], &nrv[1]);
	if (id0 == 0) {
/* Computing 2nd power */
	    r__1 = *z0 - dzz[0];
	    sqq += r__1 * r__1;
	}
/* Computing 2nd power */
	r__1 = step1;
	a[i__ + i__ * 6 - 7] = (sum[i__ - 1] + sqq - sq - sq) / (r__1 * r__1);
	if (a[i__ + i__ * 6 - 7] <= (float)0.) {
	    goto L80;
	}
	g[i__ - 1] = (sqq - sum[i__ - 1]) / (step1 + step1);
	dzz[l - 1] = dz[l];
/* L30: */
    }
    if (number == 1) {
	goto L60;
    }
    i__1 = number;
    for (i__ = 2; i__ <= i__1; ++i__) {
	l1 = nr[i__ - 1];
	step1 = delta[i__ - 1];
	dzz[l1 - 1] = dz[l1] + step1;
	i1 = i__ - 1;
	i__2 = i1;
	for (j = 1; j <= i__2; ++j) {
	    l2 = nr[j - 1];
	    step2 = delta[j - 1];
	    dzz[l2 - 1] = dz[l2] + step2;
	    fpgrdi_(ifsu, ifsv, ifbu, ifbv, &c__1, &u[1], mu, &v[1], mv, &z__[
		    1], mz, dzz, &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &
		    c__[1], nc, &sqq, fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[
		    lsu], &wrk[lsv], &wrk[lri], &wrk[lq], &wrk[lau], &wrk[
		    lav1], &wrk[lav2], &wrk[lbu], &wrk[lbv], &wrk[laa], &wrk[
		    lbb], &wrk[lcc], &wrk[lcs], &nru[1], &nrv[1]);
	    if (id0 == 0) {
/* Computing 2nd power */
		r__1 = *z0 - dzz[0];
		sqq += r__1 * r__1;
	    }
	    a[i__ + j * 6 - 7] = (sq + sqq - sum[i__ - 1] - sum[j - 1]) / (
		    step1 * step2);
	    dzz[l2 - 1] = dz[l2];
/* L40: */
	}
	dzz[l1 - 1] = dz[l1];
/* L50: */
    }
/* the optimal values g(j) are found as the solution of the system */
/* d (sq) / d (g(j)) = 0 , j=1,...,number. */
L60:
    fpsysy_(a, &number, g);
    i__1 = number;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l = nr[i__ - 1];
	dz[l] += g[i__ - 1];
/* L70: */
    }
/* we determine the spline sp(u,v) according to the optimal values g(j). */
L80:
    fpgrdi_(ifsu, ifsv, ifbu, ifbv, &c__0, &u[1], mu, &v[1], mv, &z__[1], mz, 
	    &dz[1], &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &c__[1], nc, &sq,
	     fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[lsu], &wrk[lsv], &wrk[lri]
	    , &wrk[lq], &wrk[lau], &wrk[lav1], &wrk[lav2], &wrk[lbu], &wrk[
	    lbv], &wrk[laa], &wrk[lbb], &wrk[lcc], &wrk[lcs], &nru[1], &nrv[1]
	    );
    if (id0 == 0) {
/* Computing 2nd power */
	r__1 = *z0 - dz[1];
	*fp += r__1 * r__1;
    }
    return 0;
} /* fpopdi_ */

/* Subroutine */ int fpopsp_(ifsu, ifsv, ifbu, ifbv, u, mu, v, mv, r__, mr, 
	r0, r1, dr, iopt, ider, tu, nu, tv, nv, nuest, nvest, p, step, c__, 
	nc, fp, fpu, fpv, nru, nrv, wrk, lwrk)
integer *ifsu, *ifsv, *ifbu, *ifbv;
real *u;
integer *mu;
real *v;
integer *mv;
real *r__;
integer *mr;
real *r0, *r1, *dr;
integer *iopt, *ider;
real *tu;
integer *nu;
real *tv;
integer *nv, *nuest, *nvest;
real *p, *step, *c__;
integer *nc;
real *fp, *fpu, *fpv;
integer *nru, *nrv;
real *wrk;
integer *lwrk;
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Local variables */
    static integer mvnu;
    static real step1, step2, a[36]	/* was [6][6] */, g[6];
    static integer i__, j, l;
    static real delta[6], three;
    static integer i1, l1, l2, mm, lq, nr[6];
    static real sq;
    static integer number, id0, la0, la1, lb0, lb1, lc0, lc1, id1;
    extern /* Subroutine */ int fpgrsp_();
    static real sq0, sq1;
    extern /* Subroutine */ int fpsysy_();
    static integer lau, lbu, lbv, lcs, lri;
    static real drr[6], sqq;
    static integer lsu, lsv;
    static real sum[6];
    static integer lav1, lav2, iop0, iop1;

/*  given the set of function values r(i,j) defined on the rectangular */
/*  grid (u(i),v(j)),i=1,2,...,mu;j=1,2,...,mv, fpopsp determines a */
/*  smooth bicubic spline approximation with given knots tu(i),i=1,..,nu */
/*  in the u-direction and tv(j),j=1,2,...,nv in the v-direction. this */
/*  spline sp(u,v) will be periodic in the variable v and will satisfy */
/*  the following constraints */

/*     s(tu(1),v) = dr(1) , tv(4) <=v<= tv(nv-3) */

/*     s(tu(nu),v) = dr(4) , tv(4) <=v<= tv(nv-3) */

/*  and (if iopt(2) = 1) */

/*     d s(tu(1),v) */
/*     ------------ =  dr(2)*cos(v)+dr(3)*sin(v) , tv(4) <=v<= tv(nv-3) */
/*     d u */

/*  and (if iopt(3) = 1) */

/*     d s(tu(nu),v) */
/*     ------------- =  dr(5)*cos(v)+dr(6)*sin(v) , tv(4) <=v<= tv(nv-3) */
/*     d u */

/*  where the parameters dr(i) correspond to the derivative values at the */
/*  poles as defined in subroutine spgrid. */

/*  the b-spline coefficients of sp(u,v) are determined as the least- */
/*  squares solution  of an overdetermined linear system which depends */
/*  on the value of p and on the values dr(i),i=1,...,6. the correspond- */
/*  ing sum of squared residuals sq is a simple quadratic function in */
/*  the variables dr(i). these may or may not be provided. the values */
/*  dr(i) which are not given will be determined so as to minimize the */
/*  resulting sum of squared residuals sq. in that case the user must */
/*  provide some initial guess dr(i) and some estimate (dr(i)-step, */
/*  dr(i)+step) of the range of possible values for these latter. */

/*  sp(u,v) also depends on the parameter p (p>0) in such a way that */
/*    - if p tends to infinity, sp(u,v) becomes the least-squares spline */
/*      with given knots, satisfying the constraints. */
/*    - if p tends to zero, sp(u,v) becomes the least-squares polynomial, */
/*      satisfying the constraints. */
/*    - the function  f(p)=sumi=1,mu(sumj=1,mv((r(i,j)-sp(u(i),v(j)))**2) */
/*      is continuous and strictly decreasing for p>0. */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpgrsp,fpsysy */
/*  .. */
/*  set constant */
    /* Parameter adjustments */
    --nru;
    --u;
    --nrv;
    --v;
    --r__;
    --dr;
    --iopt;
    --ider;
    --fpu;
    --tu;
    --fpv;
    --tv;
    --step;
    --c__;
    --wrk;

    /* Function Body */
    three = (float)3.;
/*  we partition the working space */
    lsu = 1;
    lsv = lsu + (*mu << 2);
    lri = lsv + (*mv << 2);
/* Computing MAX */
    i__1 = *nuest, i__2 = *mv + *nvest;
    mm = max(i__1,i__2);
    lq = lri + mm;
    mvnu = *nuest * (*mv + *nvest - 8);
    lau = lq + mvnu;
    lav1 = lau + *nuest * 5;
    lav2 = lav1 + *nvest * 6;
    lbu = lav2 + (*nvest << 2);
    lbv = lbu + *nuest * 5;
    la0 = lbv + *nvest * 5;
    la1 = la0 + (*mv << 1);
    lb0 = la1 + (*mv << 1);
    lb1 = lb0 + (*nvest << 1);
    lc0 = lb1 + (*nvest << 1);
    lc1 = lc0 + *nvest;
    lcs = lc1 + *nvest;
/*  we calculate the smoothing spline sp(u,v) according to the input */
/*  values dr(i),i=1,...,6. */
    iop0 = iopt[2];
    iop1 = iopt[3];
    id0 = ider[1];
    id1 = ider[3];
    fpgrsp_(ifsu, ifsv, ifbu, ifbv, &c__0, &u[1], mu, &v[1], mv, &r__[1], mr, 
	    &dr[1], &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &c__[1], nc, &sq,
	     fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[lsu], &wrk[lsv], &wrk[lri]
	    , &wrk[lq], &wrk[lau], &wrk[lav1], &wrk[lav2], &wrk[lbu], &wrk[
	    lbv], &wrk[la0], &wrk[la1], &wrk[lb0], &wrk[lb1], &wrk[lc0], &wrk[
	    lc1], &wrk[lcs], &nru[1], &nrv[1]);
    sq0 = (float)0.;
    sq1 = (float)0.;
    if (id0 == 0) {
/* Computing 2nd power */
	r__1 = *r0 - dr[1];
	sq0 = r__1 * r__1;
    }
    if (id1 == 0) {
/* Computing 2nd power */
	r__1 = *r1 - dr[4];
	sq1 = r__1 * r__1;
    }
    sq = sq + sq0 + sq1;
/* in case all derivative values dr(i) are given (step<=0) or in case */
/* we have spline interpolation, we accept this spline as a solution. */
    if (sq <= (float)0.) {
	return 0;
    }
    if (step[1] <= (float)0. && step[2] <= (float)0.) {
	return 0;
    }
    for (i__ = 1; i__ <= 6; ++i__) {
	drr[i__ - 1] = dr[i__];
/* L10: */
    }
/* number denotes the number of derivative values dr(i) that still must */
/* be optimized. let us denote these parameters by g(j),j=1,...,number. */
    number = 0;
    if (id0 > 0) {
	goto L20;
    }
    number = 1;
    nr[0] = 1;
    delta[0] = step[1];
L20:
    if (iop0 == 0) {
	goto L30;
    }
    if (ider[2] != 0) {
	goto L30;
    }
    step2 = step[1] * three / (tu[5] - tu[4]);
    nr[number] = 2;
    nr[number + 1] = 3;
    delta[number] = step2;
    delta[number + 1] = step2;
    number += 2;
L30:
    if (id1 > 0) {
	goto L40;
    }
    ++number;
    nr[number - 1] = 4;
    delta[number - 1] = step[2];
L40:
    if (iop1 == 0) {
	goto L50;
    }
    if (ider[4] != 0) {
	goto L50;
    }
    step2 = step[2] * three / (tu[*nu] - tu[*nu - 4]);
    nr[number] = 5;
    nr[number + 1] = 6;
    delta[number] = step2;
    delta[number + 1] = step2;
    number += 2;
L50:
    if (number == 0) {
	return 0;
    }
/* the sum of squared residulas sq is a quadratic polynomial in the */
/* parameters g(j). we determine the unknown coefficients of this */
/* polymomial by calculating (number+1)*(number+2)/2 different splines */
/* according to specific values for g(j). */
    i__1 = number;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l = nr[i__ - 1];
	step1 = delta[i__ - 1];
	drr[l - 1] = dr[l] + step1;
	fpgrsp_(ifsu, ifsv, ifbu, ifbv, &c__1, &u[1], mu, &v[1], mv, &r__[1], 
		mr, drr, &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &c__[1], nc,
		 &sum[i__ - 1], fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[lsu], &
		wrk[lsv], &wrk[lri], &wrk[lq], &wrk[lau], &wrk[lav1], &wrk[
		lav2], &wrk[lbu], &wrk[lbv], &wrk[la0], &wrk[la1], &wrk[lb0], 
		&wrk[lb1], &wrk[lc0], &wrk[lc1], &wrk[lcs], &nru[1], &nrv[1]);
	if (id0 == 0) {
/* Computing 2nd power */
	    r__1 = *r0 - drr[0];
	    sq0 = r__1 * r__1;
	}
	if (id1 == 0) {
/* Computing 2nd power */
	    r__1 = *r1 - drr[3];
	    sq1 = r__1 * r__1;
	}
	sum[i__ - 1] = sum[i__ - 1] + sq0 + sq1;
	drr[l - 1] = dr[l] - step1;
	fpgrsp_(ifsu, ifsv, ifbu, ifbv, &c__1, &u[1], mu, &v[1], mv, &r__[1], 
		mr, drr, &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &c__[1], nc,
		 &sqq, fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[lsu], &wrk[lsv],
		 &wrk[lri], &wrk[lq], &wrk[lau], &wrk[lav1], &wrk[lav2], &wrk[
		lbu], &wrk[lbv], &wrk[la0], &wrk[la1], &wrk[lb0], &wrk[lb1], &
		wrk[lc0], &wrk[lc1], &wrk[lcs], &nru[1], &nrv[1]);
	if (id0 == 0) {
/* Computing 2nd power */
	    r__1 = *r0 - drr[0];
	    sq0 = r__1 * r__1;
	}
	if (id1 == 0) {
/* Computing 2nd power */
	    r__1 = *r1 - drr[3];
	    sq1 = r__1 * r__1;
	}
	sqq = sqq + sq0 + sq1;
	drr[l - 1] = dr[l];
/* Computing 2nd power */
	r__1 = step1;
	a[i__ + i__ * 6 - 7] = (sum[i__ - 1] + sqq - sq - sq) / (r__1 * r__1);
	if (a[i__ + i__ * 6 - 7] <= (float)0.) {
	    goto L110;
	}
	g[i__ - 1] = (sqq - sum[i__ - 1]) / (step1 + step1);
/* L60: */
    }
    if (number == 1) {
	goto L90;
    }
    i__1 = number;
    for (i__ = 2; i__ <= i__1; ++i__) {
	l1 = nr[i__ - 1];
	step1 = delta[i__ - 1];
	drr[l1 - 1] = dr[l1] + step1;
	i1 = i__ - 1;
	i__2 = i1;
	for (j = 1; j <= i__2; ++j) {
	    l2 = nr[j - 1];
	    step2 = delta[j - 1];
	    drr[l2 - 1] = dr[l2] + step2;
	    fpgrsp_(ifsu, ifsv, ifbu, ifbv, &c__1, &u[1], mu, &v[1], mv, &r__[
		    1], mr, drr, &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &
		    c__[1], nc, &sqq, fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[
		    lsu], &wrk[lsv], &wrk[lri], &wrk[lq], &wrk[lau], &wrk[
		    lav1], &wrk[lav2], &wrk[lbu], &wrk[lbv], &wrk[la0], &wrk[
		    la1], &wrk[lb0], &wrk[lb1], &wrk[lc0], &wrk[lc1], &wrk[
		    lcs], &nru[1], &nrv[1]);
	    if (id0 == 0) {
/* Computing 2nd power */
		r__1 = *r0 - drr[0];
		sq0 = r__1 * r__1;
	    }
	    if (id1 == 0) {
/* Computing 2nd power */
		r__1 = *r1 - drr[3];
		sq1 = r__1 * r__1;
	    }
	    sqq = sqq + sq0 + sq1;
	    a[i__ + j * 6 - 7] = (sq + sqq - sum[i__ - 1] - sum[j - 1]) / (
		    step1 * step2);
	    drr[l2 - 1] = dr[l2];
/* L70: */
	}
	drr[l1 - 1] = dr[l1];
/* L80: */
    }
/* the optimal values g(j) are found as the solution of the system */
/* d (sq) / d (g(j)) = 0 , j=1,...,number. */
L90:
    fpsysy_(a, &number, g);
    i__1 = number;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l = nr[i__ - 1];
	dr[l] += g[i__ - 1];
/* L100: */
    }
/* we determine the spline sp(u,v) according to the optimal values g(j). */
L110:
    fpgrsp_(ifsu, ifsv, ifbu, ifbv, &c__0, &u[1], mu, &v[1], mv, &r__[1], mr, 
	    &dr[1], &iop0, &iop1, &tu[1], nu, &tv[1], nv, p, &c__[1], nc, &sq,
	     fp, &fpu[1], &fpv[1], &mm, &mvnu, &wrk[lsu], &wrk[lsv], &wrk[lri]
	    , &wrk[lq], &wrk[lau], &wrk[lav1], &wrk[lav2], &wrk[lbu], &wrk[
	    lbv], &wrk[la0], &wrk[la1], &wrk[lb0], &wrk[lb1], &wrk[lc0], &wrk[
	    lc1], &wrk[lcs], &nru[1], &nrv[1]);
    if (id0 == 0) {
/* Computing 2nd power */
	r__1 = *r0 - dr[1];
	sq0 = r__1 * r__1;
    }
    if (id1 == 0) {
/* Computing 2nd power */
	r__1 = *r1 - dr[4];
	sq1 = r__1 * r__1;
    }
    sq = sq + sq0 + sq1;
    return 0;
} /* fpopsp_ */

/* Subroutine */ int fporde_(x, y, m, kx, ky, tx, nx, ty, ny, nummer, index, 
	nreg)
real *x, *y;
integer *m, *kx, *ky;
real *tx;
integer *nx;
real *ty;
integer *ny, *nummer, *index, *nreg;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k, l, k1, l1, im;
    static real xi, yi;
    static integer kx1, ky1, num, nyy, nk1x, nk1y;

/*  subroutine fporde sorts the data points (x(i),y(i)),i=1,2,...,m */
/*  according to the panel tx(l)<=x<tx(l+1),ty(k)<=y<ty(k+1), they belong */
/*  to. for each panel a stack is constructed  containing the numbers */
/*  of data points lying inside; index(j),j=1,2,...,nreg points to the */
/*  first data point in the jth panel while nummer(i),i=1,2,...,m gives */
/*  the number of the next data point in the panel. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --nummer;
    --y;
    --x;
    --tx;
    --ty;
    --index;

    /* Function Body */
    kx1 = *kx + 1;
    ky1 = *ky + 1;
    nk1x = *nx - kx1;
    nk1y = *ny - ky1;
    nyy = nk1y - *ky;
    i__1 = *nreg;
    for (i__ = 1; i__ <= i__1; ++i__) {
	index[i__] = 0;
/* L10: */
    }
    i__1 = *m;
    for (im = 1; im <= i__1; ++im) {
	xi = x[im];
	yi = y[im];
	l = kx1;
	l1 = l + 1;
L20:
	if (xi < tx[l1] || l == nk1x) {
	    goto L30;
	}
	l = l1;
	l1 = l + 1;
	goto L20;
L30:
	k = ky1;
	k1 = k + 1;
L40:
	if (yi < ty[k1] || k == nk1y) {
	    goto L50;
	}
	k = k1;
	k1 = k + 1;
	goto L40;
L50:
	num = (l - kx1) * nyy + k - *ky;
	nummer[im] = index[num];
	index[num] = im;
/* L60: */
    }
    return 0;
} /* fporde_ */

/* Subroutine */ int fppara_(iopt, idim, m, u, mx, x, w, ub, ue, k, s, nest, 
	tol, maxit, k1, k2, n, t, nc, c__, fp, fpint, z__, a, b, g, q, nrdata,
	 ier)
integer *iopt, *idim, *m;
real *u;
integer *mx;
real *x, *w, *ub, *ue;
integer *k;
real *s;
integer *nest;
real *tol;
integer *maxit, *k1, *k2, *n;
real *t;
integer *nc;
real *c__, *fp, *fpint, *z__, *a, *b, *g, *q;
integer *nrdata, *ier;
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, g_dim1, g_offset, q_dim1, 
	    q_offset, i__1, i__2, i__3, i__4, i__5;
    real r__1;

    /* Local variables */
    static real half;
    static integer nmin, iter, nmax;
    static real fpms, term, pinv, h__[7];
    static integer i__, j, l;
    static real p, fpold, fpart, f1, f2, f3;
    static integer i1, i2;
    static real store;
    static integer i3, j1;
    static real p1, p2, p3;
    static integer j2, k3, l0, nplus, nrint, n8, jj;
    extern /* Subroutine */ int fpback_();
    static integer it;
    static real ui, rn, wi, xi[10];
    extern /* Subroutine */ int fpdisc_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpbspl_(), fprota_(), fpgivs_(), fpknot_();
    static real fp0;
    static integer mk1, nk1;
    static real acc, fac, one, cos__, sin__;
    static integer new__;
    static real piv;
    static integer ich1, ich3;
    static real con1, con4, con9;
    static integer npl1;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references */
/*  ..subroutine references.. */
/*    fpback,fpbspl,fpgivs,fpdisc,fpknot,fprota */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --w;
    --u;
    --x;
    --nrdata;
    --fpint;
    --t;
    q_dim1 = *m;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    a_dim1 = *nest;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    g_dim1 = *nest;
    g_offset = 1 + g_dim1 * 1;
    g -= g_offset;
    b_dim1 = *nest;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;
    --z__;
    --c__;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
    half = (float).5;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 1: determination of the number of knots and their position     c */
/*  **************************************************************      c */
/*  given a set of knots we compute the least-squares curve sinf(u),    c */
/*  and the corresponding sum of squared residuals fp=f(p=inf).         c */
/*  if iopt=-1 sinf(u) is the requested curve.                          c */
/*  if iopt=0 or iopt=1 we check whether we can accept the knots:       c */
/*    if fp <=s we will continue with the current set of knots.         c */
/*    if fp > s we will increase the number of knots and compute the    c */
/*       corresponding least-squares curve until finally fp<=s.         c */
/*    the initial choice of knots depends on the value of s and iopt.   c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*    knots equals nmax = m+k+1.                                        c */
/*    if s > 0 and                                                      c */
/*      iopt=0 we first compute the least-squares polynomial curve of   c */
/*      degree k; n = nmin = 2*k+2                                      c */
/*      iopt=1 we start with the set of knots found at the last         c */
/*      call of the routine, except for the case that s > fp0; then     c */
/*      we compute directly the polynomial curve of degree k.           c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  determine nmin, the number of knots for polynomial approximation. */
    nmin = *k1 << 1;
    if (*iopt < 0) {
	goto L60;
    }
/*  calculation of acc, the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  determine nmax, the number of knots for spline interpolation. */
    nmax = *m + *k1;
    if (*s > (float)0.) {
	goto L45;
    }
/*  if s=0, s(u) is an interpolating curve. */
/*  test whether the required storage space exceeds the available one. */
    *n = nmax;
    if (nmax > *nest) {
	goto L420;
    }
/*  find the position of the interior knots in case of interpolation. */
L10:
    mk1 = *m - *k1;
    if (mk1 == 0) {
	goto L60;
    }
    k3 = *k / 2;
    i__ = *k2;
    j = k3 + 2;
    if (k3 << 1 == *k) {
	goto L30;
    }
    i__1 = mk1;
    for (l = 1; l <= i__1; ++l) {
	t[i__] = u[j];
	++i__;
	++j;
/* L20: */
    }
    goto L60;
L30:
    i__1 = mk1;
    for (l = 1; l <= i__1; ++l) {
	t[i__] = (u[j] + u[j - 1]) * half;
	++i__;
	++j;
/* L40: */
    }
    goto L60;
/*  if s>0 our initial choice of knots depends on the value of iopt. */
/*  if iopt=0 or iopt=1 and s>=fp0, we start computing the least-squares */
/*  polynomial curve which is a spline curve without interior knots. */
/*  if iopt=1 and fp0>s we start computing the least squares spline curve */
/*  according to the set of knots found at the last call of the routine. */
L45:
    if (*iopt == 0) {
	goto L50;
    }
    if (*n == nmin) {
	goto L50;
    }
    fp0 = fpint[*n];
    fpold = fpint[*n - 1];
    nplus = nrdata[*n];
    if (fp0 > *s) {
	goto L60;
    }
L50:
    *n = nmin;
    fpold = (float)0.;
    nplus = 0;
    nrdata[1] = *m - 2;
/*  main loop for the different sets of knots. m is a save upper bound */
/*  for the number of trials. */
L60:
    i__1 = *m;
    for (iter = 1; iter <= i__1; ++iter) {
	if (*n == nmin) {
	    *ier = -2;
	}
/*  find nrint, tne number of knot intervals. */
	nrint = *n - nmin + 1;
/*  find the position of the additional knots which are needed for */
/*  the b-spline representation of s(u). */
	nk1 = *n - *k1;
	i__ = *n;
	i__2 = *k1;
	for (j = 1; j <= i__2; ++j) {
	    t[j] = *ub;
	    t[i__] = *ue;
	    --i__;
/* L70: */
	}
/*  compute the b-spline coefficients of the least-squares spline curve */
/*  sinf(u). the observation matrix a is built up row by row and */
/*  reduced to upper triangular form by givens transformations. */
/*  at the same time fp=f(p=inf) is computed. */
	*fp = (float)0.;
/*  initialize the b-spline coefficients and the observation matrix a. */
	i__2 = *nc;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    z__[i__] = (float)0.;
/* L75: */
	}
	i__2 = nk1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = *k1;
	    for (j = 1; j <= i__3; ++j) {
		a[i__ + j * a_dim1] = (float)0.;
/* L80: */
	    }
	}
	l = *k1;
	jj = 0;
	i__3 = *m;
	for (it = 1; it <= i__3; ++it) {
/*  fetch the current data point u(it),x(it). */
	    ui = u[it];
	    wi = w[it];
	    i__2 = *idim;
	    for (j = 1; j <= i__2; ++j) {
		++jj;
		xi[j - 1] = x[jj] * wi;
/* L83: */
	    }
/*  search for knot interval t(l) <= ui < t(l+1). */
L85:
	    if (ui < t[l + 1] || l == nk1) {
		goto L90;
	    }
	    ++l;
	    goto L85;
/*  evaluate the (k+1) non-zero b-splines at ui and store them in q. */
L90:
	    fpbspl_(&t[1], n, k, &ui, &l, h__);
	    i__2 = *k1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		q[it + i__ * q_dim1] = h__[i__ - 1];
		h__[i__ - 1] *= wi;
/* L95: */
	    }
/*  rotate the new row of the observation matrix into triangle. */
	    j = l - *k1;
	    i__2 = *k1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		++j;
		piv = h__[i__ - 1];
		if (piv == (float)0.) {
		    goto L110;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a[j + a_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		j1 = j;
		i__4 = *idim;
		for (j2 = 1; j2 <= i__4; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &z__[j1]);
		    j1 += *n;
/* L97: */
		}
		if (i__ == *k1) {
		    goto L120;
		}
		i2 = 1;
		i3 = i__ + 1;
		i__4 = *k1;
		for (i1 = i3; i1 <= i__4; ++i1) {
		    ++i2;
/*  transformations to left hand side. */
		    fprota_(&cos__, &sin__, &h__[i1 - 1], &a[j + i2 * a_dim1])
			    ;
/* L100: */
		}
L110:
		;
	    }
/*  add contribution of this row to the sum of squares of residual */
/*  right hand sides. */
L120:
	    i__2 = *idim;
	    for (j2 = 1; j2 <= i__2; ++j2) {
/* Computing 2nd power */
		r__1 = xi[j2 - 1];
		*fp += r__1 * r__1;
/* L125: */
	    }
/* L130: */
	}
	if (*ier == -2) {
	    fp0 = *fp;
	}
	fpint[*n] = fp0;
	fpint[*n - 1] = fpold;
	nrdata[*n] = nplus;
/*  backward substitution to obtain the b-spline coefficients. */
	j1 = 1;
	i__3 = *idim;
	for (j2 = 1; j2 <= i__3; ++j2) {
	    fpback_(&a[a_offset], &z__[j1], &nk1, k1, &c__[j1], nest);
	    j1 += *n;
/* L135: */
	}
/*  test whether the approximation sinf(u) is an acceptable solution. */
	if (*iopt < 0) {
	    goto L440;
	}
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  if f(p=inf) < s accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L250;
	}
/*  if n = nmax, sinf(u) is an interpolating spline curve. */
	if (*n == nmax) {
	    goto L430;
	}
/*  increase the number of knots. */
/*  if n=nest we cannot increase the number of knots because of */
/*  the storage capacity limitation. */
	if (*n == *nest) {
	    goto L420;
	}
/*  determine the number of knots nplus we are going to add. */
	if (*ier == 0) {
	    goto L140;
	}
	nplus = 1;
	*ier = 0;
	goto L150;
L140:
	npl1 = nplus << 1;
	rn = (real) nplus;
	if (fpold - *fp > acc) {
	    npl1 = rn * fpms / (fpold - *fp);
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = nplus / 2, i__4 = max(i__4,i__5);
	i__3 = nplus << 1, i__2 = max(i__4,1);
	nplus = min(i__3,i__2);
L150:
	fpold = *fp;
/*  compute the sum of squared residuals for each knot interval */
/*  t(j+k) <= u(i) <= t(j+k+1) and store it in fpint(j),j=1,2,...nrint. */
	fpart = (float)0.;
	i__ = 1;
	l = *k2;
	new__ = 0;
	jj = 0;
	i__3 = *m;
	for (it = 1; it <= i__3; ++it) {
	    if (u[it] < t[l] || l > nk1) {
		goto L160;
	    }
	    new__ = 1;
	    ++l;
L160:
	    term = (float)0.;
	    l0 = l - *k2;
	    i__2 = *idim;
	    for (j2 = 1; j2 <= i__2; ++j2) {
		fac = (float)0.;
		j1 = l0;
		i__4 = *k1;
		for (j = 1; j <= i__4; ++j) {
		    ++j1;
		    fac += c__[j1] * q[it + j * q_dim1];
/* L170: */
		}
		++jj;
/* Computing 2nd power */
		r__1 = w[it] * (fac - x[jj]);
		term += r__1 * r__1;
		l0 += *n;
/* L175: */
	    }
	    fpart += term;
	    if (new__ == 0) {
		goto L180;
	    }
	    store = term * half;
	    fpint[i__] = fpart - store;
	    ++i__;
	    fpart = store;
	    new__ = 0;
L180:
	    ;
	}
	fpint[nrint] = fpart;
	i__3 = nplus;
	for (l = 1; l <= i__3; ++l) {
/*  add a new knot. */
	    fpknot_(&u[1], m, &t[1], n, &fpint[1], &nrdata[1], &nrint, nest, &
		    c__1);
/*  if n=nmax we locate the knots as for interpolation */
	    if (*n == nmax) {
		goto L10;
	    }
/*  test whether we cannot further increase the number of knots. */
	    if (*n == *nest) {
		goto L200;
	    }
/* L190: */
	}
/*  restart the computations with the new set of knots. */
L200:
	;
    }
/*  test whether the least-squares kth degree polynomial curve is a */
/*  solution of our approximation problem. */
L250:
    if (*ier == -2) {
	goto L440;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 2: determination of the smoothing spline curve sp(u).          c */
/*  **********************************************************          c */
/*  we have determined the number of knots and their position.          c */
/*  we now compute the b-spline coefficients of the smoothing curve     c */
/*  sp(u). the observation matrix a is extended by the rows of matrix   c */
/*  b expressing that the kth derivative discontinuities of sp(u) at    c */
/*  the interior knots t(k+2),...t(n-k-1) must be zero. the corres-     c */
/*  ponding weights of these additional rows are set to 1/p.            c */
/*  iteratively we then have to determine the value of p such that f(p),c */
/*  the sum of squared residuals be = s. we already know that the least c */
/*  squares kth degree polynomial curve corresponds to p=0, and that    c */
/*  the least-squares spline curve corresponds to p=infinity. the       c */
/*  iteration process which is proposed here, makes use of rational     c */
/*  interpolation. since f(p) is a convex and strictly decreasing       c */
/*  function of p, it can be approximated by a rational function        c */
/*  r(p) = (u*p+v)/(p+w). three values of p(p1,p2,p3) with correspond-  c */
/*  ing values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s) are used      c */
/*  to calculate the new value of p such that r(p)=s. convergence is    c */
/*  guaranteed by taking f1>0 and f3<0.                                 c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  evaluate the discontinuity jump of the kth derivative of the */
/*  b-splines at the knots t(l),l=k+2,...n-k-1 and store in b. */
    fpdisc_(&t[1], n, k2, &b[b_offset], nest);
/*  initial value for p. */
    p1 = (float)0.;
    f1 = fp0 - *s;
    p3 = -one;
    f3 = fpms;
    p = (float)0.;
    i__1 = nk1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p += a[i__ + a_dim1];
/* L252: */
    }
    rn = (real) nk1;
    p = rn / p;
    ich1 = 0;
    ich3 = 0;
    n8 = *n - nmin;
/*  iteration process to find the root of f(p) = s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  the rows of matrix b with weight 1/p are rotated into the */
/*  triangularised observation matrix a which is stored in g. */
	pinv = one / p;
	i__3 = *nc;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    c__[i__] = z__[i__];
/* L255: */
	}
	i__3 = nk1;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    g[i__ + *k2 * g_dim1] = (float)0.;
	    i__2 = *k1;
	    for (j = 1; j <= i__2; ++j) {
		g[i__ + j * g_dim1] = a[i__ + j * a_dim1];
/* L260: */
	    }
	}
	i__2 = n8;
	for (it = 1; it <= i__2; ++it) {
/*  the row of matrix b is rotated into triangle by givens transformation */
	    i__3 = *k2;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h__[i__ - 1] = b[it + i__ * b_dim1] * pinv;
/* L270: */
	    }
	    i__3 = *idim;
	    for (j = 1; j <= i__3; ++j) {
		xi[j - 1] = (float)0.;
/* L275: */
	    }
	    i__3 = nk1;
	    for (j = it; j <= i__3; ++j) {
		piv = h__[0];
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &g[j + g_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		j1 = j;
		i__4 = *idim;
		for (j2 = 1; j2 <= i__4; ++j2) {
		    fprota_(&cos__, &sin__, &xi[j2 - 1], &c__[j1]);
		    j1 += *n;
/* L277: */
		}
		if (j == nk1) {
		    goto L300;
		}
		i2 = *k1;
		if (j > n8) {
		    i2 = nk1 - j;
		}
		i__4 = i2;
		for (i__ = 1; i__ <= i__4; ++i__) {
/*  transformations to left hand side. */
		    i1 = i__ + 1;
		    fprota_(&cos__, &sin__, &h__[i1 - 1], &g[j + i1 * g_dim1])
			    ;
		    h__[i__ - 1] = h__[i1 - 1];
/* L280: */
		}
		h__[i2] = (float)0.;
/* L290: */
	    }
L300:
	    ;
	}
/*  backward substitution to obtain the b-spline coefficients. */
	j1 = 1;
	i__2 = *idim;
	for (j2 = 1; j2 <= i__2; ++j2) {
	    fpback_(&g[g_offset], &c__[j1], &nk1, k2, &c__[j1], nest);
	    j1 += *n;
/* L305: */
	}
/*  computation of f(p). */
	*fp = (float)0.;
	l = *k2;
	jj = 0;
	i__2 = *m;
	for (it = 1; it <= i__2; ++it) {
	    if (u[it] < t[l] || l > nk1) {
		goto L310;
	    }
	    ++l;
L310:
	    l0 = l - *k2;
	    term = (float)0.;
	    i__3 = *idim;
	    for (j2 = 1; j2 <= i__3; ++j2) {
		fac = (float)0.;
		j1 = l0;
		i__4 = *k1;
		for (j = 1; j <= i__4; ++j) {
		    ++j1;
		    fac += c__[j1] * q[it + j * q_dim1];
/* L320: */
		}
		++jj;
/* Computing 2nd power */
		r__1 = fac - x[jj];
		term += r__1 * r__1;
		l0 += *n;
/* L325: */
	    }
/* Computing 2nd power */
	    r__1 = w[it];
	    *fp += term * (r__1 * r__1);
/* L330: */
	}
/*  test whether the approximation sp(u) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  test whether the maximal number of iterations is reached. */
	if (iter == *maxit) {
	    goto L400;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L340;
	}
	if (f2 - f3 > acc) {
	    goto L335;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L360;
L335:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L340:
	if (ich1 != 0) {
	    goto L350;
	}
	if (f1 - f2 > acc) {
	    goto L345;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L360;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L360;
L345:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L350:
	if (f2 >= f1 || f2 <= f3) {
	    goto L410;
	}
/*  find the new value for p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L360:
	;
    }
/*  error codes and messages. */
L400:
    *ier = 3;
    goto L440;
L410:
    *ier = 2;
    goto L440;
L420:
    *ier = 1;
    goto L440;
L430:
    *ier = -1;
L440:
    return 0;
} /* fppara_ */

/* Subroutine */ int fppasu_(iopt, ipar, idim, u, mu, v, mv, z__, mz, s, 
	nuest, nvest, tol, maxit, nc, nu, tu, nv, tv, c__, fp, fp0, fpold, 
	reducu, reducv, fpintu, fpintv, lastdi, nplusu, nplusv, nru, nrv, 
	nrdatu, nrdatv, wrk, lwrk, ier)
integer *iopt, *ipar, *idim;
real *u;
integer *mu;
real *v;
integer *mv;
real *z__;
integer *mz;
real *s;
integer *nuest, *nvest;
real *tol;
integer *maxit, *nc, *nu;
real *tu;
integer *nv;
real *tv, *c__, *fp, *fp0, *fpold, *reducu, *reducv, *fpintu, *fpintv;
integer *lastdi, *nplusu, *nplusv, *nru, *nrv, *nrdatu, *nrdatv;
real *wrk;
integer *lwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;

    /* Local variables */
    static integer ifbu, ifbv, ncof, iter;
    static real fpms;
    static integer ifsu, ifsv;
    static real peru, perv;
    static integer nplu, nplv, mvnu, i__, j, l;
    static real p;
    static integer nminu;
    static real f1, f2, f3;
    static integer nmaxu, nmaxv, nminv, l1, l2;
    static real p1, p2, p3;
    static integer l3, l4;
    static real ub, vb, ue, ve;
    static integer mm, lq;
    static real rn;
    extern /* Subroutine */ int fpgrpa_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpknot_();
    static integer nrintu, nrintv;
    static real acc;
    static integer laa;
    static real one;
    static integer lau, lav, lbu, lbv, lri, nue, nve, mpm, nuk, lsu, lsv, nuu,
	     nvv, ich1, ich3;
    static real con1;
    static integer lau1;
    static real con4;
    static integer lav1;
    static real con9;
    static integer npl1, nk1u, nk1v;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpgrpa,fpknot */
/*  .. */
/*   set constants */
    /* Parameter adjustments */
    --ipar;
    --nru;
    --u;
    --nrv;
    --v;
    --z__;
    --nrdatu;
    --fpintu;
    --tu;
    --nrdatv;
    --fpintv;
    --tv;
    --c__;
    --wrk;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
/*  set boundaries of the approximation domain */
    ub = u[1];
    ue = u[*mu];
    vb = v[1];
    ve = v[*mv];
/*  we partition the working space. */
    lsu = 1;
    lsv = lsu + (*mu << 2);
    lri = lsv + (*mv << 2);
    mm = max(*nuest,*mv);
    lq = lri + mm * *idim;
    mvnu = *nuest * *mv * *idim;
    lau = lq + mvnu;
    nuk = *nuest * 5;
    lbu = lau + nuk;
    lav = lbu + nuk;
    nuk = *nvest * 5;
    lbv = lav + nuk;
    laa = lbv + nuk;
    lau1 = lau;
    if (ipar[1] == 0) {
	goto L10;
    }
    peru = ue - ub;
    lau1 = laa;
    laa += *nuest << 2;
L10:
    lav1 = lav;
    if (ipar[2] == 0) {
	goto L20;
    }
    perv = ve - vb;
    lav1 = laa;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 1: determination of the number of knots and their position.     c */
/* ****************************************************************     c */
/*  given a set of knots we compute the least-squares spline sinf(u,v), c */
/*  and the corresponding sum of squared residuals fp=f(p=inf).         c */
/*  if iopt=-1  sinf(u,v) is the requested approximation.               c */
/*  if iopt=0 or iopt=1 we check whether we can accept the knots:       c */
/*    if fp <=s we will continue with the current set of knots.         c */
/*    if fp > s we will increase the number of knots and compute the    c */
/*       corresponding least-squares spline until finally fp<=s.        c */
/*    the initial choice of knots depends on the value of s and iopt.   c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*    knots equals nmaxu = mu+4+2*ipar(1) and  nmaxv = mv+4+2*ipar(2)   c */
/*    if s>0 and                                                        c */
/*     *iopt=0 we first compute the least-squares polynomial            c */
/*          nu=nminu=8 and nv=nminv=8                                   c */
/*     *iopt=1 we start with the knots found at the last call of the    c */
/*      routine, except for the case that s > fp0; then we can compute  c */
/*      the least-squares polynomial directly.                          c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  determine the number of knots for polynomial approximation. */
L20:
    nminu = 8;
    nminv = 8;
    if (*iopt < 0) {
	goto L100;
    }
/*  acc denotes the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  find nmaxu and nmaxv which denote the number of knots in u- and v- */
/*  direction in case of spline interpolation. */
    nmaxu = *mu + 4 + (ipar[1] << 1);
    nmaxv = *mv + 4 + (ipar[2] << 1);
/*  find nue and nve which denote the maximum number of knots */
/*  allowed in each direction */
    nue = min(nmaxu,*nuest);
    nve = min(nmaxv,*nvest);
    if (*s > (float)0.) {
	goto L60;
    }
/*  if s = 0, s(u,v) is an interpolating spline. */
    *nu = nmaxu;
    *nv = nmaxv;
/*  test whether the required storage space exceeds the available one. */
    if (*nv > *nvest || *nu > *nuest) {
	goto L420;
    }
/*  find the position of the interior knots in case of interpolation. */
/*  the knots in the u-direction. */
    nuu = *nu - 8;
    if (nuu == 0) {
	goto L40;
    }
    i__ = 5;
    j = 3 - ipar[1];
    i__1 = nuu;
    for (l = 1; l <= i__1; ++l) {
	tu[i__] = u[j];
	++i__;
	++j;
/* L30: */
    }
/*  the knots in the v-direction. */
L40:
    nvv = *nv - 8;
    if (nvv == 0) {
	goto L60;
    }
    i__ = 5;
    j = 3 - ipar[2];
    i__1 = nvv;
    for (l = 1; l <= i__1; ++l) {
	tv[i__] = v[j];
	++i__;
	++j;
/* L50: */
    }
    goto L100;
/*  if s > 0 our initial choice of knots depends on the value of iopt. */
L60:
    if (*iopt == 0) {
	goto L90;
    }
    if (*fp0 <= *s) {
	goto L90;
    }
/*  if iopt=1 and fp0 > s we start computing the least- squares spline */
/*  according to the set of knots found at the last call of the routine. */
/*  we determine the number of grid coordinates u(i) inside each knot */
/*  interval (tu(l),tu(l+1)). */
    l = 5;
    j = 1;
    nrdatu[1] = 0;
    mpm = *mu - 1;
    i__1 = mpm;
    for (i__ = 2; i__ <= i__1; ++i__) {
	++nrdatu[j];
	if (u[i__] < tu[l]) {
	    goto L70;
	}
	--nrdatu[j];
	++l;
	++j;
	nrdatu[j] = 0;
L70:
	;
    }
/*  we determine the number of grid coordinates v(i) inside each knot */
/*  interval (tv(l),tv(l+1)). */
    l = 5;
    j = 1;
    nrdatv[1] = 0;
    mpm = *mv - 1;
    i__1 = mpm;
    for (i__ = 2; i__ <= i__1; ++i__) {
	++nrdatv[j];
	if (v[i__] < tv[l]) {
	    goto L80;
	}
	--nrdatv[j];
	++l;
	++j;
	nrdatv[j] = 0;
L80:
	;
    }
    goto L100;
/*  if iopt=0 or iopt=1 and s>=fp0, we start computing the least-squares */
/*  polynomial (which is a spline without interior knots). */
L90:
    *nu = nminu;
    *nv = nminv;
    nrdatu[1] = *mu - 2;
    nrdatv[1] = *mv - 2;
    *lastdi = 0;
    *nplusu = 0;
    *nplusv = 0;
    *fp0 = (float)0.;
    *fpold = (float)0.;
    *reducu = (float)0.;
    *reducv = (float)0.;
L100:
    mpm = *mu + *mv;
    ifsu = 0;
    ifsv = 0;
    ifbu = 0;
    ifbv = 0;
    p = -one;
/*  main loop for the different sets of knots.mpm=mu+mv is a save upper */
/*  bound for the number of trials. */
    i__1 = mpm;
    for (iter = 1; iter <= i__1; ++iter) {
	if (*nu == nminu && *nv == nminv) {
	    *ier = -2;
	}
/*  find nrintu (nrintv) which is the number of knot intervals in the */
/*  u-direction (v-direction). */
	nrintu = *nu - nminu + 1;
	nrintv = *nv - nminv + 1;
/*  find ncof, the number of b-spline coefficients for the current set */
/*  of knots. */
	nk1u = *nu - 4;
	nk1v = *nv - 4;
	ncof = nk1u * nk1v;
/*  find the position of the additional knots which are needed for the */
/*  b-spline representation of s(u,v). */
	if (ipar[1] != 0) {
	    goto L110;
	}
	i__ = *nu;
	for (j = 1; j <= 4; ++j) {
	    tu[j] = ub;
	    tu[i__] = ue;
	    --i__;
/* L105: */
	}
	goto L120;
L110:
	l1 = 4;
	l2 = l1;
	l3 = *nu - 3;
	l4 = l3;
	tu[l2] = ub;
	tu[l3] = ue;
	for (j = 1; j <= 3; ++j) {
	    ++l1;
	    --l2;
	    ++l3;
	    --l4;
	    tu[l2] = tu[l4] - peru;
	    tu[l3] = tu[l1] + peru;
/* L115: */
	}
L120:
	if (ipar[2] != 0) {
	    goto L130;
	}
	i__ = *nv;
	for (j = 1; j <= 4; ++j) {
	    tv[j] = vb;
	    tv[i__] = ve;
	    --i__;
/* L125: */
	}
	goto L140;
L130:
	l1 = 4;
	l2 = l1;
	l3 = *nv - 3;
	l4 = l3;
	tv[l2] = vb;
	tv[l3] = ve;
	for (j = 1; j <= 3; ++j) {
	    ++l1;
	    --l2;
	    ++l3;
	    --l4;
	    tv[l2] = tv[l4] - perv;
	    tv[l3] = tv[l1] + perv;
/* L135: */
	}
/*  find the least-squares spline sinf(u,v) and calculate for each knot */
/*  interval tu(j+3)<=u<=tu(j+4) (tv(j+3)<=v<=tv(j+4)) the sum */
/*  of squared residuals fpintu(j),j=1,2,...,nu-7 (fpintv(j),j=1,2,... */
/*  ,nv-7) for the data points having their absciss (ordinate)-value */
/*  belonging to that interval. */
/*  fp gives the total sum of squared residuals. */
L140:
	fpgrpa_(&ifsu, &ifsv, &ifbu, &ifbv, idim, &ipar[1], &u[1], mu, &v[1], 
		mv, &z__[1], mz, &tu[1], nu, &tv[1], nv, &p, &c__[1], nc, fp, 
		&fpintu[1], &fpintv[1], &mm, &mvnu, &wrk[lsu], &wrk[lsv], &
		wrk[lri], &wrk[lq], &wrk[lau], &wrk[lau1], &wrk[lav], &wrk[
		lav1], &wrk[lbu], &wrk[lbv], &nru[1], &nrv[1]);
	if (*ier == -2) {
	    *fp0 = *fp;
	}
/*  test whether the least-squares spline is an acceptable solution. */
	if (*iopt < 0) {
	    goto L440;
	}
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  if f(p=inf) < s, we accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L300;
	}
/*  if nu=nmaxu and nv=nmaxv, sinf(u,v) is an interpolating spline. */
	if (*nu == nmaxu && *nv == nmaxv) {
	    goto L430;
	}
/*  increase the number of knots. */
/*  if nu=nue and nv=nve we cannot further increase the number of knots */
/*  because of the storage capacity limitation. */
	if (*nu == nue && *nv == nve) {
	    goto L420;
	}
	*ier = 0;
/*  adjust the parameter reducu or reducv according to the direction */
/*  in which the last added knots were located. */
	if (*lastdi < 0) {
	    goto L150;
	} else if (*lastdi == 0) {
	    goto L170;
	} else {
	    goto L160;
	}
L150:
	*reducu = *fpold - *fp;
	goto L170;
L160:
	*reducv = *fpold - *fp;
/*  store the sum of squared residuals for the current set of knots. */
L170:
	*fpold = *fp;
/*  find nplu, the number of knots we should add in the u-direction. */
	nplu = 1;
	if (*nu == nminu) {
	    goto L180;
	}
	npl1 = *nplusu << 1;
	rn = (real) (*nplusu);
	if (*reducu > acc) {
	    npl1 = rn * fpms / *reducu;
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = *nplusu / 2, i__4 = max(i__4,i__5);
	i__2 = *nplusu << 1, i__3 = max(i__4,1);
	nplu = min(i__2,i__3);
/*  find nplv, the number of knots we should add in the v-direction. */
L180:
	nplv = 1;
	if (*nv == nminv) {
	    goto L190;
	}
	npl1 = *nplusv << 1;
	rn = (real) (*nplusv);
	if (*reducv > acc) {
	    npl1 = rn * fpms / *reducv;
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = *nplusv / 2, i__4 = max(i__4,i__5);
	i__2 = *nplusv << 1, i__3 = max(i__4,1);
	nplv = min(i__2,i__3);
L190:
	if ((i__2 = nplu - nplv) < 0) {
	    goto L210;
	} else if (i__2 == 0) {
	    goto L200;
	} else {
	    goto L230;
	}
L200:
	if (*lastdi < 0) {
	    goto L230;
	}
L210:
	if (*nu == nue) {
	    goto L230;
	}
/*  addition in the u-direction. */
	*lastdi = -1;
	*nplusu = nplu;
	ifsu = 0;
	i__2 = *nplusu;
	for (l = 1; l <= i__2; ++l) {
/*  add a new knot in the u-direction */
	    fpknot_(&u[1], mu, &tu[1], nu, &fpintu[1], &nrdatu[1], &nrintu, 
		    nuest, &c__1);
/*  test whether we cannot further increase the number of knots in the */
/*  u-direction. */
	    if (*nu == nue) {
		goto L250;
	    }
/* L220: */
	}
	goto L250;
L230:
	if (*nv == nve) {
	    goto L210;
	}
/*  addition in the v-direction. */
	*lastdi = 1;
	*nplusv = nplv;
	ifsv = 0;
	i__2 = *nplusv;
	for (l = 1; l <= i__2; ++l) {
/*  add a new knot in the v-direction. */
	    fpknot_(&v[1], mv, &tv[1], nv, &fpintv[1], &nrdatv[1], &nrintv, 
		    nvest, &c__1);
/*  test whether we cannot further increase the number of knots in the */
/*  v-direction. */
	    if (*nv == nve) {
		goto L250;
	    }
/* L240: */
	}
/*  restart the computations with the new set of knots. */
L250:
	;
    }
/*  test whether the least-squares polynomial is a solution of our */
/*  approximation problem. */
L300:
    if (*ier == -2) {
	goto L440;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 2: determination of the smoothing spline sp(u,v)                c */
/* *****************************************************                c */
/*  we have determined the number of knots and their position. we now   c */
/*  compute the b-spline coefficients of the smoothing spline sp(u,v).  c */
/*  this smoothing spline varies with the parameter p in such a way thatc */
/*  f(p)=suml=1,idim(sumi=1,mu(sumj=1,mv((z(i,j,l)-sp(u(i),v(j),l))**2) c */
/*  is a continuous, strictly decreasing function of p. moreover the    c */
/*  least-squares polynomial corresponds to p=0 and the least-squares   c */
/*  spline to p=infinity. iteratively we then have to determine the     c */
/*  positive value of p such that f(p)=s. the process which is proposed c */
/*  here makes use of rational interpolation. f(p) is approximated by a c */
/*  rational function r(p)=(u*p+v)/(p+w); three values of p (p1,p2,p3)  c */
/*  with corresponding values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s)c */
/*  are used to calculate the new value of p such that r(p)=s.          c */
/*  convergence is guaranteed by taking f1 > 0 and f3 < 0.              c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  initial value for p. */
    p1 = (float)0.;
    f1 = *fp0 - *s;
    p3 = -one;
    f3 = fpms;
    p = one;
    ich1 = 0;
    ich3 = 0;
/*  iteration process to find the root of f(p)=s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find the smoothing spline sp(u,v) and the corresponding sum of */
/*  squared residuals fp. */
	fpgrpa_(&ifsu, &ifsv, &ifbu, &ifbv, idim, &ipar[1], &u[1], mu, &v[1], 
		mv, &z__[1], mz, &tu[1], nu, &tv[1], nv, &p, &c__[1], nc, fp, 
		&fpintu[1], &fpintv[1], &mm, &mvnu, &wrk[lsu], &wrk[lsv], &
		wrk[lri], &wrk[lq], &wrk[lau], &wrk[lau1], &wrk[lav], &wrk[
		lav1], &wrk[lbu], &wrk[lbv], &nru[1], &nrv[1]);
/*  test whether the approximation sp(u,v) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  test whether the maximum allowable number of iterations has been */
/*  reached. */
	if (iter == *maxit) {
	    goto L400;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L320;
	}
	if (f2 - f3 > acc) {
	    goto L310;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L350;
L310:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L320:
	if (ich1 != 0) {
	    goto L340;
	}
	if (f1 - f2 > acc) {
	    goto L330;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L350;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L350;
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L330:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
L340:
	if (f2 >= f1 || f2 <= f3) {
	    goto L410;
	}
/*  find the new value of p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L350:
	;
    }
/*  error codes and messages. */
L400:
    *ier = 3;
    goto L440;
L410:
    *ier = 2;
    goto L440;
L420:
    *ier = 1;
    goto L440;
L430:
    *ier = -1;
    *fp = (float)0.;
L440:
    return 0;
} /* fppasu_ */

/* Subroutine */ int fpperi_(iopt, x, y, w, m, k, s, nest, tol, maxit, k1, k2,
	 n, t, c__, fp, fpint, z__, a1, a2, b, g1, g2, q, nrdata, ier)
integer *iopt;
real *x, *y, *w;
integer *m, *k;
real *s;
integer *nest;
real *tol;
integer *maxit, *k1, *k2, *n;
real *t, *c__, *fp, *fpint, *z__, *a1, *a2, *b, *g1, *g2, *q;
integer *nrdata, *ier;
{
    /* System generated locals */
    integer a1_dim1, a1_offset, a2_dim1, a2_offset, b_dim1, b_offset, g1_dim1,
	     g1_offset, g2_dim1, g2_offset, q_dim1, q_offset, i__1, i__2, 
	    i__3, i__4, i__5;
    real r__1;

    /* Local variables */
    static real half;
    static integer jper, nmin, iter, nmax;
    static real fpms, term, pinv, h__[6];
    static integer i__, j, l;
    static real p, fpold, fpart, c1, d1, f1, f2, f3, store;
    static integer i1, i2;
    static real p1, p2, p3;
    static integer i3, j1, j2, k3, l0, l1, l5, m1, nplus, nrint, n7, n8;
    static real h1[7], h2[6];
    static integer n10, n11, ij, ik, jk, kk, mm;
    extern /* Subroutine */ int fpbacp_();
    static integer it;
    static real wi, xi, yi, rn;
    extern /* Subroutine */ int fpdisc_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpbspl_(), fprota_(), fpgivs_(), fpknot_();
    static real fp0;
    static integer kk1, nk1, nk2;
    static real acc, one, cos__, per, sin__;
    static integer new__;
    static real piv;
    static integer ich1, ich3;
    static real con1, con4, con9;
    static integer npl1;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpbacp,fpbspl,fpgivs,fpdisc,fpknot,fprota */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --w;
    --y;
    --x;
    --nrdata;
    a2_dim1 = *nest;
    a2_offset = 1 + a2_dim1 * 1;
    a2 -= a2_offset;
    --z__;
    --fpint;
    --c__;
    --t;
    q_dim1 = *m;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    g2_dim1 = *nest;
    g2_offset = 1 + g2_dim1 * 1;
    g2 -= g2_offset;
    a1_dim1 = *nest;
    a1_offset = 1 + a1_dim1 * 1;
    a1 -= a1_offset;
    g1_dim1 = *nest;
    g1_offset = 1 + g1_dim1 * 1;
    g1 -= g1_offset;
    b_dim1 = *nest;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
    half = (float).5;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 1: determination of the number of knots and their position     c */
/*  **************************************************************      c */
/*  given a set of knots we compute the least-squares periodic spline   c */
/*  sinf(x). if the sum f(p=inf) <= s we accept the choice of knots.    c */
/*  the initial choice of knots depends on the value of s and iopt.     c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*    knots equals nmax = m+2*k.                                        c */
/*    if s > 0 and                                                      c */
/*      iopt=0 we first compute the least-squares polynomial of         c */
/*      degree k; n = nmin = 2*k+2. since s(x) must be periodic we      c */
/*      find that s(x) is a constant function.                          c */
/*      iopt=1 we start with the set of knots found at the last         c */
/*      call of the routine, except for the case that s > fp0; then     c */
/*      we compute directly the least-squares periodic polynomial.      c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
    m1 = *m - 1;
    kk = *k;
    kk1 = *k1;
    k3 = *k * 3 + 1;
    nmin = *k1 << 1;
/*  determine the length of the period of s(x). */
    per = x[*m] - x[1];
    if (*iopt < 0) {
	goto L50;
    }
/*  calculation of acc, the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  determine nmax, the number of knots for periodic spline interpolation */
    nmax = *m + (*k << 1);
    if (*s > (float)0. || nmax == nmin) {
	goto L30;
    }
/*  if s=0, s(x) is an interpolating spline. */
    *n = nmax;
/*  test whether the required storage space exceeds the available one. */
    if (*n > *nest) {
	goto L620;
    }
/*  find the position of the interior knots in case of interpolation. */
L5:
    if (*k / 2 << 1 == *k) {
	goto L20;
    }
    i__1 = m1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	j = i__ + *k;
	t[j] = x[i__];
/* L10: */
    }
    if (*s > (float)0.) {
	goto L50;
    }
    kk = *k - 1;
    kk1 = *k;
    if (kk > 0) {
	goto L50;
    }
    t[1] = t[*m] - per;
    t[2] = x[1];
    t[*m + 1] = x[*m];
    t[*m + 2] = t[3] + per;
    i__1 = m1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = y[i__];
/* L15: */
    }
    c__[*m] = c__[1];
    *fp = (float)0.;
    fpint[*n] = fp0;
    fpint[*n - 1] = (float)0.;
    nrdata[*n] = 0;
    goto L630;
L20:
    i__1 = m1;
    for (i__ = 2; i__ <= i__1; ++i__) {
	j = i__ + *k;
	t[j] = (x[i__] + x[i__ - 1]) * half;
/* L25: */
    }
    goto L50;
/*  if s > 0 our initial choice depends on the value of iopt. */
/*  if iopt=0 or iopt=1 and s>=fp0, we start computing the least-squares */
/*  periodic polynomial. (i.e. a constant function). */
/*  if iopt=1 and fp0>s we start computing the least-squares periodic */
/*  spline according the set of knots found at the last call of the */
/*  routine. */
L30:
    if (*iopt == 0) {
	goto L35;
    }
    if (*n == nmin) {
	goto L35;
    }
    fp0 = fpint[*n];
    fpold = fpint[*n - 1];
    nplus = nrdata[*n];
    if (fp0 > *s) {
	goto L50;
    }
/*  the case that s(x) is a constant function is treated separetely. */
/*  find the least-squares constant c1 and compute fp0 at the same time. */
L35:
    fp0 = (float)0.;
    d1 = (float)0.;
    c1 = (float)0.;
    i__1 = m1;
    for (it = 1; it <= i__1; ++it) {
	wi = w[it];
	yi = y[it] * wi;
	fpgivs_(&wi, &d1, &cos__, &sin__);
	fprota_(&cos__, &sin__, &yi, &c1);
/* Computing 2nd power */
	r__1 = yi;
	fp0 += r__1 * r__1;
/* L40: */
    }
    c1 /= d1;
/*  test whether that constant function is a solution of our problem. */
    fpms = fp0 - *s;
    if (fpms < acc || nmax == nmin) {
	goto L640;
    }
    fpold = fp0;
/*  test whether the required storage space exceeds the available one. */
    if (nmin >= *nest) {
	goto L620;
    }
/*  start computing the least-squares periodic spline with one */
/*  interior knot. */
    nplus = 1;
    *n = nmin + 1;
    mm = (*m + 1) / 2;
    t[*k2] = x[mm];
    nrdata[1] = mm - 2;
    nrdata[2] = m1 - mm;
/*  main loop for the different sets of knots. m is a save upper */
/*  bound for the number of trials. */
L50:
    i__1 = *m;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find nrint, the number of knot intervals. */
	nrint = *n - nmin + 1;
/*  find the position of the additional knots which are needed for */
/*  the b-spline representation of s(x). if we take */
/*      t(k+1) = x(1), t(n-k) = x(m) */
/*      t(k+1-j) = t(n-k-j) - per, j=1,2,...k */
/*      t(n-k+j) = t(k+1+j) + per, j=1,2,...k */
/*  then s(x) is a periodic spline with period per if the b-spline */
/*  coefficients satisfy the following conditions */
/*      c(n7+j) = c(j), j=1,...k   (**)   with n7=n-2*k-1. */
	t[*k1] = x[1];
	nk1 = *n - *k1;
	nk2 = nk1 + 1;
	t[nk2] = x[*m];
	i__2 = *k;
	for (j = 1; j <= i__2; ++j) {
	    i1 = nk2 + j;
	    i2 = nk2 - j;
	    j1 = *k1 + j;
	    j2 = *k1 - j;
	    t[i1] = t[j1] + per;
	    t[j2] = t[i2] - per;
/* L60: */
	}
/*  compute the b-spline coefficients c(j),j=1,...n7 of the least-squares */
/*  periodic spline sinf(x). the observation matrix a is built up row */
/*  by row while taking into account condition (**) and is reduced to */
/*  triangular form by givens transformations . */
/*  at the same time fp=f(p=inf) is computed. */
/*  the n7 x n7 triangularised upper matrix a has the form */
/*            ! a1 '    ! */
/*        a = !    ' a2 ! */
/*            ! 0  '    ! */
/*  with a2 a n7 x k matrix and a1 a n10 x n10 upper triangular */
/*  matrix of bandwith k+1 ( n10 = n7-k). */
/*  initialization. */
	i__2 = nk1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    z__[i__] = (float)0.;
	    i__3 = kk1;
	    for (j = 1; j <= i__3; ++j) {
		a1[i__ + j * a1_dim1] = (float)0.;
/* L70: */
	    }
	}
	n7 = nk1 - *k;
	n10 = n7 - kk;
	jper = 0;
	*fp = (float)0.;
	l = *k1;
	i__3 = m1;
	for (it = 1; it <= i__3; ++it) {
/*  fetch the current data point x(it),y(it) */
	    xi = x[it];
	    wi = w[it];
	    yi = y[it] * wi;
/*  search for knot interval t(l) <= xi < t(l+1). */
L80:
	    if (xi < t[l + 1]) {
		goto L85;
	    }
	    ++l;
	    goto L80;
/*  evaluate the (k+1) non-zero b-splines at xi and store them in q. */
L85:
	    fpbspl_(&t[1], n, k, &xi, &l, h__);
	    i__2 = *k1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		q[it + i__ * q_dim1] = h__[i__ - 1];
		h__[i__ - 1] *= wi;
/* L90: */
	    }
	    l5 = l - *k1;
/*  test whether the b-splines nj,k+1(x),j=1+n7,...nk1 are all zero at xi */
	    if (l5 < n10) {
		goto L285;
	    }
	    if (jper != 0) {
		goto L160;
	    }
/*  initialize the matrix a2. */
	    i__2 = n7;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		i__4 = kk;
		for (j = 1; j <= i__4; ++j) {
		    a2[i__ + j * a2_dim1] = (float)0.;
/* L95: */
		}
	    }
	    jk = n10 + 1;
	    i__4 = kk;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		ik = jk;
		i__2 = kk1;
		for (j = 1; j <= i__2; ++j) {
		    if (ik <= 0) {
			goto L105;
		    }
		    a2[ik + i__ * a2_dim1] = a1[ik + j * a1_dim1];
		    --ik;
/* L100: */
		}
L105:
		++jk;
/* L110: */
	    }
	    jper = 1;
/*  if one of the b-splines nj,k+1(x),j=n7+1,...nk1 is not zero at xi */
/*  we take account of condition (**) for setting up the new row */
/*  of the observation matrix a. this row is stored in the arrays h1 */
/*  (the part with respect to a1) and h2 (the part with */
/*  respect to a2). */
L160:
	    i__4 = kk;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		h1[i__ - 1] = (float)0.;
		h2[i__ - 1] = (float)0.;
/* L170: */
	    }
	    h1[kk1 - 1] = (float)0.;
	    j = l5 - n10;
	    i__4 = kk1;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		++j;
		l0 = j;
L180:
		l1 = l0 - kk;
		if (l1 <= 0) {
		    goto L200;
		}
		if (l1 <= n10) {
		    goto L190;
		}
		l0 = l1 - n10;
		goto L180;
L190:
		h1[l1 - 1] = h__[i__ - 1];
		goto L210;
L200:
		h2[l0 - 1] += h__[i__ - 1];
L210:
		;
	    }
/*  rotate the new row of the observation matrix into triangle */
/*  by givens transformations. */
	    if (n10 <= 0) {
		goto L250;
	    }
/*  rotation with the rows 1,2,...n10 of matrix a. */
	    i__4 = n10;
	    for (j = 1; j <= i__4; ++j) {
		piv = h1[0];
		if (piv != (float)0.) {
		    goto L214;
		}
		i__2 = kk;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    h1[i__ - 1] = h1[i__];
/* L212: */
		}
		h1[kk1 - 1] = (float)0.;
		goto L240;
/*  calculate the parameters of the givens transformation. */
L214:
		fpgivs_(&piv, &a1[j + a1_dim1], &cos__, &sin__);
/*  transformation to the right hand side. */
		fprota_(&cos__, &sin__, &yi, &z__[j]);
/*  transformations to the left hand side with respect to a2. */
		i__2 = kk;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    fprota_(&cos__, &sin__, &h2[i__ - 1], &a2[j + i__ * 
			    a2_dim1]);
/* L220: */
		}
		if (j == n10) {
		    goto L250;
		}
/* Computing MIN */
		i__2 = n10 - j;
		i2 = min(i__2,kk);
/*  transformations to the left hand side with respect to a1. */
		i__2 = i2;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i1 = i__ + 1;
		    fprota_(&cos__, &sin__, &h1[i1 - 1], &a1[j + i1 * a1_dim1]
			    );
		    h1[i__ - 1] = h1[i1 - 1];
/* L230: */
		}
		h1[i1 - 1] = (float)0.;
L240:
		;
	    }
/*  rotation with the rows n10+1,...n7 of matrix a. */
L250:
	    i__4 = kk;
	    for (j = 1; j <= i__4; ++j) {
		ij = n10 + j;
		if (ij <= 0) {
		    goto L270;
		}
		piv = h2[j - 1];
		if (piv == (float)0.) {
		    goto L270;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a2[ij + j * a2_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		fprota_(&cos__, &sin__, &yi, &z__[ij]);
		if (j == kk) {
		    goto L280;
		}
		j1 = j + 1;
/*  transformations to left hand side. */
		i__2 = kk;
		for (i__ = j1; i__ <= i__2; ++i__) {
		    fprota_(&cos__, &sin__, &h2[i__ - 1], &a2[ij + i__ * 
			    a2_dim1]);
/* L260: */
		}
L270:
		;
	    }
/*  add contribution of this row to the sum of squares of residual */
/*  right hand sides. */
L280:
/* Computing 2nd power */
	    r__1 = yi;
	    *fp += r__1 * r__1;
	    goto L290;
/*  rotation of the new row of the observation matrix into */
/*  triangle in case the b-splines nj,k+1(x),j=n7+1,...n-k-1 are all zero */
/*  at xi. */
L285:
	    j = l5;
	    i__4 = kk1;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		++j;
		piv = h__[i__ - 1];
		if (piv == (float)0.) {
		    goto L140;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a1[j + a1_dim1], &cos__, &sin__);
/*  transformations to right hand side. */
		fprota_(&cos__, &sin__, &yi, &z__[j]);
		if (i__ == kk1) {
		    goto L150;
		}
		i2 = 1;
		i3 = i__ + 1;
/*  transformations to left hand side. */
		i__2 = kk1;
		for (i1 = i3; i1 <= i__2; ++i1) {
		    ++i2;
		    fprota_(&cos__, &sin__, &h__[i1 - 1], &a1[j + i2 * 
			    a1_dim1]);
/* L130: */
		}
L140:
		;
	    }
/*  add contribution of this row to the sum of squares of residual */
/*  right hand sides. */
L150:
/* Computing 2nd power */
	    r__1 = yi;
	    *fp += r__1 * r__1;
L290:
	    ;
	}
	fpint[*n] = fp0;
	fpint[*n - 1] = fpold;
	nrdata[*n] = nplus;
/*  backward substitution to obtain the b-spline coefficients c(j),j=1,.n */
	fpbacp_(&a1[a1_offset], &a2[a2_offset], &z__[1], &n7, &kk, &c__[1], &
		kk1, nest);
/*  calculate from condition (**) the coefficients c(j+n7),j=1,2,...k. */
	i__3 = *k;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    j = i__ + n7;
	    c__[j] = c__[i__];
/* L295: */
	}
	if (*iopt < 0) {
	    goto L660;
	}
/*  test whether the approximation sinf(x) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L660;
	}
/*  if f(p=inf) < s accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L350;
	}
/*  if n=nmax, sinf(x) is an interpolating spline. */
	if (*n == nmax) {
	    goto L630;
	}
/*  increase the number of knots. */
/*  if n=nest we cannot increase the number of knots because of the */
/*  storage capacity limitation. */
	if (*n == *nest) {
	    goto L620;
	}
/*  determine the number of knots nplus we are going to add. */
	npl1 = nplus << 1;
	rn = (real) nplus;
	if (fpold - *fp > acc) {
	    npl1 = rn * fpms / (fpold - *fp);
	}
/* Computing MIN */
/* Computing MAX */
	i__2 = npl1, i__5 = nplus / 2, i__2 = max(i__2,i__5);
	i__3 = nplus << 1, i__4 = max(i__2,1);
	nplus = min(i__3,i__4);
	fpold = *fp;
/*  compute the sum(wi*(yi-s(xi))**2) for each knot interval */
/*  t(j+k) <= xi <= t(j+k+1) and store it in fpint(j),j=1,2,...nrint. */
	fpart = (float)0.;
	i__ = 1;
	l = *k1;
	i__3 = m1;
	for (it = 1; it <= i__3; ++it) {
	    if (x[it] < t[l]) {
		goto L300;
	    }
	    new__ = 1;
	    ++l;
L300:
	    term = (float)0.;
	    l0 = l - *k2;
	    i__4 = *k1;
	    for (j = 1; j <= i__4; ++j) {
		++l0;
		term += c__[l0] * q[it + j * q_dim1];
/* L310: */
	    }
/* Computing 2nd power */
	    r__1 = w[it] * (term - y[it]);
	    term = r__1 * r__1;
	    fpart += term;
	    if (new__ == 0) {
		goto L320;
	    }
	    if (l > *k2) {
		goto L315;
	    }
	    fpint[nrint] = term;
	    new__ = 0;
	    goto L320;
L315:
	    store = term * half;
	    fpint[i__] = fpart - store;
	    ++i__;
	    fpart = store;
	    new__ = 0;
L320:
	    ;
	}
	fpint[nrint] += fpart;
	i__3 = nplus;
	for (l = 1; l <= i__3; ++l) {
/*  add a new knot */
	    fpknot_(&x[1], m, &t[1], n, &fpint[1], &nrdata[1], &nrint, nest, &
		    c__1);
/*  if n=nmax we locate the knots as for interpolation. */
	    if (*n == nmax) {
		goto L5;
	    }
/*  test whether we cannot further increase the number of knots. */
	    if (*n == *nest) {
		goto L340;
	    }
/* L330: */
	}
/*  restart the computations with the new set of knots. */
L340:
	;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 2: determination of the smoothing periodic spline sp(x).       c */
/*  *************************************************************       c */
/*  we have determined the number of knots and their position.          c */
/*  we now compute the b-spline coefficients of the smoothing spline    c */
/*  sp(x). the observation matrix a is extended by the rows of matrix   c */
/*  b expressing that the kth derivative discontinuities of sp(x) at    c */
/*  the interior knots t(k+2),...t(n-k-1) must be zero. the corres-     c */
/*  ponding weights of these additional rows are set to 1/sqrt(p).      c */
/*  iteratively we then have to determine the value of p such that      c */
/*  f(p)=sum(w(i)*(y(i)-sp(x(i)))**2) be = s. we already know that      c */
/*  the least-squares constant function corresponds to p=0, and that    c */
/*  the least-squares periodic spline corresponds to p=infinity. the    c */
/*  iteration process which is proposed here, makes use of rational     c */
/*  interpolation. since f(p) is a convex and strictly decreasing       c */
/*  function of p, it can be approximated by a rational function        c */
/*  r(p) = (u*p+v)/(p+w). three values of p(p1,p2,p3) with correspond-  c */
/*  ing values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s) are used      c */
/*  to calculate the new value of p such that r(p)=s. convergence is    c */
/*  guaranteed by taking f1>0 and f3<0.                                 c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  evaluate the discontinuity jump of the kth derivative of the */
/*  b-splines at the knots t(l),l=k+2,...n-k-1 and store in b. */
L350:
    fpdisc_(&t[1], n, k2, &b[b_offset], nest);
/*  initial value for p. */
    p1 = (float)0.;
    f1 = fp0 - *s;
    p3 = -one;
    f3 = fpms;
    n11 = n10 - 1;
    n8 = n7 - 1;
    p = (float)0.;
    l = n7;
    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = *k + 1 - i__;
	p += a2[l + j * a2_dim1];
	--l;
	if (l == 0) {
	    goto L356;
	}
/* L352: */
    }
    i__1 = n10;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p += a1[i__ + a1_dim1];
/* L354: */
    }
L356:
    rn = (real) n7;
    p = rn / p;
    ich1 = 0;
    ich3 = 0;
/*  iteration process to find the root of f(p) = s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  form the matrix g  as the matrix a extended by the rows of matrix b. */
/*  the rows of matrix b with weight 1/p are rotated into */
/*  the triangularised observation matrix a. */
/*  after triangularisation our n7 x n7 matrix g takes the form */
/*            ! g1 '    ! */
/*        g = !    ' g2 ! */
/*            ! 0  '    ! */
/*  with g2 a n7 x (k+1) matrix and g1 a n11 x n11 upper triangular */
/*  matrix of bandwidth k+2. ( n11 = n7-k-1) */
	pinv = one / p;
/*  store matrix a into g */
	i__3 = n7;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    c__[i__] = z__[i__];
	    g1[i__ + *k1 * g1_dim1] = a1[i__ + *k1 * a1_dim1];
	    g1[i__ + *k2 * g1_dim1] = (float)0.;
	    g2[i__ + g2_dim1] = (float)0.;
	    i__4 = *k;
	    for (j = 1; j <= i__4; ++j) {
		g1[i__ + j * g1_dim1] = a1[i__ + j * a1_dim1];
		g2[i__ + (j + 1) * g2_dim1] = a2[i__ + j * a2_dim1];
/* L360: */
	    }
	}
	l = n10;
	i__4 = *k1;
	for (j = 1; j <= i__4; ++j) {
	    if (l <= 0) {
		goto L375;
	    }
	    g2[l + g2_dim1] = a1[l + j * a1_dim1];
	    --l;
/* L370: */
	}
L375:
	i__4 = n8;
	for (it = 1; it <= i__4; ++it) {
/*  fetch a new row of matrix b and store it in the arrays h1 (the part */
/*  with respect to g1) and h2 (the part with respect to g2). */
	    yi = (float)0.;
	    i__3 = *k1;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h1[i__ - 1] = (float)0.;
		h2[i__ - 1] = (float)0.;
/* L380: */
	    }
	    h1[*k2 - 1] = (float)0.;
	    if (it > n11) {
		goto L420;
	    }
	    l = it;
	    l0 = it;
	    i__3 = *k2;
	    for (j = 1; j <= i__3; ++j) {
		if (l0 == n10) {
		    goto L400;
		}
		h1[j - 1] = b[it + j * b_dim1] * pinv;
		++l0;
/* L390: */
	    }
	    goto L470;
L400:
	    l0 = 1;
	    i__3 = *k2;
	    for (l1 = j; l1 <= i__3; ++l1) {
		h2[l0 - 1] = b[it + l1 * b_dim1] * pinv;
		++l0;
/* L410: */
	    }
	    goto L470;
L420:
	    l = 1;
	    i__ = it - n10;
	    i__3 = *k2;
	    for (j = 1; j <= i__3; ++j) {
		++i__;
		l0 = i__;
L430:
		l1 = l0 - *k1;
		if (l1 <= 0) {
		    goto L450;
		}
		if (l1 <= n11) {
		    goto L440;
		}
		l0 = l1 - n11;
		goto L430;
L440:
		h1[l1 - 1] = b[it + j * b_dim1] * pinv;
		goto L460;
L450:
		h2[l0 - 1] += b[it + j * b_dim1] * pinv;
L460:
		;
	    }
	    if (n11 <= 0) {
		goto L510;
	    }
/*  rotate this row into triangle by givens transformations without */
/*  square roots. */
/*  rotation with the rows l,l+1,...n11. */
L470:
	    i__3 = n11;
	    for (j = l; j <= i__3; ++j) {
		piv = h1[0];
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &g1[j + g1_dim1], &cos__, &sin__);
/*  transformation to right hand side. */
		fprota_(&cos__, &sin__, &yi, &c__[j]);
/*  transformation to the left hand side with respect to g2. */
		i__2 = *k1;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    fprota_(&cos__, &sin__, &h2[i__ - 1], &g2[j + i__ * 
			    g2_dim1]);
/* L480: */
		}
		if (j == n11) {
		    goto L510;
		}
/* Computing MIN */
		i__2 = n11 - j;
		i2 = min(i__2,*k1);
/*  transformation to the left hand side with respect to g1. */
		i__2 = i2;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i1 = i__ + 1;
		    fprota_(&cos__, &sin__, &h1[i1 - 1], &g1[j + i1 * g1_dim1]
			    );
		    h1[i__ - 1] = h1[i1 - 1];
/* L490: */
		}
		h1[i1 - 1] = (float)0.;
/* L500: */
	    }
/*  rotation with the rows n11+1,...n7 */
L510:
	    i__3 = *k1;
	    for (j = 1; j <= i__3; ++j) {
		ij = n11 + j;
		if (ij <= 0) {
		    goto L530;
		}
		piv = h2[j - 1];
/*  calculate the parameters of the givens transformation */
		fpgivs_(&piv, &g2[ij + j * g2_dim1], &cos__, &sin__);
/*  transformation to the right hand side. */
		fprota_(&cos__, &sin__, &yi, &c__[ij]);
		if (j == *k1) {
		    goto L540;
		}
		j1 = j + 1;
/*  transformation to the left hand side. */
		i__2 = *k1;
		for (i__ = j1; i__ <= i__2; ++i__) {
		    fprota_(&cos__, &sin__, &h2[i__ - 1], &g2[ij + i__ * 
			    g2_dim1]);
/* L520: */
		}
L530:
		;
	    }
L540:
	    ;
	}
/*  backward substitution to obtain the b-spline coefficients */
/*  c(j),j=1,2,...n7 of sp(x). */
	fpbacp_(&g1[g1_offset], &g2[g2_offset], &c__[1], &n7, k1, &c__[1], k2,
		 nest);
/*  calculate from condition (**) the b-spline coefficients c(n7+j),j=1,. */
	i__4 = *k;
	for (i__ = 1; i__ <= i__4; ++i__) {
	    j = i__ + n7;
	    c__[j] = c__[i__];
/* L545: */
	}
/*  computation of f(p). */
	*fp = (float)0.;
	l = *k1;
	i__4 = m1;
	for (it = 1; it <= i__4; ++it) {
	    if (x[it] < t[l]) {
		goto L550;
	    }
	    ++l;
L550:
	    l0 = l - *k2;
	    term = (float)0.;
	    i__3 = *k1;
	    for (j = 1; j <= i__3; ++j) {
		++l0;
		term += c__[l0] * q[it + j * q_dim1];
/* L560: */
	    }
/* Computing 2nd power */
	    r__1 = w[it] * (term - y[it]);
	    *fp += r__1 * r__1;
/* L570: */
	}
/*  test whether the approximation sp(x) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L660;
	}
/*  test whether the maximal number of iterations is reached. */
	if (iter == *maxit) {
	    goto L600;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L580;
	}
	if (f2 - f3 > acc) {
	    goto L575;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L595;
L575:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L580:
	if (ich1 != 0) {
	    goto L590;
	}
	if (f1 - f2 > acc) {
	    goto L585;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L595;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L595;
L585:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L590:
	if (f2 >= f1 || f2 <= f3) {
	    goto L610;
	}
/*  find the new value for p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L595:
	;
    }
/*  error codes and messages. */
L600:
    *ier = 3;
    goto L660;
L610:
    *ier = 2;
    goto L660;
L620:
    *ier = 1;
    goto L660;
L630:
    *ier = -1;
    goto L660;
L640:
    *ier = -2;
/*  the least-squares constant function c1 is a solution of our problem. */
/*  a constant function is a spline of degree k with all b-spline */
/*  coefficients equal to that constant c1. */
    i__1 = *k1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rn = (real) (*k1 - i__);
	t[i__] = x[1] - rn * per;
	c__[i__] = c1;
	j = i__ + *k1;
	rn = (real) (i__ - 1);
	t[j] = x[*m] + rn * per;
/* L650: */
    }
    *n = nmin;
    *fp = fp0;
    fpint[*n] = fp0;
    fpint[*n - 1] = (float)0.;
    nrdata[*n] = 0;
L660:
    return 0;
} /* fpperi_ */

/* Subroutine */ int fppocu_(idim, k, a, b, ib, db, nb, ie, de, ne, cp, np)
integer *idim, *k;
real *a, *b;
integer *ib;
real *db;
integer *nb, *ie;
real *de;
integer *ne;
real *cp;
integer *np;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static real work[36]	/* was [6][6] */;
    static integer i__, j, l, k1, k2;
    static real ab;
    static integer id, jj, ll;
    static real aki;

/*  subroutine fppocu finds a idim-dimensional polynomial curve p(u) = */
/*  (p1(u),p2(u),...,pidim(u)) of degree k, satisfying certain derivative */
/*  constraints at the end points a and b, i.e. */
/*                  (l) */
/*    if ib > 0 : pj   (a) = db(idim*l+j), l=0,1,...,ib-1 */
/*                  (l) */
/*    if ie > 0 : pj   (b) = de(idim*l+j), l=0,1,...,ie-1 */

/*  the polynomial curve is returned in its b-spline representation */
/*  ( coefficients cp(j), j=1,2,...,np ) */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  .. */
    /* Parameter adjustments */
    --db;
    --de;
    --cp;

    /* Function Body */
    k1 = *k + 1;
    k2 = k1 << 1;
    ab = *b - *a;
    i__1 = *idim;
    for (id = 1; id <= i__1; ++id) {
	i__2 = k1;
	for (j = 1; j <= i__2; ++j) {
	    work[j - 1] = (float)0.;
/* L10: */
	}
	if (*ib == 0) {
	    goto L50;
	}
	l = id;
	i__2 = *ib;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    work[i__ * 6 - 6] = db[l];
	    l += *idim;
/* L20: */
	}
	if (*ib == 1) {
	    goto L50;
	}
	ll = *ib;
	i__2 = *ib;
	for (j = 2; j <= i__2; ++j) {
	    --ll;
	    i__3 = ll;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		aki = (real) (k1 - i__);
		work[j + i__ * 6 - 7] = ab * work[j - 1 + (i__ + 1) * 6 - 7] /
			 aki + work[j - 1 + i__ * 6 - 7];
/* L30: */
	    }
/* L40: */
	}
L50:
	if (*ie == 0) {
	    goto L90;
	}
	l = id;
	j = k1;
	i__2 = *ie;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    work[j + i__ * 6 - 7] = de[l];
	    l += *idim;
	    --j;
/* L60: */
	}
	if (*ie == 1) {
	    goto L90;
	}
	ll = *ie;
	i__2 = *ie;
	for (jj = 2; jj <= i__2; ++jj) {
	    --ll;
	    j = k1 + 1 - jj;
	    i__3 = ll;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		aki = (real) (k1 - i__);
		work[j + i__ * 6 - 7] = work[j + 1 + i__ * 6 - 7] - ab * work[
			j + (i__ + 1) * 6 - 7] / aki;
		--j;
/* L70: */
	    }
/* L80: */
	}
L90:
	l = (id - 1) * k2;
	i__2 = k1;
	for (j = 1; j <= i__2; ++j) {
	    ++l;
	    cp[l] = work[j - 1];
/* L100: */
	}
/* L110: */
    }
    return 0;
} /* fppocu_ */

/* Subroutine */ int fppogr_(iopt, ider, u, mu, v, mv, z__, mz, z0, r__, s, 
	nuest, nvest, tol, maxit, nc, nu, tu, nv, tv, c__, fp, fp0, fpold, 
	reducu, reducv, fpintu, fpintv, dz, step, lastdi, nplusu, nplusv, 
	lasttu, nru, nrv, nrdatu, nrdatv, wrk, lwrk, ier)
integer *iopt, *ider;
real *u;
integer *mu;
real *v;
integer *mv;
real *z__;
integer *mz;
real *z0, *r__, *s;
integer *nuest, *nvest;
real *tol;
integer *maxit, *nc, *nu;
real *tu;
integer *nv;
real *tv, *c__, *fp, *fp0, *fpold, *reducu, *reducv, *fpintu, *fpintv, *dz, *
	step;
integer *lastdi, *nplusu, *nplusv, *lasttu, *nru, *nrv, *nrdatu, *nrdatv;
real *wrk;
integer *lwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    real r__1;

    /* Builtin functions */
    double atan2();

    /* Local variables */
    static integer ifbu, ifbv, iter;
    static real fpms;
    static integer ifsu, ifsv;
    static real zmin;
    static integer nplu;
    static real zmax;
    static integer nplv, i__, j, l;
    static real p;
    static integer mumin;
    static real f1, f2, f3;
    static integer i1, i2, numax, l1, l2;
    static real p1, p2, p3;
    static integer l3, l4, nvmax;
    static real vb, pi, ve;
    static integer nn, ju;
    static real rn;
    extern /* Subroutine */ int fpopdi_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpknot_();
    static integer istart, nrintu, nrintv, mu0, mu1;
    static real acc;
    static integer idd[2];
    static real one, per;
    static integer nue, nve, mpm, ktu;
    static real dzz[3];
    static integer ich1, ich3;
    static real con1, con4, con9;
    static integer npl1;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpknot,fpopdi */
/*  .. */
/*   set constants */
    /* Parameter adjustments */
    --iopt;
    --ider;
    --nru;
    --u;
    --nrv;
    --v;
    --z__;
    --nrdatu;
    --fpintu;
    --tu;
    --nrdatv;
    --fpintv;
    --tv;
    --c__;
    --dz;
    --wrk;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
/*   initialization */
    ifsu = 0;
    ifsv = 0;
    ifbu = 0;
    ifbv = 0;
    p = -one;
    mumin = 4 - iopt[3];
    if (ider[1] >= 0) {
	--mumin;
    }
    if (iopt[2] == 1 && ider[2] == 1) {
	--mumin;
    }
    pi = atan2((float)0., -one);
    per = pi + pi;
    vb = v[1];
    ve = vb + per;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 1: determination of the number of knots and their position.     c */
/* ****************************************************************     c */
/*  given a set of knots we compute the least-squares spline sinf(u,v)  c */
/*  and the corresponding sum of squared residuals fp = f(p=inf).       c */
/*  if iopt(1)=-1  sinf(u,v) is the requested approximation.            c */
/*  if iopt(1)>=0  we check whether we can accept the knots:            c */
/*    if fp <= s we will continue with the current set of knots.        c */
/*    if fp >  s we will increase the number of knots and compute the   c */
/*       corresponding least-squares spline until finally fp <= s.      c */
/*    the initial choice of knots depends on the value of s and iopt.   c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*     knots in the u-direction equals nu=numax=mu+5+iopt(2)+iopt(3)    c */
/*     and in the v-direction nv=nvmax=mv+7.                            c */
/*    if s>0 and                                                        c */
/*      iopt(1)=0 we first compute the least-squares polynomial,i.e. a  c */
/*       spline without interior knots : nu=8 ; nv=8.                   c */
/*      iopt(1)=1 we start with the set of knots found at the last call c */
/*       of the routine, except for the case that s > fp0; then we      c */
/*       compute the least-squares polynomial directly.                 c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
    if (iopt[1] < 0) {
	goto L120;
    }
/*  acc denotes the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  numax and nvmax denote the number of knots needed for interpolation. */
    numax = *mu + 5 + iopt[2] + iopt[3];
    nvmax = *mv + 7;
    nue = min(numax,*nuest);
    nve = min(nvmax,*nvest);
    if (*s > (float)0.) {
	goto L100;
    }
/*  if s = 0, s(u,v) is an interpolating spline. */
    *nu = numax;
    *nv = nvmax;
/*  test whether the required storage space exceeds the available one. */
    if (*nu > *nuest || *nv > *nvest) {
	goto L420;
    }
/*  find the position of the knots in the v-direction. */
    i__1 = *mv;
    for (l = 1; l <= i__1; ++l) {
	tv[l + 3] = v[l];
/* L10: */
    }
    tv[*mv + 4] = ve;
    l1 = *mv - 2;
    l2 = *mv + 5;
    for (i__ = 1; i__ <= 3; ++i__) {
	tv[i__] = v[l1] - per;
	tv[l2] = v[i__ + 1] + per;
	++l1;
	++l2;
/* L20: */
    }
/*  if not all the derivative values g(i,j) are given, we will first */
/*  estimate these values by computing a least-squares spline */
    idd[0] = ider[1];
    if (idd[0] == 0) {
	idd[0] = 1;
    }
    if (idd[0] > 0) {
	dz[1] = *z0;
    }
    idd[1] = ider[2];
    if (ider[1] < 0) {
	goto L30;
    }
    if (iopt[2] == 0 || ider[2] != 0) {
	goto L70;
    }
/* we set up the knots in the u-direction for computing the least-squares */
/* spline. */
L30:
    i1 = 3;
    i2 = *mu - 2;
    *nu = 4;
    i__1 = *mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (i1 > i2) {
	    goto L50;
	}
	++(*nu);
	tu[*nu] = u[i1];
	i1 += 2;
/* L40: */
    }
L50:
    for (i__ = 1; i__ <= 4; ++i__) {
	tu[i__] = (float)0.;
	++(*nu);
	tu[*nu] = *r__;
/* L60: */
    }
/* we compute the least-squares spline for estimating the derivatives. */
    fpopdi_(&ifsu, &ifsv, &ifbu, &ifbv, &u[1], mu, &v[1], mv, &z__[1], mz, z0,
	     &dz[1], &iopt[1], idd, &tu[1], nu, &tv[1], nv, nuest, nvest, &p, 
	    step, &c__[1], nc, fp, &fpintu[1], &fpintv[1], &nru[1], &nrv[1], &
	    wrk[1], lwrk);
    ifsu = 0;
/* if all the derivatives at the origin are known, we compute the */
/* interpolating spline. */
/* we set up the knots in the u-direction, needed for interpolation. */
L70:
    nn = numax - 8;
    if (nn == 0) {
	goto L95;
    }
    ju = 2 - iopt[2];
    i__1 = nn;
    for (l = 1; l <= i__1; ++l) {
	tu[l + 4] = u[ju];
	++ju;
/* L80: */
    }
    *nu = numax;
    l = *nu;
    for (i__ = 1; i__ <= 4; ++i__) {
	tu[i__] = (float)0.;
	tu[l] = *r__;
	--l;
/* L90: */
    }
/* we compute the interpolating spline. */
L95:
    fpopdi_(&ifsu, &ifsv, &ifbu, &ifbv, &u[1], mu, &v[1], mv, &z__[1], mz, z0,
	     &dz[1], &iopt[1], idd, &tu[1], nu, &tv[1], nv, nuest, nvest, &p, 
	    step, &c__[1], nc, fp, &fpintu[1], &fpintv[1], &nru[1], &nrv[1], &
	    wrk[1], lwrk);
    goto L430;
/*  if s>0 our initial choice of knots depends on the value of iopt(1). */
L100:
    *ier = 0;
    if (iopt[1] == 0) {
	goto L115;
    }
    *step = -(*step);
    if (*fp0 <= *s) {
	goto L115;
    }
/*  if iopt(1)=1 and fp0 > s we start computing the least-squares spline */
/*  according to the set of knots found at the last call of the routine. */
/*  we determine the number of grid coordinates u(i) inside each knot */
/*  interval (tu(l),tu(l+1)). */
    l = 5;
    j = 1;
    nrdatu[1] = 0;
    mu0 = 2 - iopt[2];
    mu1 = *mu - 2 + iopt[3];
    i__1 = mu1;
    for (i__ = mu0; i__ <= i__1; ++i__) {
	++nrdatu[j];
	if (u[i__] < tu[l]) {
	    goto L105;
	}
	--nrdatu[j];
	++l;
	++j;
	nrdatu[j] = 0;
L105:
	;
    }
/*  we determine the number of grid coordinates v(i) inside each knot */
/*  interval (tv(l),tv(l+1)). */
    l = 5;
    j = 1;
    nrdatv[1] = 0;
    i__1 = *mv;
    for (i__ = 2; i__ <= i__1; ++i__) {
	++nrdatv[j];
	if (v[i__] < tv[l]) {
	    goto L110;
	}
	--nrdatv[j];
	++l;
	++j;
	nrdatv[j] = 0;
L110:
	;
    }
    idd[0] = ider[1];
    idd[1] = ider[2];
    goto L120;
/*  if iopt(1)=0 or iopt(1)=1 and s >= fp0,we start computing the least- */
/*  squares polynomial (which is a spline without interior knots). */
L115:
    *ier = -2;
    idd[0] = ider[1];
    idd[1] = 1;
    *nu = 8;
    *nv = 8;
    nrdatu[1] = *mu - 3 + iopt[2] + iopt[3];
    nrdatv[1] = *mv - 1;
    *lastdi = 0;
    *nplusu = 0;
    *nplusv = 0;
    *fp0 = (float)0.;
    *fpold = (float)0.;
    *reducu = (float)0.;
    *reducv = (float)0.;
/*  main loop for the different sets of knots.mpm=mu+mv is a save upper */
/*  bound for the number of trials. */
L120:
    mpm = *mu + *mv;
    i__1 = mpm;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find nrintu (nrintv) which is the number of knot intervals in the */
/*  u-direction (v-direction). */
	nrintu = *nu - 7;
	nrintv = *nv - 7;
/*  find the position of the additional knots which are needed for the */
/*  b-spline representation of s(u,v). */
	i__ = *nu;
	for (j = 1; j <= 4; ++j) {
	    tu[j] = (float)0.;
	    tu[i__] = *r__;
	    --i__;
/* L130: */
	}
	l1 = 4;
	l2 = l1;
	l3 = *nv - 3;
	l4 = l3;
	tv[l2] = vb;
	tv[l3] = ve;
	for (j = 1; j <= 3; ++j) {
	    ++l1;
	    --l2;
	    ++l3;
	    --l4;
	    tv[l2] = tv[l4] - per;
	    tv[l3] = tv[l1] + per;
/* L140: */
	}
/*  find an estimate of the range of possible values for the optimal */
/*  derivatives at the origin. */
	ktu = nrdatu[1] + 2 - iopt[2];
	if (nrintu == 1) {
	    ktu = *mu;
	}
	if (ktu < mumin) {
	    ktu = mumin;
	}
	if (ktu == *lasttu) {
	    goto L150;
	}
	zmin = *z0;
	zmax = *z0;
	l = *mv * ktu;
	i__2 = l;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (z__[i__] < zmin) {
		zmin = z__[i__];
	    }
	    if (z__[i__] > zmax) {
		zmax = z__[i__];
	    }
/* L145: */
	}
	*step = zmax - zmin;
	*lasttu = ktu;
/*  find the least-squares spline sinf(u,v). */
L150:
	fpopdi_(&ifsu, &ifsv, &ifbu, &ifbv, &u[1], mu, &v[1], mv, &z__[1], mz,
		 z0, &dz[1], &iopt[1], idd, &tu[1], nu, &tv[1], nv, nuest, 
		nvest, &p, step, &c__[1], nc, fp, &fpintu[1], &fpintv[1], &
		nru[1], &nrv[1], &wrk[1], lwrk);
	if (*step < (float)0.) {
	    *step = -(*step);
	}
	if (*ier == -2) {
	    *fp0 = *fp;
	}
/*  test whether the least-squares spline is an acceptable solution. */
	if (iopt[1] < 0) {
	    goto L440;
	}
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  if f(p=inf) < s, we accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L300;
	}
/*  if nu=numax and nv=nvmax, sinf(u,v) is an interpolating spline */
	if (*nu == numax && *nv == nvmax) {
	    goto L430;
	}
/*  increase the number of knots. */
/*  if nu=nue and nv=nve we cannot further increase the number of knots */
/*  because of the storage capacity limitation. */
	if (*nu == nue && *nv == nve) {
	    goto L420;
	}
	if (ider[1] == 0) {
/* Computing 2nd power */
	    r__1 = *z0 - c__[1];
	    fpintu[1] += r__1 * r__1;
	}
	*ier = 0;
/*  adjust the parameter reducu or reducv according to the direction */
/*  in which the last added knots were located. */
	if (*lastdi < 0) {
	    goto L160;
	} else if (*lastdi == 0) {
	    goto L155;
	} else {
	    goto L170;
	}
L155:
	nplv = 3;
	idd[1] = ider[2];
	*fpold = *fp;
	goto L230;
L160:
	*reducu = *fpold - *fp;
	goto L175;
L170:
	*reducv = *fpold - *fp;
/*  store the sum of squared residuals for the current set of knots. */
L175:
	*fpold = *fp;
/*  find nplu, the number of knots we should add in the u-direction. */
	nplu = 1;
	if (*nu == 8) {
	    goto L180;
	}
	npl1 = *nplusu << 1;
	rn = (real) (*nplusu);
	if (*reducu > acc) {
	    npl1 = rn * fpms / *reducu;
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = *nplusu / 2, i__4 = max(i__4,i__5);
	i__2 = *nplusu << 1, i__3 = max(i__4,1);
	nplu = min(i__2,i__3);
/*  find nplv, the number of knots we should add in the v-direction. */
L180:
	nplv = 3;
	if (*nv == 8) {
	    goto L190;
	}
	npl1 = *nplusv << 1;
	rn = (real) (*nplusv);
	if (*reducv > acc) {
	    npl1 = rn * fpms / *reducv;
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = *nplusv / 2, i__4 = max(i__4,i__5);
	i__2 = *nplusv << 1, i__3 = max(i__4,1);
	nplv = min(i__2,i__3);
/*  test whether we are going to add knots in the u- or v-direction. */
L190:
	if ((i__2 = nplu - nplv) < 0) {
	    goto L210;
	} else if (i__2 == 0) {
	    goto L200;
	} else {
	    goto L230;
	}
L200:
	if (*lastdi < 0) {
	    goto L230;
	}
L210:
	if (*nu == nue) {
	    goto L230;
	}
/*  addition in the u-direction. */
	*lastdi = -1;
	*nplusu = nplu;
	ifsu = 0;
	istart = 0;
	if (iopt[2] == 0) {
	    istart = 1;
	}
	i__2 = *nplusu;
	for (l = 1; l <= i__2; ++l) {
/*  add a new knot in the u-direction */
	    fpknot_(&u[1], mu, &tu[1], nu, &fpintu[1], &nrdatu[1], &nrintu, 
		    nuest, &istart);
/*  test whether we cannot further increase the number of knots in the */
/*  u-direction. */
	    if (*nu == nue) {
		goto L270;
	    }
/* L220: */
	}
	goto L270;
L230:
	if (*nv == nve) {
	    goto L210;
	}
/*  addition in the v-direction. */
	*lastdi = 1;
	*nplusv = nplv;
	ifsv = 0;
	i__2 = *nplusv;
	for (l = 1; l <= i__2; ++l) {
/*  add a new knot in the v-direction. */
	    fpknot_(&v[1], mv, &tv[1], nv, &fpintv[1], &nrdatv[1], &nrintv, 
		    nvest, &c__1);
/*  test whether we cannot further increase the number of knots in the */
/*  v-direction. */
	    if (*nv == nve) {
		goto L270;
	    }
/* L240: */
	}
/*  restart the computations with the new set of knots. */
L270:
	;
    }
/*  test whether the least-squares polynomial is a solution of our */
/*  approximation problem. */
L300:
    if (*ier == -2) {
	goto L440;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 2: determination of the smoothing spline sp(u,v)                c */
/* *****************************************************                c */
/*  we have determined the number of knots and their position. we now   c */
/*  compute the b-spline coefficients of the smoothing spline sp(u,v).  c */
/*  this smoothing spline depends on the parameter p in such a way that c */
/*    f(p) = sumi=1,mu(sumj=1,mv((z(i,j)-sp(u(i),v(j)))**2)             c */
/*  is a continuous, strictly decreasing function of p. moreover the    c */
/*  least-squares polynomial corresponds to p=0 and the least-squares   c */
/*  spline to p=infinity. then iteratively we have to determine the     c */
/*  positive value of p such that f(p)=s. the process which is proposed c */
/*  here makes use of rational interpolation. f(p) is approximated by a c */
/*  rational function r(p)=(u*p+v)/(p+w); three values of p (p1,p2,p3)  c */
/*  with corresponding values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s)c */
/*  are used to calculate the new value of p such that r(p)=s.          c */
/*  convergence is guaranteed by taking f1 > 0 and f3 < 0.              c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  initial value for p. */
    p1 = (float)0.;
    f1 = *fp0 - *s;
    p3 = -one;
    f3 = fpms;
    p = one;
    dzz[0] = dz[1];
    dzz[1] = dz[2];
    dzz[2] = dz[3];
    ich1 = 0;
    ich3 = 0;
/*  iteration process to find the root of f(p)=s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find the smoothing spline sp(u,v) and the corresponding sum f(p). */
	fpopdi_(&ifsu, &ifsv, &ifbu, &ifbv, &u[1], mu, &v[1], mv, &z__[1], mz,
		 z0, dzz, &iopt[1], idd, &tu[1], nu, &tv[1], nv, nuest, nvest,
		 &p, step, &c__[1], nc, fp, &fpintu[1], &fpintv[1], &nru[1], &
		nrv[1], &wrk[1], lwrk);
/*  test whether the approximation sp(u,v) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  test whether the maximum allowable number of iterations has been */
/*  reached. */
	if (iter == *maxit) {
	    goto L400;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L320;
	}
	if (f2 - f3 > acc) {
	    goto L310;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L350;
L310:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L320:
	if (ich1 != 0) {
	    goto L340;
	}
	if (f1 - f2 > acc) {
	    goto L330;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L350;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L350;
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L330:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
L340:
	if (f2 >= f1 || f2 <= f3) {
	    goto L410;
	}
/*  find the new value of p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L350:
	;
    }
/*  error codes and messages. */
L400:
    *ier = 3;
    goto L440;
L410:
    *ier = 2;
    goto L440;
L420:
    *ier = 1;
    goto L440;
L430:
    *ier = -1;
    *fp = (float)0.;
L440:
    return 0;
} /* fppogr_ */

/* Subroutine */ int fppola_(iopt1, iopt2, iopt3, m, u, v, z__, w, rad, s, 
	nuest, nvest, eta, tol, maxit, ib1, ib3, nc, ncc, intest, nrest, nu, 
	tu, nv, tv, c__, fp, sup, fpint, coord, f, ff, row, cs, cosi, a, q, 
	bu, bv, spu, spv, h__, index, nummer, wrk, lwrk, ier)
integer *iopt1, *iopt2, *iopt3, *m;
real *u, *v, *z__, *w;
doublereal (*rad) ();
real *s;
integer *nuest, *nvest;
real *eta, *tol;
integer *maxit, *ib1, *ib3, *nc, *ncc, *intest, *nrest, *nu;
real *tu;
integer *nv;
real *tv, *c__, *fp, *sup, *fpint, *coord, *f, *ff, *row, *cs, *cosi, *a, *q, 
	*bu, *bv, *spu, *spv, *h__;
integer *index, *nummer;
real *wrk;
integer *lwrk, *ier;
{
    /* System generated locals */
    integer a_dim1, a_offset, q_dim1, q_offset, bu_dim1, bu_offset, bv_dim1, 
	    bv_offset, spu_dim1, spu_offset, spv_dim1, spv_offset, i__1, i__2,
	     i__3, i__4, i__5, i__6;
    real r__1;

    /* Builtin functions */
    double atan(), sqrt(), cos(), sin();

    /* Local variables */
    static real half;
    static integer ncof;
    static real dmax__;
    static integer ipar, nreg, rank, iter;
    static real fpms, pinv;
    static integer irot, jrot, ipar1, i__, j, k, l, iband;
    static real p, r__;
    static integer ncoff;
    static real sigma, three, fpmax, ratio, c1, c2, c3, c4, f1, f2, f3, store;
    static integer i1, iband3;
    static real p1, p2, p3;
    static integer iband4, i2, i3;
    static real u2, u3;
    static integer j1, j2, lwest, l1, l2, l3, l4, nrint, numin, nvmin, iband1,
	     la;
    static real co;
    static integer ii, lf, il;
    static real pi;
    static integer in;
    static real si;
    static integer lh, ll;
    static real wi, rn;
    static integer lu;
    static real sq, zi;
    static integer lv;
    static real hu[4], uu, hv[4];
    extern /* Subroutine */ int fpback_(), fpdisc_(), fporde_(), fprank_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpbspl_(), fprota_(), fpgivs_(), fprppo_();
    static real pi2;
    static integer nr1, nu4, nv4;
    static real acc, fac, arg, one, hui, huj, eps, ten;
    static integer jlu;
    static real piv;
    static integer num, nrr;
    static real fac1, fac2, two;
    static integer nvv, nuu, ich1, ich3;
    static real con1, con4, con9;
    static integer num1;

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..user supplied function.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fporde,fpbspl,fpback,fpgivs,fprota,fprank,fpdisc,fprppo */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --nummer;
    spv_dim1 = *m;
    spv_offset = 1 + spv_dim1 * 1;
    spv -= spv_offset;
    spu_dim1 = *m;
    spu_offset = 1 + spu_dim1 * 1;
    spu -= spu_offset;
    --w;
    --z__;
    --v;
    --u;
    bu_dim1 = *nuest;
    bu_offset = 1 + bu_dim1 * 1;
    bu -= bu_offset;
    --tu;
    bv_dim1 = *nvest;
    bv_offset = 1 + bv_dim1 * 1;
    bv -= bv_offset;
    cosi -= 6;
    --cs;
    --row;
    --tv;
    --h__;
    --ff;
    --c__;
    q_dim1 = *ncc;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    a_dim1 = *ncc;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --f;
    --coord;
    --fpint;
    --index;
    --wrk;

    /* Function Body */
    one = (float)1.;
    two = (float)2.;
    three = (float)3.;
    ten = (float)10.;
    half = (float).5;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
    pi = atan(one) * 4;
    pi2 = pi + pi;
    ipar = *iopt2 * (*iopt2 + 3) / 2;
    ipar1 = ipar + 1;
    eps = sqrt(*eta);
    if (*iopt1 < 0) {
	goto L90;
    }
    numin = 9;
    nvmin = *iopt2 * (*iopt2 + 1) + 9;
/*  calculation of acc, the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
    if (*iopt1 == 0) {
	goto L10;
    }
    if (*s < *sup) {
	if (*nv - nvmin >= 0) {
	    goto L90;
	} else {
	    goto L70;
	}
    }
/*  if iopt1 = 0 we begin by computing the weighted least-squares */
/*  polymomial of the form */
/*     s(u,v) = f(1)*(1-u**3)+f(2)*u**3+f(3)*(u**2-u**3)+f(4)*(u-u**3) */
/*  where f(4) = 0 if iopt2> 0 , f(3) = 0 if iopt2 > 1 and */
/*        f(2) = 0 if iopt3> 0. */
/*  the corresponding weighted sum of squared residuals gives the upper */
/*  bound sup for the smoothing factor s. */
L10:
    *sup = (float)0.;
    for (i__ = 1; i__ <= 4; ++i__) {
	f[i__] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    a[i__ + j * a_dim1] = (float)0.;
/* L20: */
	}
    }
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wi = w[i__];
	zi = z__[i__] * wi;
	uu = u[i__];
	u2 = uu * uu;
	u3 = uu * u2;
	h__[1] = (one - u3) * wi;
	h__[2] = u3 * wi;
	h__[3] = u2 * (one - uu) * wi;
	h__[4] = uu * (one - u2) * wi;
	if (*iopt3 != 0) {
	    h__[2] = (float)0.;
	}
	if (*iopt2 > 1) {
	    h__[3] = (float)0.;
	}
	if (*iopt2 > 0) {
	    h__[4] = (float)0.;
	}
	for (j = 1; j <= 4; ++j) {
	    piv = h__[j];
	    if (piv == (float)0.) {
		goto L40;
	    }
	    fpgivs_(&piv, &a[j + a_dim1], &co, &si);
	    fprota_(&co, &si, &zi, &f[j]);
	    if (j == 4) {
		goto L40;
	    }
	    j1 = j + 1;
	    j2 = 1;
	    for (l = j1; l <= 4; ++l) {
		++j2;
		fprota_(&co, &si, &h__[l], &a[j + j2 * a_dim1]);
/* L30: */
	    }
L40:
	    ;
	}
	*sup += zi * zi;
/* L50: */
    }
    if (a[a_dim1 + 4] != (float)0.) {
	f[4] /= a[a_dim1 + 4];
    }
    if (a[a_dim1 + 3] != (float)0.) {
	f[3] = (f[3] - a[(a_dim1 << 1) + 3] * f[4]) / a[a_dim1 + 3];
    }
    if (a[a_dim1 + 2] != (float)0.) {
	f[2] = (f[2] - a[(a_dim1 << 1) + 2] * f[3] - a[a_dim1 * 3 + 2] * f[4])
		 / a[a_dim1 + 2];
    }
    if (a[a_dim1 + 1] != (float)0.) {
	f[1] = (f[1] - a[(a_dim1 << 1) + 1] * f[2] - a[a_dim1 * 3 + 1] * f[3] 
		- a[(a_dim1 << 2) + 1] * f[4]) / a[a_dim1 + 1];
    }
/*  find the b-spline representation of this least-squares polynomial */
    c1 = f[1];
    c4 = f[2];
    c2 = f[4] / three + c1;
    c3 = (f[3] + two * f[4]) / three + c1;
    *nu = 8;
    *nv = 8;
    for (i__ = 1; i__ <= 4; ++i__) {
	c__[i__] = c1;
	c__[i__ + 4] = c2;
	c__[i__ + 8] = c3;
	c__[i__ + 12] = c4;
	tu[i__] = (float)0.;
	tu[i__ + 4] = one;
	rn = (real) ((i__ << 1) - 9);
	tv[i__] = rn * pi;
	rn = (real) ((i__ << 1) - 1);
	tv[i__ + 4] = rn * pi;
/* L60: */
    }
    *fp = *sup;
/*  test whether the least-squares polynomial is an acceptable solution */
    fpms = *sup - *s;
    if (fpms < acc) {
	goto L960;
    }
/*  test whether we cannot further increase the number of knots. */
L70:
    if (*nuest < numin || *nvest < nvmin) {
	goto L950;
    }
/*  find the initial set of interior knots of the spline in case iopt1=0. */
    *nu = numin;
    *nv = nvmin;
    tu[5] = half;
    nvv = *nv - 8;
    rn = (real) (nvv + 1);
    fac = pi2 / rn;
    i__1 = nvv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rn = (real) i__;
	tv[i__ + 4] = rn * fac - pi;
/* L80: */
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 1 : computation of least-squares bicubic splines.              c */
/*  ******************************************************              c */
/*  if iopt1<0 we compute the least-squares bicubic spline according    c */
/*  to the given set of knots.                                          c */
/*  if iopt1>=0 we compute least-squares bicubic splines with in-       c */
/*  creasing numbers of knots until the corresponding sum f(p=inf)<=s.  c */
/*  the initial set of knots then depends on the value of iopt1         c */
/*    if iopt1=0 we start with one interior knot in the u-direction     c */
/*              (0.5) and 1+iopt2*(iopt2+1) in the v-direction.         c */
/*    if iopt1>0 we start with the set of knots found at the last       c */
/*              call of the routine.                                    c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  main loop for the different sets of knots. m is a save upper bound */
/*  for the number of trials. */
L90:
    i__1 = *m;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find the position of the additional knots which are needed for the */
/*  b-spline representation of s(u,v). */
	l1 = 4;
	l2 = l1;
	l3 = *nv - 3;
	l4 = l3;
	tv[l2] = -pi;
	tv[l3] = pi;
	for (i__ = 1; i__ <= 3; ++i__) {
	    ++l1;
	    --l2;
	    ++l3;
	    --l4;
	    tv[l2] = tv[l4] - pi2;
	    tv[l3] = tv[l1] + pi2;
/* L120: */
	}
	l = *nu;
	for (i__ = 1; i__ <= 4; ++i__) {
	    tu[i__] = (float)0.;
	    tu[l] = one;
	    --l;
/* L130: */
	}
/*  find nrint, the total number of knot intervals and nreg, the number */
/*  of panels in which the approximation domain is subdivided by the */
/*  intersection of knots. */
	nuu = *nu - 7;
	nvv = *nv - 7;
	nrr = nvv / 2;
	nr1 = nrr + 1;
	nrint = nuu + nvv;
	nreg = nuu * nvv;
/*  arrange the data points according to the panel they belong to. */
	fporde_(&u[1], &v[1], m, &c__3, &c__3, &tu[1], nu, &tv[1], nv, &
		nummer[1], &index[1], &nreg);
	if (*iopt2 == 0) {
	    goto L195;
	}
/*  find the b-spline coefficients cosi of the cubic spline */
/*  approximations for cr(v)=rad(v)*cos(v) and sr(v) = rad(v)*sin(v) */
/*  if iopt2=1, and additionally also for cr(v)**2,sr(v)**2 and */
/*  2*cr(v)*sr(v) if iopt2=2 */
	i__2 = nvv;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = ipar;
	    for (j = 1; j <= i__3; ++j) {
		cosi[j + i__ * 5] = (float)0.;
/* L135: */
	    }
	    i__3 = nvv;
	    for (j = 1; j <= i__3; ++j) {
		a[i__ + j * a_dim1] = (float)0.;
/* L140: */
	    }
	}
/*  the coefficients cosi are obtained from interpolation conditions */
/*  at the knots tv(i),i=4,5,...nv-4. */
	i__3 = nvv;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    l2 = i__ + 3;
	    arg = tv[l2];
	    fpbspl_(&tv[1], nv, &c__3, &arg, &l2, hv);
	    i__2 = nvv;
	    for (j = 1; j <= i__2; ++j) {
		row[j] = (float)0.;
/* L145: */
	    }
	    ll = i__;
	    for (j = 1; j <= 3; ++j) {
		if (ll > nvv) {
		    ll = 1;
		}
		row[ll] += hv[j - 1];
		++ll;
/* L150: */
	    }
	    co = cos(arg);
	    si = sin(arg);
	    r__ = (*rad)(&arg);
	    cs[1] = co * r__;
	    cs[2] = si * r__;
	    if (*iopt2 == 1) {
		goto L155;
	    }
	    cs[3] = cs[1] * cs[1];
	    cs[4] = cs[2] * cs[2];
	    cs[5] = cs[1] * cs[2];
L155:
	    i__2 = nvv;
	    for (j = 1; j <= i__2; ++j) {
		piv = row[j];
		if (piv == (float)0.) {
		    goto L170;
		}
		fpgivs_(&piv, &a[j + a_dim1], &co, &si);
		i__4 = ipar;
		for (l = 1; l <= i__4; ++l) {
		    fprota_(&co, &si, &cs[l], &cosi[l + j * 5]);
/* L160: */
		}
		if (j == nvv) {
		    goto L175;
		}
		j1 = j + 1;
		j2 = 1;
		i__4 = nvv;
		for (l = j1; l <= i__4; ++l) {
		    ++j2;
		    fprota_(&co, &si, &row[l], &a[j + j2 * a_dim1]);
/* L165: */
		}
L170:
		;
	    }
L175:
	    ;
	}
	i__3 = ipar;
	for (l = 1; l <= i__3; ++l) {
	    i__2 = nvv;
	    for (j = 1; j <= i__2; ++j) {
		cs[j] = cosi[l + j * 5];
/* L180: */
	    }
	    fpback_(&a[a_offset], &cs[1], &nvv, &nvv, &cs[1], ncc);
	    i__2 = nvv;
	    for (j = 1; j <= i__2; ++j) {
		cosi[l + j * 5] = cs[j];
/* L185: */
	    }
/* L190: */
	}
/*  find ncof, the dimension of the spline and ncoff, the number */
/*  of coefficients in the standard b-spline representation. */
L195:
	nu4 = *nu - 4;
	nv4 = *nv - 4;
	ncoff = nu4 * nv4;
	ncof = ipar1 + nvv * (nu4 - 1 - *iopt2 - *iopt3);
/*  find the bandwidth of the observation matrix a. */
	iband = nvv << 2;
	if (nuu - *iopt2 - *iopt3 <= 1) {
	    iband = ncof;
	}
	iband1 = iband - 1;
/*  initialize the observation matrix a. */
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    f[i__] = (float)0.;
	    i__2 = iband;
	    for (j = 1; j <= i__2; ++j) {
		a[i__ + j * a_dim1] = (float)0.;
/* L200: */
	    }
	}
/*  initialize the sum of squared residuals. */
	*fp = (float)0.;
	ratio = one + tu[6] / tu[5];
/*  fetch the data points in the new order. main loop for the */
/*  different panels. */
	i__2 = nreg;
	for (num = 1; num <= i__2; ++num) {
/*  fix certain constants for the current panel; jrot records the column */
/*  number of the first non-zero element in a row of the observation */
/*  matrix according to a data point of the panel. */
	    num1 = num - 1;
	    lu = num1 / nvv;
	    l1 = lu + 4;
	    lv = num1 - lu * nvv + 1;
	    l2 = lv + 3;
	    jrot = 0;
	    if (lu > *iopt2) {
		jrot = ipar1 + (lu - *iopt2 - 1) * nvv;
	    }
	    ++lu;
/*  test whether there are still data points in the current panel. */
	    in = index[num];
L210:
	    if (in == 0) {
		goto L380;
	    }
/*  fetch a new data point. */
	    wi = w[in];
	    zi = z__[in] * wi;
/*  evaluate for the u-direction, the 4 non-zero b-splines at u(in) */
	    fpbspl_(&tu[1], nu, &c__3, &u[in], &l1, hu);
/*  evaluate for the v-direction, the 4 non-zero b-splines at v(in) */
	    fpbspl_(&tv[1], nv, &c__3, &v[in], &l2, hv);
/*  store the value of these b-splines in spu and spv resp. */
	    for (i__ = 1; i__ <= 4; ++i__) {
		spu[in + i__ * spu_dim1] = hu[i__ - 1];
		spv[in + i__ * spv_dim1] = hv[i__ - 1];
/* L220: */
	    }
/*  initialize the new row of observation matrix. */
	    i__3 = iband;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h__[i__] = (float)0.;
/* L240: */
	    }
/*  calculate the non-zero elements of the new row by making the cross */
/*  products of the non-zero b-splines in u- and v-direction and */
/*  by taking into account the conditions of the splines. */
	    i__3 = nvv;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		row[i__] = (float)0.;
/* L250: */
	    }
/*  take into account the periodicity condition of the bicubic splines. */
	    ll = lv;
	    for (i__ = 1; i__ <= 4; ++i__) {
		if (ll > nvv) {
		    ll = 1;
		}
		row[ll] += hv[i__ - 1];
		++ll;
/* L260: */
	    }
/*  take into account the other conditions of the splines. */
	    if (*iopt2 == 0 || lu > *iopt2 + 1) {
		goto L280;
	    }
	    i__3 = ipar;
	    for (l = 1; l <= i__3; ++l) {
		cs[l] = (float)0.;
		i__4 = nvv;
		for (i__ = 1; i__ <= i__4; ++i__) {
		    cs[l] += row[i__] * cosi[l + i__ * 5];
/* L270: */
		}
	    }
/*  fill in the non-zero elements of the new row. */
L280:
	    j1 = 0;
	    for (j = 1; j <= 4; ++j) {
		jlu = j + lu;
		huj = hu[j - 1];
		if (jlu > *iopt2 + 2) {
		    goto L320;
		}
		switch ((int)jlu) {
		    case 1:  goto L290;
		    case 2:  goto L290;
		    case 3:  goto L300;
		    case 4:  goto L310;
		}
L290:
		h__[1] = huj;
		j1 = 1;
		goto L330;
L300:
		h__[1] += huj;
		h__[2] = huj * cs[1];
		h__[3] = huj * cs[2];
		j1 = 3;
		goto L330;
L310:
		h__[1] += huj;
		h__[2] += huj * ratio * cs[1];
		h__[3] += huj * ratio * cs[2];
		h__[4] = huj * cs[3];
		h__[5] = huj * cs[4];
		h__[6] = huj * cs[5];
		j1 = 6;
		goto L330;
L320:
		if (jlu > nu4 && *iopt3 != 0) {
		    goto L330;
		}
		i__4 = nvv;
		for (i__ = 1; i__ <= i__4; ++i__) {
		    ++j1;
		    h__[j1] = row[i__] * huj;
/* L325: */
		}
L330:
		;
	    }
	    i__4 = iband;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		h__[i__] *= wi;
/* L335: */
	    }
/*  rotate the row into triangle by givens transformations. */
	    irot = jrot;
	    i__4 = iband;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		++irot;
		piv = h__[i__];
		if (piv == (float)0.) {
		    goto L350;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a[irot + a_dim1], &co, &si);
/*  apply that transformation to the right hand side. */
		fprota_(&co, &si, &zi, &f[irot]);
		if (i__ == iband) {
		    goto L360;
		}
/*  apply that transformation to the left hand side. */
		i2 = 1;
		i3 = i__ + 1;
		i__3 = iband;
		for (j = i3; j <= i__3; ++j) {
		    ++i2;
		    fprota_(&co, &si, &h__[j], &a[irot + i2 * a_dim1]);
/* L340: */
		}
L350:
		;
	    }
/*  add the contribution of the row to the sum of squares of residual */
/*  right hand sides. */
L360:
/* Computing 2nd power */
	    r__1 = zi;
	    *fp += r__1 * r__1;
/*  find the number of the next data point in the panel. */
/* L370: */
	    in = nummer[in];
	    goto L210;
L380:
	    ;
	}
/*  find dmax, the maximum value for the diagonal elements in the reduced */
/*  triangle. */
	dmax__ = (float)0.;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (a[i__ + a_dim1] <= dmax__) {
		goto L390;
	    }
	    dmax__ = a[i__ + a_dim1];
L390:
	    ;
	}
/*  check whether the observation matrix is rank deficient. */
	sigma = eps * dmax__;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (a[i__ + a_dim1] <= sigma) {
		goto L410;
	    }
/* L400: */
	}
/*  backward substitution in case of full rank. */
	fpback_(&a[a_offset], &f[1], &ncof, &iband, &c__[1], ncc);
	rank = ncof;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    q[i__ + q_dim1] = a[i__ + a_dim1] / dmax__;
/* L405: */
	}
	goto L430;
/*  in case of rank deficiency, find the minimum norm solution. */
L410:
	lwest = ncof * iband + ncof + iband;
	if (*lwrk < lwest) {
	    goto L925;
	}
	lf = 1;
	lh = lf + ncof;
	la = lh + iband;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ff[i__] = f[i__];
	    i__4 = iband;
	    for (j = 1; j <= i__4; ++j) {
		q[i__ + j * q_dim1] = a[i__ + j * a_dim1];
/* L420: */
	    }
	}
	fprank_(&q[q_offset], &ff[1], &ncof, &iband, ncc, &sigma, &c__[1], &
		sq, &rank, &wrk[la], &wrk[lf], &wrk[lh]);
	i__4 = ncof;
	for (i__ = 1; i__ <= i__4; ++i__) {
	    q[i__ + q_dim1] /= dmax__;
/* L425: */
	}
/*  add to the sum of squared residuals, the contribution of reducing */
/*  the rank. */
	*fp += sq;
/*  find the coefficients in the standard b-spline representation of */
/*  the spline. */
L430:
	fprppo_(nu, nv, iopt2, iopt3, &cosi[6], &ratio, &c__[1], &ff[1], &
		ncoff);
/*  test whether the least-squares spline is an acceptable solution. */
	if (*iopt1 < 0) {
	    if (*fp <= (float)0.) {
		goto L970;
	    } else {
		goto L980;
	    }
	}
	fpms = *fp - *s;
	if (dabs(fpms) <= acc) {
	    if (*fp <= (float)0.) {
		goto L970;
	    } else {
		goto L980;
	    }
	}
/*  if f(p=inf) < s, accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L580;
	}
/*  test whether we cannot further increase the number of knots */
	if (*m < ncof) {
	    goto L935;
	}
/*  search where to add a new knot. */
/*  find for each interval the sum of squared residuals fpint for the */
/*  data points having the coordinate belonging to that knot interval. */
/*  calculate also coord which is the same sum, weighted by the position */
/*  of the data points considered. */
/* L440: */
	i__4 = nrint;
	for (i__ = 1; i__ <= i__4; ++i__) {
	    fpint[i__] = (float)0.;
	    coord[i__] = (float)0.;
/* L450: */
	}
	i__4 = nreg;
	for (num = 1; num <= i__4; ++num) {
	    num1 = num - 1;
	    lu = num1 / nvv;
	    l1 = lu + 1;
	    lv = num1 - lu * nvv;
	    l2 = lv + 1 + nuu;
	    jrot = lu * nv4 + lv;
	    in = index[num];
L460:
	    if (in == 0) {
		goto L490;
	    }
	    store = (float)0.;
	    i1 = jrot;
	    for (i__ = 1; i__ <= 4; ++i__) {
		hui = spu[in + i__ * spu_dim1];
		j1 = i1;
		for (j = 1; j <= 4; ++j) {
		    ++j1;
		    store += hui * spv[in + j * spv_dim1] * c__[j1];
/* L470: */
		}
		i1 += nv4;
/* L480: */
	    }
/* Computing 2nd power */
	    r__1 = w[in] * (z__[in] - store);
	    store = r__1 * r__1;
	    fpint[l1] += store;
	    coord[l1] += store * u[in];
	    fpint[l2] += store;
	    coord[l2] += store * v[in];
	    in = nummer[in];
	    goto L460;
L490:
	    ;
	}
/* bring together the information concerning knot panels which are */
/* symmetric with respect to the origin. */
	i__4 = nrr;
	for (i__ = 1; i__ <= i__4; ++i__) {
	    l1 = nuu + i__;
	    l2 = l1 + nrr;
	    fpint[l1] += fpint[l2];
	    coord[l1] = coord[l1] + coord[l2] - pi * fpint[l2];
/* L495: */
	}
/*  find the interval for which fpint is maximal on the condition that */
/*  there still can be added a knot. */
	l1 = 1;
	l2 = nuu + nrr;
	if (*nuest < *nu + 1) {
	    l1 = nuu + 1;
	}
	if (*nvest < *nv + 2) {
	    l2 = nuu;
	}
/*  test whether we cannot further increase the number of knots. */
	if (l1 > l2) {
	    goto L950;
	}
L500:
	fpmax = (float)0.;
	l = 0;
	i__4 = l2;
	for (i__ = l1; i__ <= i__4; ++i__) {
	    if (fpmax >= fpint[i__]) {
		goto L510;
	    }
	    l = i__;
	    fpmax = fpint[i__];
L510:
	    ;
	}
	if (l == 0) {
	    goto L930;
	}
/*  calculate the position of the new knot. */
	arg = coord[l] / fpint[l];
/*  test in what direction the new knot is going to be added. */
	if (l > nuu) {
	    goto L530;
	}
/*  addition in the u-direction */
	l4 = l + 4;
	fpint[l] = (float)0.;
	fac1 = tu[l4] - arg;
	fac2 = arg - tu[l4 - 1];
	if (fac1 > ten * fac2 || fac2 > ten * fac1) {
	    goto L500;
	}
	j = *nu;
	i__4 = *nu;
	for (i__ = l4; i__ <= i__4; ++i__) {
	    tu[j + 1] = tu[j];
	    --j;
/* L520: */
	}
	tu[l4] = arg;
	++(*nu);
	goto L570;
/*  addition in the v-direction */
L530:
	l4 = l + 4 - nuu;
	fpint[l] = (float)0.;
	fac1 = tv[l4] - arg;
	fac2 = arg - tv[l4 - 1];
	if (fac1 > ten * fac2 || fac2 > ten * fac1) {
	    goto L500;
	}
	ll = nrr + 4;
	j = ll;
	i__4 = ll;
	for (i__ = l4; i__ <= i__4; ++i__) {
	    tv[j + 1] = tv[j];
	    --j;
/* L550: */
	}
	tv[l4] = arg;
	*nv += 2;
	++nrr;
	i__4 = ll;
	for (i__ = 5; i__ <= i__4; ++i__) {
	    j = i__ + nrr;
	    tv[j] = tv[i__] + pi;
/* L560: */
	}
/*  restart the computations with the new set of knots. */
L570:
	;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 2: determination of the smoothing bicubic spline.               c */
/* ******************************************************               c */
/* we have determined the number of knots and their position. we now    c */
/* compute the coefficients of the smoothing spline sp(u,v).            c */
/* the observation matrix a is extended by the rows of a matrix, expres-c */
/* sing that sp(u,v) must be a constant function in the variable        c */
/* v and a cubic polynomial in the variable u. the corresponding        c */
/* weights of these additional rows are set to 1/(p). iteratively       c */
/* we than have to determine the value of p such that f(p) = sum((w(i)* c */
/* (z(i)-sp(u(i),v(i))))**2)  be = s.                                   c */
/* we already know that the least-squares polynomial corresponds to p=0,c */
/* and that the least-squares bicubic spline corresponds to p=infin.    c */
/* the iteration process makes use of rational interpolation. since f(p)c */
/* is a convex and strictly decreasing function of p, it can be approx- c */
/* imated by a rational function of the form r(p) = (u*p+v)/(p+w).      c */
/* three values of p (p1,p2,p3) with corresponding values of f(p) (f1=  c */
/* f(p1)-s,f2=f(p2)-s,f3=f(p3)-s) are used to calculate the new value   c */
/* of p such that r(p)=s. convergence is guaranteed by taking f1>0,f3<0.c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  evaluate the discontinuity jumps of the 3-th order derivative of */
/*  the b-splines at the knots tu(l),l=5,...,nu-4. */
L580:
    fpdisc_(&tu[1], nu, &c__5, &bu[bu_offset], nuest);
/*  evaluate the discontinuity jumps of the 3-th order derivative of */
/*  the b-splines at the knots tv(l),l=5,...,nv-4. */
    fpdisc_(&tv[1], nv, &c__5, &bv[bv_offset], nvest);
/*  initial value for p. */
    p1 = (float)0.;
    f1 = *sup - *s;
    p3 = -one;
    f3 = fpms;
    p = (float)0.;
    i__1 = ncof;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p += a[i__ + a_dim1];
/* L590: */
    }
    rn = (real) ncof;
    p = rn / p;
/*  find the bandwidth of the extended observation matrix. */
    iband4 = iband + ipar1;
    if (iband4 > ncof) {
	iband4 = ncof;
    }
    iband3 = iband4 - 1;
    ich1 = 0;
    ich3 = 0;
    nuu = nu4 - *iopt3 - 1;
/*  iteration process to find the root of f(p)=s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
	pinv = one / p;
/*  store the triangularized observation matrix into q. */
	i__4 = ncof;
	for (i__ = 1; i__ <= i__4; ++i__) {
	    ff[i__] = f[i__];
	    i__2 = iband4;
	    for (j = 1; j <= i__2; ++j) {
		q[i__ + j * q_dim1] = (float)0.;
/* L620: */
	    }
	    i__2 = iband;
	    for (j = 1; j <= i__2; ++j) {
		q[i__ + j * q_dim1] = a[i__ + j * a_dim1];
/* L630: */
	    }
	}
/*  extend the observation matrix with the rows of a matrix, expressing */
/*  that for u=constant sp(u,v) must be a constant function. */
	i__2 = nv4;
	for (i__ = 5; i__ <= i__2; ++i__) {
	    ii = i__ - 4;
	    i__4 = nvv;
	    for (l = 1; l <= i__4; ++l) {
		row[l] = (float)0.;
/* L635: */
	    }
	    ll = ii;
	    for (l = 1; l <= 5; ++l) {
		if (ll > nvv) {
		    ll = 1;
		}
		row[ll] += bv[ii + l * bv_dim1];
		++ll;
/* L640: */
	    }
	    i__4 = nuu;
	    for (j = 1; j <= i__4; ++j) {
/*  initialize the new row. */
		i__3 = iband;
		for (l = 1; l <= i__3; ++l) {
		    h__[l] = (float)0.;
/* L645: */
		}
/*  fill in the non-zero elements of the row. jrot records the column */
/*  number of the first non-zero element in the row. */
		if (j > *iopt2) {
		    goto L665;
		}
		if (j == 2) {
		    goto L655;
		}
		for (k = 1; k <= 2; ++k) {
		    cs[k] = (float)0.;
		    i__3 = nvv;
		    for (l = 1; l <= i__3; ++l) {
			cs[k] += cosi[k + l * 5] * row[l];
/* L650: */
		    }
		}
		h__[1] = cs[1];
		h__[2] = cs[2];
		jrot = 2;
		goto L675;
L655:
		for (k = 3; k <= 5; ++k) {
		    cs[k] = (float)0.;
		    i__3 = nvv;
		    for (l = 1; l <= i__3; ++l) {
			cs[k] += cosi[k + l * 5] * row[l];
/* L660: */
		    }
		}
		h__[1] = cs[1] * ratio;
		h__[2] = cs[2] * ratio;
		h__[3] = cs[3];
		h__[4] = cs[4];
		h__[5] = cs[5];
		jrot = 2;
		goto L675;
L665:
		i__3 = nvv;
		for (l = 1; l <= i__3; ++l) {
		    h__[l] = row[l];
/* L670: */
		}
		jrot = ipar1 + 1 + (j - *iopt2 - 1) * nvv;
L675:
		i__3 = iband;
		for (l = 1; l <= i__3; ++l) {
		    h__[l] *= pinv;
/* L677: */
		}
		zi = (float)0.;
/*  rotate the new row into triangle by givens transformations. */
		i__3 = ncof;
		for (irot = jrot; irot <= i__3; ++irot) {
		    piv = h__[1];
/* Computing MIN */
		    i__5 = iband1, i__6 = ncof - irot;
		    i2 = min(i__5,i__6);
		    if (piv == (float)0.) {
			if (i2 <= 0) {
			    goto L720;
			} else {
			    goto L690;
			}
		    }
/*  calculate the parameters of the givens transformation. */
		    fpgivs_(&piv, &q[irot + q_dim1], &co, &si);
/*  apply that givens transformation to the right hand side. */
		    fprota_(&co, &si, &zi, &ff[irot]);
		    if (i2 == 0) {
			goto L720;
		    }
/*  apply that givens transformation to the left hand side. */
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			l1 = l + 1;
			fprota_(&co, &si, &h__[l1], &q[irot + l1 * q_dim1]);
/* L680: */
		    }
L690:
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			h__[l] = h__[l + 1];
/* L700: */
		    }
		    h__[i2 + 1] = (float)0.;
/* L710: */
		}
L720:
		;
	    }
	}
/*  extend the observation matrix with the rows of a matrix expressing */
/*  that for v=constant. sp(u,v) must be a cubic polynomial. */
	i__4 = nu4;
	for (i__ = 5; i__ <= i__4; ++i__) {
	    ii = i__ - 4;
	    i__2 = nvv;
	    for (j = 1; j <= i__2; ++j) {
/*  initialize the new row */
		i__3 = iband4;
		for (l = 1; l <= i__3; ++l) {
		    h__[l] = (float)0.;
/* L730: */
		}
/*  fill in the non-zero elements of the row. jrot records the column */
/*  number of the first non-zero element in the row. */
		j1 = 1;
		for (l = 1; l <= 5; ++l) {
		    il = ii + l - 1;
		    if (il == nu4 && *iopt3 != 0) {
			goto L760;
		    }
		    if (il > *iopt2 + 1) {
			goto L750;
		    }
		    switch ((int)il) {
			case 1:  goto L735;
			case 2:  goto L740;
			case 3:  goto L745;
		    }
L735:
		    h__[1] = bu[ii + l * bu_dim1];
		    j1 = j + 1;
		    goto L760;
L740:
		    h__[1] += bu[ii + l * bu_dim1];
		    h__[2] = bu[ii + l * bu_dim1] * cosi[j * 5 + 1];
		    h__[3] = bu[ii + l * bu_dim1] * cosi[j * 5 + 2];
		    j1 = j + 3;
		    goto L760;
L745:
		    h__[1] += bu[ii + l * bu_dim1];
		    h__[2] = bu[ii + l * bu_dim1] * cosi[j * 5 + 1] * ratio;
		    h__[3] = bu[ii + l * bu_dim1] * cosi[j * 5 + 2] * ratio;
		    h__[4] = bu[ii + l * bu_dim1] * cosi[j * 5 + 3];
		    h__[5] = bu[ii + l * bu_dim1] * cosi[j * 5 + 4];
		    h__[6] = bu[ii + l * bu_dim1] * cosi[j * 5 + 5];
		    j1 = j + 6;
		    goto L760;
L750:
		    h__[j1] = bu[ii + l * bu_dim1];
		    j1 += nvv;
L760:
		    ;
		}
		i__3 = iband4;
		for (l = 1; l <= i__3; ++l) {
		    h__[l] *= pinv;
/* L765: */
		}
		zi = (float)0.;
		jrot = 1;
		if (ii > *iopt2 + 1) {
		    jrot = ipar1 + (ii - *iopt2 - 2) * nvv + j;
		}
/*  rotate the new row into triangle by givens transformations. */
		i__3 = ncof;
		for (irot = jrot; irot <= i__3; ++irot) {
		    piv = h__[1];
/* Computing MIN */
		    i__5 = iband3, i__6 = ncof - irot;
		    i2 = min(i__5,i__6);
		    if (piv == (float)0.) {
			if (i2 <= 0) {
			    goto L810;
			} else {
			    goto L780;
			}
		    }
/*  calculate the parameters of the givens transformation. */
		    fpgivs_(&piv, &q[irot + q_dim1], &co, &si);
/*  apply that givens transformation to the right hand side. */
		    fprota_(&co, &si, &zi, &ff[irot]);
		    if (i2 == 0) {
			goto L810;
		    }
/*  apply that givens transformation to the left hand side. */
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			l1 = l + 1;
			fprota_(&co, &si, &h__[l1], &q[irot + l1 * q_dim1]);
/* L770: */
		    }
L780:
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			h__[l] = h__[l + 1];
/* L790: */
		    }
		    h__[i2 + 1] = (float)0.;
/* L800: */
		}
L810:
		;
	    }
	}
/*  find dmax, the maximum value for the diagonal elements in the */
/*  reduced triangle. */
	dmax__ = (float)0.;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (q[i__ + q_dim1] <= dmax__) {
		goto L820;
	    }
	    dmax__ = q[i__ + q_dim1];
L820:
	    ;
	}
/*  check whether the matrix is rank deficient. */
	sigma = eps * dmax__;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (q[i__ + q_dim1] <= sigma) {
		goto L840;
	    }
/* L830: */
	}
/*  backward substitution in case of full rank. */
	fpback_(&q[q_offset], &ff[1], &ncof, &iband4, &c__[1], ncc);
	rank = ncof;
	goto L845;
/*  in case of rank deficiency, find the minimum norm solution. */
L840:
	lwest = ncof * iband4 + ncof + iband4;
	if (*lwrk < lwest) {
	    goto L925;
	}
	lf = 1;
	lh = lf + ncof;
	la = lh + iband4;
	fprank_(&q[q_offset], &ff[1], &ncof, &iband4, ncc, &sigma, &c__[1], &
		sq, &rank, &wrk[la], &wrk[lf], &wrk[lh]);
L845:
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    q[i__ + q_dim1] /= dmax__;
/* L850: */
	}
/*  find the coefficients in the standard b-spline representation of */
/*  the polar spline. */
	fprppo_(nu, nv, iopt2, iopt3, &cosi[6], &ratio, &c__[1], &ff[1], &
		ncoff);
/*  compute f(p). */
	*fp = (float)0.;
	i__2 = nreg;
	for (num = 1; num <= i__2; ++num) {
	    num1 = num - 1;
	    lu = num1 / nvv;
	    lv = num1 - lu * nvv;
	    jrot = lu * nv4 + lv;
	    in = index[num];
L860:
	    if (in == 0) {
		goto L890;
	    }
	    store = (float)0.;
	    i1 = jrot;
	    for (i__ = 1; i__ <= 4; ++i__) {
		hui = spu[in + i__ * spu_dim1];
		j1 = i1;
		for (j = 1; j <= 4; ++j) {
		    ++j1;
		    store += hui * spv[in + j * spv_dim1] * c__[j1];
/* L870: */
		}
		i1 += nv4;
/* L880: */
	    }
/* Computing 2nd power */
	    r__1 = w[in] * (z__[in] - store);
	    *fp += r__1 * r__1;
	    in = nummer[in];
	    goto L860;
L890:
	    ;
	}
/*  test whether the approximation sp(u,v) is an acceptable solution */
	fpms = *fp - *s;
	if (dabs(fpms) <= acc) {
	    goto L980;
	}
/*  test whether the maximum allowable number of iterations has been */
/*  reached. */
	if (iter == *maxit) {
	    goto L940;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L900;
	}
	if (f2 - f3 > acc) {
	    goto L895;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L920;
L895:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L900:
	if (ich1 != 0) {
	    goto L910;
	}
	if (f1 - f2 > acc) {
	    goto L905;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L920;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L920;
L905:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L910:
	if (f2 >= f1 || f2 <= f3) {
	    goto L945;
	}
/*  find the new value of p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L920:
	;
    }
/*  error codes and messages. */
L925:
    *ier = lwest;
    goto L990;
L930:
    *ier = 5;
    goto L990;
L935:
    *ier = 4;
    goto L990;
L940:
    *ier = 3;
    goto L990;
L945:
    *ier = 2;
    goto L990;
L950:
    *ier = 1;
    goto L990;
L960:
    *ier = -2;
    goto L990;
L970:
    *ier = -1;
    *fp = (float)0.;
L980:
    if (ncof != rank) {
	*ier = -rank;
    }
L990:
    return 0;
} /* fppola_ */

/* Subroutine */ int fprank_(a, f, n, m, na, tol, c__, sq, rank, aa, ff, h__)
real *a, *f;
integer *n, *m, *na;
real *tol, *c__, *sq;
integer *rank;
real *aa, *ff, *h__;
{
    /* System generated locals */
    integer a_dim1, a_offset, aa_dim1, aa_offset, i__1, i__2, i__3;
    real r__1;

    /* Local variables */
    static integer i__, j, k;
    static doublereal stor1, stor2, stor3;
    static integer i1, i2, j1, j2, j3, m1;
    static doublereal store;
    static integer ii, ij, jj, kk, nl;
    static real yi;
    extern /* Subroutine */ int fprota_(), fpgivs_();
    static real fac, cos__, sin__, piv;

/*  subroutine fprank finds the minimum norm solution of a least- */
/*  squares problem in case of rank deficiency. */

/*  input parameters: */
/*    a : array, which contains the non-zero elements of the observation */
/*        matrix after triangularization by givens transformations. */
/*    f : array, which contains the transformed right hand side. */
/*    n : integer,wich contains the dimension of a. */
/*    m : integer, which denotes the bandwidth of a. */
/*  tol : real value, giving a threshold to determine the rank of a. */

/*  output parameters: */
/*    c : array, which contains the minimum norm solution. */
/*   sq : real value, giving the contribution of reducing the rank */
/*        to the sum of squared residuals. */
/* rank : integer, which contains the rank of matrix a. */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpgivs,fprota */
/*  .. */
    /* Parameter adjustments */
    --ff;
    --c__;
    --f;
    --h__;
    aa_dim1 = *n;
    aa_offset = 1 + aa_dim1 * 1;
    aa -= aa_offset;
    a_dim1 = *na;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;

    /* Function Body */
    m1 = *m - 1;
/*  the rank deficiency nl is considered to be the number of sufficient */
/*  small diagonal elements of a. */
    nl = 0;
    *sq = (float)0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (a[i__ + a_dim1] > *tol) {
	    goto L90;
	}
/*  if a sufficient small diagonal element is found, we put it to */
/*  zero. the remainder of the row corresponding to that zero diagonal */
/*  element is then rotated into triangle by givens rotations . */
/*  the rank deficiency is increased by one. */
	++nl;
	if (i__ == *n) {
	    goto L90;
	}
	yi = f[i__];
	i__2 = m1;
	for (j = 1; j <= i__2; ++j) {
	    h__[j] = a[i__ + (j + 1) * a_dim1];
/* L10: */
	}
	h__[*m] = (float)0.;
	i1 = i__ + 1;
	i__2 = *n;
	for (ii = i1; ii <= i__2; ++ii) {
/* Computing MIN */
	    i__3 = *n - ii;
	    i2 = min(i__3,m1);
	    piv = h__[1];
	    if (piv == (float)0.) {
		goto L30;
	    }
	    fpgivs_(&piv, &a[ii + a_dim1], &cos__, &sin__);
	    fprota_(&cos__, &sin__, &yi, &f[ii]);
	    if (i2 == 0) {
		goto L70;
	    }
	    i__3 = i2;
	    for (j = 1; j <= i__3; ++j) {
		j1 = j + 1;
		fprota_(&cos__, &sin__, &h__[j1], &a[ii + j1 * a_dim1]);
		h__[j] = h__[j1];
/* L20: */
	    }
	    goto L50;
L30:
	    if (i2 == 0) {
		goto L70;
	    }
	    i__3 = i2;
	    for (j = 1; j <= i__3; ++j) {
		h__[j] = h__[j + 1];
/* L40: */
	    }
L50:
	    h__[i2 + 1] = (float)0.;
/* L60: */
	}
/*  add to the sum of squared residuals the contribution of deleting */
/*  the row with small diagonal element. */
L70:
/* Computing 2nd power */
	r__1 = yi;
	*sq += r__1 * r__1;
L90:
	;
    }
/*  rank denotes the rank of a. */
    *rank = *n - nl;
/*  let b denote the (rank*n) upper trapezoidal matrix which can be */
/*  obtained from the (n*n) upper triangular matrix a by deleting */
/*  the rows and interchanging the columns corresponding to a zero */
/*  diagonal element. if this matrix is factorized using givens */
/*  transformations as  b = (r) (u)  where */
/*    r is a (rank*rank) upper triangular matrix, */
/*    u is a (rank*n) orthonormal matrix */
/*  then the minimal least-squares solution c is given by c = b' v, */
/*  where v is the solution of the system  (r) (r)' v = g  and */
/*  g denotes the vector obtained from the old right hand side f, by */
/*  removing the elements corresponding to a zero diagonal element of a. */
/*  initialization. */
    i__1 = *rank;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {
	    aa[i__ + j * aa_dim1] = (float)0.;
/* L100: */
	}
    }
/*  form in aa the upper triangular matrix obtained from a by */
/*  removing rows and columns with zero diagonal elements. form in ff */
/*  the new right hand side by removing the elements of the old right */
/*  hand side corresponding to a deleted row. */
    ii = 0;
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
	if (a[i__ + a_dim1] <= *tol) {
	    goto L120;
	}
	++ii;
	ff[ii] = f[i__];
	aa[ii + aa_dim1] = a[i__ + a_dim1];
	jj = ii;
	kk = 1;
	j = i__;
/* Computing MIN */
	i__1 = j - 1;
	j1 = min(i__1,m1);
	if (j1 == 0) {
	    goto L120;
	}
	i__1 = j1;
	for (k = 1; k <= i__1; ++k) {
	    --j;
	    if (a[j + a_dim1] <= *tol) {
		goto L110;
	    }
	    ++kk;
	    --jj;
	    aa[jj + kk * aa_dim1] = a[j + (k + 1) * a_dim1];
L110:
	    ;
	}
L120:
	;
    }
/*  form successively in h the columns of a with a zero diagonal element. */
    ii = 0;
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
	++ii;
	if (a[i__ + a_dim1] > *tol) {
	    goto L200;
	}
	--ii;
	if (ii == 0) {
	    goto L200;
	}
	jj = 1;
	j = i__;
/* Computing MIN */
	i__1 = j - 1;
	j1 = min(i__1,m1);
	i__1 = j1;
	for (k = 1; k <= i__1; ++k) {
	    --j;
	    if (a[j + a_dim1] <= *tol) {
		goto L130;
	    }
	    h__[jj] = a[j + (k + 1) * a_dim1];
	    ++jj;
L130:
	    ;
	}
	i__1 = *m;
	for (kk = jj; kk <= i__1; ++kk) {
	    h__[kk] = (float)0.;
/* L140: */
	}
/*  rotate this column into aa by givens transformations. */
	jj = ii;
	i__1 = ii;
	for (i1 = 1; i1 <= i__1; ++i1) {
/* Computing MIN */
	    i__3 = jj - 1;
	    j1 = min(i__3,m1);
	    piv = h__[1];
	    if (piv != (float)0.) {
		goto L160;
	    }
	    if (j1 == 0) {
		goto L200;
	    }
	    i__3 = j1;
	    for (j2 = 1; j2 <= i__3; ++j2) {
		j3 = j2 + 1;
		h__[j2] = h__[j3];
/* L150: */
	    }
	    goto L180;
L160:
	    fpgivs_(&piv, &aa[jj + aa_dim1], &cos__, &sin__);
	    if (j1 == 0) {
		goto L200;
	    }
	    kk = jj;
	    i__3 = j1;
	    for (j2 = 1; j2 <= i__3; ++j2) {
		j3 = j2 + 1;
		--kk;
		fprota_(&cos__, &sin__, &h__[j3], &aa[kk + j3 * aa_dim1]);
		h__[j2] = h__[j3];
/* L170: */
	    }
L180:
	    --jj;
	    h__[j3] = (float)0.;
/* L190: */
	}
L200:
	;
    }
/*  solve the system (aa) (f1) = ff */
    ff[*rank] /= aa[*rank + aa_dim1];
    i__ = *rank - 1;
    if (i__ == 0) {
	goto L230;
    }
    i__2 = *rank;
    for (j = 2; j <= i__2; ++j) {
	store = ff[i__];
/* Computing MIN */
	i__1 = j - 1;
	i1 = min(i__1,m1);
	k = i__;
	i__1 = i1;
	for (ii = 1; ii <= i__1; ++ii) {
	    ++k;
	    stor1 = ff[k];
	    stor2 = aa[i__ + (ii + 1) * aa_dim1];
	    store -= stor1 * stor2;
/* L210: */
	}
	stor1 = aa[i__ + aa_dim1];
	ff[i__] = store / stor1;
	--i__;
/* L220: */
    }
/*  solve the system  (aa)' (f2) = f1 */
L230:
    ff[1] /= aa[aa_dim1 + 1];
    if (*rank == 1) {
	goto L260;
    }
    i__2 = *rank;
    for (j = 2; j <= i__2; ++j) {
	store = ff[j];
/* Computing MIN */
	i__1 = j - 1;
	i1 = min(i__1,m1);
	k = j;
	i__1 = i1;
	for (ii = 1; ii <= i__1; ++ii) {
	    --k;
	    stor1 = ff[k];
	    stor2 = aa[k + (ii + 1) * aa_dim1];
	    store -= stor1 * stor2;
/* L240: */
	}
	stor1 = aa[j + aa_dim1];
	ff[j] = store / stor1;
/* L250: */
    }
/*  premultiply f2 by the transpoze of a. */
L260:
    k = 0;
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
	store = (float)0.;
	if (a[i__ + a_dim1] > *tol) {
	    ++k;
	}
	j1 = min(i__,*m);
	kk = k;
	ij = i__ + 1;
	i__1 = j1;
	for (j = 1; j <= i__1; ++j) {
	    --ij;
	    if (a[ij + a_dim1] <= *tol) {
		goto L270;
	    }
	    stor1 = a[ij + j * a_dim1];
	    stor2 = ff[kk];
	    store += stor1 * stor2;
	    --kk;
L270:
	    ;
	}
	c__[i__] = store;
/* L280: */
    }
/*  add to the sum of squared residuals the contribution of putting */
/*  to zero the small diagonal elements of matrix (a). */
    stor3 = (float)0.;
    i__2 = *n;
    for (i__ = 1; i__ <= i__2; ++i__) {
	if (a[i__ + a_dim1] > *tol) {
	    goto L310;
	}
	store = f[i__];
/* Computing MIN */
	i__1 = *n - i__;
	i1 = min(i__1,m1);
	if (i1 == 0) {
	    goto L300;
	}
	i__1 = i1;
	for (j = 1; j <= i__1; ++j) {
	    ij = i__ + j;
	    stor1 = c__[ij];
	    stor2 = a[i__ + (j + 1) * a_dim1];
	    store -= stor1 * stor2;
/* L290: */
	}
L300:
	fac = a[i__ + a_dim1] * c__[i__];
	stor1 = a[i__ + a_dim1];
	stor2 = c__[i__];
	stor1 *= stor2;
	stor3 += stor1 * (stor1 - store - store);
L310:
	;
    }
    fac = stor3;
    *sq += fac;
    return 0;
} /* fprank_ */

doublereal fprati_(p1, f1, p2, f2, p3, f3)
real *p1, *f1, *p2, *f2, *p3, *f3;
{
    /* System generated locals */
    real ret_val;

    /* Local variables */
    static real p, h1, h2, h3;

/*  given three points (p1,f1),(p2,f2) and (p3,f3), function fprati */
/*  gives the value of p such that the rational interpolating function */
/*  of the form r(p) = (u*p+v)/(p+w) equals zero at p. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..local scalars.. */
/*  .. */
    if (*p3 > (float)0.) {
	goto L10;
    }
/*  value of p in case p3 = infinity. */
    p = (*p1 * (*f1 - *f3) * *f2 - *p2 * (*f2 - *f3) * *f1) / ((*f1 - *f2) * *
	    f3);
    goto L20;
/*  value of p in case p3 ^= infinity. */
L10:
    h1 = *f1 * (*f2 - *f3);
    h2 = *f2 * (*f3 - *f1);
    h3 = *f3 * (*f1 - *f2);
    p = -(*p1 * *p2 * h3 + *p2 * *p3 * h1 + *p3 * *p1 * h2) / (*p1 * h1 + *p2 
	    * h2 + *p3 * h3);
/*  adjust the value of p1,f1,p3 and f3 such that f1 > 0 and f3 < 0. */
L20:
    if (*f2 < (float)0.) {
	goto L30;
    }
    *p1 = *p2;
    *f1 = *f2;
    goto L40;
L30:
    *p3 = *p2;
    *f3 = *f2;
L40:
    ret_val = p;
    return ret_val;
} /* fprati_ */

/* Subroutine */ int fpregr_(iopt, x, mx, y, my, z__, mz, xb, xe, yb, ye, kx, 
	ky, s, nxest, nyest, tol, maxit, nc, nx, tx, ny, ty, c__, fp, fp0, 
	fpold, reducx, reducy, fpintx, fpinty, lastdi, nplusx, nplusy, nrx, 
	nry, nrdatx, nrdaty, wrk, lwrk, ier)
integer *iopt;
real *x;
integer *mx;
real *y;
integer *my;
real *z__;
integer *mz;
real *xb, *xe, *yb, *ye;
integer *kx, *ky;
real *s;
integer *nxest, *nyest;
real *tol;
integer *maxit, *nc, *nx;
real *tx;
integer *ny;
real *ty, *c__, *fp, *fp0, *fpold, *reducx, *reducy, *fpintx, *fpinty;
integer *lastdi, *nplusx, *nplusy, *nrx, *nry, *nrdatx, *nrdaty;
real *wrk;
integer *lwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;

    /* Local variables */
    static real half;
    static integer ncof, ifbx, ifby, iter;
    static real fpms;
    static integer ifsx, ifsy, nplx, nply, mynx, i__, j, l;
    static real p, f1, f2, f3;
    static integer nminx, nmaxx, nmaxy, nminy, k3;
    static real p1, p2, p3;
    static integer mm, lq;
    static real rn;
    extern doublereal fprati_();
    extern /* Subroutine */ int fpgrre_(), fpknot_();
    static integer mk1, nrintx, nrinty, kx1, kx2, ky1, ky2;
    static real acc, one;
    static integer lax, lay, lbx, lby, lri, mpm, nxe, nye, nxk, lsx, lsy, 
	    ich1, ich3;
    static real con1, con4, con9;
    static integer npl1, nk1x, nk1y;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpgrre,fpknot */
/*  .. */
/*   set constants */
    /* Parameter adjustments */
    --nrx;
    --x;
    --nry;
    --y;
    --z__;
    --nrdatx;
    --fpintx;
    --tx;
    --nrdaty;
    --fpinty;
    --ty;
    --c__;
    --wrk;

    /* Function Body */
    one = (float)1.;
    half = (float).5;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
/*  we partition the working space. */
    kx1 = *kx + 1;
    ky1 = *ky + 1;
    kx2 = kx1 + 1;
    ky2 = ky1 + 1;
    lsx = 1;
    lsy = lsx + *mx * kx1;
    lri = lsy + *my * ky1;
    mm = max(*nxest,*my);
    lq = lri + mm;
    mynx = *nxest * *my;
    lax = lq + mynx;
    nxk = *nxest * kx2;
    lbx = lax + nxk;
    lay = lbx + nxk;
    lby = lay + *nyest * ky2;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 1: determination of the number of knots and their position.     c */
/* ****************************************************************     c */
/*  given a set of knots we compute the least-squares spline sinf(x,y), c */
/*  and the corresponding sum of squared residuals fp=f(p=inf).         c */
/*  if iopt=-1  sinf(x,y) is the requested approximation.               c */
/*  if iopt=0 or iopt=1 we check whether we can accept the knots:       c */
/*    if fp <=s we will continue with the current set of knots.         c */
/*    if fp > s we will increase the number of knots and compute the    c */
/*       corresponding least-squares spline until finally fp<=s.        c */
/*    the initial choice of knots depends on the value of s and iopt.   c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*    knots equals nmaxx = mx+kx+1  and  nmaxy = my+ky+1.               c */
/*    if s>0 and                                                        c */
/*     *iopt=0 we first compute the least-squares polynomial of degree  c */
/*      kx in x and ky in y; nx=nminx=2*kx+2 and ny=nymin=2*ky+2.       c */
/*     *iopt=1 we start with the knots found at the last call of the    c */
/*      routine, except for the case that s > fp0; then we can compute  c */
/*      the least-squares polynomial directly.                          c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  determine the number of knots for polynomial approximation. */
    nminx = kx1 << 1;
    nminy = ky1 << 1;
    if (*iopt < 0) {
	goto L120;
    }
/*  acc denotes the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  find nmaxx and nmaxy which denote the number of knots in x- and y- */
/*  direction in case of spline interpolation. */
    nmaxx = *mx + kx1;
    nmaxy = *my + ky1;
/*  find nxe and nye which denote the maximum number of knots */
/*  allowed in each direction */
    nxe = min(nmaxx,*nxest);
    nye = min(nmaxy,*nyest);
    if (*s > (float)0.) {
	goto L100;
    }
/*  if s = 0, s(x,y) is an interpolating spline. */
    *nx = nmaxx;
    *ny = nmaxy;
/*  test whether the required storage space exceeds the available one. */
    if (*ny > *nyest || *nx > *nxest) {
	goto L420;
    }
/*  find the position of the interior knots in case of interpolation. */
/*  the knots in the x-direction. */
    mk1 = *mx - kx1;
    if (mk1 == 0) {
	goto L60;
    }
    k3 = *kx / 2;
    i__ = kx1 + 1;
    j = k3 + 2;
    if (k3 << 1 == *kx) {
	goto L40;
    }
    i__1 = mk1;
    for (l = 1; l <= i__1; ++l) {
	tx[i__] = x[j];
	++i__;
	++j;
/* L30: */
    }
    goto L60;
L40:
    i__1 = mk1;
    for (l = 1; l <= i__1; ++l) {
	tx[i__] = (x[j] + x[j - 1]) * half;
	++i__;
	++j;
/* L50: */
    }
/*  the knots in the y-direction. */
L60:
    mk1 = *my - ky1;
    if (mk1 == 0) {
	goto L120;
    }
    k3 = *ky / 2;
    i__ = ky1 + 1;
    j = k3 + 2;
    if (k3 << 1 == *ky) {
	goto L80;
    }
    i__1 = mk1;
    for (l = 1; l <= i__1; ++l) {
	ty[i__] = y[j];
	++i__;
	++j;
/* L70: */
    }
    goto L120;
L80:
    i__1 = mk1;
    for (l = 1; l <= i__1; ++l) {
	ty[i__] = (y[j] + y[j - 1]) * half;
	++i__;
	++j;
/* L90: */
    }
    goto L120;
/*  if s > 0 our initial choice of knots depends on the value of iopt. */
L100:
    if (*iopt == 0) {
	goto L115;
    }
    if (*fp0 <= *s) {
	goto L115;
    }
/*  if iopt=1 and fp0 > s we start computing the least- squares spline */
/*  according to the set of knots found at the last call of the routine. */
/*  we determine the number of grid coordinates x(i) inside each knot */
/*  interval (tx(l),tx(l+1)). */
    l = kx2;
    j = 1;
    nrdatx[1] = 0;
    mpm = *mx - 1;
    i__1 = mpm;
    for (i__ = 2; i__ <= i__1; ++i__) {
	++nrdatx[j];
	if (x[i__] < tx[l]) {
	    goto L105;
	}
	--nrdatx[j];
	++l;
	++j;
	nrdatx[j] = 0;
L105:
	;
    }
/*  we determine the number of grid coordinates y(i) inside each knot */
/*  interval (ty(l),ty(l+1)). */
    l = ky2;
    j = 1;
    nrdaty[1] = 0;
    mpm = *my - 1;
    i__1 = mpm;
    for (i__ = 2; i__ <= i__1; ++i__) {
	++nrdaty[j];
	if (y[i__] < ty[l]) {
	    goto L110;
	}
	--nrdaty[j];
	++l;
	++j;
	nrdaty[j] = 0;
L110:
	;
    }
    goto L120;
/*  if iopt=0 or iopt=1 and s>=fp0, we start computing the least-squares */
/*  polynomial of degree kx in x and ky in y (which is a spline without */
/*  interior knots). */
L115:
    *nx = nminx;
    *ny = nminy;
    nrdatx[1] = *mx - 2;
    nrdaty[1] = *my - 2;
    *lastdi = 0;
    *nplusx = 0;
    *nplusy = 0;
    *fp0 = (float)0.;
    *fpold = (float)0.;
    *reducx = (float)0.;
    *reducy = (float)0.;
L120:
    mpm = *mx + *my;
    ifsx = 0;
    ifsy = 0;
    ifbx = 0;
    ifby = 0;
    p = -one;
/*  main loop for the different sets of knots.mpm=mx+my is a save upper */
/*  bound for the number of trials. */
    i__1 = mpm;
    for (iter = 1; iter <= i__1; ++iter) {
	if (*nx == nminx && *ny == nminy) {
	    *ier = -2;
	}
/*  find nrintx (nrinty) which is the number of knot intervals in the */
/*  x-direction (y-direction). */
	nrintx = *nx - nminx + 1;
	nrinty = *ny - nminy + 1;
/*  find ncof, the number of b-spline coefficients for the current set */
/*  of knots. */
	nk1x = *nx - kx1;
	nk1y = *ny - ky1;
	ncof = nk1x * nk1y;
/*  find the position of the additional knots which are needed for the */
/*  b-spline representation of s(x,y). */
	i__ = *nx;
	i__2 = kx1;
	for (j = 1; j <= i__2; ++j) {
	    tx[j] = *xb;
	    tx[i__] = *xe;
	    --i__;
/* L130: */
	}
	i__ = *ny;
	i__2 = ky1;
	for (j = 1; j <= i__2; ++j) {
	    ty[j] = *yb;
	    ty[i__] = *ye;
	    --i__;
/* L140: */
	}
/*  find the least-squares spline sinf(x,y) and calculate for each knot */
/*  interval tx(j+kx)<=x<=tx(j+kx+1) (ty(j+ky)<=y<=ty(j+ky+1)) the sum */
/*  of squared residuals fpintx(j),j=1,2,...,nx-2*kx-1 (fpinty(j),j=1,2, */
/*  ...,ny-2*ky-1) for the data points having their absciss (ordinate)- */
/*  value belonging to that interval. */
/*  fp gives the total sum of squared residuals. */
	fpgrre_(&ifsx, &ifsy, &ifbx, &ifby, &x[1], mx, &y[1], my, &z__[1], mz,
		 kx, ky, &tx[1], nx, &ty[1], ny, &p, &c__[1], nc, fp, &fpintx[
		1], &fpinty[1], &mm, &mynx, &kx1, &kx2, &ky1, &ky2, &wrk[lsx],
		 &wrk[lsy], &wrk[lri], &wrk[lq], &wrk[lax], &wrk[lay], &wrk[
		lbx], &wrk[lby], &nrx[1], &nry[1]);
	if (*ier == -2) {
	    *fp0 = *fp;
	}
/*  test whether the least-squares spline is an acceptable solution. */
	if (*iopt < 0) {
	    goto L440;
	}
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  if f(p=inf) < s, we accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L300;
	}
/*  if nx=nmaxx and ny=nmaxy, sinf(x,y) is an interpolating spline. */
	if (*nx == nmaxx && *ny == nmaxy) {
	    goto L430;
	}
/*  increase the number of knots. */
/*  if nx=nxe and ny=nye we cannot further increase the number of knots */
/*  because of the storage capacity limitation. */
	if (*nx == nxe && *ny == nye) {
	    goto L420;
	}
	*ier = 0;
/*  adjust the parameter reducx or reducy according to the direction */
/*  in which the last added knots were located. */
	if (*lastdi < 0) {
	    goto L150;
	} else if (*lastdi == 0) {
	    goto L170;
	} else {
	    goto L160;
	}
L150:
	*reducx = *fpold - *fp;
	goto L170;
L160:
	*reducy = *fpold - *fp;
/*  store the sum of squared residuals for the current set of knots. */
L170:
	*fpold = *fp;
/*  find nplx, the number of knots we should add in the x-direction. */
	nplx = 1;
	if (*nx == nminx) {
	    goto L180;
	}
	npl1 = *nplusx << 1;
	rn = (real) (*nplusx);
	if (*reducx > acc) {
	    npl1 = rn * fpms / *reducx;
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = *nplusx / 2, i__4 = max(i__4,i__5);
	i__2 = *nplusx << 1, i__3 = max(i__4,1);
	nplx = min(i__2,i__3);
/*  find nply, the number of knots we should add in the y-direction. */
L180:
	nply = 1;
	if (*ny == nminy) {
	    goto L190;
	}
	npl1 = *nplusy << 1;
	rn = (real) (*nplusy);
	if (*reducy > acc) {
	    npl1 = rn * fpms / *reducy;
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = *nplusy / 2, i__4 = max(i__4,i__5);
	i__2 = *nplusy << 1, i__3 = max(i__4,1);
	nply = min(i__2,i__3);
L190:
	if ((i__2 = nplx - nply) < 0) {
	    goto L210;
	} else if (i__2 == 0) {
	    goto L200;
	} else {
	    goto L230;
	}
L200:
	if (*lastdi < 0) {
	    goto L230;
	}
L210:
	if (*nx == nxe) {
	    goto L230;
	}
/*  addition in the x-direction. */
	*lastdi = -1;
	*nplusx = nplx;
	ifsx = 0;
	i__2 = *nplusx;
	for (l = 1; l <= i__2; ++l) {
/*  add a new knot in the x-direction */
	    fpknot_(&x[1], mx, &tx[1], nx, &fpintx[1], &nrdatx[1], &nrintx, 
		    nxest, &c__1);
/*  test whether we cannot further increase the number of knots in the */
/*  x-direction. */
	    if (*nx == nxe) {
		goto L250;
	    }
/* L220: */
	}
	goto L250;
L230:
	if (*ny == nye) {
	    goto L210;
	}
/*  addition in the y-direction. */
	*lastdi = 1;
	*nplusy = nply;
	ifsy = 0;
	i__2 = *nplusy;
	for (l = 1; l <= i__2; ++l) {
/*  add a new knot in the y-direction. */
	    fpknot_(&y[1], my, &ty[1], ny, &fpinty[1], &nrdaty[1], &nrinty, 
		    nyest, &c__1);
/*  test whether we cannot further increase the number of knots in the */
/*  y-direction. */
	    if (*ny == nye) {
		goto L250;
	    }
/* L240: */
	}
/*  restart the computations with the new set of knots. */
L250:
	;
    }
/*  test whether the least-squares polynomial is a solution of our */
/*  approximation problem. */
L300:
    if (*ier == -2) {
	goto L440;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 2: determination of the smoothing spline sp(x,y)                c */
/* *****************************************************                c */
/*  we have determined the number of knots and their position. we now   c */
/*  compute the b-spline coefficients of the smoothing spline sp(x,y).  c */
/*  this smoothing spline varies with the parameter p in such a way thatc */
/*    f(p) = sumi=1,mx(sumj=1,my((z(i,j)-sp(x(i),y(j)))**2)             c */
/*  is a continuous, strictly decreasing function of p. moreover the    c */
/*  least-squares polynomial corresponds to p=0 and the least-squares   c */
/*  spline to p=infinity. iteratively we then have to determine the     c */
/*  positive value of p such that f(p)=s. the process which is proposed c */
/*  here makes use of rational interpolation. f(p) is approximated by a c */
/*  rational function r(p)=(u*p+v)/(p+w); three values of p (p1,p2,p3)  c */
/*  with corresponding values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s)c */
/*  are used to calculate the new value of p such that r(p)=s.          c */
/*  convergence is guaranteed by taking f1 > 0 and f3 < 0.              c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  initial value for p. */
    p1 = (float)0.;
    f1 = *fp0 - *s;
    p3 = -one;
    f3 = fpms;
    p = one;
    ich1 = 0;
    ich3 = 0;
/*  iteration process to find the root of f(p)=s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find the smoothing spline sp(x,y) and the corresponding sum of */
/*  squared residuals fp. */
	fpgrre_(&ifsx, &ifsy, &ifbx, &ifby, &x[1], mx, &y[1], my, &z__[1], mz,
		 kx, ky, &tx[1], nx, &ty[1], ny, &p, &c__[1], nc, fp, &fpintx[
		1], &fpinty[1], &mm, &mynx, &kx1, &kx2, &ky1, &ky2, &wrk[lsx],
		 &wrk[lsy], &wrk[lri], &wrk[lq], &wrk[lax], &wrk[lay], &wrk[
		lbx], &wrk[lby], &nrx[1], &nry[1]);
/*  test whether the approximation sp(x,y) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  test whether the maximum allowable number of iterations has been */
/*  reached. */
	if (iter == *maxit) {
	    goto L400;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L320;
	}
	if (f2 - f3 > acc) {
	    goto L310;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L350;
L310:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L320:
	if (ich1 != 0) {
	    goto L340;
	}
	if (f1 - f2 > acc) {
	    goto L330;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L350;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L350;
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L330:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
L340:
	if (f2 >= f1 || f2 <= f3) {
	    goto L410;
	}
/*  find the new value of p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L350:
	;
    }
/*  error codes and messages. */
L400:
    *ier = 3;
    goto L440;
L410:
    *ier = 2;
    goto L440;
L420:
    *ier = 1;
    goto L440;
L430:
    *ier = -1;
    *fp = (float)0.;
L440:
    return 0;
} /* fpregr_ */

/* Subroutine */ int fprota_(cos__, sin__, a, b)
real *cos__, *sin__, *a, *b;
{
    static real stor1, stor2;

/*  subroutine fprota applies a givens rotation to a and b. */
/*  .. */
/*  ..scalar arguments.. */
/* ..local scalars.. */
/*  .. */
    stor1 = *a;
    stor2 = *b;
    *b = *cos__ * stor2 + *sin__ * stor1;
    *a = *cos__ * stor1 - *sin__ * stor2;
    return 0;
} /* fprota_ */

/* Subroutine */ int fprppo_(nu, nv, if1, if2, cosi, ratio, c__, f, ncoff)
integer *nu, *nv, *if1, *if2;
real *cosi, *ratio, *c__, *f;
integer *ncoff;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer iopt, i__, j, k, l, ii, nu4, nvv;

/*  given the coefficients of a constrained bicubic spline, as determined */
/*  in subroutine fppola, subroutine fprppo calculates the coefficients */
/*  in the standard b-spline representation of bicubic splines. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    cosi -= 6;
    --f;
    --c__;

    /* Function Body */
    nu4 = *nu - 4;
    nvv = *nv - 7;
    iopt = *if1 + 1;
    i__1 = *ncoff;
    for (i__ = 1; i__ <= i__1; ++i__) {
	f[i__] = (float)0.;
/* L10: */
    }
    i__ = 0;
    i__1 = nu4;
    for (l = 1; l <= i__1; ++l) {
	ii = i__;
	if (l > iopt) {
	    goto L80;
	}
	switch ((int)l) {
	    case 1:  goto L20;
	    case 2:  goto L40;
	    case 3:  goto L60;
	}
L20:
	i__2 = nvv;
	for (k = 1; k <= i__2; ++k) {
	    ++i__;
	    f[i__] = c__[1];
/* L30: */
	}
	j = 1;
	goto L100;
L40:
	i__2 = nvv;
	for (k = 1; k <= i__2; ++k) {
	    ++i__;
	    f[i__] = c__[1] + c__[2] * cosi[k * 5 + 1] + c__[3] * cosi[k * 5 
		    + 2];
/* L50: */
	}
	j = 3;
	goto L100;
L60:
	i__2 = nvv;
	for (k = 1; k <= i__2; ++k) {
	    ++i__;
	    f[i__] = c__[1] + *ratio * (c__[2] * cosi[k * 5 + 1] + c__[3] * 
		    cosi[k * 5 + 2]) + c__[4] * cosi[k * 5 + 3] + c__[5] * 
		    cosi[k * 5 + 4] + c__[6] * cosi[k * 5 + 5];
/* L70: */
	}
	j = 6;
	goto L100;
L80:
	if (l == nu4 && *if2 != 0) {
	    goto L120;
	}
	i__2 = nvv;
	for (k = 1; k <= i__2; ++k) {
	    ++i__;
	    ++j;
	    f[i__] = c__[j];
/* L90: */
	}
L100:
	for (k = 1; k <= 3; ++k) {
	    ++ii;
	    ++i__;
	    f[i__] = f[ii];
/* L110: */
	}
L120:
	;
    }
    i__1 = *ncoff;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = f[i__];
/* L130: */
    }
    return 0;
} /* fprppo_ */

/* Subroutine */ int fprpsp_(nt, np, co, si, c__, f, ncoff)
integer *nt, *np;
real *co, *si, *c__, *f;
integer *ncoff;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer ncof, i__, j, k, l;
    static real c1, c2, c3, cn;
    static integer ii, np4, nt4, npp;

/*  given the coefficients of a spherical spline function, subroutine */
/*  fprpsp calculates the coefficients in the standard b-spline re- */
/*  presentation of this bicubic spline. */
/*  .. */
/*  ..scalar arguments */
/*  ..array arguments */
/*  ..local scalars */
/*  .. */
    /* Parameter adjustments */
    --si;
    --co;
    --f;
    --c__;

    /* Function Body */
    nt4 = *nt - 4;
    np4 = *np - 4;
    npp = np4 - 3;
    ncof = npp * (nt4 - 4) + 6;
    c1 = c__[1];
    cn = c__[ncof];
    j = *ncoff;
    i__1 = np4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	f[i__] = c1;
	f[j] = cn;
	--j;
/* L10: */
    }
    i__ = np4;
    j = 1;
    i__1 = nt4;
    for (l = 3; l <= i__1; ++l) {
	ii = i__;
	if (l == 3 || l == nt4) {
	    goto L30;
	}
	i__2 = npp;
	for (k = 1; k <= i__2; ++k) {
	    ++i__;
	    ++j;
	    f[i__] = c__[j];
/* L20: */
	}
	goto L50;
L30:
	if (l == nt4) {
	    c1 = cn;
	}
	c2 = c__[j + 1];
	c3 = c__[j + 2];
	j += 2;
	i__2 = npp;
	for (k = 1; k <= i__2; ++k) {
	    ++i__;
	    f[i__] = c1 + c2 * co[k] + c3 * si[k];
/* L40: */
	}
L50:
	for (k = 1; k <= 3; ++k) {
	    ++ii;
	    ++i__;
	    f[i__] = f[ii];
/* L60: */
	}
/* L70: */
    }
    i__1 = *ncoff;
    for (i__ = 1; i__ <= i__1; ++i__) {
	c__[i__] = f[i__];
/* L80: */
    }
    return 0;
} /* fprpsp_ */

/* Subroutine */ int fpseno_(maxtr, up, left, right, info, merk, ibind, nbind)
integer *maxtr, *up, *left, *right, *info, *merk, *ibind, *nbind;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k;

/*  subroutine fpseno fetches a branch of a triply linked tree the */
/*  information of which is kept in the arrays up,left,right and info. */
/*  the branch has a specified length nbind and is determined by the */
/*  parameter merk which points to its terminal node. the information */
/*  field of the nodes of this branch is stored in the array ibind. on */
/*  exit merk points to a new branch of length nbind or takes the value */
/*  1 if no such branch was found. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..scalar arguments.. */
/*  .. */
    /* Parameter adjustments */
    --info;
    --right;
    --left;
    --up;
    --ibind;

    /* Function Body */
    k = *merk;
    j = *nbind;
    i__1 = *nbind;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ibind[j] = info[k];
	k = up[k];
	--j;
/* L10: */
    }
L20:
    k = right[*merk];
    if (k != 0) {
	goto L30;
    }
    *merk = up[*merk];
    if (*merk - 1 <= 0) {
	goto L40;
    } else {
	goto L20;
    }
L30:
    *merk = k;
    k = left[*merk];
    if (k != 0) {
	goto L30;
    }
L40:
    return 0;
} /* fpseno_ */

/* Subroutine */ int fpspgr_(iopt, ider, u, mu, v, mv, r__, mr, r0, r1, s, 
	nuest, nvest, tol, maxit, nc, nu, tu, nv, tv, c__, fp, fp0, fpold, 
	reducu, reducv, fpintu, fpintv, dr, step, lastdi, nplusu, nplusv, 
	lastu0, lastu1, nru, nrv, nrdatu, nrdatv, wrk, lwrk, ier)
integer *iopt, *ider;
real *u;
integer *mu;
real *v;
integer *mv;
real *r__;
integer *mr;
real *r0, *r1, *s;
integer *nuest, *nvest;
real *tol;
integer *maxit, *nc, *nu;
real *tu;
integer *nv;
real *tv, *c__, *fp, *fp0, *fpold, *reducu, *reducv, *fpintu, *fpintv, *dr, *
	step;
integer *lastdi, *nplusu, *nplusv, *lastu0, *lastu1, *nru, *nrv, *nrdatu, *
	nrdatv;
real *wrk;
integer *lwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    real r__1;

    /* Builtin functions */
    double atan2();

    /* Local variables */
    static integer ifbu, ifbv, iter;
    static real fpms, rmin, rmax;
    static integer ifsu, ifsv, nplu, nplv, i__, j, l;
    static real p;
    static integer mumin;
    static real f1, f2, f3;
    static integer i1, i2, numax, l1, l2;
    static real p1, p2, p3;
    static integer l3, l4, nvmax;
    static real vb, pi, ve;
    static integer nn;
    static real rn;
    static integer ju;
    extern doublereal fprati_();
    extern /* Subroutine */ int fpknot_();
    static integer istart;
    extern /* Subroutine */ int fpopsp_();
    static integer nrintu, nrintv, mu0, mu1;
    static real acc;
    static integer idd[4];
    static real one, per;
    static integer nue, nve, mpm;
    static real drr[6];
    static integer ktu, ich1, ich3;
    static real con1, con4, con9;
    static integer npl1;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpknot,fpopsp */
/*  .. */
/*   set constants */
    /* Parameter adjustments */
    --iopt;
    --ider;
    --nru;
    --u;
    --nrv;
    --v;
    --r__;
    --nrdatu;
    --fpintu;
    --tu;
    --nrdatv;
    --fpintv;
    --tv;
    --c__;
    --dr;
    --step;
    --wrk;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
/*   initialization */
    ifsu = 0;
    ifsv = 0;
    ifbu = 0;
    ifbv = 0;
    p = -one;
    mumin = 4;
    if (ider[1] >= 0) {
	--mumin;
    }
    if (iopt[2] == 1 && ider[2] == 1) {
	--mumin;
    }
    if (ider[3] >= 0) {
	--mumin;
    }
    if (iopt[3] == 1 && ider[4] == 1) {
	--mumin;
    }
    if (mumin == 0) {
	mumin = 1;
    }
    pi = atan2((float)0., -one);
    per = pi + pi;
    vb = v[1];
    ve = vb + per;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 1: determination of the number of knots and their position.     c */
/* ****************************************************************     c */
/*  given a set of knots we compute the least-squares spline sinf(u,v)  c */
/*  and the corresponding sum of squared residuals fp = f(p=inf).       c */
/*  if iopt(1)=-1  sinf(u,v) is the requested approximation.            c */
/*  if iopt(1)>=0  we check whether we can accept the knots:            c */
/*    if fp <= s we will continue with the current set of knots.        c */
/*    if fp >  s we will increase the number of knots and compute the   c */
/*       corresponding least-squares spline until finally fp <= s.      c */
/*    the initial choice of knots depends on the value of s and iopt.   c */
/*    if s=0 we have spline interpolation; in that case the number of   c */
/*     knots in the u-direction equals nu=numax=mu+6+iopt(2)+iopt(3)    c */
/*     and in the v-direction nv=nvmax=mv+7.                            c */
/*    if s>0 and                                                        c */
/*      iopt(1)=0 we first compute the least-squares polynomial,i.e. a  c */
/*       spline without interior knots : nu=8 ; nv=8.                   c */
/*      iopt(1)=1 we start with the set of knots found at the last call c */
/*       of the routine, except for the case that s > fp0; then we      c */
/*       compute the least-squares polynomial directly.                 c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
    if (iopt[1] < 0) {
	goto L120;
    }
/*  acc denotes the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
/*  numax and nvmax denote the number of knots needed for interpolation. */
    numax = *mu + 6 + iopt[2] + iopt[3];
    nvmax = *mv + 7;
    nue = min(numax,*nuest);
    nve = min(nvmax,*nvest);
    if (*s > (float)0.) {
	goto L100;
    }
/*  if s = 0, s(u,v) is an interpolating spline. */
    *nu = numax;
    *nv = nvmax;
/*  test whether the required storage space exceeds the available one. */
    if (*nu > *nuest || *nv > *nvest) {
	goto L420;
    }
/*  find the position of the knots in the v-direction. */
    i__1 = *mv;
    for (l = 1; l <= i__1; ++l) {
	tv[l + 3] = v[l];
/* L10: */
    }
    tv[*mv + 4] = ve;
    l1 = *mv - 2;
    l2 = *mv + 5;
    for (i__ = 1; i__ <= 3; ++i__) {
	tv[i__] = v[l1] - per;
	tv[l2] = v[i__ + 1] + per;
	++l1;
	++l2;
/* L20: */
    }
/*  if not all the derivative values g(i,j) are given, we will first */
/*  estimate these values by computing a least-squares spline */
    idd[0] = ider[1];
    if (idd[0] == 0) {
	idd[0] = 1;
    }
    if (idd[0] > 0) {
	dr[1] = *r0;
    }
    idd[1] = ider[2];
    idd[2] = ider[3];
    if (idd[2] == 0) {
	idd[2] = 1;
    }
    if (idd[2] > 0) {
	dr[4] = *r1;
    }
    idd[3] = ider[4];
    if (ider[1] < 0 || ider[3] < 0) {
	goto L30;
    }
    if (iopt[2] != 0 && ider[2] == 0) {
	goto L30;
    }
    if (iopt[3] == 0 || ider[4] != 0) {
	goto L70;
    }
/* we set up the knots in the u-direction for computing the least-squares */
/* spline. */
L30:
    i1 = 3;
    i2 = *mu - 2;
    *nu = 4;
    i__1 = *mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (i1 > i2) {
	    goto L50;
	}
	++(*nu);
	tu[*nu] = u[i1];
	i1 += 2;
/* L40: */
    }
L50:
    for (i__ = 1; i__ <= 4; ++i__) {
	tu[i__] = (float)0.;
	++(*nu);
	tu[*nu] = pi;
/* L60: */
    }
/* we compute the least-squares spline for estimating the derivatives. */
    fpopsp_(&ifsu, &ifsv, &ifbu, &ifbv, &u[1], mu, &v[1], mv, &r__[1], mr, r0,
	     r1, &dr[1], &iopt[1], idd, &tu[1], nu, &tv[1], nv, nuest, nvest, 
	    &p, &step[1], &c__[1], nc, fp, &fpintu[1], &fpintv[1], &nru[1], &
	    nrv[1], &wrk[1], lwrk);
    ifsu = 0;
/* if all the derivatives at the origin are known, we compute the */
/* interpolating spline. */
/* we set up the knots in the u-direction, needed for interpolation. */
L70:
    nn = numax - 8;
    if (nn == 0) {
	goto L95;
    }
    ju = 2 - iopt[2];
    i__1 = nn;
    for (l = 1; l <= i__1; ++l) {
	tu[l + 4] = u[ju];
	++ju;
/* L80: */
    }
    *nu = numax;
    l = *nu;
    for (i__ = 1; i__ <= 4; ++i__) {
	tu[i__] = (float)0.;
	tu[l] = pi;
	--l;
/* L90: */
    }
/* we compute the interpolating spline. */
L95:
    fpopsp_(&ifsu, &ifsv, &ifbu, &ifbv, &u[1], mu, &v[1], mv, &r__[1], mr, r0,
	     r1, &dr[1], &iopt[1], idd, &tu[1], nu, &tv[1], nv, nuest, nvest, 
	    &p, &step[1], &c__[1], nc, fp, &fpintu[1], &fpintv[1], &nru[1], &
	    nrv[1], &wrk[1], lwrk);
    goto L430;
/*  if s>0 our initial choice of knots depends on the value of iopt(1). */
L100:
    *ier = 0;
    if (iopt[1] == 0) {
	goto L115;
    }
    step[1] = -step[1];
    step[2] = -step[2];
    if (*fp0 <= *s) {
	goto L115;
    }
/*  if iopt(1)=1 and fp0 > s we start computing the least-squares spline */
/*  according to the set of knots found at the last call of the routine. */
/*  we determine the number of grid coordinates u(i) inside each knot */
/*  interval (tu(l),tu(l+1)). */
    l = 5;
    j = 1;
    nrdatu[1] = 0;
    mu0 = 2 - iopt[2];
    mu1 = *mu - 1 + iopt[3];
    i__1 = mu1;
    for (i__ = mu0; i__ <= i__1; ++i__) {
	++nrdatu[j];
	if (u[i__] < tu[l]) {
	    goto L105;
	}
	--nrdatu[j];
	++l;
	++j;
	nrdatu[j] = 0;
L105:
	;
    }
/*  we determine the number of grid coordinates v(i) inside each knot */
/*  interval (tv(l),tv(l+1)). */
    l = 5;
    j = 1;
    nrdatv[1] = 0;
    i__1 = *mv;
    for (i__ = 2; i__ <= i__1; ++i__) {
	++nrdatv[j];
	if (v[i__] < tv[l]) {
	    goto L110;
	}
	--nrdatv[j];
	++l;
	++j;
	nrdatv[j] = 0;
L110:
	;
    }
    idd[0] = ider[1];
    idd[1] = ider[2];
    idd[2] = ider[3];
    idd[3] = ider[4];
    goto L120;
/*  if iopt(1)=0 or iopt(1)=1 and s >= fp0,we start computing the least- */
/*  squares polynomial (which is a spline without interior knots). */
L115:
    *ier = -2;
    idd[0] = ider[1];
    idd[1] = 1;
    idd[2] = ider[3];
    idd[3] = 1;
    *nu = 8;
    *nv = 8;
    nrdatu[1] = *mu - 2 + iopt[2] + iopt[3];
    nrdatv[1] = *mv - 1;
    *lastdi = 0;
    *nplusu = 0;
    *nplusv = 0;
    *fp0 = (float)0.;
    *fpold = (float)0.;
    *reducu = (float)0.;
    *reducv = (float)0.;
/*  main loop for the different sets of knots.mpm=mu+mv is a save upper */
/*  bound for the number of trials. */
L120:
    mpm = *mu + *mv;
    i__1 = mpm;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find nrintu (nrintv) which is the number of knot intervals in the */
/*  u-direction (v-direction). */
	nrintu = *nu - 7;
	nrintv = *nv - 7;
/*  find the position of the additional knots which are needed for the */
/*  b-spline representation of s(u,v). */
	i__ = *nu;
	for (j = 1; j <= 4; ++j) {
	    tu[j] = (float)0.;
	    tu[i__] = pi;
	    --i__;
/* L125: */
	}
	l1 = 4;
	l2 = l1;
	l3 = *nv - 3;
	l4 = l3;
	tv[l2] = vb;
	tv[l3] = ve;
	for (j = 1; j <= 3; ++j) {
	    ++l1;
	    --l2;
	    ++l3;
	    --l4;
	    tv[l2] = tv[l4] - per;
	    tv[l3] = tv[l1] + per;
/* L130: */
	}
/*  find an estimate of the range of possible values for the optimal */
/*  derivatives at the origin. */
	ktu = nrdatu[1] + 2 - iopt[2];
	if (ktu < mumin) {
	    ktu = mumin;
	}
	if (ktu == *lastu0) {
	    goto L140;
	}
	rmin = *r0;
	rmax = *r0;
	l = *mv * ktu;
	i__2 = l;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (r__[i__] < rmin) {
		rmin = r__[i__];
	    }
	    if (r__[i__] > rmax) {
		rmax = r__[i__];
	    }
/* L135: */
	}
	step[1] = rmax - rmin;
	*lastu0 = ktu;
L140:
	ktu = nrdatu[nrintu] + 2 - iopt[3];
	if (ktu < mumin) {
	    ktu = mumin;
	}
	if (ktu == *lastu1) {
	    goto L150;
	}
	rmin = *r1;
	rmax = *r1;
	l = *mv * ktu;
	j = *mr;
	i__2 = l;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (r__[j] < rmin) {
		rmin = r__[j];
	    }
	    if (r__[j] > rmax) {
		rmax = r__[j];
	    }
	    --j;
/* L145: */
	}
	step[2] = rmax - rmin;
	*lastu1 = ktu;
/*  find the least-squares spline sinf(u,v). */
L150:
	fpopsp_(&ifsu, &ifsv, &ifbu, &ifbv, &u[1], mu, &v[1], mv, &r__[1], mr,
		 r0, r1, &dr[1], &iopt[1], idd, &tu[1], nu, &tv[1], nv, nuest,
		 nvest, &p, &step[1], &c__[1], nc, fp, &fpintu[1], &fpintv[1],
		 &nru[1], &nrv[1], &wrk[1], lwrk);
	if (step[1] < (float)0.) {
	    step[1] = -step[1];
	}
	if (step[2] < (float)0.) {
	    step[2] = -step[2];
	}
	if (*ier == -2) {
	    *fp0 = *fp;
	}
/*  test whether the least-squares spline is an acceptable solution. */
	if (iopt[1] < 0) {
	    goto L440;
	}
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  if f(p=inf) < s, we accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L300;
	}
/*  if nu=numax and nv=nvmax, sinf(u,v) is an interpolating spline */
	if (*nu == numax && *nv == nvmax) {
	    goto L430;
	}
/*  increase the number of knots. */
/*  if nu=nue and nv=nve we cannot further increase the number of knots */
/*  because of the storage capacity limitation. */
	if (*nu == nue && *nv == nve) {
	    goto L420;
	}
	if (ider[1] == 0) {
/* Computing 2nd power */
	    r__1 = *r0 - dr[1];
	    fpintu[1] += r__1 * r__1;
	}
	if (ider[3] == 0) {
/* Computing 2nd power */
	    r__1 = *r1 - dr[4];
	    fpintu[nrintu] += r__1 * r__1;
	}
	*ier = 0;
/*  adjust the parameter reducu or reducv according to the direction */
/*  in which the last added knots were located. */
	if (*lastdi < 0) {
	    goto L160;
	} else if (*lastdi == 0) {
	    goto L155;
	} else {
	    goto L170;
	}
L155:
	nplv = 3;
	idd[1] = ider[2];
	idd[3] = ider[4];
	*fpold = *fp;
	goto L230;
L160:
	*reducu = *fpold - *fp;
	goto L175;
L170:
	*reducv = *fpold - *fp;
/*  store the sum of squared residuals for the current set of knots. */
L175:
	*fpold = *fp;
/*  find nplu, the number of knots we should add in the u-direction. */
	nplu = 1;
	if (*nu == 8) {
	    goto L180;
	}
	npl1 = *nplusu << 1;
	rn = (real) (*nplusu);
	if (*reducu > acc) {
	    npl1 = rn * fpms / *reducu;
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = *nplusu / 2, i__4 = max(i__4,i__5);
	i__2 = *nplusu << 1, i__3 = max(i__4,1);
	nplu = min(i__2,i__3);
/*  find nplv, the number of knots we should add in the v-direction. */
L180:
	nplv = 3;
	if (*nv == 8) {
	    goto L190;
	}
	npl1 = *nplusv << 1;
	rn = (real) (*nplusv);
	if (*reducv > acc) {
	    npl1 = rn * fpms / *reducv;
	}
/* Computing MIN */
/* Computing MAX */
	i__4 = npl1, i__5 = *nplusv / 2, i__4 = max(i__4,i__5);
	i__2 = *nplusv << 1, i__3 = max(i__4,1);
	nplv = min(i__2,i__3);
/*  test whether we are going to add knots in the u- or v-direction. */
L190:
	if ((i__2 = nplu - nplv) < 0) {
	    goto L210;
	} else if (i__2 == 0) {
	    goto L200;
	} else {
	    goto L230;
	}
L200:
	if (*lastdi < 0) {
	    goto L230;
	}
L210:
	if (*nu == nue) {
	    goto L230;
	}
/*  addition in the u-direction. */
	*lastdi = -1;
	*nplusu = nplu;
	ifsu = 0;
	istart = 0;
	if (iopt[2] == 0) {
	    istart = 1;
	}
	i__2 = *nplusu;
	for (l = 1; l <= i__2; ++l) {
/*  add a new knot in the u-direction */
	    fpknot_(&u[1], mu, &tu[1], nu, &fpintu[1], &nrdatu[1], &nrintu, 
		    nuest, &istart);
/*  test whether we cannot further increase the number of knots in the */
/*  u-direction. */
	    if (*nu == nue) {
		goto L270;
	    }
/* L220: */
	}
	goto L270;
L230:
	if (*nv == nve) {
	    goto L210;
	}
/*  addition in the v-direction. */
	*lastdi = 1;
	*nplusv = nplv;
	ifsv = 0;
	i__2 = *nplusv;
	for (l = 1; l <= i__2; ++l) {
/*  add a new knot in the v-direction. */
	    fpknot_(&v[1], mv, &tv[1], nv, &fpintv[1], &nrdatv[1], &nrintv, 
		    nvest, &c__1);
/*  test whether we cannot further increase the number of knots in the */
/*  v-direction. */
	    if (*nv == nve) {
		goto L270;
	    }
/* L240: */
	}
/*  restart the computations with the new set of knots. */
L270:
	;
    }
/*  test whether the least-squares polynomial is a solution of our */
/*  approximation problem. */
L300:
    if (*ier == -2) {
	goto L440;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 2: determination of the smoothing spline sp(u,v)                c */
/* *****************************************************                c */
/*  we have determined the number of knots and their position. we now   c */
/*  compute the b-spline coefficients of the smoothing spline sp(u,v).  c */
/*  this smoothing spline depends on the parameter p in such a way that c */
/*    f(p) = sumi=1,mu(sumj=1,mv((z(i,j)-sp(u(i),v(j)))**2)             c */
/*  is a continuous, strictly decreasing function of p. moreover the    c */
/*  least-squares polynomial corresponds to p=0 and the least-squares   c */
/*  spline to p=infinity. then iteratively we have to determine the     c */
/*  positive value of p such that f(p)=s. the process which is proposed c */
/*  here makes use of rational interpolation. f(p) is approximated by a c */
/*  rational function r(p)=(u*p+v)/(p+w); three values of p (p1,p2,p3)  c */
/*  with corresponding values of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s)c */
/*  are used to calculate the new value of p such that r(p)=s.          c */
/*  convergence is guaranteed by taking f1 > 0 and f3 < 0.              c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  initial value for p. */
    p1 = (float)0.;
    f1 = *fp0 - *s;
    p3 = -one;
    f3 = fpms;
    p = one;
    for (i__ = 1; i__ <= 6; ++i__) {
	drr[i__ - 1] = dr[i__];
/* L305: */
    }
    ich1 = 0;
    ich3 = 0;
/*  iteration process to find the root of f(p)=s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find the smoothing spline sp(u,v) and the corresponding sum f(p). */
	fpopsp_(&ifsu, &ifsv, &ifbu, &ifbv, &u[1], mu, &v[1], mv, &r__[1], mr,
		 r0, r1, drr, &iopt[1], idd, &tu[1], nu, &tv[1], nv, nuest, 
		nvest, &p, &step[1], &c__[1], nc, fp, &fpintu[1], &fpintv[1], 
		&nru[1], &nrv[1], &wrk[1], lwrk);
/*  test whether the approximation sp(u,v) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) < acc) {
	    goto L440;
	}
/*  test whether the maximum allowable number of iterations has been */
/*  reached. */
	if (iter == *maxit) {
	    goto L400;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L320;
	}
	if (f2 - f3 > acc) {
	    goto L310;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L350;
L310:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L320:
	if (ich1 != 0) {
	    goto L340;
	}
	if (f1 - f2 > acc) {
	    goto L330;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L350;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L350;
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L330:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
L340:
	if (f2 >= f1 || f2 <= f3) {
	    goto L410;
	}
/*  find the new value of p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L350:
	;
    }
/*  error codes and messages. */
L400:
    *ier = 3;
    goto L440;
L410:
    *ier = 2;
    goto L440;
L420:
    *ier = 1;
    goto L440;
L430:
    *ier = -1;
    *fp = (float)0.;
L440:
    return 0;
} /* fpspgr_ */

/* Subroutine */ int fpsphe_(iopt, m, teta, phi, r__, w, s, ntest, npest, eta,
	 tol, maxit, ib1, ib3, nc, ncc, intest, nrest, nt, tt, np, tp, c__, 
	fp, sup, fpint, coord, f, ff, row, coco, cosi, a, q, bt, bp, spt, spp,
	 h__, index, nummer, wrk, lwrk, ier)
integer *iopt, *m;
real *teta, *phi, *r__, *w, *s;
integer *ntest, *npest;
real *eta, *tol;
integer *maxit, *ib1, *ib3, *nc, *ncc, *intest, *nrest, *nt;
real *tt;
integer *np;
real *tp, *c__, *fp, *sup, *fpint, *coord, *f, *ff, *row, *coco, *cosi, *a, *
	q, *bt, *bp, *spt, *spp, *h__;
integer *index, *nummer;
real *wrk;
integer *lwrk, *ier;
{
    /* System generated locals */
    integer a_dim1, a_offset, q_dim1, q_offset, bt_dim1, bt_offset, bp_dim1, 
	    bp_offset, spt_dim1, spt_offset, spp_dim1, spp_offset, i__1, i__2,
	     i__3, i__4, i__5, i__6;
    real r__1;

    /* Builtin functions */
    double atan(), sqrt(), cos(), sin();

    /* Local variables */
    static real half, facs;
    static integer ncof;
    static real dmax__;
    static integer nreg, rank, iter;
    static real fpms, pinv;
    static integer irot, jrot, i__, j, l, iband;
    static real p;
    static integer ncoff;
    static real sigma, fpmax, c1, d1, d2, f1, f2, f3, store;
    static integer iband1, i1, iband3;
    static real p1, p2, p3;
    static integer iband4, i2, i3, j1, j2, lwest, l1, l2, l3, l4, nrint;
    static real aa;
    static integer la;
    static real cn, co, fn;
    static integer ii;
    static real pi;
    static integer ij;
    static real ri, si;
    static integer il, in;
    static real wi, rn;
    static integer lf;
    static real sq;
    static integer lh, ll, lp, lt;
    static real ht[4], hp[4];
    extern /* Subroutine */ int fpback_(), fpdisc_(), fporde_(), fprank_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpbspl_(), fprota_(), fpgivs_(), fprpsp_();
    static real pi2;
    static integer nr1, np4, nt4, nt6;
    static real acc, arg, one, hti, htj, ten, eps;
    static integer jlt, npp;
    static real piv;
    static integer num, nrr, ntt;
    static real fac1, fac2;
    static integer ich1, ich3;
    static real con1, con4, con9;
    static integer num1;
    static real facc;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*   fpback,fpbspl,fpgivs,fpdisc,fporde,fprank,fprota,fprpsp */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --nummer;
    spp_dim1 = *m;
    spp_offset = 1 + spp_dim1 * 1;
    spp -= spp_offset;
    spt_dim1 = *m;
    spt_offset = 1 + spt_dim1 * 1;
    spt -= spt_offset;
    --w;
    --r__;
    --phi;
    --teta;
    bt_dim1 = *ntest;
    bt_offset = 1 + bt_dim1 * 1;
    bt -= bt_offset;
    --tt;
    bp_dim1 = *npest;
    bp_offset = 1 + bp_dim1 * 1;
    bp -= bp_offset;
    --cosi;
    --coco;
    --row;
    --tp;
    --h__;
    --ff;
    --c__;
    q_dim1 = *ncc;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    a_dim1 = *ncc;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --f;
    --coord;
    --fpint;
    --index;
    --wrk;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
    half = (float).5;
    ten = (float)10.;
    pi = atan(one) * 4;
    pi2 = pi + pi;
    eps = sqrt(*eta);
    if (*iopt < 0) {
	goto L70;
    }
/*  calculation of acc, the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
    if (*iopt == 0) {
	goto L10;
    }
    if (*s < *sup) {
	if (*np - 11 >= 0) {
	    goto L70;
	} else {
	    goto L60;
	}
    }
/*  if iopt=0 we begin by computing the weighted least-squares polynomial */
/*  of the form */
/*     s(teta,phi) = c1*f1(teta) + cn*fn(teta) */
/*  where f1(teta) and fn(teta) are the cubic polynomials satisfying */
/*     f1(0) = 1, f1(pi) = f1'(0) = f1'(pi) = 0 ; fn(teta) = 1-f1(teta). */
/*  the corresponding weighted sum of squared residuals gives the upper */
/*  bound sup for the smoothing factor s. */
L10:
    *sup = (float)0.;
    d1 = (float)0.;
    d2 = (float)0.;
    c1 = (float)0.;
    cn = (float)0.;
    fac1 = pi * (one + half);
/* Computing 3rd power */
    r__1 = pi;
    fac2 = (one + one) / (r__1 * (r__1 * r__1));
    aa = (float)0.;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wi = w[i__];
	ri = r__[i__] * wi;
	arg = teta[i__];
	fn = fac2 * arg * arg * (fac1 - arg);
	f1 = (one - fn) * wi;
	fn *= wi;
	if (fn == (float)0.) {
	    goto L20;
	}
	fpgivs_(&fn, &d1, &co, &si);
	fprota_(&co, &si, &f1, &aa);
	fprota_(&co, &si, &ri, &cn);
L20:
	if (f1 == (float)0.) {
	    goto L30;
	}
	fpgivs_(&f1, &d2, &co, &si);
	fprota_(&co, &si, &ri, &c1);
L30:
	*sup += ri * ri;
/* L40: */
    }
    if (d2 != (float)0.) {
	c1 /= d2;
    }
    if (d1 != (float)0.) {
	cn = (cn - aa * c1) / d1;
    }
/*  find the b-spline representation of this least-squares polynomial */
    *nt = 8;
    *np = 8;
    for (i__ = 1; i__ <= 4; ++i__) {
	c__[i__] = c1;
	c__[i__ + 4] = c1;
	c__[i__ + 8] = cn;
	c__[i__ + 12] = cn;
	tt[i__] = (float)0.;
	tt[i__ + 4] = pi;
	tp[i__] = (float)0.;
	tp[i__ + 4] = pi2;
/* L50: */
    }
    *fp = *sup;
/*  test whether the least-squares polynomial is an acceptable solution */
    fpms = *sup - *s;
    if (fpms < acc) {
	goto L960;
    }
/*  test whether we cannot further increase the number of knots. */
L60:
    if (*npest < 11 || *ntest < 9) {
	goto L950;
    }
/*  find the initial set of interior knots of the spherical spline in */
/*  case iopt = 0. */
    *np = 11;
    tp[5] = pi * half;
    tp[6] = pi;
    tp[7] = tp[5] + pi;
    *nt = 9;
    tt[5] = tp[5];
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  part 1 : computation of least-squares spherical splines.            c */
/*  ********************************************************            c */
/*  if iopt < 0 we compute the least-squares spherical spline according c */
/*  to the given set of knots.                                          c */
/*  if iopt >=0 we compute least-squares spherical splines with increas-c */
/*  ing numbers of knots until the corresponding sum f(p=inf)<=s.       c */
/*  the initial set of knots then depends on the value of iopt:         c */
/*    if iopt=0 we start with one interior knot in the teta-direction   c */
/*              (pi/2) and three in the phi-direction (pi/2,pi,3*pi/2). c */
/*    if iopt>0 we start with the set of knots found at the last call   c */
/*              of the routine.                                         c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  main loop for the different sets of knots. m is a save upper bound */
/*  for the number of trials. */
L70:
    i__1 = *m;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find the position of the additional knots which are needed for the */
/*  b-spline representation of s(teta,phi). */
	l1 = 4;
	l2 = l1;
	l3 = *np - 3;
	l4 = l3;
	tp[l2] = (float)0.;
	tp[l3] = pi2;
	for (i__ = 1; i__ <= 3; ++i__) {
	    ++l1;
	    --l2;
	    ++l3;
	    --l4;
	    tp[l2] = tp[l4] - pi2;
	    tp[l3] = tp[l1] + pi2;
/* L80: */
	}
	l = *nt;
	for (i__ = 1; i__ <= 4; ++i__) {
	    tt[i__] = (float)0.;
	    tt[l] = pi;
	    --l;
/* L90: */
	}
/*  find nrint, the total number of knot intervals and nreg, the number */
/*  of panels in which the approximation domain is subdivided by the */
/*  intersection of knots. */
	ntt = *nt - 7;
	npp = *np - 7;
	nrr = npp / 2;
	nr1 = nrr + 1;
	nrint = ntt + npp;
	nreg = ntt * npp;
/*  arrange the data points according to the panel they belong to. */
	fporde_(&teta[1], &phi[1], m, &c__3, &c__3, &tt[1], nt, &tp[1], np, &
		nummer[1], &index[1], &nreg);
/*  find the b-spline coefficients coco and cosi of the cubic spline */
/*  approximations sc(phi) and ss(phi) for cos(phi) and sin(phi). */
	i__2 = npp;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    coco[i__] = (float)0.;
	    cosi[i__] = (float)0.;
	    i__3 = npp;
	    for (j = 1; j <= i__3; ++j) {
		a[i__ + j * a_dim1] = (float)0.;
/* L100: */
	    }
	}
/*  the coefficients coco and cosi are obtained from the conditions */
/*  sc(tp(i))=cos(tp(i)),resp. ss(tp(i))=sin(tp(i)),i=4,5,...np-4. */
	i__3 = npp;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    l2 = i__ + 3;
	    arg = tp[l2];
	    fpbspl_(&tp[1], np, &c__3, &arg, &l2, hp);
	    i__2 = npp;
	    for (j = 1; j <= i__2; ++j) {
		row[j] = (float)0.;
/* L110: */
	    }
	    ll = i__;
	    for (j = 1; j <= 3; ++j) {
		if (ll > npp) {
		    ll = 1;
		}
		row[ll] += hp[j - 1];
		++ll;
/* L120: */
	    }
	    facc = cos(arg);
	    facs = sin(arg);
	    i__2 = npp;
	    for (j = 1; j <= i__2; ++j) {
		piv = row[j];
		if (piv == (float)0.) {
		    goto L140;
		}
		fpgivs_(&piv, &a[j + a_dim1], &co, &si);
		fprota_(&co, &si, &facc, &coco[j]);
		fprota_(&co, &si, &facs, &cosi[j]);
		if (j == npp) {
		    goto L150;
		}
		j1 = j + 1;
		i2 = 1;
		i__4 = npp;
		for (l = j1; l <= i__4; ++l) {
		    ++i2;
		    fprota_(&co, &si, &row[l], &a[j + i2 * a_dim1]);
/* L130: */
		}
L140:
		;
	    }
L150:
	    ;
	}
	fpback_(&a[a_offset], &coco[1], &npp, &npp, &coco[1], ncc);
	fpback_(&a[a_offset], &cosi[1], &npp, &npp, &cosi[1], ncc);
/*  find ncof, the dimension of the spherical spline and ncoff, the */
/*  number of coefficients in the standard b-spline representation. */
	nt4 = *nt - 4;
	np4 = *np - 4;
	ncoff = nt4 * np4;
	ncof = npp * (ntt - 1) + 6;
/*  find the bandwidth of the observation matrix a. */
	iband = npp << 2;
	if (ntt == 4) {
	    iband = (npp + 1) * 3;
	}
	if (ntt < 4) {
	    iband = ncof;
	}
	iband1 = iband - 1;
/*  initialize the observation matrix a. */
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    f[i__] = (float)0.;
	    i__2 = iband;
	    for (j = 1; j <= i__2; ++j) {
		a[i__ + j * a_dim1] = (float)0.;
/* L160: */
	    }
	}
/*  initialize the sum of squared residuals. */
	*fp = (float)0.;
/*  fetch the data points in the new order. main loop for the */
/*  different panels. */
	i__2 = nreg;
	for (num = 1; num <= i__2; ++num) {
/*  fix certain constants for the current panel; jrot records the column */
/*  number of the first non-zero element in a row of the observation */
/*  matrix according to a data point of the panel. */
	    num1 = num - 1;
	    lt = num1 / npp;
	    l1 = lt + 4;
	    lp = num1 - lt * npp + 1;
	    l2 = lp + 3;
	    ++lt;
	    jrot = 0;
	    if (lt > 2) {
		jrot = (lt - 3) * npp + 3;
	    }
/*  test whether there are still data points in the current panel. */
	    in = index[num];
L170:
	    if (in == 0) {
		goto L340;
	    }
/*  fetch a new data point. */
	    wi = w[in];
	    ri = r__[in] * wi;
/*  evaluate for the teta-direction, the 4 non-zero b-splines at teta(in) */
	    fpbspl_(&tt[1], nt, &c__3, &teta[in], &l1, ht);
/*  evaluate for the phi-direction, the 4 non-zero b-splines at phi(in) */
	    fpbspl_(&tp[1], np, &c__3, &phi[in], &l2, hp);
/*  store the value of these b-splines in spt and spp resp. */
	    for (i__ = 1; i__ <= 4; ++i__) {
		spp[in + i__ * spp_dim1] = hp[i__ - 1];
		spt[in + i__ * spt_dim1] = ht[i__ - 1];
/* L180: */
	    }
/*  initialize the new row of observation matrix. */
	    i__3 = iband;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h__[i__] = (float)0.;
/* L190: */
	    }
/*  calculate the non-zero elements of the new row by making the cross */
/*  products of the non-zero b-splines in teta- and phi-direction and */
/*  by taking into account the conditions of the spherical splines. */
	    i__3 = npp;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		row[i__] = (float)0.;
/* L200: */
	    }
/*  take into account the condition (3) of the spherical splines. */
	    ll = lp;
	    for (i__ = 1; i__ <= 4; ++i__) {
		if (ll > npp) {
		    ll = 1;
		}
		row[ll] += hp[i__ - 1];
		++ll;
/* L210: */
	    }
/*  take into account the other conditions of the spherical splines. */
	    if (lt > 2 && lt < ntt - 1) {
		goto L230;
	    }
	    facc = (float)0.;
	    facs = (float)0.;
	    i__3 = npp;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		facc += row[i__] * coco[i__];
		facs += row[i__] * cosi[i__];
/* L220: */
	    }
/*  fill in the non-zero elements of the new row. */
L230:
	    j1 = 0;
	    for (j = 1; j <= 4; ++j) {
		jlt = j + lt;
		htj = ht[j - 1];
		if (jlt > 2 && jlt <= nt4) {
		    goto L240;
		}
		++j1;
		h__[j1] += htj;
		goto L280;
L240:
		if (jlt == 3 || jlt == nt4) {
		    goto L260;
		}
		i__3 = npp;
		for (i__ = 1; i__ <= i__3; ++i__) {
		    ++j1;
		    h__[j1] = row[i__] * htj;
/* L250: */
		}
		goto L280;
L260:
		if (jlt == 3) {
		    goto L270;
		}
		h__[j1 + 1] = facc * htj;
		h__[j1 + 2] = facs * htj;
		h__[j1 + 3] = htj;
		j1 += 2;
		goto L280;
L270:
		h__[1] += htj;
		h__[2] = facc * htj;
		h__[3] = facs * htj;
		j1 = 3;
L280:
		;
	    }
	    i__3 = iband;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		h__[i__] *= wi;
/* L290: */
	    }
/*  rotate the row into triangle by givens transformations. */
	    irot = jrot;
	    i__3 = iband;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		++irot;
		piv = h__[i__];
		if (piv == (float)0.) {
		    goto L310;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a[irot + a_dim1], &co, &si);
/*  apply that transformation to the right hand side. */
		fprota_(&co, &si, &ri, &f[irot]);
		if (i__ == iband) {
		    goto L320;
		}
/*  apply that transformation to the left hand side. */
		i2 = 1;
		i3 = i__ + 1;
		i__4 = iband;
		for (j = i3; j <= i__4; ++j) {
		    ++i2;
		    fprota_(&co, &si, &h__[j], &a[irot + i2 * a_dim1]);
/* L300: */
		}
L310:
		;
	    }
/*  add the contribution of the row to the sum of squares of residual */
/*  right hand sides. */
L320:
/* Computing 2nd power */
	    r__1 = ri;
	    *fp += r__1 * r__1;
/*  find the number of the next data point in the panel. */
/* L330: */
	    in = nummer[in];
	    goto L170;
L340:
	    ;
	}
/*  find dmax, the maximum value for the diagonal elements in the reduced */
/*  triangle. */
	dmax__ = (float)0.;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (a[i__ + a_dim1] <= dmax__) {
		goto L350;
	    }
	    dmax__ = a[i__ + a_dim1];
L350:
	    ;
	}
/*  check whether the observation matrix is rank deficient. */
	sigma = eps * dmax__;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (a[i__ + a_dim1] <= sigma) {
		goto L370;
	    }
/* L360: */
	}
/*  backward substitution in case of full rank. */
	fpback_(&a[a_offset], &f[1], &ncof, &iband, &c__[1], ncc);
	rank = ncof;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    q[i__ + q_dim1] = a[i__ + a_dim1] / dmax__;
/* L365: */
	}
	goto L390;
/*  in case of rank deficiency, find the minimum norm solution. */
L370:
	lwest = ncof * iband + ncof + iband;
	if (*lwrk < lwest) {
	    goto L925;
	}
	lf = 1;
	lh = lf + ncof;
	la = lh + iband;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ff[i__] = f[i__];
	    i__3 = iband;
	    for (j = 1; j <= i__3; ++j) {
		q[i__ + j * q_dim1] = a[i__ + j * a_dim1];
/* L380: */
	    }
	}
	fprank_(&q[q_offset], &ff[1], &ncof, &iband, ncc, &sigma, &c__[1], &
		sq, &rank, &wrk[la], &wrk[lf], &wrk[lh]);
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    q[i__ + q_dim1] /= dmax__;
/* L385: */
	}
/*  add to the sum of squared residuals, the contribution of reducing */
/*  the rank. */
	*fp += sq;
/*  find the coefficients in the standard b-spline representation of */
/*  the spherical spline. */
L390:
	fprpsp_(nt, np, &coco[1], &cosi[1], &c__[1], &ff[1], &ncoff);
/*  test whether the least-squares spline is an acceptable solution. */
	if (*iopt < 0) {
	    if (*fp <= (float)0.) {
		goto L970;
	    } else {
		goto L980;
	    }
	}
	fpms = *fp - *s;
	if (dabs(fpms) <= acc) {
	    if (*fp <= (float)0.) {
		goto L970;
	    } else {
		goto L980;
	    }
	}
/*  if f(p=inf) < s, accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L580;
	}
/*  test whether we cannot further increase the number of knots. */
	if (ncof > *m) {
	    goto L935;
	}
/*  search where to add a new knot. */
/*  find for each interval the sum of squared residuals fpint for the */
/*  data points having the coordinate belonging to that knot interval. */
/*  calculate also coord which is the same sum, weighted by the position */
/*  of the data points considered. */
/* L440: */
	i__3 = nrint;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    fpint[i__] = (float)0.;
	    coord[i__] = (float)0.;
/* L450: */
	}
	i__3 = nreg;
	for (num = 1; num <= i__3; ++num) {
	    num1 = num - 1;
	    lt = num1 / npp;
	    l1 = lt + 1;
	    lp = num1 - lt * npp;
	    l2 = lp + 1 + ntt;
	    jrot = lt * np4 + lp;
	    in = index[num];
L460:
	    if (in == 0) {
		goto L490;
	    }
	    store = (float)0.;
	    i1 = jrot;
	    for (i__ = 1; i__ <= 4; ++i__) {
		hti = spt[in + i__ * spt_dim1];
		j1 = i1;
		for (j = 1; j <= 4; ++j) {
		    ++j1;
		    store += hti * spp[in + j * spp_dim1] * c__[j1];
/* L470: */
		}
		i1 += np4;
/* L480: */
	    }
/* Computing 2nd power */
	    r__1 = w[in] * (r__[in] - store);
	    store = r__1 * r__1;
	    fpint[l1] += store;
	    coord[l1] += store * teta[in];
	    fpint[l2] += store;
	    coord[l2] += store * phi[in];
	    in = nummer[in];
	    goto L460;
L490:
	    ;
	}
/*  find the interval for which fpint is maximal on the condition that */
/*  there still can be added a knot. */
	l1 = 1;
	l2 = nrint;
	if (*ntest < *nt + 1) {
	    l1 = ntt + 1;
	}
	if (*npest < *np + 2) {
	    l2 = ntt;
	}
/*  test whether we cannot further increase the number of knots. */
	if (l1 > l2) {
	    goto L950;
	}
L500:
	fpmax = (float)0.;
	l = 0;
	i__3 = l2;
	for (i__ = l1; i__ <= i__3; ++i__) {
	    if (fpmax >= fpint[i__]) {
		goto L510;
	    }
	    l = i__;
	    fpmax = fpint[i__];
L510:
	    ;
	}
	if (l == 0) {
	    goto L930;
	}
/*  calculate the position of the new knot. */
	arg = coord[l] / fpint[l];
/*  test in what direction the new knot is going to be added. */
	if (l > ntt) {
	    goto L530;
	}
/*  addition in the teta-direction */
	l4 = l + 4;
	fpint[l] = (float)0.;
	fac1 = tt[l4] - arg;
	fac2 = arg - tt[l4 - 1];
	if (fac1 > ten * fac2 || fac2 > ten * fac1) {
	    goto L500;
	}
	j = *nt;
	i__3 = *nt;
	for (i__ = l4; i__ <= i__3; ++i__) {
	    tt[j + 1] = tt[j];
	    --j;
/* L520: */
	}
	tt[l4] = arg;
	++(*nt);
	goto L570;
/*  addition in the phi-direction */
L530:
	l4 = l + 4 - ntt;
	if (arg < pi) {
	    goto L540;
	}
	arg -= pi;
	l4 -= nrr;
L540:
	fpint[l] = (float)0.;
	fac1 = tp[l4] - arg;
	fac2 = arg - tp[l4 - 1];
	if (fac1 > ten * fac2 || fac2 > ten * fac1) {
	    goto L500;
	}
	ll = nrr + 4;
	j = ll;
	i__3 = ll;
	for (i__ = l4; i__ <= i__3; ++i__) {
	    tp[j + 1] = tp[j];
	    --j;
/* L550: */
	}
	tp[l4] = arg;
	*np += 2;
	++nrr;
	i__3 = ll;
	for (i__ = 5; i__ <= i__3; ++i__) {
	    j = i__ + nrr;
	    tp[j] = tp[i__] + pi;
/* L560: */
	}
/*  restart the computations with the new set of knots. */
L570:
	;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 2: determination of the smoothing spherical spline.             c */
/* ********************************************************             c */
/* we have determined the number of knots and their position. we now    c */
/* compute the coefficients of the smoothing spline sp(teta,phi).       c */
/* the observation matrix a is extended by the rows of a matrix, expres-c */
/* sing that sp(teta,phi) must be a constant function in the variable   c */
/* phi and a cubic polynomial in the variable teta. the corresponding   c */
/* weights of these additional rows are set to 1/(p). iteratively       c */
/* we than have to determine the value of p such that f(p) = sum((w(i)* c */
/* (r(i)-sp(teta(i),phi(i))))**2)  be = s.                              c */
/* we already know that the least-squares polynomial corresponds to p=0,c */
/* and that the least-squares spherical spline corresponds to p=infin.  c */
/* the iteration process makes use of rational interpolation. since f(p)c */
/* is a convex and strictly decreasing function of p, it can be approx- c */
/* imated by a rational function of the form r(p) = (u*p+v)/(p+w).      c */
/* three values of p (p1,p2,p3) with corresponding values of f(p) (f1=  c */
/* f(p1)-s,f2=f(p2)-s,f3=f(p3)-s) are used to calculate the new value   c */
/* of p such that r(p)=s. convergence is guaranteed by taking f1>0,f3<0.c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  evaluate the discontinuity jumps of the 3-th order derivative of */
/*  the b-splines at the knots tt(l),l=5,...,nt-4. */
L580:
    fpdisc_(&tt[1], nt, &c__5, &bt[bt_offset], ntest);
/*  evaluate the discontinuity jumps of the 3-th order derivative of */
/*  the b-splines at the knots tp(l),l=5,...,np-4. */
    fpdisc_(&tp[1], np, &c__5, &bp[bp_offset], npest);
/*  initial value for p. */
    p1 = (float)0.;
    f1 = *sup - *s;
    p3 = -one;
    f3 = fpms;
    p = (float)0.;
    i__1 = ncof;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p += a[i__ + a_dim1];
/* L585: */
    }
    rn = (real) ncof;
    p = rn / p;
/*  find the bandwidth of the extended observation matrix. */
    iband4 = iband + 3;
    if (ntt <= 4) {
	iband4 = ncof;
    }
    iband3 = iband4 - 1;
    ich1 = 0;
    ich3 = 0;
/*  iteration process to find the root of f(p)=s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
	pinv = one / p;
/*  store the triangularized observation matrix into q. */
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    ff[i__] = f[i__];
	    i__2 = iband4;
	    for (j = 1; j <= i__2; ++j) {
		q[i__ + j * q_dim1] = (float)0.;
/* L590: */
	    }
	    i__2 = iband;
	    for (j = 1; j <= i__2; ++j) {
		q[i__ + j * q_dim1] = a[i__ + j * a_dim1];
/* L600: */
	    }
	}
/*  extend the observation matrix with the rows of a matrix, expressing */
/*  that for teta=cst. sp(teta,phi) must be a constant function. */
	nt6 = *nt - 6;
	i__2 = np4;
	for (i__ = 5; i__ <= i__2; ++i__) {
	    ii = i__ - 4;
	    i__3 = npp;
	    for (l = 1; l <= i__3; ++l) {
		row[l] = (float)0.;
/* L610: */
	    }
	    ll = ii;
	    for (l = 1; l <= 5; ++l) {
		if (ll > npp) {
		    ll = 1;
		}
		row[ll] += bp[ii + l * bp_dim1];
		++ll;
/* L620: */
	    }
	    facc = (float)0.;
	    facs = (float)0.;
	    i__3 = npp;
	    for (l = 1; l <= i__3; ++l) {
		facc += row[l] * coco[l];
		facs += row[l] * cosi[l];
/* L630: */
	    }
	    i__3 = nt6;
	    for (j = 1; j <= i__3; ++j) {
/*  initialize the new row. */
		i__4 = iband;
		for (l = 1; l <= i__4; ++l) {
		    h__[l] = (float)0.;
/* L640: */
		}
/*  fill in the non-zero elements of the row. jrot records the column */
/*  number of the first non-zero element in the row. */
		jrot = (j - 2) * npp + 4;
		if (j > 1 && j < nt6) {
		    goto L650;
		}
		h__[1] = facc;
		h__[2] = facs;
		if (j == 1) {
		    jrot = 2;
		}
		goto L670;
L650:
		i__4 = npp;
		for (l = 1; l <= i__4; ++l) {
		    h__[l] = row[l];
/* L660: */
		}
L670:
		i__4 = iband;
		for (l = 1; l <= i__4; ++l) {
		    h__[l] *= pinv;
/* L675: */
		}
		ri = (float)0.;
/*  rotate the new row into triangle by givens transformations. */
		i__4 = ncof;
		for (irot = jrot; irot <= i__4; ++irot) {
		    piv = h__[1];
/* Computing MIN */
		    i__5 = iband1, i__6 = ncof - irot;
		    i2 = min(i__5,i__6);
		    if (piv == (float)0.) {
			if (i2 <= 0) {
			    goto L720;
			} else {
			    goto L690;
			}
		    }
/*  calculate the parameters of the givens transformation. */
		    fpgivs_(&piv, &q[irot + q_dim1], &co, &si);
/*  apply that givens transformation to the right hand side. */
		    fprota_(&co, &si, &ri, &ff[irot]);
		    if (i2 == 0) {
			goto L720;
		    }
/*  apply that givens transformation to the left hand side. */
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			l1 = l + 1;
			fprota_(&co, &si, &h__[l1], &q[irot + l1 * q_dim1]);
/* L680: */
		    }
L690:
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			h__[l] = h__[l + 1];
/* L700: */
		    }
		    h__[i2 + 1] = (float)0.;
/* L710: */
		}
L720:
		;
	    }
	}
/*  extend the observation matrix with the rows of a matrix expressing */
/*  that for phi=cst. sp(teta,phi) must be a cubic polynomial. */
	i__3 = nt4;
	for (i__ = 5; i__ <= i__3; ++i__) {
	    ii = i__ - 4;
	    i__2 = npp;
	    for (j = 1; j <= i__2; ++j) {
/*  initialize the new row */
		i__4 = iband4;
		for (l = 1; l <= i__4; ++l) {
		    h__[l] = (float)0.;
/* L730: */
		}
/*  fill in the non-zero elements of the row. jrot records the column */
/*  number of the first non-zero element in the row. */
		j1 = 1;
		for (l = 1; l <= 5; ++l) {
		    il = ii + l;
		    ij = npp;
		    if (il != 3 && il != nt4) {
			goto L750;
		    }
		    j1 = j1 + 3 - j;
		    j2 = j1 - 2;
		    ij = 0;
		    if (il != 3) {
			goto L740;
		    }
		    j1 = 1;
		    j2 = 2;
		    ij = j + 2;
L740:
		    h__[j2] = bt[ii + l * bt_dim1] * coco[j];
		    h__[j2 + 1] = bt[ii + l * bt_dim1] * cosi[j];
L750:
		    h__[j1] += bt[ii + l * bt_dim1];
		    j1 += ij;
/* L760: */
		}
		i__4 = iband4;
		for (l = 1; l <= i__4; ++l) {
		    h__[l] *= pinv;
/* L765: */
		}
		ri = (float)0.;
		jrot = 1;
		if (ii > 2) {
		    jrot = j + 3 + (ii - 3) * npp;
		}
/*  rotate the new row into triangle by givens transformations. */
		i__4 = ncof;
		for (irot = jrot; irot <= i__4; ++irot) {
		    piv = h__[1];
/* Computing MIN */
		    i__5 = iband3, i__6 = ncof - irot;
		    i2 = min(i__5,i__6);
		    if (piv == (float)0.) {
			if (i2 <= 0) {
			    goto L810;
			} else {
			    goto L780;
			}
		    }
/*  calculate the parameters of the givens transformation. */
		    fpgivs_(&piv, &q[irot + q_dim1], &co, &si);
/*  apply that givens transformation to the right hand side. */
		    fprota_(&co, &si, &ri, &ff[irot]);
		    if (i2 == 0) {
			goto L810;
		    }
/*  apply that givens transformation to the left hand side. */
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			l1 = l + 1;
			fprota_(&co, &si, &h__[l1], &q[irot + l1 * q_dim1]);
/* L770: */
		    }
L780:
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			h__[l] = h__[l + 1];
/* L790: */
		    }
		    h__[i2 + 1] = (float)0.;
/* L800: */
		}
L810:
		;
	    }
	}
/*  find dmax, the maximum value for the diagonal elements in the */
/*  reduced triangle. */
	dmax__ = (float)0.;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (q[i__ + q_dim1] <= dmax__) {
		goto L820;
	    }
	    dmax__ = q[i__ + q_dim1];
L820:
	    ;
	}
/*  check whether the matrix is rank deficient. */
	sigma = eps * dmax__;
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (q[i__ + q_dim1] <= sigma) {
		goto L840;
	    }
/* L830: */
	}
/*  backward substitution in case of full rank. */
	fpback_(&q[q_offset], &ff[1], &ncof, &iband4, &c__[1], ncc);
	rank = ncof;
	goto L845;
/*  in case of rank deficiency, find the minimum norm solution. */
L840:
	lwest = ncof * iband4 + ncof + iband4;
	if (*lwrk < lwest) {
	    goto L925;
	}
	lf = 1;
	lh = lf + ncof;
	la = lh + iband4;
	fprank_(&q[q_offset], &ff[1], &ncof, &iband4, ncc, &sigma, &c__[1], &
		sq, &rank, &wrk[la], &wrk[lf], &wrk[lh]);
L845:
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    q[i__ + q_dim1] /= dmax__;
/* L850: */
	}
/*  find the coefficients in the standard b-spline representation of */
/*  the spherical spline. */
	fprpsp_(nt, np, &coco[1], &cosi[1], &c__[1], &ff[1], &ncoff);
/*  compute f(p). */
	*fp = (float)0.;
	i__2 = nreg;
	for (num = 1; num <= i__2; ++num) {
	    num1 = num - 1;
	    lt = num1 / npp;
	    lp = num1 - lt * npp;
	    jrot = lt * np4 + lp;
	    in = index[num];
L860:
	    if (in == 0) {
		goto L890;
	    }
	    store = (float)0.;
	    i1 = jrot;
	    for (i__ = 1; i__ <= 4; ++i__) {
		hti = spt[in + i__ * spt_dim1];
		j1 = i1;
		for (j = 1; j <= 4; ++j) {
		    ++j1;
		    store += hti * spp[in + j * spp_dim1] * c__[j1];
/* L870: */
		}
		i1 += np4;
/* L880: */
	    }
/* Computing 2nd power */
	    r__1 = w[in] * (r__[in] - store);
	    *fp += r__1 * r__1;
	    in = nummer[in];
	    goto L860;
L890:
	    ;
	}
/*  test whether the approximation sp(teta,phi) is an acceptable solution */
	fpms = *fp - *s;
	if (dabs(fpms) <= acc) {
	    goto L980;
	}
/*  test whether the maximum allowable number of iterations has been */
/*  reached. */
	if (iter == *maxit) {
	    goto L940;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L900;
	}
	if (f2 - f3 > acc) {
	    goto L895;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L920;
L895:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L900:
	if (ich1 != 0) {
	    goto L910;
	}
	if (f1 - f2 > acc) {
	    goto L905;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L920;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L920;
L905:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L910:
	if (f2 >= f1 || f2 <= f3) {
	    goto L945;
	}
/*  find the new value of p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L920:
	;
    }
/*  error codes and messages. */
L925:
    *ier = lwest;
    goto L990;
L930:
    *ier = 5;
    goto L990;
L935:
    *ier = 4;
    goto L990;
L940:
    *ier = 3;
    goto L990;
L945:
    *ier = 2;
    goto L990;
L950:
    *ier = 1;
    goto L990;
L960:
    *ier = -2;
    goto L990;
L970:
    *ier = -1;
    *fp = (float)0.;
L980:
    if (ncof != rank) {
	*ier = -rank;
    }
L990:
    return 0;
} /* fpsphe_ */

/* Subroutine */ int fpsuev_(idim, tu, nu, tv, nv, c__, u, mu, v, mv, f, wu, 
	wv, lu, lv)
integer *idim;
real *tu;
integer *nu;
real *tv;
integer *nv;
real *c__, *u;
integer *mu;
real *v;
integer *mv;
real *f, *wu, *wv;
integer *lu, *lv;
{
    /* System generated locals */
    integer wu_dim1, wu_offset, wv_dim1, wv_offset, i__1, i__2, i__3;

    /* Local variables */
    static real h__[4];
    static integer i__, j, k, l, m, i1, j1, l1, l2, l3;
    static real tb, te, sp;
    extern /* Subroutine */ int fpbspl_();
    static integer nu4, nv4;
    static real arg;
    static integer nuv;

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..subroutine references.. */
/*    fpbspl */
/*  .. */
    /* Parameter adjustments */
    --tu;
    --c__;
    --tv;
    --lu;
    wu_dim1 = *mu;
    wu_offset = 1 + wu_dim1 * 1;
    wu -= wu_offset;
    --u;
    --lv;
    wv_dim1 = *mv;
    wv_offset = 1 + wv_dim1 * 1;
    wv -= wv_offset;
    --f;
    --v;

    /* Function Body */
    nu4 = *nu - 4;
    tb = tu[4];
    te = tu[nu4 + 1];
    l = 4;
    l1 = l + 1;
    i__1 = *mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	arg = u[i__];
	if (arg < tb) {
	    arg = tb;
	}
	if (arg > te) {
	    arg = te;
	}
L10:
	if (arg < tu[l1] || l == nu4) {
	    goto L20;
	}
	l = l1;
	l1 = l + 1;
	goto L10;
L20:
	fpbspl_(&tu[1], nu, &c__3, &arg, &l, h__);
	lu[i__] = l - 4;
	for (j = 1; j <= 4; ++j) {
	    wu[i__ + j * wu_dim1] = h__[j - 1];
/* L30: */
	}
/* L40: */
    }
    nv4 = *nv - 4;
    tb = tv[4];
    te = tv[nv4 + 1];
    l = 4;
    l1 = l + 1;
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	arg = v[i__];
	if (arg < tb) {
	    arg = tb;
	}
	if (arg > te) {
	    arg = te;
	}
L50:
	if (arg < tv[l1] || l == nv4) {
	    goto L60;
	}
	l = l1;
	l1 = l + 1;
	goto L50;
L60:
	fpbspl_(&tv[1], nv, &c__3, &arg, &l, h__);
	lv[i__] = l - 4;
	for (j = 1; j <= 4; ++j) {
	    wv[i__ + j * wv_dim1] = h__[j - 1];
/* L70: */
	}
/* L80: */
    }
    m = 0;
    nuv = nu4 * nv4;
    i__1 = *idim;
    for (k = 1; k <= i__1; ++k) {
	l3 = (k - 1) * nuv;
	i__2 = *mu;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    l = lu[i__] * nv4 + l3;
	    for (i1 = 1; i1 <= 4; ++i1) {
		h__[i1 - 1] = wu[i__ + i1 * wu_dim1];
/* L90: */
	    }
	    i__3 = *mv;
	    for (j = 1; j <= i__3; ++j) {
		l1 = l + lv[j];
		sp = (float)0.;
		for (i1 = 1; i1 <= 4; ++i1) {
		    l2 = l1;
		    for (j1 = 1; j1 <= 4; ++j1) {
			++l2;
			sp += c__[l2] * h__[i1 - 1] * wv[j + j1 * wv_dim1];
/* L100: */
		    }
		    l1 += nv4;
/* L110: */
		}
		++m;
		f[m] = sp;
/* L120: */
	    }
/* L130: */
	}
/* L140: */
    }
    return 0;
} /* fpsuev_ */

/* Subroutine */ int fpsurf_(iopt, m, x, y, z__, w, xb, xe, yb, ye, kxx, kyy, 
	s, nxest, nyest, eta, tol, maxit, nmax, km1, km2, ib1, ib3, nc, 
	intest, nrest, nx0, tx, ny0, ty, c__, fp, fp0, fpint, coord, f, ff, a,
	 q, bx, by, spx, spy, h__, index, nummer, wrk, lwrk, ier)
integer *iopt, *m;
real *x, *y, *z__, *w, *xb, *xe, *yb, *ye;
integer *kxx, *kyy;
real *s;
integer *nxest, *nyest;
real *eta, *tol;
integer *maxit, *nmax, *km1, *km2, *ib1, *ib3, *nc, *intest, *nrest, *nx0;
real *tx;
integer *ny0;
real *ty, *c__, *fp, *fp0, *fpint, *coord, *f, *ff, *a, *q, *bx, *by, *spx, *
	spy, *h__;
integer *index, *nummer;
real *wrk;
integer *lwrk, *ier;
{
    /* System generated locals */
    integer a_dim1, a_offset, q_dim1, q_offset, bx_dim1, bx_offset, by_dim1, 
	    by_offset, spx_dim1, spx_offset, spy_dim1, spy_offset, i__1, i__2,
	     i__3, i__4, i__5, i__6;
    real r__1;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    static real half;
    static integer ncof;
    static real dmax__;
    static integer nreg, rank, iter;
    static real fpms, pinv;
    static integer irot, jrot, i__, j, l, iband, n;
    static real p, sigma, fpmax, f1, f2, f3;
    static integer i1;
    static real store;
    static integer i2, iband1, i3, iband3;
    static real p1, p2, p3;
    static integer iband4, j1;
    static real x0, x1, y0, y1;
    static integer lwest, l1, l2, nminx, nminy, nrint, n1, la, ii, lf, lh, in;
    extern /* Subroutine */ int fpback_();
    static integer ichang;
    static real wi, rn, hx[6], zi, sq;
    static integer kx, ky, lx, ly, nx, ny;
    static real hy[6];
    extern /* Subroutine */ int fpdisc_(), fporde_(), fprank_();
    extern doublereal fprati_();
    extern /* Subroutine */ int fpbspl_(), fprota_(), fpgivs_();
    static integer kx1, kx2, ky1, ky2;
    static real acc;
    static integer ibb;
    static real arg, one, cos__, ten, eps, hxi, sin__;
    static integer nxe, nye;
    static real piv;
    static integer num;
    static real fac1, fac2;
    static integer jxy, nxx, nyy, ich1, ich3;
    static real con1, con4, con9;
    static integer num1, nk1x, nk1y;

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpback,fpbspl,fpgivs,fpdisc,fporde,fprank,fprota */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --nummer;
    --w;
    --z__;
    --y;
    --x;
    --ty;
    --tx;
    spy_dim1 = *m;
    spy_offset = 1 + spy_dim1 * 1;
    spy -= spy_offset;
    spx_dim1 = *m;
    spx_offset = 1 + spx_dim1 * 1;
    spx -= spx_offset;
    by_dim1 = *nmax;
    by_offset = 1 + by_dim1 * 1;
    by -= by_offset;
    bx_dim1 = *nmax;
    bx_offset = 1 + bx_dim1 * 1;
    bx -= bx_offset;
    --h__;
    q_dim1 = *nc;
    q_offset = 1 + q_dim1 * 1;
    q -= q_offset;
    a_dim1 = *nc;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    --ff;
    --f;
    --c__;
    --coord;
    --fpint;
    --index;
    --wrk;

    /* Function Body */
    one = (float)1.;
    con1 = (float).1;
    con9 = (float).9;
    con4 = (float).04;
    half = (float).5;
    ten = (float)10.;
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 1: determination of the number of knots and their position.     c */
/* ****************************************************************     c */
/* given a set of knots we compute the least-squares spline sinf(x,y),  c */
/* and the corresponding weighted sum of squared residuals fp=f(p=inf). c */
/* if iopt=-1  sinf(x,y) is the requested approximation.                c */
/* if iopt=0 or iopt=1 we check whether we can accept the knots:        c */
/*   if fp <=s we will continue with the current set of knots.          c */
/*   if fp > s we will increase the number of knots and compute the     c */
/*      corresponding least-squares spline until finally  fp<=s.        c */
/* the initial choice of knots depends on the value of s and iopt.      c */
/*   if iopt=0 we first compute the least-squares polynomial of degree  c */
/*     kx in x and ky in y; nx=nminx=2*kx+2 and ny=nminy=2*ky+2.        c */
/*     fp0=f(0) denotes the corresponding weighted sum of squared       c */
/*     residuals                                                        c */
/*   if iopt=1 we start with the knots found at the last call of the    c */
/*     routine, except for the case that s>=fp0; then we can compute    c */
/*     the least-squares polynomial directly.                           c */
/* eventually the independent variables x and y (and the corresponding  c */
/* parameters) will be switched if this can reduce the bandwidth of the c */
/* system to be solved.                                                 c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  ichang denotes whether(1) or not(-1) the directions have been inter- */
/*  changed. */
    ichang = -1;
    x0 = *xb;
    x1 = *xe;
    y0 = *yb;
    y1 = *ye;
    kx = *kxx;
    ky = *kyy;
    kx1 = kx + 1;
    ky1 = ky + 1;
    nxe = *nxest;
    nye = *nyest;
    eps = sqrt(*eta);
    if (*iopt < 0) {
	goto L20;
    }
/*  calculation of acc, the absolute tolerance for the root of f(p)=s. */
    acc = *tol * *s;
    if (*iopt == 0) {
	goto L10;
    }
    if (*fp0 > *s) {
	goto L20;
    }
/*  initialization for the least-squares polynomial. */
L10:
    nminx = kx1 << 1;
    nminy = ky1 << 1;
    nx = nminx;
    ny = nminy;
    *ier = -2;
    goto L30;
L20:
    nx = *nx0;
    ny = *ny0;
/*  main loop for the different sets of knots. m is a save upper bound */
/*  for the number of trials. */
L30:
    i__1 = *m;
    for (iter = 1; iter <= i__1; ++iter) {
/*  find the position of the additional knots which are needed for the */
/*  b-spline representation of s(x,y). */
	l = nx;
	i__2 = kx1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    tx[i__] = x0;
	    tx[l] = x1;
	    --l;
/* L40: */
	}
	l = ny;
	i__2 = ky1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ty[i__] = y0;
	    ty[l] = y1;
	    --l;
/* L50: */
	}
/*  find nrint, the total number of knot intervals and nreg, the number */
/*  of panels in which the approximation domain is subdivided by the */
/*  intersection of knots. */
	nxx = nx - (kx1 << 1) + 1;
	nyy = ny - (ky1 << 1) + 1;
	nrint = nxx + nyy;
	nreg = nxx * nyy;
/*  find the bandwidth of the observation matrix a. */
/*  if necessary, interchange the variables x and y, in order to obtain */
/*  a minimal bandwidth. */
	iband1 = kx * (ny - ky1) + ky;
	l = ky * (nx - kx1) + kx;
	if (iband1 <= l) {
	    goto L130;
	}
	iband1 = l;
	ichang = -ichang;
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    store = x[i__];
	    x[i__] = y[i__];
	    y[i__] = store;
/* L60: */
	}
	store = x0;
	x0 = y0;
	y0 = store;
	store = x1;
	x1 = y1;
	y1 = store;
	n = min(nx,ny);
	i__2 = n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    store = tx[i__];
	    tx[i__] = ty[i__];
	    ty[i__] = store;
/* L70: */
	}
	n1 = n + 1;
	if ((i__2 = nx - ny) < 0) {
	    goto L80;
	} else if (i__2 == 0) {
	    goto L120;
	} else {
	    goto L100;
	}
L80:
	i__2 = ny;
	for (i__ = n1; i__ <= i__2; ++i__) {
	    tx[i__] = ty[i__];
/* L90: */
	}
	goto L120;
L100:
	i__2 = nx;
	for (i__ = n1; i__ <= i__2; ++i__) {
	    ty[i__] = tx[i__];
/* L110: */
	}
L120:
	l = nx;
	nx = ny;
	ny = l;
	l = nxe;
	nxe = nye;
	nye = l;
	l = nxx;
	nxx = nyy;
	nyy = l;
	l = kx;
	kx = ky;
	ky = l;
	kx1 = kx + 1;
	ky1 = ky + 1;
L130:
	iband = iband1 + 1;
/*  arrange the data points according to the panel they belong to. */
	fporde_(&x[1], &y[1], m, &kx, &ky, &tx[1], &nx, &ty[1], &ny, &nummer[
		1], &index[1], &nreg);
/*  find ncof, the number of b-spline coefficients. */
	nk1x = nx - kx1;
	nk1y = ny - ky1;
	ncof = nk1x * nk1y;
/*  initialize the observation matrix a. */
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    f[i__] = (float)0.;
	    i__3 = iband;
	    for (j = 1; j <= i__3; ++j) {
		a[i__ + j * a_dim1] = (float)0.;
/* L140: */
	    }
	}
/*  initialize the sum of squared residuals. */
	*fp = (float)0.;
/*  fetch the data points in the new order. main loop for the */
/*  different panels. */
	i__3 = nreg;
	for (num = 1; num <= i__3; ++num) {
/*  fix certain constants for the current panel; jrot records the column */
/*  number of the first non-zero element in a row of the observation */
/*  matrix according to a data point of the panel. */
	    num1 = num - 1;
	    lx = num1 / nyy;
	    l1 = lx + kx1;
	    ly = num1 - lx * nyy;
	    l2 = ly + ky1;
	    jrot = lx * nk1y + ly;
/*  test whether there are still data points in the panel. */
	    in = index[num];
L150:
	    if (in == 0) {
		goto L250;
	    }
/*  fetch a new data point. */
	    wi = w[in];
	    zi = z__[in] * wi;
/*  evaluate for the x-direction, the (kx+1) non-zero b-splines at x(in). */
	    fpbspl_(&tx[1], &nx, &kx, &x[in], &l1, hx);
/*  evaluate for the y-direction, the (ky+1) non-zero b-splines at y(in). */
	    fpbspl_(&ty[1], &ny, &ky, &y[in], &l2, hy);
/*  store the value of these b-splines in spx and spy respectively. */
	    i__2 = kx1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		spx[in + i__ * spx_dim1] = hx[i__ - 1];
/* L160: */
	    }
	    i__2 = ky1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		spy[in + i__ * spy_dim1] = hy[i__ - 1];
/* L170: */
	    }
/*  initialize the new row of observation matrix. */
	    i__2 = iband;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		h__[i__] = (float)0.;
/* L180: */
	    }
/*  calculate the non-zero elements of the new row by making the cross */
/*  products of the non-zero b-splines in x- and y-direction. */
	    i1 = 0;
	    i__2 = kx1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		hxi = hx[i__ - 1];
		j1 = i1;
		i__4 = ky1;
		for (j = 1; j <= i__4; ++j) {
		    ++j1;
		    h__[j1] = hxi * hy[j - 1] * wi;
/* L190: */
		}
		i1 += nk1y;
/* L200: */
	    }
/*  rotate the row into triangle by givens transformations . */
	    irot = jrot;
	    i__2 = iband;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		++irot;
		piv = h__[i__];
		if (piv == (float)0.) {
		    goto L220;
		}
/*  calculate the parameters of the givens transformation. */
		fpgivs_(&piv, &a[irot + a_dim1], &cos__, &sin__);
/*  apply that transformation to the right hand side. */
		fprota_(&cos__, &sin__, &zi, &f[irot]);
		if (i__ == iband) {
		    goto L230;
		}
/*  apply that transformation to the left hand side. */
		i2 = 1;
		i3 = i__ + 1;
		i__4 = iband;
		for (j = i3; j <= i__4; ++j) {
		    ++i2;
		    fprota_(&cos__, &sin__, &h__[j], &a[irot + i2 * a_dim1]);
/* L210: */
		}
L220:
		;
	    }
/*  add the contribution of the row to the sum of squares of residual */
/*  right hand sides. */
L230:
/* Computing 2nd power */
	    r__1 = zi;
	    *fp += r__1 * r__1;
/*  find the number of the next data point in the panel. */
/* L240: */
	    in = nummer[in];
	    goto L150;
L250:
	    ;
	}
/*  find dmax, the maximum value for the diagonal elements in the reduced */
/*  triangle. */
	dmax__ = (float)0.;
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    if (a[i__ + a_dim1] <= dmax__) {
		goto L260;
	    }
	    dmax__ = a[i__ + a_dim1];
L260:
	    ;
	}
/*  check whether the observation matrix is rank deficient. */
	sigma = eps * dmax__;
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    if (a[i__ + a_dim1] <= sigma) {
		goto L280;
	    }
/* L270: */
	}
/*  backward substitution in case of full rank. */
	fpback_(&a[a_offset], &f[1], &ncof, &iband, &c__[1], nc);
	rank = ncof;
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    q[i__ + q_dim1] = a[i__ + a_dim1] / dmax__;
/* L275: */
	}
	goto L300;
/*  in case of rank deficiency, find the minimum norm solution. */
/*  check whether there is sufficient working space */
L280:
	lwest = ncof * iband + ncof + iband;
	if (*lwrk < lwest) {
	    goto L780;
	}
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    ff[i__] = f[i__];
	    i__2 = iband;
	    for (j = 1; j <= i__2; ++j) {
		q[i__ + j * q_dim1] = a[i__ + j * a_dim1];
/* L290: */
	    }
	}
	lf = 1;
	lh = lf + ncof;
	la = lh + iband;
	fprank_(&q[q_offset], &ff[1], &ncof, &iband, nc, &sigma, &c__[1], &sq,
		 &rank, &wrk[la], &wrk[lf], &wrk[lh]);
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    q[i__ + q_dim1] /= dmax__;
/* L295: */
	}
/*  add to the sum of squared residuals, the contribution of reducing */
/*  the rank. */
	*fp += sq;
L300:
	if (*ier == -2) {
	    *fp0 = *fp;
	}
/*  test whether the least-squares spline is an acceptable solution. */
	if (*iopt < 0) {
	    goto L820;
	}
	fpms = *fp - *s;
	if (dabs(fpms) <= acc) {
	    if (*fp <= (float)0.) {
		goto L815;
	    } else {
		goto L820;
	    }
	}
/*  test whether we can accept the choice of knots. */
	if (fpms < (float)0.) {
	    goto L430;
	}
/*  test whether we cannot further increase the number of knots. */
	if (ncof > *m) {
	    goto L790;
	}
	*ier = 0;
/*  search where to add a new knot. */
/*  find for each interval the sum of squared residuals fpint for the */
/*  data points having the coordinate belonging to that knot interval. */
/*  calculate also coord which is the same sum, weighted by the position */
/*  of the data points considered. */
/* L310: */
	i__2 = nrint;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    fpint[i__] = (float)0.;
	    coord[i__] = (float)0.;
/* L320: */
	}
	i__2 = nreg;
	for (num = 1; num <= i__2; ++num) {
	    num1 = num - 1;
	    lx = num1 / nyy;
	    l1 = lx + 1;
	    ly = num1 - lx * nyy;
	    l2 = ly + 1 + nxx;
	    jrot = lx * nk1y + ly;
	    in = index[num];
L330:
	    if (in == 0) {
		goto L360;
	    }
	    store = (float)0.;
	    i1 = jrot;
	    i__3 = kx1;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		hxi = spx[in + i__ * spx_dim1];
		j1 = i1;
		i__4 = ky1;
		for (j = 1; j <= i__4; ++j) {
		    ++j1;
		    store += hxi * spy[in + j * spy_dim1] * c__[j1];
/* L340: */
		}
		i1 += nk1y;
/* L350: */
	    }
/* Computing 2nd power */
	    r__1 = w[in] * (z__[in] - store);
	    store = r__1 * r__1;
	    fpint[l1] += store;
	    coord[l1] += store * x[in];
	    fpint[l2] += store;
	    coord[l2] += store * y[in];
	    in = nummer[in];
	    goto L330;
L360:
	    ;
	}
/*  find the interval for which fpint is maximal on the condition that */
/*  there still can be added a knot. */
L370:
	l = 0;
	fpmax = (float)0.;
	l1 = 1;
	l2 = nrint;
	if (nx == nxe) {
	    l1 = nxx + 1;
	}
	if (ny == nye) {
	    l2 = nxx;
	}
	if (l1 > l2) {
	    goto L810;
	}
	i__2 = l2;
	for (i__ = l1; i__ <= i__2; ++i__) {
	    if (fpmax >= fpint[i__]) {
		goto L380;
	    }
	    l = i__;
	    fpmax = fpint[i__];
L380:
	    ;
	}
/*  test whether we cannot further increase the number of knots. */
	if (l == 0) {
	    goto L785;
	}
/*  calculate the position of the new knot. */
	arg = coord[l] / fpint[l];
/*  test in what direction the new knot is going to be added. */
	if (l > nxx) {
	    goto L400;
	}
/*  addition in the x-direction. */
	jxy = l + kx1;
	fpint[l] = (float)0.;
	fac1 = tx[jxy] - arg;
	fac2 = arg - tx[jxy - 1];
	if (fac1 > ten * fac2 || fac2 > ten * fac1) {
	    goto L370;
	}
	j = nx;
	i__2 = nx;
	for (i__ = jxy; i__ <= i__2; ++i__) {
	    tx[j + 1] = tx[j];
	    --j;
/* L390: */
	}
	tx[jxy] = arg;
	++nx;
	goto L420;
/*  addition in the y-direction. */
L400:
	jxy = l + ky1 - nxx;
	fpint[l] = (float)0.;
	fac1 = ty[jxy] - arg;
	fac2 = arg - ty[jxy - 1];
	if (fac1 > ten * fac2 || fac2 > ten * fac1) {
	    goto L370;
	}
	j = ny;
	i__2 = ny;
	for (i__ = jxy; i__ <= i__2; ++i__) {
	    ty[j + 1] = ty[j];
	    --j;
/* L410: */
	}
	ty[jxy] = arg;
	++ny;
/*  restart the computations with the new set of knots. */
L420:
	;
    }
/*  test whether the least-squares polynomial is a solution of our */
/*  approximation problem. */
L430:
    if (*ier == -2) {
	goto L830;
    }
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/* part 2: determination of the smoothing spline sp(x,y)                c */
/* *****************************************************                c */
/* we have determined the number of knots and their position. we now    c */
/* compute the b-spline coefficients of the smoothing spline sp(x,y).   c */
/* the observation matrix a is extended by the rows of a matrix,        c */
/* expressing that sp(x,y) must be a polynomial of degree kx in x and   c */
/* ky in y. the corresponding weights of these additional rows are set  c */
/* to 1./p.  iteratively we than have to determine the value of p       c */
/* such that f(p)=sum((w(i)*(z(i)-sp(x(i),y(i))))**2) be = s.           c */
/* we already know that the least-squares polynomial corresponds to     c */
/* p=0  and that the least-squares spline corresponds to p=infinity.    c */
/* the iteration process which is proposed here makes use of rational   c */
/* interpolation. since f(p) is a convex and strictly decreasing        c */
/* function of p, it can be approximated by a rational function r(p)=   c */
/* (u*p+v)/(p+w). three values of p(p1,p2,p3) with corresponding values c */
/* of f(p) (f1=f(p1)-s,f2=f(p2)-s,f3=f(p3)-s) are used to calculate the c */
/* new value of p such that r(p)=s. convergence is guaranteed by taking c */
/* f1 > 0 and f3 < 0.                                                   c */
/* ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
    kx2 = kx1 + 1;
/*  test whether there are interior knots in the x-direction. */
    if (nk1x == kx1) {
	goto L440;
    }
/*  evaluate the discotinuity jumps of the kx-th order derivative of */
/*  the b-splines at the knots tx(l),l=kx+2,...,nx-kx-1. */
    fpdisc_(&tx[1], &nx, &kx2, &bx[bx_offset], nmax);
L440:
    ky2 = ky1 + 1;
/*  test whether there are interior knots in the y-direction. */
    if (nk1y == ky1) {
	goto L450;
    }
/*  evaluate the discontinuity jumps of the ky-th order derivative of */
/*  the b-splines at the knots ty(l),l=ky+2,...,ny-ky-1. */
    fpdisc_(&ty[1], &ny, &ky2, &by[by_offset], nmax);
/*  initial value for p. */
L450:
    p1 = (float)0.;
    f1 = *fp0 - *s;
    p3 = -one;
    f3 = fpms;
    p = (float)0.;
    i__1 = ncof;
    for (i__ = 1; i__ <= i__1; ++i__) {
	p += a[i__ + a_dim1];
/* L460: */
    }
    rn = (real) ncof;
    p = rn / p;
/*  find the bandwidth of the extended observation matrix. */
    iband3 = kx1 * nk1y;
    iband4 = iband3 + 1;
    ich1 = 0;
    ich3 = 0;
/*  iteration process to find the root of f(p)=s. */
    i__1 = *maxit;
    for (iter = 1; iter <= i__1; ++iter) {
	pinv = one / p;
/*  store the triangularized observation matrix into q. */
	i__2 = ncof;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ff[i__] = f[i__];
	    i__3 = iband;
	    for (j = 1; j <= i__3; ++j) {
		q[i__ + j * q_dim1] = a[i__ + j * a_dim1];
/* L470: */
	    }
	    ibb = iband + 1;
	    i__3 = iband4;
	    for (j = ibb; j <= i__3; ++j) {
		q[i__ + j * q_dim1] = (float)0.;
/* L480: */
	    }
	}
	if (nk1y == ky1) {
	    goto L560;
	}
/*  extend the observation matrix with the rows of a matrix, expressing */
/*  that for x=cst. sp(x,y) must be a polynomial in y of degree ky. */
	i__3 = nk1y;
	for (i__ = ky2; i__ <= i__3; ++i__) {
	    ii = i__ - ky1;
	    i__2 = nk1x;
	    for (j = 1; j <= i__2; ++j) {
/*  initialize the new row. */
		i__4 = iband;
		for (l = 1; l <= i__4; ++l) {
		    h__[l] = (float)0.;
/* L490: */
		}
/*  fill in the non-zero elements of the row. jrot records the column */
/*  number of the first non-zero element in the row. */
		i__4 = ky2;
		for (l = 1; l <= i__4; ++l) {
		    h__[l] = by[ii + l * by_dim1] * pinv;
/* L500: */
		}
		zi = (float)0.;
		jrot = (j - 1) * nk1y + ii;
/*  rotate the new row into triangle by givens transformations without */
/*  square roots. */
		i__4 = ncof;
		for (irot = jrot; irot <= i__4; ++irot) {
		    piv = h__[1];
/* Computing MIN */
		    i__5 = iband1, i__6 = ncof - irot;
		    i2 = min(i__5,i__6);
		    if (piv == (float)0.) {
			if (i2 <= 0) {
			    goto L550;
			} else {
			    goto L520;
			}
		    }
/*  calculate the parameters of the givens transformation. */
		    fpgivs_(&piv, &q[irot + q_dim1], &cos__, &sin__);
/*  apply that givens transformation to the right hand side. */
		    fprota_(&cos__, &sin__, &zi, &ff[irot]);
		    if (i2 == 0) {
			goto L550;
		    }
/*  apply that givens transformation to the left hand side. */
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			l1 = l + 1;
			fprota_(&cos__, &sin__, &h__[l1], &q[irot + l1 * 
				q_dim1]);
/* L510: */
		    }
L520:
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			h__[l] = h__[l + 1];
/* L530: */
		    }
		    h__[i2 + 1] = (float)0.;
/* L540: */
		}
L550:
		;
	    }
	}
L560:
	if (nk1x == kx1) {
	    goto L640;
	}
/*  extend the observation matrix with the rows of a matrix expressing */
/*  that for y=cst. sp(x,y) must be a polynomial in x of degree kx. */
	i__2 = nk1x;
	for (i__ = kx2; i__ <= i__2; ++i__) {
	    ii = i__ - kx1;
	    i__3 = nk1y;
	    for (j = 1; j <= i__3; ++j) {
/*  initialize the new row */
		i__4 = iband4;
		for (l = 1; l <= i__4; ++l) {
		    h__[l] = (float)0.;
/* L570: */
		}
/*  fill in the non-zero elements of the row. jrot records the column */
/*  number of the first non-zero element in the row. */
		j1 = 1;
		i__4 = kx2;
		for (l = 1; l <= i__4; ++l) {
		    h__[j1] = bx[ii + l * bx_dim1] * pinv;
		    j1 += nk1y;
/* L580: */
		}
		zi = (float)0.;
		jrot = (i__ - kx2) * nk1y + j;
/*  rotate the new row into triangle by givens transformations . */
		i__4 = ncof;
		for (irot = jrot; irot <= i__4; ++irot) {
		    piv = h__[1];
/* Computing MIN */
		    i__5 = iband3, i__6 = ncof - irot;
		    i2 = min(i__5,i__6);
		    if (piv == (float)0.) {
			if (i2 <= 0) {
			    goto L630;
			} else {
			    goto L600;
			}
		    }
/*  calculate the parameters of the givens transformation. */
		    fpgivs_(&piv, &q[irot + q_dim1], &cos__, &sin__);
/*  apply that givens transformation to the right hand side. */
		    fprota_(&cos__, &sin__, &zi, &ff[irot]);
		    if (i2 == 0) {
			goto L630;
		    }
/*  apply that givens transformation to the left hand side. */
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			l1 = l + 1;
			fprota_(&cos__, &sin__, &h__[l1], &q[irot + l1 * 
				q_dim1]);
/* L590: */
		    }
L600:
		    i__5 = i2;
		    for (l = 1; l <= i__5; ++l) {
			h__[l] = h__[l + 1];
/* L610: */
		    }
		    h__[i2 + 1] = (float)0.;
/* L620: */
		}
L630:
		;
	    }
	}
/*  find dmax, the maximum value for the diagonal elements in the */
/*  reduced triangle. */
L640:
	dmax__ = (float)0.;
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    if (q[i__ + q_dim1] <= dmax__) {
		goto L650;
	    }
	    dmax__ = q[i__ + q_dim1];
L650:
	    ;
	}
/*  check whether the matrix is rank deficient. */
	sigma = eps * dmax__;
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    if (q[i__ + q_dim1] <= sigma) {
		goto L670;
	    }
/* L660: */
	}
/*  backward substitution in case of full rank. */
	fpback_(&q[q_offset], &ff[1], &ncof, &iband4, &c__[1], nc);
	rank = ncof;
	goto L675;
/*  in case of rank deficiency, find the minimum norm solution. */
L670:
	lwest = ncof * iband4 + ncof + iband4;
	if (*lwrk < lwest) {
	    goto L780;
	}
	lf = 1;
	lh = lf + ncof;
	la = lh + iband4;
	fprank_(&q[q_offset], &ff[1], &ncof, &iband4, nc, &sigma, &c__[1], &
		sq, &rank, &wrk[la], &wrk[lf], &wrk[lh]);
L675:
	i__3 = ncof;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    q[i__ + q_dim1] /= dmax__;
/* L680: */
	}
/*  compute f(p). */
	*fp = (float)0.;
	i__3 = nreg;
	for (num = 1; num <= i__3; ++num) {
	    num1 = num - 1;
	    lx = num1 / nyy;
	    ly = num1 - lx * nyy;
	    jrot = lx * nk1y + ly;
	    in = index[num];
L690:
	    if (in == 0) {
		goto L720;
	    }
	    store = (float)0.;
	    i1 = jrot;
	    i__2 = kx1;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		hxi = spx[in + i__ * spx_dim1];
		j1 = i1;
		i__4 = ky1;
		for (j = 1; j <= i__4; ++j) {
		    ++j1;
		    store += hxi * spy[in + j * spy_dim1] * c__[j1];
/* L700: */
		}
		i1 += nk1y;
/* L710: */
	    }
/* Computing 2nd power */
	    r__1 = w[in] * (z__[in] - store);
	    *fp += r__1 * r__1;
	    in = nummer[in];
	    goto L690;
L720:
	    ;
	}
/*  test whether the approximation sp(x,y) is an acceptable solution. */
	fpms = *fp - *s;
	if (dabs(fpms) <= acc) {
	    goto L820;
	}
/*  test whether the maximum allowable number of iterations has been */
/*  reached. */
	if (iter == *maxit) {
	    goto L795;
	}
/*  carry out one more step of the iteration process. */
	p2 = p;
	f2 = fpms;
	if (ich3 != 0) {
	    goto L740;
	}
	if (f2 - f3 > acc) {
	    goto L730;
	}
/*  our initial choice of p is too large. */
	p3 = p2;
	f3 = f2;
	p *= con4;
	if (p <= p1) {
	    p = p1 * con9 + p2 * con1;
	}
	goto L770;
L730:
	if (f2 < (float)0.) {
	    ich3 = 1;
	}
L740:
	if (ich1 != 0) {
	    goto L760;
	}
	if (f1 - f2 > acc) {
	    goto L750;
	}
/*  our initial choice of p is too small */
	p1 = p2;
	f1 = f2;
	p /= con4;
	if (p3 < (float)0.) {
	    goto L770;
	}
	if (p >= p3) {
	    p = p2 * con1 + p3 * con9;
	}
	goto L770;
L750:
	if (f2 > (float)0.) {
	    ich1 = 1;
	}
/*  test whether the iteration process proceeds as theoretically */
/*  expected. */
L760:
	if (f2 >= f1 || f2 <= f3) {
	    goto L800;
	}
/*  find the new value of p. */
	p = fprati_(&p1, &f1, &p2, &f2, &p3, &f3);
L770:
	;
    }
/*  error codes and messages. */
L780:
    *ier = lwest;
    goto L830;
L785:
    *ier = 5;
    goto L830;
L790:
    *ier = 4;
    goto L830;
L795:
    *ier = 3;
    goto L830;
L800:
    *ier = 2;
    goto L830;
L810:
    *ier = 1;
    goto L830;
L815:
    *ier = -1;
    *fp = (float)0.;
L820:
    if (ncof != rank) {
	*ier = -rank;
    }
/*  test whether x and y are in the original order. */
L830:
    if (ichang < 0) {
	goto L930;
    }
/*  if not, interchange x and y once more. */
    l1 = 1;
    i__1 = nk1x;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l2 = i__;
	i__3 = nk1y;
	for (j = 1; j <= i__3; ++j) {
	    f[l2] = c__[l1];
	    ++l1;
	    l2 += nk1x;
/* L840: */
	}
    }
    i__3 = ncof;
    for (i__ = 1; i__ <= i__3; ++i__) {
	c__[i__] = f[i__];
/* L850: */
    }
    i__3 = *m;
    for (i__ = 1; i__ <= i__3; ++i__) {
	store = x[i__];
	x[i__] = y[i__];
	y[i__] = store;
/* L860: */
    }
    n = min(nx,ny);
    i__3 = n;
    for (i__ = 1; i__ <= i__3; ++i__) {
	store = tx[i__];
	tx[i__] = ty[i__];
	ty[i__] = store;
/* L870: */
    }
    n1 = n + 1;
    if ((i__3 = nx - ny) < 0) {
	goto L880;
    } else if (i__3 == 0) {
	goto L920;
    } else {
	goto L900;
    }
L880:
    i__3 = ny;
    for (i__ = n1; i__ <= i__3; ++i__) {
	tx[i__] = ty[i__];
/* L890: */
    }
    goto L920;
L900:
    i__3 = nx;
    for (i__ = n1; i__ <= i__3; ++i__) {
	ty[i__] = tx[i__];
/* L910: */
    }
L920:
    l = nx;
    nx = ny;
    ny = l;
L930:
    if (*iopt < 0) {
	goto L940;
    }
    *nx0 = nx;
    *ny0 = ny;
L940:
    return 0;
} /* fpsurf_ */

/* Subroutine */ int fpsysy_(a, n, g)
real *a;
integer *n;
real *g;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, i1;
    static real fac;

/* subroutine fpsysy solves a linear n x n symmetric system */
/*    (a) * (b) = (g) */
/* on input, vector g contains the right hand side ; on output it will */
/* contain the solution (b). */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --g;
    a -= 7;

    /* Function Body */
    g[1] /= a[7];
    if (*n == 1) {
	return 0;
    }
/*  decomposition of the symmetric matrix (a) = (l) * (d) *(l)' */
/*  with (l) a unit lower triangular matrix and (d) a diagonal */
/*  matrix */
    i__1 = *n;
    for (k = 2; k <= i__1; ++k) {
	a[k + 6] /= a[7];
/* L10: */
    }
    i__1 = *n;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i1 = i__ - 1;
	i__2 = *n;
	for (k = i__; k <= i__2; ++k) {
	    fac = a[k + i__ * 6];
	    i__3 = i1;
	    for (j = 1; j <= i__3; ++j) {
		fac -= a[j + j * 6] * a[k + j * 6] * a[i__ + j * 6];
/* L20: */
	    }
	    a[k + i__ * 6] = fac;
	    if (k > i__) {
		a[k + i__ * 6] = fac / a[i__ + i__ * 6];
	    }
/* L30: */
	}
/* L40: */
    }
/*  solve the system (l)*(d)*(l)'*(b) = (g). */
/*  first step : solve (l)*(d)*(c) = (g). */
    i__1 = *n;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i1 = i__ - 1;
	fac = g[i__];
	i__2 = i1;
	for (j = 1; j <= i__2; ++j) {
	    fac -= g[j] * a[j + j * 6] * a[i__ + j * 6];
/* L50: */
	}
	g[i__] = fac / a[i__ + i__ * 6];
/* L60: */
    }
/*  second step : solve (l)'*(b) = (c) */
    i__ = *n;
    i__1 = *n;
    for (j = 2; j <= i__1; ++j) {
	i1 = i__;
	--i__;
	fac = g[i__];
	i__2 = *n;
	for (k = i1; k <= i__2; ++k) {
	    fac -= g[k] * a[k + i__ * 6];
/* L70: */
	}
	g[i__] = fac;
/* L80: */
    }
    return 0;
} /* fpsysy_ */

/* Subroutine */ int fptrnp_(m, mm, idim, n, nr, sp, p, b, z__, a, q, right)
integer *m, *mm, *idim, *n, *nr;
real *sp, *p, *b, *z__, *a, *q, *right;
{
    /* System generated locals */
    integer sp_dim1, sp_offset, b_dim1, b_offset, a_dim1, a_offset, i__1, 
	    i__2, i__3, i__4;

    /* Local variables */
    static real pinv;
    static integer irot;
    static real h__[7];
    static integer i__, j, l, iband, nrold, i2, i3, m2, m3, n1, n4, ii, jj, 
	    it, number;
    extern /* Subroutine */ int fprota_(), fpgivs_();
    static integer mid, nmd;
    static real one, cos__, sin__, piv;

/*  subroutine fptrnp reduces the (m+n-7) x (n-4) matrix a to upper */
/*  triangular form and applies the same givens transformations to */
/*  the (m) x (mm) x (idim) matrix z to obtain the (n-4) x (mm) x */
/*  (idim) matrix q */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..subroutine references.. */
/*    fpgivs,fprota */
/*  .. */
    /* Parameter adjustments */
    sp_dim1 = *m;
    sp_offset = 1 + sp_dim1 * 1;
    sp -= sp_offset;
    --nr;
    --right;
    --z__;
    --q;
    a_dim1 = *n;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    b_dim1 = *n;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    one = (float)1.;
    if (*p > (float)0.) {
	pinv = one / *p;
    }
    n4 = *n - 4;
    mid = *mm * *idim;
    m2 = *m * *mm;
    m3 = n4 * *mm;
/*  reduce the matrix (a) to upper triangular form (r) using givens */
/*  rotations. apply the same transformations to the rows of matrix z */
/*  to obtain the mm x (n-4) matrix g. */
/*  store matrix (r) into (a) and g into q. */
/*  initialization. */
    nmd = n4 * mid;
    i__1 = nmd;
    for (i__ = 1; i__ <= i__1; ++i__) {
	q[i__] = (float)0.;
/* L50: */
    }
    i__1 = n4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 5; ++j) {
	    a[i__ + j * a_dim1] = (float)0.;
/* L100: */
	}
    }
    nrold = 0;
/*  iband denotes the bandwidth of the matrices (a) and (r). */
    iband = 4;
    i__1 = *m;
    for (it = 1; it <= i__1; ++it) {
	number = nr[it];
L150:
	if (nrold == number) {
	    goto L300;
	}
	if (*p <= (float)0.) {
	    goto L700;
	}
	iband = 5;
/*  fetch a new row of matrix (b). */
	n1 = nrold + 1;
	for (j = 1; j <= 5; ++j) {
	    h__[j - 1] = b[n1 + j * b_dim1] * pinv;
/* L200: */
	}
/*  find the appropriate column of q. */
	i__2 = mid;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = (float)0.;
/* L250: */
	}
	irot = nrold;
	goto L450;
/*  fetch a new row of matrix (sp). */
L300:
	h__[iband - 1] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    h__[j - 1] = sp[it + j * sp_dim1];
/* L350: */
	}
/*  find the appropriate column of q. */
	j = 0;
	i__2 = *idim;
	for (ii = 1; ii <= i__2; ++ii) {
	    l = (ii - 1) * m2 + (it - 1) * *mm;
	    i__3 = *mm;
	    for (jj = 1; jj <= i__3; ++jj) {
		++j;
		++l;
		right[j] = z__[l];
/* L400: */
	    }
	}
	irot = number;
/*  rotate the new row of matrix (a) into triangle. */
L450:
	i__3 = iband;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    ++irot;
	    piv = h__[i__ - 1];
	    if (piv == (float)0.) {
		goto L600;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &a[irot + a_dim1], &cos__, &sin__);
/*  apply that transformation to the rows of matrix q. */
	    j = 0;
	    i__2 = *idim;
	    for (ii = 1; ii <= i__2; ++ii) {
		l = (ii - 1) * m3 + irot;
		i__4 = *mm;
		for (jj = 1; jj <= i__4; ++jj) {
		    ++j;
		    fprota_(&cos__, &sin__, &right[j], &q[l]);
		    l += n4;
/* L500: */
		}
	    }
/*  apply that transformation to the columns of (a). */
	    if (i__ == iband) {
		goto L650;
	    }
	    i2 = 1;
	    i3 = i__ + 1;
	    i__4 = iband;
	    for (j = i3; j <= i__4; ++j) {
		++i2;
		fprota_(&cos__, &sin__, &h__[j - 1], &a[irot + i2 * a_dim1]);
/* L550: */
	    }
L600:
	    ;
	}
L650:
	if (nrold == number) {
	    goto L750;
	}
L700:
	++nrold;
	goto L150;
L750:
	;
    }
    return 0;
} /* fptrnp_ */

/* Subroutine */ int fptrpe_(m, mm, idim, n, nr, sp, p, b, z__, a, aa, q, 
	right)
integer *m, *mm, *idim, *n, *nr;
real *sp, *p, *b, *z__, *a, *aa, *q, *right;
{
    /* System generated locals */
    integer sp_dim1, sp_offset, b_dim1, b_offset, a_dim1, a_offset, aa_dim1, 
	    aa_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer jper;
    static real pinv;
    static integer irot;
    static real h__[5];
    static integer i__, j, l, nrold;
    static real h1[5], h2[4];
    static integer i2, i3, l0, m1, m2, m3, n1, n4, l1, i1, n7, j1, n11;
    static real co;
    static integer ii, jj, jk, ik, ij;
    static real si;
    static integer it, number;
    extern /* Subroutine */ int fprota_(), fpgivs_();
    static integer mid, nmd;
    static real one, piv;

/*  subroutine fptrpe reduces the (m+n-7) x (n-7) cyclic bandmatrix a */
/*  to upper triangular form and applies the same givens transformations */
/*  to the (m) x (mm) x (idim) matrix z to obtain the (n-7) x (mm) x */
/*  (idim) matrix q. */
/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays.. */
/*  ..subroutine references.. */
/*    fpgivs,fprota */
/*  .. */
    /* Parameter adjustments */
    sp_dim1 = *m;
    sp_offset = 1 + sp_dim1 * 1;
    sp -= sp_offset;
    --nr;
    --right;
    --z__;
    --q;
    aa_dim1 = *n;
    aa_offset = 1 + aa_dim1 * 1;
    aa -= aa_offset;
    a_dim1 = *n;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    b_dim1 = *n;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    one = (float)1.;
    if (*p > (float)0.) {
	pinv = one / *p;
    }
    n4 = *n - 4;
    n7 = *n - 7;
    n11 = *n - 11;
    mid = *mm * *idim;
    m2 = *m * *mm;
    m3 = n7 * *mm;
    m1 = *m - 1;
/*  we determine the matrix (a) and then we reduce her to */
/*  upper triangular form (r) using givens rotations. */
/*  we apply the same transformations to the rows of matrix */
/*  z to obtain the (mm) x (n-7) matrix g. */
/*  we store matrix (r) into a and aa, g into q. */
/*  the n7 x n7 upper triangular matrix (r) has the form */
/*             | a1 '     | */
/*       (r) = |    ' a2  | */
/*             |  0 '     | */
/*  with (a2) a n7 x 4 matrix and (a1) a n11 x n11 upper */
/*  triangular matrix of bandwidth 5. */
/*  initialization. */
    nmd = n7 * mid;
    i__1 = nmd;
    for (i__ = 1; i__ <= i__1; ++i__) {
	q[i__] = (float)0.;
/* L50: */
    }
    i__1 = n4;
    for (i__ = 1; i__ <= i__1; ++i__) {
	a[i__ + a_dim1 * 5] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    a[i__ + j * a_dim1] = (float)0.;
	    aa[i__ + j * aa_dim1] = (float)0.;
/* L100: */
	}
    }
    jper = 0;
    nrold = 0;
    i__1 = m1;
    for (it = 1; it <= i__1; ++it) {
	number = nr[it];
L120:
	if (nrold == number) {
	    goto L180;
	}
	if (*p <= (float)0.) {
	    goto L740;
	}
/*  fetch a new row of matrix (b). */
	n1 = nrold + 1;
	for (j = 1; j <= 5; ++j) {
	    h__[j - 1] = b[n1 + j * b_dim1] * pinv;
/* L140: */
	}
/*  find the appropiate row of q. */
	i__2 = mid;
	for (j = 1; j <= i__2; ++j) {
	    right[j] = (float)0.;
/* L160: */
	}
	goto L240;
/*  fetch a new row of matrix (sp) */
L180:
	h__[4] = (float)0.;
	for (j = 1; j <= 4; ++j) {
	    h__[j - 1] = sp[it + j * sp_dim1];
/* L200: */
	}
/*  find the appropiate row of q. */
	j = 0;
	i__2 = *idim;
	for (ii = 1; ii <= i__2; ++ii) {
	    l = (ii - 1) * m2 + (it - 1) * *mm;
	    i__3 = *mm;
	    for (jj = 1; jj <= i__3; ++jj) {
		++j;
		++l;
		right[j] = z__[l];
/* L220: */
	    }
	}
/*  test whether there are non-zero values in the new row of (a) */
/*  corresponding to the b-splines n(j,*),j=n7+1,...,n4. */
L240:
	if (nrold < n11) {
	    goto L640;
	}
	if (jper != 0) {
	    goto L320;
	}
/*  initialize the matrix (aa). */
	jk = n11 + 1;
	for (i__ = 1; i__ <= 4; ++i__) {
	    ik = jk;
	    for (j = 1; j <= 5; ++j) {
		if (ik <= 0) {
		    goto L280;
		}
		aa[ik + i__ * aa_dim1] = a[ik + j * a_dim1];
		--ik;
/* L260: */
	    }
L280:
	    ++jk;
/* L300: */
	}
	jper = 1;
/*  if one of the non-zero elements of the new row corresponds to one of */
/*  the b-splines n(j;*),j=n7+1,...,n4,we take account of the periodicity */
/*  conditions for setting up this row of (a). */
L320:
	for (i__ = 1; i__ <= 4; ++i__) {
	    h1[i__ - 1] = (float)0.;
	    h2[i__ - 1] = (float)0.;
/* L340: */
	}
	h1[4] = (float)0.;
	j = nrold - n11;
	for (i__ = 1; i__ <= 5; ++i__) {
	    ++j;
	    l0 = j;
L360:
	    l1 = l0 - 4;
	    if (l1 <= 0) {
		goto L400;
	    }
	    if (l1 <= n11) {
		goto L380;
	    }
	    l0 = l1 - n11;
	    goto L360;
L380:
	    h1[l1 - 1] = h__[i__ - 1];
	    goto L420;
L400:
	    h2[l0 - 1] += h__[i__ - 1];
L420:
	    ;
	}
/*  rotate the new row of (a) into triangle. */
	if (n11 <= 0) {
	    goto L560;
	}
/*  rotations with the rows 1,2,...,n11 of (a). */
	i__3 = n11;
	for (irot = 1; irot <= i__3; ++irot) {
	    piv = h1[0];
/* Computing MIN */
	    i__2 = n11 - irot;
	    i2 = min(i__2,4);
	    if (piv == (float)0.) {
		goto L500;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &a[irot + a_dim1], &co, &si);
/*  apply that transformation to the columns of matrix q. */
	    j = 0;
	    i__2 = *idim;
	    for (ii = 1; ii <= i__2; ++ii) {
		l = (ii - 1) * m3 + irot;
		i__4 = *mm;
		for (jj = 1; jj <= i__4; ++jj) {
		    ++j;
		    fprota_(&co, &si, &right[j], &q[l]);
		    l += n7;
/* L440: */
		}
	    }
/*  apply that transformation to the rows of (a) with respect to aa. */
	    for (i__ = 1; i__ <= 4; ++i__) {
		fprota_(&co, &si, &h2[i__ - 1], &aa[irot + i__ * aa_dim1]);
/* L460: */
	    }
/*  apply that transformation to the rows of (a) with respect to a. */
	    if (i2 == 0) {
		goto L560;
	    }
	    i__4 = i2;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		i1 = i__ + 1;
		fprota_(&co, &si, &h1[i1 - 1], &a[irot + i1 * a_dim1]);
/* L480: */
	    }
L500:
	    i__4 = i2;
	    for (i__ = 1; i__ <= i__4; ++i__) {
		h1[i__ - 1] = h1[i__];
/* L520: */
	    }
	    h1[i2] = (float)0.;
/* L540: */
	}
/*  rotations with the rows n11+1,...,n7 of a. */
L560:
	for (irot = 1; irot <= 4; ++irot) {
	    ij = n11 + irot;
	    if (ij <= 0) {
		goto L620;
	    }
	    piv = h2[irot - 1];
	    if (piv == (float)0.) {
		goto L620;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &aa[ij + irot * aa_dim1], &co, &si);
/*  apply that transformation to the columns of matrix q. */
	    j = 0;
	    i__3 = *idim;
	    for (ii = 1; ii <= i__3; ++ii) {
		l = (ii - 1) * m3 + ij;
		i__4 = *mm;
		for (jj = 1; jj <= i__4; ++jj) {
		    ++j;
		    fprota_(&co, &si, &right[j], &q[l]);
		    l += n7;
/* L580: */
		}
	    }
	    if (irot == 4) {
		goto L620;
	    }
/*  apply that transformation to the rows of (a) with respect to aa. */
	    j1 = irot + 1;
	    for (i__ = j1; i__ <= 4; ++i__) {
		fprota_(&co, &si, &h2[i__ - 1], &aa[ij + i__ * aa_dim1]);
/* L600: */
	    }
L620:
	    ;
	}
	goto L720;
/*  rotation into triangle of the new row of (a), in case the elements */
/*  corresponding to the b-splines n(j;*),j=n7+1,...,n4 are all zero. */
L640:
	irot = nrold;
	for (i__ = 1; i__ <= 5; ++i__) {
	    ++irot;
	    piv = h__[i__ - 1];
	    if (piv == (float)0.) {
		goto L700;
	    }
/*  calculate the parameters of the givens transformation. */
	    fpgivs_(&piv, &a[irot + a_dim1], &co, &si);
/*  apply that transformation to the columns of matrix g. */
	    j = 0;
	    i__4 = *idim;
	    for (ii = 1; ii <= i__4; ++ii) {
		l = (ii - 1) * m3 + irot;
		i__3 = *mm;
		for (jj = 1; jj <= i__3; ++jj) {
		    ++j;
		    fprota_(&co, &si, &right[j], &q[l]);
		    l += n7;
/* L660: */
		}
	    }
/*  apply that transformation to the rows of (a). */
	    if (i__ == 5) {
		goto L700;
	    }
	    i2 = 1;
	    i3 = i__ + 1;
	    for (j = i3; j <= 5; ++j) {
		++i2;
		fprota_(&co, &si, &h__[j - 1], &a[irot + i2 * a_dim1]);
/* L680: */
	    }
L700:
	    ;
	}
L720:
	if (nrold == number) {
	    goto L760;
	}
L740:
	++nrold;
	goto L120;
L760:
	;
    }
    return 0;
} /* fptrpe_ */

/* Subroutine */ int insert_(iopt, t, n, c__, k, x, tt, nn, cc, nest, ier)
integer *iopt;
real *t;
integer *n;
real *c__;
integer *k;
real *x, *tt;
integer *nn;
real *cc;
integer *nest, *ier;
{
    static integer l, k1, kk, nk;
    extern /* Subroutine */ int fpinst_();
    static integer nk1;

/*  subroutine insert inserts a new knot x into a spline function s(x) */
/*  of degree k and calculates the b-spline representation of s(x) with */
/*  respect to the new set of knots. in addition, if iopt.ne.0, s(x) */
/*  will be considered as a periodic spline with period per=t(n-k)-t(k+1) */
/*  satisfying the boundary constraints */
/*       t(i+n-2*k-1) = t(i)+per  ,i=1,2,...,2*k+1 */
/*       c(i+n-2*k-1) = c(i)      ,i=1,2,...,k */
/*  in that case, the knots and b-spline coefficients returned will also */
/*  satisfy these boundary constraints, i.e. */
/*       tt(i+nn-2*k-1) = tt(i)+per  ,i=1,2,...,2*k+1 */
/*       cc(i+nn-2*k-1) = cc(i)      ,i=1,2,...,k */

/*  calling sequence: */
/*     call insert(iopt,t,n,c,k,x,tt,nn,cc,nest,ier) */

/*  input parameters: */
/*    iopt : integer flag, specifying whether (iopt.ne.0) or not (iopt=0) */
/*           the given spline must be considered as being periodic. */
/*    t    : array,length nest, which contains the position of the knots. */
/*    n    : integer, giving the total number of knots of s(x). */
/*    c    : array,length nest, which contains the b-spline coefficients. */
/*    k    : integer, giving the degree of s(x). */
/*    x    : real, which gives the location of the knot to be inserted. */
/*    nest : integer specifying the dimension of the arrays t,c,tt and cc */
/*           nest > n. */

/*  output parameters: */
/*    tt   : array,length nest, which contains the position of the knots */
/*           after insertion. */
/*    nn   : integer, giving the total number of knots after insertion */
/*    cc   : array,length nest, which contains the b-spline coefficients */
/*           of s(x) with respect to the new set of knots. */
/*    ier  : error flag */
/*      ier = 0 : normal return */
/*      ier =10 : invalid input data (see restrictions) */

/*  restrictions: */
/*    nest > n */
/*    t(k+1) <= x <= t(n-k) */
/*    in case of a periodic spline (iopt.ne.0) there must be */
/*       either at least k interior knots t(j) satisfying t(k+1)<t(j)<=x */
/*       or at least k interior knots t(j) satisfying x<=t(j)<t(n-k) */

/*  other subroutines required: fpinst. */

/*  further comments: */
/*   subroutine insert may be called as follows */
/*        call insert(iopt,t,n,c,k,x,t,n,c,nest,ier) */
/*   in which case the new representation will simply replace the old one */

/*  references : */
/*    boehm w : inserting new knots into b-spline curves. computer aided */
/*              design 12 (1980) 199-201. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --cc;
    --tt;
    --c__;
    --t;

    /* Function Body */
    *ier = 10;
    if (*nest <= *n) {
	goto L40;
    }
    k1 = *k + 1;
    nk = *n - *k;
    if (*x < t[k1] || *x > t[nk]) {
	goto L40;
    }
/*  search for knot interval t(l) <= x < t(l+1). */
    nk1 = nk - 1;
    l = k1;
L10:
    if (*x < t[l + 1] || l == nk1) {
	goto L20;
    }
    ++l;
    goto L10;
L20:
    if (t[l] >= t[l + 1]) {
	goto L40;
    }
    if (*iopt == 0) {
	goto L30;
    }
    kk = *k << 1;
    if (l <= kk && l >= *n - kk) {
	goto L40;
    }
L30:
    *ier = 0;
/*  insert the new knot. */
    fpinst_(iopt, &t[1], n, &c__[1], k, x, &l, &tt[1], nn, &cc[1], nest);
L40:
    return 0;
} /* insert_ */

/* Subroutine */ int parcur_(iopt, ipar, idim, m, u, mx, x, w, ub, ue, k, s, 
	nest, n, t, nc, c__, fp, wrk, lwrk, iwrk, ier)
integer *iopt, *ipar, *idim, *m;
real *u;
integer *mx;
real *x, *w, *ub, *ue;
integer *k;
real *s;
integer *nest, *n;
real *t;
integer *nc;
real *c__, *fp, *wrk;
integer *lwrk, *iwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1;

    /* Builtin functions */
    double sqrt();

    /* Local variables */
    static integer nmin;
    static real dist;
    static integer i__, j, maxit, i1, i2, k1, k2, lwest, ia, ib, ig;
    extern /* Subroutine */ int fpchec_();
    static integer iq, iz;
    extern /* Subroutine */ int fppara_();
    static integer ncc, ifp;
    static real tol;

/*  given the ordered set of m points x(i) in the idim-dimensional space */
/*  and given also a corresponding set of strictly increasing values u(i) */
/*  and the set of positive numbers w(i),i=1,2,...,m, subroutine parcur */
/*  determines a smooth approximating spline curve s(u), i.e. */
/*      x1 = s1(u) */
/*      x2 = s2(u)       ub <= u <= ue */
/*      ......... */
/*      xidim = sidim(u) */
/*  with sj(u),j=1,2,...,idim spline functions of degree k with common */
/*  knots t(j),j=1,2,...,n. */
/*  if ipar=1 the values ub,ue and u(i),i=1,2,...,m must be supplied by */
/*  the user. if ipar=0 these values are chosen automatically by parcur */
/*  as  v(1) = 0 */
/*      v(i) = v(i-1) + dist(x(i),x(i-1)) ,i=2,3,...,m */
/*      u(i) = v(i)/v(m) ,i=1,2,...,m */
/*      ub = u(1) = 0, ue = u(m) = 1. */
/*  if iopt=-1 parcur calculates the weighted least-squares spline curve */
/*  according to a given set of knots. */
/*  if iopt>=0 the number of knots of the splines sj(u) and the position */
/*  t(j),j=1,2,...,n is chosen automatically by the routine. the smooth- */
/*  ness of s(u) is then achieved by minimalizing the discontinuity */
/*  jumps of the k-th derivative of s(u) at the knots t(j),j=k+2,k+3,..., */
/*  n-k-1. the amount of smoothness is determined by the condition that */
/*  f(p)=sum((w(i)*dist(x(i),s(u(i))))**2) be <= s, with s a given non- */
/*  negative constant, called the smoothing factor. */
/*  the fit s(u) is given in the b-spline representation and can be */
/*  evaluated by means of subroutine curev. */

/*  calling sequence: */
/*     call parcur(iopt,ipar,idim,m,u,mx,x,w,ub,ue,k,s,nest,n,t,nc,c, */
/*    * fp,wrk,lwrk,iwrk,ier) */

/*  parameters: */
/*   iopt  : integer flag. on entry iopt must specify whether a weighted */
/*           least-squares spline curve (iopt=-1) or a smoothing spline */
/*           curve (iopt=0 or 1) must be determined.if iopt=0 the routine */
/*           will start with an initial set of knots t(i)=ub,t(i+k+1)=ue, */
/*           i=1,2,...,k+1. if iopt=1 the routine will continue with the */
/*           knots found at the last call of the routine. */
/*           attention: a call with iopt=1 must always be immediately */
/*           preceded by another call with iopt=1 or iopt=0. */
/*           unchanged on exit. */
/*   ipar  : integer flag. on entry ipar must specify whether (ipar=1) */
/*           the user will supply the parameter values u(i),ub and ue */
/*           or whether (ipar=0) these values are to be calculated by */
/*           parcur. unchanged on exit. */
/*   idim  : integer. on entry idim must specify the dimension of the */
/*           curve. 0 < idim < 11. */
/*           unchanged on exit. */
/*   m     : integer. on entry m must specify the number of data points. */
/*           m > k. unchanged on exit. */
/*   u     : real array of dimension at least (m). in case ipar=1,before */
/*           entry, u(i) must be set to the i-th value of the parameter */
/*           variable u for i=1,2,...,m. these values must then be */
/*           supplied in strictly ascending order and will be unchanged */
/*           on exit. in case ipar=0, on exit,array u will contain the */
/*           values u(i) as determined by parcur. */
/*   mx    : integer. on entry mx must specify the actual dimension of */
/*           the array x as declared in the calling (sub)program. mx must */
/*           not be too small (see x). unchanged on exit. */
/*   x     : real array of dimension at least idim*m. */
/*           before entry, x(idim*(i-1)+j) must contain the j-th coord- */
/*           inate of the i-th data point for i=1,2,...,m and j=1,2,..., */
/*           idim. unchanged on exit. */
/*   w     : real array of dimension at least (m). before entry, w(i) */
/*           must be set to the i-th value in the set of weights. the */
/*           w(i) must be strictly positive. unchanged on exit. */
/*           see also further comments. */
/*   ub,ue : real values. on entry (in case ipar=1) ub and ue must */
/*           contain the lower and upper bound for the parameter u. */
/*           ub <=u(1), ue>= u(m). if ipar = 0 these values will */
/*           automatically be set to 0 and 1 by parcur. */
/*   k     : integer. on entry k must specify the degree of the splines. */
/*           1<=k<=5. it is recommended to use cubic splines (k=3). */
/*           the user is strongly dissuaded from choosing k even,together */
/*           with a small s-value. unchanged on exit. */
/*   s     : real.on entry (in case iopt>=0) s must specify the smoothing */
/*           factor. s >=0. unchanged on exit. */
/*           for advice on the choice of s see further comments. */
/*   nest  : integer. on entry nest must contain an over-estimate of the */
/*           total number of knots of the splines returned, to indicate */
/*           the storage space available to the routine. nest >=2*k+2. */
/*           in most practical situation nest=m/2 will be sufficient. */
/*           always large enough is nest=m+k+1, the number of knots */
/*           needed for interpolation (s=0). unchanged on exit. */
/*   n     : integer. */
/*           unless ier = 10 (in case iopt >=0), n will contain the */
/*           total number of knots of the smoothing spline curve returned */
/*           if the computation mode iopt=1 is used this value of n */
/*           should be left unchanged between subsequent calls. */
/*           in case iopt=-1, the value of n must be specified on entry. */
/*   t     : real array of dimension at least (nest). */
/*           on succesful exit, this array will contain the knots of the */
/*           spline curve,i.e. the position of the interior knots t(k+2), */
/*           t(k+3),..,t(n-k-1) as well as the position of the additional */
/*           t(1)=t(2)=...=t(k+1)=ub and t(n-k)=...=t(n)=ue needed for */
/*           the b-spline representation. */
/*           if the computation mode iopt=1 is used, the values of t(1), */
/*           t(2),...,t(n) should be left unchanged between subsequent */
/*           calls. if the computation mode iopt=-1 is used, the values */
/*           t(k+2),...,t(n-k-1) must be supplied by the user, before */
/*           entry. see also the restrictions (ier=10). */
/*   nc    : integer. on entry nc must specify the actual dimension of */
/*           the array c as declared in the calling (sub)program. nc */
/*           must not be too small (see c). unchanged on exit. */
/*   c     : real array of dimension at least (nest*idim). */
/*           on succesful exit, this array will contain the coefficients */
/*           in the b-spline representation of the spline curve s(u),i.e. */
/*           the b-spline coefficients of the spline sj(u) will be given */
/*           in c(n*(j-1)+i),i=1,2,...,n-k-1 for j=1,2,...,idim. */
/*   fp    : real. unless ier = 10, fp contains the weighted sum of */
/*           squared residuals of the spline curve returned. */
/*   wrk   : real array of dimension at least m*(k+1)+nest*(6+idim+3*k). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used, the values wrk(1),...,wrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   lwrk  : integer. on entry,lwrk must specify the actual dimension of */
/*           the array wrk as declared in the calling (sub)program. lwrk */
/*           must not be too small (see wrk). unchanged on exit. */
/*   iwrk  : integer array of dimension at least (nest). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used,the values iwrk(1),...,iwrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   ier   : integer. unless the routine detects an error, ier contains a */
/*           non-positive value on exit, i.e. */
/*    ier=0  : normal return. the curve returned has a residual sum of */
/*             squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*             ive tolerance set to 0.001 by the program. */
/*    ier=-1 : normal return. the curve returned is an interpolating */
/*             spline curve (fp=0). */
/*    ier=-2 : normal return. the curve returned is the weighted least- */
/*             squares polynomial curve of degree k.in this extreme case */
/*             fp gives the upper bound fp0 for the smoothing factor s. */
/*    ier=1  : error. the required storage space exceeds the available */
/*             storage space, as specified by the parameter nest. */
/*             probably causes : nest too small. if nest is already */
/*             large (say nest > m/2), it may also indicate that s is */
/*             too small */
/*             the approximation returned is the least-squares spline */
/*             curve according to the knots t(1),t(2),...,t(n). (n=nest) */
/*             the parameter fp gives the corresponding weighted sum of */
/*             squared residuals (fp>s). */
/*    ier=2  : error. a theoretically impossible result was found during */
/*             the iteration proces for finding a smoothing spline curve */
/*             with fp = s. probably causes : s too small. */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*             by the program) allowed for finding a smoothing curve */
/*             with fp=s has been reached. probably causes : s too small */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=10 : error. on entry, the input data are controlled on validity */
/*             the following restrictions must be satisfied. */
/*             -1<=iopt<=1, 1<=k<=5, m>k, nest>2*k+2, w(i)>0,i=1,2,...,m */
/*             0<=ipar<=1, 0<idim<=10, lwrk>=(k+1)*m+nest*(6+idim+3*k), */
/*             nc>=nest*idim */
/*             if ipar=0: sum j=1,idim (x(idim*i+j)-x(idim*(i-1)+j))**2>0 */
/*                        i=1,2,...,m-1. */
/*             if ipar=1: ub<=u(1)<u(2)<...<u(m)<=ue */
/*             if iopt=-1: 2*k+2<=n<=min(nest,m+k+1) */
/*                         ub<t(k+2)<t(k+3)<...<t(n-k-1)<ue */
/*                            (ub=0 and ue=1 in case ipar=0) */
/*                       the schoenberg-whitney conditions, i.e. there */
/*                       must be a subset of data points uu(j) such that */
/*                         t(j) < uu(j) < t(j+k+1), j=1,2,...,n-k-1 */
/*             if iopt>=0: s>=0 */
/*                         if s=0 : nest >= m+k+1 */
/*             if one of these conditions is found to be violated,control */
/*             is immediately repassed to the calling program. in that */
/*             case there is no approximation returned. */

/*  further comments: */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the curve will be too smooth and signal will be */
/*   lost ; if s is too small the curve will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating curve if */
/*   s=0 and the least-squares polynomial curve of degree k if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the weights w(i). if these are */
/*   taken as 1/d(i) with d(i) an estimate of the standard deviation of */
/*   x(i), a good s-value should be found in the range (m-sqrt(2*m),m+ */
/*   sqrt(2*m)). if nothing is known about the statistical error in x(i) */
/*   each w(i) can be set equal to one and s determined by trial and */
/*   error, taking account of the comments above. the best is then to */
/*   start with a very large value of s ( to determine the least-squares */
/*   polynomial curve and the upper bound fp0 for s) and then to */
/*   progressively decrease the value of s ( say by a factor 10 in the */
/*   beginning, i.e. s=fp0/10, fp0/100,...and more carefully as the */
/*   approximating curve shows more detail) to obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt=1 the program will continue with the set of knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if parcur is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   curve underlying the data. but, if the computation mode iopt=1 is */
/*   used, the knots returned may also depend on the s-values at previous */
/*   calls (if these were smaller). therefore, if after a number of */
/*   trials with different s-values and iopt=1, the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   parcur once more with the selected value for s but now with iopt=0. */
/*   indeed, parcur may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */

/*   the form of the approximating curve can strongly be affected by */
/*   the choice of the parameter values u(i). if there is no physical */
/*   reason for choosing a particular parameter u, often good results */
/*   will be obtained with the choice of parcur (in case ipar=0), i.e. */
/*        v(1)=0, v(i)=v(i-1)+q(i), i=2,...,m, u(i)=v(i)/v(m), i=1,..,m */
/*   where */
/*        q(i)= sqrt(sum j=1,idim (xj(i)-xj(i-1))**2 ) */
/*   other possibilities for q(i) are */
/*        q(i)= sum j=1,idim (xj(i)-xj(i-1))**2 */
/*        q(i)= sum j=1,idim abs(xj(i)-xj(i-1)) */
/*        q(i)= max j=1,idim abs(xj(i)-xj(i-1)) */
/*        q(i)= 1 */

/*  other subroutines required: */
/*    fpback,fpbspl,fpchec,fppara,fpdisc,fpgivs,fpknot,fprati,fprota */

/*  references: */
/*   dierckx p. : algorithms for smoothing data with periodic and */
/*                parametric splines, computer graphics and image */
/*                processing 20 (1982) 171-184. */
/*   dierckx p. : algorithms for smoothing data with periodic and param- */
/*                etric splines, report tw55, dept. computer science, */
/*                k.u.leuven, 1981. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : may 1979 */
/*  latest update : march 1987 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/* ..function references */
/*  .. */
/*  we set up the parameters tol and maxit */
    /* Parameter adjustments */
    --w;
    --u;
    --x;
    --iwrk;
    --t;
    --c__;
    --wrk;

    /* Function Body */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*iopt < -1 || *iopt > 1) {
	goto L90;
    }
    if (*ipar < 0 || *ipar > 1) {
	goto L90;
    }
    if (*idim <= 0 || *idim > 10) {
	goto L90;
    }
    if (*k <= 0 || *k > 5) {
	goto L90;
    }
    k1 = *k + 1;
    k2 = k1 + 1;
    nmin = k1 << 1;
    if (*m < k1 || *nest < nmin) {
	goto L90;
    }
    ncc = *nest * *idim;
    if (*mx < *m * *idim || *nc < ncc) {
	goto L90;
    }
    lwest = *m * k1 + *nest * (*idim + 6 + *k * 3);
    if (*lwrk < lwest) {
	goto L90;
    }
    if (*ipar != 0 || *iopt > 0) {
	goto L40;
    }
    i1 = 0;
    i2 = *idim;
    u[1] = (float)0.;
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	dist = (float)0.;
	i__2 = *idim;
	for (j = 1; j <= i__2; ++j) {
	    ++i1;
	    ++i2;
/* Computing 2nd power */
	    r__1 = x[i2] - x[i1];
	    dist += r__1 * r__1;
/* L10: */
	}
	u[i__] = u[i__ - 1] + sqrt(dist);
/* L20: */
    }
    if (u[*m] <= (float)0.) {
	goto L90;
    }
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	u[i__] /= u[*m];
/* L30: */
    }
    *ub = (float)0.;
    *ue = (float)1.;
    u[*m] = *ue;
L40:
    if (*ub > u[1] || *ue < u[*m] || w[1] <= (float)0.) {
	goto L90;
    }
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (u[i__ - 1] >= u[i__] || w[i__] <= (float)0.) {
	    goto L90;
	}
/* L50: */
    }
    if (*iopt >= 0) {
	goto L70;
    }
    if (*n < nmin || *n > *nest) {
	goto L90;
    }
    j = *n;
    i__1 = k1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t[i__] = *ub;
	t[j] = *ue;
	--j;
/* L60: */
    }
    fpchec_(&u[1], m, &t[1], n, k, ier);
    if (*ier != 0) {
	goto L90;
    } else {
	goto L80;
    }
L70:
    if (*s < (float)0.) {
	goto L90;
    }
    if (*s == (float)0. && *nest < *m + k1) {
	goto L90;
    }
    *ier = 0;
/* we partition the working space and determine the spline curve. */
L80:
    ifp = 1;
    iz = ifp + *nest;
    ia = iz + ncc;
    ib = ia + *nest * k1;
    ig = ib + *nest * k2;
    iq = ig + *nest * k2;
    fppara_(iopt, idim, m, &u[1], mx, &x[1], &w[1], ub, ue, k, s, nest, &tol, 
	    &maxit, &k1, &k2, n, &t[1], &ncc, &c__[1], fp, &wrk[ifp], &wrk[iz]
	    , &wrk[ia], &wrk[ib], &wrk[ig], &wrk[iq], &iwrk[1], ier);
L90:
    return 0;
} /* parcur_ */

/* Subroutine */ int parder_(tx, nx, ty, ny, c__, kx, ky, nux, nuy, x, mx, y, 
	my, z__, wrk, lwrk, iwrk, kwrk, ier)
real *tx;
integer *nx;
real *ty;
integer *ny;
real *c__;
integer *kx, *ky, *nux, *nuy;
real *x;
integer *mx;
real *y;
integer *my;
real *z__, *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, m, l1, lwest, l2, m0, m1;
    static real ak;
    static integer nc, lx, ly;
    extern /* Subroutine */ int fpbisp_();
    static integer kx1, ky1;
    static real fac;
    static integer kkx, kky, iwx, iwy, nxx, nyy, nkx1, nky1;

/*  subroutine parder evaluates on a grid (x(i),y(j)),i=1,...,mx; j=1,... */
/*  ,my the partial derivative ( order nux,nuy) of a bivariate spline */
/*  s(x,y) of degrees kx and ky, given in the b-spline representation. */

/*  calling sequence: */
/*     call parder(tx,nx,ty,ny,c,kx,ky,nux,nuy,x,mx,y,my,z,wrk,lwrk, */
/*    * iwrk,kwrk,ier) */

/*  input parameters: */
/*   tx    : real array, length nx, which contains the position of the */
/*           knots in the x-direction. */
/*   nx    : integer, giving the total number of knots in the x-direction */
/*   ty    : real array, length ny, which contains the position of the */
/*           knots in the y-direction. */
/*   ny    : integer, giving the total number of knots in the y-direction */
/*   c     : real array, length (nx-kx-1)*(ny-ky-1), which contains the */
/*           b-spline coefficients. */
/*   kx,ky : integer values, giving the degrees of the spline. */
/*   nux   : integer values, specifying the order of the partial */
/*   nuy     derivative. 0<=nux<kx, 0<=nuy<ky. */
/*   x     : real array of dimension (mx). */
/*           before entry x(i) must be set to the x co-ordinate of the */
/*           i-th grid point along the x-axis. */
/*           tx(kx+1)<=x(i-1)<=x(i)<=tx(nx-kx), i=2,...,mx. */
/*   mx    : on entry mx must specify the number of grid points along */
/*           the x-axis. mx >=1. */
/*   y     : real array of dimension (my). */
/*           before entry y(j) must be set to the y co-ordinate of the */
/*           j-th grid point along the y-axis. */
/*           ty(ky+1)<=y(j-1)<=y(j)<=ty(ny-ky), j=2,...,my. */
/*   my    : on entry my must specify the number of grid points along */
/*           the y-axis. my >=1. */
/*   wrk   : real array of dimension lwrk. used as workspace. */
/*   lwrk  : integer, specifying the dimension of wrk. */
/*           lwrk >= mx*(kx+1-nux)+my*(ky+1-nuy)+(nx-kx-1)*(ny-ky-1) */
/*   iwrk  : integer array of dimension kwrk. used as workspace. */
/*   kwrk  : integer, specifying the dimension of iwrk. kwrk >= mx+my. */

/*  output parameters: */
/*   z     : real array of dimension (mx*my). */
/*           on succesful exit z(my*(i-1)+j) contains the value of the */
/*           specified partial derivative of s(x,y) at the point */
/*           (x(i),y(j)),i=1,...,mx;j=1,...,my. */
/*   ier   : integer error flag */
/*    ier=0 : normal return */
/*    ier=10: invalid input data (see restrictions) */

/*  restrictions: */
/*   mx >=1, my >=1, 0 <= nux < kx, 0 <= nuy < ky, kwrk>=mx+my */
/*   lwrk>=mx*(kx+1-nux)+my*(ky+1-nuy)+(nx-kx-1)*(ny-ky-1), */
/*   tx(kx+1) <= x(i-1) <= x(i) <= tx(nx-kx), i=2,...,mx */
/*   ty(ky+1) <= y(j-1) <= y(j) <= ty(ny-ky), j=2,...,my */

/*  other subroutines required: */
/*    fpbisp,fpbspl */

/*  references : */
/*    de boor c : on calculating with b-splines, j. approximation theory */
/*                6 (1972) 50-62. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1989 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --tx;
    --ty;
    --c__;
    --x;
    --z__;
    --y;
    --wrk;
    --iwrk;

    /* Function Body */
    *ier = 10;
    kx1 = *kx + 1;
    ky1 = *ky + 1;
    nkx1 = *nx - kx1;
    nky1 = *ny - ky1;
    nc = nkx1 * nky1;
    if (*nux < 0 || *nux >= *kx) {
	goto L400;
    }
    if (*nuy < 0 || *nuy >= *ky) {
	goto L400;
    }
    lwest = nc + (kx1 - *nux) * *mx + (ky1 - *nuy) * *my;
    if (*lwrk < lwest) {
	goto L400;
    }
    if (*kwrk < *mx + *my) {
	goto L400;
    }
    if ((i__1 = *mx - 1) < 0) {
	goto L400;
    } else if (i__1 == 0) {
	goto L30;
    } else {
	goto L10;
    }
L10:
    i__1 = *mx;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (x[i__] < x[i__ - 1]) {
	    goto L400;
	}
/* L20: */
    }
L30:
    if ((i__1 = *my - 1) < 0) {
	goto L400;
    } else if (i__1 == 0) {
	goto L60;
    } else {
	goto L40;
    }
L40:
    i__1 = *my;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (y[i__] < y[i__ - 1]) {
	    goto L400;
	}
/* L50: */
    }
L60:
    *ier = 0;
    nxx = nkx1;
    nyy = nky1;
    kkx = *kx;
    kky = *ky;
/*  the partial derivative of order (nux,nuy) of a bivariate spline of */
/*  degrees kx,ky is a bivariate spline of degrees kx-nux,ky-nuy. */
/*  we calculate the b-spline coefficients of this spline */
    i__1 = nc;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wrk[i__] = c__[i__];
/* L70: */
    }
    if (*nux == 0) {
	goto L200;
    }
    lx = 1;
    i__1 = *nux;
    for (j = 1; j <= i__1; ++j) {
	ak = (real) kkx;
	--nxx;
	l1 = lx;
	m0 = 1;
	i__2 = nxx;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ++l1;
	    l2 = l1 + kkx;
	    fac = tx[l2] - tx[l1];
	    if (fac <= (float)0.) {
		goto L90;
	    }
	    i__3 = nyy;
	    for (m = 1; m <= i__3; ++m) {
		m1 = m0 + nyy;
		wrk[m0] = (wrk[m1] - wrk[m0]) * ak / fac;
		++m0;
/* L80: */
	    }
L90:
	    ;
	}
	++lx;
	--kkx;
/* L100: */
    }
L200:
    if (*nuy == 0) {
	goto L300;
    }
    ly = 1;
    i__1 = *nuy;
    for (j = 1; j <= i__1; ++j) {
	ak = (real) kky;
	--nyy;
	l1 = ly;
	i__2 = nyy;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ++l1;
	    l2 = l1 + kky;
	    fac = ty[l2] - ty[l1];
	    if (fac <= (float)0.) {
		goto L220;
	    }
	    m0 = i__;
	    i__3 = nxx;
	    for (m = 1; m <= i__3; ++m) {
		m1 = m0 + 1;
		wrk[m0] = (wrk[m1] - wrk[m0]) * ak / fac;
		m0 += nky1;
/* L210: */
	    }
L220:
	    ;
	}
	++ly;
	--kky;
/* L230: */
    }
    m0 = nyy;
    m1 = nky1;
    i__1 = nxx;
    for (m = 2; m <= i__1; ++m) {
	i__2 = nyy;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ++m0;
	    ++m1;
	    wrk[m0] = wrk[m1];
/* L240: */
	}
	m1 += *nuy;
/* L250: */
    }
/*  we partition the working space and evaluate the partial derivative */
L300:
    iwx = nxx * nyy + 1;
    iwy = iwx + *mx * (kx1 - *nux);
    i__1 = *nx - (*nux << 1);
    i__2 = *ny - (*nuy << 1);
    fpbisp_(&tx[*nux + 1], &i__1, &ty[*nuy + 1], &i__2, &wrk[1], &kkx, &kky, &
	    x[1], mx, &y[1], my, &z__[1], &wrk[iwx], &wrk[iwy], &iwrk[1], &
	    iwrk[*mx + 1]);
L400:
    return 0;
} /* parder_ */

/* Subroutine */ int parsur_(iopt, ipar, idim, mu, u, mv, v, f, s, nuest, 
	nvest, nu, tu, nv, tv, c__, fp, wrk, lwrk, iwrk, kwrk, ier)
integer *iopt, *ipar, *idim, *mu;
real *u;
integer *mv;
real *v, *f, *s;
integer *nuest, *nvest, *nu;
real *tu;
integer *nv;
real *tv, *c__, *fp, *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer kndu, kndv, lfpu, lfpv;
    static real peru, perv;
    static integer jwrk, knru, knrv, i__, j, maxit, mumin, mvmin, kwest, l1, 
	    l2, l3, l4, lwest, nc, mf;
    static real ub, vb;
    extern /* Subroutine */ int fpchec_();
    static real ue, ve;
    extern /* Subroutine */ int fpchep_(), fppasu_();
    static real tol;
    static integer lww;

/*  given the set of ordered points f(i,j) in the idim-dimensional space, */
/*  corresponding to grid values (u(i),v(j)) ,i=1,...,mu ; j=1,...,mv, */
/*  parsur determines a smooth approximating spline surface s(u,v) , i.e. */
/*    f1 = s1(u,v) */
/*      ...                u(1) <= u <= u(mu) ; v(1) <= v <= v(mv) */
/*    fidim = sidim(u,v) */
/*  with sl(u,v), l=1,2,...,idim bicubic spline functions with common */
/*  knots tu(i),i=1,...,nu in the u-variable and tv(j),j=1,...,nv in the */
/*  v-variable. */
/*  in addition, these splines will be periodic in the variable u if */
/*  ipar(1) = 1 and periodic in the variable v if ipar(2) = 1. */
/*  if iopt=-1, parsur determines the least-squares bicubic spline */
/*  surface according to a given set of knots. */
/*  if iopt>=0, the number of knots of s(u,v) and their position */
/*  is chosen automatically by the routine. the smoothness of s(u,v) is */
/*  achieved by minimalizing the discontinuity jumps of the derivatives */
/*  of the splines at the knots. the amount of smoothness of s(u,v) is */
/*  determined by the condition that */
/*  fp=sumi=1,mu(sumj=1,mv(dist(f(i,j)-s(u(i),v(j)))**2))<=s, */
/*  with s a given non-negative constant. */
/*  the fit s(u,v) is given in its b-spline representation and can be */
/*  evaluated by means of routine surev. */

/* calling sequence: */
/*     call parsur(iopt,ipar,idim,mu,u,mv,v,f,s,nuest,nvest,nu,tu, */
/*    *  nv,tv,c,fp,wrk,lwrk,iwrk,kwrk,ier) */

/* parameters: */
/*  iopt  : integer flag. unchanged on exit. */
/*          on entry iopt must specify whether a least-squares surface */
/*          (iopt=-1) or a smoothing surface (iopt=0 or 1)must be */
/*          determined. */
/*          if iopt=0 the routine will start with the initial set of */
/*          knots needed for determining the least-squares polynomial */
/*          surface. */
/*          if iopt=1 the routine will continue with the set of knots */
/*          found at the last call of the routine. */
/*          attention: a call with iopt=1 must always be immediately */
/*          preceded by another call with iopt = 1 or iopt = 0. */
/*  ipar  : integer array of dimension 2. unchanged on exit. */
/*          on entry ipar(1) must specify whether (ipar(1)=1) or not */
/*          (ipar(1)=0) the splines must be periodic in the variable u. */
/*          on entry ipar(2) must specify whether (ipar(2)=1) or not */
/*          (ipar(2)=0) the splines must be periodic in the variable v. */
/*  idim  : integer. on entry idim must specify the dimension of the */
/*          surface. 1 <= idim <= 3. unchanged on exit. */
/*  mu    : integer. on entry mu must specify the number of grid points */
/*          along the u-axis. unchanged on exit. */
/*          mu >= mumin where mumin=4-2*ipar(1) */
/*  u     : real array of dimension at least (mu). before entry, u(i) */
/*          must be set to the u-co-ordinate of the i-th grid point */
/*          along the u-axis, for i=1,2,...,mu. these values must be */
/*          supplied in strictly ascending order. unchanged on exit. */
/*  mv    : integer. on entry mv must specify the number of grid points */
/*          along the v-axis. unchanged on exit. */
/*          mv >= mvmin where mvmin=4-2*ipar(2) */
/*  v     : real array of dimension at least (mv). before entry, v(j) */
/*          must be set to the v-co-ordinate of the j-th grid point */
/*          along the v-axis, for j=1,2,...,mv. these values must be */
/*          supplied in strictly ascending order. unchanged on exit. */
/*  f     : real array of dimension at least (mu*mv*idim). */
/*          before entry, f(mu*mv*(l-1)+mv*(i-1)+j) must be set to the */
/*          l-th co-ordinate of the data point corresponding to the */
/*          the grid point (u(i),v(j)) for l=1,...,idim ,i=1,...,mu */
/*          and j=1,...,mv. unchanged on exit. */
/*          if ipar(1)=1 it is expected that f(mu*mv*(l-1)+mv*(mu-1)+j) */
/*          = f(mu*mv*(l-1)+j), l=1,...,idim ; j=1,...,mv */
/*          if ipar(2)=1 it is expected that f(mu*mv*(l-1)+mv*(i-1)+mv) */
/*          = f(mu*mv*(l-1)+mv*(i-1)+1), l=1,...,idim ; i=1,...,mu */
/*  s     : real. on entry (if iopt>=0) s must specify the smoothing */
/*          factor. s >=0. unchanged on exit. */
/*          for advice on the choice of s see further comments */
/*  nuest : integer. unchanged on exit. */
/*  nvest : integer. unchanged on exit. */
/*          on entry, nuest and nvest must specify an upper bound for the */
/*          number of knots required in the u- and v-directions respect. */
/*          these numbers will also determine the storage space needed by */
/*          the routine. nuest >= 8, nvest >= 8. */
/*          in most practical situation nuest = mu/2, nvest=mv/2, will */
/*          be sufficient. always large enough are nuest=mu+4+2*ipar(1), */
/*          nvest = mv+4+2*ipar(2), the number of knots needed for */
/*          interpolation (s=0). see also further comments. */
/*  nu    : integer. */
/*          unless ier=10 (in case iopt>=0), nu will contain the total */
/*          number of knots with respect to the u-variable, of the spline */
/*          surface returned. if the computation mode iopt=1 is used, */
/*          the value of nu should be left unchanged between subsequent */
/*          calls. in case iopt=-1, the value of nu should be specified */
/*          on entry. */
/*  tu    : real array of dimension at least (nuest). */
/*          on succesful exit, this array will contain the knots of the */
/*          splines with respect to the u-variable, i.e. the position of */
/*          the interior knots tu(5),...,tu(nu-4) as well as the position */
/*          of the additional knots tu(1),...,tu(4) and tu(nu-3),..., */
/*          tu(nu) needed for the b-spline representation. */
/*          if the computation mode iopt=1 is used,the values of tu(1) */
/*          ...,tu(nu) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt=-1 is used, the values tu(5), */
/*          ...tu(nu-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  nv    : integer. */
/*          unless ier=10 (in case iopt>=0), nv will contain the total */
/*          number of knots with respect to the v-variable, of the spline */
/*          surface returned. if the computation mode iopt=1 is used, */
/*          the value of nv should be left unchanged between subsequent */
/*          calls. in case iopt=-1, the value of nv should be specified */
/*          on entry. */
/*  tv    : real array of dimension at least (nvest). */
/*          on succesful exit, this array will contain the knots of the */
/*          splines with respect to the v-variable, i.e. the position of */
/*          the interior knots tv(5),...,tv(nv-4) as well as the position */
/*          of the additional knots tv(1),...,tv(4) and tv(nv-3),..., */
/*          tv(nv) needed for the b-spline representation. */
/*          if the computation mode iopt=1 is used,the values of tv(1) */
/*          ...,tv(nv) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt=-1 is used, the values tv(5), */
/*          ...tv(nv-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  c     : real array of dimension at least (nuest-4)*(nvest-4)*idim. */
/*          on succesful exit, c contains the coefficients of the spline */
/*          approximation s(u,v) */
/*  fp    : real. unless ier=10, fp contains the sum of squared */
/*          residuals of the spline surface returned. */
/*  wrk   : real array of dimension (lwrk). used as workspace. */
/*          if the computation mode iopt=1 is used the values of */
/*          wrk(1),...,wrk(4) should be left unchanged between subsequent */
/*          calls. */
/*  lwrk  : integer. on entry lwrk must specify the actual dimension of */
/*          the array wrk as declared in the calling (sub)program. */
/*          lwrk must not be too small. */
/*           lwrk >= 4+nuest*(mv*idim+11+4*ipar(1))+nvest*(11+4*ipar(2))+ */
/*           4*(mu+mv)+q*idim where q is the larger of mv and nuest. */
/*  iwrk  : integer array of dimension (kwrk). used as workspace. */
/*          if the computation mode iopt=1 is used the values of */
/*          iwrk(1),.,iwrk(3) should be left unchanged between subsequent */
/*          calls. */
/*  kwrk  : integer. on entry kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. */
/*          kwrk >= 3+mu+mv+nuest+nvest. */
/*  ier   : integer. unless the routine detects an error, ier contains a */
/*          non-positive value on exit, i.e. */
/*   ier=0  : normal return. the surface returned has a residual sum of */
/*            squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*            ive tolerance set to 0.001 by the program. */
/*   ier=-1 : normal return. the spline surface returned is an */
/*            interpolating surface (fp=0). */
/*   ier=-2 : normal return. the surface returned is the least-squares */
/*            polynomial surface. in this extreme case fp gives the */
/*            upper bound for the smoothing factor s. */
/*   ier=1  : error. the required storage space exceeds the available */
/*            storage space, as specified by the parameters nuest and */
/*            nvest. */
/*            probably causes : nuest or nvest too small. if these param- */
/*            eters are already large, it may also indicate that s is */
/*            too small */
/*            the approximation returned is the least-squares surface */
/*            according to the current set of knots. the parameter fp */
/*            gives the corresponding sum of squared residuals (fp>s). */
/*   ier=2  : error. a theoretically impossible result was found during */
/*            the iteration proces for finding a smoothing surface with */
/*            fp = s. probably causes : s too small. */
/*            there is an approximation returned but the corresponding */
/*            sum of squared residuals does not satisfy the condition */
/*            abs(fp-s)/s < tol. */
/*   ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*            by the program) allowed for finding a smoothing surface */
/*            with fp=s has been reached. probably causes : s too small */
/*            there is an approximation returned but the corresponding */
/*            sum of squared residuals does not satisfy the condition */
/*            abs(fp-s)/s < tol. */
/*   ier=10 : error. on entry, the input data are controlled on validity */
/*            the following restrictions must be satisfied. */
/*            -1<=iopt<=1, 0<=ipar(1)<=1, 0<=ipar(2)<=1, 1 <=idim<=3 */
/*            mu >= 4-2*ipar(1),mv >= 4-2*ipar(2), nuest >=8, nvest >= 8, */
/*            kwrk>=3+mu+mv+nuest+nvest, */
/*            lwrk >= 4+nuest*(mv*idim+11+4*ipar(1))+nvest*(11+4*ipar(2)) */
/*             +4*(mu+mv)+max(nuest,mv)*idim */
/*            u(i-1)<u(i),i=2,..,mu, v(i-1)<v(i),i=2,...,mv */
/*            if iopt=-1: 8<=nu<=min(nuest,mu+4+2*ipar(1)) */
/*                        u(1)<tu(5)<tu(6)<...<tu(nu-4)<u(mu) */
/*                        8<=nv<=min(nvest,mv+4+2*ipar(2)) */
/*                        v(1)<tv(5)<tv(6)<...<tv(nv-4)<v(mv) */
/*                    the schoenberg-whitney conditions, i.e. there must */
/*                    be subset of grid co-ordinates uu(p) and vv(q) such */
/*                    that   tu(p) < uu(p) < tu(p+4) ,p=1,...,nu-4 */
/*                           tv(q) < vv(q) < tv(q+4) ,q=1,...,nv-4 */
/*                     (see fpchec or fpchep) */
/*            if iopt>=0: s>=0 */
/*                       if s=0: nuest>=mu+4+2*ipar(1) */
/*                               nvest>=mv+4+2*ipar(2) */
/*            if one of these conditions is found to be violated,control */
/*            is immediately repassed to the calling program. in that */
/*            case there is no approximation returned. */

/* further comments: */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the surface will be too smooth and signal will be */
/*   lost ; if s is too small the surface will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating surface */
/*   if s=0 and the constrained least-squares polynomial surface if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the accuracy of the data values. */
/*   if the user has an idea of the statistical errors on the data, he */
/*   can also find a proper estimate for s. for, by assuming that, if he */
/*   specifies the right s, parsur will return a surface s(u,v) which */
/*   exactly reproduces the surface underlying the data he can evaluate */
/*   the sum(dist(f(i,j)-s(u(i),v(j)))**2) to find a good estimate for s. */
/*   for example, if he knows that the statistical errors on his f(i,j)- */
/*   values is not greater than 0.1, he may expect that a good s should */
/*   have a value not larger than mu*mv*(0.1)**2. */
/*   if nothing is known about the statistical error in f(i,j), s must */
/*   be determined by trial and error, taking account of the comments */
/*   above. the best is then to start with a very large value of s (to */
/*   determine the le-sq polynomial surface and the corresponding upper */
/*   bound fp0 for s) and then to progressively decrease the value of s */
/*   ( say by a factor 10 in the beginning, i.e. s=fp0/10,fp0/100,... */
/*   and more carefully as the approximation shows more detail) to */
/*   obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt = 1 the program will continue with the knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if parsur is called repeatedly for different values of s. */
/*   the number of knots of the surface returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   surface underlying the data. if the computation mode iopt = 1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt=1,the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   parsur once more with the chosen value for s but now with iopt=0. */
/*   indeed, parsur may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */
/*   the number of knots may also depend on the upper bounds nuest and */
/*   nvest. indeed, if at a certain stage in parsur the number of knots */
/*   in one direction (say nu) has reached the value of its upper bound */
/*   (nuest), then from that moment on all subsequent knots are added */
/*   in the other (v) direction. this may indicate that the value of */
/*   nuest is too small. on the other hand, it gives the user the option */
/*   of limiting the number of knots the routine locates in any direction */
/*   for example, by setting nuest=8 (the lowest allowable value for */
/*   nuest), the user can indicate that he wants an approximation with */
/*   splines which are simple cubic polynomials in the variable u. */

/*  other subroutines required: */
/*    fppasu,fpchec,fpchep,fpknot,fprati,fpgrpa,fptrnp,fpback, */
/*    fpbacp,fpbspl,fptrpe,fpdisc,fpgivs,fprota */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1989 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fppasu,fpchec,fpchep */
/*  .. */
/*  we set up the parameters tol and maxit. */
    /* Parameter adjustments */
    --ipar;
    --u;
    --f;
    --v;
    --tu;
    --c__;
    --tv;
    --wrk;
    --iwrk;

    /* Function Body */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*iopt < -1 || *iopt > 1) {
	goto L200;
    }
    if (ipar[1] < 0 || ipar[1] > 1) {
	goto L200;
    }
    if (ipar[2] < 0 || ipar[2] > 1) {
	goto L200;
    }
    if (*idim <= 0 || *idim > 3) {
	goto L200;
    }
    mumin = 4 - (ipar[1] << 1);
    if (*mu < mumin || *nuest < 8) {
	goto L200;
    }
    mvmin = 4 - (ipar[2] << 1);
    if (*mv < mvmin || *nvest < 8) {
	goto L200;
    }
    mf = *mu * *mv;
    nc = (*nuest - 4) * (*nvest - 4);
    lwest = *nuest * (*mv * *idim + 11 + (ipar[1] << 2)) + 4 + *nvest * ((
	    ipar[2] << 2) + 11) + (*mu + *mv << 2) + max(*nuest,*mv) * *idim;
    kwest = *mu + 3 + *mv + *nuest + *nvest;
    if (*lwrk < lwest || *kwrk < kwest) {
	goto L200;
    }
    i__1 = *mu;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (u[i__ - 1] >= u[i__]) {
	    goto L200;
	}
/* L10: */
    }
    i__1 = *mv;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (v[i__ - 1] >= v[i__]) {
	    goto L200;
	}
/* L20: */
    }
    if (*iopt >= 0) {
	goto L100;
    }
    if (*nu < 8 || *nu > *nuest) {
	goto L200;
    }
    ub = u[1];
    ue = u[*mu];
    if (ipar[1] != 0) {
	goto L40;
    }
    j = *nu;
    for (i__ = 1; i__ <= 4; ++i__) {
	tu[i__] = ub;
	tu[j] = ue;
	--j;
/* L30: */
    }
    fpchec_(&u[1], mu, &tu[1], nu, &c__3, ier);
    if (*ier != 0) {
	goto L200;
    }
    goto L60;
L40:
    l1 = 4;
    l2 = l1;
    l3 = *nu - 3;
    l4 = l3;
    peru = ue - ub;
    tu[l2] = ub;
    tu[l3] = ue;
    for (j = 1; j <= 3; ++j) {
	++l1;
	--l2;
	++l3;
	--l4;
	tu[l2] = tu[l4] - peru;
	tu[l3] = tu[l1] + peru;
/* L50: */
    }
    fpchep_(&u[1], mu, &tu[1], nu, &c__3, ier);
    if (*ier != 0) {
	goto L200;
    }
L60:
    if (*nv < 8 || *nv > *nvest) {
	goto L200;
    }
    vb = v[1];
    ve = v[*mv];
    if (ipar[2] != 0) {
	goto L80;
    }
    j = *nv;
    for (i__ = 1; i__ <= 4; ++i__) {
	tv[i__] = vb;
	tv[j] = ve;
	--j;
/* L70: */
    }
    fpchec_(&v[1], mv, &tv[1], nv, &c__3, ier);
    if (*ier != 0) {
	goto L200;
    }
    goto L150;
L80:
    l1 = 4;
    l2 = l1;
    l3 = *nv - 3;
    l4 = l3;
    perv = ve - vb;
    tv[l2] = vb;
    tv[l3] = ve;
    for (j = 1; j <= 3; ++j) {
	++l1;
	--l2;
	++l3;
	--l4;
	tv[l2] = tv[l4] - perv;
	tv[l3] = tv[l1] + perv;
/* L90: */
    }
    fpchep_(&v[1], mv, &tv[1], nv, &c__3, ier);
    if (*ier != 0) {
	goto L200;
    } else {
	goto L150;
    }
L100:
    if (*s < (float)0.) {
	goto L200;
    }
    if (*s == (float)0. && (*nuest < *mu + 4 + (ipar[1] << 1) || *nvest < *mv 
	    + 4 + (ipar[2] << 1))) {
	goto L200;
    }
    *ier = 0;
/*  we partition the working space and determine the spline approximation */
L150:
    lfpu = 5;
    lfpv = lfpu + *nuest;
    lww = lfpv + *nvest;
    jwrk = *lwrk - 4 - *nuest - *nvest;
    knru = 4;
    knrv = knru + *mu;
    kndu = knrv + *mv;
    kndv = kndu + *nuest;
    fppasu_(iopt, &ipar[1], idim, &u[1], mu, &v[1], mv, &f[1], &mf, s, nuest, 
	    nvest, &tol, &maxit, &nc, nu, &tu[1], nv, &tv[1], &c__[1], fp, &
	    wrk[1], &wrk[2], &wrk[3], &wrk[4], &wrk[lfpu], &wrk[lfpv], &iwrk[
	    1], &iwrk[2], &iwrk[3], &iwrk[knru], &iwrk[knrv], &iwrk[kndu], &
	    iwrk[kndv], &wrk[lww], &jwrk, ier);
L200:
    return 0;
} /* parsur_ */

/* Subroutine */ int percur_(iopt, m, x, y, w, k, s, nest, n, t, c__, fp, wrk,
	 lwrk, iwrk, ier)
integer *iopt, *m;
real *x, *y, *w;
integer *k;
real *s;
integer *nest, *n;
real *t, *c__, *fp, *wrk;
integer *lwrk, *iwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer nmin, i__, maxit, i1, i2, j1, j2, k1, k2, lwest, m1, ib, 
	    iq, iz;
    extern /* Subroutine */ int fpchep_(), fpperi_();
    static integer ia1, ia2, ig1, ig2, ifp;
    static real per, tol;

/*  given the set of data points (x(i),y(i)) and the set of positive */
/*  numbers w(i),i=1,2,...,m-1, subroutine percur determines a smooth */
/*  periodic spline approximation of degree k with period per=x(m)-x(1). */
/*  if iopt=-1 percur calculates the weighted least-squares periodic */
/*  spline according to a given set of knots. */
/*  if iopt>=0 the number of knots of the spline s(x) and the position */
/*  t(j),j=1,2,...,n is chosen automatically by the routine. the smooth- */
/*  ness of s(x) is then achieved by minimalizing the discontinuity */
/*  jumps of the k-th derivative of s(x) at the knots t(j),j=k+2,k+3,..., */
/*  n-k-1. the amount of smoothness is determined by the condition that */
/*  f(p)=sum((w(i)*(y(i)-s(x(i))))**2) be <= s, with s a given non- */
/*  negative constant, called the smoothing factor. */
/*  the fit s(x) is given in the b-spline representation (b-spline coef- */
/*  ficients c(j),j=1,2,...,n-k-1) and can be evaluated by means of */
/*  subroutine splev. */

/*  calling sequence: */
/*     call percur(iopt,m,x,y,w,k,s,nest,n,t,c,fp,wrk, */
/*    * lwrk,iwrk,ier) */

/*  parameters: */
/*   iopt  : integer flag. on entry iopt must specify whether a weighted */
/*           least-squares spline (iopt=-1) or a smoothing spline (iopt= */
/*           0 or 1) must be determined. if iopt=0 the routine will start */
/*           with an initial set of knots t(i)=x(1)+(x(m)-x(1))*(i-k-1), */
/*           i=1,2,...,2*k+2. if iopt=1 the routine will continue with */
/*           the knots found at the last call of the routine. */
/*           attention: a call with iopt=1 must always be immediately */
/*           preceded by another call with iopt=1 or iopt=0. */
/*           unchanged on exit. */
/*   m     : integer. on entry m must specify the number of data points. */
/*           m > 1. unchanged on exit. */
/*   x     : real array of dimension at least (m). before entry, x(i) */
/*           must be set to the i-th value of the independent variable x, */
/*           for i=1,2,...,m. these values must be supplied in strictly */
/*           ascending order. x(m) only indicates the length of the */
/*           period of the spline, i.e per=x(m)-x(1). */
/*           unchanged on exit. */
/*   y     : real array of dimension at least (m). before entry, y(i) */
/*           must be set to the i-th value of the dependent variable y, */
/*           for i=1,2,...,m-1. the element y(m) is not used. */
/*           unchanged on exit. */
/*   w     : real array of dimension at least (m). before entry, w(i) */
/*           must be set to the i-th value in the set of weights. the */
/*           w(i) must be strictly positive. w(m) is not used. */
/*           see also further comments. unchanged on exit. */
/*   k     : integer. on entry k must specify the degree of the spline. */
/*           1<=k<=5. it is recommended to use cubic splines (k=3). */
/*           the user is strongly dissuaded from choosing k even,together */
/*           with a small s-value. unchanged on exit. */
/*   s     : real.on entry (in case iopt>=0) s must specify the smoothing */
/*           factor. s >=0. unchanged on exit. */
/*           for advice on the choice of s see further comments. */
/*   nest  : integer. on entry nest must contain an over-estimate of the */
/*           total number of knots of the spline returned, to indicate */
/*           the storage space available to the routine. nest >=2*k+2. */
/*           in most practical situation nest=m/2 will be sufficient. */
/*           always large enough is nest=m+2*k,the number of knots needed */
/*           for interpolation (s=0). unchanged on exit. */
/*   n     : integer. */
/*           unless ier = 10 (in case iopt >=0), n will contain the */
/*           total number of knots of the spline approximation returned. */
/*           if the computation mode iopt=1 is used this value of n */
/*           should be left unchanged between subsequent calls. */
/*           in case iopt=-1, the value of n must be specified on entry. */
/*   t     : real array of dimension at least (nest). */
/*           on succesful exit, this array will contain the knots of the */
/*           spline,i.e. the position of the interior knots t(k+2),t(k+3) */
/*           ...,t(n-k-1) as well as the position of the additional knots */
/*           t(1),t(2),...,t(k+1)=x(1) and t(n-k)=x(m),..,t(n) needed for */
/*           the b-spline representation. */
/*           if the computation mode iopt=1 is used, the values of t(1), */
/*           t(2),...,t(n) should be left unchanged between subsequent */
/*           calls. if the computation mode iopt=-1 is used, the values */
/*           t(k+2),...,t(n-k-1) must be supplied by the user, before */
/*           entry. see also the restrictions (ier=10). */
/*   c     : real array of dimension at least (nest). */
/*           on succesful exit, this array will contain the coefficients */
/*           c(1),c(2),..,c(n-k-1) in the b-spline representation of s(x) */
/*   fp    : real. unless ier = 10, fp contains the weighted sum of */
/*           squared residuals of the spline approximation returned. */
/*   wrk   : real array of dimension at least (m*(k+1)+nest*(8+5*k)). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used, the values wrk(1),...,wrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   lwrk  : integer. on entry,lwrk must specify the actual dimension of */
/*           the array wrk as declared in the calling (sub)program. lwrk */
/*           must not be too small (see wrk). unchanged on exit. */
/*   iwrk  : integer array of dimension at least (nest). */
/*           used as working space. if the computation mode iopt=1 is */
/*           used,the values iwrk(1),...,iwrk(n) should be left unchanged */
/*           between subsequent calls. */
/*   ier   : integer. unless the routine detects an error, ier contains a */
/*           non-positive value on exit, i.e. */
/*    ier=0  : normal return. the spline returned has a residual sum of */
/*             squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*             ive tolerance set to 0.001 by the program. */
/*    ier=-1 : normal return. the spline returned is an interpolating */
/*             periodic spline (fp=0). */
/*    ier=-2 : normal return. the spline returned is the weighted least- */
/*             squares constant. in this extreme case fp gives the upper */
/*             bound fp0 for the smoothing factor s. */
/*    ier=1  : error. the required storage space exceeds the available */
/*             storage space, as specified by the parameter nest. */
/*             probably causes : nest too small. if nest is already */
/*             large (say nest > m/2), it may also indicate that s is */
/*             too small */
/*             the approximation returned is the least-squares periodic */
/*             spline according to the knots t(1),t(2),...,t(n). (n=nest) */
/*             the parameter fp gives the corresponding weighted sum of */
/*             squared residuals (fp>s). */
/*    ier=2  : error. a theoretically impossible result was found during */
/*             the iteration proces for finding a smoothing spline with */
/*             fp = s. probably causes : s too small. */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*             by the program) allowed for finding a smoothing spline */
/*             with fp=s has been reached. probably causes : s too small */
/*             there is an approximation returned but the corresponding */
/*             weighted sum of squared residuals does not satisfy the */
/*             condition abs(fp-s)/s < tol. */
/*    ier=10 : error. on entry, the input data are controlled on validity */
/*             the following restrictions must be satisfied. */
/*             -1<=iopt<=1, 1<=k<=5, m>1, nest>2*k+2, w(i)>0,i=1,...,m-1 */
/*             x(1)<x(2)<...<x(m), lwrk>=(k+1)*m+nest*(8+5*k) */
/*             if iopt=-1: 2*k+2<=n<=min(nest,m+2*k) */
/*                         x(1)<t(k+2)<t(k+3)<...<t(n-k-1)<x(m) */
/*                       the schoenberg-whitney conditions, i.e. there */
/*                       must be a subset of data points xx(j) with */
/*                       xx(j) = x(i) or x(i)+(x(m)-x(1)) such that */
/*                         t(j) < xx(j) < t(j+k+1), j=k+1,...,n-k-1 */
/*             if iopt>=0: s>=0 */
/*                         if s=0 : nest >= m+2*k */
/*             if one of these conditions is found to be violated,control */
/*             is immediately repassed to the calling program. in that */
/*             case there is no approximation returned. */

/*  further comments: */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the spline will be too smooth and signal will be */
/*   lost ; if s is too small the spline will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating periodic */
/*   spline if s=0 and the weighted least-squares constant if s is very */
/*   large. between these extremes, a properly chosen s will result in */
/*   a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the weights w(i). if these are */
/*   taken as 1/d(i) with d(i) an estimate of the standard deviation of */
/*   y(i), a good s-value should be found in the range (m-sqrt(2*m),m+ */
/*   sqrt(2*m)). if nothing is known about the statistical error in y(i) */
/*   each w(i) can be set equal to one and s determined by trial and */
/*   error, taking account of the comments above. the best is then to */
/*   start with a very large value of s ( to determine the least-squares */
/*   constant and the corresponding upper bound fp0 for s) and then to */
/*   progressively decrease the value of s ( say by a factor 10 in the */
/*   beginning, i.e. s=fp0/10, fp0/100,...and more carefully as the */
/*   approximation shows more detail) to obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt=1 the program will continue with the set of knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if percur is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   function underlying the data. but, if the computation mode iopt=1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt=1, the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   percur once more with the selected value for s but now with iopt=0. */
/*   indeed, percur may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */

/*  other subroutines required: */
/*    fpbacp,fpbspl,fpchep,fpperi,fpdisc,fpgivs,fpknot,fprati,fprota */

/*  references: */
/*   dierckx p. : algorithms for smoothing data with periodic and */
/*                parametric splines, computer graphics and image */
/*                processing 20 (1982) 171-184. */
/*   dierckx p. : algorithms for smoothing data with periodic and param- */
/*                etric splines, report tw55, dept. computer science, */
/*                k.u.leuven, 1981. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : may 1979 */
/*  latest update : march 1987 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..subroutine references.. */
/*    perper,pcheck */
/*  .. */
/*  we set up the parameters tol and maxit */
    /* Parameter adjustments */
    --w;
    --y;
    --x;
    --iwrk;
    --c__;
    --t;
    --wrk;

    /* Function Body */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*k <= 0 || *k > 5) {
	goto L50;
    }
    k1 = *k + 1;
    k2 = k1 + 1;
    if (*iopt < -1 || *iopt > 1) {
	goto L50;
    }
    nmin = k1 << 1;
    if (*m < 2 || *nest < nmin) {
	goto L50;
    }
    lwest = *m * k1 + *nest * (*k * 5 + 8);
    if (*lwrk < lwest) {
	goto L50;
    }
    m1 = *m - 1;
    i__1 = m1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (x[i__] >= x[i__ + 1] || w[i__] <= (float)0.) {
	    goto L50;
	}
/* L10: */
    }
    if (*iopt >= 0) {
	goto L30;
    }
    if (*n <= nmin || *n > *nest) {
	goto L50;
    }
    per = x[*m] - x[1];
    j1 = k1;
    t[j1] = x[1];
    i1 = *n - *k;
    t[i1] = x[*m];
    j2 = j1;
    i2 = i1;
    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	++i1;
	--i2;
	++j1;
	--j2;
	t[j2] = t[i2] - per;
	t[i1] = t[j1] + per;
/* L20: */
    }
    fpchep_(&x[1], m, &t[1], n, k, ier);
    if (*ier != 0) {
	goto L50;
    } else {
	goto L40;
    }
L30:
    if (*s < (float)0.) {
	goto L50;
    }
    if (*s == (float)0. && *nest < *m + (*k << 1)) {
	goto L50;
    }
    *ier = 0;
/* we partition the working space and determine the spline approximation. */
L40:
    ifp = 1;
    iz = ifp + *nest;
    ia1 = iz + *nest;
    ia2 = ia1 + *nest * k1;
    ib = ia2 + *nest * *k;
    ig1 = ib + *nest * k2;
    ig2 = ig1 + *nest * k2;
    iq = ig2 + *nest * k1;
    fpperi_(iopt, &x[1], &y[1], &w[1], m, k, s, nest, &tol, &maxit, &k1, &k2, 
	    n, &t[1], &c__[1], fp, &wrk[ifp], &wrk[iz], &wrk[ia1], &wrk[ia2], 
	    &wrk[ib], &wrk[ig1], &wrk[ig2], &wrk[iq], &iwrk[1], ier);
L50:
    return 0;
} /* percur_ */

/* Subroutine */ int pogrid_(iopt, ider, mu, u, mv, v, z__, z0, r__, s, nuest,
	 nvest, nu, tu, nv, tv, c__, fp, wrk, lwrk, iwrk, kwrk, ier)
integer *iopt, *ider, *mu;
real *u;
integer *mv;
real *v, *z__, *z0, *r__, *s;
integer *nuest, *nvest, *nu;
real *tu;
integer *nv;
real *tv, *c__, *fp, *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double atan2();

    /* Local variables */
    static real half;
    static integer kndu, kndv, lfpu, lfpv;
    static real zmin, zmax;
    static integer jwrk, knru, knrv, i__, j, l, m, maxit, mumin, i1, i2, j1, 
	    j2, kwest, lwest, nc;
    extern /* Subroutine */ int fpchec_();
    static real pi, ve, zb, rn;
    extern /* Subroutine */ int fpchep_();
    static real uu;
    extern /* Subroutine */ int fppogr_();
    static real one, per;
    static integer ldz;
    static real tol;
    static integer muu, lww;

/*  subroutine pogrid fits a function f(x,y) to a set of data points */
/*  z(i,j) given at the nodes (x,y)=(u(i)*cos(v(j)),u(i)*sin(v(j))), */
/*  i=1,...,mu ; j=1,...,mv , of a radius-angle grid over a disc */
/*    x ** 2  +  y ** 2  <=  r ** 2 . */

/*  this approximation problem is reduced to the determination of a */
/*  bicubic spline s(u,v) smoothing the data (u(i),v(j),z(i,j)) on the */
/*  rectangle 0<=u<=r, v(1)<=v<=v(1)+2*pi */
/*  in order to have continuous partial derivatives */
/*              i+j */
/*             d   f(0,0) */
/*    g(i,j) = ---------- */
/*                i   j */
/*              dx  dy */

/*  s(u,v)=f(x,y) must satisfy the following conditions */

/*    (1) s(0,v) = g(0,0)   v(1)<=v<= v(1)+2*pi */

/*        d s(0,v) */
/*    (2) -------- = cos(v)*g(1,0)+sin(v)*g(0,1)  v(1)<=v<= v(1)+2*pi */
/*        d u */

/*  moreover, s(u,v) must be periodic in the variable v, i.e. */

/*         j            j */
/*        d s(u,vb)   d s(u,ve) */
/*    (3) ---------- = ---------   0 <=u<= r, j=0,1,2 , vb=v(1), */
/*           j            j                             ve=vb+2*pi */
/*        d v          d v */

/*  the number of knots of s(u,v) and their position tu(i),i=1,2,...,nu; */
/*  tv(j),j=1,2,...,nv, is chosen automatically by the routine. the */
/*  smoothness of s(u,v) is achieved by minimalizing the discontinuity */
/*  jumps of the derivatives of the spline at the knots. the amount of */
/*  smoothness of s(u,v) is determined by the condition that */
/*  fp=sumi=1,mu(sumj=1,mv((z(i,j)-s(u(i),v(j)))**2))+(z0-g(0,0))**2<=s, */
/*  with s a given non-negative constant. */
/*  the fit s(u,v) is given in its b-spline representation and can be */
/*  evaluated by means of routine bispev. f(x,y) = s(u,v) can also be */
/*  evaluated by means of function program evapol. */

/* calling sequence: */
/*     call pogrid(iopt,ider,mu,u,mv,v,z,z0,r,s,nuest,nvest,nu,tu, */
/*    *  ,nv,tv,c,fp,wrk,lwrk,iwrk,kwrk,ier) */

/* parameters: */
/*  iopt  : integer array of dimension 3, specifying different options. */
/*          unchanged on exit. */
/*  iopt(1):on entry iopt(1) must specify whether a least-squares spline */
/*          (iopt(1)=-1) or a smoothing spline (iopt(1)=0 or 1) must be */
/*          determined. */
/*          if iopt(1)=0 the routine will start with an initial set of */
/*          knots tu(i)=0,tu(i+4)=r,i=1,...,4;tv(i)=v(1)+(i-4)*2*pi,i=1,. */
/*          ...,8. */
/*          if iopt(1)=1 the routine will continue with the set of knots */
/*          found at the last call of the routine. */
/*          attention: a call with iopt(1)=1 must always be immediately */
/*          preceded by another call with iopt(1) = 1 or iopt(1) = 0. */
/*  iopt(2):on entry iopt(2) must specify the requested order of conti- */
/*          nuity for f(x,y) at the origin. */
/*          if iopt(2)=0 only condition (1) must be fulfilled and */
/*          if iopt(2)=1 conditions (1)+(2) must be fulfilled. */
/*  iopt(3):on entry iopt(3) must specify whether (iopt(3)=1) or not */
/*          (iopt(3)=0) the approximation f(x,y) must vanish at the */
/*          boundary of the approximation domain. */
/*  ider  : integer array of dimension 2, specifying different options. */
/*          unchanged on exit. */
/*  ider(1):on entry ider(1) must specify whether (ider(1)=0 or 1) or not */
/*          (ider(1)=-1) there is a data value z0 at the origin. */
/*          if ider(1)=1, z0 will be considered to be the right function */
/*          value, and it will be fitted exactly (g(0,0)=z0=c(1)). */
/*          if ider(1)=0, z0 will be considered to be a data value just */
/*          like the other data values z(i,j). */
/*  ider(2):on entry ider(2) must specify whether (ider(2)=1) or not */
/*          (ider(2)=0) f(x,y) must have vanishing partial derivatives */
/*          g(1,0) and g(0,1) at the origin. (in case iopt(2)=1) */
/*  mu    : integer. on entry mu must specify the number of grid points */
/*          along the u-axis. unchanged on exit. */
/*          mu >= mumin where mumin=4-iopt(3)-ider(2) if ider(1)<0 */
/*                                 =3-iopt(3)-ider(2) if ider(1)>=0 */
/*  u     : real array of dimension at least (mu). before entry, u(i) */
/*          must be set to the u-co-ordinate of the i-th grid point */
/*          along the u-axis, for i=1,2,...,mu. these values must be */
/*          positive and supplied in strictly ascending order. */
/*          unchanged on exit. */
/*  mv    : integer. on entry mv must specify the number of grid points */
/*          along the v-axis. mv > 3 . unchanged on exit. */
/*  v     : real array of dimension at least (mv). before entry, v(j) */
/*          must be set to the v-co-ordinate of the j-th grid point */
/*          along the v-axis, for j=1,2,...,mv. these values must be */
/*          supplied in strictly ascending order. unchanged on exit. */
/*          -pi <= v(1) < pi , v(mv) < v(1)+2*pi. */
/*  z     : real array of dimension at least (mu*mv). */
/*          before entry, z(mv*(i-1)+j) must be set to the data value at */
/*          the grid point (u(i),v(j)) for i=1,...,mu and j=1,...,mv. */
/*          unchanged on exit. */
/*  z0    : real value. on entry (if ider(1) >=0 ) z0 must specify the */
/*          data value at the origin. unchanged on exit. */
/*  r     : real value. on entry r must specify the radius of the disk. */
/*          r>=u(mu) (>u(mu) if iopt(3)=1). unchanged on exit. */
/*  s     : real. on entry (if iopt(1)>=0) s must specify the smoothing */
/*          factor. s >=0. unchanged on exit. */
/*          for advice on the choice of s see further comments */
/*  nuest : integer. unchanged on exit. */
/*  nvest : integer. unchanged on exit. */
/*          on entry, nuest and nvest must specify an upper bound for the */
/*          number of knots required in the u- and v-directions respect. */
/*          these numbers will also determine the storage space needed by */
/*          the routine. nuest >= 8, nvest >= 8. */
/*          in most practical situation nuest = mu/2, nvest=mv/2, will */
/*          be sufficient. always large enough are nuest=mu+5+iopt(2)+ */
/*          iopt(3), nvest = mv+7, the number of knots needed for */
/*          interpolation (s=0). see also further comments. */
/*  nu    : integer. */
/*          unless ier=10 (in case iopt(1)>=0), nu will contain the total */
/*          number of knots with respect to the u-variable, of the spline */
/*          approximation returned. if the computation mode iopt(1)=1 is */
/*          used, the value of nu should be left unchanged between sub- */
/*          sequent calls. in case iopt(1)=-1, the value of nu should be */
/*          specified on entry. */
/*  tu    : real array of dimension at least (nuest). */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the u-variable, i.e. the position of */
/*          the interior knots tu(5),...,tu(nu-4) as well as the position */
/*          of the additional knots tu(1)=...=tu(4)=0 and tu(nu-3)=...= */
/*          tu(nu)=r needed for the b-spline representation. */
/*          if the computation mode iopt(1)=1 is used,the values of tu(1) */
/*          ...,tu(nu) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt(1)=-1 is used, the values tu(5), */
/*          ...tu(nu-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  nv    : integer. */
/*          unless ier=10 (in case iopt(1)>=0), nv will contain the total */
/*          number of knots with respect to the v-variable, of the spline */
/*          approximation returned. if the computation mode iopt(1)=1 is */
/*          used, the value of nv should be left unchanged between sub- */
/*          sequent calls. in case iopt(1) = -1, the value of nv should */
/*          be specified on entry. */
/*  tv    : real array of dimension at least (nvest). */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the v-variable, i.e. the position of */
/*          the interior knots tv(5),...,tv(nv-4) as well as the position */
/*          of the additional knots tv(1),...,tv(4) and tv(nv-3),..., */
/*          tv(nv) needed for the b-spline representation. */
/*          if the computation mode iopt(1)=1 is used,the values of tv(1) */
/*          ...,tv(nv) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt(1)=-1 is used, the values tv(5), */
/*          ...tv(nv-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  c     : real array of dimension at least (nuest-4)*(nvest-4). */
/*          on succesful exit, c contains the coefficients of the spline */
/*          approximation s(u,v) */
/*  fp    : real. unless ier=10, fp contains the sum of squared */
/*          residuals of the spline approximation returned. */
/*  wrk   : real array of dimension (lwrk). used as workspace. */
/*          if the computation mode iopt(1)=1 is used the values of */
/*          wrk(1),...,wrk(8) should be left unchanged between subsequent */
/*          calls. */
/*  lwrk  : integer. on entry lwrk must specify the actual dimension of */
/*          the array wrk as declared in the calling (sub)program. */
/*          lwrk must not be too small. */
/*           lwrk >= 8+nuest*(mv+nvest+3)+nvest*21+4*mu+6*mv+q */
/*           where q is the larger of (mv+nvest) and nuest. */
/*  iwrk  : integer array of dimension (kwrk). used as workspace. */
/*          if the computation mode iopt(1)=1 is used the values of */
/*          iwrk(1),.,iwrk(4) should be left unchanged between subsequent */
/*          calls. */
/*  kwrk  : integer. on entry kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. */
/*          kwrk >= 4+mu+mv+nuest+nvest. */
/*  ier   : integer. unless the routine detects an error, ier contains a */
/*          non-positive value on exit, i.e. */
/*   ier=0  : normal return. the spline returned has a residual sum of */
/*            squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*            ive tolerance set to 0.001 by the program. */
/*   ier=-1 : normal return. the spline returned is an interpolating */
/*            spline (fp=0). */
/*   ier=-2 : normal return. the spline returned is the least-squares */
/*            constrained polynomial. in this extreme case fp gives the */
/*            upper bound for the smoothing factor s. */
/*   ier=1  : error. the required storage space exceeds the available */
/*            storage space, as specified by the parameters nuest and */
/*            nvest. */
/*            probably causes : nuest or nvest too small. if these param- */
/*            eters are already large, it may also indicate that s is */
/*            too small */
/*            the approximation returned is the least-squares spline */
/*            according to the current set of knots. the parameter fp */
/*            gives the corresponding sum of squared residuals (fp>s). */
/*   ier=2  : error. a theoretically impossible result was found during */
/*            the iteration proces for finding a smoothing spline with */
/*            fp = s. probably causes : s too small. */
/*            there is an approximation returned but the corresponding */
/*            sum of squared residuals does not satisfy the condition */
/*            abs(fp-s)/s < tol. */
/*   ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*            by the program) allowed for finding a smoothing spline */
/*            with fp=s has been reached. probably causes : s too small */
/*            there is an approximation returned but the corresponding */
/*            sum of squared residuals does not satisfy the condition */
/*            abs(fp-s)/s < tol. */
/*   ier=10 : error. on entry, the input data are controlled on validity */
/*            the following restrictions must be satisfied. */
/*            -1<=iopt(1)<=1, 0<=iopt(2)<=1, 0<=iopt(3)<=1, */
/*            -1<=ider(1)<=1, 0<=ider(2)<=1, ider(2)=0 if iopt(2)=0. */
/*            mu >= mumin (see above), mv >= 4, nuest >=8, nvest >= 8, */
/*            kwrk>=4+mu+mv+nuest+nvest, */
/*            lwrk >= 8+nuest*(mv+nvest+3)+nvest*21+4*mu+6*mv+ */
/*             max(nuest,mv+nvest) */
/*            0< u(i-1)<u(i)<=r,i=2,..,mu, (< r if iopt(3)=1) */
/*            -pi<=v(1)< pi, v(1)<v(i-1)<v(i)<v(1)+2*pi, i=3,...,mv */
/*            if iopt(1)=-1: 8<=nu<=min(nuest,mu+5+iopt(2)+iopt(3)) */
/*                           0<tu(5)<tu(6)<...<tu(nu-4)<r */
/*                           8<=nv<=min(nvest,mv+7) */
/*                           v(1)<tv(5)<tv(6)<...<tv(nv-4)<v(1)+2*pi */
/*                    the schoenberg-whitney conditions, i.e. there must */
/*                    be subset of grid co-ordinates uu(p) and vv(q) such */
/*                    that   tu(p) < uu(p) < tu(p+4) ,p=1,...,nu-4 */
/*                     (iopt(2)=1 and iopt(3)=1 also count for a uu-value */
/*                           tv(q) < vv(q) < tv(q+4) ,q=1,...,nv-4 */
/*                     (vv(q) is either a value v(j) or v(j)+2*pi) */
/*            if iopt(1)>=0: s>=0 */
/*                       if s=0: nuest>=mu+5+iopt(2)+iopt(3), nvest>=mv+7 */
/*            if one of these conditions is found to be violated,control */
/*            is immediately repassed to the calling program. in that */
/*            case there is no approximation returned. */

/* further comments: */
/*   pogrid does not allow individual weighting of the data-values. */
/*   so, if these were determined to widely different accuracies, then */
/*   perhaps the general data set routine polar should rather be used */
/*   in spite of efficiency. */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the spline will be too smooth and signal will be */
/*   lost ; if s is too small the spline will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating spline if */
/*   s=0 and the constrained least-squares polynomial(degrees 3,0)if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the accuracy of the data values. */
/*   if the user has an idea of the statistical errors on the data, he */
/*   can also find a proper estimate for s. for, by assuming that, if he */
/*   specifies the right s, pogrid will return a spline s(u,v) which */
/*   exactly reproduces the function underlying the data he can evaluate */
/*   the sum((z(i,j)-s(u(i),v(j)))**2) to find a good estimate for this s */
/*   for example, if he knows that the statistical errors on his z(i,j)- */
/*   values is not greater than 0.1, he may expect that a good s should */
/*   have a value not larger than mu*mv*(0.1)**2. */
/*   if nothing is known about the statistical error in z(i,j), s must */
/*   be determined by trial and error, taking account of the comments */
/*   above. the best is then to start with a very large value of s (to */
/*   determine the least-squares polynomial and the corresponding upper */
/*   bound fp0 for s) and then to progressively decrease the value of s */
/*   ( say by a factor 10 in the beginning, i.e. s=fp0/10,fp0/100,... */
/*   and more carefully as the approximation shows more detail) to */
/*   obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt(1)=0. */
/*   if iopt(1) = 1 the program will continue with the knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if pogrid is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   function underlying the data. if the computation mode iopt(1) = 1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt(1)=1,the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   pogrid once more with the chosen value for s but now with iopt(1)=0. */
/*   indeed, pogrid may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */
/*   the number of knots may also depend on the upper bounds nuest and */
/*   nvest. indeed, if at a certain stage in pogrid the number of knots */
/*   in one direction (say nu) has reached the value of its upper bound */
/*   (nuest), then from that moment on all subsequent knots are added */
/*   in the other (v) direction. this may indicate that the value of */
/*   nuest is too small. on the other hand, it gives the user the option */
/*   of limiting the number of knots the routine locates in any direction */
/*   for example, by setting nuest=8 (the lowest allowable value for */
/*   nuest), the user can indicate that he wants an approximation which */
/*   is a simple cubic polynomial in the variable u. */

/*  other subroutines required: */
/*    fppogr,fpchec,fpchep,fpknot,fpopdi,fprati,fpgrdi,fpsysy,fpback, */
/*    fpbacp,fpbspl,fpcyt1,fpcyt2,fpdisc,fpgivs,fprota */

/*  references: */
/*   dierckx p. : fast algorithms for smoothing data over a disc or a */
/*                sphere using tensor product splines, in "algorithms */
/*                for approximation", ed. j.c.mason and m.g.cox, */
/*                clarendon press oxford, 1987, pp. 51-65 */
/*   dierckx p. : fast algorithms for smoothing data over a disc or a */
/*                sphere using tensor product splines, report tw73, dept. */
/*                computer science,k.u.leuven, 1985. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : july 1985 */
/*  latest update : march 1989 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpchec,fpchep,fppogr */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --iopt;
    --ider;
    --u;
    --z__;
    --v;
    --tu;
    --c__;
    --tv;
    --wrk;
    --iwrk;

    /* Function Body */
    one = (float)1.;
    half = (float).5;
    pi = atan2((float)0., -one);
    per = pi + pi;
    ve = v[1] + per;
/*  we set up the parameters tol and maxit. */
    maxit = 20;
    tol = (float).001;
/*  before starting computations, a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (iopt[1] < -1 || iopt[1] > 1) {
	goto L200;
    }
    if (iopt[2] < 0 || iopt[2] > 1) {
	goto L200;
    }
    if (iopt[3] < 0 || iopt[3] > 1) {
	goto L200;
    }
    if (ider[1] < -1 || ider[1] > 1) {
	goto L200;
    }
    if (ider[2] < 0 || ider[2] > 1) {
	goto L200;
    }
    if (ider[2] == 1 && iopt[2] == 0) {
	goto L200;
    }
    mumin = 4 - iopt[3] - ider[2];
    if (ider[1] >= 0) {
	--mumin;
    }
    if (*mu < mumin || *mv < 4) {
	goto L200;
    }
    if (*nuest < 8 || *nvest < 8) {
	goto L200;
    }
    m = *mu * *mv;
    nc = (*nuest - 4) * (*nvest - 4);
/* Computing MAX */
    i__1 = *nuest, i__2 = *mv + *nvest;
    lwest = *nuest * (*mv + *nvest + 3) + 8 + *nvest * 21 + (*mu << 2) + *mv *
	     6 + max(i__1,i__2);
    kwest = *mu + 4 + *mv + *nuest + *nvest;
    if (*lwrk < lwest || *kwrk < kwest) {
	goto L200;
    }
    if (u[1] <= (float)0. || u[*mu] > *r__) {
	goto L200;
    }
    if (iopt[3] == 0) {
	goto L10;
    }
    if (u[*mu] == *r__) {
	goto L200;
    }
L10:
    if (*mu == 1) {
	goto L30;
    }
    i__1 = *mu;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (u[i__ - 1] >= u[i__]) {
	    goto L200;
	}
/* L20: */
    }
L30:
    if (v[1] < -pi || v[1] >= pi) {
	goto L200;
    }
    if (v[*mv] >= v[1] + per) {
	goto L200;
    }
    i__1 = *mv;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (v[i__ - 1] >= v[i__]) {
	    goto L200;
	}
/* L40: */
    }
    if (iopt[1] > 0) {
	goto L140;
    }
/*  if not given, we compute an estimate for z0. */
    if (ider[1] < 0) {
	goto L50;
    }
    zb = *z0;
    goto L70;
L50:
    zb = (float)0.;
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	zb += z__[i__];
/* L60: */
    }
    rn = (real) (*mv);
    zb /= rn;
/*  we determine the range of z-values. */
L70:
    zmin = zb;
    zmax = zb;
    i__1 = m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (z__[i__] < zmin) {
	    zmin = z__[i__];
	}
	if (z__[i__] > zmax) {
	    zmax = z__[i__];
	}
/* L80: */
    }
    wrk[5] = zb;
    wrk[6] = (float)0.;
    wrk[7] = (float)0.;
    wrk[8] = zmax - zmin;
    iwrk[4] = *mu;
    if (iopt[1] == 0) {
	goto L140;
    }
    if (*nu < 8 || *nu > *nuest) {
	goto L200;
    }
    if (*nv < 11 || *nv > *nvest) {
	goto L200;
    }
    j = *nu;
    for (i__ = 1; i__ <= 4; ++i__) {
	tu[i__] = (float)0.;
	tu[j] = *r__;
	--j;
/* L90: */
    }
    l = 9;
    wrk[l] = (float)0.;
    if (iopt[2] == 0) {
	goto L100;
    }
    ++l;
    uu = u[1];
    if (uu > tu[5]) {
	uu = tu[5];
    }
    wrk[l] = uu * half;
L100:
    i__1 = *mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	++l;
	wrk[l] = u[i__];
/* L110: */
    }
    if (iopt[3] == 0) {
	goto L120;
    }
    ++l;
    wrk[l] = *r__;
L120:
    muu = l - 8;
    fpchec_(&wrk[9], &muu, &tu[1], nu, &c__3, ier);
    if (*ier != 0) {
	goto L200;
    }
    j1 = 4;
    tv[j1] = v[1];
    i1 = *nv - 3;
    tv[i1] = ve;
    j2 = j1;
    i2 = i1;
    for (i__ = 1; i__ <= 3; ++i__) {
	++i1;
	--i2;
	++j1;
	--j2;
	tv[j2] = tv[i2] - per;
	tv[i1] = tv[j1] + per;
/* L130: */
    }
    l = 9;
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wrk[l] = v[i__];
	++l;
/* L135: */
    }
    wrk[l] = ve;
    i__1 = *mv + 1;
    fpchep_(&wrk[9], &i__1, &tv[1], nv, &c__3, ier);
    if (*ier != 0) {
	goto L200;
    } else {
	goto L150;
    }
L140:
    if (*s < (float)0.) {
	goto L200;
    }
    if (*s == (float)0. && (*nuest < *mu + 5 + iopt[2] + iopt[3] || *nvest < *
	    mv + 7)) {
	goto L200;
    }
/*  we partition the working space and determine the spline approximation */
L150:
    ldz = 5;
    lfpu = 9;
    lfpv = lfpu + *nuest;
    lww = lfpv + *nvest;
    jwrk = *lwrk - 8 - *nuest - *nvest;
    knru = 5;
    knrv = knru + *mu;
    kndu = knrv + *mv;
    kndv = kndu + *nuest;
    fppogr_(&iopt[1], &ider[1], &u[1], mu, &v[1], mv, &z__[1], &m, &zb, r__, 
	    s, nuest, nvest, &tol, &maxit, &nc, nu, &tu[1], nv, &tv[1], &c__[
	    1], fp, &wrk[1], &wrk[2], &wrk[3], &wrk[4], &wrk[lfpu], &wrk[lfpv]
	    , &wrk[ldz], &wrk[8], &iwrk[1], &iwrk[2], &iwrk[3], &iwrk[4], &
	    iwrk[knru], &iwrk[knrv], &iwrk[kndu], &iwrk[kndv], &wrk[lww], &
	    jwrk, ier);
L200:
    return 0;
} /* pogrid_ */

/* Subroutine */ int polar_(iopt, m, x, y, z__, w, rad, s, nuest, nvest, eps, 
	nu, tu, nv, tv, u, v, c__, fp, wrk1, lwrk1, wrk2, lwrk2, iwrk, kwrk, 
	ier)
integer *iopt, *m;
real *x, *y, *z__, *w;
doublereal (*rad) ();
real *s;
integer *nuest, *nvest;
real *eps;
integer *nu;
real *tu;
integer *nv;
real *tv, *u, *v, *c__, *fp, *wrk1;
integer *lwrk1;
real *wrk2;
integer *lwrk2, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2;

    /* Builtin functions */
    double atan2(), sqrt();

    /* Local variables */
    static integer nreg, ipar;
    static real dist;
    static integer iopt1, iopt2, iopt3, i__, j;
    static real r__;
    static integer ncest, maxit, nvmin, nrint, kwest, lwest, la, lf, ki, lh;
    static real pi;
    static integer kn, lq;
    extern /* Subroutine */ int fppola_();
    static integer ib1, ib3, nu4, nv4, lcc, ncc, lff, lco;
    static real one;
    static integer lbu, lcs, lfp, lbv, lro;
    static real tol;
    static integer lsu, lsv, nuu, nvv;

/*  subroutine polar fits a smooth function f(x,y) to a set of data */
/*  points (x(i),y(i),z(i)) scattered arbitrarily over an approximation */
/*  domain  x**2+y**2 <= rad(atan(y/x))**2. through the transformation */
/*    x = u*rad(v)*cos(v) , y = u*rad(v)*sin(v) */
/*  the approximation problem is reduced to the determination of a bi- */
/*  cubic spline s(u,v) fitting a corresponding set of data points */
/*  (u(i),v(i),z(i)) on the rectangle 0<=u<=1,-pi<=v<=pi. */
/*  in order to have continuous partial derivatives */
/*              i+j */
/*             d   f(0,0) */
/*    g(i,j) = ---------- */
/*                i   j */
/*              dx  dy */

/*  s(u,v)=f(x,y) must satisfy the following conditions */

/*    (1) s(0,v) = g(0,0)   -pi <=v<= pi. */

/*        d s(0,v) */
/*    (2) -------- = rad(v)*(cos(v)*g(1,0)+sin(v)*g(0,1)) */
/*        d u */
/*                                                    -pi <=v<= pi */
/*         2 */
/*        d s(0,v)         2       2             2 */
/*    (3) -------- = rad(v)*(cos(v)*g(2,0)+sin(v)*g(0,2)+sin(2*v)*g(1,1)) */
/*           2 */
/*        d u                                         -pi <=v<= pi */

/*  moreover, s(u,v) must be periodic in the variable v, i.e. */

/*         j            j */
/*        d s(u,-pi)   d s(u,pi) */
/*    (4) ---------- = ---------   0 <=u<= 1, j=0,1,2 */
/*           j           j */
/*        d v         d v */

/*  if iopt(1) < 0 circle calculates a weighted least-squares spline */
/*  according to a given set of knots in u- and v- direction. */
/*  if iopt(1) >=0, the number of knots in each direction and their pos- */
/*  ition tu(j),j=1,2,...,nu ; tv(j),j=1,2,...,nv are chosen automatical- */
/*  ly by the routine. the smoothness of s(u,v) is then achieved by mini- */
/*  malizing the discontinuity jumps of the derivatives of the spline */
/*  at the knots. the amount of smoothness of s(u,v) is determined  by */
/*  the condition that fp = sum((w(i)*(z(i)-s(u(i),v(i))))**2) be <= s, */
/*  with s a given non-negative constant. */
/*  the bicubic spline is given in its standard b-spline representation */
/*  and the corresponding function f(x,y) can be evaluated by means of */
/*  function program evapol. */

/* calling sequence: */
/*     call polar(iopt,m,x,y,z,w,rad,s,nuest,nvest,eps,nu,tu, */
/*    *  nv,tv,u,v,wrk1,lwrk1,wrk2,lwrk2,iwrk,kwrk,ier) */

/* parameters: */
/*  iopt  : integer array of dimension 3, specifying different options. */
/*          unchanged on exit. */
/*  iopt(1):on entry iopt(1) must specify whether a weighted */
/*          least-squares polar spline (iopt(1)=-1) or a smoothing */
/*          polar spline (iopt(1)=0 or 1) must be determined. */
/*          if iopt(1)=0 the routine will start with an initial set of */
/*          knots tu(i)=0,tu(i+4)=1,i=1,...,4;tv(i)=(2*i-9)*pi,i=1,...,8. */
/*          if iopt(1)=1 the routine will continue with the set of knots */
/*          found at the last call of the routine. */
/*          attention: a call with iopt(1)=1 must always be immediately */
/*          preceded by another call with iopt(1) = 1 or iopt(1) = 0. */
/*  iopt(2):on entry iopt(2) must specify the requested order of conti- */
/*          nuity for f(x,y) at the origin. */
/*          if iopt(2)=0 only condition (1) must be fulfilled, */
/*          if iopt(2)=1 conditions (1)+(2) must be fulfilled and */
/*          if iopt(2)=2 conditions (1)+(2)+(3) must be fulfilled. */
/*  iopt(3):on entry iopt(3) must specify whether (iopt(3)=1) or not */
/*          (iopt(3)=0) the approximation f(x,y) must vanish at the */
/*          boundary of the approximation domain. */
/*  m     : integer. on entry m must specify the number of data points. */
/*          m >= 4-iopt(2)-iopt(3) unchanged on exit. */
/*  x     : real array of dimension at least (m). */
/*  y     : real array of dimension at least (m). */
/*  z     : real array of dimension at least (m). */
/*          before entry, x(i),y(i),z(i) must be set to the co-ordinates */
/*          of the i-th data point, for i=1,...,m. the order of the data */
/*          points is immaterial. unchanged on exit. */
/*  w     : real array of dimension at least (m). before entry, w(i) must */
/*          be set to the i-th value in the set of weights. the w(i) must */
/*          be strictly positive. unchanged on exit. */
/*  rad   : real function subprogram defining the boundary of the approx- */
/*          imation domain, i.e   x = rad(v)*cos(v) , y = rad(v)*sin(v), */
/*          -pi <= v <= pi. */
/*          must be declared external in the calling (sub)program. */
/*  s     : real. on entry (in case iopt(1) >=0) s must specify the */
/*          smoothing factor. s >=0. unchanged on exit. */
/*          for advice on the choice of s see further comments */
/*  nuest : integer. unchanged on exit. */
/*  nvest : integer. unchanged on exit. */
/*          on entry, nuest and nvest must specify an upper bound for the */
/*          number of knots required in the u- and v-directions resp. */
/*          these numbers will also determine the storage space needed by */
/*          the routine. nuest >= 8, nvest >= 8. */
/*          in most practical situation nuest = nvest = 8+sqrt(m/2) will */
/*          be sufficient. see also further comments. */
/*  eps   : real. */
/*          on entry, eps must specify a threshold for determining the */
/*          effective rank of an over-determined linear system of equat- */
/*          ions. 0 < eps < 1.  if the number of decimal digits in the */
/*          computer representation of a real number is q, then 10**(-q) */
/*          is a suitable value for eps in most practical applications. */
/*          unchanged on exit. */
/*  nu    : integer. */
/*          unless ier=10 (in case iopt(1) >=0),nu will contain the total */
/*          number of knots with respect to the u-variable, of the spline */
/*          approximation returned. if the computation mode iopt(1)=1 */
/*          is used, the value of nu should be left unchanged between */
/*          subsequent calls. */
/*          in case iopt(1)=-1,the value of nu must be specified on entry */
/*  tu    : real array of dimension at least nuest. */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the u-variable, i.e. the position */
/*          of the interior knots tu(5),...,tu(nu-4) as well as the */
/*          position of the additional knots tu(1)=...=tu(4)=0 and */
/*          tu(nu-3)=...=tu(nu)=1 needed for the b-spline representation */
/*          if the computation mode iopt(1)=1 is used,the values of */
/*          tu(1),...,tu(nu) should be left unchanged between subsequent */
/*          calls. if the computation mode iopt(1)=-1 is used,the values */
/*          tu(5),...tu(nu-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  nv    : integer. */
/*          unless ier=10 (in case iopt(1)>=0), nv will contain the total */
/*          number of knots with respect to the v-variable, of the spline */
/*          approximation returned. if the computation mode iopt(1)=1 */
/*          is used, the value of nv should be left unchanged between */
/*          subsequent calls. in case iopt(1)=-1, the value of nv should */
/*          be specified on entry. */
/*  tv    : real array of dimension at least nvest. */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the v-variable, i.e. the position of */
/*          the interior knots tv(5),...,tv(nv-4) as well as the position */
/*          of the additional knots tv(1),...,tv(4) and tv(nv-3),..., */
/*          tv(nv) needed for the b-spline representation. */
/*          if the computation mode iopt(1)=1 is used, the values of */
/*          tv(1),...,tv(nv) should be left unchanged between subsequent */
/*          calls. if the computation mode iopt(1)=-1 is used,the values */
/*          tv(5),...tv(nv-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  u     : real array of dimension at least (m). */
/*  v     : real array of dimension at least (m). */
/*          on succesful exit, u(i),v(i) contains the co-ordinates of */
/*          the i-th data point with respect to the transformed rectan- */
/*          gular approximation domain, for i=1,2,...,m. */
/*          if the computation mode iopt(1)=1 is used the values of */
/*          u(i),v(i) should be left unchanged between subsequent calls. */
/*  c     : real array of dimension at least (nuest-4)*(nvest-4). */
/*          on succesful exit, c contains the coefficients of the spline */
/*          approximation s(u,v). */
/*  fp    : real. unless ier=10, fp contains the weighted sum of */
/*          squared residuals of the spline approximation returned. */
/*  wrk1  : real array of dimension (lwrk1). used as workspace. */
/*          if the computation mode iopt(1)=1 is used the value of */
/*          wrk1(1) should be left unchanged between subsequent calls. */
/*          on exit wrk1(2),wrk1(3),...,wrk1(1+ncof) will contain the */
/*          values d(i)/max(d(i)),i=1,...,ncof=1+iopt(2)*(iopt(2)+3)/2+ */
/*          (nv-7)*(nu-5-iopt(2)-iopt(3)) with d(i) the i-th diagonal el- */
/*          ement of the triangular matrix for calculating the b-spline */
/*          coefficients.it includes those elements whose square is < eps */
/*          which are treated as 0 in the case of rank deficiency(ier=-2) */
/*  lwrk1 : integer. on entry lwrk1 must specify the actual dimension of */
/*          the array wrk1 as declared in the calling (sub)program. */
/*          lwrk1 must not be too small. let */
/*            k = nuest-7, l = nvest-7, p = 1+iopt(2)*(iopt(2)+3)/2, */
/*            q = k+2-iopt(2)-iopt(3) then */
/*          lwrk1 >= 129+10*k+21*l+k*l+(p+l*q)*(1+8*l+p)+8*m */
/*  wrk2  : real array of dimension (lwrk2). used as workspace, but */
/*          only in the case a rank deficient system is encountered. */
/*  lwrk2 : integer. on entry lwrk2 must specify the actual dimension of */
/*          the array wrk2 as declared in the calling (sub)program. */
/*          lwrk2 > 0 . a save upper bound  for lwrk2 = (p+l*q+1)*(4*l+p) */
/*          +p+l*q where p,l,q are as above. if there are enough data */
/*          points, scattered uniformly over the approximation domain */
/*          and if the smoothing factor s is not too small, there is a */
/*          good chance that this extra workspace is not needed. a lot */
/*          of memory might therefore be saved by setting lwrk2=1. */
/*          (see also ier > 10) */
/*  iwrk  : integer array of dimension (kwrk). used as workspace. */
/*  kwrk  : integer. on entry kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. */
/*          kwrk >= m+(nuest-7)*(nvest-7). */
/*  ier   : integer. unless the routine detects an error, ier contains a */
/*          non-positive value on exit, i.e. */
/*   ier=0  : normal return. the spline returned has a residual sum of */
/*            squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*            ive tolerance set to 0.001 by the program. */
/*   ier=-1 : normal return. the spline returned is an interpolating */
/*            spline (fp=0). */
/*   ier=-2 : normal return. the spline returned is the weighted least- */
/*            squares constrained polynomial . in this extreme case */
/*            fp gives the upper bound for the smoothing factor s. */
/*   ier<-2 : warning. the coefficients of the spline returned have been */
/*            computed as the minimal norm least-squares solution of a */
/*            (numerically) rank deficient system. (-ier) gives the rank. */
/*            especially if the rank deficiency which can be computed as */
/*            1+iopt(2)*(iopt(2)+3)/2+(nv-7)*(nu-5-iopt(2)-iopt(3))+ier */
/*            is large the results may be inaccurate. */
/*            they could also seriously depend on the value of eps. */
/*   ier=1  : error. the required storage space exceeds the available */
/*            storage space, as specified by the parameters nuest and */
/*            nvest. */
/*            probably causes : nuest or nvest too small. if these param- */
/*            eters are already large, it may also indicate that s is */
/*            too small */
/*            the approximation returned is the weighted least-squares */
/*            polar spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=2  : error. a theoretically impossible result was found during */
/*            the iteration proces for finding a smoothing spline with */
/*            fp = s. probably causes : s too small or badly chosen eps. */
/*            there is an approximation returned but the corresponding */
/*            weighted sum of squared residuals does not satisfy the */
/*            condition abs(fp-s)/s < tol. */
/*   ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*            by the program) allowed for finding a smoothing spline */
/*            with fp=s has been reached. probably causes : s too small */
/*            there is an approximation returned but the corresponding */
/*            weighted sum of squared residuals does not satisfy the */
/*            condition abs(fp-s)/s < tol. */
/*   ier=4  : error. no more knots can be added because the dimension */
/*            of the spline 1+iopt(2)*(iopt(2)+3)/2+(nv-7)*(nu-5-iopt(2) */
/*            -iopt(3)) already exceeds the number of data points m. */
/*            probably causes : either s or m too small. */
/*            the approximation returned is the weighted least-squares */
/*            polar spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=5  : error. no more knots can be added because the additional */
/*            knot would (quasi) coincide with an old one. */
/*            probably causes : s too small or too large a weight to an */
/*            inaccurate data point. */
/*            the approximation returned is the weighted least-squares */
/*            polar spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=10 : error. on entry, the input data are controlled on validity */
/*            the following restrictions must be satisfied. */
/*            -1<=iopt(1)<=1 , 0<=iopt(2)<=2 , 0<=iopt(3)<=1 , */
/*            m>=4-iopt(2)-iopt(3) , nuest>=8 ,nvest >=8, 0<eps<1, */
/*            0<=teta(i)<=pi, 0<=phi(i)<=2*pi, w(i)>0, i=1,...,m */
/*            lwrk1 >= 129+10*k+21*l+k*l+(p+l*q)*(1+8*l+p)+8*m */
/*            kwrk >= m+(nuest-7)*(nvest-7) */
/*            if iopt(1)=-1:9<=nu<=nuest,9+iopt(2)*(iopt(2)+1)<=nv<=nvest */
/*                          0<tu(5)<tu(6)<...<tu(nu-4)<1 */
/*                          -pi<tv(5)<tv(6)<...<tv(nv-4)<pi */
/*            if iopt(1)>=0: s>=0 */
/*            if one of these conditions is found to be violated,control */
/*            is immediately repassed to the calling program. in that */
/*            case there is no approximation returned. */
/*   ier>10 : error. lwrk2 is too small, i.e. there is not enough work- */
/*            space for computing the minimal least-squares solution of */
/*            a rank deficient system of linear equations. ier gives the */
/*            requested value for lwrk2. there is no approximation re- */
/*            turned but, having saved the information contained in nu, */
/*            nv,tu,tv,wrk1,u,v and having adjusted the value of lwrk2 */
/*            and the dimension of the array wrk2 accordingly, the user */
/*            can continue at the point the program was left, by calling */
/*            polar with iopt(1)=1. */

/* further comments: */
/*  by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the spline will be too smooth and signal will be */
/*   lost ; if s is too small the spline will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating spline if */
/*   s=0 and the constrained weighted least-squares polynomial if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the weights w(i). if these are */
/*   taken as 1/d(i) with d(i) an estimate of the standard deviation of */
/*   z(i), a good s-value should be found in the range (m-sqrt(2*m),m+ */
/*   sqrt(2*m)). if nothing is known about the statistical error in z(i) */
/*   each w(i) can be set equal to one and s determined by trial and */
/*   error, taking account of the comments above. the best is then to */
/*   start with a very large value of s ( to determine the least-squares */
/*   polynomial and the corresponding upper bound fp0 for s) and then to */
/*   progressively decrease the value of s ( say by a factor 10 in the */
/*   beginning, i.e. s=fp0/10, fp0/100,...and more carefully as the */
/*   approximation shows more detail) to obtain closer fits. */
/*   to choose s very small is strongly discouraged. this considerably */
/*   increases computation time and memory requirements. it may also */
/*   cause rank-deficiency (ier<-2) and endager numerical stability. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt(1)=0. */
/*   if iopt(1)=1 the program will continue with the set of knots found */
/*   at the last call of the routine. this will save a lot of computation */
/*   time if polar is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   function underlying the data. if the computation mode iopt(1)=1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt(1)=1,the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   polar once more with the selected value for s but now with iopt(1)=0 */
/*   indeed, polar may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */
/*   the number of knots may also depend on the upper bounds nuest and */
/*   nvest. indeed, if at a certain stage in polar the number of knots */
/*   in one direction (say nu) has reached the value of its upper bound */
/*   (nuest), then from that moment on all subsequent knots are added */
/*   in the other (v) direction. this may indicate that the value of */
/*   nuest is too small. on the other hand, it gives the user the option */
/*   of limiting the number of knots the routine locates in any direction */

/*  other subroutines required: */
/*    fpback,fpbspl,fppola,fpdisc,fpgivs,fprank,fprati,fprota,fporde, */
/*    fprppo */

/*  references: */
/*   dierckx p.: an algorithm for fitting data over a circle using tensor */
/*               product splines,j.comp.appl.maths 15 (1986) 161-173. */
/*   dierckx p.: an algorithm for fitting data on a circle using tensor */
/*               product splines, report tw68, dept. computer science, */
/*               k.u.leuven, 1984. */
/*   dierckx p.: curve and surface fitting with splines, monographs on */
/*               numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : june 1984 */
/*  latest update : march 1989 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..user specified function */
/*  ..local scalars.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fppola */
/*  .. */
/*  set up constants */
    /* Parameter adjustments */
    --iopt;
    --v;
    --u;
    --w;
    --z__;
    --y;
    --x;
    --tu;
    --c__;
    --tv;
    --wrk1;
    --wrk2;
    --iwrk;

    /* Function Body */
    one = (float)1.;
/*  we set up the parameters tol and maxit. */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid,control is immediately repassed to the calling program. */
    *ier = 10;
    if (*eps <= (float)0. || *eps >= (float)1.) {
	goto L60;
    }
    iopt1 = iopt[1];
    if (iopt1 < -1 || iopt1 > 1) {
	goto L60;
    }
    iopt2 = iopt[2];
    if (iopt2 < 0 || iopt2 > 2) {
	goto L60;
    }
    iopt3 = iopt[3];
    if (iopt3 < 0 || iopt3 > 1) {
	goto L60;
    }
    if (*m < 4 - iopt2 - iopt3) {
	goto L60;
    }
    if (*nuest < 8 || *nvest < 8) {
	goto L60;
    }
    nu4 = *nuest - 4;
    nv4 = *nvest - 4;
    ncest = nu4 * nv4;
    nuu = *nuest - 7;
    nvv = *nvest - 7;
    ipar = iopt2 * (iopt2 + 3) / 2 + 1;
    ncc = ipar + nvv * (*nuest - 5 - iopt2 - iopt3);
    nrint = nuu + nvv;
    nreg = nuu * nvv;
    ib1 = nvv << 2;
    ib3 = ib1 + ipar;
    lwest = ncc * (ib1 + 1 + ib3) + (nrint << 1) + ncest + (*m << 3) + ib3 + *
	    nuest * 5 + *nvest * 12;
    kwest = *m + nreg;
    if (*lwrk1 < lwest || *kwrk < kwest) {
	goto L60;
    }
    if (iopt1 > 0) {
	goto L40;
    }
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (w[i__] <= (float)0.) {
	    goto L60;
	}
/* Computing 2nd power */
	r__1 = x[i__];
/* Computing 2nd power */
	r__2 = y[i__];
	dist = r__1 * r__1 + r__2 * r__2;
	u[i__] = (float)0.;
	v[i__] = (float)0.;
	if (dist <= (float)0.) {
	    goto L10;
	}
	v[i__] = atan2(y[i__], x[i__]);
	r__ = (*rad)(&v[i__]);
	if (r__ <= (float)0.) {
	    goto L60;
	}
	u[i__] = sqrt(dist) / r__;
	if (u[i__] > one) {
	    goto L60;
	}
L10:
	;
    }
    if (iopt1 == 0) {
	goto L40;
    }
    nuu = *nu - 8;
    if (nuu < 1 || *nu > *nuest) {
	goto L60;
    }
    tu[4] = (float)0.;
    i__1 = nuu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = i__ + 4;
	if (tu[j] <= tu[j - 1] || tu[j] >= one) {
	    goto L60;
	}
/* L20: */
    }
    nvv = *nv - 8;
    nvmin = iopt2 * (iopt2 + 1) + 9;
    if (*nv < nvmin || *nv > *nvest) {
	goto L60;
    }
    pi = atan2((float)0., -one);
    tv[4] = -pi;
    i__1 = nvv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = i__ + 4;
	if (tv[j] <= tv[j - 1] || tv[j] >= pi) {
	    goto L60;
	}
/* L30: */
    }
    goto L50;
L40:
    if (*s < (float)0.) {
	goto L60;
    }
L50:
    *ier = 0;
/*  we partition the working space and determine the spline approximation */
    kn = 1;
    ki = kn + *m;
    lq = 2;
    la = lq + ncc * ib3;
    lf = la + ncc * ib1;
    lff = lf + ncc;
    lfp = lff + ncest;
    lco = lfp + nrint;
    lh = lco + nrint;
    lbu = lh + ib3;
    lbv = lbu + *nuest * 5;
    lro = lbv + *nvest * 5;
    lcc = lro + *nvest;
    lcs = lcc + *nvest;
    lsu = lcs + *nvest * 5;
    lsv = lsu + (*m << 2);
    fppola_(&iopt1, &iopt2, &iopt3, m, &u[1], &v[1], &z__[1], &w[1], rad, s, 
	    nuest, nvest, eps, &tol, &maxit, &ib1, &ib3, &ncest, &ncc, &nrint,
	     &nreg, nu, &tu[1], nv, &tv[1], &c__[1], fp, &wrk1[1], &wrk1[lfp],
	     &wrk1[lco], &wrk1[lf], &wrk1[lff], &wrk1[lro], &wrk1[lcc], &wrk1[
	    lcs], &wrk1[la], &wrk1[lq], &wrk1[lbu], &wrk1[lbv], &wrk1[lsu], &
	    wrk1[lsv], &wrk1[lh], &iwrk[ki], &iwrk[kn], &wrk2[1], lwrk2, ier);
L60:
    return 0;
} /* polar_ */

/* Subroutine */ int profil_(iopt, tx, nx, ty, ny, c__, kx, ky, u, nu, cu, 
	ier)
integer *iopt;
real *tx;
integer *nx;
real *ty;
integer *ny;
real *c__;
integer *kx, *ky;
real *u;
integer *nu;
real *cu;
integer *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static real h__[6];
    static integer i__, j, l, m, l1, m0;
    extern /* Subroutine */ int fpbspl_();
    static integer kx1, ky1;
    static real sum;
    static integer nkx1, nky1;

/*  if iopt=0 subroutine profil calculates the b-spline coefficients of */
/*  the univariate spline f(y) = s(u,y) with s(x,y) a bivariate spline of */
/*  degrees kx and ky, given in the b-spline representation. */
/*  if iopt = 1 it calculates the b-spline coefficients of the univariate */
/*  spline g(x) = s(x,u) */

/*  calling sequence: */
/*     call profil(iopt,tx,nx,ty,ny,c,kx,ky,u,nu,cu,ier) */

/*  input parameters: */
/*   iopt  : integer flag, specifying whether the profile f(y) (iopt=0) */
/*           or the profile g(x) (iopt=1) must be determined. */
/*   tx    : real array, length nx, which contains the position of the */
/*           knots in the x-direction. */
/*   nx    : integer, giving the total number of knots in the x-direction */
/*   ty    : real array, length ny, which contains the position of the */
/*           knots in the y-direction. */
/*   ny    : integer, giving the total number of knots in the y-direction */
/*   c     : real array, length (nx-kx-1)*(ny-ky-1), which contains the */
/*           b-spline coefficients. */
/*   kx,ky : integer values, giving the degrees of the spline. */
/*   u     : real value, specifying the requested profile. */
/*           tx(kx+1)<=u<=tx(nx-kx), if iopt=0. */
/*           ty(ky+1)<=u<=ty(ny-ky), if iopt=1. */
/*   nu    : on entry nu must specify the dimension of the array cu. */
/*           nu >= ny if iopt=0, nu >= nx if iopt=1. */

/*  output parameters: */
/*   cu    : real array of dimension (nu). */
/*           on succesful exit this array contains the b-spline */
/*   ier   : integer error flag */
/*    ier=0 : normal return */
/*    ier=10: invalid input data (see restrictions) */

/*  restrictions: */
/*   if iopt=0 : tx(kx+1) <= u <= tx(nx-kx), nu >=ny. */
/*   if iopt=1 : ty(ky+1) <= u <= ty(ny-ky), nu >=nx. */

/*  other subroutines required: */
/*    fpbspl */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --tx;
    --ty;
    --c__;
    --cu;

    /* Function Body */
    kx1 = *kx + 1;
    ky1 = *ky + 1;
    nkx1 = *nx - kx1;
    nky1 = *ny - ky1;
    *ier = 10;
    if (*iopt != 0) {
	goto L200;
    }
    if (*nu < *ny) {
	goto L300;
    }
    if (*u < tx[kx1] || *u > tx[nkx1 + 1]) {
	goto L300;
    }
/*  the b-splinecoefficients of f(y) = s(u,y). */
    *ier = 0;
    l = kx1;
    l1 = l + 1;
L110:
    if (*u < tx[l1] || l == nkx1) {
	goto L120;
    }
    l = l1;
    l1 = l + 1;
    goto L110;
L120:
    fpbspl_(&tx[1], nx, kx, u, &l, h__);
    m0 = (l - kx1) * nky1 + 1;
    i__1 = nky1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	m = m0;
	sum = (float)0.;
	i__2 = kx1;
	for (j = 1; j <= i__2; ++j) {
	    sum += h__[j - 1] * c__[m];
	    m += nky1;
/* L130: */
	}
	cu[i__] = sum;
	++m0;
/* L140: */
    }
    goto L300;
L200:
    if (*nu < *nx) {
	goto L300;
    }
    if (*u < ty[ky1] || *u > ty[nky1 + 1]) {
	goto L300;
    }
/*  the b-splinecoefficients of g(x) = s(x,u). */
    *ier = 0;
    l = ky1;
    l1 = l + 1;
L210:
    if (*u < ty[l1] || l == nky1) {
	goto L220;
    }
    l = l1;
    l1 = l + 1;
    goto L210;
L220:
    fpbspl_(&ty[1], ny, ky, u, &l, h__);
    m0 = l - *ky;
    i__1 = nkx1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	m = m0;
	sum = (float)0.;
	i__2 = ky1;
	for (j = 1; j <= i__2; ++j) {
	    sum += h__[j - 1] * c__[m];
	    ++m;
/* L230: */
	}
	cu[i__] = sum;
	m0 += nky1;
/* L240: */
    }
L300:
    return 0;
} /* profil_ */

/* Subroutine */ int regrid_(iopt, mx, x, my, y, z__, xb, xe, yb, ye, kx, ky, 
	s, nxest, nyest, nx, tx, ny, ty, c__, fp, wrk, lwrk, iwrk, kwrk, ier)
integer *iopt, *mx;
real *x;
integer *my;
real *y, *z__, *xb, *xe, *yb, *ye;
integer *kx, *ky;
real *s;
integer *nxest, *nyest, *nx;
real *tx;
integer *ny;
real *ty, *c__, *fp, *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer kndx, kndy, lfpx, lfpy, jwrk, knrx, knry, i__, j, maxit, 
	    nminx, nminy, kwest, lwest, nc;
    extern /* Subroutine */ int fpchec_();
    static integer mz;
    extern /* Subroutine */ int fpregr_();
    static integer kx1, kx2, ky1, ky2;
    static real tol;
    static integer lww;

/* given the set of values z(i,j) on the rectangular grid (x(i),y(j)), */
/* i=1,...,mx;j=1,...,my, subroutine regrid determines a smooth bivar- */
/* iate spline approximation s(x,y) of degrees kx and ky on the rect- */
/* angle xb <= x <= xe, yb <= y <= ye. */
/* if iopt = -1 regrid calculates the least-squares spline according */
/* to a given set of knots. */
/* if iopt >= 0 the total numbers nx and ny of these knots and their */
/* position tx(j),j=1,...,nx and ty(j),j=1,...,ny are chosen automatic- */
/* ally by the routine. the smoothness of s(x,y) is then achieved by */
/* minimalizing the discontinuity jumps in the derivatives of s(x,y) */
/* across the boundaries of the subpanels (tx(i),tx(i+1))*(ty(j),ty(j+1). */
/* the amounth of smoothness is determined by the condition that f(p) = */
/* sum ((z(i,j)-s(x(i),y(j))))**2) be <= s, with s a given non-negative */
/* constant, called the smoothing factor. */
/* the fit is given in the b-spline representation (b-spline coefficients */
/* c((ny-ky-1)*(i-1)+j),i=1,...,nx-kx-1;j=1,...,ny-ky-1) and can be eval- */
/* uated by means of subroutine bispev. */

/* calling sequence: */
/*     call regrid(iopt,mx,x,my,y,z,xb,xe,yb,ye,kx,ky,s,nxest,nyest, */
/*    *  nx,tx,ny,ty,c,fp,wrk,lwrk,iwrk,kwrk,ier) */

/* parameters: */
/*  iopt  : integer flag. on entry iopt must specify whether a least- */
/*          squares spline (iopt=-1) or a smoothing spline (iopt=0 or 1) */
/*          must be determined. */
/*          if iopt=0 the routine will start with an initial set of knots */
/*          tx(i)=xb,tx(i+kx+1)=xe,i=1,...,kx+1;ty(i)=yb,ty(i+ky+1)=ye,i= */
/*          1,...,ky+1. if iopt=1 the routine will continue with the set */
/*          of knots found at the last call of the routine. */
/*          attention: a call with iopt=1 must always be immediately pre- */
/*                     ceded by another call with iopt=1 or iopt=0 and */
/*                     s.ne.0. */
/*          unchanged on exit. */
/*  mx    : integer. on entry mx must specify the number of grid points */
/*          along the x-axis. mx > kx . unchanged on exit. */
/*  x     : real array of dimension at least (mx). before entry, x(i) */
/*          must be set to the x-co-ordinate of the i-th grid point */
/*          along the x-axis, for i=1,2,...,mx. these values must be */
/*          supplied in strictly ascending order. unchanged on exit. */
/*  my    : integer. on entry my must specify the number of grid points */
/*          along the y-axis. my > ky . unchanged on exit. */
/*  y     : real array of dimension at least (my). before entry, y(j) */
/*          must be set to the y-co-ordinate of the j-th grid point */
/*          along the y-axis, for j=1,2,...,my. these values must be */
/*          supplied in strictly ascending order. unchanged on exit. */
/*  z     : real array of dimension at least (mx*my). */
/*          before entry, z(my*(i-1)+j) must be set to the data value at */
/*          the grid point (x(i),y(j)) for i=1,...,mx and j=1,...,my. */
/*          unchanged on exit. */
/*  xb,xe : real values. on entry xb,xe,yb and ye must specify the bound- */
/*  yb,ye   aries of the rectangular approximation domain. */
/*          xb<=x(i)<=xe,i=1,...,mx; yb<=y(j)<=ye,j=1,...,my. */
/*          unchanged on exit. */
/*  kx,ky : integer values. on entry kx and ky must specify the degrees */
/*          of the spline. 1<=kx,ky<=5. it is recommended to use bicubic */
/*          (kx=ky=3) splines. unchanged on exit. */
/*  s     : real. on entry (in case iopt>=0) s must specify the smoothing */
/*          factor. s >=0. unchanged on exit. */
/*          for advice on the choice of s see further comments */
/*  nxest : integer. unchanged on exit. */
/*  nyest : integer. unchanged on exit. */
/*          on entry, nxest and nyest must specify an upper bound for the */
/*          number of knots required in the x- and y-directions respect. */
/*          these numbers will also determine the storage space needed by */
/*          the routine. nxest >= 2*(kx+1), nyest >= 2*(ky+1). */
/*          in most practical situation nxest = mx/2, nyest=my/2, will */
/*          be sufficient. always large enough are nxest=mx+kx+1, nyest= */
/*          my+ky+1, the number of knots needed for interpolation (s=0). */
/*          see also further comments. */
/*  nx    : integer. */
/*          unless ier=10 (in case iopt >=0), nx will contain the total */
/*          number of knots with respect to the x-variable, of the spline */
/*          approximation returned. if the computation mode iopt=1 is */
/*          used, the value of nx should be left unchanged between sub- */
/*          sequent calls. */
/*          in case iopt=-1, the value of nx should be specified on entry */
/*  tx    : real array of dimension nmax. */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the x-variable, i.e. the position of */
/*          the interior knots tx(kx+2),...,tx(nx-kx-1) as well as the */
/*          position of the additional knots tx(1)=...=tx(kx+1)=xb and */
/*          tx(nx-kx)=...=tx(nx)=xe needed for the b-spline representat. */
/*          if the computation mode iopt=1 is used, the values of tx(1), */
/*          ...,tx(nx) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt=-1 is used, the values tx(kx+2), */
/*          ...tx(nx-kx-1) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  ny    : integer. */
/*          unless ier=10 (in case iopt >=0), ny will contain the total */
/*          number of knots with respect to the y-variable, of the spline */
/*          approximation returned. if the computation mode iopt=1 is */
/*          used, the value of ny should be left unchanged between sub- */
/*          sequent calls. */
/*          in case iopt=-1, the value of ny should be specified on entry */
/*  ty    : real array of dimension nmax. */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the y-variable, i.e. the position of */
/*          the interior knots ty(ky+2),...,ty(ny-ky-1) as well as the */
/*          position of the additional knots ty(1)=...=ty(ky+1)=yb and */
/*          ty(ny-ky)=...=ty(ny)=ye needed for the b-spline representat. */
/*          if the computation mode iopt=1 is used, the values of ty(1), */
/*          ...,ty(ny) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt=-1 is used, the values ty(ky+2), */
/*          ...ty(ny-ky-1) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  c     : real array of dimension at least (nxest-kx-1)*(nyest-ky-1). */
/*          on succesful exit, c contains the coefficients of the spline */
/*          approximation s(x,y) */
/*  fp    : real. unless ier=10, fp contains the sum of squared */
/*          residuals of the spline approximation returned. */
/*  wrk   : real array of dimension (lwrk). used as workspace. */
/*          if the computation mode iopt=1 is used the values of wrk(1), */
/*          ...,wrk(4) should be left unchanged between subsequent calls. */
/*  lwrk  : integer. on entry lwrk must specify the actual dimension of */
/*          the array wrk as declared in the calling (sub)program. */
/*          lwrk must not be too small. */
/*           lwrk >= 4+nxest*(my+2*kx+5)+nyest*(2*ky+5)+mx*(kx+1)+ */
/*            my*(ky+1) +u */
/*           where u is the larger of my and nxest. */
/*  iwrk  : integer array of dimension (kwrk). used as workspace. */
/*          if the computation mode iopt=1 is used the values of iwrk(1), */
/*          ...,iwrk(3) should be left unchanged between subsequent calls */
/*  kwrk  : integer. on entry kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. */
/*          kwrk >= 3+mx+my+nxest+nyest. */
/*  ier   : integer. unless the routine detects an error, ier contains a */
/*          non-positive value on exit, i.e. */
/*   ier=0  : normal return. the spline returned has a residual sum of */
/*            squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*            ive tolerance set to 0.001 by the program. */
/*   ier=-1 : normal return. the spline returned is an interpolating */
/*            spline (fp=0). */
/*   ier=-2 : normal return. the spline returned is the least-squares */
/*            polynomial of degrees kx and ky. in this extreme case fp */
/*            gives the upper bound for the smoothing factor s. */
/*   ier=1  : error. the required storage space exceeds the available */
/*            storage space, as specified by the parameters nxest and */
/*            nyest. */
/*            probably causes : nxest or nyest too small. if these param- */
/*            eters are already large, it may also indicate that s is */
/*            too small */
/*            the approximation returned is the least-squares spline */
/*            according to the current set of knots. the parameter fp */
/*            gives the corresponding sum of squared residuals (fp>s). */
/*   ier=2  : error. a theoretically impossible result was found during */
/*            the iteration proces for finding a smoothing spline with */
/*            fp = s. probably causes : s too small. */
/*            there is an approximation returned but the corresponding */
/*            sum of squared residuals does not satisfy the condition */
/*            abs(fp-s)/s < tol. */
/*   ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*            by the program) allowed for finding a smoothing spline */
/*            with fp=s has been reached. probably causes : s too small */
/*            there is an approximation returned but the corresponding */
/*            sum of squared residuals does not satisfy the condition */
/*            abs(fp-s)/s < tol. */
/*   ier=10 : error. on entry, the input data are controlled on validity */
/*            the following restrictions must be satisfied. */
/*            -1<=iopt<=1, 1<=kx,ky<=5, mx>kx, my>ky, nxest>=2*kx+2, */
/*            nyest>=2*ky+2, kwrk>=3+mx+my+nxest+nyest, */
/*            lwrk >= 4+nxest*(my+2*kx+5)+nyest*(2*ky+5)+mx*(kx+1)+ */
/*             my*(ky+1) +max(my,nxest), */
/*            xb<=x(i-1)<x(i)<=xe,i=2,..,mx,yb<=y(j-1)<y(j)<=ye,j=2,..,my */
/*            if iopt=-1: 2*kx+2<=nx<=min(nxest,mx+kx+1) */
/*                        xb<tx(kx+2)<tx(kx+3)<...<tx(nx-kx-1)<xe */
/*                        2*ky+2<=ny<=min(nyest,my+ky+1) */
/*                        yb<ty(ky+2)<ty(ky+3)<...<ty(ny-ky-1)<ye */
/*                    the schoenberg-whitney conditions, i.e. there must */
/*                    be subset of grid co-ordinates xx(p) and yy(q) such */
/*                    that   tx(p) < xx(p) < tx(p+kx+1) ,p=1,...,nx-kx-1 */
/*                           ty(q) < yy(q) < ty(q+ky+1) ,q=1,...,ny-ky-1 */
/*            if iopt>=0: s>=0 */
/*                        if s=0 : nxest>=mx+kx+1, nyest>=my+ky+1 */
/*            if one of these conditions is found to be violated,control */
/*            is immediately repassed to the calling program. in that */
/*            case there is no approximation returned. */

/* further comments: */
/*   regrid does not allow individual weighting of the data-values. */
/*   so, if these were determined to widely different accuracies, then */
/*   perhaps the general data set routine surfit should rather be used */
/*   in spite of efficiency. */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the spline will be too smooth and signal will be */
/*   lost ; if s is too small the spline will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating spline if */
/*   s=0 and the least-squares polynomial (degrees kx,ky) if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the accuracy of the data values. */
/*   if the user has an idea of the statistical errors on the data, he */
/*   can also find a proper estimate for s. for, by assuming that, if he */
/*   specifies the right s, regrid will return a spline s(x,y) which */
/*   exactly reproduces the function underlying the data he can evaluate */
/*   the sum((z(i,j)-s(x(i),y(j)))**2) to find a good estimate for this s */
/*   for example, if he knows that the statistical errors on his z(i,j)- */
/*   values is not greater than 0.1, he may expect that a good s should */
/*   have a value not larger than mx*my*(0.1)**2. */
/*   if nothing is known about the statistical error in z(i,j), s must */
/*   be determined by trial and error, taking account of the comments */
/*   above. the best is then to start with a very large value of s (to */
/*   determine the least-squares polynomial and the corresponding upper */
/*   bound fp0 for s) and then to progressively decrease the value of s */
/*   ( say by a factor 10 in the beginning, i.e. s=fp0/10,fp0/100,... */
/*   and more carefully as the approximation shows more detail) to */
/*   obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt=1 the program will continue with the set of knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if regrid is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   function underlying the data. if the computation mode iopt=1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt=1, the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   regrid once more with the selected value for s but now with iopt=0. */
/*   indeed, regrid may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */
/*   the number of knots may also depend on the upper bounds nxest and */
/*   nyest. indeed, if at a certain stage in regrid the number of knots */
/*   in one direction (say nx) has reached the value of its upper bound */
/*   (nxest), then from that moment on all subsequent knots are added */
/*   in the other (y) direction. this may indicate that the value of */
/*   nxest is too small. on the other hand, it gives the user the option */
/*   of limiting the number of knots the routine locates in any direction */
/*   for example, by setting nxest=2*kx+2 (the lowest allowable value for */
/*   nxest), the user can indicate that he wants an approximation which */
/*   is a simple polynomial of degree kx in the variable x. */

/*  other subroutines required: */
/*    fpback,fpbspl,fpregr,fpdisc,fpgivs,fpgrre,fprati,fprota,fpchec, */
/*    fpknot */

/*  references: */
/*   dierckx p. : a fast algorithm for smoothing data on a rectangular */
/*                grid while using spline functions, siam j.numer.anal. */
/*                19 (1982) 1286-1304. */
/*   dierckx p. : a fast algorithm for smoothing data on a rectangular */
/*                grid while using spline functions, report tw53, dept. */
/*                computer science,k.u.leuven, 1980. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : may 1979 */
/*  latest update : march 1989 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpregr,fpchec */
/*  .. */
/*  we set up the parameters tol and maxit. */
    /* Parameter adjustments */
    --x;
    --z__;
    --y;
    --tx;
    --c__;
    --ty;
    --wrk;
    --iwrk;

    /* Function Body */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (*kx <= 0 || *kx > 5) {
	goto L70;
    }
    kx1 = *kx + 1;
    kx2 = kx1 + 1;
    if (*ky <= 0 || *ky > 5) {
	goto L70;
    }
    ky1 = *ky + 1;
    ky2 = ky1 + 1;
    if (*iopt < -1 || *iopt > 1) {
	goto L70;
    }
    nminx = kx1 << 1;
    if (*mx < kx1 || *nxest < nminx) {
	goto L70;
    }
    nminy = ky1 << 1;
    if (*my < ky1 || *nyest < nminy) {
	goto L70;
    }
    mz = *mx * *my;
    nc = (*nxest - kx1) * (*nyest - ky1);
    lwest = *nxest * (*my + (kx2 << 1) + 1) + 4 + *nyest * ((ky2 << 1) + 1) + 
	    *mx * kx1 + *my * ky1 + max(*nxest,*my);
    kwest = *mx + 3 + *my + *nxest + *nyest;
    if (*lwrk < lwest || *kwrk < kwest) {
	goto L70;
    }
    if (*xb > x[1] || *xe < x[*mx]) {
	goto L70;
    }
    i__1 = *mx;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (x[i__ - 1] >= x[i__]) {
	    goto L70;
	}
/* L10: */
    }
    if (*yb > y[1] || *ye < y[*my]) {
	goto L70;
    }
    i__1 = *my;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (y[i__ - 1] >= y[i__]) {
	    goto L70;
	}
/* L20: */
    }
    if (*iopt >= 0) {
	goto L50;
    }
    if (*nx < nminx || *nx > *nxest) {
	goto L70;
    }
    j = *nx;
    i__1 = kx1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	tx[i__] = *xb;
	tx[j] = *xe;
	--j;
/* L30: */
    }
    fpchec_(&x[1], mx, &tx[1], nx, kx, ier);
    if (*ier != 0) {
	goto L70;
    }
    if (*ny < nminy || *ny > *nyest) {
	goto L70;
    }
    j = *ny;
    i__1 = ky1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ty[i__] = *yb;
	ty[j] = *ye;
	--j;
/* L40: */
    }
    fpchec_(&y[1], my, &ty[1], ny, ky, ier);
    if (*ier != 0) {
	goto L70;
    } else {
	goto L60;
    }
L50:
    if (*s < (float)0.) {
	goto L70;
    }
    if (*s == (float)0. && (*nxest < *mx + kx1 || *nyest < *my + ky1)) {
	goto L70;
    }
    *ier = 0;
/*  we partition the working space and determine the spline approximation */
L60:
    lfpx = 5;
    lfpy = lfpx + *nxest;
    lww = lfpy + *nyest;
    jwrk = *lwrk - 4 - *nxest - *nyest;
    knrx = 4;
    knry = knrx + *mx;
    kndx = knry + *my;
    kndy = kndx + *nxest;
    fpregr_(iopt, &x[1], mx, &y[1], my, &z__[1], &mz, xb, xe, yb, ye, kx, ky, 
	    s, nxest, nyest, &tol, &maxit, &nc, nx, &tx[1], ny, &ty[1], &c__[
	    1], fp, &wrk[1], &wrk[2], &wrk[3], &wrk[4], &wrk[lfpx], &wrk[lfpy]
	    , &iwrk[1], &iwrk[2], &iwrk[3], &iwrk[knrx], &iwrk[knry], &iwrk[
	    kndx], &iwrk[kndy], &wrk[lww], &jwrk, ier);
L70:
    return 0;
} /* regrid_ */

/* Subroutine */ int spalde_(t, n, c__, k1, x, d__, ier)
real *t;
integer *n;
real *c__;
integer *k1;
real *x, *d__;
integer *ier;
{
    static integer l;
    extern /* Subroutine */ int fpader_();
    static integer nk1;

/*  subroutine spalde evaluates at a point x all the derivatives */
/*              (j-1) */
/*      d(j) = s     (x) , j=1,2,...,k1 */
/*  of a spline s(x) of order k1 (degree k=k1-1), given in its b-spline */
/*  representation. */

/*  calling sequence: */
/*     call spalde(t,n,c,k1,x,d,ier) */

/*  input parameters: */
/*    t    : array,length n, which contains the position of the knots. */
/*    n    : integer, giving the total number of knots of s(x). */
/*    c    : array,length n, which contains the b-spline coefficients. */
/*    k1   : integer, giving the order of s(x) (order=degree+1) */
/*    x    : real, which contains the point where the derivatives must */
/*           be evaluated. */

/*  output parameters: */
/*    d    : array,length k1, containing the derivative values of s(x). */
/*    ier  : error flag */
/*      ier = 0 : normal return */
/*      ier =10 : invalid input data (see restrictions) */

/*  restrictions: */
/*    t(k1) <= x <= t(n-k1+1) */

/*  further comments: */
/*    if x coincides with a knot, right derivatives are computed */
/*    ( left derivatives if x = t(n-k1+1) ). */

/*  other subroutines required: fpader. */

/*  references : */
/*    de boor c : on calculating with b-splines, j. approximation theory */
/*                6 (1972) 50-62. */
/*    cox m.g.  : the numerical evaluation of b-splines, j. inst. maths */
/*                applics 10 (1972) 134-149. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --c__;
    --t;
    --d__;

    /* Function Body */
    *ier = 10;
    nk1 = *n - *k1;
    if (*x < t[*k1] || *x > t[nk1 + 1]) {
	goto L300;
    }
/*  search for knot interval t(l) <= x < t(l+1) */
    l = *k1;
L100:
    if (*x < t[l + 1] || l == nk1) {
	goto L200;
    }
    ++l;
    goto L100;
L200:
    if (t[l] >= t[l + 1]) {
	goto L300;
    }
    *ier = 0;
/*  calculate the derivatives. */
    fpader_(&t[1], n, &c__[1], k1, x, &l, &d__[1]);
L300:
    return 0;
} /* spalde_ */

/* Subroutine */ int spgrid_(iopt, ider, mu, u, mv, v, r__, r0, r1, s, nuest, 
	nvest, nu, tu, nv, tv, c__, fp, wrk, lwrk, iwrk, kwrk, ier)
integer *iopt, *ider, *mu;
real *u;
integer *mv;
real *v, *r__, *r0, *r1, *s;
integer *nuest, *nvest, *nu;
real *tu;
integer *nv;
real *tv, *c__, *fp, *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double atan2();

    /* Local variables */
    static real half;
    static integer kndu, kndv;
    static real rmin;
    static integer lfpu;
    static real rmax;
    static integer lfpv, jwrk, knru, knrv, i__, j, l, m, maxit, mumin, i1, i2,
	     j1, j2, kwest, lwest, nc;
    static real rb, re;
    extern /* Subroutine */ int fpchec_();
    static real pi, ve, rn;
    extern /* Subroutine */ int fpchep_();
    static real uu;
    extern /* Subroutine */ int fpspgr_();
    static real one;
    static integer ldr;
    static real per, tol;
    static integer muu, lww;

/*  given the function values r(i,j) on the latitude-longitude grid */
/*  (u(i),v(j)), i=1,...,mu ; j=1,...,mv , spgrid determines a smooth */
/*  bicubic spline approximation on the rectangular domain 0<=u<=pi, */
/*  vb<=v<=ve (vb = v(1), ve=vb+2*pi). */
/*  this approximation s(u,v) will satisfy the properties */

/*    (1) s(0,v) = s(0,0) = dr(1) */

/*        d s(0,v)           d s(0,0)           d s(0,pi/2) */
/*    (2) -------- = cos(v)* -------- + sin(v)* ----------- */
/*        d u                d u                d u */

/*                 = cos(v)*dr(2)+sin(v)*dr(3) */
/*                                                     vb <= v <= ve */
/*    (3) s(pi,v) = s(pi,0) = dr(4) */

/*        d s(pi,v)           d s(pi,0)           d s(pi,pi/2) */
/*    (4) -------- = cos(v)*  --------- + sin(v)* ------------ */
/*        d u                 d u                 d u */

/*                 = cos(v)*dr(5)+sin(v)*dr(6) */

/*  and will be periodic in the variable v, i.e. */

/*         j           j */
/*        d s(u,vb)   d s(u,ve) */
/*    (5) --------- = ---------   0 <=u<= pi , j=0,1,2 */
/*           j           j */
/*        d v         d v */

/*  the number of knots of s(u,v) and their position tu(i),i=1,2,...,nu; */
/*  tv(j),j=1,2,...,nv, is chosen automatically by the routine. the */
/*  smoothness of s(u,v) is achieved by minimalizing the discontinuity */
/*  jumps of the derivatives of the spline at the knots. the amount of */
/*  smoothness of s(u,v) is determined by the condition that */
/*  fp=sumi=1,mu(sumj=1,mv((r(i,j)-s(u(i),v(j)))**2))+(r0-s(0,v))**2 */
/*  + (r1-s(pi,v))**2 <= s, with s a given non-negative constant. */
/*  the fit s(u,v) is given in its b-spline representation and can be */
/*  evaluated by means of routine bispev */

/* calling sequence: */
/*     call spgrid(iopt,ider,mu,u,mv,v,r,r0,r1,s,nuest,nvest,nu,tu, */
/*    *  ,nv,tv,c,fp,wrk,lwrk,iwrk,kwrk,ier) */

/* parameters: */
/*  iopt  : integer array of dimension 3, specifying different options. */
/*          unchanged on exit. */
/*  iopt(1):on entry iopt(1) must specify whether a least-squares spline */
/*          (iopt(1)=-1) or a smoothing spline (iopt(1)=0 or 1) must be */
/*          determined. */
/*          if iopt(1)=0 the routine will start with an initial set of */
/*          knots tu(i)=0,tu(i+4)=pi,i=1,...,4;tv(i)=v(1)+(i-4)*2*pi, */
/*          i=1,...,8. */
/*          if iopt(1)=1 the routine will continue with the set of knots */
/*          found at the last call of the routine. */
/*          attention: a call with iopt(1)=1 must always be immediately */
/*          preceded by another call with iopt(1) = 1 or iopt(1) = 0. */
/*  iopt(2):on entry iopt(2) must specify the requested order of conti- */
/*          nuity at the pole u=0. */
/*          if iopt(2)=0 only condition (1) must be fulfilled and */
/*          if iopt(2)=1 conditions (1)+(2) must be fulfilled. */
/*  iopt(3):on entry iopt(3) must specify the requested order of conti- */
/*          nuity at the pole u=pi. */
/*          if iopt(3)=0 only condition (3) must be fulfilled and */
/*          if iopt(3)=1 conditions (3)+(4) must be fulfilled. */
/*  ider  : integer array of dimension 4, specifying different options. */
/*          unchanged on exit. */
/*  ider(1):on entry ider(1) must specify whether (ider(1)=0 or 1) or not */
/*          (ider(1)=-1) there is a data value r0 at the pole u=0. */
/*          if ider(1)=1, r0 will be considered to be the right function */
/*          value, and it will be fitted exactly (s(0,v)=r0). */
/*          if ider(1)=0, r0 will be considered to be a data value just */
/*          like the other data values r(i,j). */
/*  ider(2):on entry ider(2) must specify whether (ider(2)=1) or not */
/*          (ider(2)=0) the approximation has vanishing derivatives */
/*          dr(2) and dr(3) at the pole u=0  (in case iopt(2)=1) */
/*  ider(3):on entry ider(3) must specify whether (ider(3)=0 or 1) or not */
/*          (ider(3)=-1) there is a data value r1 at the pole u=pi. */
/*          if ider(3)=1, r1 will be considered to be the right function */
/*          value, and it will be fitted exactly (s(pi,v)=r1). */
/*          if ider(3)=0, r1 will be considered to be a data value just */
/*          like the other data values r(i,j). */
/*  ider(4):on entry ider(4) must specify whether (ider(4)=1) or not */
/*          (ider(4)=0) the approximation has vanishing derivatives */
/*          dr(5) and dr(6) at the pole u=pi (in case iopt(3)=1) */
/*  mu    : integer. on entry mu must specify the number of grid points */
/*          along the u-axis. unchanged on exit. */
/*          mu >= 1, mu >=mumin=4-i0-i1-ider(2)-ider(4) with */
/*            i0=min(1,ider(1)+1), i1=min(1,ider(3)+1) */
/*  u     : real array of dimension at least (mu). before entry, u(i) */
/*          must be set to the u-co-ordinate of the i-th grid point */
/*          along the u-axis, for i=1,2,...,mu. these values must be */
/*          supplied in strictly ascending order. unchanged on exit. */
/*          0 < u(i) < pi. */
/*  mv    : integer. on entry mv must specify the number of grid points */
/*          along the v-axis. mv > 3 . unchanged on exit. */
/*  v     : real array of dimension at least (mv). before entry, v(j) */
/*          must be set to the v-co-ordinate of the j-th grid point */
/*          along the v-axis, for j=1,2,...,mv. these values must be */
/*          supplied in strictly ascending order. unchanged on exit. */
/*          -pi <= v(1) < pi , v(mv) < v(1)+2*pi. */
/*  r     : real array of dimension at least (mu*mv). */
/*          before entry, r(mv*(i-1)+j) must be set to the data value at */
/*          the grid point (u(i),v(j)) for i=1,...,mu and j=1,...,mv. */
/*          unchanged on exit. */
/*  r0    : real value. on entry (if ider(1) >=0 ) r0 must specify the */
/*          data value at the pole u=0. unchanged on exit. */
/*  r1    : real value. on entry (if ider(1) >=0 ) r1 must specify the */
/*          data value at the pole u=pi. unchanged on exit. */
/*  s     : real. on entry (if iopt(1)>=0) s must specify the smoothing */
/*          factor. s >=0. unchanged on exit. */
/*          for advice on the choice of s see further comments */
/*  nuest : integer. unchanged on exit. */
/*  nvest : integer. unchanged on exit. */
/*          on entry, nuest and nvest must specify an upper bound for the */
/*          number of knots required in the u- and v-directions respect. */
/*          these numbers will also determine the storage space needed by */
/*          the routine. nuest >= 8, nvest >= 8. */
/*          in most practical situation nuest = mu/2, nvest=mv/2, will */
/*          be sufficient. always large enough are nuest=mu+6+iopt(2)+ */
/*          iopt(3), nvest = mv+7, the number of knots needed for */
/*          interpolation (s=0). see also further comments. */
/*  nu    : integer. */
/*          unless ier=10 (in case iopt(1)>=0), nu will contain the total */
/*          number of knots with respect to the u-variable, of the spline */
/*          approximation returned. if the computation mode iopt(1)=1 is */
/*          used, the value of nu should be left unchanged between sub- */
/*          sequent calls. in case iopt(1)=-1, the value of nu should be */
/*          specified on entry. */
/*  tu    : real array of dimension at least (nuest). */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the u-variable, i.e. the position of */
/*          the interior knots tu(5),...,tu(nu-4) as well as the position */
/*          of the additional knots tu(1)=...=tu(4)=0 and tu(nu-3)=...= */
/*          tu(nu)=pi needed for the b-spline representation. */
/*          if the computation mode iopt(1)=1 is used,the values of tu(1) */
/*          ...,tu(nu) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt(1)=-1 is used, the values tu(5), */
/*          ...tu(nu-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  nv    : integer. */
/*          unless ier=10 (in case iopt(1)>=0), nv will contain the total */
/*          number of knots with respect to the v-variable, of the spline */
/*          approximation returned. if the computation mode iopt(1)=1 is */
/*          used, the value of nv should be left unchanged between sub- */
/*          sequent calls. in case iopt(1) = -1, the value of nv should */
/*          be specified on entry. */
/*  tv    : real array of dimension at least (nvest). */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the v-variable, i.e. the position of */
/*          the interior knots tv(5),...,tv(nv-4) as well as the position */
/*          of the additional knots tv(1),...,tv(4) and tv(nv-3),..., */
/*          tv(nv) needed for the b-spline representation. */
/*          if the computation mode iopt(1)=1 is used,the values of tv(1) */
/*          ...,tv(nv) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt(1)=-1 is used, the values tv(5), */
/*          ...tv(nv-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  c     : real array of dimension at least (nuest-4)*(nvest-4). */
/*          on succesful exit, c contains the coefficients of the spline */
/*          approximation s(u,v) */
/*  fp    : real. unless ier=10, fp contains the sum of squared */
/*          residuals of the spline approximation returned. */
/*  wrk   : real array of dimension (lwrk). used as workspace. */
/*          if the computation mode iopt(1)=1 is used the values of */
/*          wrk(1),..,wrk(12) should be left unchanged between subsequent */
/*          calls. */
/*  lwrk  : integer. on entry lwrk must specify the actual dimension of */
/*          the array wrk as declared in the calling (sub)program. */
/*          lwrk must not be too small. */
/*           lwrk >= 12+nuest*(mv+nvest+3)+nvest*24+4*mu+8*mv+q */
/*           where q is the larger of (mv+nvest) and nuest. */
/*  iwrk  : integer array of dimension (kwrk). used as workspace. */
/*          if the computation mode iopt(1)=1 is used the values of */
/*          iwrk(1),.,iwrk(5) should be left unchanged between subsequent */
/*          calls. */
/*  kwrk  : integer. on entry kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. */
/*          kwrk >= 5+mu+mv+nuest+nvest. */
/*  ier   : integer. unless the routine detects an error, ier contains a */
/*          non-positive value on exit, i.e. */
/*   ier=0  : normal return. the spline returned has a residual sum of */
/*            squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*            ive tolerance set to 0.001 by the program. */
/*   ier=-1 : normal return. the spline returned is an interpolating */
/*            spline (fp=0). */
/*   ier=-2 : normal return. the spline returned is the least-squares */
/*            constrained polynomial. in this extreme case fp gives the */
/*            upper bound for the smoothing factor s. */
/*   ier=1  : error. the required storage space exceeds the available */
/*            storage space, as specified by the parameters nuest and */
/*            nvest. */
/*            probably causes : nuest or nvest too small. if these param- */
/*            eters are already large, it may also indicate that s is */
/*            too small */
/*            the approximation returned is the least-squares spline */
/*            according to the current set of knots. the parameter fp */
/*            gives the corresponding sum of squared residuals (fp>s). */
/*   ier=2  : error. a theoretically impossible result was found during */
/*            the iteration proces for finding a smoothing spline with */
/*            fp = s. probably causes : s too small. */
/*            there is an approximation returned but the corresponding */
/*            sum of squared residuals does not satisfy the condition */
/*            abs(fp-s)/s < tol. */
/*   ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*            by the program) allowed for finding a smoothing spline */
/*            with fp=s has been reached. probably causes : s too small */
/*            there is an approximation returned but the corresponding */
/*            sum of squared residuals does not satisfy the condition */
/*            abs(fp-s)/s < tol. */
/*   ier=10 : error. on entry, the input data are controlled on validity */
/*            the following restrictions must be satisfied. */
/*            -1<=iopt(1)<=1, 0<=iopt(2)<=1, 0<=iopt(3)<=1, */
/*            -1<=ider(1)<=1, 0<=ider(2)<=1, ider(2)=0 if iopt(2)=0. */
/*            -1<=ider(3)<=1, 0<=ider(4)<=1, ider(4)=0 if iopt(3)=0. */
/*            mu >= mumin (see above), mv >= 4, nuest >=8, nvest >= 8, */
/*            kwrk>=5+mu+mv+nuest+nvest, */
/*            lwrk >= 12+nuest*(mv+nvest+3)+nvest*24+4*mu+8*mv+ */
/*             max(nuest,mv+nvest) */
/*            0< u(i-1)<u(i)< pi,i=2,..,mu, */
/*            -pi<=v(1)< pi, v(1)<v(i-1)<v(i)<v(1)+2*pi, i=3,...,mv */
/*            if iopt(1)=-1: 8<=nu<=min(nuest,mu+6+iopt(2)+iopt(3)) */
/*                           0<tu(5)<tu(6)<...<tu(nu-4)< pi */
/*                           8<=nv<=min(nvest,mv+7) */
/*                           v(1)<tv(5)<tv(6)<...<tv(nv-4)<v(1)+2*pi */
/*                    the schoenberg-whitney conditions, i.e. there must */
/*                    be subset of grid co-ordinates uu(p) and vv(q) such */
/*                    that   tu(p) < uu(p) < tu(p+4) ,p=1,...,nu-4 */
/*                     (iopt(2)=1 and iopt(3)=1 also count for a uu-value */
/*                           tv(q) < vv(q) < tv(q+4) ,q=1,...,nv-4 */
/*                     (vv(q) is either a value v(j) or v(j)+2*pi) */
/*            if iopt(1)>=0: s>=0 */
/*                       if s=0: nuest>=mu+6+iopt(2)+iopt(3), nvest>=mv+7 */
/*            if one of these conditions is found to be violated,control */
/*            is immediately repassed to the calling program. in that */
/*            case there is no approximation returned. */

/* further comments: */
/*   spgrid does not allow individual weighting of the data-values. */
/*   so, if these were determined to widely different accuracies, then */
/*   perhaps the general data set routine sphere should rather be used */
/*   in spite of efficiency. */
/*   by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the spline will be too smooth and signal will be */
/*   lost ; if s is too small the spline will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating spline if */
/*   s=0 and the constrained least-squares polynomial(degrees 3,0)if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the accuracy of the data values. */
/*   if the user has an idea of the statistical errors on the data, he */
/*   can also find a proper estimate for s. for, by assuming that, if he */
/*   specifies the right s, spgrid will return a spline s(u,v) which */
/*   exactly reproduces the function underlying the data he can evaluate */
/*   the sum((r(i,j)-s(u(i),v(j)))**2) to find a good estimate for this s */
/*   for example, if he knows that the statistical errors on his r(i,j)- */
/*   values is not greater than 0.1, he may expect that a good s should */
/*   have a value not larger than mu*mv*(0.1)**2. */
/*   if nothing is known about the statistical error in r(i,j), s must */
/*   be determined by trial and error, taking account of the comments */
/*   above. the best is then to start with a very large value of s (to */
/*   determine the least-squares polynomial and the corresponding upper */
/*   bound fp0 for s) and then to progressively decrease the value of s */
/*   ( say by a factor 10 in the beginning, i.e. s=fp0/10,fp0/100,... */
/*   and more carefully as the approximation shows more detail) to */
/*   obtain closer fits. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt(1)=0. */
/*   if iopt(1) = 1 the program will continue with the knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if spgrid is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   function underlying the data. if the computation mode iopt(1) = 1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt(1)=1,the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   spgrid once more with the chosen value for s but now with iopt(1)=0. */
/*   indeed, spgrid may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */
/*   the number of knots may also depend on the upper bounds nuest and */
/*   nvest. indeed, if at a certain stage in spgrid the number of knots */
/*   in one direction (say nu) has reached the value of its upper bound */
/*   (nuest), then from that moment on all subsequent knots are added */
/*   in the other (v) direction. this may indicate that the value of */
/*   nuest is too small. on the other hand, it gives the user the option */
/*   of limiting the number of knots the routine locates in any direction */
/*   for example, by setting nuest=8 (the lowest allowable value for */
/*   nuest), the user can indicate that he wants an approximation which */
/*   is a simple cubic polynomial in the variable u. */

/*  other subroutines required: */
/*    fpspgr,fpchec,fpchep,fpknot,fpopsp,fprati,fpgrsp,fpsysy,fpback, */
/*    fpbacp,fpbspl,fpcyt1,fpcyt2,fpdisc,fpgivs,fprota */

/*  references: */
/*   dierckx p. : fast algorithms for smoothing data over a disc or a */
/*                sphere using tensor product splines, in "algorithms */
/*                for approximation", ed. j.c.mason and m.g.cox, */
/*                clarendon press oxford, 1987, pp. 51-65 */
/*   dierckx p. : fast algorithms for smoothing data over a disc or a */
/*                sphere using tensor product splines, report tw73, dept. */
/*                computer science,k.u.leuven, 1985. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : july 1985 */
/*  latest update : march 1989 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpchec,fpchep,fpspgr */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --iopt;
    --ider;
    --u;
    --r__;
    --v;
    --tu;
    --c__;
    --tv;
    --wrk;
    --iwrk;

    /* Function Body */
    one = (float)1.;
    half = (float).5;
    pi = atan2((float)0., -one);
    per = pi + pi;
    ve = v[1] + per;
/*  we set up the parameters tol and maxit. */
    maxit = 20;
    tol = (float).001;
/*  before starting computations, a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    *ier = 10;
    if (iopt[1] < -1 || iopt[1] > 1) {
	goto L200;
    }
    if (iopt[2] < 0 || iopt[2] > 1) {
	goto L200;
    }
    if (iopt[3] < 0 || iopt[3] > 1) {
	goto L200;
    }
    if (ider[1] < -1 || ider[1] > 1) {
	goto L200;
    }
    if (ider[2] < 0 || ider[2] > 1) {
	goto L200;
    }
    if (ider[2] == 1 && iopt[2] == 0) {
	goto L200;
    }
    if (ider[3] < -1 || ider[3] > 1) {
	goto L200;
    }
    if (ider[4] < 0 || ider[4] > 1) {
	goto L200;
    }
    if (ider[4] == 1 && iopt[3] == 0) {
	goto L200;
    }
    mumin = 4;
    if (ider[1] >= 0) {
	--mumin;
    }
    if (iopt[2] == 1 && ider[2] == 1) {
	--mumin;
    }
    if (ider[3] >= 0) {
	--mumin;
    }
    if (iopt[3] == 1 && ider[4] == 1) {
	--mumin;
    }
    if (mumin == 0) {
	mumin = 1;
    }
    if (*mu < mumin || *mv < 4) {
	goto L200;
    }
    if (*nuest < 8 || *nvest < 8) {
	goto L200;
    }
    m = *mu * *mv;
    nc = (*nuest - 4) * (*nvest - 4);
/* Computing MAX */
    i__1 = *nuest, i__2 = *mv + *nvest;
    lwest = *nuest * (*mv + *nvest + 3) + 12 + *nvest * 24 + (*mu << 2) + (*
	    mv << 3) + max(i__1,i__2);
    kwest = *mu + 5 + *mv + *nuest + *nvest;
    if (*lwrk < lwest || *kwrk < kwest) {
	goto L200;
    }
    if (u[1] <= (float)0. || u[*mu] >= pi) {
	goto L200;
    }
    if (*mu == 1) {
	goto L30;
    }
    i__1 = *mu;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (u[i__ - 1] >= u[i__]) {
	    goto L200;
	}
/* L20: */
    }
L30:
    if (v[1] < -pi || v[1] >= pi) {
	goto L200;
    }
    if (v[*mv] >= v[1] + per) {
	goto L200;
    }
    i__1 = *mv;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (v[i__ - 1] >= v[i__]) {
	    goto L200;
	}
/* L40: */
    }
    if (iopt[1] > 0) {
	goto L140;
    }
/*  if not given, we compute an estimate for r0. */
    rn = (real) (*mv);
    if (ider[1] < 0) {
	goto L45;
    }
    rb = *r0;
    goto L55;
L45:
    rb = (float)0.;
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	rb += r__[i__];
/* L50: */
    }
    rb /= rn;
/*  if not given, we compute an estimate for r1. */
L55:
    if (ider[3] < 0) {
	goto L60;
    }
    re = *r1;
    goto L70;
L60:
    re = (float)0.;
    j = m;
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	re += r__[j];
	--j;
/* L65: */
    }
    re /= rn;
/*  we determine the range of r-values. */
L70:
    rmin = rb;
    rmax = re;
    i__1 = m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (r__[i__] < rmin) {
	    rmin = r__[i__];
	}
	if (r__[i__] > rmax) {
	    rmax = r__[i__];
	}
/* L80: */
    }
    wrk[5] = rb;
    wrk[6] = (float)0.;
    wrk[7] = (float)0.;
    wrk[8] = re;
    wrk[9] = (float)0.;
    wrk[10] = (float)0.;
    wrk[11] = rmax - rmin;
    wrk[12] = wrk[11];
    iwrk[4] = *mu;
    iwrk[5] = *mu;
    if (iopt[1] == 0) {
	goto L140;
    }
    if (*nu < 8 || *nu > *nuest) {
	goto L200;
    }
    if (*nv < 11 || *nv > *nvest) {
	goto L200;
    }
    j = *nu;
    for (i__ = 1; i__ <= 4; ++i__) {
	tu[i__] = (float)0.;
	tu[j] = pi;
	--j;
/* L90: */
    }
    l = 13;
    wrk[l] = (float)0.;
    if (iopt[2] == 0) {
	goto L100;
    }
    ++l;
    uu = u[1];
    if (uu > tu[5]) {
	uu = tu[5];
    }
    wrk[l] = uu * half;
L100:
    i__1 = *mu;
    for (i__ = 1; i__ <= i__1; ++i__) {
	++l;
	wrk[l] = u[i__];
/* L110: */
    }
    if (iopt[3] == 0) {
	goto L120;
    }
    ++l;
    uu = u[*mu];
    if (uu < tu[*nu - 4]) {
	uu = tu[*nu - 4];
    }
    wrk[l] = uu + (pi - uu) * half;
L120:
    ++l;
    wrk[l] = pi;
    muu = l - 12;
    fpchec_(&wrk[13], &muu, &tu[1], nu, &c__3, ier);
    if (*ier != 0) {
	goto L200;
    }
    j1 = 4;
    tv[j1] = v[1];
    i1 = *nv - 3;
    tv[i1] = ve;
    j2 = j1;
    i2 = i1;
    for (i__ = 1; i__ <= 3; ++i__) {
	++i1;
	--i2;
	++j1;
	--j2;
	tv[j2] = tv[i2] - per;
	tv[i1] = tv[j1] + per;
/* L130: */
    }
    l = 13;
    i__1 = *mv;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wrk[l] = v[i__];
	++l;
/* L135: */
    }
    wrk[l] = ve;
    i__1 = *mv + 1;
    fpchep_(&wrk[13], &i__1, &tv[1], nv, &c__3, ier);
    if (*ier != 0) {
	goto L200;
    } else {
	goto L150;
    }
L140:
    if (*s < (float)0.) {
	goto L200;
    }
    if (*s == (float)0. && (*nuest < *mu + 6 + iopt[2] + iopt[3] || *nvest < *
	    mv + 7)) {
	goto L200;
    }
/*  we partition the working space and determine the spline approximation */
L150:
    ldr = 5;
    lfpu = 13;
    lfpv = lfpu + *nuest;
    lww = lfpv + *nvest;
    jwrk = *lwrk - 12 - *nuest - *nvest;
    knru = 6;
    knrv = knru + *mu;
    kndu = knrv + *mv;
    kndv = kndu + *nuest;
    fpspgr_(&iopt[1], &ider[1], &u[1], mu, &v[1], mv, &r__[1], &m, &rb, &re, 
	    s, nuest, nvest, &tol, &maxit, &nc, nu, &tu[1], nv, &tv[1], &c__[
	    1], fp, &wrk[1], &wrk[2], &wrk[3], &wrk[4], &wrk[lfpu], &wrk[lfpv]
	    , &wrk[ldr], &wrk[11], &iwrk[1], &iwrk[2], &iwrk[3], &iwrk[4], &
	    iwrk[5], &iwrk[knru], &iwrk[knrv], &iwrk[kndu], &iwrk[kndv], &wrk[
	    lww], &jwrk, ier);
L200:
    return 0;
} /* spgrid_ */

/* Subroutine */ int sphere_(iopt, m, teta, phi, r__, w, s, ntest, npest, eps,
	 nt, tt, np, tp, c__, fp, wrk1, lwrk1, wrk2, lwrk2, iwrk, kwrk, ier)
integer *iopt, *m;
real *teta, *phi, *r__, *w, *s;
integer *ntest, *npest;
real *eps;
integer *nt;
real *tt;
integer *np;
real *tp, *c__, *fp, *wrk1;
integer *lwrk1;
real *wrk2;
integer *lwrk2, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double atan();

    /* Local variables */
    static integer ncof, nreg, i__, j, ncest, maxit, nrint, kwest, lwest, la, 
	    lf, ki, lh;
    static real pi;
    static integer kn, lq;
    extern /* Subroutine */ int fpsphe_();
    static integer ib1, ib3;
    static real pi2;
    static integer np4, nt4, lcc, ncc, lff, lco, lbp;
    static real one;
    static integer lbt, lcs, lfp, lro, npp;
    static real tol;
    static integer lsp, lst, ntt;

/*  subroutine sphere determines a smooth bicubic spherical spline */
/*  approximation s(teta,phi), 0 <= teta <= pi ; 0 <= phi <= 2*pi */
/*  to a given set of data points (teta(i),phi(i),r(i)),i=1,2,...,m. */
/*  such a spline has the following specific properties */

/*    (1) s(0,phi)  = constant   0 <=phi<= 2*pi. */

/*    (2) s(pi,phi) = constant   0 <=phi<= 2*pi */

/*         j             j */
/*        d s(teta,0)   d s(teta,2*pi) */
/*    (3) ----------- = ------------   0 <=teta<=pi, j=0,1,2 */
/*             j             j */
/*        d phi         d phi */

/*        d s(0,phi)    d s(0,0)             d s(0,pi/2) */
/*    (4) ----------  = -------- *cos(phi) + ----------- *sin(phi) */
/*        d teta        d teta               d teta */

/*        d s(pi,phi)   d s(pi,0)            d s(pi,pi/2) */
/*    (5) ----------- = ---------*cos(phi) + ------------*sin(phi) */
/*        d teta        d teta               d teta */

/*  if iopt =-1 sphere calculates a weighted least-squares spherical */
/*  spline according to a given set of knots in teta- and phi- direction. */
/*  if iopt >=0, the number of knots in each direction and their position */
/*  tt(j),j=1,2,...,nt ; tp(j),j=1,2,...,np are chosen automatically by */
/*  the routine. the smoothness of s(teta,phi) is then achieved by mini- */
/*  malizing the discontinuity jumps of the derivatives of the spline */
/*  at the knots. the amount of smoothness of s(teta,phi) is determined */
/*  by the condition that fp = sum((w(i)*(r(i)-s(teta(i),phi(i))))**2) */
/*  be <= s, with s a given non-negative constant. */
/*  the spherical spline is given in the standard b-spline representation */
/*  of bicubic splines and can be evaluated by means of subroutine bispev */

/* calling sequence: */
/*     call sphere(iopt,m,teta,phi,r,w,s,ntest,npest,eps, */
/*    *  nt,tt,np,tp,c,fp,wrk1,lwrk1,wrk2,lwrk2,iwrk,kwrk,ier) */

/* parameters: */
/*  iopt  : integer flag. on entry iopt must specify whether a weighted */
/*          least-squares spherical spline (iopt=-1) or a smoothing */
/*          spherical spline (iopt=0 or 1) must be determined. */
/*          if iopt=0 the routine will start with an initial set of knots */
/*          tt(i)=0,tt(i+4)=pi,i=1,...,4;tp(i)=0,tp(i+4)=2*pi,i=1,...,4. */
/*          if iopt=1 the routine will continue with the set of knots */
/*          found at the last call of the routine. */
/*          attention: a call with iopt=1 must always be immediately pre- */
/*                     ceded by another call with iopt=1 or iopt=0. */
/*          unchanged on exit. */
/*  m     : integer. on entry m must specify the number of data points. */
/*          m >= 2. unchanged on exit. */
/*  teta  : real array of dimension at least (m). */
/*  phi   : real array of dimension at least (m). */
/*  r     : real array of dimension at least (m). */
/*          before entry,teta(i),phi(i),r(i) must be set to the spherical */
/*          co-ordinates of the i-th data point, for i=1,...,m.the order */
/*          of the data points is immaterial. unchanged on exit. */
/*  w     : real array of dimension at least (m). before entry, w(i) must */
/*          be set to the i-th value in the set of weights. the w(i) must */
/*          be strictly positive. unchanged on exit. */
/*  s     : real. on entry (in case iopt>=0) s must specify the smoothing */
/*          factor. s >=0. unchanged on exit. */
/*          for advice on the choice of s see further comments */
/*  ntest : integer. unchanged on exit. */
/*  npest : integer. unchanged on exit. */
/*          on entry, ntest and npest must specify an upper bound for the */
/*          number of knots required in the teta- and phi-directions. */
/*          these numbers will also determine the storage space needed by */
/*          the routine. ntest >= 8, npest >= 8. */
/*          in most practical situation ntest = npest = 8+sqrt(m/2) will */
/*          be sufficient. see also further comments. */
/*  eps   : real. */
/*          on entry, eps must specify a threshold for determining the */
/*          effective rank of an over-determined linear system of equat- */
/*          ions. 0 < eps < 1.  if the number of decimal digits in the */
/*          computer representation of a real number is q, then 10**(-q) */
/*          is a suitable value for eps in most practical applications. */
/*          unchanged on exit. */
/*  nt    : integer. */
/*          unless ier=10 (in case iopt >=0), nt will contain the total */
/*          number of knots with respect to the teta-variable, of the */
/*          spline approximation returned. if the computation mode iopt=1 */
/*          is used, the value of nt should be left unchanged between */
/*          subsequent calls. */
/*          in case iopt=-1, the value of nt should be specified on entry */
/*  tt    : real array of dimension at least ntest. */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the teta-variable, i.e. the position */
/*          of the interior knots tt(5),...,tt(nt-4) as well as the */
/*          position of the additional knots tt(1)=...=tt(4)=0 and */
/*          tt(nt-3)=...=tt(nt)=pi needed for the b-spline representation */
/*          if the computation mode iopt=1 is used, the values of tt(1), */
/*          ...,tt(nt) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt=-1 is used, the values tt(5), */
/*          ...tt(nt-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  np    : integer. */
/*          unless ier=10 (in case iopt >=0), np will contain the total */
/*          number of knots with respect to the phi-variable, of the */
/*          spline approximation returned. if the computation mode iopt=1 */
/*          is used, the value of np should be left unchanged between */
/*          subsequent calls. */
/*          in case iopt=-1, the value of np (>=9) should be specified */
/*          on entry. */
/*  tp    : real array of dimension at least npest. */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the phi-variable, i.e. the position of */
/*          the interior knots tp(5),...,tp(np-4) as well as the position */
/*          of the additional knots tp(1),...,tp(4) and tp(np-3),..., */
/*          tp(np) needed for the b-spline representation. */
/*          if the computation mode iopt=1 is used, the values of tp(1), */
/*          ...,tp(np) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt=-1 is used, the values tp(5), */
/*          ...tp(np-4) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  c     : real array of dimension at least (ntest-4)*(npest-4). */
/*          on succesful exit, c contains the coefficients of the spline */
/*          approximation s(teta,phi). */
/*  fp    : real. unless ier=10, fp contains the weighted sum of */
/*          squared residuals of the spline approximation returned. */
/*  wrk1  : real array of dimension (lwrk1). used as workspace. */
/*          if the computation mode iopt=1 is used the value of wrk1(1) */
/*          should be left unchanged between subsequent calls. */
/*          on exit wrk1(2),wrk1(3),...,wrk1(1+ncof) will contain the */
/*          values d(i)/max(d(i)),i=1,...,ncof=6+(np-7)*(nt-8) */
/*          with d(i) the i-th diagonal element of the reduced triangular */
/*          matrix for calculating the b-spline coefficients. it includes */
/*          those elements whose square is less than eps,which are treat- */
/*          ed as 0 in the case of presumed rank deficiency (ier<-2). */
/*  lwrk1 : integer. on entry lwrk1 must specify the actual dimension of */
/*          the array wrk1 as declared in the calling (sub)program. */
/*          lwrk1 must not be too small. let */
/*            u = ntest-7, v = npest-7, then */
/*          lwrk1 >= 185+52*v+10*u+14*u*v+8*(u-1)*v**2+8*m */
/*  wrk2  : real array of dimension (lwrk2). used as workspace, but */
/*          only in the case a rank deficient system is encountered. */
/*  lwrk2 : integer. on entry lwrk2 must specify the actual dimension of */
/*          the array wrk2 as declared in the calling (sub)program. */
/*          lwrk2 > 0 . a save upper bound  for lwrk2 = 48+21*v+7*u*v+ */
/*          4*(u-1)*v**2 where u,v are as above. if there are enough data */
/*          points, scattered uniformly over the approximation domain */
/*          and if the smoothing factor s is not too small, there is a */
/*          good chance that this extra workspace is not needed. a lot */
/*          of memory might therefore be saved by setting lwrk2=1. */
/*          (see also ier > 10) */
/*  iwrk  : integer array of dimension (kwrk). used as workspace. */
/*  kwrk  : integer. on entry kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. */
/*          kwrk >= m+(ntest-7)*(npest-7). */
/*  ier   : integer. unless the routine detects an error, ier contains a */
/*          non-positive value on exit, i.e. */
/*   ier=0  : normal return. the spline returned has a residual sum of */
/*            squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*            ive tolerance set to 0.001 by the program. */
/*   ier=-1 : normal return. the spline returned is a spherical */
/*            interpolating spline (fp=0). */
/*   ier=-2 : normal return. the spline returned is the weighted least- */
/*            squares constrained polynomial . in this extreme case */
/*            fp gives the upper bound for the smoothing factor s. */
/*   ier<-2 : warning. the coefficients of the spline returned have been */
/*            computed as the minimal norm least-squares solution of a */
/*            (numerically) rank deficient system. (-ier) gives the rank. */
/*            especially if the rank deficiency which can be computed as */
/*            6+(nt-8)*(np-7)+ier, is large the results may be inaccurate */
/*            they could also seriously depend on the value of eps. */
/*   ier=1  : error. the required storage space exceeds the available */
/*            storage space, as specified by the parameters ntest and */
/*            npest. */
/*            probably causes : ntest or npest too small. if these param- */
/*            eters are already large, it may also indicate that s is */
/*            too small */
/*            the approximation returned is the weighted least-squares */
/*            spherical spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=2  : error. a theoretically impossible result was found during */
/*            the iteration proces for finding a smoothing spline with */
/*            fp = s. probably causes : s too small or badly chosen eps. */
/*            there is an approximation returned but the corresponding */
/*            weighted sum of squared residuals does not satisfy the */
/*            condition abs(fp-s)/s < tol. */
/*   ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*            by the program) allowed for finding a smoothing spline */
/*            with fp=s has been reached. probably causes : s too small */
/*            there is an approximation returned but the corresponding */
/*            weighted sum of squared residuals does not satisfy the */
/*            condition abs(fp-s)/s < tol. */
/*   ier=4  : error. no more knots can be added because the dimension */
/*            of the spherical spline 6+(nt-8)*(np-7) already exceeds */
/*            the number of data points m. */
/*            probably causes : either s or m too small. */
/*            the approximation returned is the weighted least-squares */
/*            spherical spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=5  : error. no more knots can be added because the additional */
/*            knot would (quasi) coincide with an old one. */
/*            probably causes : s too small or too large a weight to an */
/*            inaccurate data point. */
/*            the approximation returned is the weighted least-squares */
/*            spherical spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=10 : error. on entry, the input data are controlled on validity */
/*            the following restrictions must be satisfied. */
/*            -1<=iopt<=1,  m>=2, ntest>=8 ,npest >=8, 0<eps<1, */
/*            0<=teta(i)<=pi, 0<=phi(i)<=2*pi, w(i)>0, i=1,...,m */
/*            lwrk1 >= 185+52*v+10*u+14*u*v+8*(u-1)*v**2+8*m */
/*            kwrk >= m+(ntest-7)*(npest-7) */
/*            if iopt=-1: 8<=nt<=ntest , 9<=np<=npest */
/*                        0<tt(5)<tt(6)<...<tt(nt-4)<pi */
/*                        0<tp(5)<tp(6)<...<tp(np-4)<2*pi */
/*            if iopt>=0: s>=0 */
/*            if one of these conditions is found to be violated,control */
/*            is immediately repassed to the calling program. in that */
/*            case there is no approximation returned. */
/*   ier>10 : error. lwrk2 is too small, i.e. there is not enough work- */
/*            space for computing the minimal least-squares solution of */
/*            a rank deficient system of linear equations. ier gives the */
/*            requested value for lwrk2. there is no approximation re- */
/*            turned but, having saved the information contained in nt, */
/*            np,tt,tp,wrk1, and having adjusted the value of lwrk2 and */
/*            the dimension of the array wrk2 accordingly, the user can */
/*            continue at the point the program was left, by calling */
/*            sphere with iopt=1. */

/* further comments: */
/*  by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the spline will be too smooth and signal will be */
/*   lost ; if s is too small the spline will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating spline if */
/*   s=0 and the constrained weighted least-squares polynomial if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the weights w(i). if these are */
/*   taken as 1/d(i) with d(i) an estimate of the standard deviation of */
/*   r(i), a good s-value should be found in the range (m-sqrt(2*m),m+ */
/*   sqrt(2*m)). if nothing is known about the statistical error in r(i) */
/*   each w(i) can be set equal to one and s determined by trial and */
/*   error, taking account of the comments above. the best is then to */
/*   start with a very large value of s ( to determine the least-squares */
/*   polynomial and the corresponding upper bound fp0 for s) and then to */
/*   progressively decrease the value of s ( say by a factor 10 in the */
/*   beginning, i.e. s=fp0/10, fp0/100,...and more carefully as the */
/*   approximation shows more detail) to obtain closer fits. */
/*   to choose s very small is strongly discouraged. this considerably */
/*   increases computation time and memory requirements. it may also */
/*   cause rank-deficiency (ier<-2) and endager numerical stability. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt=1 the program will continue with the set of knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if sphere is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   function underlying the data. if the computation mode iopt=1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt=1, the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   sphere once more with the selected value for s but now with iopt=0. */
/*   indeed, sphere may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */
/*   the number of knots may also depend on the upper bounds ntest and */
/*   npest. indeed, if at a certain stage in sphere the number of knots */
/*   in one direction (say nt) has reached the value of its upper bound */
/*   (ntest), then from that moment on all subsequent knots are added */
/*   in the other (phi) direction. this may indicate that the value of */
/*   ntest is too small. on the other hand, it gives the user the option */
/*   of limiting the number of knots the routine locates in any direction */
/*   for example, by setting ntest=8 (the lowest allowable value for */
/*   ntest), the user can indicate that he wants an approximation which */
/*   is a cubic polynomial in the variable teta. */

/*  other subroutines required: */
/*    fpback,fpbspl,fpsphe,fpdisc,fpgivs,fprank,fprati,fprota,fporde, */
/*    fprpsp */

/*  references: */
/*   dierckx p. : algorithms for smoothing data on the sphere with tensor */
/*                product splines, computing 32 (1984) 319-342. */
/*   dierckx p. : algorithms for smoothing data on the sphere with tensor */
/*                product splines, report tw62, dept. computer science, */
/*                k.u.leuven, 1983. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : july 1983 */
/*  latest update : march 1989 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpsphe */
/*  .. */
/*  set constants */
    /* Parameter adjustments */
    --w;
    --r__;
    --phi;
    --teta;
    --tt;
    --c__;
    --tp;
    --wrk1;
    --wrk2;
    --iwrk;

    /* Function Body */
    one = (float)1.;
/*  we set up the parameters tol and maxit. */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid,control is immediately repassed to the calling program. */
    *ier = 10;
    if (*eps <= (float)0. || *eps >= (float)1.) {
	goto L80;
    }
    if (*iopt < -1 || *iopt > 1) {
	goto L80;
    }
    if (*m < 2) {
	goto L80;
    }
    if (*ntest < 8 || *npest < 8) {
	goto L80;
    }
    nt4 = *ntest - 4;
    np4 = *npest - 4;
    ncest = nt4 * np4;
    ntt = *ntest - 7;
    npp = *npest - 7;
    ncc = npp * (ntt - 1) + 6;
    nrint = ntt + npp;
    nreg = ntt * npp;
    ncof = npp * 3 + 6;
    ib1 = npp << 2;
    ib3 = ib1 + 3;
    if (ncof > ib1) {
	ib1 = ncof;
    }
    if (ncof > ib3) {
	ib3 = ncof;
    }
/* Computing 2nd power */
    i__1 = npp;
    lwest = npp * 52 + 185 + ntt * 10 + ntt * 14 * npp + (*m + (ntt - 1) * (
	    i__1 * i__1) << 3);
    kwest = *m + nreg;
    if (*lwrk1 < lwest || *kwrk < kwest) {
	goto L80;
    }
    if (*iopt > 0) {
	goto L60;
    }
    pi = atan(one) * 4;
    pi2 = pi + pi;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (w[i__] <= (float)0.) {
	    goto L80;
	}
	if (teta[i__] < (float)0. || teta[i__] > pi) {
	    goto L80;
	}
	if (phi[i__] < (float)0. || phi[i__] > pi2) {
	    goto L80;
	}
/* L20: */
    }
    if (*iopt == 0) {
	goto L60;
    }
    ntt = *nt - 8;
    if (ntt < 0 || *nt > *ntest) {
	goto L80;
    }
    if (ntt == 0) {
	goto L40;
    }
    tt[4] = (float)0.;
    i__1 = ntt;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = i__ + 4;
	if (tt[j] <= tt[j - 1] || tt[j] >= pi) {
	    goto L80;
	}
/* L30: */
    }
L40:
    npp = *np - 8;
    if (npp < 1 || *np > *npest) {
	goto L80;
    }
    tp[4] = (float)0.;
    i__1 = npp;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = i__ + 4;
	if (tp[j] <= tp[j - 1] || tp[j] >= pi2) {
	    goto L80;
	}
/* L50: */
    }
    goto L70;
L60:
    if (*s < (float)0.) {
	goto L80;
    }
L70:
    *ier = 0;
/*  we partition the working space and determine the spline approximation */
    kn = 1;
    ki = kn + *m;
    lq = 2;
    la = lq + ncc * ib3;
    lf = la + ncc * ib1;
    lff = lf + ncc;
    lfp = lff + ncest;
    lco = lfp + nrint;
    lh = lco + nrint;
    lbt = lh + ib3;
    lbp = lbt + *ntest * 5;
    lro = lbp + *npest * 5;
    lcc = lro + *npest;
    lcs = lcc + *npest;
    lst = lcs + *npest;
    lsp = lst + (*m << 2);
    fpsphe_(iopt, m, &teta[1], &phi[1], &r__[1], &w[1], s, ntest, npest, eps, 
	    &tol, &maxit, &ib1, &ib3, &ncest, &ncc, &nrint, &nreg, nt, &tt[1],
	     np, &tp[1], &c__[1], fp, &wrk1[1], &wrk1[lfp], &wrk1[lco], &wrk1[
	    lf], &wrk1[lff], &wrk1[lro], &wrk1[lcc], &wrk1[lcs], &wrk1[la], &
	    wrk1[lq], &wrk1[lbt], &wrk1[lbp], &wrk1[lst], &wrk1[lsp], &wrk1[
	    lh], &iwrk[ki], &iwrk[kn], &wrk2[1], lwrk2, ier);
L80:
    return 0;
} /* sphere_ */

/* Subroutine */ int splder_(t, n, c__, k, nu, x, y, m, wrk, ier)
real *t;
integer *n;
real *c__;
integer *k, *nu;
real *x, *y;
integer *m;
real *wrk;
integer *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static real h__[6];
    static integer i__, j, l, k1, k2, l1, l2;
    static real ak;
    static integer kk;
    static real tb;
    static integer ll;
    static real te;
    static integer nn;
    static real sp;
    extern /* Subroutine */ int fpbspl_();
    static integer nk1, nk2;
    static real fac, arg;

/*  subroutine splder evaluates in a number of points x(i),i=1,2,...,m */
/*  the derivative of order nu of a spline s(x) of degree k,given in */
/*  its b-spline representation. */

/*  calling sequence: */
/*     call splder(t,n,c,k,nu,x,y,m,wrk,ier) */

/*  input parameters: */
/*    t    : array,length n, which contains the position of the knots. */
/*    n    : integer, giving the total number of knots of s(x). */
/*    c    : array,length n, which contains the b-spline coefficients. */
/*    k    : integer, giving the degree of s(x). */
/*    nu   : integer, specifying the order of the derivative. 0<=nu<=k */
/*    x    : array,length m, which contains the points where the deriv- */
/*           ative of s(x) must be evaluated. */
/*    m    : integer, giving the number of points where the derivative */
/*           of s(x) must be evaluated */
/*    wrk  : real array of dimension n. used as working space. */

/*  output parameters: */
/*    y    : array,length m, giving the value of the derivative of s(x) */
/*           at the different points. */
/*    ier  : error flag */
/*      ier = 0 : normal return */
/*      ier =10 : invalid input data (see restrictions) */

/*  restrictions: */
/*    0 <= nu <= k */
/*    m >= 1 */
/*    t(k+1) <= x(i) <= x(i+1) <= t(n-k) , i=1,2,...,m-1. */

/*  other subroutines required: fpbspl */

/*  references : */
/*    de boor c : on calculating with b-splines, j. approximation theory */
/*                6 (1972) 50-62. */
/*    cox m.g.  : the numerical evaluation of b-splines, j. inst. maths */
/*                applics 10 (1972) 134-149. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local arrays .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --wrk;
    --c__;
    --t;
    --y;
    --x;

    /* Function Body */
    *ier = 10;
    if (*nu < 0 || *nu > *k) {
	goto L200;
    }
    if ((i__1 = *m - 1) < 0) {
	goto L200;
    } else if (i__1 == 0) {
	goto L30;
    } else {
	goto L10;
    }
L10:
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (x[i__] < x[i__ - 1]) {
	    goto L200;
	}
/* L20: */
    }
L30:
    *ier = 0;
/*  fetch tb and te, the boundaries of the approximation interval. */
    k1 = *k + 1;
    nk1 = *n - k1;
    tb = t[k1];
    te = t[nk1 + 1];
/*  the derivative of order nu of a spline of degree k is a spline of */
/*  degree k-nu,the b-spline coefficients wrk(i) of which can be found */
/*  using the recurrence scheme of de boor. */
    l = 1;
    kk = *k;
    nn = *n;
    i__1 = nk1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wrk[i__] = c__[i__];
/* L40: */
    }
    if (*nu == 0) {
	goto L100;
    }
    nk2 = nk1;
    i__1 = *nu;
    for (j = 1; j <= i__1; ++j) {
	ak = (real) kk;
	--nk2;
	l1 = l;
	i__2 = nk2;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ++l1;
	    l2 = l1 + kk;
	    fac = t[l2] - t[l1];
	    if (fac <= (float)0.) {
		goto L50;
	    }
	    wrk[i__] = ak * (wrk[i__ + 1] - wrk[i__]) / fac;
L50:
	    ;
	}
	++l;
	--kk;
/* L60: */
    }
    if (kk != 0) {
	goto L100;
    }
/*  if nu=k the derivative is a piecewise constant function */
    j = 1;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	arg = x[i__];
L70:
	if (arg < t[l + 1] || l == nk1) {
	    goto L80;
	}
	++l;
	++j;
	goto L70;
L80:
	y[i__] = wrk[j];
/* L90: */
    }
    goto L200;
L100:
    l = k1;
    l1 = l + 1;
    k2 = k1 - *nu;
/*  main loop for the different points. */
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*  fetch a new x-value arg. */
	arg = x[i__];
	if (arg < tb) {
	    arg = tb;
	}
	if (arg > te) {
	    arg = te;
	}
/*  search for knot interval t(l) <= arg < t(l+1) */
L140:
	if (arg < t[l1] || l == nk1) {
	    goto L150;
	}
	l = l1;
	l1 = l + 1;
	goto L140;
/*  evaluate the non-zero b-splines of degree k-nu at arg. */
L150:
	fpbspl_(&t[1], n, &kk, &arg, &l, h__);
/*  find the value of the derivative at x=arg. */
	sp = (float)0.;
	ll = l - k1;
	i__2 = k2;
	for (j = 1; j <= i__2; ++j) {
	    ++ll;
	    sp += wrk[ll] * h__[j - 1];
/* L160: */
	}
	y[i__] = sp;
/* L180: */
    }
L200:
    return 0;
} /* splder_ */

/* Subroutine */ int splev_(t, n, c__, k, x, y, m, ier)
real *t;
integer *n;
real *c__;
integer *k;
real *x, *y;
integer *m, *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static real h__[6];
    static integer i__, j, l, k1, l1;
    static real tb;
    static integer ll;
    static real te, sp;
    extern /* Subroutine */ int fpbspl_();
    static integer nk1;
    static real arg;

/*  subroutine splev evaluates in a number of points x(i),i=1,2,...,m */
/*  a spline s(x) of degree k, given in its b-spline representation. */

/*  calling sequence: */
/*     call splev(t,n,c,k,x,y,m,ier) */

/*  input parameters: */
/*    t    : array,length n, which contains the position of the knots. */
/*    n    : integer, giving the total number of knots of s(x). */
/*    c    : array,length n, which contains the b-spline coefficients. */
/*    k    : integer, giving the degree of s(x). */
/*    x    : array,length m, which contains the points where s(x) must */
/*           be evaluated. */
/*    m    : integer, giving the number of points where s(x) must be */
/*           evaluated. */

/*  output parameter: */
/*    y    : array,length m, giving the value of s(x) at the different */
/*           points. */
/*    ier  : error flag */
/*      ier = 0 : normal return */
/*      ier =10 : invalid input data (see restrictions) */

/*  restrictions: */
/*    m >= 1 */
/*    t(k+1) <= x(i) <= x(i+1) <= t(n-k) , i=1,2,...,m-1. */

/*  other subroutines required: fpbspl. */

/*  references : */
/*    de boor c  : on calculating with b-splines, j. approximation theory */
/*                 6 (1972) 50-62. */
/*    cox m.g.   : the numerical evaluation of b-splines, j. inst. maths */
/*                 applics 10 (1972) 134-149. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --c__;
    --t;
    --y;
    --x;

    /* Function Body */
    *ier = 10;
    if ((i__1 = *m - 1) < 0) {
	goto L100;
    } else if (i__1 == 0) {
	goto L30;
    } else {
	goto L10;
    }
L10:
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (x[i__] < x[i__ - 1]) {
	    goto L100;
	}
/* L20: */
    }
L30:
    *ier = 0;
/*  fetch tb and te, the boundaries of the approximation interval. */
    k1 = *k + 1;
    nk1 = *n - k1;
    tb = t[k1];
    te = t[nk1 + 1];
    l = k1;
    l1 = l + 1;
/*  main loop for the different points. */
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*  fetch a new x-value arg. */
	arg = x[i__];
	if (arg < tb) {
	    arg = tb;
	}
	if (arg > te) {
	    arg = te;
	}
/*  search for knot interval t(l) <= arg < t(l+1) */
L40:
	if (arg < t[l1] || l == nk1) {
	    goto L50;
	}
	l = l1;
	l1 = l + 1;
	goto L40;
/*  evaluate the non-zero b-splines at arg. */
L50:
	fpbspl_(&t[1], n, k, &arg, &l, h__);
/*  find the value of s(x) at x=arg. */
	sp = (float)0.;
	ll = l - k1;
	i__2 = k1;
	for (j = 1; j <= i__2; ++j) {
	    ++ll;
	    sp += c__[ll] * h__[j - 1];
/* L60: */
	}
	y[i__] = sp;
/* L80: */
    }
L100:
    return 0;
} /* splev_ */

doublereal splint_(t, n, c__, k, a, b, wrk)
real *t;
integer *n;
real *c__;
integer *k;
real *a, *b, *wrk;
{
    /* System generated locals */
    integer i__1;
    real ret_val;

    /* Local variables */
    static integer i__;
    extern /* Subroutine */ int fpintb_();
    static integer nk1;

/*  function splint calculates the integral of a spline function s(x) */
/*  of degree k, which is given in its normalized b-spline representation */

/*  calling sequence: */
/*     aint = splint(t,n,c,k,a,b,wrk) */

/*  input parameters: */
/*    t    : array,length n,which contains the position of the knots */
/*           of s(x). */
/*    n    : integer, giving the total number of knots of s(x). */
/*    c    : array,length n, containing the b-spline coefficients. */
/*    k    : integer, giving the degree of s(x). */
/*    a,b  : real values, containing the end points of the integration */
/*           interval. s(x) is considered to be identically zero outside */
/*           the interval (t(k+1),t(n-k)). */

/*  output parameter: */
/*    aint : real, containing the integral of s(x) between a and b. */
/*    wrk  : real array, length n.  used as working space */
/*           on output, wrk will contain the integrals of the normalized */
/*           b-splines defined on the set of knots. */

/*  other subroutines required: fpintb. */

/*  references : */
/*    gaffney p.w. : the calculation of indefinite integrals of b-splines */
/*                   j. inst. maths applics 17 (1976) 37-41. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
    /* Parameter adjustments */
    --wrk;
    --c__;
    --t;

    /* Function Body */
    nk1 = *n - *k - 1;
/*  calculate the integrals wrk(i) of the normalized b-splines */
/*  ni,k+1(x), i=1,2,...nk1. */
    fpintb_(&t[1], n, &wrk[1], &nk1, a, b);
/*  calculate the integral of s(x). */
    ret_val = (float)0.;
    i__1 = nk1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ret_val += c__[i__] * wrk[i__];
/* L10: */
    }
    return ret_val;
} /* splint_ */

/* Subroutine */ int sproot_(t, n, c__, zero, mest, m, ier)
real *t;
integer *n;
real *c__, *zero;
integer *mest, *m, *ier;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, l;
    static real y[3], three, a0, a1, a2, a3, b0, b1, c1, c2, c3, c4;
    static integer j1;
    static real c5, d4, d5, h1, h2;
    static integer n4;
    static real t1, t2, t3, t4, t5;
    static logical z0, z1, z2, z3, z4;
    static real ah, bh, zz;
    extern /* Subroutine */ int fpcuro_();
    static logical nz0, nz1, nz2, nz3, nz4;
    static real two;

/*  subroutine sproot finds the zeros of a cubic spline s(x),which is */
/*  given in its normalized b-spline representation. */

/*  calling sequence: */
/*     call sproot(t,n,c,zero,mest,m,ier) */

/*  input parameters: */
/*    t    : real array,length n, containing the knots of s(x). */
/*    n    : integer, containing the number of knots.  n>=8 */
/*    c    : real array,length n, containing the b-spline coefficients. */
/*    mest : integer, specifying the dimension of array zero. */

/*  output parameters: */
/*    zero : real array,lenth mest, containing the zeros of s(x). */
/*    m    : integer,giving the number of zeros. */
/*    ier  : error flag: */
/*      ier = 0: normal return. */
/*      ier = 1: the number of zeros exceeds mest. */
/*      ier =10: invalid input data (see restrictions). */

/*  other subroutines required: fpcuro */

/*  restrictions: */
/*    1) n>= 8. */
/*    2) t(4) < t(5) < ... < t(n-4) < t(n-3). */
/*       t(1) <= t(2) <= t(3) <= t(4) */
/*       t(n-3) <= t(n-2) <= t(n-1) <= t(n) */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/* .. */
/* ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..local array.. */
/*  .. */
/*  set some constants */
    /* Parameter adjustments */
    --c__;
    --t;
    --zero;

    /* Function Body */
    two = (float)2.;
    three = (float)3.;
/*  before starting computations a data check is made. if the input data */
/*  are invalid, control is immediately repassed to the calling program. */
    n4 = *n - 4;
    *ier = 10;
    if (*n < 8) {
	goto L800;
    }
    j = *n;
    for (i__ = 1; i__ <= 3; ++i__) {
	if (t[i__] > t[i__ + 1]) {
	    goto L800;
	}
	if (t[j] < t[j - 1]) {
	    goto L800;
	}
	--j;
/* L10: */
    }
    i__1 = n4;
    for (i__ = 4; i__ <= i__1; ++i__) {
	if (t[i__] >= t[i__ + 1]) {
	    goto L800;
	}
/* L20: */
    }
/*  the problem considered reduces to finding the zeros of the cubic */
/*  polynomials pl(x) which define the cubic spline in each knot */
/*  interval t(l)<=x<=t(l+1). a zero of pl(x) is also a zero of s(x) on */
/*  the condition that it belongs to the knot interval. */
/*  the cubic polynomial pl(x) is determined by computing s(t(l)), */
/*  s'(t(l)),s(t(l+1)) and s'(t(l+1)). in fact we only have to compute */
/*  s(t(l+1)) and s'(t(l+1)); because of the continuity conditions of */
/*  splines and their derivatives, the value of s(t(l)) and s'(t(l)) */
/*  is already known from the foregoing knot interval. */
    *ier = 0;
/*  evaluate some constants for the first knot interval */
    h1 = t[4] - t[3];
    h2 = t[5] - t[4];
    t1 = t[4] - t[2];
    t2 = t[5] - t[3];
    t3 = t[6] - t[4];
    t4 = t[5] - t[2];
    t5 = t[6] - t[3];
/*  calculate a0 = s(t(4)) and ah = s'(t(4)). */
    c1 = c__[1];
    c2 = c__[2];
    c3 = c__[3];
    c4 = (c2 - c1) / t4;
    c5 = (c3 - c2) / t5;
    d4 = (h2 * c1 + t1 * c2) / t4;
    d5 = (t3 * c2 + h1 * c3) / t5;
    a0 = (h2 * d4 + h1 * d5) / t2;
    ah = three * (h2 * c4 + h1 * c5) / t2;
    z1 = TRUE_;
    if (ah < (float)0.) {
	z1 = FALSE_;
    }
    nz1 = ! z1;
    *m = 0;
/*  main loop for the different knot intervals. */
    i__1 = n4;
    for (l = 4; l <= i__1; ++l) {
/*  evaluate some constants for the knot interval t(l) <= x <= t(l+1). */
	h1 = h2;
	h2 = t[l + 2] - t[l + 1];
	t1 = t2;
	t2 = t3;
	t3 = t[l + 3] - t[l + 1];
	t4 = t5;
	t5 = t[l + 3] - t[l];
/*  find a0 = s(t(l)), ah = s'(t(l)), b0 = s(t(l+1)) and bh = s'(t(l+1)). */
	c1 = c2;
	c2 = c3;
	c3 = c__[l];
	c4 = c5;
	c5 = (c3 - c2) / t5;
	d4 = (h2 * c1 + t1 * c2) / t4;
	d5 = (h1 * c3 + t3 * c2) / t5;
	b0 = (h2 * d4 + h1 * d5) / t2;
	bh = three * (h2 * c4 + h1 * c5) / t2;
/*  calculate the coefficients a0,a1,a2 and a3 of the cubic polynomial */
/*  pl(x) = ql(y) = a0+a1*y+a2*y**2+a3*y**3 ; y = (x-t(l))/(t(l+1)-t(l)). */
	a1 = ah * h1;
	b1 = bh * h1;
	a2 = three * (b0 - a0) - b1 - two * a1;
	a3 = two * (a0 - b0) + b1 + a1;
/*  test whether or not pl(x) could have a zero in the range */
/*  t(l) <= x <= t(l+1). */
	z3 = TRUE_;
	if (b1 < (float)0.) {
	    z3 = FALSE_;
	}
	nz3 = ! z3;
	if (a0 * b0 <= (float)0.) {
	    goto L100;
	}
	z0 = TRUE_;
	if (a0 < (float)0.) {
	    z0 = FALSE_;
	}
	nz0 = ! z0;
	z2 = TRUE_;
	if (a2 < (float)0.) {
	    z2 = FALSE_;
	}
	nz2 = ! z2;
	z4 = TRUE_;
	if (a3 * (float)3. + a2 < (float)0.) {
	    z4 = FALSE_;
	}
	nz4 = ! z4;
	if (! (z0 && (nz1 && (z3 || z2 && nz4) || nz2 && z3 && z4) || nz0 && (
		z1 && (nz3 || nz2 && z4) || z2 && nz3 && nz4))) {
	    goto L200;
	}
/*  find the zeros of ql(y). */
L100:
	fpcuro_(&a3, &a2, &a1, &a0, y, &j);
	if (j == 0) {
	    goto L200;
	}
/*  find which zeros of pl(x) are zeros of s(x). */
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (y[i__ - 1] < (float)0. || y[i__ - 1] > (float)1.) {
		goto L150;
	    }
/*  test whether the number of zeros of s(x) exceeds mest. */
	    if (*m >= *mest) {
		goto L700;
	    }
	    ++(*m);
	    zero[*m] = t[l] + h1 * y[i__ - 1];
L150:
	    ;
	}
L200:
	a0 = b0;
	ah = bh;
	z1 = z3;
	nz1 = nz3;
/* L300: */
    }
/*  the zeros of s(x) are arranged in increasing order. */
    if (*m < 2) {
	goto L800;
    }
    i__1 = *m;
    for (i__ = 2; i__ <= i__1; ++i__) {
	j = i__;
L350:
	j1 = j - 1;
	if (j1 == 0) {
	    goto L400;
	}
	if (zero[j] >= zero[j1]) {
	    goto L400;
	}
	zz = zero[j];
	zero[j] = zero[j1];
	zero[j1] = zz;
	j = j1;
	goto L350;
L400:
	;
    }
    j = *m;
    *m = 1;
    i__1 = j;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (zero[i__] == zero[*m]) {
	    goto L500;
	}
	++(*m);
	zero[*m] = zero[i__];
L500:
	;
    }
    goto L800;
L700:
    *ier = 1;
L800:
    return 0;
} /* sproot_ */

/* Subroutine */ int surev_(idim, tu, nu, tv, nv, c__, u, mu, v, mv, f, mf, 
	wrk, lwrk, iwrk, kwrk, ier)
integer *idim;
real *tu;
integer *nu;
real *tv;
integer *nv;
real *c__, *u;
integer *mu;
real *v;
integer *mv;
real *f;
integer *mf;
real *wrk;
integer *lwrk, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;
    extern /* Subroutine */ int fpsuev_();
    static integer muv;

/*  subroutine surev evaluates on a grid (u(i),v(j)),i=1,...,mu; j=1,... */
/*  ,mv a bicubic spline surface of dimension idim, given in the */
/*  b-spline representation. */

/*  calling sequence: */
/*     call surev(idim,tu,nu,tv,nv,c,u,mu,v,mv,f,mf,wrk,lwrk, */
/*    * iwrk,kwrk,ier) */

/*  input parameters: */
/*   idim  : integer, specifying the dimension of the spline surface. */
/*   tu    : real array, length nu, which contains the position of the */
/*           knots in the u-direction. */
/*   nu    : integer, giving the total number of knots in the u-direction */
/*   tv    : real array, length nv, which contains the position of the */
/*           knots in the v-direction. */
/*   nv    : integer, giving the total number of knots in the v-direction */
/*   c     : real array, length (nu-4)*(nv-4)*idim, which contains the */
/*           b-spline coefficients. */
/*   u     : real array of dimension (mu). */
/*           before entry u(i) must be set to the u co-ordinate of the */
/*           i-th grid point along the u-axis. */
/*           tu(4)<=u(i-1)<=u(i)<=tu(nu-3), i=2,...,mu. */
/*   mu    : on entry mu must specify the number of grid points along */
/*           the u-axis. mu >=1. */
/*   v     : real array of dimension (mv). */
/*           before entry v(j) must be set to the v co-ordinate of the */
/*           j-th grid point along the v-axis. */
/*           tv(4)<=v(j-1)<=v(j)<=tv(nv-3), j=2,...,mv. */
/*   mv    : on entry mv must specify the number of grid points along */
/*           the v-axis. mv >=1. */
/*   mf    : on entry, mf must specify the dimension of the array f. */
/*           mf >= mu*mv*idim */
/*   wrk   : real array of dimension lwrk. used as workspace. */
/*   lwrk  : integer, specifying the dimension of wrk. */
/*           lwrk >= 4*(mu+mv) */
/*   iwrk  : integer array of dimension kwrk. used as workspace. */
/*   kwrk  : integer, specifying the dimension of iwrk. kwrk >= mu+mv. */

/*  output parameters: */
/*   f     : real array of dimension (mf). */
/*           on succesful exit f(mu*mv*(l-1)+mv*(i-1)+j) contains the */
/*           l-th co-ordinate of the bicubic spline surface at the */
/*           point (u(i),v(j)),l=1,...,idim,i=1,...,mu;j=1,...,mv. */
/*   ier   : integer error flag */
/*    ier=0 : normal return */
/*    ier=10: invalid input data (see restrictions) */

/*  restrictions: */
/*   mu >=1, mv >=1, lwrk>=4*(mu+mv), kwrk>=mu+mv , mf>=mu*mv*idim */
/*   tu(4) <= u(i-1) <= u(i) <= tu(nu-3), i=2,...,mu */
/*   tv(4) <= v(j-1) <= v(j) <= tv(nv-3), j=2,...,mv */

/*  other subroutines required: */
/*    fpsuev,fpbspl */

/*  references : */
/*    de boor c : on calculating with b-splines, j. approximation theory */
/*                6 (1972) 50-62. */
/*    cox m.g.  : the numerical evaluation of b-splines, j. inst. maths */
/*                applics 10 (1972) 134-149. */
/*    dierckx p. : curve and surface fitting with splines, monographs on */
/*                 numerical analysis, oxford university press, 1993. */

/*  author : */
/*    p.dierckx */
/*    dept. computer science, k.u.leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  latest update : march 1987 */

/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  .. */
/*  before starting computations a data check is made. if the input data */
/*  are invalid control is immediately repassed to the calling program. */
    /* Parameter adjustments */
    --tu;
    --c__;
    --tv;
    --u;
    --v;
    --f;
    --wrk;
    --iwrk;

    /* Function Body */
    *ier = 10;
    if (*mf < *mu * *mv * *idim) {
	goto L100;
    }
    muv = *mu + *mv;
    if (*lwrk < muv << 2) {
	goto L100;
    }
    if (*kwrk < muv) {
	goto L100;
    }
    if ((i__1 = *mu - 1) < 0) {
	goto L100;
    } else if (i__1 == 0) {
	goto L30;
    } else {
	goto L10;
    }
L10:
    i__1 = *mu;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (u[i__] < u[i__ - 1]) {
	    goto L100;
	}
/* L20: */
    }
L30:
    if ((i__1 = *mv - 1) < 0) {
	goto L100;
    } else if (i__1 == 0) {
	goto L60;
    } else {
	goto L40;
    }
L40:
    i__1 = *mv;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if (v[i__] < v[i__ - 1]) {
	    goto L100;
	}
/* L50: */
    }
L60:
    *ier = 0;
    fpsuev_(idim, &tu[1], nu, &tv[1], nv, &c__[1], &u[1], mu, &v[1], mv, &f[1]
	    , &wrk[1], &wrk[(*mu << 2) + 1], &iwrk[1], &iwrk[*mu + 1]);
L100:
    return 0;
} /* surev_ */

/* Subroutine */ int surfit_(iopt, m, x, y, z__, w, xb, xe, yb, ye, kx, ky, s,
	 nxest, nyest, nmax, eps, nx, tx, ny, ty, c__, fp, wrk1, lwrk1, wrk2, 
	lwrk2, iwrk, kwrk, ier)
integer *iopt, *m;
real *x, *y, *z__, *w, *xb, *xe, *yb, *ye;
integer *kx, *ky;
real *s;
integer *nxest, *nyest, *nmax;
real *eps;
integer *nx;
real *tx;
integer *ny;
real *ty, *c__, *fp, *wrk1;
integer *lwrk1;
real *wrk2;
integer *lwrk2, *iwrk, *kwrk, *ier;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer nreg, kmax, nest, i__, ncest, maxit, nminx, nminy, nrint, 
	    kwest, lwest, la, lf, ki, lh, kn, lq, ib1, jb1, ib3;
    extern /* Subroutine */ int fpsurf_();
    static integer km1, km2, kx1, ky1, lff, lco, nek, lfp, lbx, lby;
    static real tol;
    static integer nxk, nyk, nmx, nmy, lsx, lsy;

/* given the set of data points (x(i),y(i),z(i)) and the set of positive */
/* numbers w(i),i=1,...,m, subroutine surfit determines a smooth bivar- */
/* iate spline approximation s(x,y) of degrees kx and ky on the rect- */
/* angle xb <= x <= xe, yb <= y <= ye. */
/* if iopt = -1 surfit calculates the weighted least-squares spline */
/* according to a given set of knots. */
/* if iopt >= 0 the total numbers nx and ny of these knots and their */
/* position tx(j),j=1,...,nx and ty(j),j=1,...,ny are chosen automatic- */
/* ally by the routine. the smoothness of s(x,y) is then achieved by */
/* minimalizing the discontinuity jumps in the derivatives of s(x,y) */
/* across the boundaries of the subpanels (tx(i),tx(i+1))*(ty(j),ty(j+1). */
/* the amounth of smoothness is determined by the condition that f(p) = */
/* sum ((w(i)*(z(i)-s(x(i),y(i))))**2) be <= s, with s a given non-neg- */
/* ative constant, called the smoothing factor. */
/* the fit is given in the b-spline representation (b-spline coefficients */
/* c((ny-ky-1)*(i-1)+j),i=1,...,nx-kx-1;j=1,...,ny-ky-1) and can be eval- */
/* uated by means of subroutine bispev. */

/* calling sequence: */
/*     call surfit(iopt,m,x,y,z,w,xb,xe,yb,ye,kx,ky,s,nxest,nyest, */
/*    *  nmax,eps,nx,tx,ny,ty,c,fp,wrk1,lwrk1,wrk2,lwrk2,iwrk,kwrk,ier) */

/* parameters: */
/*  iopt  : integer flag. on entry iopt must specify whether a weighted */
/*          least-squares spline (iopt=-1) or a smoothing spline (iopt=0 */
/*          or 1) must be determined. */
/*          if iopt=0 the routine will start with an initial set of knots */
/*          tx(i)=xb,tx(i+kx+1)=xe,i=1,...,kx+1;ty(i)=yb,ty(i+ky+1)=ye,i= */
/*          1,...,ky+1. if iopt=1 the routine will continue with the set */
/*          of knots found at the last call of the routine. */
/*          attention: a call with iopt=1 must always be immediately pre- */
/*                     ceded by another call with iopt=1 or iopt=0. */
/*          unchanged on exit. */
/*  m     : integer. on entry m must specify the number of data points. */
/*          m >= (kx+1)*(ky+1). unchanged on exit. */
/*  x     : real array of dimension at least (m). */
/*  y     : real array of dimension at least (m). */
/*  z     : real array of dimension at least (m). */
/*          before entry, x(i),y(i),z(i) must be set to the co-ordinates */
/*          of the i-th data point, for i=1,...,m. the order of the data */
/*          points is immaterial. unchanged on exit. */
/*  w     : real array of dimension at least (m). before entry, w(i) must */
/*          be set to the i-th value in the set of weights. the w(i) must */
/*          be strictly positive. unchanged on exit. */
/*  xb,xe : real values. on entry xb,xe,yb and ye must specify the bound- */
/*  yb,ye   aries of the rectangular approximation domain. */
/*          xb<=x(i)<=xe,yb<=y(i)<=ye,i=1,...,m. unchanged on exit. */
/*  kx,ky : integer values. on entry kx and ky must specify the degrees */
/*          of the spline. 1<=kx,ky<=5. it is recommended to use bicubic */
/*          (kx=ky=3) splines. unchanged on exit. */
/*  s     : real. on entry (in case iopt>=0) s must specify the smoothing */
/*          factor. s >=0. unchanged on exit. */
/*          for advice on the choice of s see further comments */
/*  nxest : integer. unchanged on exit. */
/*  nyest : integer. unchanged on exit. */
/*          on entry, nxest and nyest must specify an upper bound for the */
/*          number of knots required in the x- and y-directions respect. */
/*          these numbers will also determine the storage space needed by */
/*          the routine. nxest >= 2*(kx+1), nyest >= 2*(ky+1). */
/*          in most practical situation nxest = kx+1+sqrt(m/2), nyest = */
/*          ky+1+sqrt(m/2) will be sufficient. see also further comments. */
/*  nmax  : integer. on entry nmax must specify the actual dimension of */
/*          the arrays tx and ty. nmax >= nxest, nmax >=nyest. */
/*          unchanged on exit. */
/*  eps   : real. */
/*          on entry, eps must specify a threshold for determining the */
/*          effective rank of an over-determined linear system of equat- */
/*          ions. 0 < eps < 1.  if the number of decimal digits in the */
/*          computer representation of a real number is q, then 10**(-q) */
/*          is a suitable value for eps in most practical applications. */
/*          unchanged on exit. */
/*  nx    : integer. */
/*          unless ier=10 (in case iopt >=0), nx will contain the total */
/*          number of knots with respect to the x-variable, of the spline */
/*          approximation returned. if the computation mode iopt=1 is */
/*          used, the value of nx should be left unchanged between sub- */
/*          sequent calls. */
/*          in case iopt=-1, the value of nx should be specified on entry */
/*  tx    : real array of dimension nmax. */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the x-variable, i.e. the position of */
/*          the interior knots tx(kx+2),...,tx(nx-kx-1) as well as the */
/*          position of the additional knots tx(1)=...=tx(kx+1)=xb and */
/*          tx(nx-kx)=...=tx(nx)=xe needed for the b-spline representat. */
/*          if the computation mode iopt=1 is used, the values of tx(1), */
/*          ...,tx(nx) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt=-1 is used, the values tx(kx+2), */
/*          ...tx(nx-kx-1) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  ny    : integer. */
/*          unless ier=10 (in case iopt >=0), ny will contain the total */
/*          number of knots with respect to the y-variable, of the spline */
/*          approximation returned. if the computation mode iopt=1 is */
/*          used, the value of ny should be left unchanged between sub- */
/*          sequent calls. */
/*          in case iopt=-1, the value of ny should be specified on entry */
/*  ty    : real array of dimension nmax. */
/*          on succesful exit, this array will contain the knots of the */
/*          spline with respect to the y-variable, i.e. the position of */
/*          the interior knots ty(ky+2),...,ty(ny-ky-1) as well as the */
/*          position of the additional knots ty(1)=...=ty(ky+1)=yb and */
/*          ty(ny-ky)=...=ty(ny)=ye needed for the b-spline representat. */
/*          if the computation mode iopt=1 is used, the values of ty(1), */
/*          ...,ty(ny) should be left unchanged between subsequent calls. */
/*          if the computation mode iopt=-1 is used, the values ty(ky+2), */
/*          ...ty(ny-ky-1) must be supplied by the user, before entry. */
/*          see also the restrictions (ier=10). */
/*  c     : real array of dimension at least (nxest-kx-1)*(nyest-ky-1). */
/*          on succesful exit, c contains the coefficients of the spline */
/*          approximation s(x,y) */
/*  fp    : real. unless ier=10, fp contains the weighted sum of */
/*          squared residuals of the spline approximation returned. */
/*  wrk1  : real array of dimension (lwrk1). used as workspace. */
/*          if the computation mode iopt=1 is used the value of wrk1(1) */
/*          should be left unchanged between subsequent calls. */
/*          on exit wrk1(2),wrk1(3),...,wrk1(1+(nx-kx-1)*(ny-ky-1)) will */
/*          contain the values d(i)/max(d(i)),i=1,...,(nx-kx-1)*(ny-ky-1) */
/*          with d(i) the i-th diagonal element of the reduced triangular */
/*          matrix for calculating the b-spline coefficients. it includes */
/*          those elements whose square is less than eps,which are treat- */
/*          ed as 0 in the case of presumed rank deficiency (ier<-2). */
/*  lwrk1 : integer. on entry lwrk1 must specify the actual dimension of */
/*          the array wrk1 as declared in the calling (sub)program. */
/*          lwrk1 must not be too small. let */
/*            u = nxest-kx-1, v = nyest-ky-1, km = max(kx,ky)+1, */
/*            ne = max(nxest,nyest), bx = kx*v+ky+1, by = ky*u+kx+1, */
/*            if(bx.le.by) b1 = bx, b2 = b1+v-ky */
/*            if(bx.gt.by) b1 = by, b2 = b1+u-kx  then */
/*          lwrk1 >= u*v*(2+b1+b2)+2*(u+v+km*(m+ne)+ne-kx-ky)+b2+1 */
/*  wrk2  : real array of dimension (lwrk2). used as workspace, but */
/*          only in the case a rank deficient system is encountered. */
/*  lwrk2 : integer. on entry lwrk2 must specify the actual dimension of */
/*          the array wrk2 as declared in the calling (sub)program. */
/*          lwrk2 > 0 . a save upper boundfor lwrk2 = u*v*(b2+1)+b2 */
/*          where u,v and b2 are as above. if there are enough data */
/*          points, scattered uniformly over the approximation domain */
/*          and if the smoothing factor s is not too small, there is a */
/*          good chance that this extra workspace is not needed. a lot */
/*          of memory might therefore be saved by setting lwrk2=1. */
/*          (see also ier > 10) */
/*  iwrk  : integer array of dimension (kwrk). used as workspace. */
/*  kwrk  : integer. on entry kwrk must specify the actual dimension of */
/*          the array iwrk as declared in the calling (sub)program. */
/*          kwrk >= m+(nxest-2*kx-1)*(nyest-2*ky-1). */
/*  ier   : integer. unless the routine detects an error, ier contains a */
/*          non-positive value on exit, i.e. */
/*   ier=0  : normal return. the spline returned has a residual sum of */
/*            squares fp such that abs(fp-s)/s <= tol with tol a relat- */
/*            ive tolerance set to 0.001 by the program. */
/*   ier=-1 : normal return. the spline returned is an interpolating */
/*            spline (fp=0). */
/*   ier=-2 : normal return. the spline returned is the weighted least- */
/*            squares polynomial of degrees kx and ky. in this extreme */
/*            case fp gives the upper bound for the smoothing factor s. */
/*   ier<-2 : warning. the coefficients of the spline returned have been */
/*            computed as the minimal norm least-squares solution of a */
/*            (numerically) rank deficient system. (-ier) gives the rank. */
/*            especially if the rank deficiency which can be computed as */
/*            (nx-kx-1)*(ny-ky-1)+ier, is large the results may be inac- */
/*            curate. they could also seriously depend on the value of */
/*            eps. */
/*   ier=1  : error. the required storage space exceeds the available */
/*            storage space, as specified by the parameters nxest and */
/*            nyest. */
/*            probably causes : nxest or nyest too small. if these param- */
/*            eters are already large, it may also indicate that s is */
/*            too small */
/*            the approximation returned is the weighted least-squares */
/*            spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=2  : error. a theoretically impossible result was found during */
/*            the iteration proces for finding a smoothing spline with */
/*            fp = s. probably causes : s too small or badly chosen eps. */
/*            there is an approximation returned but the corresponding */
/*            weighted sum of squared residuals does not satisfy the */
/*            condition abs(fp-s)/s < tol. */
/*   ier=3  : error. the maximal number of iterations maxit (set to 20 */
/*            by the program) allowed for finding a smoothing spline */
/*            with fp=s has been reached. probably causes : s too small */
/*            there is an approximation returned but the corresponding */
/*            weighted sum of squared residuals does not satisfy the */
/*            condition abs(fp-s)/s < tol. */
/*   ier=4  : error. no more knots can be added because the number of */
/*            b-spline coefficients (nx-kx-1)*(ny-ky-1) already exceeds */
/*            the number of data points m. */
/*            probably causes : either s or m too small. */
/*            the approximation returned is the weighted least-squares */
/*            spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=5  : error. no more knots can be added because the additional */
/*            knot would (quasi) coincide with an old one. */
/*            probably causes : s too small or too large a weight to an */
/*            inaccurate data point. */
/*            the approximation returned is the weighted least-squares */
/*            spline according to the current set of knots. */
/*            the parameter fp gives the corresponding weighted sum of */
/*            squared residuals (fp>s). */
/*   ier=10 : error. on entry, the input data are controlled on validity */
/*            the following restrictions must be satisfied. */
/*            -1<=iopt<=1, 1<=kx,ky<=5, m>=(kx+1)*(ky+1), nxest>=2*kx+2, */
/*            nyest>=2*ky+2, 0<eps<1, nmax>=nxest, nmax>=nyest, */
/*            xb<=x(i)<=xe, yb<=y(i)<=ye, w(i)>0, i=1,...,m */
/*            lwrk1 >= u*v*(2+b1+b2)+2*(u+v+km*(m+ne)+ne-kx-ky)+b2+1 */
/*            kwrk >= m+(nxest-2*kx-1)*(nyest-2*ky-1) */
/*            if iopt=-1: 2*kx+2<=nx<=nxest */
/*                        xb<tx(kx+2)<tx(kx+3)<...<tx(nx-kx-1)<xe */
/*                        2*ky+2<=ny<=nyest */
/*                        yb<ty(ky+2)<ty(ky+3)<...<ty(ny-ky-1)<ye */
/*            if iopt>=0: s>=0 */
/*            if one of these conditions is found to be violated,control */
/*            is immediately repassed to the calling program. in that */
/*            case there is no approximation returned. */
/*   ier>10 : error. lwrk2 is too small, i.e. there is not enough work- */
/*            space for computing the minimal least-squares solution of */
/*            a rank deficient system of linear equations. ier gives the */
/*            requested value for lwrk2. there is no approximation re- */
/*            turned but, having saved the information contained in nx, */
/*            ny,tx,ty,wrk1, and having adjusted the value of lwrk2 and */
/*            the dimension of the array wrk2 accordingly, the user can */
/*            continue at the point the program was left, by calling */
/*            surfit with iopt=1. */

/* further comments: */
/*  by means of the parameter s, the user can control the tradeoff */
/*   between closeness of fit and smoothness of fit of the approximation. */
/*   if s is too large, the spline will be too smooth and signal will be */
/*   lost ; if s is too small the spline will pick up too much noise. in */
/*   the extreme cases the program will return an interpolating spline if */
/*   s=0 and the weighted least-squares polynomial (degrees kx,ky)if s is */
/*   very large. between these extremes, a properly chosen s will result */
/*   in a good compromise between closeness of fit and smoothness of fit. */
/*   to decide whether an approximation, corresponding to a certain s is */
/*   satisfactory the user is highly recommended to inspect the fits */
/*   graphically. */
/*   recommended values for s depend on the weights w(i). if these are */
/*   taken as 1/d(i) with d(i) an estimate of the standard deviation of */
/*   z(i), a good s-value should be found in the range (m-sqrt(2*m),m+ */
/*   sqrt(2*m)). if nothing is known about the statistical error in z(i) */
/*   each w(i) can be set equal to one and s determined by trial and */
/*   error, taking account of the comments above. the best is then to */
/*   start with a very large value of s ( to determine the least-squares */
/*   polynomial and the corresponding upper bound fp0 for s) and then to */
/*   progressively decrease the value of s ( say by a factor 10 in the */
/*   beginning, i.e. s=fp0/10, fp0/100,...and more carefully as the */
/*   approximation shows more detail) to obtain closer fits. */
/*   to choose s very small is strongly discouraged. this considerably */
/*   increases computation time and memory requirements. it may also */
/*   cause rank-deficiency (ier<-2) and endager numerical stability. */
/*   to economize the search for a good s-value the program provides with */
/*   different modes of computation. at the first call of the routine, or */
/*   whenever he wants to restart with the initial set of knots the user */
/*   must set iopt=0. */
/*   if iopt=1 the program will continue with the set of knots found at */
/*   the last call of the routine. this will save a lot of computation */
/*   time if surfit is called repeatedly for different values of s. */
/*   the number of knots of the spline returned and their location will */
/*   depend on the value of s and on the complexity of the shape of the */
/*   function underlying the data. if the computation mode iopt=1 */
/*   is used, the knots returned may also depend on the s-values at */
/*   previous calls (if these were smaller). therefore, if after a number */
/*   of trials with different s-values and iopt=1, the user can finally */
/*   accept a fit as satisfactory, it may be worthwhile for him to call */
/*   surfit once more with the selected value for s but now with iopt=0. */
/*   indeed, surfit may then return an approximation of the same quality */
/*   of fit but with fewer knots and therefore better if data reduction */
/*   is also an important objective for the user. */
/*   the number of knots may also depend on the upper bounds nxest and */
/*   nyest. indeed, if at a certain stage in surfit the number of knots */
/*   in one direction (say nx) has reached the value of its upper bound */
/*   (nxest), then from that moment on all subsequent knots are added */
/*   in the other (y) direction. this may indicate that the value of */
/*   nxest is too small. on the other hand, it gives the user the option */
/*   of limiting the number of knots the routine locates in any direction */
/*   for example, by setting nxest=2*kx+2 (the lowest allowable value for */
/*   nxest), the user can indicate that he wants an approximation which */
/*   is a simple polynomial of degree kx in the variable x. */

/*  other subroutines required: */
/*    fpback,fpbspl,fpsurf,fpdisc,fpgivs,fprank,fprati,fprota,fporde */

/*  references: */
/*   dierckx p. : an algorithm for surface fitting with spline functions */
/*                ima j. numer. anal. 1 (1981) 267-283. */
/*   dierckx p. : an algorithm for surface fitting with spline functions */
/*                report tw50, dept. computer science,k.u.leuven, 1980. */
/*   dierckx p. : curve and surface fitting with splines, monographs on */
/*                numerical analysis, oxford university press, 1993. */

/*  author: */
/*    p.dierckx */
/*    dept. computer science, k.u. leuven */
/*    celestijnenlaan 200a, b-3001 heverlee, belgium. */
/*    e-mail : Paul.Dierckx@cs.kuleuven.ac.be */

/*  creation date : may 1979 */
/*  latest update : march 1987 */

/*  .. */
/*  ..scalar arguments.. */
/*  ..array arguments.. */
/*  ..local scalars.. */
/*  ..function references.. */
/*  ..subroutine references.. */
/*    fpsurf */
/*  .. */
/*  we set up the parameters tol and maxit. */
    /* Parameter adjustments */
    --w;
    --z__;
    --y;
    --x;
    --c__;
    --ty;
    --tx;
    --wrk1;
    --wrk2;
    --iwrk;

    /* Function Body */
    maxit = 20;
    tol = (float).001;
/*  before starting computations a data check is made. if the input data */
/*  are invalid,control is immediately repassed to the calling program. */
    *ier = 10;
    if (*eps <= (float)0. || *eps >= (float)1.) {
	goto L70;
    }
    if (*kx <= 0 || *kx > 5) {
	goto L70;
    }
    kx1 = *kx + 1;
    if (*ky <= 0 || *ky > 5) {
	goto L70;
    }
    ky1 = *ky + 1;
    kmax = max(*kx,*ky);
    km1 = kmax + 1;
    km2 = km1 + 1;
    if (*iopt < -1 || *iopt > 1) {
	goto L70;
    }
    if (*m < kx1 * ky1) {
	goto L70;
    }
    nminx = kx1 << 1;
    if (*nxest < nminx || *nxest > *nmax) {
	goto L70;
    }
    nminy = ky1 << 1;
    if (*nyest < nminy || *nyest > *nmax) {
	goto L70;
    }
    nest = max(*nxest,*nyest);
    nxk = *nxest - kx1;
    nyk = *nyest - ky1;
    ncest = nxk * nyk;
    nmx = *nxest - nminx + 1;
    nmy = *nyest - nminy + 1;
    nrint = nmx + nmy;
    nreg = nmx * nmy;
    ib1 = *kx * nyk + ky1;
    jb1 = *ky * nxk + kx1;
    ib3 = kx1 * nyk + 1;
    if (ib1 <= jb1) {
	goto L10;
    }
    ib1 = jb1;
    ib3 = ky1 * nxk + 1;
L10:
    lwest = ncest * (ib1 + 2 + ib3) + (nrint + nest * km2 + *m * km1 << 1) + 
	    ib3;
    kwest = *m + nreg;
    if (*lwrk1 < lwest || *kwrk < kwest) {
	goto L70;
    }
    if (*xb >= *xe || *yb >= *ye) {
	goto L70;
    }
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (w[i__] <= (float)0.) {
	    goto L70;
	}
	if (x[i__] < *xb || x[i__] > *xe) {
	    goto L70;
	}
	if (y[i__] < *yb || y[i__] > *ye) {
	    goto L70;
	}
/* L20: */
    }
    if (*iopt >= 0) {
	goto L50;
    }
    if (*nx < nminx || *nx > *nxest) {
	goto L70;
    }
    nxk = *nx - kx1;
    tx[kx1] = *xb;
    tx[nxk + 1] = *xe;
    i__1 = nxk;
    for (i__ = kx1; i__ <= i__1; ++i__) {
	if (tx[i__ + 1] <= tx[i__]) {
	    goto L70;
	}
/* L30: */
    }
    if (*ny < nminy || *ny > *nyest) {
	goto L70;
    }
    nyk = *ny - ky1;
    ty[ky1] = *yb;
    ty[nyk + 1] = *ye;
    i__1 = nyk;
    for (i__ = ky1; i__ <= i__1; ++i__) {
	if (ty[i__ + 1] <= ty[i__]) {
	    goto L70;
	}
/* L40: */
    }
    goto L60;
L50:
    if (*s < (float)0.) {
	goto L70;
    }
L60:
    *ier = 0;
/*  we partition the working space and determine the spline approximation */
    kn = 1;
    ki = kn + *m;
    lq = 2;
    la = lq + ncest * ib3;
    lf = la + ncest * ib1;
    lff = lf + ncest;
    lfp = lff + ncest;
    lco = lfp + nrint;
    lh = lco + nrint;
    lbx = lh + ib3;
    nek = nest * km2;
    lby = lbx + nek;
    lsx = lby + nek;
    lsy = lsx + *m * km1;
    fpsurf_(iopt, m, &x[1], &y[1], &z__[1], &w[1], xb, xe, yb, ye, kx, ky, s, 
	    nxest, nyest, eps, &tol, &maxit, &nest, &km1, &km2, &ib1, &ib3, &
	    ncest, &nrint, &nreg, nx, &tx[1], ny, &ty[1], &c__[1], fp, &wrk1[
	    1], &wrk1[lfp], &wrk1[lco], &wrk1[lf], &wrk1[lff], &wrk1[la], &
	    wrk1[lq], &wrk1[lbx], &wrk1[lby], &wrk1[lsx], &wrk1[lsy], &wrk1[
	    lh], &iwrk[ki], &iwrk[kn], &wrk2[1], lwrk2, ier);
L70:
    return 0;
} /* surfit_ */


