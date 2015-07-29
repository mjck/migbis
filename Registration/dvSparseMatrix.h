
//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


#ifndef SPARSE_H
#define SPARSE_H
// $Id: dvSparseMatrix.h,v 1.1 2002/11/06 16:24:56 papad Exp papad $

#include <utility>
#include <map>
#include "vtkpxMatrix.h"


class vtkpxSparseMatrix; // forward declaration


/*
  See also examples in  Dirk Vermeier
  "Multi-Paradigm Programming Using C+"

  adapted and modified by Xenios Papademetris
  papad@noodle.med.yale.edu

  October 29th 2002
  Used by permission of the original author, Dirk Vermeier, for release in GPL software.


  e.g.

  dvSparseMatrix<double> m(0.0); 
  m[0][2]=1.0;
  m[1][3]=4.0;
  m.listall();
  m.cleanup();


*/

class dvSparseRow;
class dvSparseMatrix;

class dvSparseElement {

  public:
  
  dvSparseElement(dvSparseRow& r,int k2): row_(r), key2_(k2) {}
  void operator=(const float& val);
  operator float() { return Value();}
  

private:

  dvSparseRow&	   row_; // E.g. a[k1][k2] returns element refering to a[k1] row, key2=k2.
  unsigned long	  key2_;
  float Value();
};

// ------------------------------------------------------------------------------------


class dvSparseRow 
{ 
  // a row is a [Key2->float] map
  friend class dvSparseElement;
  friend class dvSparseMatrix;
  
private:
  dvSparseRow& operator=(const dvSparseRow&);   // Forbid assignment
  dvSparseMatrix&      matrix_;
  
  
public:
  dvSparseRow(dvSparseMatrix& matrix): matrix_(matrix) { columns_[0]=0.0;} // Remember matrix for getting default value.
  dvSparseElement operator[](const unsigned long& k2) { return dvSparseElement(*this,k2); }

  std::map<unsigned long,float>    columns_;  // one element for every non-default element in row
  
  // ------------------------------------------------------------------------------------
  //
  //
  //            Additional Functions  Xenios Papademetris Oct 2002
  //
  //
  // ------------------------------------------------------------------------------------
  
  unsigned long getsize()  { return columns_.size(); }
  
  void cleanup() { columns_.clear();}
  
  void getrange(unsigned long r[2]) {
    
    std::map<unsigned long,float>::iterator i = columns_.begin();    
    r[0]=(*i).first;
    r[1]=(*i).first;
    ++i;
    while (i != columns_.end())                    
      {
	if (r[0] >  (*i).first)
	  r[0]=(*i).first;
	if (r[1] <  (*i).first)
	  r[1]=(*i).first;	
	++i;
      }
  }
  
  
  
  float getsum()  { 
    float sm=0.0;
    std::map<unsigned long,float>::iterator i = columns_.begin();    
    while (i != columns_.end())                    
      {
	sm+=(*i).second;
	++i;
      }
    return sm;
  }
  

  // ------------------------------------------------------
  // RPM Stuff
  // ------------------------------------------------------
  void addtosum(float* sum) {
    std::map<unsigned long,float>::iterator i = columns_.begin();    
    while (i != columns_.end())                    
      {
	sum[(*i).first]+=(*i).second;
	++i;
      }
  }

  void scalebyarray(float* sum) {
    std::map<unsigned long,float>::iterator i = columns_.begin();    
    while (i != columns_.end())                    
      {
	(*i).second=(*i).second*sum[(*i).first];
	++i;
      }
  }
  

  void scale(float s) {
    std::map<unsigned long,float>::iterator i = columns_.begin();    
    while (i != columns_.end())                    
      {
	(*i).second=(*i).second*s;
	++i;
      }
  }
  
  
  void multiplywith(int colindex,int rowindex,vtkpxMatrix*b,vtkpxMatrix*c) {
    std::map<unsigned long,float>::iterator i = columns_.begin();    
    float sum=0.0;
    while (i != columns_.end())                    
      {
	sum+=(*i).second*b->GetDirectElement((*i).first,colindex);
	++i;
      }
    c->SetDirectElement(rowindex,colindex,sum);
  }
  
  
  void multiplywith3vector(int rowindex,vtkpxMatrix*b,vtkpxMatrix*c,float newscale=1.0) {
    
    std::map<unsigned long,float>::iterator i = columns_.begin();    
    float sum[3]={ 0.0,0.0,0.0 };
    while (i != columns_.end())                    
      {
	for (int j=0;j<=2;j++)
	  sum[j]+=(*i).second*b->GetDirectElement((*i).first,j);
	++i;
      }
    for (int j=0;j<=2;j++)
      c->SetDirectElement(rowindex,j,sum[j]*newscale);
  }
  
  
  // ------------------------------------------------------------------------------------
  //
  //   E n d   o f   A d d i t i o n s
  //
  //
  // ------------------------------------------------------------------------------------
  
};

// ------------------------------------------------------------------------------------
// This class uses maps to implement 2-dimensional sparse matrices.
// The default value is an argument of the constructor.
// ------------------------------------------------------------------------------------

class dvSparseMatrix 
{
public:
  friend class vtkpxSparseMatrix;
  std::map<unsigned long,dvSparseRow> rows_;


  dvSparseMatrix(const float& default_vvalue=0): default_(default_vvalue) { index1_min=0; index1_max=0;}
  const float&	default_value() const { return default_; }
  dvSparseRow&	operator[](const unsigned long& k1) 
  { 
    // Return reference to row from rows_ (possibly after inserting it).

    /* pair<iterator, bool>
       insert(const value_type& x)
       Unique Associative Container Inserts */

    return (*((rows_.insert(std::make_pair(k1,dvSparseRow(*this)))).first)).second;
  }


protected:
  float		default_;

  unsigned long          index1_min,index1_max;
  unsigned long          index2_min,index2_max;


public:

  // ------------------------------------------------------------------------------------
  //
  //
  //            Additional Functions  Xenios Papademetris Oct 2002
  //
  //
  // ------------------------------------------------------------------------------------



  unsigned long getminindex1() { return index1_min; }
  unsigned long getmaxindex1() { return index1_max; }
  unsigned long getminindex2() { return index2_min; }
  unsigned long getmaxindex2() { return index2_max; }

  void setindices(int sz[2]) { index1_min=0;index2_min=0; index1_max=sz[0]-1; index2_max=sz[1]-1;}
  

  unsigned long totalsize() {
    std::map<unsigned long,dvSparseRow>::iterator i = rows_.begin();    
    unsigned long sz=0;
    while (i != rows_.end())                    
      {
	dvSparseRow  b=(*i).second;
	sz+=b.getsize();
	++i;
      }
    return sz;
  }


  int getnumrowelements(int row) {
    std::map<unsigned long,dvSparseRow>::iterator i = rows_.find(row);    
    if (i!=rows_.end())
      return (*i).second.getsize();
    else
      fprintf(stderr,"* %d * ",row);
    
    return 0;
  }
    
  // ------------------------------------------------------------------------------------

  void cleanup () {

    std::map<unsigned long,dvSparseRow>::iterator i = rows_.begin();    
    while (i != rows_.end())                    
      {
	dvSparseRow  b=(*i).second;
        b.cleanup();
	i++;
      }
    rows_.clear();

  }

  // ------------------------------------------------------------------------------------

  void updaterange1() {
    std::map<unsigned long,dvSparseRow>::iterator i = rows_.begin();    
    index1_min=(*i).first;
    index1_max=(*i).first;
    ++i;
    while (i != rows_.end())                    
      {
	
	if (index1_min >  (*i).first)
	  index1_min=(*i).first;
	if (index1_max <  (*i).first)
	  index1_max=(*i).first;	
	++i;
      }
  }


  void updaterange2() {

    std::map<unsigned long,dvSparseRow>::iterator i = rows_.begin();    
    unsigned long r[2];
    (*i).second.getrange(r);
    index2_min=r[0];
    index2_max=r[1];
    
    while (i != rows_.end())                    
      {
	(*i).second.getrange(r);
	if (index2_min>r[0]) index2_min=r[0];
	if (index2_max<r[1]) index2_max=r[1];
	i++;
      }
  }



  // ------------------------------------------------------------------------------------

  void print(unsigned long upd=0) {
    if (upd)
      {
	updaterange1();
	updaterange2();
      }

    for (unsigned long i=index1_min;i<=index1_max;i++)
      {
	fprintf(stderr,"%5d [ ",getnumrowelements(i));
	for (unsigned long j=index2_min;j<=index2_max;j++)
	  { 
	    float a=(*this)[i][j];
	    if (a!=default_ || j==0)
	      fprintf(stderr,"%6.3f ",float(a));
	    else
	      fprintf(stderr,"...... ");
	  }
	fprintf(stderr,"]\n");
      }
  }

  void printbw(unsigned long upd=0) {
    if (upd)
      {
	updaterange1();
	updaterange2();
      }

    for (unsigned long i=index1_min;i<=index1_max;i++)
      {
	fprintf(stderr,"%5d [ ",getnumrowelements(i));
	for (unsigned long j=index2_min;j<=index2_max;j++)
	  { 
	    float a=(*this)[i][j];
	    if (a!=default_ || j==0)
	      fprintf(stderr,"*");
	    else
	      fprintf(stderr,".");
	  }
	fprintf(stderr,"]\n");
      }
  }


  // ------------------------------------------------------------------------------------
  void scale(float s) {
    std::map<unsigned long,dvSparseRow>::iterator i = rows_.begin();    
    while (i != rows_.end())                    
      {
	(*i).second.scale(s);
	i++;
      }
  }

  void multiplywith(int s1[2],vtkpxMatrix* b,vtkpxMatrix* c)
  {
    int s2[2],s3[2];
    b->GetSize(s2);
    
    if (s1[1]!=s2[0])
      {
	fprintf(stderr,"Cannot multiply matrices bad size! a=%dx%d, b=%dx%d\n",s1[0],s1[1],s2[0],s2[1]);
	return;
      }
    
    c->Allocate(s1[0],s2[1]);
    c->GetSize(s3);


    std::map<unsigned long,dvSparseRow>::iterator iter = rows_.begin();    

    while (iter!=rows_.end())
      {
	for(int col = 0; col < s3[1]; col++) 
	  (*iter).second.multiplywith(col,(*iter).first,b,c);
	++iter;
      }
  }


  void multiplywith3vector(int s1[2],vtkpxMatrix* b,vtkpxMatrix* c)
  {
    int s2[2],s3[2];
    b->GetSize(s2);
    
    if (s1[1]!=s2[0] || s2[1]!=3)
      {
	fprintf(stderr,"Cannot multiply matrice x col=3 bad size! a=%dx%d, b=%dx%d\n",s1[0],s1[1],s2[0],s2[1]);
	return;
      }
    
    c->Allocate(s1[0],s2[1]);
    c->GetSize(s3);

    std::map<unsigned long,dvSparseRow>::iterator iter = rows_.begin();    

    while (iter!=rows_.end())
      {
	(*iter).second.multiplywith3vector((*iter).first,b,c);
	++iter;
      }
  }


  // ------------------------------------------------------------------------------------
  //
  //   E n d   o f   A d d i t i o n s
  //
  //
  // ------------------------------------------------------------------------------------
  
};


inline void dvSparseElement::operator=(const float& val) {  // E.g. a[k1][k2] = val.

  if (val==row_.matrix_.default_value() &&    // Val is default, erase possible k2 entry from row
      key2_ !=0  )  // Not Diagonal Element
    row_.columns_.erase(key2_); 
  else
    row_.columns_[key2_] = val;  // Non-default val: store in row[k2].
}

inline float dvSparseElement::Value() { // Return value of a[k1][k2].
  if (row_.columns_.count(key2_)>0) // A[k1][k2] explicitely stored: retrieve it.
    return row_.columns_[key2_]; 
  else // A[k1][k2] not stored: return default value.
    return row_.matrix_.default_value();
}




#endif

