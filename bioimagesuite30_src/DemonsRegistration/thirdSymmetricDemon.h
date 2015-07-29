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

namespace THIRD_SEDEMON{
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
  typedef itk::Image< GrayPixelType, Dimension > GrayImageType;
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

    typedef itk::Image< GrayPixelType, Dimension > GrayImageType;
    typedef itk::Vector< float, Dimension >    VectorPixelType;
    typedef itk::Image<  VectorPixelType, Dimension > DeformationFieldType;
    typedef itk::jwsDemonsRegistrationFilter<
                                  InternalImageType,
                                  InternalImageType,
                                  DeformationFieldType> RegistrationFilterType;
	
    typedef itk::ImageRegionIterator< DeformationFieldType > FieldIterator;
    //typedef itk::ImageRegionConstIterator< GrayImageType > DMapIterator;
	
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
                                                        

#endif
      }

    void Execute(const itk::Object * object, const itk::EventObject & event)
      {
	}
  };


#if  1
void DemonRegistrationMain(GrayImageType::Pointer fixedImage, GrayImageType::Pointer movingImage, DeformationFieldType::Pointer resultingDField, string registeredImageName)
{
  std::cout <<" start Third registration!! " <<std::endl;   
  std::cout <<"1: accept parameter  " <<std::endl;
  
  typedef itk::CastImageFilter<GrayImageType,  
                                InternalImageType > FixedImageCasterType; 
  typedef itk::CastImageFilter< GrayImageType,  
                                InternalImageType > MovingImageCasterType;
  
  FixedImageCasterType::Pointer fixedImageCaster   = FixedImageCasterType::New(); 
  MovingImageCasterType::Pointer movingImageCaster = MovingImageCasterType::New();
   
  fixedImageCaster->SetInput( fixedImage ); 
  movingImageCaster->SetInput( movingImage ); 
  
#if 1
  typedef itk::HistogramMatchingImageFilter<                      
                                    InternalImageType,
                                    InternalImageType >   MatchingFilterType;
  MatchingFilterType::Pointer matcher = MatchingFilterType::New();

  std::cout <<"2: set HistogramMatchingImageFilter  " <<std::endl;
  matcher->SetInput( movingImageCaster->GetOutput()  );       
  matcher->SetReferenceImage( fixedImageCaster->GetOutput() );
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
  filter->SetStandardDeviations( 1.0 );   // good
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
                                DeformationFieldType >   GrayMultiResRegistrationFilterType;

  std::cout <<"10: set MultiResolutionPDEDeformableRegistration " <<std::endl;
  GrayMultiResRegistrationFilterType::Pointer Gmultires = GrayMultiResRegistrationFilterType::New();
  Gmultires->SetRegistrationFilter( filter );
  
  int numberOfLevels = 3;
//  unsigned int nIterations[3] = { 10,10,3 };     // for test
//unsigned int nIterations[3] = { 400,400, 400 };     // for test
unsigned int nIterations[3] = { 400,400, 200 };     // good
 
  Gmultires->SetNumberOfLevels( numberOfLevels ); 
  Gmultires->SetFixedImage( fixedImageCaster->GetOutput() );
  Gmultires->SetMovingImage( matcher->GetOutput() );   // turn off the histogram matching for distance mapping demon
                                                                               // by wook 02/28/08
  //Gmultires->SetMovingImage( movingImage );

  #ifdef SET_SMALL_NUMBER_THREADS
    Gmultires->SetNumberOfThreads(1);
  #endif
 
  Gmultires->SetNumberOfIterations( nIterations );  


   /******************  by wook    ***************************/
   std::cout <<"11: set multires->AddObserver " <<std::endl;
   typedef RegistrationInterfaceCommand<GrayMultiResRegistrationFilterType> CommandType;
   CommandType::Pointer command = CommandType::New();
   Gmultires->AddObserver( itk::IterationEvent(), command );
//   multires->Update();
   /*****************************************************************/


 #ifdef FORCED_UPDATE
   Gmultires->Update();
 #endif


#ifdef GET_NUMBER_OF_THREADS

  std::cout <<" Number of Threads for MultiResolutionPDEDeformableRegistration: "<<multires->GetNumberOfThreads() << std::endl;

#endif
                

  std::cout <<"12: WarpImageFilter  &  LinearInterpolateImageFunction" <<std::endl;

  typedef itk::LinearInterpolateImageFunction<
                                   GrayImageType,
                                   double          >  InterpolatorType;
  GrayWarperType::Pointer warper = GrayWarperType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
//  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

  std::cout <<"13: initialize WarpImageFilter  &  LinearInterpolateImageFunction" <<std::endl;

  warper->SetInput( movingImage );
  warper->SetInterpolator( interpolator );


  warper->SetOutputSpacing( fixedImage->GetSpacing() );
  warper->SetOutputOrigin( fixedImage->GetOrigin() );
  warper->SetDeformationField( Gmultires->GetOutput() );

  // by wook to set the large value as outside pixel value
  warper->SetEdgePaddingValue(65535L);

 #ifdef SET_SMALL_NUMBER_THREADS
   warper->SetNumberOfThreads(1);
 #endif

   //#ifdef FORCED_UPDATE
     warper->Update();
     std::cout<< "warper spacing:" <<warper->GetOutput()->GetSpacing()    <<"warper origin:"<<warper->GetOutput()->GetOrigin() <<std::endl;
     std::cout<< "warper size:" <<warper->GetOutput()->GetRequestedRegion().GetSize()    <<std::endl; 
   //#endif


               
   if(registeredImageName.length()>0){
	   typedef itk::ImageFileWriter< GrayImageType >  WriterType;
	   WriterType::Pointer      writer =  WriterType::New();
  		
	   string registeredFileNamewithExtension =registeredImageName + ".vtk";
	   writer->SetFileName( registeredFileNamewithExtension );
	   writer->SetInput( warper->GetOutput()  );

           #ifdef SET_SMALL_NUMBER_THREADS
	   writer->SetNumberOfThreads(1);
           #endif
 	  
	   writer->Update();
	   
	   FieldWriterType::Pointer fieldWriter = FieldWriterType::New();

	   string deformationFieldFileNamewithExtension = registeredImageName + "_DField.vtk";
	   fieldWriter->SetFileName( deformationFieldFileNamewithExtension );
	   fieldWriter->SetInput( Gmultires->GetOutput() );
	   fieldWriter->Update();
        }
   
   observer->CloseFile();  // by wook

	////////////////////// store half DField  /////////////////////////////////////////
  	 int ddx, ddy, ddz;
	 ddx = Gmultires->GetOutput()->GetRequestedRegion().GetSize()[0];
	 ddy = Gmultires->GetOutput()->GetRequestedRegion().GetSize()[1];
	 ddz = Gmultires->GetOutput()->GetRequestedRegion().GetSize()[2];
	 

	//TNT::Array3D<VectorPixelType> vField( dx, dy, dz, (float)0);
	DfieldIteratorType fieldIt( Gmultires->GetOutput(), Gmultires->GetOutput()->GetRequestedRegion().GetSize() );
	DfieldIteratorType fieldOut( resultingDField, resultingDField->GetRequestedRegion().GetSize() );
	
	fieldIt.GoToBegin();
	fieldOut.GoToBegin();
	
	int i,j,k;

	for(k=0; k < ddz; k++){
	  for(j=0; j < ddy; j++){
	    for(i=0; i < ddx; i++){
	      fieldOut.Set(  fieldIt.Get() );
	      ++fieldOut;	
	      ++fieldIt;
	      
	    }
	  }
         }
}
}   //   namespace THIRD_EDEMON
#endif


