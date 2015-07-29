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

#include <string.h>
#include "wook.h"

#ifndef __jwsReadWriteImages__
#define __jwsReadWriteImages__



namespace jws_demons {

  class imagePreProcessing {
    
  public:
    ReaderType::Pointer FixedReader;
    WriterType::Pointer FixedWriter;
    ReaderType::Pointer MovingReader;
    WriterType::Pointer MovingWriter;
    InputImageType::RegionType::SizeType  ImageSize;
    
    OutputImageType::Pointer newFixedImage;
    OutputImageType::Pointer newMovingImage;
    
    OutputImageType::Pointer outImage;	 
    OutputImageType::RegionType region; 
    InputImageType::RegionType::IndexType indexStart;
    
    GrayReaderType::Pointer GrayFixedReader;
    GrayReaderType::Pointer GrayMovingReader;
    
    GrayImageType::Pointer newFixedGrayImage;
    GrayImageType::Pointer newMovingGrayImage;
    
    string inputFixedName, inputMovingName;
    string inputFixedGrayName, inputMovingGrayName, RegisteredGrayName;
    string FixedSkinName, FixedBoneName, MovingSkinName, MovingBoneName;
    
    DMapFilterType::Pointer FixedSkinDMapfilter;
    DMapFilterType::Pointer FixedBoneDMapfilter;		 
    
  public:
    imagePreProcessing();
    void loadImages(char * argv[]);
    InputImageType::RegionType::SizeType decideImageSize(ReaderType::Pointer reader1, ReaderType::Pointer reader2);
    void makeSameSizeAllInputImages(void);
    void prepareSameSizedNewImages(void);
    void makeSameSize(ReaderType::Pointer reader, OutputImageType::Pointer outImage, OutputImageType::RegionType::SizeType imageSize);
    void makeSameSizeGray(GrayReaderType::Pointer reader, GrayImageType::Pointer outImage, OutputImageType::RegionType::SizeType imageSize);
    // for image processing
    void imageCleaning(OutputImageType::Pointer cleanFixedBone,  OutputImageType::Pointer cleanFixedSkin, OutputImageType::Pointer cleanMovingBone, OutputImageType::Pointer cleanMovingSkin);
    void imagePreparation(InputImageType::Pointer inputImage, string &inputFileName, InputImageType::RegionType inputRegion, WriterType::Pointer writer,  OutputImageType::Pointer cleanOutImage, const InputPixelType upperTh, const InputPixelType lowerTh);
    void morphologyBinaryFiltering(InputImageType::Pointer inputImage, WriterType::Pointer writer, InputImageType::RegionType inputRegion, const InputPixelType upperTh, const InputPixelType lowerTh, string simpleThresholdImageName, ClosingFilterType::Pointer binaryClosing );
    void GetNewSizedGrayImages(GrayImageType::Pointer outFixedGrayImage, GrayImageType::Pointer outMovingGrayImage);
    
    
    // for test
    void testGrayImageToFile(GrayImageType::Pointer outImage, string InputFileName);
    
  };
};
#endif

