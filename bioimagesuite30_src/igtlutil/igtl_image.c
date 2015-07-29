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

  Program:   Open ITK Link Library
  Module:    $RCSfile: $
  Language:  C
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <string.h>
#include <math.h>

#include "igtl_image.h"
#include "igtl_util.h"

igtl_uint64 igtl_image_get_data_size(igtl_image_header * header)
{
  igtl_uint64 si;
  igtl_uint64 sj;
  igtl_uint64 sk;
  igtl_uint64 sp;
  igtl_uint64 data_size;

  si = header->subvol_size[0];
  sj = header->subvol_size[1];
  sk = header->subvol_size[2];

  switch (header->scalar_type) {
  case IGTL_IMAGE_STYPE_TYPE_INT8:
  case IGTL_IMAGE_STYPE_TYPE_UINT8:
    sp = 1;
    break;
  case IGTL_IMAGE_STYPE_TYPE_INT16:
  case IGTL_IMAGE_STYPE_TYPE_UINT16:
    sp = 2;
    break;
  case IGTL_IMAGE_STYPE_TYPE_INT32:
  case IGTL_IMAGE_STYPE_TYPE_UINT32:
    sp = 4;
    break;
  default:
    sp = 0;
    break;
  }

  data_size = si*sj*sk*sp;
  return data_size;
}


void igtl_image_set_matrix(float spacing[3], float origin[3],
                            float norm_i[3], float norm_j[3], float norm_k[3],
                            igtl_image_header * header)
{
  header->matrix[0]  = (igtl_float32) (norm_i[0] * spacing[0]);
  header->matrix[1]  = (igtl_float32) (norm_i[1] * spacing[0]);
  header->matrix[2]  = (igtl_float32) (norm_i[2] * spacing[0]);
  header->matrix[3]  = (igtl_float32) (norm_j[0] * spacing[1]);
  header->matrix[4]  = (igtl_float32) (norm_j[1] * spacing[1]);
  header->matrix[5]  = (igtl_float32) (norm_j[2] * spacing[1]);
  header->matrix[6]  = (igtl_float32) (norm_k[0] * spacing[2]);
  header->matrix[7]  = (igtl_float32) (norm_k[1] * spacing[2]);
  header->matrix[8]  = (igtl_float32) (norm_k[2] * spacing[2]);
  header->matrix[9]  = (igtl_float32) (origin[0]);
  header->matrix[10] = (igtl_float32) (origin[1]);
  header->matrix[11] = (igtl_float32) (origin[2]);
}

void igtl_image_get_matrix(float spacing[3], float origin[3],
                            float norm_i[3], float norm_j[3], float norm_k[3],
                            igtl_image_header * header)
{
  float tx;
  float ty;
  float tz;
  float sx;
  float sy;
  float sz;
  float nx;
  float ny;
  float nz;
  float px;
  float py;
  float pz;

  tx = (float) header->matrix[0];
  ty = (float) header->matrix[1];
  tz = (float) header->matrix[2];
  sx = (float) header->matrix[3];
  sy = (float) header->matrix[4];
  sz = (float) header->matrix[5];
  nx = (float) header->matrix[6];
  ny = (float) header->matrix[7];
  nz = (float) header->matrix[8];
  px = (float) header->matrix[9];
  py = (float) header->matrix[10];
  pz = (float) header->matrix[11];

  spacing[0] = sqrt(tx*tx + ty*ty + tz*tz);
  spacing[1] = sqrt(sx*sx + sy*sy + sz*sz);
  spacing[2] = sqrt(nx*nx + ny*ny + nz*nz);
  norm_i[0] = header->matrix[0]  / spacing[0];
  norm_i[1] = header->matrix[1]  / spacing[0];
  norm_i[2] = header->matrix[2]  / spacing[0];
  norm_j[0] = - header->matrix[3] / spacing[1];
  norm_j[1] = - header->matrix[4] / spacing[1];
  norm_j[2] = - header->matrix[5] / spacing[1];
  norm_k[0] = - header->matrix[6] / spacing[2];
  norm_k[1] = - header->matrix[7] / spacing[2];
  norm_k[2] = - header->matrix[8] / spacing[2];
  origin[0] = header->matrix[9];
  origin[1] = header->matrix[10];
  origin[2] = header->matrix[11];

}


void igtl_image_convert_byte_order(igtl_image_header * header)
{
  int i;
  igtl_uint32 tmp[12];

  if (igtl_is_little_endian()) 
    {

    header->version = BYTE_SWAP_INT16(header->version);

    for (i = 0; i < 3; i ++) 
      {
      header->size[i] = BYTE_SWAP_INT16(header->size[i]);
      header->subvol_size[i] = BYTE_SWAP_INT16(header->subvol_size[i]);
      header->subvol_offset[i] = BYTE_SWAP_INT16(header->subvol_offset[i]);
      }
    memcpy((void*)tmp, (void*)(header->matrix), sizeof(igtl_float32)*12);

    for (i = 0; i < 12; i ++) 
      {
      tmp[i] = BYTE_SWAP_INT32(tmp[i]);
      }

    memcpy((void*)(header->matrix), (void*)tmp, sizeof(igtl_float32)*12);
  }
}


igtl_uint64 igtl_image_get_crc(igtl_image_header * header, void* image)
{
  igtl_uint64   crc;
  igtl_uint64   img_size;

  img_size = igtl_image_get_data_size(header);
  crc = crc64(0, 0, 0);
  crc = crc64((unsigned char*) header, IGTL_IMAGE_HEADER_SIZE, crc);
  crc = crc64((unsigned char*) image, img_size, crc);

  return crc;
}

