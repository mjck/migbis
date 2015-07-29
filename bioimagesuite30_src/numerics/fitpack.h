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

//---------------------------------------------------------------------------------------------------------
//
//  This is the .h file for fitpack as produced by f2c
//
//---------------------------------------------------------------------------------------------------------

#ifndef FITPACK_INCLUDE
#define FITPACK_INCLUDE
#include "bio_f2c.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int bispev_(real *tx, integer *nx, real *ty, integer *ny, real *c__, integer *kx, integer *ky, real *x, integer *mx, real *y, integer *my, real *z__, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int clocur_(integer *iopt, integer *ipar, integer *idim, integer *m, real *u, integer *mx, real *x, real *w, integer *k, real *s, integer *nest, integer *n, real *t, integer *nc, real *c__, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *ier);

extern int cocosp_(integer *m, real *x, real *y, real *w, integer *n, real *t, real *e, integer *maxtr, integer *maxbin, real *c__, real *sq, real *sx, logical *bind, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int concon_(integer *iopt, integer *m, real *x, real *y, real *w, real *v, real *s, integer *nest, integer *maxtr, integer *maxbin, integer *n, real *t, real *c__, real *sq, real *sx, logical *bind, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int concur_(integer *iopt, integer *idim, integer *m, real *u, integer *mx, real *x, real *xx, real *w, integer *ib, real *db, integer *nb, integer *ie, real *de, integer *ne, integer *k, real *s, integer *nest, integer *n, real *t, integer *nc, real *c__, integer *np, real *cp, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *ier);

extern int cualde_(integer *idim, real *t, integer *n, real *c__, integer *nc, integer *k1, real *u, real *d__, integer *nd, integer *ier);

extern int curev_(integer *idim, real *t, integer *n, real *c__, integer *nc, integer *k, real *u, integer *m, real *x, integer *mx, integer *ier);

extern int curfit_(integer *iopt, integer *m, real *x, real *y, real *w, real *xb, real *xe, integer *k, real *s, integer *nest, integer *n, real *t, real *c__, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *ier);

extern E_f dblint_(real *tx, integer *nx, real *ty, integer *ny, real *c__, integer *kx, integer *ky, real *xb, real *xe, real *yb, real *ye, real *wrk);

extern E_f evapol_(real *tu, integer *nu, real *tv, integer *nv, real *c__, E_fp rad, real *x, real *y);

extern int fourco_(real *t, integer *n, real *c__, real *alfa, integer *m, real *ress, real *resc, real *wrk1, real *wrk2, integer *ier);

extern int fpader_(real *t, integer *n, real *c__, integer *k1, real *x, integer *l, real *d__);

extern int fpadno_(integer *maxtr, integer *up, integer *left, integer *right, integer *info, integer *count, integer *merk, integer *jbind, integer *n1, integer *ier);

extern int fpadpo_(integer *idim, real *t, integer *n, real *c__, integer *nc, integer *k, real *cp, integer *np, real *cc, real *t1, real *t2);

extern int fpback_(real *a, real *z__, integer *n, integer *k, real *c__, integer *nest);

extern int fpbacp_(real *a, real *b, real *z__, integer *n, integer *k, real *c__, integer *k1, integer *nest);

extern int fpbfou_(real *t, integer *n, real *par, real *ress, real *resc);

extern int fpbisp_(real *tx, integer *nx, real *ty, integer *ny, real *c__, integer *kx, integer *ky, real *x, integer *mx, real *y, integer *my, real *z__, real *wx, real *wy, integer *lx, integer *ly);

extern int fpbspl_(real *t, integer *n, integer *k, real *x, integer *l, real *h__);

extern int fpchec_(real *x, integer *m, real *t, integer *n, integer *k, integer *ier);

extern int fpched_(real *x, integer *m, real *t, integer *n, integer *k, integer *ib, integer *ie, integer *ier);

extern int fpchep_(real *x, integer *m, real *t, integer *n, integer *k, integer *ier);

extern int fpclos_(integer *iopt, integer *idim, integer *m, real *u, integer *mx, real *x, real *w, integer *k, real *s, integer *nest, real *tol, integer *maxit, integer *k1, integer *k2, integer *n, real *t, integer *nc, real *c__, real *fp, real *fpint, real *z__, real *a1, real *a2, real *b, real *g1, real *g2, real *q, integer *nrdata, integer *ier);

extern int fpcoco_(integer *iopt, integer *m, real *x, real *y, real *w, real *v, real *s, integer *nest, integer *maxtr, integer *maxbin, integer *n, real *t, real *c__, real *sq, real *sx, logical *bind, real *e, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int fpcons_(integer *iopt, integer *idim, integer *m, real *u, integer *mx, real *x, real *w, integer *ib, integer *ie, integer *k, real *s, integer *nest, real *tol, integer *maxit, integer *k1, integer *k2, integer *n, real *t, integer *nc, real *c__, real *fp, real *fpint, real *z__, real *a, real *b, real *g, real *q, integer *nrdata, integer *ier);

extern int fpcosp_(integer *m, real *x, real *y, real *w, integer *n, real *t, real *e, integer *maxtr, integer *maxbin, real *c__, real *sq, real *sx, logical *bind, integer *nm, integer *mb, real *a, real *b, real *const__, real *z__, real *zz, real *u, real *q, integer *info, integer *up, integer *left, integer *right, integer *jbind, integer *ibind, integer *ier);

extern int fpcsin_(real *a, real *b, real *par, real *sia, real *coa, real *sib, real *cob, real *ress, real *resc);

extern int fpcurf_(integer *iopt, real *x, real *y, real *w, integer *m, real *xb, real *xe, integer *k, real *s, integer *nest, real *tol, integer *maxit, integer *k1, integer *k2, integer *n, real *t, real *c__, real *fp, real *fpint, real *z__, real *a, real *b, real *g, real *q, integer *nrdata, integer *ier);

extern int fpcuro_(real *a, real *b, real *c__, real *d__, real *x, integer *n);

extern int fpcyt1_(real *a, integer *n, integer *nn);

extern int fpcyt2_(real *a, integer *n, real *b, real *c__, integer *nn);

extern int fpdeno_(integer *maxtr, integer *up, integer *left, integer *right, integer *nbind, integer *merk);

extern int fpdisc_(real *t, integer *n, integer *k2, real *b, integer *nest);

extern int fpfrno_(integer *maxtr, integer *up, integer *left, integer *right, integer *info, integer *point, integer *merk, integer *n1, integer *count, integer *ier);

extern int fpgivs_(real *piv, real *ww, real *cos__, real *sin__);

extern int fpgrdi_(integer *ifsu, integer *ifsv, integer *ifbu, integer *ifbv, integer *iback, real *u, integer *mu, real *v, integer *mv, real *z__, integer *mz, real *dz, integer *iop0, integer *iop1, real *tu, integer *nu, real *tv, integer *nv, real *p, real *c__, integer *nc, real *sq, real *fp, real *fpu, real *fpv, integer *mm, integer *mvnu, real *spu, real *spv, real *right, real *q, real *au, real *av1, real *av2, real *bu, real *bv, real *aa, real *bb, real *cc, real *cosi, integer *nru, integer *nrv);

extern int fpgrpa_(integer *ifsu, integer *ifsv, integer *ifbu, integer *ifbv, integer *idim, integer *ipar, real *u, integer *mu, real *v, integer *mv, real *z__, integer *mz, real *tu, integer *nu, real *tv, integer *nv, real *p, real *c__, integer *nc, real *fp, real *fpu, real *fpv, integer *mm, integer *mvnu, real *spu, real *spv, real *right, real *q, real *au, real *au1, real *av, real *av1, real *bu, real *bv, integer *nru, integer *nrv);

extern int fpgrre_(integer *ifsx, integer *ifsy, integer *ifbx, integer *ifby, real *x, integer *mx, real *y, integer *my, real *z__, integer *mz, integer *kx, integer *ky, real *tx, integer *nx, real *ty, integer *ny, real *p, real *c__, integer *nc, real *fp, real *fpx, real *fpy, integer *mm, integer *mynx, integer *kx1, integer *kx2, integer *ky1, integer *ky2, real *spx, real *spy, real *right, real *q, real *ax, real *ay, real *bx, real *by, integer *nrx, integer *nry);

extern int fpgrsp_(integer *ifsu, integer *ifsv, integer *ifbu, integer *ifbv, integer *iback, real *u, integer *mu, real *v, integer *mv, real *r__, integer *mr, real *dr, integer *iop0, integer *iop1, real *tu, integer *nu, real *tv, integer *nv, real *p, real *c__, integer *nc, real *sq, real *fp, real *fpu, real *fpv, integer *mm, integer *mvnu, real *spu, real *spv, real *right, real *q, real *au, real *av1, real *av2, real *bu, real *bv, real *a0, real *a1, real *b0, real *b1, real *c0, real *c1, real *cosi, integer *nru, integer *nrv);

extern int fpinst_(integer *iopt, real *t, integer *n, real *c__, integer *k, real *x, integer *l, real *tt, integer *nn, real *cc, integer *nest);

extern int fpintb_(real *t, integer *n, real *bint, integer *nk1, real *x, real *y);

extern int fpknot_(real *x, integer *m, real *t, integer *n, real *fpint, integer *nrdata, integer *nrint, integer *nest, integer *istart);

extern int fpopdi_(integer *ifsu, integer *ifsv, integer *ifbu, integer *ifbv, real *u, integer *mu, real *v, integer *mv, real *z__, integer *mz, real *z0, real *dz, integer *iopt, integer *ider, real *tu, integer *nu, real *tv, integer *nv, integer *nuest, integer *nvest, real *p, real *step, real *c__, integer *nc, real *fp, real *fpu, real *fpv, integer *nru, integer *nrv, real *wrk, integer *lwrk);

extern int fpopsp_(integer *ifsu, integer *ifsv, integer *ifbu, integer *ifbv, real *u, integer *mu, real *v, integer *mv, real *r__, integer *mr, real *r0, real *r1, real *dr, integer *iopt, integer *ider, real *tu, integer *nu, real *tv, integer *nv, integer *nuest, integer *nvest, real *p, real *step, real *c__, integer *nc, real *fp, real *fpu, real *fpv, integer *nru, integer *nrv, real *wrk, integer *lwrk);

extern int fporde_(real *x, real *y, integer *m, integer *kx, integer *ky, real *tx, integer *nx, real *ty, integer *ny, integer *nummer, integer *index, integer *nreg);

extern int fppara_(integer *iopt, integer *idim, integer *m, real *u, integer *mx, real *x, real *w, real *ub, real *ue, integer *k, real *s, integer *nest, real *tol, integer *maxit, integer *k1, integer *k2, integer *n, real *t, integer *nc, real *c__, real *fp, real *fpint, real *z__, real *a, real *b, real *g, real *q, integer *nrdata, integer *ier);

extern int fppasu_(integer *iopt, integer *ipar, integer *idim, real *u, integer *mu, real *v, integer *mv, real *z__, integer *mz, real *s, integer *nuest, integer *nvest, real *tol, integer *maxit, integer *nc, integer *nu, real *tu, integer *nv, real *tv, real *c__, real *fp, real *fp0, real *fpold, real *reducu, real *reducv, real *fpintu, real *fpintv, integer *lastdi, integer *nplusu, integer *nplusv, integer *nru, integer *nrv, integer *nrdatu, integer *nrdatv, real *wrk, integer *lwrk, integer *ier);

extern int fpperi_(integer *iopt, real *x, real *y, real *w, integer *m, integer *k, real *s, integer *nest, real *tol, integer *maxit, integer *k1, integer *k2, integer *n, real *t, real *c__, real *fp, real *fpint, real *z__, real *a1, real *a2, real *b, real *g1, real *g2, real *q, integer *nrdata, integer *ier);

extern int fppocu_(integer *idim, integer *k, real *a, real *b, integer *ib, real *db, integer *nb, integer *ie, real *de, integer *ne, real *cp, integer *np);

extern int fppogr_(integer *iopt, integer *ider, real *u, integer *mu, real *v, integer *mv, real *z__, integer *mz, real *z0, real *r__, real *s, integer *nuest, integer *nvest, real *tol, integer *maxit, integer *nc, integer *nu, real *tu, integer *nv, real *tv, real *c__, real *fp, real *fp0, real *fpold, real *reducu, real *reducv, real *fpintu, real *fpintv, real *dz, real *step, integer *lastdi, integer *nplusu, integer *nplusv, integer *lasttu, integer *nru, integer *nrv, integer *nrdatu, integer *nrdatv, real *wrk, integer *lwrk, integer *ier);

extern int fppola_(integer *iopt1, integer *iopt2, integer *iopt3, integer *m, real *u, real *v, real *z__, real *w, E_fp rad, real *s, integer *nuest, integer *nvest, real *eta, real *tol, integer *maxit, integer *ib1, integer *ib3, integer *nc, integer *ncc, integer *intest, integer *nrest, integer *nu, real *tu, integer *nv, real *tv, real *c__, real *fp, real *sup, real *fpint, real *coord, real *f, real *ff, real *row, real *cs, real *cosi, real *a, real *q, real *bu, real *bv, real *spu, real *spv, real *h__, integer *index, integer *nummer, real *wrk, integer *lwrk, integer *ier);

extern int fprank_(real *a, real *f, integer *n, integer *m, integer *na, real *tol, real *c__, real *sq, integer *rank, real *aa, real *ff, real *h__);

extern E_f fprati_(real *p1, real *f1, real *p2, real *f2, real *p3, real *f3);

extern int fpregr_(integer *iopt, real *x, integer *mx, real *y, integer *my, real *z__, integer *mz, real *xb, real *xe, real *yb, real *ye, integer *kx, integer *ky, real *s, integer *nxest, integer *nyest, real *tol, integer *maxit, integer *nc, integer *nx, real *tx, integer *ny, real *ty, real *c__, real *fp, real *fp0, real *fpold, real *reducx, real *reducy, real *fpintx, real *fpinty, integer *lastdi, integer *nplusx, integer *nplusy, integer *nrx, integer *nry, integer *nrdatx, integer *nrdaty, real *wrk, integer *lwrk, integer *ier);

extern int fprota_(real *cos__, real *sin__, real *a, real *b);

extern int fprppo_(integer *nu, integer *nv, integer *if1, integer *if2, real *cosi, real *ratio, real *c__, real *f, integer *ncoff);

extern int fprpsp_(integer *nt, integer *np, real *co, real *si, real *c__, real *f, integer *ncoff);

extern int fpseno_(integer *maxtr, integer *up, integer *left, integer *right, integer *info, integer *merk, integer *ibind, integer *nbind);

extern int fpspgr_(integer *iopt, integer *ider, real *u, integer *mu, real *v, integer *mv, real *r__, integer *mr, real *r0, real *r1, real *s, integer *nuest, integer *nvest, real *tol, integer *maxit, integer *nc, integer *nu, real *tu, integer *nv, real *tv, real *c__, real *fp, real *fp0, real *fpold, real *reducu, real *reducv, real *fpintu, real *fpintv, real *dr, real *step, integer *lastdi, integer *nplusu, integer *nplusv, integer *lastu0, integer *lastu1, integer *nru, integer *nrv, integer *nrdatu, integer *nrdatv, real *wrk, integer *lwrk, integer *ier);

extern int fpsphe_(integer *iopt, integer *m, real *teta, real *phi, real *r__, real *w, real *s, integer *ntest, integer *npest, real *eta, real *tol, integer *maxit, integer *ib1, integer *ib3, integer *nc, integer *ncc, integer *intest, integer *nrest, integer *nt, real *tt, integer *np, real *tp, real *c__, real *fp, real *sup, real *fpint, real *coord, real *f, real *ff, real *row, real *coco, real *cosi, real *a, real *q, real *bt, real *bp, real *spt, real *spp, real *h__, integer *index, integer *nummer, real *wrk, integer *lwrk, integer *ier);

extern int fpsuev_(integer *idim, real *tu, integer *nu, real *tv, integer *nv, real *c__, real *u, integer *mu, real *v, integer *mv, real *f, real *wu, real *wv, integer *lu, integer *lv);

extern int fpsurf_(integer *iopt, integer *m, real *x, real *y, real *z__, real *w, real *xb, real *xe, real *yb, real *ye, integer *kxx, integer *kyy, real *s, integer *nxest, integer *nyest, real *eta, real *tol, integer *maxit, integer *nmax, integer *km1, integer *km2, integer *ib1, integer *ib3, integer *nc, integer *intest, integer *nrest, integer *nx0, real *tx, integer *ny0, real *ty, real *c__, real *fp, real *fp0, real *fpint, real *coord, real *f, real *ff, real *a, real *q, real *bx, real *by, real *spx, real *spy, real *h__, integer *index, integer *nummer, real *wrk, integer *lwrk, integer *ier);

extern int fpsysy_(real *a, integer *n, real *g);

extern int fptrnp_(integer *m, integer *mm, integer *idim, integer *n, integer *nr, real *sp, real *p, real *b, real *z__, real *a, real *q, real *right);

extern int fptrpe_(integer *m, integer *mm, integer *idim, integer *n, integer *nr, real *sp, real *p, real *b, real *z__, real *a, real *aa, real *q, real *right);

extern int insert_(integer *iopt, real *t, integer *n, real *c__, integer *k, real *x, real *tt, integer *nn, real *cc, integer *nest, integer *ier);

extern int parcur_(integer *iopt, integer *ipar, integer *idim, integer *m, real *u, integer *mx, real *x, real *w, real *ub, real *ue, integer *k, real *s, integer *nest, integer *n, real *t, integer *nc, real *c__, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *ier);

extern int parder_(real *tx, integer *nx, real *ty, integer *ny, real *c__, integer *kx, integer *ky, integer *nux, integer *nuy, real *x, integer *mx, real *y, integer *my, real *z__, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int parsur_(integer *iopt, integer *ipar, integer *idim, integer *mu, real *u, integer *mv, real *v, real *f, real *s, integer *nuest, integer *nvest, integer *nu, real *tu, integer *nv, real *tv, real *c__, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int percur_(integer *iopt, integer *m, real *x, real *y, real *w, integer *k, real *s, integer *nest, integer *n, real *t, real *c__, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *ier);

extern int pogrid_(integer *iopt, integer *ider, integer *mu, real *u, integer *mv, real *v, real *z__, real *z0, real *r__, real *s, integer *nuest, integer *nvest, integer *nu, real *tu, integer *nv, real *tv, real *c__, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int polar_(integer *iopt, integer *m, real *x, real *y, real *z__, real *w, E_fp rad, real *s, integer *nuest, integer *nvest, real *eps, integer *nu, real *tu, integer *nv, real *tv, real *u, real *v, real *c__, real *fp, real *wrk1, integer *lwrk1, real *wrk2, integer *lwrk2, integer *iwrk, integer *kwrk, integer *ier);

extern int profil_(integer *iopt, real *tx, integer *nx, real *ty, integer *ny, real *c__, integer *kx, integer *ky, real *u, integer *nu, real *cu, integer *ier);

extern int regrid_(integer *iopt, integer *mx, real *x, integer *my, real *y, real *z__, real *xb, real *xe, real *yb, real *ye, integer *kx, integer *ky, real *s, integer *nxest, integer *nyest, integer *nx, real *tx, integer *ny, real *ty, real *c__, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int spalde_(real *t, integer *n, real *c__, integer *k1, real *x, real *d__, integer *ier);

extern int spgrid_(integer *iopt, integer *ider, integer *mu, real *u, integer *mv, real *v, real *r__, real *r0, real *r1, real *s, integer *nuest, integer *nvest, integer *nu, real *tu, integer *nv, real *tv, real *c__, real *fp, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int sphere_(integer *iopt, integer *m, real *teta, real *phi, real *r__, real *w, real *s, integer *ntest, integer *npest, real *eps, integer *nt, real *tt, integer *np, real *tp, real *c__, real *fp, real *wrk1, integer *lwrk1, real *wrk2, integer *lwrk2, integer *iwrk, integer *kwrk, integer *ier);

extern int splder_(real *t, integer *n, real *c__, integer *k, integer *nu, real *x, real *y, integer *m, real *wrk, integer *ier);

extern int splev_(real *t, integer *n, real *c__, integer *k, real *x, real *y, integer *m, integer *ier);

extern E_f splint_(real *t, integer *n, real *c__, integer *k, real *a, real *b, real *wrk);

extern int sproot_(real *t, integer *n, real *c__, real *zero, integer *mest, integer *m, integer *ier);

extern int surev_(integer *idim, real *tu, integer *nu, real *tv, integer *nv, real *c__, real *u, integer *mu, real *v, integer *mv, real *f, integer *mf, real *wrk, integer *lwrk, integer *iwrk, integer *kwrk, integer *ier);

extern int surfit_(integer *iopt, integer *m, real *x, real *y, real *z__, real *w, real *xb, real *xe, real *yb, real *ye, integer *kx, integer *ky, real *s, integer *nxest, integer *nyest, integer *nmax, real *eps, integer *nx, real *tx, integer *ny, real *ty, real *c__, real *fp, real *wrk1, integer *lwrk1, real *wrk2, integer *lwrk2, integer *iwrk, integer *kwrk, integer *ier);

#ifdef __cplusplus
}
#endif

#endif

