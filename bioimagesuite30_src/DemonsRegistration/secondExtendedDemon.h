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

//#include "wook.h"
#include "tnt/tnt_array3d.h"
#include <math.h>



//#define GET_NUMBER_OF_THREADS
#define GET_INFORMATION_ON_ITERATION

//#define SET_SMALL_NUMBER_THREADS
//#define FORCED_UPDATE
extern void testDMapImageToFile(TNT::Array3D<float> &DMap, int sizeX, int sizeY, int sizeZ,  FixedImageType::ConstPointer fxImage);
extern void testDfieldImageToFile(TNT::Array3D<VectorPixelType> &DField, int sizeX, int sizeY, int sizeZ,  DeformationFieldType::Pointer DImage);

extern void gaussianFilteringForVectorImage(TNT::Array3D<VectorPixelType>& vFieldCombined, int sizeX, int sizeY, int sizeZ, float variance);

namespace SECOND_EDEMON{
template <typename TRegistration>
class RegistrationInterfaceCommand : public itk::Command
{

public:
  typedef  RegistrationInterfaceCommand   Self;
  typedef  itk::Command                   Superclass;
  typedef  itk::SmartPointer<Self>        Pointer;
  itkNewMacro( Self );
protected:
  RegistrationInterfaceCommand() {};

public:
  typedef   TRegistration                              RegistrationType;
  typedef   RegistrationType *                         RegistrationPointer;

  typedef itk::Vector< float, Dimension >    VectorPixelType;
  typedef itk::Image< float, Dimension > InternalImageType;
  typedef itk::Image<  VectorPixelType, Dimension > DeformationFieldType;
  typedef itk::jwsDemonsRegistrationFilter<
                            InternalImageType,
                            InternalImageType,
                            DeformationFieldType> RegistrationFilterType;
  
  void Execute(itk::Object * object, const itk::EventObject & event)
  {

    if( !(itk::IterationEvent().CheckEvent( &event )) )
      {
      return;
      }
     

    RegistrationPointer registration =
                            dynamic_cast<RegistrationPointer>( object );
    // for MultiResolutionPDEDeformableRegistration


    RegistrationFilterType * filter = dynamic_cast< RegistrationFilterType * >(
                       registration->GetRegistrationFilter() );
       // for Demon Registration algorithm
    

    std::cout << "level:"<<registration->GetCurrentLevel() <<std::endl;
                 
    if ( registration->GetCurrentLevel() == 1 )
      {
        std::cout << "*****   level 111111" <<std::endl;
        std::cout<<"In Multilevel : "  <<  filter->GetElapsedIterations()<< std::endl;
      }
     
  }


  void Execute(const itk::Object * , const itk::EventObject & )
    { return; }
};




//  The following section of code implements a Command observer
//  that will monitor the evolution of the registration process.
//
  class CommandIterationUpdate : public itk::Command
  {
  public:
    typedef  CommandIterationUpdate   Self;
    typedef  itk::Command             Superclass;
    typedef  itk::SmartPointer<CommandIterationUpdate>  Pointer;

    double lPreviousValue;
    double lCurrentValue ;
    double lDiff;
    double lDiff_Diff;
    double lPrevDiff;
    int     nContinueCount, nIncreased;
    FILE *fp;


    itkNewMacro( CommandIterationUpdate );
  protected:
    CommandIterationUpdate() {};

    typedef itk::Image< float, Dimension > InternalImageType;
    typedef itk::Vector< float, Dimension >    VectorPixelType;
    typedef itk::Image<  VectorPixelType, Dimension > DeformationFieldType;
    typedef itk::jwsDemonsRegistrationFilter<
                                  InternalImageType,
                                  InternalImageType,
                                  DeformationFieldType> RegistrationFilterType;
	
    typedef itk::ImageRegionIterator< DeformationFieldType > FieldIterator;
    typedef itk::ImageRegionConstIterator< InternalImageType > DMapIterator;
	
    VectorPixelType displacement;
    DeformationFieldType::IndexType index;

  public:

    void InitFile(void)
    {
         //fp = fopen("Diff_data.txt","w");
         nIncreased = 0;
    }

    void CloseFile(void)
    {
        //fclose(fp);
    } 
        
  
    void Execute(itk::Object *caller, const itk::EventObject & event)
      {
       // Execute( (const itk::Object *)caller, event);
#if 1
           RegistrationFilterType * filter =
            dynamic_cast< RegistrationFilterType * >( caller );

            unsigned int unElapsedIter;  // by wook
	   DeformationFieldType::Pointer Dfield;
	   FixedImageType::ConstPointer fxImage;   // Ohhh  -> ConstPointer for const pointer
	   
          if( !(itk::IterationEvent().CheckEvent( &event )) )
            {
            return;
            }
            
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0                        // with my smoothing
		  
	 Dfield = filter->GetDeformationField();
	 
	 fxImage =filter->GetFixedImage();
//	 fxImage =filter->GetMovingImage();
	 
	 int dx, dy, dz;
	 dx = Dfield->GetRequestedRegion().GetSize()[0];
	 dy = Dfield->GetRequestedRegion().GetSize()[1];
	 dz = Dfield->GetRequestedRegion().GetSize()[2];
	 cout << "DeformableRegistration.cxx dx =" <<dx<<" dy="<<dy<<" dz="<<dz<<endl;
	 
	 int orgSizeX = dx >> 1;
	 
         TNT::Array3D<float> fixedSkinDMap( orgSizeX, dy, dz, (float)0);
	 TNT::Array3D<float> fixedBoneDMap( orgSizeX, dy, dz, (float)0);
	 TNT::Array3D<VectorPixelType> vFieldSkin( orgSizeX, dy, dz, (float)0);
	 TNT::Array3D<VectorPixelType> vFieldBone( orgSizeX, dy, dz, (float)0);
	 
	 TNT::Array3D<VectorPixelType> vFieldCombined( orgSizeX, dy, dz, (float)0);
	 TNT::Array3D<VectorPixelType> vFieldAdjusted( dx, dy, dz, (float)0);
	 
         FieldIterator fi( Dfield,  Dfield->GetRequestedRegion().GetSize() );
	 DMapIterator DMapIt( fxImage,   fxImage->GetRequestedRegion().GetSize());

  	fi.GoToBegin();   //std::cout <<"test!!!"<<endl;
	DMapIt.GoToBegin();

	int i,j,k;

	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < dx; i++){
			
		if(i<orgSizeX) {		
			
			fixedSkinDMap[i][j][k] =  DMapIt.Get();
			vFieldSkin[i][j][k] =  fi.Get();
		
		}
		else {
			fixedBoneDMap[i - orgSizeX][j][k] =  DMapIt.Get();
			vFieldBone[i - orgSizeX][j][k] =  fi.Get();
		}
	    
             ++fi;  
	     ++DMapIt;

         }
         }
         }		

	
//	if(dx == 328) {
//		testDMapImageToFile(fixedSkinDMap, orgSizeX, dy, dz, fxImage);
//		testDfieldImageToFile(vFieldSkin, orgSizeX, dy, dz, Dfield);
//        }
	
	float abs_skinDMap, abs_boneDMap, abs_sum;
	abs_skinDMap = 0.0; 	abs_boneDMap = 0.0;	abs_sum = 0.0;
	
	float skin_ratio=0.0, bone_ratio=0.0;
	float bone_no_change_value;
	
	bone_no_change_value = 0.0;
	//bone_no_change_value = 1.0;
	
	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < orgSizeX; i++){
			
			if(fixedBoneDMap[i][j][k] <= bone_no_change_value) {  // bone	
				
				    vFieldCombined[i][j][k][0] = 0.0;
                                    vFieldCombined[i][j][k][1] = 0.0;
                                    vFieldCombined[i][j][k][2] = 0.0;
				
			}
			else  {  // skin
				
				    vFieldCombined[i][j][k][0] = vFieldSkin[i][j][k][0];
                                    vFieldCombined[i][j][k][1] = vFieldSkin[i][j][k][1];
                                    vFieldCombined[i][j][k][2] = vFieldSkin[i][j][k][2];								
			}
	}
	}
        }
	
	// smoothing
	float variance = 1.0;
	gaussianFilteringForVectorImage( vFieldCombined, orgSizeX, dy, dz, variance);
	
	// make the vector of the bone(which was smoothed) zero vector
	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < orgSizeX; i++){
			
			if(fixedBoneDMap[i][j][k] <= bone_no_change_value) {  // bone	
				
				    vFieldCombined[i][j][k][0] = 0.0;
                                    vFieldCombined[i][j][k][1] = 0.0;
                                    vFieldCombined[i][j][k][2] = 0.0;				
			}
	}
	}
        }	

	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < orgSizeX; i++){
	
		vFieldAdjusted[i][j][k][0] = vFieldCombined[i][j][k][0];
		vFieldAdjusted[i+orgSizeX][j][k][0] = vFieldCombined[i][j][k][0];
			
		vFieldAdjusted[i][j][k][1] = vFieldCombined[i][j][k][1];
		vFieldAdjusted[i+orgSizeX][j][k][1] = vFieldCombined[i][j][k][1];
		
		vFieldAdjusted[i][j][k][2] = vFieldCombined[i][j][k][2];	
		vFieldAdjusted[i+orgSizeX][j][k][2] = vFieldCombined[i][j][k][2];

         }
         }
         }
	

	fi.GoToBegin(); 
	
  	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < dx; i++) {       
//		displacement = fi.Get();
//             	std::cout <<"("<<displacement[0]<<","<<displacement[1]<<","<<displacement[2]<<")   : ";
    		
		#if 1
		displacement[0] = vFieldAdjusted[i][j][k][0];
    		displacement[1] = vFieldAdjusted[i][j][k][1];
		displacement[2] = vFieldAdjusted[i][j][k][2];
		#endif
//		std::cout <<"("<<displacement[0]<<","<<displacement[1]<<","<<displacement[2]<<")"<<std::endl;
    		fi.Set( displacement );
    		++fi;
    	}
	}
	}
	
	 filter->setMyDField(Dfield);
#endif
	 
#if 1          // without my smoothing  because there is another smoothing within the Demon
	 Dfield = filter->GetDeformationField();
	 
	 fxImage =filter->GetFixedImage();
//	 fxImage =filter->GetMovingImage();
	 
	 int dx, dy, dz;
	 dx = Dfield->GetRequestedRegion().GetSize()[0];
	 dy = Dfield->GetRequestedRegion().GetSize()[1];
	 dz = Dfield->GetRequestedRegion().GetSize()[2];
	 cout << "dx =" <<dx<<" dy="<<dy<<" dz="<<dz<<endl;
	 
	 int orgSizeX = dx >> 1;
	 
         TNT::Array3D<float> fixedSkinDMap( orgSizeX, dy, dz, (float)0);
	 TNT::Array3D<float> fixedBoneDMap( orgSizeX, dy, dz, (float)0);
	 TNT::Array3D<VectorPixelType> vFieldSkin( orgSizeX, dy, dz, (float)0);
	 TNT::Array3D<VectorPixelType> vFieldBone( orgSizeX, dy, dz, (float)0);
	 
	 TNT::Array3D<VectorPixelType> vFieldCombined( orgSizeX, dy, dz, (float)0);
	 TNT::Array3D<VectorPixelType> vFieldAdjusted( dx, dy, dz, (float)0);
	 
         FieldIterator fi( Dfield,  Dfield->GetRequestedRegion().GetSize() );
	 DMapIterator DMapIt( fxImage,   fxImage->GetRequestedRegion().GetSize());

  	fi.GoToBegin();   //std::cout <<"test!!!"<<endl;
	DMapIt.GoToBegin();

	int i,j,k;

	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < dx; i++){
			
		if(i<orgSizeX) {		
			
			fixedSkinDMap[i][j][k] =  DMapIt.Get();
			vFieldSkin[i][j][k] =  fi.Get();
		
		}
		else {
			fixedBoneDMap[i - orgSizeX][j][k] =  DMapIt.Get();
			vFieldBone[i - orgSizeX][j][k] =  fi.Get();
		}
	    
             ++fi;  
	     ++DMapIt;

         }
         }
         }		

	
//	if(dx == 328) {
//		testDMapImageToFile(fixedSkinDMap, orgSizeX, dy, dz, fxImage);
//		testDfieldImageToFile(vFieldSkin, orgSizeX, dy, dz, Dfield);
//        }
	
	float abs_skinDMap, abs_boneDMap, abs_sum;
	abs_skinDMap = 0.0; 	abs_boneDMap = 0.0;	abs_sum = 0.0;
	
	float skin_ratio=0.0, bone_ratio=0.0;
	
	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < orgSizeX; i++){
			
			if((fixedSkinDMap[i][j][k]<0) && (fixedBoneDMap[i][j][k] <=0)) {  // bone	
				
				    vFieldCombined[i][j][k][0] = vFieldBone[i][j][k][0];
                                    vFieldCombined[i][j][k][1] = vFieldBone[i][j][k][1];
                                    vFieldCombined[i][j][k][2] = vFieldBone[i][j][k][2];
				
			}
			else if((fixedSkinDMap[i][j][k]>=0) && (fixedBoneDMap[i][j][k] >0)) {  // skin
				
				    vFieldCombined[i][j][k][0] = vFieldSkin[i][j][k][0];
                                    vFieldCombined[i][j][k][1] = vFieldSkin[i][j][k][1];
                                    vFieldCombined[i][j][k][2] = vFieldSkin[i][j][k][2];				
				
			}
			else if((fixedSkinDMap[i][j][k]<0) && (fixedBoneDMap[i][j][k] >0)){
				
				    abs_skinDMap 	= 	fabs(fixedSkinDMap[i][j][k]);
				    abs_boneDMap 	=	fabs(fixedBoneDMap[i][j][k]);
				    abs_sum = abs_skinDMap + abs_boneDMap;
					
				    skin_ratio = abs_boneDMap / abs_sum;
				    bone_ratio = abs_skinDMap / abs_sum;
					
//cout<<"abs_skin="<<abs_skinDMap <<" abs_bone="<<abs_boneDMap<<" abs_sum="<<abs_sum<<endl;
				    vFieldCombined[i][j][k][0] = (bone_ratio * vFieldBone[i][j][k][0]) + (skin_ratio * vFieldSkin[i][j][k][0]);
                                    vFieldCombined[i][j][k][1] =  (bone_ratio * vFieldBone[i][j][k][1]) + (skin_ratio * vFieldSkin[i][j][k][1]);
                                    vFieldCombined[i][j][k][2] =  (bone_ratio * vFieldBone[i][j][k][2]) + (skin_ratio * vFieldSkin[i][j][k][2]);			
			}
			else {  // exceptional cases
				cout<<"exceptional case at ("<<i<<", "<<j<<", "<<k<<") fixedSkinDMap="<<fixedSkinDMap[i][j][k]<<" fixedBoneDMap="<< fixedBoneDMap[i][j][k]<<endl;
				    vFieldCombined[i][j][k][0] = 0.0;
                                    vFieldCombined[i][j][k][1] = 0.0;
                                    vFieldCombined[i][j][k][2] = 0.0;				
			}
	}
	}
        }
	
	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < orgSizeX; i++){
	
		vFieldAdjusted[i][j][k][0] = vFieldCombined[i][j][k][0];
		vFieldAdjusted[i+orgSizeX][j][k][0] = vFieldCombined[i][j][k][0];
			
		vFieldAdjusted[i][j][k][1] = vFieldCombined[i][j][k][1];
		vFieldAdjusted[i+orgSizeX][j][k][1] = vFieldCombined[i][j][k][1];
		
		vFieldAdjusted[i][j][k][2] = vFieldCombined[i][j][k][2];	
		vFieldAdjusted[i+orgSizeX][j][k][2] = vFieldCombined[i][j][k][2];

         }
         }
         }
	

	fi.GoToBegin(); 
	
  	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < dx; i++) {       
//		displacement = fi.Get();
//             	std::cout <<"("<<displacement[0]<<","<<displacement[1]<<","<<displacement[2]<<")   : ";
    		
		#if 1
		displacement[0] = vFieldAdjusted[i][j][k][0];
    		displacement[1] = vFieldAdjusted[i][j][k][1];
		displacement[2] = vFieldAdjusted[i][j][k][2];
		#endif
//		std::cout <<"("<<displacement[0]<<","<<displacement[1]<<","<<displacement[2]<<")"<<std::endl;
    		fi.Set( displacement );
    		++fi;
    	}
	}
	}

	 filter->setMyDField(Dfield);
#endif
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          unElapsedIter = filter->GetElapsedIterations();
          lPreviousValue = lCurrentValue;
          lCurrentValue = filter->GetMetric();

          lPrevDiff = lDiff;
          lDiff = fabs(lPreviousValue - lCurrentValue);
          lDiff_Diff = fabs(lPrevDiff - lDiff) *100;
                   
          //fprintf(fp,"%d %f\n",unElapsedIter,lCurrentValue);
          
          
          if(lDiff < 0.0001) nContinueCount++;    // when input data is signed float (minus value for inside)
          //if(lDiff < 0.1) nContinueCount++;  // when input data is unsigned short
          else  nContinueCount = 0;

          #ifdef GET_INFORMATION_ON_ITERATION
          if(lPreviousValue > lCurrentValue){
            std::cout <<unElapsedIter<<" : prv="<<lPreviousValue<<" curr="<<lCurrentValue<< " diff= -"<<lDiff<<" nCnt="<<nContinueCount<<" nIn="<<nIncreased<<std::endl;
            nIncreased = 0;                                                                                                              
          } 
          else{
            std::cout <<unElapsedIter<<" : prv="<<lPreviousValue<<" curr="<<lCurrentValue<< " diff= +"<<lDiff<<" nCnt="<<nContinueCount<<" nIn="<<nIncreased<< std::endl;
            nIncreased++;                                                                                                                         
          } 
                                                                                                                                                                                                                                
          if(nContinueCount > 10){
                nContinueCount = 0;
                filter->StopRegistration();
                std::cout << "**************** registration Stop by continuous negligible variations     "<<std::endl;                                
          }

          if(nIncreased > 20){
                nIncreased = 0;
                filter->StopRegistration();
                std::cout << "**************** registration Stop by continumes metric increasement      "<<std::endl;                              
          }
          
                        
          #endif
                                                        
          #if 0  
          
          std::cout << unElapsedIter << " " << filter->GetMetric() << std::endl;

          if(unElapsedIter ==20){ std::cout << "********************************  count!!"<<std::endl;
              filter->StopRegistration();
          }
          #endif
#endif
      }

    void Execute(const itk::Object * object, const itk::EventObject & event)
      {
	}
  };


#if  1
void ExtendedDemonRegistrationMain(FixedImageType::Pointer fixedImage, MovingImageType::Pointer movingImage, DeformationFieldType::Pointer resultingHalfDField, string registeredImageName)
{
  std::cout <<" start second registration!! " <<std::endl;   
  std::cout <<"1: accept parameter  " <<std::endl;
		
#if 0
  typedef itk::HistogramMatchingImageFilter<                      
                                    InternalImageType,
                                    InternalImageType >   MatchingFilterType;
  MatchingFilterType::Pointer matcher = MatchingFilterType::New();

  std::cout <<"2: set HistogramMatchingImageFilter  " <<std::endl;
  matcher->SetInput( movingImage );       
  matcher->SetReferenceImage( fixedImage );
  matcher->SetNumberOfHistogramLevels( 1024 );
  matcher->SetNumberOfMatchPoints( 7 );
  matcher->ThresholdAtMeanIntensityOn();

   #ifdef SET_SMALL_NUMBER_THREADS
     matcher->SetNumberOfThreads(1);
   #endif

   #ifdef FORCED_UPDATE
     matcher->Update();
   #endif

  #ifdef GET_NUMBER_OF_THREADS

    std::cout <<" Number of Threads for HistogramMatchingImageFilter: "<<matcher->GetNumberOfThreads() << std::endl;
                                  
  #endif
#endif
  std::cout <<"6: generate DeformationFieldType format  " <<std::endl;

  typedef itk::jwsDemonsRegistrationFilter<
                            InternalImageType,
                            InternalImageType,
                            DeformationFieldType> RegistrationFilterType;

  std::cout <<"7: set  DemonsRegistrationFilter " <<std::endl;
  RegistrationFilterType::Pointer filter = RegistrationFilterType::New();

  filter->SetNumberOfIterations( 50 );
  filter->SetStandardDeviations( 1.0 );
  //filter->SetStandardDeviations( 0.0 );


     #ifdef SET_SMALL_NUMBER_THREADS
       filter->SetNumberOfThreads(1);
     #endif

  
  #ifdef GET_NUMBER_OF_THREADS

    std::cout <<" Number of Threads for DemonsRegistrationFilter: "<<filter->GetNumberOfThreads() << std::endl;
                                  
  #endif
  /////////////////////////////////////////////////////////////////////////////
  std::cout <<"8: set  CommandIterationUpdate " <<std::endl;
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  observer->InitFile();

  std::cout <<"9: filter->AddObserver " <<std::endl;
  filter->AddObserver( itk::IterationEvent(), observer );

     
  /////////////////////////////////////////////////////////////////////////////

  typedef itk::jwsMultiResolutionPDEDeformableRegistration<
                                InternalImageType,
                                InternalImageType,
                                DeformationFieldType >   MultiResRegistrationFilterType;

  std::cout <<"10: set MultiResolutionPDEDeformableRegistration " <<std::endl;
  MultiResRegistrationFilterType::Pointer multires = MultiResRegistrationFilterType::New();
  multires->SetRegistrationFilter( filter );
  
  int numberOfLevels = 3;
  //unsigned int nIterations[3] = { 2,2,1 };     // for test
//  unsigned int nIterations[3] = { 10,10, 4 };     // for test
unsigned int nIterations[3] = { 200,200, 30 };     // for test
 
  multires->SetNumberOfLevels( numberOfLevels ); 
  multires->SetFixedImage( fixedImage );
  //multires->SetMovingImage( matcher->GetOutput() );   // turn off the histogram matching for distance mapping demon
                                                                               // by wook 02/28/08
  multires->SetMovingImage( movingImage );

  #ifdef SET_SMALL_NUMBER_THREADS
    multires->SetNumberOfThreads(1);
  #endif
 
  multires->SetNumberOfIterations( nIterations );  


   /******************  by wook    ***************************/
   std::cout <<"11: set multires->AddObserver " <<std::endl;
   typedef RegistrationInterfaceCommand<MultiResRegistrationFilterType> CommandType;
   CommandType::Pointer command = CommandType::New();
   multires->AddObserver( itk::IterationEvent(), command );
//   multires->Update();
   /*****************************************************************/


 #ifdef FORCED_UPDATE
   multires->Update();
 #endif


#ifdef GET_NUMBER_OF_THREADS

  std::cout <<" Number of Threads for MultiResolutionPDEDeformableRegistration: "<<multires->GetNumberOfThreads() << std::endl;

#endif
                

  std::cout <<"12: WarpImageFilter  &  LinearInterpolateImageFunction" <<std::endl;

  typedef itk::LinearInterpolateImageFunction<
                                   MovingImageType,
                                   double          >  InterpolatorType;
  WarperType::Pointer warper = WarperType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
//  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

  std::cout <<"13: initialize WarpImageFilter  &  LinearInterpolateImageFunction" <<std::endl;

  warper->SetInput( movingImage );
  warper->SetInterpolator( interpolator );


  warper->SetOutputSpacing( fixedImage->GetSpacing() );
  warper->SetOutputOrigin( fixedImage->GetOrigin() );
  warper->SetDeformationField( multires->GetOutput() );

  // by wook to set the large value as outside pixel value
  warper->SetEdgePaddingValue(65535L);
     warper->Update();

 if(registeredImageName.length()>0){
	 typedef float  OutputPixelType;
	 typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
	 typedef itk::CastImageFilter< 
			 MovingImageType,
			 OutputImageType > CastFilterType;
  
	  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  
	  WriterType::Pointer      writer =  WriterType::New();
	  CastFilterType::Pointer  caster =  CastFilterType::New();

	  string boneRegisteredImageFileName = registeredImageName + "Both";
	  string skinRegisteredImageFileName = registeredImageName + "Skin";
		
	  string boneRegisteredFileNamewithExtension =boneRegisteredImageFileName + ".vtk";
	  writer->SetFileName( boneRegisteredFileNamewithExtension );
  
	  caster->SetInput( warper->GetOutput() );
	  writer->SetInput( caster->GetOutput() );
	  writer->Update();
  

	  FieldWriterType::Pointer fieldWriter = FieldWriterType::New();

	  string deformationFieldFileNamewithExtension = registeredImageName + "_DField.vtk";
	  fieldWriter->SetFileName( deformationFieldFileNamewithExtension );
	  fieldWriter->SetInput( multires->GetOutput() );
	  fieldWriter->Update();

   }
  std::cout <<"16: observer->CloseFile()" <<std::endl;
  observer->CloseFile();  // by wook



	////////////////////// store half DField  /////////////////////////////////////////
  	 int dx, dy, dz;
	 dx = multires->GetOutput()->GetRequestedRegion().GetSize()[0];
	 dy = multires->GetOutput()->GetRequestedRegion().GetSize()[1];
	 dz = multires->GetOutput()->GetRequestedRegion().GetSize()[2];
	 
	 int halfSizeX = dx >>1; cout<<" halfSizeX= "<<halfSizeX<<endl;
	//TNT::Array3D<VectorPixelType> vField( dx, dy, dz, (float)0);
	DfieldIteratorType fieldIt( multires->GetOutput(), multires->GetOutput()->GetRequestedRegion().GetSize() );
	DfieldIteratorType fieldOut( resultingHalfDField, resultingHalfDField->GetRequestedRegion().GetSize() );
	
	cout<< "size of From SecondDField = "<<multires->GetOutput()->GetRequestedRegion().GetSize()<<endl;
	cout<<"size of To SecondDField=" <<resultingHalfDField->GetRequestedRegion().GetSize()<<endl;
	
	fieldIt.GoToBegin();
	fieldOut.GoToBegin();
	
	int i,j,k;

	for(k=0; k < dz; k++){
	for(j=0; j < dy; j++){
        for(i=0; i < dx; i++){
			
		if(i<halfSizeX) {		
	
			fieldOut.Set(  fieldIt.Get() );
			++fieldOut;
		}
	        ++fieldIt;

         }
         }
         }
	
//  return 0;
}
}   //   namespace SECOND_EDEMON
#endif


