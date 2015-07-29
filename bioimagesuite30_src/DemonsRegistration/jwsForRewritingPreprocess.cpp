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

namespace jws_demons {

void imagePreProcessing::imageCleaning(OutputImageType::Pointer cleanFixedBone,  OutputImageType::Pointer cleanFixedSkin, OutputImageType::Pointer cleanMovingBone, OutputImageType::Pointer cleanMovingSkin)
{     
	const InputPixelType lowerThSkin = (unsigned short) 1;
	const InputPixelType upperThSkin =(unsigned short) 2 ;    
	std::cout<<"lowerThSkin= "<< lowerThSkin  <<"  upperThSkin= "<< upperThSkin <<std::endl;
     
	const InputPixelType lowerThBone = (unsigned short) 2;
	const InputPixelType upperThBone =(unsigned short) 3 ;    
	std::cout<<"lowerThBone= "<< lowerThBone  <<"  upperThBone= "<< upperThBone <<std::endl;
     
	cout<<"### FixedImage for Skin Processing Begin ###"<<endl;
	imagePreparation(newFixedImage, FixedSkinName, region, FixedWriter, cleanFixedSkin,upperThSkin, lowerThSkin);
     
	cout<<"### FixedImage for Bone Processing Begin ###"<<endl;
	imagePreparation(newFixedImage, FixedBoneName, region, FixedWriter, cleanFixedBone, upperThBone, lowerThBone);
	
	cout<<"### MovingImage for Skin Processing Begin ###"<<endl;
	imagePreparation(newMovingImage, MovingSkinName, region, MovingWriter, cleanMovingSkin,upperThSkin, lowerThSkin);
     
	cout<<"### MovingImage for Bone Processing Begin ###"<<endl;
	imagePreparation(newMovingImage, MovingBoneName, region, MovingWriter, cleanMovingBone, upperThBone, lowerThBone);	
	
}


//////////////////// for test  ///////////////////////////////////////////////////////////////
void testImageToFile1(OutputImageType::Pointer outImage)
{
	string fileName = "testImage1.vtk";
	
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( fileName );
	writer->SetInput( outImage );
	writer->Update();
}
/////////////////////////////////////////////////////////////////////////////////////////////////


void imagePreProcessing::imagePreparation(InputImageType::Pointer inputImage, string &inputFileName, InputImageType::RegionType inputRegion, WriterType::Pointer writer,  OutputImageType::Pointer cleanOutImage, const InputPixelType upperTh, const InputPixelType lowerTh)
{

  string simpleThresholdImageFileName = inputFileName + "_ThresholdImage.vtk";
  string morphologyBinaryFileName = inputFileName + "_morphologyBinaryImage.vtk";

  ClosingFilterType::Pointer binaryClosing = ClosingFilterType::New();  

  cout<<"	morphologyBinaryFiltering ..."<<endl;
  morphologyBinaryFiltering(inputImage, writer, inputRegion, upperTh, lowerTh, simpleThresholdImageFileName, binaryClosing);
  
  writer->SetFileName( morphologyBinaryFileName );
  writer->SetInput( binaryClosing->GetOutput() );
  writer->Update();

   //cleanOutImage = binaryClosing->GetOutput();
     ///////////////////// save the mrphologyBImage   /////////////////////////////////////
 IteratorType  inputIt( binaryClosing->GetOutput(), inputRegion );
 IteratorType  outputIt( cleanOutImage, inputRegion );
 
 inputIt.GoToBegin();
 outputIt.GoToBegin();
 
    while( (!inputIt.IsAtEnd())&&(!outputIt.IsAtEnd()) )
    {                       
	outputIt.Set(inputIt.Get());
                                             
	++inputIt;
	++outputIt;
    
    }   
  // testImageToFile1(cleanOutImage);
}

void imagePreProcessing::morphologyBinaryFiltering(InputImageType::Pointer inputImage, WriterType::Pointer writer, InputImageType::RegionType inputRegion, const InputPixelType upperTh, const InputPixelType lowerTh, string simpleThresholdImageName, ClosingFilterType::Pointer binaryClosing )
{
 
  ThresholdFilterType::Pointer thresholder = ThresholdFilterType::New();


  StructuringElementType  structuringElement;
  structuringElement.SetRadius( 1 );  // 3x3 structuring element
  //structuringElement.SetRadius( 3 );  // 9x9 structuring element
  //structuringElement.SetRadius( 5 );  // 9x9 structuring element
  structuringElement.CreateStructuringElement();

  // added by wook 
  binaryClosing->SetKernel( structuringElement );
  
//  thresholder->SetInput( reader->GetOutput() );
  thresholder->SetInput( inputImage );

  InputPixelType background =   0;
  InputPixelType foreground =  3056;  //255;

  thresholder->SetOutsideValue( background );
  thresholder->SetInsideValue(  foreground );

  thresholder->SetLowerThreshold( lowerTh );
  thresholder->SetUpperThreshold( upperTh );
  thresholder->Update(); // modified by wook 04/24/08
  ///////////////////////   write simpleThresholdImageFile    ///////////////////////////
  //writer->SetFileName( simpleThresholdImageName );
  //writer->SetInput( thresholder->GetOutput() );
  //writer->Update();
  //////////////////////////////////////////////////////////////////////////////////////////////////
  
  binaryClosing->SetInput( thresholder->GetOutput() );
  binaryClosing->SetForegroundValue( foreground );   // Is it correct? by wook
  //binaryClosing->Update();
  //outImage = binaryClosing->GetOutput() ;  //
}
};

