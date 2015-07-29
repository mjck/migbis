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
#include "jwsReadWriteImages.h"
using namespace jws_demons;

namespace jws_demons {

imagePreProcessing::imagePreProcessing()
{
	FixedReader = ReaderType::New();
	FixedWriter = WriterType::New();
        MovingReader = ReaderType::New();
	MovingWriter = WriterType::New();
	
	newFixedImage = OutputImageType::New();
	newMovingImage = OutputImageType::New();
	outImage = OutputImageType::New();	
	
	newFixedGrayImage = GrayImageType::New();
        newMovingGrayImage = GrayImageType::New();	
	
}

void imagePreProcessing::makeSameSizeAllInputImages(void)
{
	ImageSize = decideImageSize( FixedReader, MovingReader);
	std::cout<<"[ForRewriting Class:: makeSameSizeAllInputImages()]  Internal image sizeX="<< ImageSize[0]<<" sizeY="<<ImageSize[1]<<" sizeZ="<<ImageSize[2]<<std::endl;
	
	prepareSameSizedNewImages();
	makeSameSize(FixedReader, newFixedImage, ImageSize);
        makeSameSize(MovingReader, newMovingImage, ImageSize);
	makeSameSizeGray(GrayFixedReader, newFixedGrayImage, ImageSize);
	makeSameSizeGray(GrayMovingReader, newMovingGrayImage, ImageSize);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//   for test

	testGrayImageToFile(newFixedGrayImage, inputFixedGrayName);
	testGrayImageToFile(newMovingGrayImage, inputMovingGrayName);
}

void imagePreProcessing::prepareSameSizedNewImages(void)
{
	 indexStart[0]=0;   indexStart[1]=0;  indexStart[2]=0;
	 region.SetSize( ImageSize );
	 region.SetIndex( indexStart ); 	
	
	 newFixedImage->SetRegions( region );
	 newFixedImage->SetOrigin( FixedReader->GetOutput()->GetOrigin() );
	 newFixedImage->SetSpacing( FixedReader->GetOutput()->GetSpacing() );
	 newFixedImage->Allocate();
     
	 newMovingImage->SetRegions( region );
	 newMovingImage->SetOrigin( FixedReader->GetOutput()->GetOrigin() );
	 newMovingImage->SetSpacing( FixedReader->GetOutput()->GetSpacing() );
	 newMovingImage->Allocate();
     
	 outImage->SetRegions( region );
	 outImage->SetOrigin( FixedReader->GetOutput()->GetOrigin() );
	 outImage->SetSpacing( FixedReader->GetOutput()->GetSpacing() );
	 outImage->Allocate();	 	
	 
	 newFixedGrayImage->SetRegions( region );
	 newFixedGrayImage->SetOrigin( FixedReader->GetOutput()->GetOrigin() );
	 newFixedGrayImage->SetSpacing( FixedReader->GetOutput()->GetSpacing() );
	 newFixedGrayImage->Allocate();
     
	 newMovingGrayImage->SetRegions( region );
	 newMovingGrayImage->SetOrigin( FixedReader->GetOutput()->GetOrigin() );
	 newMovingGrayImage->SetSpacing( FixedReader->GetOutput()->GetSpacing() );
	 newMovingGrayImage->Allocate();		 
}


void imagePreProcessing::loadImages(char * argv[])
{
	  inputFixedName = argv[1];
	  inputMovingName = argv[2]; 
	  FixedSkinName = inputFixedName + "SKIN";
	  FixedBoneName = inputFixedName + "BONE";
	  MovingSkinName = inputMovingName + "SKIN";
	  MovingBoneName = inputMovingName + "BONE";

	  inputFixedGrayName = argv[4];
	  inputMovingGrayName = argv[5];
	  RegisteredGrayName = argv[6];

     // Read Input Files
     // Binary classification image 0,1,2 -- unsigned short 
	  string inputFixedFileName = inputFixedName + ".vtk";
	  FixedReader->SetFileName( inputFixedFileName );  FixedReader->Update();
     
	  string inputMovingFileName = inputMovingName + ".vtk";    
	  MovingReader->SetFileName( inputMovingFileName );   MovingReader->Update();  
	 
	 GrayFixedReader = GrayReaderType::New();
	 GrayMovingReader = GrayReaderType::New();
     
     // Read Gray Level Files
     // Gray Level Images -- short
	 string inputFixedGrayFileName = inputFixedGrayName + ".vtk";
	 GrayFixedReader->SetFileName( inputFixedGrayFileName );  GrayFixedReader->Update();		
	 string inputMovingGrayFileName = inputMovingGrayName + ".vtk";
	 GrayMovingReader->SetFileName( inputMovingGrayFileName );  GrayMovingReader->Update();
     // End of Read 
	 	 	 
}

// Because of the 3 Layered-multiresolution implementation of registration, Image size should be adjusted to the multiple of 4.
InputImageType::RegionType::SizeType imagePreProcessing::decideImageSize(ReaderType::Pointer reader1, ReaderType::Pointer reader2)
{
		InputImageType::RegionType::SizeType  Image1size;
		Image1size =  reader1->GetOutput()->GetRequestedRegion().GetSize();
		
		InputImageType::RegionType::SizeType  Image2size;
		Image2size =  reader2->GetOutput()->GetRequestedRegion().GetSize();
		
		int sizeX1, sizeY1, sizeZ1;
		int sizeX2, sizeY2, sizeZ2;
		sizeX1 =Image1size[0];     sizeY1 = Image1size[1];    sizeZ1 = Image1size[2];
		sizeX2 =Image2size[0];     sizeY2 = Image2size[1];    sizeZ2 = Image2size[2];
		cout<<"Input Fixed image sizeX="<< sizeX1<<" sizeY="<<sizeY1<<" sizeZ="<<sizeZ1<<endl;
		cout<<"Input Moving image sizeX="<< sizeX2<<" sizeY="<<sizeY2 <<" sizeZ="<<sizeZ2<<endl;
		
		int tempX, tempY, tempZ;
		
		InputImageType::RegionType::SizeType InternalImageSize;
		if(sizeX1 >= sizeX2) tempX = sizeX1;
		else tempX = sizeX2;
		
		if(sizeY1 >= sizeY2) tempY = sizeY1;
		else tempY = sizeY2;
		
		if(sizeZ1 >= sizeZ2) tempZ = sizeZ1;
		else tempZ = sizeZ2;
		
		int adjustX, adjustY, adjustZ;
		
		//adjustX =  ((tempX+2)>>2)<<2;	adjustY=  ((tempY+2)>>2)<<2;	adjustZ =  ((tempZ+2)>>2)<<2;
		adjustX =  ((tempX+3)>>2)<<2;	adjustY=  ((tempY+3)>>2)<<2;	adjustZ =  ((tempZ+3)>>2)<<2;
		cout<<"Adjusted image sizeX="<<adjustX<<" sizeY="<<adjustY <<" sizeZ="<<adjustZ<<endl;
		
		InternalImageSize[0] = adjustX;
		InternalImageSize[1] = adjustY;
		InternalImageSize[2] = adjustZ;
		
		return InternalImageSize;
}

void imagePreProcessing::makeSameSize(ReaderType::Pointer reader, OutputImageType::Pointer outImage, OutputImageType::RegionType::SizeType imageSize)
{
	InputImageType::RegionType::SizeType  inputImagesize;
	inputImagesize =  reader->GetOutput()->GetRequestedRegion().GetSize();	
	//cout<<"input image sizeX="<< inputImagesize[0]<<" sizeY="<<inputImagesize[1]<<" sizeZ="<<inputImagesize[2]<<endl;
					  
	IteratorType  inputIt( reader->GetOutput(), inputImagesize );
	OutImageIteratorType  outIt( outImage, outImage->GetBufferedRegion() );
	
	int i,j,k;
        int newSizeX, newSizeY, newSizeZ;
	newSizeX = imageSize[0];	newSizeY = imageSize[1];	newSizeZ = imageSize[2];
	
	int oldSizeX, oldSizeY, oldSizeZ;
	oldSizeX = inputImagesize[0];	oldSizeY = inputImagesize[1];	oldSizeZ = inputImagesize[2];
	
	for(k=0; k < newSizeZ; k++){
	for(j=0; j < newSizeY; j++){
        for(i=0; i < newSizeX; i++){

		if((i<oldSizeX) && (j<oldSizeY) && (k<oldSizeZ)){
			 outIt.Set(inputIt.Get());   
			  ++inputIt;
		 }
		else {
			outIt.Set((unsigned short)0);
		}
            
	     ++outIt;

        }
        }
        }	
	
//  for test
	//testImageToFile(outImage);
}

void imagePreProcessing::GetNewSizedGrayImages(GrayImageType::Pointer outFixedGrayImage, GrayImageType::Pointer outMovingGrayImage)
{
	IteratorGrayType fi( newFixedGrayImage , ImageSize );
	IteratorGrayType outIt( outFixedGrayImage, ImageSize );
  
	fi.GoToBegin();
	outIt.GoToBegin();
  
	GrayPixelType t=0;
	while( (!fi.IsAtEnd())&&(!outIt.IsAtEnd()) ){
		t = fi.Get();     //cout << "2 " <<endl;
    		outIt.Set(  t );  // cout << "3 " <<endl;
    		++fi;
		++outIt;
    	}
	
	IteratorGrayType fi2( newMovingGrayImage , ImageSize );
	IteratorGrayType outIt2( outMovingGrayImage, ImageSize );
  
	fi2.GoToBegin();
	outIt2.GoToBegin();
  
	while( (!fi2.IsAtEnd())&&(!outIt2.IsAtEnd()) ){
		t = fi2.Get();     //cout << "2 " <<endl;
    		outIt2.Set(  t );  // cout << "3 " <<endl;
    		++fi2;
		++outIt2;
    	}	
}

void imagePreProcessing::makeSameSizeGray(GrayReaderType::Pointer reader, GrayImageType::Pointer outImage, OutputImageType::RegionType::SizeType imageSize)
{
	GrayImageType::RegionType::SizeType  inputImagesize;
	inputImagesize =  reader->GetOutput()->GetRequestedRegion().GetSize();	
	cout<<"Gray image sizeX="<< inputImagesize[0]<<" sizeY="<<inputImagesize[1]<<" sizeZ="<<inputImagesize[2]<<endl;
					  
	IteratorGrayType  inputIt( reader->GetOutput(), inputImagesize );
	OutImageIteratorGrayType  outIt( outImage, outImage->GetBufferedRegion() );
	
	int i,j,k;
        int newSizeX, newSizeY, newSizeZ;
	newSizeX = imageSize[0];	newSizeY = imageSize[1];	newSizeZ = imageSize[2];
	
	int oldSizeX, oldSizeY, oldSizeZ;
	oldSizeX = inputImagesize[0];	oldSizeY = inputImagesize[1];	oldSizeZ = inputImagesize[2];
	
	for(k=0; k < newSizeZ; k++){
	for(j=0; j < newSizeY; j++){
        for(i=0; i < newSizeX; i++){

		if((i<oldSizeX) && (j<oldSizeY) && (k<oldSizeZ)){
			 outIt.Set(inputIt.Get());   
			  ++inputIt;
		 }
		else {
			outIt.Set((GrayPixelType)0);
		}
            
	     ++outIt;

        }
        }
        }	
	
//  for test
	//testImageToFile(outImage);
}
	
};

