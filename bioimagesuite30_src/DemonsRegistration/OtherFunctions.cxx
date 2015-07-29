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
#include "tnt/tnt_array3d.h"

namespace jws_demons {

void testImageToFile2(OutputImageType::Pointer outImage)
{
	string fileName = "testImage2.vtk";
	
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( fileName );
	writer->SetInput( outImage );
	writer->Update();
}

void testFloatImageToFile(InternalImageType::Pointer outImage, string fileName)
{
	cout << "Combined image saved : "<<fileName ;
	//string fileName = "testFloatImage.vtk";
	
	typedef itk::ImageFileWriter< InternalImageType >   FloatWriterType;
	FloatWriterType::Pointer writer = FloatWriterType::New();
	writer->SetFileName( fileName );
	writer->SetInput( outImage );
	writer->Update();
	cout <<" :File Save Done !!"<<endl;
}


void DistanceMapGeneration(string dmapName, OutputImageType::Pointer image, FixedImageType::Pointer outDMapImage, InputImageType::RegionType inputRegion)
{
	
  // testImageToFile2(image);
  /////// generate Signed Distance Map  ////////////////
  cout<<"	Generate Signed Distance Map ..."<<endl;	
  DMapFilterType::Pointer DMapfilter = DMapFilterType::New();
  
  //string inputFileName = argv;
  //cout<<" input string name ="<< inputFileName<<endl;
  //string DMapFileName = inputFileName + "_DMap.vtk";
 
  
  DMapfilter->SetInput( image );
  DMapfilter->Update();

     ///////////////////// save the mrphologyBImage   /////////////////////////////////////
 InternalIteratorType  inputIt( DMapfilter->GetOutput(), inputRegion );
 InternalIteratorType  outputIt( outDMapImage, inputRegion );
 
 inputIt.GoToBegin();
 outputIt.GoToBegin();
 
 while( (!inputIt.IsAtEnd())&&(!outputIt.IsAtEnd()) )
    {                       
	outputIt.Set(inputIt.Get());
                                             
	++inputIt;
	++outputIt;
    
    }   
   /////////////////////////////////////////////////////////////////////////////////////

 if (dmapName.length()>0)
   {
     string DMapFileName = dmapName + "_DMap.vtk";	 
     FloatWriterType::Pointer floatWriter = FloatWriterType::New();
     floatWriter->SetFileName( DMapFileName );
     floatWriter->SetInput( DMapfilter->GetOutput() );
     floatWriter->Update();
   }
}


void testGrayImageToFile(GrayImageType::Pointer outImage, string InputFileName)
{
	string fileName = InputFileName + ".vtk";
	
	GrayWriterType::Pointer writer = GrayWriterType::New();
	writer->SetFileName( fileName );
	writer->SetInput( outImage );
	writer->Update();
}

void myWarperGray(GrayImageType::Pointer inputImg,  GrayImageType::Pointer deformedImg, DeformationFieldType::Pointer DField, string fileName )
{

  GrayWarperType::Pointer filter = GrayWarperType::New();

  typedef itk::LinearInterpolateImageFunction< 
                       GrayImageType, double >  InterpolatorType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  filter->SetInterpolator( interpolator );
  filter->SetOutputSpacing( DField->GetSpacing() );
  filter->SetOutputOrigin(  DField->GetOrigin() );

  filter->SetDeformationField( DField );
  filter->SetInput( inputImg );
  filter->Update();
  
  IteratorGrayType fi( filter->GetOutput() , inputImg->GetRequestedRegion().GetSize() );
  //InternalIteratorType fi( inputImg , inputImg->GetRequestedRegion().GetSize() );
  IteratorGrayType outIt( deformedImg, deformedImg->GetRequestedRegion().GetSize() );
  fi.GoToBegin();
  outIt.GoToBegin();
	
  //cout << "myWarper() : 1" <<endl;
  
  GrayPixelType t=0;
   while( (!fi.IsAtEnd())&&(!outIt.IsAtEnd()) ){
		t = fi.Get();     //cout << "2 " <<endl;
    		outIt.Set(  t );  // cout << "3 " <<endl;
    		++fi;
		++outIt;
    	}

   int sizeX, sizeY, sizeZ;
   sizeX = deformedImg->GetRequestedRegion().GetSize()[0];
   sizeY = deformedImg->GetRequestedRegion().GetSize()[1];
   sizeZ = deformedImg->GetRequestedRegion().GetSize()[2];
   
   cout<<"Deformed sizeX="<<sizeX<<" sizeY="<<sizeY <<" sizeZ="<<sizeZ<<endl;
   
   //testGrayImageToFile(filter->GetOutput(), fileName);

}

void myWarper(InternalImageType::Pointer inputImg,  InternalImageType::Pointer deformedImg, DeformationFieldType::Pointer DField )
{

  //cout << "myWarper()" <<endl;
	
  //DeformationFieldType::ConstPointer deformationField = fieldReader->GetOutput();
  typedef itk::WarpImageFilter< InternalImageType, 
                               InternalImageType, 
                                DeformationFieldType  >  FilterType;

  FilterType::Pointer filter = FilterType::New();
	
     
     ////////////////////////////////// First Extended Demon with only Bone Distance Map   ///////////////////////////////////////////////

  typedef itk::LinearInterpolateImageFunction< 
                       InternalImageType, double >  InterpolatorType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  filter->SetInterpolator( interpolator );
  filter->SetOutputSpacing( DField->GetSpacing() );
  filter->SetOutputOrigin(  DField->GetOrigin() );

  filter->SetDeformationField( DField );
  filter->SetInput( inputImg );
  filter->Update();
  
  InternalIteratorType fi( filter->GetOutput() , inputImg->GetRequestedRegion().GetSize() );
  //InternalIteratorType fi( inputImg , inputImg->GetRequestedRegion().GetSize() );
  InternalIteratorType outIt( deformedImg, deformedImg->GetRequestedRegion().GetSize() );
  fi.GoToBegin();
  outIt.GoToBegin();
	
  //cout << "myWarper() : 1" <<endl;
  
  float t=0;
   while( (!fi.IsAtEnd())&&(!outIt.IsAtEnd()) ){
		t = fi.Get();     //cout << "2 " <<endl;
    		outIt.Set(  t );  // cout << "3 " <<endl;
    		++fi;
		++outIt;
    	}

   int sizeX, sizeY, sizeZ;
   sizeX = deformedImg->GetRequestedRegion().GetSize()[0];
   sizeY = deformedImg->GetRequestedRegion().GetSize()[1];
   sizeZ = deformedImg->GetRequestedRegion().GetSize()[2];
   
   cout<<"Deformed sizeX="<<sizeX<<" sizeY="<<sizeY <<" sizeZ="<<sizeZ<<endl;
    string boneOnlyRegistrationResult = "FirstRegResultHalf";
   //testDeformedImageToFile(deformedImg);
   //testDfieldToFile(DField);
}

void myWarperUShort(InputImageType::Pointer inputImg,  InputImageType::Pointer deformedImg, DeformationFieldType::Pointer DField )
{

  //cout << "myWarper()" <<endl;
	
  //DeformationFieldType::ConstPointer deformationField = fieldReader->GetOutput();
	
  //testImageToFile(inputImg);	
  typedef itk::WarpImageFilter< InputImageType, 
                               InputImageType, 
                                DeformationFieldType  >  FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef itk::LinearInterpolateImageFunction< 
                       InputImageType, double >  InterpolatorType;

  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  filter->SetInterpolator( interpolator );
  filter->SetOutputSpacing( DField->GetSpacing() );
  filter->SetOutputOrigin(  DField->GetOrigin() );

  filter->SetDeformationField( DField );
  filter->SetInput( inputImg );
  filter->Update();
  
  IteratorType fi( filter->GetOutput() , inputImg->GetRequestedRegion().GetSize() );
  IteratorType outIt( deformedImg, deformedImg->GetRequestedRegion().GetSize() );
  fi.GoToBegin();
  outIt.GoToBegin();
	
  //cout << "myWarper() : 1" <<endl;
  
  unsigned short t=0;
   while( (!fi.IsAtEnd())&&(!outIt.IsAtEnd()) ){
		t = fi.Get();     //cout << "2 " <<endl;
    		outIt.Set(  t );  // cout << "3 " <<endl;
    		++fi;
		++outIt;
    	}

   int sizeX, sizeY, sizeZ;
   sizeX = deformedImg->GetRequestedRegion().GetSize()[0];
   sizeY = deformedImg->GetRequestedRegion().GetSize()[1];
   sizeZ = deformedImg->GetRequestedRegion().GetSize()[2];
   
   cout<<"Deformed sizeX="<<sizeX<<" sizeY="<<sizeY <<" sizeZ="<<sizeZ<<endl;
   
   //testImageToFile(deformedImg);
   //testDeformedImageToFile(deformedImg);
   //testDfieldToFile(DField);
}

void mergeImages(InternalImageType::Pointer Image1, InternalImageType::Pointer Image2, InternalImageType::Pointer CombinedImage, string fileName)
{
	InternalImageType::RegionType::SizeType  inputImage1size;
	InternalImageType::RegionType::SizeType  inputImage2size;
	inputImage1size =  Image1->GetRequestedRegion().GetSize();
	inputImage2size =  Image2->GetRequestedRegion().GetSize();
	
	int sizeX, sizeY, sizeZ;
	sizeX =inputImage1size[0];     sizeY = inputImage1size[1];    sizeZ = inputImage1size[2];
	cout<<"sizeX="<< sizeX<<" sizeY="<<sizeY<<" sizeZ="<<sizeZ<<endl;	
	
	int newSizeX = sizeX * 2;
	
	InternalImageType::RegionType::SizeType  combinedImageSize;
	combinedImageSize[0] = newSizeX;
	combinedImageSize[1] = sizeY;
	combinedImageSize[2] = sizeZ;
	cout<<"NewImage sizeX="<< combinedImageSize[0]<<" sizeY="<<combinedImageSize[1]<<" sizeZ="<<combinedImageSize[2]<<endl;	
	// I decided to connect the two images in x-direction only because of visualization reason.
	// by wook 08/02/27
	TNT::Array3D<float> Voxel1(sizeX, sizeY, sizeZ, (float)0);
	TNT::Array3D<float> Voxel2(sizeX, sizeY, sizeZ, (float)0);
	TNT::Array3D<float> newVoxel(newSizeX, sizeY, sizeZ, (float)0);
	
	InternalIteratorType  It1( Image1, inputImage1size );
	InternalIteratorType  It2( Image2, inputImage2size );

	
	int i,j,k;

	for(k=0; k < sizeZ; k++){
	for(j=0; j < sizeY; j++){
        for(i=0; i < sizeX; i++){
			
             Voxel1[i][j][k] =  It1.Get();
	     Voxel2[i][j][k] =  It2.Get();
             ++It1;  
	     ++It2;

         }
         }
         }	
	
	for(k=0; k < sizeZ; k++){
	for(j=0; j < sizeY; j++){
        for(i=0; i <newSizeX; i++){

		if(i<sizeX)      newVoxel[i][j][k] = Voxel1[i][j][k];
		else                newVoxel[i][j][k] = Voxel2[ i - sizeX ][j][k];

         }
         }
         }	
	
	InternalImageType::RegionType region; 
	InternalImageType::RegionType::IndexType indexStart;
	indexStart[0]=0;   indexStart[1]=0;  indexStart[2]=0;
	region.SetSize( combinedImageSize );
	region.SetIndex( indexStart ); 		
	
	CombinedImage->SetRegions( region );
	CombinedImage->SetOrigin(  Image1->GetOrigin() );
	CombinedImage->SetSpacing(  Image1->GetSpacing() );
	CombinedImage->Allocate();
	
	InternalIteratorType  OutIt( CombinedImage, combinedImageSize );
	
#if 1
	for(k=0; k < sizeZ; k++){
	for(j=0; j < sizeY; j++){
        for(i=0; i <newSizeX; i++){

			OutIt.Set(  newVoxel[i][j][k] );
			++OutIt;
         }
         }
         }	
	
	//testFloatImageToFile(CombinedImage, fileName);
#endif	
}

void DfieldToFile(DeformationFieldType::Pointer DImage, string fileName)
{
	FieldWriterType::Pointer fieldwriter = FieldWriterType::New();

	string outFile = fileName +"_DField.vtk";
	fieldwriter->SetFileName( outFile );
	fieldwriter->SetInput(  DImage  );
	fieldwriter->Update();
}
}

