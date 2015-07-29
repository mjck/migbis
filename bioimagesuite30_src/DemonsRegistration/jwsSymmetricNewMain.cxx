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
#include "OtherFunctions.h"



using namespace jws_demons;

#include "firstExtendedSymmetricDemon.h"
#include "secondExtendedSymmetricDemon.h"
#include "thirdSymmetricDemon.h"
#include "jwsReadWriteImages.h"




// OtherFunctions.cxx
/*
// New Functions (three of them)
// And key parameters
DField*  RunRegistration1(Image* FixedBinary,Image* FixedGray,Image* MovingBinary, Image* MovingGray,int parameter1, float parameter2, int parameter 3)
DField*  RunRegistration2(Image* FixedBinary,Image* FixedGray,Image* MovingBinary, Image* MovingGray)
DField*  RunRegistration3(Image* FixedBinary,Image* FixedGray,Image* MovingBinary, Image* MovingGray)


// Rewrite main Function ...
NewMainFunction


   1. Load Images
   2. RunRegistration1 --- output should only be displacement field
   3. Save Something   --- optionally warp and save
   4. RunRegistration2
   5. SaveSomething
   6. RunRegistration3
   7. Save
   8. Done
}

See Examples in Levelset Directory

vtkpx -- is a VTK Class wrapper around ITK


Test how to convert vector images to VTK

vtkImageData* img;   Allocatee
for all pixels
img->SetPixel(itkimg->GetPixel());


Look at Levelset/vtkjwsDemonsRegistration for a boilerplate VTK Class

*/



int main( int argc, char * argv[] )
{
  if( argc < 6 )
    {
      std::cerr<< std::endl ;
      std::cerr << "Usage (all input files should .vtk format): " << std::endl ;
      std::cerr << argv[0]<<" FixedImageFile(No extension)  MovingImageFile(No extension) RegisteredImageFile(No extension) FixedGray(No extension) MovingGray(No extension) RegisteredGray(No extension)  ";   // 1 ,2, 3
      std::cerr << std::endl<< std::endl;  
      return EXIT_FAILURE;
    }


  imagePreProcessing PreClass;
  
  PreClass.loadImages( argv );
  PreClass.makeSameSizeAllInputImages();       cout<<"0.5" <<endl;
  
  OutputImageType::Pointer cleanFixedBoneImage = OutputImageType::New();
  OutputImageType::Pointer cleanFixedSkinImage = OutputImageType::New();
  OutputImageType::Pointer cleanMovingBoneImage = OutputImageType::New();
  OutputImageType::Pointer cleanMovingSkinImage = OutputImageType::New();
  
	
  	cleanFixedBoneImage->SetRegions( PreClass.region );  							  
	cleanFixedBoneImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );     
	cleanFixedBoneImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );  
	cleanFixedBoneImage->Allocate();    
	
  	cleanFixedSkinImage->SetRegions( PreClass.region );  							  
	cleanFixedSkinImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );     
	cleanFixedSkinImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );  
	cleanFixedSkinImage->Allocate();  
	
	cleanMovingBoneImage->SetRegions( PreClass.region );  							  
	cleanMovingBoneImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );     
	cleanMovingBoneImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );  
	cleanMovingBoneImage->Allocate();  
	
  	cleanMovingSkinImage->SetRegions( PreClass.region );  							  
	cleanMovingSkinImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );     
	cleanMovingSkinImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );  
	cleanMovingSkinImage->Allocate();  	
  
  PreClass.imageCleaning(cleanFixedBoneImage, cleanFixedSkinImage, cleanMovingBoneImage, cleanMovingSkinImage);             
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GrayImageType::Pointer newFixedGrayImage = GrayImageType::New();
		GrayImageType::Pointer newMovingGrayImage = GrayImageType::New();
		
		newFixedGrayImage->SetRegions( PreClass.region );
		newFixedGrayImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );
		newFixedGrayImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );
		newFixedGrayImage->Allocate();
		
		newMovingGrayImage->SetRegions( PreClass.region );
		newMovingGrayImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );
		newMovingGrayImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );
		newMovingGrayImage->Allocate();	
		
   PreClass.GetNewSizedGrayImages(newFixedGrayImage, newMovingGrayImage);
		
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
  
  FixedImageType::Pointer fixedBoneDMapImage = FixedImageType::New();
  FixedImageType::Pointer fixedSkinDMapImage = FixedImageType::New();
  MovingImageType::Pointer movingBoneDMapImage = MovingImageType::New();
  
  	fixedBoneDMapImage->SetRegions( PreClass.region );  							  
	fixedBoneDMapImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );     
	fixedBoneDMapImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );  
	fixedBoneDMapImage->Allocate(); 
	
  	fixedSkinDMapImage->SetRegions( PreClass.region );  							  
	fixedSkinDMapImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );     
	fixedSkinDMapImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );  
	fixedSkinDMapImage->Allocate(); 
	
	movingBoneDMapImage->SetRegions( PreClass.region );  							  
	movingBoneDMapImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );     
	movingBoneDMapImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );  
	movingBoneDMapImage->Allocate(); 
  // Fixed Bone
  DistanceMapGeneration( "cleanFixedBone", cleanFixedBoneImage, fixedBoneDMapImage, PreClass.region);
 
    // Fixed Skin
  DistanceMapGeneration( "cleanFixedSkin", cleanFixedSkinImage, fixedSkinDMapImage, PreClass.region);
  
  // Moving Bone
  DistanceMapGeneration( "cleanMovingBone", cleanMovingBoneImage, movingBoneDMapImage, PreClass.region);

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

     DeformationFieldType::Pointer DFieldBone = DeformationFieldType::New();     
     DFieldBone->SetRegions( PreClass.region  );
     DFieldBone->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin()  );
     DFieldBone->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );
     DFieldBone->Allocate();
     
     InternalImageType::Pointer deformedBoneImg = InternalImageType::New();
     deformedBoneImg->SetRegions( PreClass.region );
     deformedBoneImg->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin());
     deformedBoneImg->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );
     deformedBoneImg->Allocate();
     
     InputImageType::Pointer UshortDeformedSkinImg = InputImageType::New();
     UshortDeformedSkinImg ->SetRegions(PreClass.region );
     UshortDeformedSkinImg ->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );
     UshortDeformedSkinImg ->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );
     UshortDeformedSkinImg ->Allocate();		
     
     GrayImageType::Pointer firstDeformedGrayImg = GrayImageType::New();
     firstDeformedGrayImg->SetRegions( PreClass.region);
     firstDeformedGrayImg->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );
     firstDeformedGrayImg->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );
     firstDeformedGrayImg->Allocate();		
     
     ////////////////////////////////// First Extended Demon with only Bone Distance Map   ///////////////////////////////////////////////
     string boneOnlyRegistrationResult = "FirstRegResultHalf";
     FIRST_SEDEMON::firstExtendedDemonRegistration(fixedBoneDMapImage, movingBoneDMapImage, DFieldBone, boneOnlyRegistrationResult);	
	 
     myWarper(movingBoneDMapImage,  deformedBoneImg, DFieldBone );
     myWarperGray(newMovingGrayImage,  firstDeformedGrayImg, DFieldBone, "firstDeformedGray" ); 
	 
     // 1. Deform Skin(actually whole moving image) by bone-DField
     // 2. Generate distance map of the deformed skin
     myWarperUShort(cleanMovingSkinImage, UshortDeformedSkinImg , DFieldBone );	
    
     MovingImageType::Pointer movingSkinDMapImage = MovingImageType::New();
	
	movingSkinDMapImage->SetRegions( PreClass.region );  							  
	movingSkinDMapImage->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );     
	movingSkinDMapImage->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );  
	movingSkinDMapImage->Allocate(); 	 
	 // Moving Skine
    DistanceMapGeneration( "firstDeformedMovingSkin", UshortDeformedSkinImg, movingSkinDMapImage, PreClass.region);
     
	       
     ///////////////////////////////// Combine Two images for one-time registration   ///////////////////////////////////////////////////////
     InternalImageType::Pointer combinedDMapFixedImage = InternalImageType::New();
     mergeImages(fixedSkinDMapImage, fixedBoneDMapImage, combinedDMapFixedImage,"combineDMFixedImage.vtk");
     
     InternalImageType::Pointer combinedDMapFirstDeformedImage = InternalImageType::New();
     mergeImages(movingSkinDMapImage, deformedBoneImg, combinedDMapFirstDeformedImage , "combinedFirstDeformedImage.vtk");
   
/*	 
     // just for test
     string combinedDMapMovingFileName = "combinedDMMovingImage.vtk";
     InternalImageType::Pointer combinedDMapMovingImage = InternalImageType::New();
     mergeImages(movingSkinDMapImage, movingBoneDMapImage, combinedDMapMovingImage, combinedDMapMovingFileName);		
*/      	 
     /////////////////////////////////////////////////////////  Extended Demons Registration  //////////////////////////////////////////////////
     //		string registeredImageName(argv[3]);
     
     DeformationFieldType::Pointer DField2 = DeformationFieldType::New();
     DField2->SetRegions( PreClass.region );
     DField2->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );
     DField2->SetSpacing(PreClass.FixedReader->GetOutput()->GetSpacing() );
     DField2->Allocate();
     
          SECOND_SEDEMON::ExtendedDemonRegistrationMain(combinedDMapFixedImage, combinedDMapFirstDeformedImage, DField2, "SecondRegResult");	
	    DfieldToFile(DField2, "SecondRegResultHalf");
     
	 
     GrayImageType::Pointer secondDeformedGrayImg = GrayImageType::New();
     secondDeformedGrayImg->SetRegions( PreClass.region );
     secondDeformedGrayImg->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );
     secondDeformedGrayImg->SetSpacing( PreClass.FixedReader->GetOutput()->GetSpacing() );
     secondDeformedGrayImg->Allocate();		
     
     myWarperGray(firstDeformedGrayImg, secondDeformedGrayImg, DField2, "secondDeformedGray" );
#if 1

     DeformationFieldType::Pointer DField3 = DeformationFieldType::New();
     DField3->SetRegions( PreClass.region );
     DField3->SetOrigin( PreClass.FixedReader->GetOutput()->GetOrigin() );
     DField3->SetSpacing(PreClass.FixedReader->GetOutput()->GetSpacing() );
     DField3->Allocate();

          THIRD_SEDEMON::DemonRegistrationMain(newFixedGrayImage, secondDeformedGrayImg, DField3, "thirdDeformedGray");	
     
#endif	 
     return EXIT_SUCCESS;
 
}



