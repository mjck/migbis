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

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkCropVolumeRayCastCompositeShadeHelper.cxx,v $
  Language:  C++
  Date:      $Date: 2007-11-08 16:51:06 $
  Version:   $Revision: 1.9 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkCropVolumeRayCastCompositeShadeHelper.h"

#include "vtkImageData.h"
#include "vtkCommand.h"
#include "vtkCropVolumeRayCastMapper.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkFixedPointRayCastImage.h"
#include "vtkDataArray.h"

#include <math.h>


void ShadeIrregularCropper(unsigned short *val, unsigned short tmp[4], unsigned int pos[3], vtkCropVolumeRayCastMapper *mapper);
void ShadeIrregularCropperArray(unsigned short val[4], unsigned short tmp[4], unsigned int pos[3], vtkCropVolumeRayCastMapper *mapper);

vtkCxxRevisionMacro(vtkCropVolumeRayCastCompositeShadeHelper, "$Revision: 1.9 $");
vtkStandardNewMacro(vtkCropVolumeRayCastCompositeShadeHelper);


// Construct a new vtkCropVolumeRayCastCompositeShadeHelper with default values
vtkCropVolumeRayCastCompositeShadeHelper::vtkCropVolumeRayCastCompositeShadeHelper()
{
}

// Destruct a vtkCropVolumeRayCastCompositeShadeHelper - clean up any memory used
vtkCropVolumeRayCastCompositeShadeHelper::~vtkCropVolumeRayCastCompositeShadeHelper()
{
}


// This method is used when the interpolation type is nearest neighbor and
// the data has one component and scale == 1.0 and shift == 0.0. In the inner
// loop we get the data value as an unsigned short, and use this index to
// lookup a color and opacity for this sample. We then composite this into
// the color computed so far along the ray, and check if we can terminate at
// this point (if the accumulated opacity is higher than some threshold).
// Finally we move on to the next sample along the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageOneSimpleNN( T *data,
                                                   int threadID,
                                                   int threadCount,
                                                   vtkCropVolumeRayCastMapper *mapper,
                                                   vtkVolume *vol)
{
  VTKKWRCHelper_InitializationAndLoopStartShadeNN();
  VTKKWRCHelper_InitializeCompositeOneNN();
  VTKKWRCHelper_InitializeCompositeShadeNN();
  VTKKWRCHelper_SpaceLeapSetup();

  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      VTKKWRCHelper_MoveToNextSampleShadeNN();
      }

    VTKKWRCHelper_SpaceLeapCheck();
    VTKKWRCHelper_CroppingCheckNN( pos );

    unsigned short val    = static_cast<unsigned short>(((*dptr)));
    VTKKWRCHelper_LookupColorUS( colorTable[0], scalarOpacityTable[0], val, tmp );
    if (tmp[3])
      {
      unsigned short normal = *dirPtr;
      VTKKWRCHelper_LookupShading( diffuseShadingTable[0], specularShadingTable[0], normal, tmp );
      VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
      }
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}

// This method is used when the interpolation type is nearest neighbor and
// the data has one component. In the inner loop we get the data value as
// an unsigned short using the scale/shift, and use this index to lookup
// a color and opacity for this sample. We then composite this into the
// color computed so far along the ray, and check if we can terminate at
// this point (if the accumulated opacity is higher than some threshold).
// Finally we move on to the next sample along the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageOneNN( T *data,
                                             int threadID,
                                             int threadCount,
                                             vtkCropVolumeRayCastMapper *mapper,
                                             vtkVolume *vol)
{
  VTKKWRCHelper_InitializationAndLoopStartShadeNN();
  VTKKWRCHelper_InitializeCompositeOneNN();
  VTKKWRCHelper_InitializeCompositeShadeNN();
  VTKKWRCHelper_SpaceLeapSetup();

  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      VTKKWRCHelper_MoveToNextSampleShadeNN();
      }

    VTKKWRCHelper_SpaceLeapCheck();
    VTKKWRCHelper_CroppingCheckNN( pos );

    unsigned short val    = static_cast<unsigned short>(((*dptr) + shift[0])*scale[0]);
    VTKKWRCHelper_LookupColorUS( colorTable[0], scalarOpacityTable[0], val, tmp );
    if (tmp[3])
      {
      unsigned short normal = *dirPtr;
      VTKKWRCHelper_LookupShading( diffuseShadingTable[0], specularShadingTable[0], normal, tmp );
      VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
      }
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}


// This method is used when the interpolation type is nearest neighbor and
// the data has two components which are not considered independent. In the
// inner loop we compute the two unsigned short index values from the data
// values (using the scale/shift). We use the first index to lookup a color,
// and we use the second index to look up the opacity. We then composite
// the color into the color computed so far along this ray, and check to
// see if we can terminate here (if the opacity accumulated exceed some
// threshold). Finally we move to the next sample along the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageTwoDependentNN( T *data,
                                                      int threadID,
                                                      int threadCount,
                                                      vtkCropVolumeRayCastMapper *mapper,
                                                      vtkVolume *vol)
{
  VTKKWRCHelper_InitializationAndLoopStartShadeNN();
  VTKKWRCHelper_InitializeCompositeOneNN();
  VTKKWRCHelper_InitializeCompositeShadeNN();
  VTKKWRCHelper_SpaceLeapSetup();

  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      VTKKWRCHelper_MoveToNextSampleShadeNN();
      }

    VTKKWRCHelper_SpaceLeapCheck();
    VTKKWRCHelper_CroppingCheckNN( pos );

    unsigned short val[2];
    val[0] = static_cast<unsigned short>(((*(dptr  )) + shift[0])*scale[0]);
    val[1] = static_cast<unsigned short>(((*(dptr+1)) + shift[1])*scale[1]);

    tmp[3] = scalarOpacityTable[0][val[1]];
    if (tmp[3])
      {
      tmp[0] = static_cast<unsigned short>
        ((colorTable[0][3*val[0]  ]*tmp[3] + 0x7fff)>>(VTKKW_FP_SHIFT));
      tmp[1] = static_cast<unsigned short>
        ((colorTable[0][3*val[0]+1]*tmp[3] + 0x7fff)>>(VTKKW_FP_SHIFT));
      tmp[2] = static_cast<unsigned short>
        ((colorTable[0][3*val[0]+2]*tmp[3] + 0x7fff)>>(VTKKW_FP_SHIFT));

      unsigned short normal = *dirPtr;
      VTKKWRCHelper_LookupShading( diffuseShadingTable[0], specularShadingTable[0], normal, tmp );
      VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
      }
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}

// This method is used when the interpolation type is nearest neighbor and
// the data has four components which are not considered independent . This
// means that the first three components directly represent color, and this
// data must be of unsigned char type. In the inner loop we directly access
// the four data values (no scale/shift is needed). The first three are the
// color of this sample and the fourth is used to look up an opacity in the
// scalar opacity transfer function. We then composite this color into the
// color we have accumulated so far along the ray, and check if we can
// terminate here (if our accumulated opacity has exceed some threshold).
// Finally we move onto the next sample along the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageFourDependentNN( T *data,
                                                       int threadID,
                                                       int threadCount,
                                                       vtkCropVolumeRayCastMapper *mapper,
                                                       vtkVolume *vol)
{
  VTKKWRCHelper_InitializationAndLoopStartShadeNN();
  VTKKWRCHelper_InitializeCompositeOneNN();
  VTKKWRCHelper_InitializeCompositeShadeNN();
  VTKKWRCHelper_SpaceLeapSetup();

  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      VTKKWRCHelper_MoveToNextSampleShadeNN();
      }

    VTKKWRCHelper_SpaceLeapCheck();
    VTKKWRCHelper_CroppingCheckNN( pos );

    unsigned short val[4];
    val[0] = *(dptr  );
    val[1] = *(dptr+1);
    val[2] = *(dptr+2);
    val[3] = static_cast<unsigned short>(((*(dptr+3)) + shift[3])*scale[3]);

    tmp[3] = scalarOpacityTable[0][val[3]];
    if ( tmp[3] )
      {
      tmp[0] = (val[0]*tmp[3]+0x7f)>>(8);
      tmp[1] = (val[1]*tmp[3]+0x7f)>>(8);
      tmp[2] = (val[2]*tmp[3]+0x7f)>>(8);

      unsigned short normal   = *dirPtr;
      VTKKWRCHelper_LookupShading( diffuseShadingTable[0], specularShadingTable[0], normal, tmp );
      VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
      }
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}

// This method is used when the interpolation type is nearest neighbor and
// the data has more than one component and the components are considered to
// be independent. In the inner loop we access each component value, using
// the scale/shift to turn the data value into an unsigned short index. We
// then lookup the color/opacity for each component and combine them according
// to the weighting value for each component. We composite this resulting
// color into the color already accumulated for this ray, and we check
// wether we can terminate here (if the accumulated opacity exceeds some
// threshold). Finally we increment to the next sample on the ray.
//
// TODO: short circuit calculations when opacity is 0
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageIndependentNN( T *data,
                                                     int threadID,
                                                     int threadCount,
                                                     vtkCropVolumeRayCastMapper *mapper,
                                                     vtkVolume *vol)
{
  VTKKWRCHelper_InitializeWeights();
  VTKKWRCHelper_InitializationAndLoopStartShadeNN();
  VTKKWRCHelper_InitializeCompositeMultiNN();
  VTKKWRCHelper_InitializeCompositeShadeNN();

  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      VTKKWRCHelper_MoveToNextSampleShadeNN();
      }

    VTKKWRCHelper_CroppingCheckNN( pos );

    unsigned short normal[4];
    for ( c = 0; c < components; c++ )
      {
      val[c]    = static_cast<unsigned short>(((*(dptr+c)) + shift[c])*scale[c]);
      normal[c] = *(dirPtr+c);
      }

    VTKKWRCHelper_LookupAndCombineIndependentColorsShadeUS( colorTable, scalarOpacityTable,
                                                            diffuseShadingTable,
                                                            specularShadingTable,
                                                            val, normal, weights,
                                                            components, tmp );

    if ( tmp[3] )
      {
      VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
      }
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}

// This method is used when the interpolation type is linear and the data
// has one component and scale = 1.0 and shift = 0.0. In the inner loop we
// get the data value for the eight cell corners (if we have changed cells)
// as an unsigned short (the range must be right and we don't need the
// scale/shift). We compute our weights within the cell according to our
// fractional position within the cell, apply trilinear interpolation to
// compute the index, and use this index to lookup a color and opacity for
// this sample. We then composite this into the color computed so far along
// the ray, and check if we can terminate at this point (if the accumulated
// opacity is higher than some threshold). Finally we move on to the next
// sample along the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageOneSimpleTrilin( T *data,
                                                       int threadID,
                                                       int threadCount,
                                                       vtkCropVolumeRayCastMapper *mapper,
                                                       vtkVolume *vol)
{
  VTKKWRCHelper_InitializationAndLoopStartShadeTrilin();
  VTKKWRCHelper_InitializeCompositeOneTrilin();
  VTKKWRCHelper_InitializeCompositeOneShadeTrilin();
  VTKKWRCHelper_SpaceLeapSetup();

 // printf("in lit trilinear\n"); 

  int needToSampleDirection = 0;
  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      mapper->FixedPointIncrement( pos, dir );
      }

    VTKKWRCHelper_SpaceLeapCheck();
    VTKKWRCHelper_CroppingCheckTrilin( pos );

    mapper->ShiftVectorDown( pos, spos );
    if ( spos[0] != oldSPos[0] ||
         spos[1] != oldSPos[1] ||
         spos[2] != oldSPos[2] )
      {
      oldSPos[0] = spos[0];
      oldSPos[1] = spos[1];
      oldSPos[2] = spos[2];

      dptr = data + spos[0]*inc[0] + spos[1]*inc[1] + spos[2]*inc[2];
      VTKKWRCHelper_GetCellScalarValuesSimple( dptr );
      dirPtrABCD = gradientDir[spos[2]  ] + spos[0]*dInc[0] + spos[1]*dInc[1];
      dirPtrEFGH = gradientDir[spos[2]+1] + spos[0]*dInc[0] + spos[1]*dInc[1];
      needToSampleDirection = 1;
      }

    VTKKWRCHelper_ComputeWeights(pos);
    VTKKWRCHelper_InterpolateScalar(val);

    VTKKWRCHelper_LookupColorUS( colorTable[0], scalarOpacityTable[0], val, tmp );

	ShadeIrregularCropper(&val, tmp, pos, mapper);	

    if ( needToSampleDirection )
      {
      VTKKWRCHelper_GetCellDirectionValues( dirPtrABCD, dirPtrEFGH );
      needToSampleDirection = 0;
      }

    VTKKWRCHelper_InterpolateShading( diffuseShadingTable[0], specularShadingTable[0], tmp );
    VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}

// This method is used when the interpolation type is linear and the data
// has one component and scale != 1.0 or shift != 0.0. In the inner loop we
// get the data value for the eight cell corners (if we have changed cells)
// as an unsigned short (we use the scale/shift to ensure the correct range).
// We compute our weights within the cell according to our fractional position
// within the cell, apply trilinear interpolation to compute the index, and use
// this index to lookup a color and opacity for this sample. We then composite
// this into the color computed so far along the ray, and check if we can
// terminate at this point (if the accumulated opacity is higher than some
// threshold). Finally we move on to the next sample along the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageOneTrilin( T *data,
                                                 int threadID,
                                                 int threadCount,
                                                 vtkCropVolumeRayCastMapper *mapper,
                                                 vtkVolume *vol)
{
  VTKKWRCHelper_InitializationAndLoopStartShadeTrilin();
  VTKKWRCHelper_InitializeCompositeOneTrilin();
  VTKKWRCHelper_InitializeCompositeOneShadeTrilin();
  VTKKWRCHelper_SpaceLeapSetup();

  int needToSampleDirection = 0;
  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      mapper->FixedPointIncrement( pos, dir );
      }

    VTKKWRCHelper_SpaceLeapCheck();
    VTKKWRCHelper_CroppingCheckTrilin( pos );

    mapper->ShiftVectorDown( pos, spos );
    if ( spos[0] != oldSPos[0] ||
         spos[1] != oldSPos[1] ||
         spos[2] != oldSPos[2] )
      {
      oldSPos[0] = spos[0];
      oldSPos[1] = spos[1];
      oldSPos[2] = spos[2];


      dptr = data + spos[0]*inc[0] + spos[1]*inc[1] + spos[2]*inc[2];
      VTKKWRCHelper_GetCellScalarValues( dptr, scale[0], shift[0] );
      dirPtrABCD = gradientDir[spos[2]  ] + spos[0]*dInc[0] + spos[1]*dInc[1];
      dirPtrEFGH = gradientDir[spos[2]+1] + spos[0]*dInc[0] + spos[1]*dInc[1];
      needToSampleDirection = 1;
      }

    VTKKWRCHelper_ComputeWeights(pos);
    VTKKWRCHelper_InterpolateScalar(val);

    VTKKWRCHelper_LookupColorUS( colorTable[0], scalarOpacityTable[0], val, tmp );
    if ( needToSampleDirection )
      {
      VTKKWRCHelper_GetCellDirectionValues( dirPtrABCD, dirPtrEFGH );
      needToSampleDirection = 0;
      }
    VTKKWRCHelper_InterpolateShading( diffuseShadingTable[0], specularShadingTable[0], tmp );
    VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}


// This method is used when the interpolation type is linear, the data has
// two components and the components are not considered independent. In the
// inner loop we get the data value for the eight cell corners (if we have
// changed cells) for both components as an unsigned shorts (we use the
// scale/shift to ensure the correct range). We compute our weights within
// the cell according to our fractional position within the cell, and apply
// trilinear interpolation to compute the two index value. We use the first
// index to lookup a color and the second to look up an opacity for this sample.
// We then composite this into the color computed so far along the ray, and
// check if we can terminate at this point (if the accumulated opacity is
// higher than some threshold). Finally we move on to the next sample along
// the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageTwoDependentTrilin( T *data,
                                                          int threadID,
                                                          int threadCount,
                                                          vtkCropVolumeRayCastMapper *mapper,
                                                          vtkVolume *vol)
{
  VTKKWRCHelper_InitializationAndLoopStartShadeTrilin();
  VTKKWRCHelper_InitializeCompositeMultiTrilin();
  VTKKWRCHelper_InitializeCompositeOneShadeTrilin();
  VTKKWRCHelper_SpaceLeapSetup();

  int needToSampleDirection = 0;
  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      mapper->FixedPointIncrement( pos, dir );
      }

    VTKKWRCHelper_SpaceLeapCheck();
    VTKKWRCHelper_CroppingCheckTrilin( pos );

    mapper->ShiftVectorDown( pos, spos );
    if ( spos[0] != oldSPos[0] ||
         spos[1] != oldSPos[1] ||
         spos[2] != oldSPos[2] )
      {
      oldSPos[0] = spos[0];
      oldSPos[1] = spos[1];
      oldSPos[2] = spos[2];

      dptr = data + spos[0]*inc[0] + spos[1]*inc[1] + spos[2]*inc[2];
      VTKKWRCHelper_GetCellComponentScalarValues( dptr, 0, scale[0], shift[0] );

      dptr++;
      VTKKWRCHelper_GetCellComponentScalarValues( dptr, 1, scale[1], shift[1] );

      dirPtrABCD = gradientDir[spos[2]  ] + spos[0]*dInc[0] + spos[1]*dInc[1];
      dirPtrEFGH = gradientDir[spos[2]+1] + spos[0]*dInc[0] + spos[1]*dInc[1];
      needToSampleDirection = 1;
      }

    VTKKWRCHelper_ComputeWeights(pos);
    VTKKWRCHelper_InterpolateScalarComponent( val, c, 2 );

    tmp[3] = scalarOpacityTable[0][val[1]];
    if ( !tmp[3] )
      {
      continue;
      }

    if ( needToSampleDirection )
      {
      VTKKWRCHelper_GetCellDirectionValues( dirPtrABCD, dirPtrEFGH );
      needToSampleDirection = 0;
      }

    tmp[0] = static_cast<unsigned short>
      ((colorTable[0][3*val[0]  ]*tmp[3] + 0x7fff)>>(VTKKW_FP_SHIFT));
    tmp[1] = static_cast<unsigned short>
      ((colorTable[0][3*val[0]+1]*tmp[3] + 0x7fff)>>(VTKKW_FP_SHIFT));
    tmp[2] = static_cast<unsigned short>
      ((colorTable[0][3*val[0]+2]*tmp[3] + 0x7fff)>>(VTKKW_FP_SHIFT));

    VTKKWRCHelper_InterpolateShading( diffuseShadingTable[0], specularShadingTable[0], tmp );
    VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );

    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}



// This method is used when the interpolation type is linear, the data has
// four components and the components are not considered independent. In the
// inner loop we get the data value for the eight cell corners (if we have
// changed cells) for all components as an unsigned shorts (we don't have to
// use the scale/shift because only unsigned char data is supported for four
// component data when the components are not independent). We compute our
// weights within the cell according to our fractional position within the cell,
// and apply trilinear interpolation to compute a value for each component. We
// use the first three directly as the color of the sample, and the fourth is
// used to look up an opacity for this sample. We then composite this into the
// color computed so far along the ray, and check if we can terminate at this
// point (if the accumulated opacity is higher than some threshold). Finally we
// move on to the next sample along the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageFourDependentTrilin( T *data,
                                                           int threadID,
                                                           int threadCount,
                                                           vtkCropVolumeRayCastMapper *mapper,
                                                           vtkVolume *vol)
{
  VTKKWRCHelper_InitializationAndLoopStartShadeTrilin();
  VTKKWRCHelper_InitializeCompositeMultiTrilin();
  VTKKWRCHelper_InitializeCompositeOneShadeTrilin();
  VTKKWRCHelper_SpaceLeapSetup();

  int needToSampleDirection = 0;
  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      mapper->FixedPointIncrement( pos, dir );
      }

    VTKKWRCHelper_SpaceLeapCheck();
    VTKKWRCHelper_CroppingCheckTrilin( pos );

    mapper->ShiftVectorDown( pos, spos );
    if ( spos[0] != oldSPos[0] ||
         spos[1] != oldSPos[1] ||
         spos[2] != oldSPos[2] )
      {
      oldSPos[0] = spos[0];
      oldSPos[1] = spos[1];
      oldSPos[2] = spos[2];

      dptr = data + spos[0]*inc[0] + spos[1]*inc[1] + spos[2]*inc[2];
      VTKKWRCHelper_GetCellComponentRawScalarValues( dptr, 0 );

      dptr++;
      VTKKWRCHelper_GetCellComponentRawScalarValues( dptr, 1 );

      dptr++;
      VTKKWRCHelper_GetCellComponentRawScalarValues( dptr, 2 );

      dptr++;
      VTKKWRCHelper_GetCellComponentScalarValues( dptr, 3, scale[3], shift[3] );

      dirPtrABCD = gradientDir[spos[2]  ] + spos[0]*dInc[0] + spos[1]*dInc[1];
      dirPtrEFGH = gradientDir[spos[2]+1] + spos[0]*dInc[0] + spos[1]*dInc[1];
      needToSampleDirection = 1;
      }

    VTKKWRCHelper_ComputeWeights(pos);
    VTKKWRCHelper_InterpolateScalarComponent( val, c, components );

	ShadeIrregularCropperArray(val, tmp, pos, mapper);

    tmp[3] = scalarOpacityTable[0][val[3]];
    if ( !tmp[3] )
      {
      continue;
      }

    if ( needToSampleDirection )
      {
      VTKKWRCHelper_GetCellDirectionValues( dirPtrABCD, dirPtrEFGH );
      needToSampleDirection = 0;
      }

    tmp[0] = (val[0]*tmp[3]+0x7f)>>8;
    tmp[1] = (val[1]*tmp[3]+0x7f)>>8;
    tmp[2] = (val[2]*tmp[3]+0x7f)>>8;

    VTKKWRCHelper_InterpolateShading( diffuseShadingTable[0], specularShadingTable[0], tmp );
    VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}

// This method is used when the interpolation type is linear, the data has
// more than one component and the components are considered independent. In
// the inner loop we get the data value for the eight cell corners (if we have
// changed cells) for all components as an unsigned shorts (we have to use the
// scale/shift to ensure that we obtained unsigned short indices) We compute our
// weights within the cell according to our fractional position within the cell,
// and apply trilinear interpolation to compute a value for each component. We
// look up a color/opacity for each component and blend them according to the
// component weights. We then composite this resulting color into the
// color computed so far along the ray, and check if we can terminate at this
// point (if the accumulated opacity is higher than some threshold). Finally we
// move on to the next sample along the ray.
template <class T>
void vtkCropFixedPointCompositeShadeHelperGenerateImageIndependentTrilin( T *data,
                                                         int threadID,
                                                         int threadCount,
                                                         vtkCropVolumeRayCastMapper *mapper,
                                                         vtkVolume *vol)
{
  VTKKWRCHelper_InitializeWeights();
  VTKKWRCHelper_InitializationAndLoopStartShadeTrilin();
  VTKKWRCHelper_InitializeCompositeMultiTrilin();
  VTKKWRCHelper_InitializeCompositeMultiShadeTrilin();

  for ( k = 0; k < numSteps; k++ )
    {
    if ( k )
      {
      mapper->FixedPointIncrement( pos, dir );
      }

    VTKKWRCHelper_CroppingCheckTrilin( pos );

    mapper->ShiftVectorDown( pos, spos );
    if ( spos[0] != oldSPos[0] ||
         spos[1] != oldSPos[1] ||
         spos[2] != oldSPos[2] )
      {
      oldSPos[0] = spos[0];
      oldSPos[1] = spos[1];
      oldSPos[2] = spos[2];

      dptr = data + spos[0]*inc[0] + spos[1]*inc[1] + spos[2]*inc[2];
      VTKKWRCHelper_GetCellComponentScalarValues( dptr, 0, scale[0], shift[0] );

      dptr++;
      VTKKWRCHelper_GetCellComponentScalarValues( dptr, 1, scale[1], shift[1] );

      if ( components > 2 )
        {
        dptr++;
        VTKKWRCHelper_GetCellComponentScalarValues( dptr, 2, scale[2], shift[2] );
        if ( components > 3 )
          {
          dptr++;
          VTKKWRCHelper_GetCellComponentScalarValues( dptr, 3, scale[3], shift[3] );
          }
        }


      dirPtrABCD = gradientDir[spos[2]  ] + spos[0]*dInc[0] + spos[1]*dInc[1];
      dirPtrEFGH = gradientDir[spos[2]+1] + spos[0]*dInc[0] + spos[1]*dInc[1];
      VTKKWRCHelper_GetCellComponentDirectionValues( dirPtrABCD, dirPtrEFGH, 0 );

      dirPtrABCD++;
      dirPtrEFGH++;
      VTKKWRCHelper_GetCellComponentDirectionValues( dirPtrABCD, dirPtrEFGH, 1 );

      if ( components > 2 )
        {
        dirPtrABCD++;
        dirPtrEFGH++;
        VTKKWRCHelper_GetCellComponentDirectionValues( dirPtrABCD, dirPtrEFGH, 2 );
        if ( components > 3 )
          {
          dirPtrABCD++;
          dirPtrEFGH++;
          VTKKWRCHelper_GetCellComponentDirectionValues( dirPtrABCD, dirPtrEFGH, 3 );
          }
        }
      }

    VTKKWRCHelper_ComputeWeights(pos);
    VTKKWRCHelper_InterpolateScalarComponent( val, c, components );

    VTKKWRCHelper_LookupAndCombineIndependentColorsInterpolateShadeUS(
      colorTable, scalarOpacityTable, diffuseShadingTable,
      specularShadingTable, val, weights, components, tmp );

    VTKKWRCHelper_CompositeColorAndCheckEarlyTermination( color, tmp, remainingOpacity );
    }

  VTKKWRCHelper_SetPixelColor( imagePtr, color, remainingOpacity );
  VTKKWRCHelper_IncrementAndLoopEnd();
}


void vtkCropVolumeRayCastCompositeShadeHelper::GenerateImage(
  int threadID,
  int threadCount,
  vtkVolume *vol,
  vtkCropVolumeRayCastMapper *mapper )
{
  void *data     = mapper->GetCurrentScalars()->GetVoidPointer(0);
  int scalarType = mapper->GetCurrentScalars()->GetDataType();

  // Nearest Neighbor interpolate
  if ( mapper->ShouldUseNearestNeighborInterpolation( vol ) )
    {
    // One component data
    if ( mapper->GetCurrentScalars()->GetNumberOfComponents() == 1 )
      {
      // Scale == 1.0 and shift == 0.0 - simple case (faster)
      if ( mapper->GetTableScale()[0] == 1.0 &&
           mapper->GetTableShift()[0] == 0.0 )
        {
        switch ( scalarType )
          {
          vtkTemplateMacro(
            vtkCropFixedPointCompositeShadeHelperGenerateImageOneSimpleNN(
              static_cast<VTK_TT *>(data),
              threadID, threadCount, mapper, vol) );
          }
        }
      else
        {
        switch ( scalarType )
          {
          vtkTemplateMacro(
            vtkCropFixedPointCompositeShadeHelperGenerateImageOneNN(
              static_cast<VTK_TT *>(data),
              threadID, threadCount, mapper, vol) );
          }
        }
      }
    // More that one independent components
    else if ( vol->GetProperty()->GetIndependentComponents() )
      {
      switch ( scalarType )
        {
        vtkTemplateMacro(
          vtkCropFixedPointCompositeShadeHelperGenerateImageIndependentNN(
            static_cast<VTK_TT *>(data),
            threadID, threadCount, mapper, vol) );
        }
      }
    // Dependent (color) components
    else
      {
      // Two components - the first specifies color (through a lookup table)
      // and the second specified opacity (through a lookup table)
      if ( mapper->GetCurrentScalars()->GetNumberOfComponents() == 2 )
        {
        switch ( scalarType )
          {
          vtkTemplateMacro(
            vtkCropFixedPointCompositeShadeHelperGenerateImageTwoDependentNN(
              static_cast<VTK_TT *>(data),
              threadID, threadCount, mapper, vol) );
          }
        }
      // Four components - they must be unsigned char, the first three directly
      // specify color and the fourth specifies opacity (through a lookup
      // table)
      else
        {
        if ( scalarType == VTK_UNSIGNED_CHAR )
          {
          vtkCropFixedPointCompositeShadeHelperGenerateImageFourDependentNN(
            static_cast<unsigned char *>(data), threadID, threadCount, mapper,
            vol );
          }
        else
          {
          vtkErrorMacro("Four component dependent data must be unsigned char");
          }
        }
      }
    }
  // Trilinear Interpolation
  else
    {
    // One component
    if ( mapper->GetCurrentScalars()->GetNumberOfComponents() == 1 )
      {
      // Scale == 1.0 and shift == 0.0 - simple case (faster)
      if ( mapper->GetTableScale()[0] == 1.0 &&
           mapper->GetTableShift()[0] == 0.0 )
        {
        switch ( scalarType )
          {
          vtkTemplateMacro(
            vtkCropFixedPointCompositeShadeHelperGenerateImageOneSimpleTrilin(
              static_cast<VTK_TT *>(data),
              threadID, threadCount, mapper, vol) );
          }
        }
      // Scale != 1.0 or shift != 0.0 - must apply scale/shift in inner loop
      else
        {
        switch ( scalarType )
          {
          vtkTemplateMacro(
            vtkCropFixedPointCompositeShadeHelperGenerateImageOneTrilin(
              static_cast<VTK_TT *>(data),
              threadID, threadCount, mapper, vol) );
          }
        }
      }
    // Indepedent components (more than one)
    else if ( vol->GetProperty()->GetIndependentComponents() )
      {
      switch ( scalarType )
        {
        vtkTemplateMacro(
          vtkCropFixedPointCompositeShadeHelperGenerateImageIndependentTrilin(
            static_cast<VTK_TT *>(data),
            threadID, threadCount, mapper, vol) );
        }
      }
    // Dependent components
    else
      {
      // Two components - the first specifies color (through a lookup table)
      // and the second specified opacity (through a lookup table)
      if ( mapper->GetCurrentScalars()->GetNumberOfComponents() == 2 )
        {
        switch ( scalarType )
          {
          vtkTemplateMacro(
            vtkCropFixedPointCompositeShadeHelperGenerateImageTwoDependentTrilin(
              static_cast<VTK_TT *>(data),
              threadID, threadCount, mapper, vol) );
          }
        }
      // Four components - they must be unsigned char, the first three directly
      // specify color and the fourth specifies opacity (through a lookup
      // table)
      else
        {
        if ( scalarType == VTK_UNSIGNED_CHAR )
          {
          vtkCropFixedPointCompositeShadeHelperGenerateImageFourDependentTrilin(
            static_cast<unsigned char *>(data), threadID, threadCount, mapper,
            vol );
          }
        else
          {
          vtkErrorMacro("Four component dependent data must be unsigned char");
          }
        }
      }
    }
}

// Print method for vtkCropVolumeRayCastCompositeShadeHelper
void vtkCropVolumeRayCastCompositeShadeHelper::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}


void  ShadeIrregularCropper(unsigned short *val, unsigned short tmp[4], unsigned int pos[3], vtkCropVolumeRayCastMapper *mapper)
{

	double distance_from_point = 0.0;
	int translucency_set = 0;
	float end_vector[3];

	float ray_position[3];
	ray_position[0] = pos[0]/32767 ;
	ray_position[1] = pos[1]/32767;
	ray_position[2] = pos[2]/32767;

	double locationxyz[3];
	double FixedCubeLength[3];
	double FixedEllipseRadius[3];
	double CylinderAxis[3];

	locationxyz[0] = locationxyz[1] = locationxyz[2] = 0.0;
	mapper->GetFixedSphereCenter(locationxyz);

	float angle = 0, centroid_angle =0;
	float vector1[3], vector2[3], centroid_vector[3], center_distance[3];
	float total_distance = 0;
	int c = 0;
	int i, j;
	int score = 0;
	
	switch(mapper->GetProbeShape())
	{
	case 0:
		// sphere
		distance_from_point = sqrt(pow((double)ray_position[0]-locationxyz[0], 2.0) +
			pow((double)ray_position[1]-locationxyz[1], 2.0) +
			pow((double)ray_position[2]-locationxyz[2], 2.0));
	
		// for translucency
		if(distance_from_point < mapper->GetFixedSphereRadius())
		{
			if(*val> 250)
				translucency_set = 2;
			else
				translucency_set = 1;

			*val= 0;
			tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
		}
		break;

	case 1:
		// cube
		
		mapper->GetFixedCubeLength(FixedCubeLength);
		if(fabs(ray_position[0]-(float)locationxyz[0]) < FixedCubeLength[0]/2.0 &&
			fabs(ray_position[1]-(float)locationxyz[1]) < FixedCubeLength[1]/2.0 &&
			fabs(ray_position[2]-(float)locationxyz[2]) < FixedCubeLength[2]/2.0)
		{

			if(*val > 250)
				translucency_set = 2;
			else
				translucency_set = 1;

			//printf("getting here in irregular cropper\n"); 

			*val = 0; tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;			
			if(translucency_set > 1)
				tmp[2] = 50;

		}

		//angle = 0;

		//center_distance[0] = points[0][0]- centroid[0];
		//center_distance[1] = points[0][1]- centroid[1];
		//center_distance[2] = points[0][2]- centroid[2];

		//total_distance = 0;
		//for(int index=0;index<number_of_landmarks-1;index++)
		//{
		//	// ray - index, index - index+1 < 359

		//	vector1[0] = points[index][0]- ray_position[0];
		//	vector1[1] = points[index][1]- ray_position[1];
		//	vector1[2] = points[index][2]- ray_position[2];

		//	vector2[0] = points[index+1][0]- ray_position[0];
		//	vector2[1] = points[index+1][1]- ray_position[1];
		//	vector2[2] = points[index+1][2]- ray_position[2];

		//	//angle = (vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2])/;
		//	angle += acos(dotproduct(vector1, vector2)/(vector_magnitude(vector1) * vector_magnitude(vector2)));

		//	centroid_vector[0] = ray_position[0]- centroid[0];
		//	centroid_vector[1] = ray_position[1]- centroid[1];
		//	centroid_vector[2] = ray_position[2]- centroid[2];

		//	total_distance+= vector_magnitude(centroid_vector);
		//	//printf("inside angle =%f\n", angle);
		//}

		////printf("angle =%f\n", angle);

		//if(((angle*180.0)/3.14) > 315)
		//{
		//	/*if(ray_position[0] > minpos[0] && ray_position[1] > minpos[1] && ray_position[2] > minpos[2] &&
		//		ray_position[0] < maxpos[0] && ray_position[1] < maxpos[1] && ray_position[2] < maxpos[2])
		//	{*/
		//	if(*val>10)
		//	{
		//		tmp[0] = tmp[1] = tmp[3] = 0;
		//		tmp[2] = 100; 
		//	}
		//	else
		//	{
		//		tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
		//	}
		//	*val = 0; 
		//	//}
		//}


		break;

	case 2:
		// ellipse
		
		mapper->GetFixedEllipseRadius(FixedEllipseRadius); 

		distance_from_point = pow(ray_position[0]-(float)locationxyz[0], 2.0f)/ pow(FixedEllipseRadius[0], 2.0) +
			pow(ray_position[1]-(float)locationxyz[1], 2.0f)/ pow(FixedEllipseRadius[1], 2.0) +
			pow(ray_position[2]-(float)locationxyz[2], 2.0f)/ pow(FixedEllipseRadius[2], 2.0); 

		if(distance_from_point < 1.0)
		{
			//printf("distance is %.2f\n", distance_from_point); 
			//printf("%f %f %f\n", FixedEllipseRadius[0], FixedEllipseRadius[1], FixedEllipseRadius[2]); 

			if(*val > 20)
				translucency_set = 2;
			else
				translucency_set = 1;

			//printf("getting here in irregular cropper\n"); 

			*val = pow(2.0, distance_from_point); tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;			
			if(translucency_set > 1)
				tmp[2] = 10;

		}		
		break;

	case 3:
		// cylinder
		
		mapper->GetFixedCylinderAxis(CylinderAxis);

		for(float t=0;t<mapper->GetFixedCylinderHeight();t=t+mapper->GetFixedCylinderHeight()/10)
		{
			end_vector[0] = locationxyz[0] - ( CylinderAxis[0] * t);
			end_vector[1] = locationxyz[1] - ( CylinderAxis[1] * t);
			end_vector[2] = locationxyz[2] - ( CylinderAxis[2] * t);

			distance_from_point = sqrt(pow(ray_position[0]-(float)end_vector[0], 2.0f) +
				pow(ray_position[1]-(float)end_vector[1], 2.0f) +
				pow(ray_position[2]-(float)end_vector[2], 2.0f));

			//if(distance_from_point < mapper->GetCylinderRadius())
			if(distance_from_point < mapper->GetFixedCylinderRadius())
			{

				if(*val > 0)
					translucency_set = 2;
				else
					translucency_set = 1;
				*val = 0; tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
				break;
			}
		}


		break;

	default:
		// sphere
		
		distance_from_point = sqrt(pow(ray_position[0]-(float)locationxyz[0], 2.0f) +
			pow(ray_position[1]-(float)locationxyz[1], 2.0f) +
			pow(ray_position[2]-(float)locationxyz[2], 2.0f));
		if(distance_from_point < mapper->GetFixedSphereRadius())
		{
			translucency_set = 1;
			if(*val > 0)
				translucency_set = 2;
			else
				translucency_set = 1;
			*val = 0; tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
		}
		break;
	}
}


void ShadeIrregularCropperArray(unsigned short val[4], unsigned short tmp[4], unsigned int pos[3], vtkCropVolumeRayCastMapper *mapper)
{

	double distance_from_point = 0.0;
	int translucency_set = 0;
	float end_vector[3];

	float ray_position[3];
	ray_position[0] = pos[0]/32767 ;
	ray_position[1] = pos[1]/32767;
	ray_position[2] = pos[2]/32767;

	//double CubeX, CubeY, CubeZ;

	double locationxyz[3];
	double FixedCubeLength[3];
	double FixedEllipseRadius[3];
	double CylinderAxis[3];

	locationxyz[0] = locationxyz[1] = locationxyz[2] = 0.0;
	mapper->GetFixedSphereCenter(locationxyz);

	int greyscale_flag = 0; 

	float angle = 0, centroid_angle =0;
	float vector1[3], vector2[3], centroid_vector[3], center_distance[3];
	float total_distance = 0;
	int c = 0;
	int i, j;
	int score = 0;
	double axis_magnitude = 0; 


	switch(mapper->GetProbeShape())
	{
	case 0:
		// sphere
		distance_from_point = sqrt(pow((double)ray_position[0]-locationxyz[0], 2.0) +
			pow((double)ray_position[1]-locationxyz[1], 2.0) +
			pow((double)ray_position[2]-locationxyz[2], 2.0));
		// for translucency
		if(distance_from_point < mapper->GetFixedSphereRadius())
		{
			if(val[0] > 10)
				translucency_set = 2;
			else
				translucency_set = 1;

			// Check if functional crop is set - If set then dont crop functional data
			if(mapper->GetFunctionalCropOn() == 1)
			{
				if(abs(val[0] - val[1]) < 1)
				{
					if(abs(val[1] - val[2]) < 1)
					{
						greyscale_flag = 1; 
					}
				}

				if(greyscale_flag == 1)
				{					
					if(translucency_set == 2)
					{	
						val[0] = val[1] = val[2] = val[3] = 0;
						tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
						tmp[2] = 10;
					}
					else
					{
						val[0] = val[1] = val[2] = val[3] = 0;
						tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;

					}
				}
			}
			else // Crop everything including functional data. 
			{
				val[0] = val[1] = val[2] = val[3] = 0;
				tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
			}
		}
		break;

	case 1:
		// cube
		//angle = 0;

		//center_distance[0] = points[0][0]- centroid[0];
		//center_distance[1] = points[0][1]- centroid[1];
		//center_distance[2] = points[0][2]- centroid[2];

		//total_distance = 0;
		//for(int index=0;index<number_of_landmarks-1;index++)
		//{
		//	// ray - index, index - index+1 < 359

		//	vector1[0] = points[index][0]- ray_position[0];
		//	vector1[1] = points[index][1]- ray_position[1];
		//	vector1[2] = points[index][2]- ray_position[2];

		//	vector2[0] = points[index+1][0]- ray_position[0];
		//	vector2[1] = points[index+1][1]- ray_position[1];
		//	vector2[2] = points[index+1][2]- ray_position[2];

		//	//angle = (vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2])/;
		//	angle += acos(dotproduct(vector1, vector2)/(vector_magnitude(vector1) * vector_magnitude(vector2)));

		//	centroid_vector[0] = ray_position[0]- centroid[0];
		//	centroid_vector[1] = ray_position[1]- centroid[1];
		//	centroid_vector[2] = ray_position[2]- centroid[2];

		//	total_distance+= vector_magnitude(centroid_vector);
		//	//printf("inside angle =%f\n", angle);
		//}

		////printf("angle =%f\n", angle);

		//if(((angle*180.0)/3.14) > 315)
		//{
		//	/*if(ray_position[0] > minpos[0] && ray_position[1] > minpos[1] && ray_position[2] > minpos[2] &&
		//		ray_position[0] < maxpos[0] && ray_position[1] < maxpos[1] && ray_position[2] < maxpos[2])
		//	{*/
		//	/*if(*val>10)
		//	{
		//		tmp[0] = tmp[1] = tmp[3] = 0;
		//		tmp[2] = 100; 
		//	}
		//	else
		//	{
		//		tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
		//	}
		//	val[0] = val[1] = val[2] = val[3] = 0;	*/		

		//	if(mapper->GetFunctionalCropOn() == 1)
		//	{
		//		if(abs(val[0] - val[1]) < 1)
		//		{
		//			if(abs(val[1] - val[2]) < 1)
		//			{
		//				greyscale_flag = 1; 
		//			}
		//		}

		//		if(greyscale_flag == 1)
		//		{
		//			val[0] = val[1] = val[2] = val[3] = 0;
		//			tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
		//			if(translucency_set > 0)
		//			{
		//				tmp[2] = 50;
		//			}
		//			else
		//			{
		//				tmp[2] = 0;
		//			}
		//		}
		//	}
		//	else // Crop everything including functional data. 
		//	{
		//		val[0] = val[1] = val[2] = val[3] = 0;
		//		tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
		//	}


		//	//}
		//}

		// old stuff 
		mapper->GetFixedCubeLength(FixedCubeLength);
		if(fabs(ray_position[0]-(float)locationxyz[0]) < FixedCubeLength[0]/2.0 &&
		   fabs(ray_position[1]-(float)locationxyz[1]) < FixedCubeLength[1]/2.0 &&
		   fabs(ray_position[2]-(float)locationxyz[2]) < FixedCubeLength[2]/2.0)
		  {

			if(val[0] > 10)
				translucency_set = 2;
			else
				translucency_set = 1;

			// Check if functional crop is set - If set then dont crop functional data
			if(mapper->GetFunctionalCropOn() == 1)
			{
				if(abs(val[0] - val[1]) < 1)
				{
					if(abs(val[1] - val[2]) < 1)
					{
						greyscale_flag = 1; 
					}
				}

				if(greyscale_flag == 1)
				{
					val[0] = val[1] = val[2] = val[3] = 0;
					tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
					if(translucency_set > 0)
					{
						tmp[2] = 10;
					}
					else
					{
						tmp[2] = 0;
					}
				}
			}
			else // Crop everything including functional data. 
			{
				val[0] = val[1] = val[2] = val[3] = 0;
				tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
			}
		}
		break;

	case 2:
		// ellipse 
		distance_from_point = pow(ray_position[0]-(float)locationxyz[0], 2.0f)/50.0 +
			pow(ray_position[1]-(float)locationxyz[1], 2.0f)/100.0 +
			pow(ray_position[2]-(float)locationxyz[2], 2.0f)/50.0;

		  if(distance_from_point < mapper->GetFixedSphereRadius())
		{
		
			if(val[0] > 0)
				translucency_set = 2;
			else
				translucency_set = 1; 			
		
			// Check if functional crop is set - If set then dont crop functional data
			if(mapper->GetFunctionalCropOn() == 1)
			{
				if(abs(val[0] - val[1]) < 1)
				{
					if(abs(val[1] - val[2]) < 1)
					{
						greyscale_flag = 1; 
					}
				}

				if(greyscale_flag == 1)
				{
					
					if(translucency_set == 2)
					{	
						val[0] = val[1] = val[2] = val[3] = 0;
						tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
						tmp[2] = 50;
					}
					else
					{
						val[0] = val[1] = val[2] = val[3] = 0;
						tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;

					}
				}
			}
			else // Crop everything including functional data. 
			{
				val[0] = val[1] = val[2] = val[3] = 0;
				tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
			}
		}

		break;


	case 3:
		// cylinder
		mapper->GetFixedCylinderAxis(CylinderAxis);
	
		axis_magnitude = sqrt(CylinderAxis[0] *  CylinderAxis[0] + 
							  CylinderAxis[1] *  CylinderAxis[1] +
							  CylinderAxis[2] *  CylinderAxis[2]); 

		CylinderAxis[0] /= axis_magnitude;
		CylinderAxis[1] /= axis_magnitude;
		CylinderAxis[2] /= axis_magnitude;

		
		//for(float t=0;t<mapper->GetFixedCylinderHeight();t=t+ mapper->GetFixedCylinderRadius()/4)
		for(float t=0;t<mapper->GetFixedCylinderHeight();t=t+ mapper->GetFixedCylinderHeight()/10)
		{
			end_vector[0] = locationxyz[0] - ( CylinderAxis[0] * t);
			end_vector[1] = locationxyz[1] - ( CylinderAxis[1] * t);
			end_vector[2] = locationxyz[2] - ( CylinderAxis[2] * t);

			distance_from_point = sqrt(pow(ray_position[0]-(float)end_vector[0], 2.0f) +
				pow(ray_position[1]-(float)end_vector[1], 2.0f) +
				pow(ray_position[2]-(float)end_vector[2], 2.0f));

			if(distance_from_point < mapper->GetFixedCylinderRadius())				 
			{

				if(*val > 0)
					translucency_set = 2;
				else
					translucency_set = 1;

				// Check if functional crop is set - If set then dont crop functional data
				if(mapper->GetFunctionalCropOn() == 1)
				{
					if(abs(val[0] - val[1]) < 1)
					{
						if(abs(val[1] - val[2]) < 1)
						{
							greyscale_flag = 1; 
						}
					}

					if(greyscale_flag == 1)
					{
						val[0] = val[1] = val[2] = val[3] = 0;
						tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
					}
				}
				else // Crop everything including functional data. 
				{
					val[0] = val[1] = val[2] = val[3] = 0;
					tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
				}

				break;
			}
		}


		break;

	default:
		// sphere

		distance_from_point = sqrt(pow(ray_position[0]-(float)locationxyz[0], 2.0f) +
			pow(ray_position[1]-(float)locationxyz[1], 2.0f) +
			pow(ray_position[2]-(float)locationxyz[2], 2.0f));

		printf("in default: %d\n", mapper->GetProbeShape()); 

		if(distance_from_point < mapper->GetFixedSphereRadius())
		{
			translucency_set = 1;
			if(*val > 0)
				translucency_set = 2;
			else
				translucency_set = 1;

			// Check if functional crop is set - If set then dont crop functional data
			if(mapper->GetFunctionalCropOn() == 1)
			{
				if(abs(val[0] - val[1]) < 1)
				{
					if(abs(val[1] - val[2]) < 1)
					{
						greyscale_flag = 1; 
					}
				}

				if(greyscale_flag == 1)
				{
					val[0] = val[1] = val[2] = val[3] = 0;
					tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
				}
			}
			else // Crop everything including functional data. 
			{
				val[0] = val[1] = val[2] = val[3] = 0;
				tmp[0] = tmp[1] = tmp[2] = tmp[3] = 0;
			}
		}
		break;
	}
}

