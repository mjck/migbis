// tcl wrapper for vtkpxLinearMotionCorrect object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxLinearMotionCorrect.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxLinearMotionCorrectNewCommand()
{
  vtkpxLinearMotionCorrect *temp = vtkpxLinearMotionCorrect::New();
  return static_cast<ClientData>(temp);
}

int vtkpxLinearRegistrationCppCommand(vtkpxLinearRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxLinearMotionCorrectCppCommand(vtkpxLinearMotionCorrect *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxLinearMotionCorrectCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxLinearMotionCorrectCppCommand(static_cast<vtkpxLinearMotionCorrect *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxLinearMotionCorrectCppCommand(vtkpxLinearMotionCorrect *op, Tcl_Interp *interp,
             int argc, char *argv[])
{
  int    tempi = 0;      (void)tempi;
  double tempd = 0.0;    (void)tempd;
  static char temps[80]; (void)temps;
  int    error = 0;      (void)error;
  temps[0] = 0;

  if (argc < 2)
    {
    Tcl_SetResult(interp,const_cast<char *>("Could not find requested method."), TCL_VOLATILE);
    return TCL_ERROR;
    }
  if (!interp)
    {
    if (!strcmp("DoTypecasting",argv[0]))
      {
      if (!strcmp("vtkpxLinearMotionCorrect",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxLinearRegistrationCppCommand(static_cast<vtkpxLinearRegistration *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxLinearRegistration"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxLinearMotionCorrect  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearMotionCorrect");
    return TCL_OK;
    }
  if ((!strcmp("GetClassName",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetClassName();
    if (temp20)
      {
      Tcl_SetResult(interp, const_cast<char *>(temp20), TCL_VOLATILE);
      }
    else
      {
      Tcl_ResetResult(interp);
      }
    return TCL_OK;
    }
  if ((!strcmp("IsA",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->IsA(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("NewInstance",argv[1]))&&(argc == 2))
    {
    vtkpxLinearMotionCorrect  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearMotionCorrect");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxLinearMotionCorrect  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearMotionCorrect");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRegistrationParameters",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetRegistrationParameters();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("GetSimilarityResults",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetSimilarityResults();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("GetInitialParameters",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetInitialParameters();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialParameters",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->SetInitialParameters(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetReferenceMotionImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetReferenceMotionImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetReferenceMotionImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetReferenceMotionImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetMotionSequence",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetMotionSequence(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMotionSequence",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetMotionSequence();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetLinearRegistration",argv[1]))&&(argc == 2))
    {
    vtkpxLinearRegistration  *temp20;
    temp20 = (op)->GetLinearRegistration();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SetBeginFrame",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetBeginFrame(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBeginFrame",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBeginFrame();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetEndFrame",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetEndFrame();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetEndFrame",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetEndFrame(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetReferenceFrame",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetReferenceFrame();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetReferenceFrame",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetReferenceFrame(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetUseLastFrameForInitialization",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseLastFrameForInitialization(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseLastFrameForInitializationMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseLastFrameForInitializationMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseLastFrameForInitializationMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseLastFrameForInitializationMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseLastFrameForInitialization",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseLastFrameForInitialization();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseLastFrameForInitializationOn",argv[1]))&&(argc == 2))
    {
    op->UseLastFrameForInitializationOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseLastFrameForInitializationOff",argv[1]))&&(argc == 2))
    {
    op->UseLastFrameForInitializationOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("MotionCorrectSequence",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkDoubleArray  *temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->MotionCorrectSequence(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("MotionCorrectSequenceMask",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkDoubleArray  *temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->MotionCorrectSequenceMask(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("MotionCorrectSequence",argv[1]))&&(argc == 10))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkDoubleArray  *temp3;
    int      temp4;
    int      temp5;
    short    temp6;
    short    temp7;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (Tcl_GetInt(interp,argv[9],&tempi) != TCL_OK) error = 1;
    temp7 = tempi;
    if (!error)
    {
    temp20 = (op)->MotionCorrectSequence(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SaveMotionParameters",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->SaveMotionParameters(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SaveMotionParameters",argv[1]))&&(argc == 5))
    {
    vtkDoubleArray  *temp0;
    char    *temp1;
    int      temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    temp1 = argv[3];
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->SaveMotionParameters(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("LoadMotionParameters",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->LoadMotionParameters(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("InitializeMotionParameters",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    int      temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->InitializeMotionParameters(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Run",argv[1]))&&(argc == 2))
    {
    op->Run();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ComputeAllSimilarityMetricsBetweenImages",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    int      temp1;
    vtkImageData  *temp2;
    vtkDoubleArray  *temp3;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->ComputeAllSimilarityMetricsBetweenImages(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxLinearMotionCorrectCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxLinearRegistrationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxLinearMotionCorrect:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRegistrationParameters\n",NULL);
    Tcl_AppendResult(interp,"  GetSimilarityResults\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialParameters\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetReferenceMotionImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetReferenceMotionImage\n",NULL);
    Tcl_AppendResult(interp,"  SetMotionSequence\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMotionSequence\n",NULL);
    Tcl_AppendResult(interp,"  GetLinearRegistration\n",NULL);
    Tcl_AppendResult(interp,"  SetBeginFrame\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBeginFrame\n",NULL);
    Tcl_AppendResult(interp,"  GetEndFrame\n",NULL);
    Tcl_AppendResult(interp,"  SetEndFrame\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetReferenceFrame\n",NULL);
    Tcl_AppendResult(interp,"  SetReferenceFrame\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetUseLastFrameForInitialization\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseLastFrameForInitializationMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseLastFrameForInitializationMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseLastFrameForInitialization\n",NULL);
    Tcl_AppendResult(interp,"  UseLastFrameForInitializationOn\n",NULL);
    Tcl_AppendResult(interp,"  UseLastFrameForInitializationOff\n",NULL);
    Tcl_AppendResult(interp,"  MotionCorrectSequence\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  MotionCorrectSequenceMask\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  MotionCorrectSequence\t with 8 args\n",NULL);
    Tcl_AppendResult(interp,"  SaveMotionParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SaveMotionParameters\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  LoadMotionParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  InitializeMotionParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Run\n",NULL);
    Tcl_AppendResult(interp,"  ComputeAllSimilarityMetricsBetweenImages\t with 4 args\n",NULL);
    return TCL_OK;
    }

  if (!strcmp("DescribeMethods",argv[1]))
    {
    if(argc>3) {
      Tcl_SetResult ( interp, const_cast<char*>("Wrong number of arguments: object DescribeMethods <MethodName>"), TCL_VOLATILE ); 
      return TCL_ERROR;
 }
    if(argc==2) {

  Tcl_DString dString, dStringParent;

  Tcl_DStringInit ( &dString );

  Tcl_DStringInit ( &dStringParent );
    vtkpxLinearRegistrationCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetRegistrationParameters" );
    Tcl_DStringAppendElement ( &dString, "GetSimilarityResults" );
    Tcl_DStringAppendElement ( &dString, "GetInitialParameters" );
    Tcl_DStringAppendElement ( &dString, "SetInitialParameters" );
    Tcl_DStringAppendElement ( &dString, "SetReferenceMotionImage" );
    Tcl_DStringAppendElement ( &dString, "GetReferenceMotionImage" );
    Tcl_DStringAppendElement ( &dString, "SetMotionSequence" );
    Tcl_DStringAppendElement ( &dString, "GetMotionSequence" );
    Tcl_DStringAppendElement ( &dString, "GetLinearRegistration" );
    Tcl_DStringAppendElement ( &dString, "SetBeginFrame" );
    Tcl_DStringAppendElement ( &dString, "GetBeginFrame" );
    Tcl_DStringAppendElement ( &dString, "GetEndFrame" );
    Tcl_DStringAppendElement ( &dString, "SetEndFrame" );
    Tcl_DStringAppendElement ( &dString, "GetReferenceFrame" );
    Tcl_DStringAppendElement ( &dString, "SetReferenceFrame" );
    Tcl_DStringAppendElement ( &dString, "SetUseLastFrameForInitialization" );
    Tcl_DStringAppendElement ( &dString, "GetUseLastFrameForInitializationMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseLastFrameForInitializationMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseLastFrameForInitialization" );
    Tcl_DStringAppendElement ( &dString, "UseLastFrameForInitializationOn" );
    Tcl_DStringAppendElement ( &dString, "UseLastFrameForInitializationOff" );
    Tcl_DStringAppendElement ( &dString, "MotionCorrectSequence" );
    Tcl_DStringAppendElement ( &dString, "MotionCorrectSequenceMask" );
    Tcl_DStringAppendElement ( &dString, "MotionCorrectSequence" );
    Tcl_DStringAppendElement ( &dString, "SaveMotionParameters" );
    Tcl_DStringAppendElement ( &dString, "SaveMotionParameters" );
    Tcl_DStringAppendElement ( &dString, "LoadMotionParameters" );
    Tcl_DStringAppendElement ( &dString, "InitializeMotionParameters" );
    Tcl_DStringAppendElement ( &dString, "Run" );
    Tcl_DStringAppendElement ( &dString, "ComputeAllSimilarityMetricsBetweenImages" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxLinearRegistrationCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkpxLinearMotionCorrect *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetClassName */
    if ( strcmp ( argv[2], "GetClassName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetClassName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "const char *GetClassName();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetClassName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IsA */
    if ( strcmp ( argv[2], "IsA" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IsA */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int IsA(const char *name);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for IsA */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NewInstance */
    if ( strcmp ( argv[2], "NewInstance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NewInstance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for NewInstance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SafeDownCast */
    if ( strcmp ( argv[2], "SafeDownCast" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SafeDownCast */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRegistrationParameters */
    if ( strcmp ( argv[2], "GetRegistrationParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRegistrationParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRegistrationParameters */
    Tcl_DStringAppendElement ( &dString, " Return Final Set of Registration Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetRegistrationParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetRegistrationParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSimilarityResults */
    if ( strcmp ( argv[2], "GetSimilarityResults" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSimilarityResults" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSimilarityResults */
    Tcl_DStringAppendElement ( &dString, " Return Final Set of Registration Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetSimilarityResults();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetSimilarityResults */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialParameters */
    if ( strcmp ( argv[2], "GetInitialParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialParameters */
    Tcl_DStringAppendElement ( &dString, " Return Final Set of Registration Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetInitialParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetInitialParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialParameters */
    if ( strcmp ( argv[2], "SetInitialParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialParameters */
    Tcl_DStringAppendElement ( &dString, " Return Final Set of Registration Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialParameters(vtkDoubleArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SetInitialParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetReferenceMotionImage */
    if ( strcmp ( argv[2], "SetReferenceMotionImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetReferenceMotionImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetReferenceMotionImage */
    Tcl_DStringAppendElement ( &dString, " Sets referenceimage for the registration filter\n" );
    Tcl_DStringAppendElement ( &dString, "void SetReferenceMotionImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SetReferenceMotionImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetReferenceMotionImage */
    if ( strcmp ( argv[2], "GetReferenceMotionImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetReferenceMotionImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetReferenceMotionImage */
    Tcl_DStringAppendElement ( &dString, " Sets referenceimage for the registration filter\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetReferenceMotionImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetReferenceMotionImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMotionSequence */
    if ( strcmp ( argv[2], "SetMotionSequence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMotionSequence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMotionSequence */
    Tcl_DStringAppendElement ( &dString, " Set Transform Image Sequence\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMotionSequence(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SetMotionSequence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMotionSequence */
    if ( strcmp ( argv[2], "GetMotionSequence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMotionSequence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMotionSequence */
    Tcl_DStringAppendElement ( &dString, " Set Transform Image Sequence\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetMotionSequence();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetMotionSequence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLinearRegistration */
    if ( strcmp ( argv[2], "GetLinearRegistration" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLinearRegistration" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLinearRegistration */
    Tcl_DStringAppendElement ( &dString, " Get Linear Registration \n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration *GetLinearRegistration();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetLinearRegistration */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBeginFrame */
    if ( strcmp ( argv[2], "SetBeginFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBeginFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBeginFrame */
    Tcl_DStringAppendElement ( &dString, " Begin Frame, End Frame, Reference Frame\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBeginFrame(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SetBeginFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBeginFrame */
    if ( strcmp ( argv[2], "GetBeginFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBeginFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBeginFrame */
    Tcl_DStringAppendElement ( &dString, " Begin Frame, End Frame, Reference Frame\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBeginFrame();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetBeginFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEndFrame */
    if ( strcmp ( argv[2], "GetEndFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEndFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEndFrame */
    Tcl_DStringAppendElement ( &dString, " Begin Frame, End Frame, Reference Frame\n" );
    Tcl_DStringAppendElement ( &dString, "int GetEndFrame();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetEndFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetEndFrame */
    if ( strcmp ( argv[2], "SetEndFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetEndFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetEndFrame */
    Tcl_DStringAppendElement ( &dString, " Begin Frame, End Frame, Reference Frame\n" );
    Tcl_DStringAppendElement ( &dString, "void SetEndFrame(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SetEndFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetReferenceFrame */
    if ( strcmp ( argv[2], "GetReferenceFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetReferenceFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetReferenceFrame */
    Tcl_DStringAppendElement ( &dString, " Begin Frame, End Frame, Reference Frame\n" );
    Tcl_DStringAppendElement ( &dString, "int GetReferenceFrame();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetReferenceFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetReferenceFrame */
    if ( strcmp ( argv[2], "SetReferenceFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetReferenceFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetReferenceFrame */
    Tcl_DStringAppendElement ( &dString, " Begin Frame, End Frame, Reference Frame\n" );
    Tcl_DStringAppendElement ( &dString, "void SetReferenceFrame(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SetReferenceFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseLastFrameForInitialization */
    if ( strcmp ( argv[2], "SetUseLastFrameForInitialization" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseLastFrameForInitialization" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseLastFrameForInitialization */
    Tcl_DStringAppendElement ( &dString, " UseLastFrameForInitialization\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseLastFrameForInitialization(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SetUseLastFrameForInitialization */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseLastFrameForInitializationMinValue */
    if ( strcmp ( argv[2], "GetUseLastFrameForInitializationMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseLastFrameForInitializationMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseLastFrameForInitializationMinValue */
    Tcl_DStringAppendElement ( &dString, " UseLastFrameForInitialization\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseLastFrameForInitializationMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetUseLastFrameForInitializationMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseLastFrameForInitializationMaxValue */
    if ( strcmp ( argv[2], "GetUseLastFrameForInitializationMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseLastFrameForInitializationMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseLastFrameForInitializationMaxValue */
    Tcl_DStringAppendElement ( &dString, " UseLastFrameForInitialization\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseLastFrameForInitializationMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetUseLastFrameForInitializationMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseLastFrameForInitialization */
    if ( strcmp ( argv[2], "GetUseLastFrameForInitialization" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseLastFrameForInitialization" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseLastFrameForInitialization */
    Tcl_DStringAppendElement ( &dString, " UseLastFrameForInitialization\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseLastFrameForInitialization();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for GetUseLastFrameForInitialization */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseLastFrameForInitializationOn */
    if ( strcmp ( argv[2], "UseLastFrameForInitializationOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseLastFrameForInitializationOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseLastFrameForInitializationOn */
    Tcl_DStringAppendElement ( &dString, " UseLastFrameForInitialization\n" );
    Tcl_DStringAppendElement ( &dString, "void UseLastFrameForInitializationOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for UseLastFrameForInitializationOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseLastFrameForInitializationOff */
    if ( strcmp ( argv[2], "UseLastFrameForInitializationOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseLastFrameForInitializationOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseLastFrameForInitializationOff */
    Tcl_DStringAppendElement ( &dString, " UseLastFrameForInitialization\n" );
    Tcl_DStringAppendElement ( &dString, "void UseLastFrameForInitializationOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for UseLastFrameForInitializationOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MotionCorrectSequence */
    if ( strcmp ( argv[2], "MotionCorrectSequence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MotionCorrectSequence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MotionCorrectSequence */
    Tcl_DStringAppendElement ( &dString, " Warp Output Image\n" );
    Tcl_DStringAppendElement ( &dString, "static int MotionCorrectSequence(vtkImageData *Reference, vtkImageData *Output, vtkImageData *Input, vtkDoubleArray *Parameters, int interp_mode);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for MotionCorrectSequence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MotionCorrectSequenceMask */
    if ( strcmp ( argv[2], "MotionCorrectSequenceMask" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MotionCorrectSequenceMask" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MotionCorrectSequenceMask */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int MotionCorrectSequenceMask(vtkImageData *Reference, vtkImageData *Output, vtkImageData *Input, vtkDoubleArray *Parameters, int interp_mode);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for MotionCorrectSequenceMask */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MotionCorrectSequence */
    if ( strcmp ( argv[2], "MotionCorrectSequence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MotionCorrectSequence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MotionCorrectSequence */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int MotionCorrectSequence(vtkImageData *Reference, vtkImageData *Output, vtkImageData *Input, vtkDoubleArray *Parameters, int interp_mode, int center, short background=-100, short final=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for MotionCorrectSequence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveMotionParameters */
    if ( strcmp ( argv[2], "SaveMotionParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveMotionParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveMotionParameters */
    Tcl_DStringAppendElement ( &dString, " Save/Load Motion Parameters (Scalars)\n" );
    Tcl_DStringAppendElement ( &dString, "static int SaveMotionParameters(vtkDoubleArray *scalars, char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SaveMotionParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveMotionParameters */
    if ( strcmp ( argv[2], "SaveMotionParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveMotionParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveMotionParameters */
    Tcl_DStringAppendElement ( &dString, " Save/Load Motion Parameters (Scalars)\n" );
    Tcl_DStringAppendElement ( &dString, "static int SaveMotionParameters(vtkDoubleArray *scalars, char *fname, int nparam);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for SaveMotionParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadMotionParameters */
    if ( strcmp ( argv[2], "LoadMotionParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadMotionParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadMotionParameters */
    Tcl_DStringAppendElement ( &dString, " Save/Load Motion Parameters (Scalars)\n" );
    Tcl_DStringAppendElement ( &dString, "static int LoadMotionParameters(vtkDoubleArray *scalars, char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for LoadMotionParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InitializeMotionParameters */
    if ( strcmp ( argv[2], "InitializeMotionParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InitializeMotionParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InitializeMotionParameters */
    Tcl_DStringAppendElement ( &dString, " Save/Load Motion Parameters (Scalars)\n" );
    Tcl_DStringAppendElement ( &dString, "static int InitializeMotionParameters(vtkDoubleArray *scalars, int numframes);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for InitializeMotionParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Run */
    if ( strcmp ( argv[2], "Run" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Run" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Run */
    Tcl_DStringAppendElement ( &dString, " Run Motion Correction\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Run();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for Run */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeAllSimilarityMetricsBetweenImages */
    if ( strcmp ( argv[2], "ComputeAllSimilarityMetricsBetweenImages" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeAllSimilarityMetricsBetweenImages" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeAllSimilarityMetricsBetweenImages */
    Tcl_DStringAppendElement ( &dString, " Compute All Similarity Metrics Between Images After Reslicing\n" );
    Tcl_DStringAppendElement ( &dString, "static void ComputeAllSimilarityMetricsBetweenImages(vtkImageData *reference, int frame, vtkImageData *series, vtkDoubleArray *results);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearMotionCorrect" );
    /* Closing for ComputeAllSimilarityMetricsBetweenImages */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxLinearRegistrationCppCommand(static_cast<vtkpxLinearRegistration *>(op),interp,argc,argv) == TCL_OK)
    {
    return TCL_OK;
    }
    }
  catch (std::exception &e)
    {
    Tcl_AppendResult(interp, "Uncaught exception: ",  e.what(), "\n", NULL);
    return TCL_ERROR;
    }
  return TCL_ERROR;
}
