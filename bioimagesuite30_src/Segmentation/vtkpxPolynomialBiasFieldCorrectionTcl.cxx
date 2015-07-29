// tcl wrapper for vtkpxPolynomialBiasFieldCorrection object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxPolynomialBiasFieldCorrection.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxPolynomialBiasFieldCorrectionNewCommand()
{
  vtkpxPolynomialBiasFieldCorrection *temp = vtkpxPolynomialBiasFieldCorrection::New();
  return static_cast<ClientData>(temp);
}

int vtkpxAbstractBiasFieldCorrectionCppCommand(vtkpxAbstractBiasFieldCorrection *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxPolynomialBiasFieldCorrectionCppCommand(vtkpxPolynomialBiasFieldCorrection *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxPolynomialBiasFieldCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxPolynomialBiasFieldCorrectionCppCommand(static_cast<vtkpxPolynomialBiasFieldCorrection *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxPolynomialBiasFieldCorrectionCppCommand(vtkpxPolynomialBiasFieldCorrection *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxPolynomialBiasFieldCorrection",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxAbstractBiasFieldCorrectionCppCommand(static_cast<vtkpxAbstractBiasFieldCorrection *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxAbstractBiasFieldCorrection"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxPolynomialBiasFieldCorrection  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxPolynomialBiasFieldCorrection");
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
    vtkpxPolynomialBiasFieldCorrection  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxPolynomialBiasFieldCorrection");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxPolynomialBiasFieldCorrection  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxPolynomialBiasFieldCorrection");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLastSimilarity",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLastSimilarity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeBiasFit",argv[1]))&&(argc == 3))
    {
    int      temp0;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeBiasFit(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCoefficients",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetCoefficients();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("GetInitialCoefficients",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetInitialCoefficients();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialCoefficients",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->SetInitialCoefficients(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetRecomputeParameters",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRecomputeParameters(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRecomputeParametersMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRecomputeParametersMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRecomputeParametersMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRecomputeParametersMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRecomputeParameters",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRecomputeParameters();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMinValue",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMinValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMinValueMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinValueMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinValueMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinValueMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaxValue",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMaxValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaxValueMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxValueMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxValueMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxValueMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetDegree",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDegree(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDegreeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDegreeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDegreeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDegreeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDegree",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDegree();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeBiasFieldImage",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkDoubleArray  *temp1;
    int      temp2;
    double   temp3;
    double   temp4;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeBiasFieldImage(temp0,temp1,temp2,temp3,temp4);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeBiasFieldImage",argv[1]))&&(argc == 8))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkDoubleArray  *temp2;
    int      temp3;
    double   temp4;
    double   temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeBiasFieldImage(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeEntropy",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    float    temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeEntropy(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetOptimizationMethod",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetOptimizationMethod(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOptimizationMethod",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOptimizationMethod();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToPowell",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToPowell();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToGradientDescent",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToGradientDescent();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToConjugateGradient",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToConjugateGradient();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxPolynomialBiasFieldCorrectionCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxAbstractBiasFieldCorrectionCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxPolynomialBiasFieldCorrection:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLastSimilarity\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBiasFit\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCoefficients\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialCoefficients\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialCoefficients\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetRecomputeParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRecomputeParametersMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRecomputeParametersMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRecomputeParameters\n",NULL);
    Tcl_AppendResult(interp,"  SetMinValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMinValueMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinValueMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinValue\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxValueMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxValueMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetDegree\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDegreeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDegreeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDegree\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBiasFieldImage\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBiasFieldImage\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeEntropy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethod\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOptimizationMethod\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToPowell\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToGradientDescent\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToConjugateGradient\n",NULL);
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
    vtkpxAbstractBiasFieldCorrectionCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetLastSimilarity" );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFit" );
    Tcl_DStringAppendElement ( &dString, "GetCoefficients" );
    Tcl_DStringAppendElement ( &dString, "GetInitialCoefficients" );
    Tcl_DStringAppendElement ( &dString, "SetInitialCoefficients" );
    Tcl_DStringAppendElement ( &dString, "SetRecomputeParameters" );
    Tcl_DStringAppendElement ( &dString, "GetRecomputeParametersMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRecomputeParametersMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetRecomputeParameters" );
    Tcl_DStringAppendElement ( &dString, "SetMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinValueMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinValueMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinValue" );
    Tcl_DStringAppendElement ( &dString, "SetMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxValueMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxValueMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetDegree" );
    Tcl_DStringAppendElement ( &dString, "GetDegreeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetDegreeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetDegree" );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFieldImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFieldImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeEntropy" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethod" );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationMethod" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToPowell" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToGradientDescent" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToConjugateGradient" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxAbstractBiasFieldCorrectionCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxPolynomialBiasFieldCorrection *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLastSimilarity */
    if ( strcmp ( argv[2], "GetLastSimilarity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLastSimilarity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLastSimilarity */
    Tcl_DStringAppendElement ( &dString, " Get Last Output Measures\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLastSimilarity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetLastSimilarity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBiasFit */
    if ( strcmp ( argv[2], "ComputeBiasFit" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFit" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBiasFit */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int ComputeBiasFit(int level);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for ComputeBiasFit */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCoefficients */
    if ( strcmp ( argv[2], "GetCoefficients" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCoefficients" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCoefficients */
    Tcl_DStringAppendElement ( &dString, " Get The Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetCoefficients();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetCoefficients */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialCoefficients */
    if ( strcmp ( argv[2], "GetInitialCoefficients" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialCoefficients" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialCoefficients */
    Tcl_DStringAppendElement ( &dString, " Initial Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetInitialCoefficients();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetInitialCoefficients */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialCoefficients */
    if ( strcmp ( argv[2], "SetInitialCoefficients" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialCoefficients" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialCoefficients */
    Tcl_DStringAppendElement ( &dString, " Initial Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialCoefficients(vtkDoubleArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetInitialCoefficients */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRecomputeParameters */
    if ( strcmp ( argv[2], "SetRecomputeParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRecomputeParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRecomputeParameters */
    Tcl_DStringAppendElement ( &dString, " RecomputeParameters\n 1 == update parameters & pabic\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRecomputeParameters(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetRecomputeParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRecomputeParametersMinValue */
    if ( strcmp ( argv[2], "GetRecomputeParametersMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRecomputeParametersMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRecomputeParametersMinValue */
    Tcl_DStringAppendElement ( &dString, " RecomputeParameters\n 1 == update parameters & pabic\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRecomputeParametersMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetRecomputeParametersMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRecomputeParametersMaxValue */
    if ( strcmp ( argv[2], "GetRecomputeParametersMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRecomputeParametersMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRecomputeParametersMaxValue */
    Tcl_DStringAppendElement ( &dString, " RecomputeParameters\n 1 == update parameters & pabic\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRecomputeParametersMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetRecomputeParametersMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRecomputeParameters */
    if ( strcmp ( argv[2], "GetRecomputeParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRecomputeParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRecomputeParameters */
    Tcl_DStringAppendElement ( &dString, " RecomputeParameters\n 1 == update parameters & pabic\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRecomputeParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetRecomputeParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMinValue */
    if ( strcmp ( argv[2], "SetMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMinValue */
    Tcl_DStringAppendElement ( &dString, " MinValue/MaxValue\n Minimum/Maximum Value of the Bias Field =0.2,5.0\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMinValue(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinValueMinValue */
    if ( strcmp ( argv[2], "GetMinValueMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinValueMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinValueMinValue */
    Tcl_DStringAppendElement ( &dString, " MinValue/MaxValue\n Minimum/Maximum Value of the Bias Field =0.2,5.0\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinValueMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetMinValueMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinValueMaxValue */
    if ( strcmp ( argv[2], "GetMinValueMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinValueMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinValueMaxValue */
    Tcl_DStringAppendElement ( &dString, " MinValue/MaxValue\n Minimum/Maximum Value of the Bias Field =0.2,5.0\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinValueMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetMinValueMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinValue */
    if ( strcmp ( argv[2], "GetMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinValue */
    Tcl_DStringAppendElement ( &dString, " MinValue/MaxValue\n Minimum/Maximum Value of the Bias Field =0.2,5.0\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxValue */
    if ( strcmp ( argv[2], "SetMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxValue */
    Tcl_DStringAppendElement ( &dString, " MinValue/MaxValue\n Minimum/Maximum Value of the Bias Field =0.2,5.0\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxValue(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxValueMinValue */
    if ( strcmp ( argv[2], "GetMaxValueMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxValueMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxValueMinValue */
    Tcl_DStringAppendElement ( &dString, " MinValue/MaxValue\n Minimum/Maximum Value of the Bias Field =0.2,5.0\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxValueMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetMaxValueMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxValueMaxValue */
    if ( strcmp ( argv[2], "GetMaxValueMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxValueMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxValueMaxValue */
    Tcl_DStringAppendElement ( &dString, " MinValue/MaxValue\n Minimum/Maximum Value of the Bias Field =0.2,5.0\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxValueMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetMaxValueMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxValue */
    if ( strcmp ( argv[2], "GetMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxValue */
    Tcl_DStringAppendElement ( &dString, " MinValue/MaxValue\n Minimum/Maximum Value of the Bias Field =0.2,5.0\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDegree */
    if ( strcmp ( argv[2], "SetDegree" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDegree" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDegree */
    Tcl_DStringAppendElement ( &dString, " Degre Linear or Quadratic\n" );
    Tcl_DStringAppendElement ( &dString, "void SetDegree(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetDegree */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDegreeMinValue */
    if ( strcmp ( argv[2], "GetDegreeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDegreeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDegreeMinValue */
    Tcl_DStringAppendElement ( &dString, " Degre Linear or Quadratic\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDegreeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetDegreeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDegreeMaxValue */
    if ( strcmp ( argv[2], "GetDegreeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDegreeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDegreeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Degre Linear or Quadratic\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDegreeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetDegreeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDegree */
    if ( strcmp ( argv[2], "GetDegree" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDegree" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDegree */
    Tcl_DStringAppendElement ( &dString, " Degre Linear or Quadratic\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDegree();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetDegree */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBiasFieldImage */
    if ( strcmp ( argv[2], "ComputeBiasFieldImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFieldImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBiasFieldImage */
    Tcl_DStringAppendElement ( &dString, " Create Bias Field Image\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeBiasFieldImage(vtkImageData *ref, vtkDoubleArray *tr, int degree=-1, double minvalue=0.5, double maxvalue=2.5);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for ComputeBiasFieldImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBiasFieldImage */
    if ( strcmp ( argv[2], "ComputeBiasFieldImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFieldImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBiasFieldImage */
    Tcl_DStringAppendElement ( &dString, " Create Bias Field Image\n" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeBiasFieldImage(vtkImageData *ref, vtkImageData *bf, vtkDoubleArray *tr, int degree=1, double minvalue=0.5, double maxvalue=2.5);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for ComputeBiasFieldImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeEntropy */
    if ( strcmp ( argv[2], "ComputeEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeEntropy */
    Tcl_DStringAppendElement ( &dString, " Compute Entropy\n" );
    Tcl_DStringAppendElement ( &dString, "static float ComputeEntropy(vtkImageData *tmp);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for ComputeEntropy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethod */
    if ( strcmp ( argv[2], "SetOptimizationMethod" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethod" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethod */
    Tcl_DStringAppendElement ( &dString, " Optimization Method\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOptimizationMethod(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetOptimizationMethod */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOptimizationMethod */
    if ( strcmp ( argv[2], "GetOptimizationMethod" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationMethod" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOptimizationMethod */
    Tcl_DStringAppendElement ( &dString, " Optimization Method\n" );
    Tcl_DStringAppendElement ( &dString, "int GetOptimizationMethod();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for GetOptimizationMethod */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToPowell */
    if ( strcmp ( argv[2], "SetOptimizationMethodToPowell" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToPowell" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToPowell */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToPowell();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetOptimizationMethodToPowell */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToGradientDescent */
    if ( strcmp ( argv[2], "SetOptimizationMethodToGradientDescent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToGradientDescent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToGradientDescent */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToGradientDescent();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetOptimizationMethodToGradientDescent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToConjugateGradient */
    if ( strcmp ( argv[2], "SetOptimizationMethodToConjugateGradient" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToConjugateGradient" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToConjugateGradient */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToConjugateGradient();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolynomialBiasFieldCorrection" );
    /* Closing for SetOptimizationMethodToConjugateGradient */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxAbstractBiasFieldCorrectionCppCommand(static_cast<vtkpxAbstractBiasFieldCorrection *>(op),interp,argc,argv) == TCL_OK)
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
