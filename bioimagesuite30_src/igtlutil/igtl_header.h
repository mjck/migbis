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

#ifndef __IGTL_HEADER_H
#define __IGTL_HEADER_H

#define IGTL_HEADER_VERSION   1
#define IGTL_HEADER_SIZE      58

#define IGTL_HEADER_NAMESIZE  12
#define IGTL_HEADER_DEVSIZE   20

#include "igtl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)     /* For 1-byte boundary in memroy */

/*
 * Message header of OpenIGTLink message.
 *
 * igtl_header is an overall data header for OpenIGTLink protocol.
 * It is transfered at beginning of every OpenIGTLink message to give
 * type and size of following data body to a receiver.
 * These parameters allow the receiver to parse or skip the data body.
 */

typedef struct {
  igtl_uint16    version;          /* protocol version number */
  char           name[IGTL_HEADER_NAMESIZE];       /* data type name          */
  char           device_name[IGTL_HEADER_DEVSIZE]; /* device name             */
  igtl_uint64    timestamp;        /* time stamp message      */
  igtl_uint64    body_size;        /* size of the body        */
  igtl_uint64    crc;              /* CRC                     */
} igtl_header;

#pragma pack()

/*
 * Byte order converter for the header structre
 * 
 * igtl_header_convert_byte_order convers endianness of each
 * member variable in igtl_header structre from host byte order
 * to network byte order, or vice versa.
 *
 */
void igtl_header_convert_byte_order(igtl_header * header);

#ifdef __cplusplus
}
#endif

#endif // __IGTL_HEADER_H


