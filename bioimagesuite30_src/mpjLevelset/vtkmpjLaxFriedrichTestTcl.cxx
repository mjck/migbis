// tcl wrapper for vtkmpjLaxFriedrichTest object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkmpjLaxFriedrichTest.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkmpjLaxFriedrichTestNewCommand()
{
  vtkmpjLaxFriedrichTest *temp = vtkmpjLaxFriedrichTest::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkmpjLaxFriedrichTestCppCommand(vtkmpjLaxFriedrichTest *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkmpjLaxFriedrichTestCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkmpjLaxFriedrichTestCppCommand(static_cast<vtkmpjLaxFriedrichTest *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkmpjLaxFriedrichTestCppCommand(vtkmpjLaxFriedrichTest *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkmpjLaxFriedrichTest",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxSimpleImageToImageFilterCppCommand(static_cast<vtkpxSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxSimpleImageToImageFilter"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkmpjLaxFriedrichTest  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLaxFriedrichTest");
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
    vtkmpjLaxFriedrichTest  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLaxFriedrichTest");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkmpjLaxFriedrichTest  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLaxFriedrichTest");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSpeedImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetSpeedImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetKnownPoints",argv[1]))&&(argc == 3))
    {
    vtkFloatArray  *temp0;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    op->SetKnownPoints(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetKnownPoints",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->GetKnownPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("SetBoundaryPoints",argv[1]))&&(argc == 3))
    {
    vtkFloatArray  *temp0;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    op->SetBoundaryPoints(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBoundaryPoints",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->GetBoundaryPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfIterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfIterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLabelImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetLabelImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetConvergenceImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetConvergenceImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetSolution",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetSolution(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSolution",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetSolution();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetDomainImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetDomainImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDomainImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetDomainImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetBoundaryValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetBoundaryValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetBoundaryValue",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetBoundaryValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDomainValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetDomainValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetDomainValue",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetDomainValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkmpjLaxFriedrichTestCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkmpjLaxFriedrichTest:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetSpeedImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetKnownPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetKnownPoints\n",NULL);
    Tcl_AppendResult(interp,"  SetBoundaryPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBoundaryPoints\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLabelImage\n",NULL);
    Tcl_AppendResult(interp,"  GetConvergenceImage\n",NULL);
    Tcl_AppendResult(interp,"  SetSolution\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSolution\n",NULL);
    Tcl_AppendResult(interp,"  SetDomainImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDomainImage\n",NULL);
    Tcl_AppendResult(interp,"  GetBoundaryValue\n",NULL);
    Tcl_AppendResult(interp,"  SetBoundaryValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDomainValue\n",NULL);
    Tcl_AppendResult(interp,"  SetDomainValue\t with 1 arg\n",NULL);
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
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetSpeedImage" );
    Tcl_DStringAppendElement ( &dString, "SetKnownPoints" );
    Tcl_DStringAppendElement ( &dString, "GetKnownPoints" );
    Tcl_DStringAppendElement ( &dString, "SetBoundaryPoints" );
    Tcl_DStringAppendElement ( &dString, "GetBoundaryPoints" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfIterations" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfIterations" );
    Tcl_DStringAppendElement ( &dString, "GetLabelImage" );
    Tcl_DStringAppendElement ( &dString, "GetConvergenceImage" );
    Tcl_DStringAppendElement ( &dString, "SetSolution" );
    Tcl_DStringAppendElement ( &dString, "GetSolution" );
    Tcl_DStringAppendElement ( &dString, "SetDomainImage" );
    Tcl_DStringAppendElement ( &dString, "GetDomainImage" );
    Tcl_DStringAppendElement ( &dString, "GetBoundaryValue" );
    Tcl_DStringAppendElement ( &dString, "SetBoundaryValue" );
    Tcl_DStringAppendElement ( &dString, "GetDomainValue" );
    Tcl_DStringAppendElement ( &dString, "SetDomainValue" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkmpjLaxFriedrichTest *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSpeedImage */
    if ( strcmp ( argv[2], "SetSpeedImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSpeedImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSpeedImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetSpeedImage(vtkImageData *speed);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SetSpeedImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetKnownPoints */
    if ( strcmp ( argv[2], "SetKnownPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetKnownPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetKnownPoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get boundary conditions for the propagation\n" );
    Tcl_DStringAppendElement ( &dString, "void SetKnownPoints(vtkFloatArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SetKnownPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetKnownPoints */
    if ( strcmp ( argv[2], "GetKnownPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetKnownPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetKnownPoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get boundary conditions for the propagation\n" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetKnownPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetKnownPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBoundaryPoints */
    if ( strcmp ( argv[2], "SetBoundaryPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBoundaryPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBoundaryPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetBoundaryPoints(vtkFloatArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SetBoundaryPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBoundaryPoints */
    if ( strcmp ( argv[2], "GetBoundaryPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBoundaryPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBoundaryPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetBoundaryPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetBoundaryPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfIterations */
    if ( strcmp ( argv[2], "GetNumberOfIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfIterations */
    Tcl_DStringAppendElement ( &dString, " Get/Set Number of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetNumberOfIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfIterations */
    if ( strcmp ( argv[2], "SetNumberOfIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfIterations */
    Tcl_DStringAppendElement ( &dString, " Get/Set Number of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfIterations(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SetNumberOfIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLabelImage */
    if ( strcmp ( argv[2], "GetLabelImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLabelImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLabelImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetLabelImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetConvergenceImage */
    if ( strcmp ( argv[2], "GetConvergenceImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetConvergenceImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetConvergenceImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetConvergenceImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetConvergenceImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSolution */
    if ( strcmp ( argv[2], "SetSolution" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSolution" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSolution */
    Tcl_DStringAppendElement ( &dString, " Set/Get Solution\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSolution(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SetSolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSolution */
    if ( strcmp ( argv[2], "GetSolution" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSolution" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSolution */
    Tcl_DStringAppendElement ( &dString, " Set/Get Solution\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetSolution();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetSolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDomainImage */
    if ( strcmp ( argv[2], "SetDomainImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDomainImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDomainImage */
    Tcl_DStringAppendElement ( &dString, " Set/Get domain points\n" );
    Tcl_DStringAppendElement ( &dString, "void SetDomainImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SetDomainImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDomainImage */
    if ( strcmp ( argv[2], "GetDomainImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDomainImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDomainImage */
    Tcl_DStringAppendElement ( &dString, " Set/Get domain points\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetDomainImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetDomainImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBoundaryValue */
    if ( strcmp ( argv[2], "GetBoundaryValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBoundaryValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBoundaryValue */
    Tcl_DStringAppendElement ( &dString, " Get/Set Number of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "float GetBoundaryValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetBoundaryValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBoundaryValue */
    if ( strcmp ( argv[2], "SetBoundaryValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBoundaryValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBoundaryValue */
    Tcl_DStringAppendElement ( &dString, " Get/Set Number of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBoundaryValue(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SetBoundaryValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDomainValue */
    if ( strcmp ( argv[2], "GetDomainValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDomainValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDomainValue */
    Tcl_DStringAppendElement ( &dString, " Get/Set Number of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "float GetDomainValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for GetDomainValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDomainValue */
    if ( strcmp ( argv[2], "SetDomainValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDomainValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDomainValue */
    Tcl_DStringAppendElement ( &dString, " Get/Set Number of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetDomainValue(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxFriedrichTest" );
    /* Closing for SetDomainValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxSimpleImageToImageFilterCppCommand(static_cast<vtkpxSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
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
