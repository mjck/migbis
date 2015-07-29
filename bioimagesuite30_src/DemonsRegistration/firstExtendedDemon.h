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



//#define GET_NUMBER_OF_THREADS
//#define GET_INFORMATION_ON_ITERATION

//#define SET_SMALL_NUMBER_THREADS
//#define FORCED_UPDATE


namespace FIRST_EDEMON{


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
		  cout << "firstExtendedDemon.cxx " <<endl;
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
void firstExtendedDemonRegistration(FixedImageType::Pointer fixedImage, MovingImageType::Pointer movingImage, DeformationFieldType::Pointer resultingDField, string registeredImageName)
{
  std::cout <<" start first extended demon registration " <<std::endl;   

  typedef itk::jwsDemonsRegistrationFilter<
                            InternalImageType,
                            InternalImageType,
                            DeformationFieldType> RegistrationFilterType;

  std::cout <<"7: set  DemonsRegistrationFilter " <<std::endl;
  RegistrationFilterType::Pointer filter = RegistrationFilterType::New();

  filter->SetNumberOfIterations( 50 );
  filter->SetStandardDeviations( 1.0 );


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
// unsigned int nIterations[3] = { 2,2,1 };     // for test
// unsigned int nIterations[3] = { 400,400,50 };     // for test
  unsigned int nIterations[3] = { 200,200,30 };
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

 #ifdef SET_SMALL_NUMBER_THREADS
   warper->SetNumberOfThreads(1);
 #endif

     warper->Update();
     std::cout<< "warper spacing:" <<warper->GetOutput()->GetSpacing()    <<"warper origin:"<<warper->GetOutput()->GetOrigin() <<std::endl;
     std::cout<< "warper size:" <<warper->GetOutput()->GetRequestedRegion().GetSize()    <<std::endl; 
   
                                                                                                                                              
if(registeredImageName.length()>0){
	typedef float  OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
	typedef itk::CastImageFilter< 
			MovingImageType,
                        OutputImageType > CastFilterType;
	typedef itk::ImageFileWriter< OutputImageType >  WriterType;

 
	WriterType::Pointer      writer =  WriterType::New();
	CastFilterType::Pointer  caster =  CastFilterType::New();
  
	string registeredFileNamewithExtension = registeredImageName + ".vtk";
	writer->SetFileName( registeredFileNamewithExtension );
  
	caster->SetInput( warper->GetOutput() );
	writer->SetInput( caster->GetOutput() );

	writer->Update();
	observer->CloseFile();  // by wook


	FieldWriterType::Pointer fieldWriter = FieldWriterType::New();

	string deformationFieldFileNamewithExtension = registeredImageName + "_DField.vtk";
	fieldWriter->SetFileName( deformationFieldFileNamewithExtension );
	fieldWriter->SetInput( multires->GetOutput() );
	fieldWriter->Update();

}
//  return multires->GetOutput();
	DfieldIteratorType fi( multires->GetOutput(), fixedImage->GetRequestedRegion().GetSize() );
	DfieldIteratorType resultDFit( resultingDField, resultingDField->GetRequestedRegion().GetSize() );
	fi.GoToBegin();
	resultDFit.GoToBegin();
	
	while( (!fi.IsAtEnd())&&(!resultDFit.IsAtEnd()) ){
		
    		resultDFit.Set(  fi.Get() );
    		++fi;
		++resultDFit;
    	}

}
}   // namespace FIRST_EDEMON
#endif


