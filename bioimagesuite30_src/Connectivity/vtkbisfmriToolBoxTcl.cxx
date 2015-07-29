// tcl wrapper for vtkbisfmriToolBox object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisfmriToolBox.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisfmriToolBoxNewCommand()
{
  vtkbisfmriToolBox *temp = vtkbisfmriToolBox::New();
  return static_cast<ClientData>(temp);
}

int vtkSimpleImageToImageFilterCppCommand(vtkSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisfmriToolBoxCppCommand(vtkbisfmriToolBox *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisfmriToolBoxCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisfmriToolBoxCppCommand(static_cast<vtkbisfmriToolBox *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisfmriToolBoxCppCommand(vtkbisfmriToolBox *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisfmriToolBox",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkSimpleImageToImageFilterCppCommand(static_cast<vtkSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkSimpleImageToImageFilter"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkbisfmriToolBox  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisfmriToolBox");
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
    vtkbisfmriToolBox  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisfmriToolBox");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisfmriToolBox  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisfmriToolBox");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFunctionalImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetFunctionalImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFunctionalImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetFunctionalImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetroiMap",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetroiMap(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetroiMap",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetroiMap();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetOutData2",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetOutData2(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutData2",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetOutData2();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetThreshold",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetThreshold(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetThreshold",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetFilename",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetFilename(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFilename",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetFilename();
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
  if ((!strcmp("SetFilename2",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetFilename2(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFilename2",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetFilename2();
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
  if ((!strcmp("SetFilename3",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetFilename3(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFilename3",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetFilename3();
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
  if ((!strcmp("ComputeBrainNetwork",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    float    temp1;
    int      temp2;
    vtkFloatArray  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeBrainNetwork(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeDegreesUnd",argv[1]))&&(argc == 3))
    {
    vtkFloatArray  *temp0;
    vtkIntArray  *temp20;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeDegreesUnd(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkIntArray");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeNumberOfTriangles",argv[1]))&&(argc == 4))
    {
    vtkFloatArray  *temp0;
    int      temp1;
    float    temp20;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeNumberOfTriangles(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeBetweenessCentrality",argv[1]))&&(argc == 3))
    {
    vtkFloatArray  *temp0;
    vtkpxMatrix  *temp20;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeBetweenessCentrality(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxMatrix");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeLocalEfficiency",argv[1]))&&(argc == 4))
    {
    vtkFloatArray  *temp0;
    vtkIntArray  *temp1;
    vtkFloatArray  *temp20;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    temp1 = (vtkIntArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkIntArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeLocalEfficiency(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeNetworkMeasures",argv[1]))&&(argc == 7))
    {
    vtkFloatArray  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkIntArray  *temp3;
    vtkDataArray  *temp4;
    vtkFloatArray  *temp20;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkIntArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkIntArray"),interp,error));
    temp4 = (vtkDataArray *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkDataArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeNetworkMeasures(temp0,temp1,temp2,temp3,temp4);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateOutputForAlarkViewer",argv[1]))&&(argc == 7))
    {
    vtkFloatArray  *temp0;
    vtkpxMatrix  *temp1;
    vtkFloatArray  *temp2;
    char    *temp3;
    vtkDataArray  *temp4;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp2 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkFloatArray"),interp,error));
    temp3 = argv[5];
    temp4 = (vtkDataArray *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkDataArray"),interp,error));
    if (!error)
    {
    op->GenerateOutputForAlarkViewer(temp0,temp1,temp2,temp3,temp4);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateAdjMatrixOutputForAlarkViewer",argv[1]))&&(argc == 5))
    {
    vtkFloatArray  *temp0;
    vtkFloatArray  *temp1;
    char    *temp2;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    temp1 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkFloatArray"),interp,error));
    temp2 = argv[4];
    if (!error)
    {
    op->GenerateAdjMatrixOutputForAlarkViewer(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateROICorrelationOutputForAlarkViewer",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkFloatArray  *temp1;
    char    *temp2;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkFloatArray"),interp,error));
    temp2 = argv[4];
    if (!error)
    {
    op->GenerateROICorrelationOutputForAlarkViewer(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisfmriToolBoxCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisfmriToolBox:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFunctionalImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFunctionalImage\n",NULL);
    Tcl_AppendResult(interp,"  SetroiMap\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetroiMap\n",NULL);
    Tcl_AppendResult(interp,"  SetOutData2\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutData2\n",NULL);
    Tcl_AppendResult(interp,"  SetThreshold\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetThreshold\n",NULL);
    Tcl_AppendResult(interp,"  SetFilename\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFilename\n",NULL);
    Tcl_AppendResult(interp,"  SetFilename2\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFilename2\n",NULL);
    Tcl_AppendResult(interp,"  SetFilename3\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFilename3\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBrainNetwork\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeDegreesUnd\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeNumberOfTriangles\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBetweenessCentrality\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeLocalEfficiency\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeNetworkMeasures\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  GenerateOutputForAlarkViewer\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  GenerateAdjMatrixOutputForAlarkViewer\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GenerateROICorrelationOutputForAlarkViewer\t with 3 args\n",NULL);
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
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetFunctionalImage" );
    Tcl_DStringAppendElement ( &dString, "GetFunctionalImage" );
    Tcl_DStringAppendElement ( &dString, "SetroiMap" );
    Tcl_DStringAppendElement ( &dString, "GetroiMap" );
    Tcl_DStringAppendElement ( &dString, "SetOutData2" );
    Tcl_DStringAppendElement ( &dString, "GetOutData2" );
    Tcl_DStringAppendElement ( &dString, "SetThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetFilename" );
    Tcl_DStringAppendElement ( &dString, "GetFilename" );
    Tcl_DStringAppendElement ( &dString, "SetFilename2" );
    Tcl_DStringAppendElement ( &dString, "GetFilename2" );
    Tcl_DStringAppendElement ( &dString, "SetFilename3" );
    Tcl_DStringAppendElement ( &dString, "GetFilename3" );
    Tcl_DStringAppendElement ( &dString, "ComputeBrainNetwork" );
    Tcl_DStringAppendElement ( &dString, "ComputeDegreesUnd" );
    Tcl_DStringAppendElement ( &dString, "ComputeNumberOfTriangles" );
    Tcl_DStringAppendElement ( &dString, "ComputeBetweenessCentrality" );
    Tcl_DStringAppendElement ( &dString, "ComputeLocalEfficiency" );
    Tcl_DStringAppendElement ( &dString, "ComputeNetworkMeasures" );
    Tcl_DStringAppendElement ( &dString, "GenerateOutputForAlarkViewer" );
    Tcl_DStringAppendElement ( &dString, "GenerateAdjMatrixOutputForAlarkViewer" );
    Tcl_DStringAppendElement ( &dString, "GenerateROICorrelationOutputForAlarkViewer" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkbisfmriToolBox *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFunctionalImage */
    if ( strcmp ( argv[2], "SetFunctionalImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFunctionalImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFunctionalImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetFunctionalImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for SetFunctionalImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFunctionalImage */
    if ( strcmp ( argv[2], "GetFunctionalImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFunctionalImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFunctionalImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetFunctionalImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GetFunctionalImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetroiMap */
    if ( strcmp ( argv[2], "SetroiMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetroiMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetroiMap */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetroiMap(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for SetroiMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetroiMap */
    if ( strcmp ( argv[2], "GetroiMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetroiMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetroiMap */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetroiMap();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GetroiMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutData2 */
    if ( strcmp ( argv[2], "SetOutData2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutData2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutData2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutData2(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for SetOutData2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutData2 */
    if ( strcmp ( argv[2], "GetOutData2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutData2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutData2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetOutData2();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GetOutData2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetThreshold */
    if ( strcmp ( argv[2], "SetThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetThreshold */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetThreshold(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for SetThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetThreshold */
    if ( strcmp ( argv[2], "GetThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetThreshold */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GetThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFilename */
    if ( strcmp ( argv[2], "SetFilename" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFilename" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFilename */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFilename(const char *filename);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for SetFilename */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFilename */
    if ( strcmp ( argv[2], "GetFilename" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFilename" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFilename */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *GetFilename();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GetFilename */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFilename2 */
    if ( strcmp ( argv[2], "SetFilename2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFilename2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFilename2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFilename2(const char *filename2);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for SetFilename2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFilename2 */
    if ( strcmp ( argv[2], "GetFilename2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFilename2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFilename2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *GetFilename2();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GetFilename2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFilename3 */
    if ( strcmp ( argv[2], "SetFilename3" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFilename3" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFilename3 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFilename3(const char *filename3);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for SetFilename3 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFilename3 */
    if ( strcmp ( argv[2], "GetFilename3" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFilename3" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFilename3 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *GetFilename3();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GetFilename3 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBrainNetwork */
    if ( strcmp ( argv[2], "ComputeBrainNetwork" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBrainNetwork" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBrainNetwork */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkFloatArray *ComputeBrainNetwork(vtkImageData *roiCorrelation, float threshold, int binary);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for ComputeBrainNetwork */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeDegreesUnd */
    if ( strcmp ( argv[2], "ComputeDegreesUnd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeDegreesUnd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeDegreesUnd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkIntArray *ComputeDegreesUnd(vtkFloatArray *brainNetwork);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for ComputeDegreesUnd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeNumberOfTriangles */
    if ( strcmp ( argv[2], "ComputeNumberOfTriangles" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeNumberOfTriangles" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeNumberOfTriangles */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static float ComputeNumberOfTriangles(vtkFloatArray *brainNetwork, int tuple);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for ComputeNumberOfTriangles */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBetweenessCentrality */
    if ( strcmp ( argv[2], "ComputeBetweenessCentrality" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBetweenessCentrality" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBetweenessCentrality */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkpxMatrix *ComputeBetweenessCentrality(vtkFloatArray *brainNetwork);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for ComputeBetweenessCentrality */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeLocalEfficiency */
    if ( strcmp ( argv[2], "ComputeLocalEfficiency" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeLocalEfficiency" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "vtkIntArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeLocalEfficiency */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkFloatArray *ComputeLocalEfficiency(vtkFloatArray *brainNetwork, vtkIntArray *degrees);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for ComputeLocalEfficiency */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeNetworkMeasures */
    if ( strcmp ( argv[2], "ComputeNetworkMeasures" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeNetworkMeasures" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkIntArray" );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeNetworkMeasures */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkFloatArray *ComputeNetworkMeasures(vtkFloatArray *brainNetwork, vtkImageData *brain, vtkImageData *ROI, vtkIntArray *degrees, vtkDataArray *out);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for ComputeNetworkMeasures */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateOutputForAlarkViewer */
    if ( strcmp ( argv[2], "GenerateOutputForAlarkViewer" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateOutputForAlarkViewer" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateOutputForAlarkViewer */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void GenerateOutputForAlarkViewer(vtkFloatArray *localE, vtkpxMatrix *BC, vtkFloatArray *otherData, const char *Filename2, vtkDataArray *out2);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GenerateOutputForAlarkViewer */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateAdjMatrixOutputForAlarkViewer */
    if ( strcmp ( argv[2], "GenerateAdjMatrixOutputForAlarkViewer" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateAdjMatrixOutputForAlarkViewer" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateAdjMatrixOutputForAlarkViewer */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void GenerateAdjMatrixOutputForAlarkViewer(vtkFloatArray *brainNetwork, vtkFloatArray *otherData, const char *Filename);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GenerateAdjMatrixOutputForAlarkViewer */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateROICorrelationOutputForAlarkViewer */
    if ( strcmp ( argv[2], "GenerateROICorrelationOutputForAlarkViewer" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateROICorrelationOutputForAlarkViewer" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateROICorrelationOutputForAlarkViewer */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void GenerateROICorrelationOutputForAlarkViewer(vtkImageData *roiCorrelation, vtkFloatArray *otherData, const char *Filename3);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisfmriToolBox" );
    /* Closing for GenerateROICorrelationOutputForAlarkViewer */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkSimpleImageToImageFilterCppCommand(static_cast<vtkSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
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
