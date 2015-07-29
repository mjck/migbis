// tcl wrapper for vtkbisConnectivityUtility object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisConnectivityUtility.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisConnectivityUtilityNewCommand()
{
  vtkbisConnectivityUtility *temp = vtkbisConnectivityUtility::New();
  return static_cast<ClientData>(temp);
}

int vtkObjectCppCommand(vtkObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisConnectivityUtilityCppCommand(vtkbisConnectivityUtility *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisConnectivityUtilityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisConnectivityUtilityCppCommand(static_cast<vtkbisConnectivityUtility *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisConnectivityUtilityCppCommand(vtkbisConnectivityUtility *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisConnectivityUtility",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkObject"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
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
    vtkbisConnectivityUtility  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisConnectivityUtility");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisConnectivityUtility  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisConnectivityUtility");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkbisConnectivityUtility  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisConnectivityUtility");
    return TCL_OK;
    }
  if ((!strcmp("ComputeRatio",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    double   temp2;
    double   temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeRatio(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ImagetoText",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    char    *temp1;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    op->ImagetoText(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("TexttoImage",argv[1]))&&(argc == 4))
    {
    char    *temp0;
    vtkImageData  *temp1;
    error = 0;

    temp0 = argv[2];
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->TexttoImage(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("TexttoMarix6Cols",argv[1]))&&(argc == 4))
    {
    char    *temp0;
    vtkpxMatrix  *temp1;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    temp20 = (op)->TexttoMarix6Cols(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SplitBlocks",argv[1]))&&(argc == 6))
    {
    vtkpxMatrix  *temp0;
    int      temp1;
    int      temp2;
    vtkIntArray  *temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    temp3 = (vtkIntArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkIntArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->SplitBlocks(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ExtractImageBlocks",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkIntArray  *temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkIntArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkIntArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->ExtractImageBlocks(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("DiscardImageBlocks",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkIntArray  *temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkIntArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkIntArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->DiscardImageBlocks(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Ztransform",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->Ztransform(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ROCCurve",argv[1]))&&(argc == 8))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    int      temp3;
    vtkDoubleArray  *temp4;
    char    *temp5;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    temp4 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkDoubleArray"),interp,error));
    temp5 = argv[7];
    if (!error)
    {
    op->ROCCurve(temp0,temp1,temp2,temp3,temp4,temp5);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisConnectivityUtilityCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisConnectivityUtility:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  ComputeRatio\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ImagetoText\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  TexttoImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  TexttoMarix6Cols\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SplitBlocks\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ExtractImageBlocks\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  DiscardImageBlocks\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  Ztransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ROCCurve\t with 6 args\n",NULL);
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
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "ComputeRatio" );
    Tcl_DStringAppendElement ( &dString, "ImagetoText" );
    Tcl_DStringAppendElement ( &dString, "TexttoImage" );
    Tcl_DStringAppendElement ( &dString, "TexttoMarix6Cols" );
    Tcl_DStringAppendElement ( &dString, "SplitBlocks" );
    Tcl_DStringAppendElement ( &dString, "ExtractImageBlocks" );
    Tcl_DStringAppendElement ( &dString, "DiscardImageBlocks" );
    Tcl_DStringAppendElement ( &dString, "Ztransform" );
    Tcl_DStringAppendElement ( &dString, "ROCCurve" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkObjectCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
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
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkbisConnectivityUtility *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeRatio */
    if ( strcmp ( argv[2], "ComputeRatio" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeRatio" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeRatio */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static double ComputeRatio(vtkImageData *input, vtkImageData *mask, double threshold);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for ComputeRatio */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ImagetoText */
    if ( strcmp ( argv[2], "ImagetoText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ImagetoText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ImagetoText */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void ImagetoText(vtkImageData *input, const char *filename);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for ImagetoText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TexttoImage */
    if ( strcmp ( argv[2], "TexttoImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TexttoImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TexttoImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void TexttoImage(const char *filename, vtkImageData *out);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for TexttoImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TexttoMarix6Cols */
    if ( strcmp ( argv[2], "TexttoMarix6Cols" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TexttoMarix6Cols" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TexttoMarix6Cols */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int TexttoMarix6Cols(const char *filename, vtkpxMatrix *outmatrix);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for TexttoMarix6Cols */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SplitBlocks */
    if ( strcmp ( argv[2], "SplitBlocks" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SplitBlocks" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkIntArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SplitBlocks */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int SplitBlocks(vtkpxMatrix *matrix, int column, int numrows, vtkIntArray *output);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for SplitBlocks */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ExtractImageBlocks */
    if ( strcmp ( argv[2], "ExtractImageBlocks" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ExtractImageBlocks" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkIntArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ExtractImageBlocks */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int ExtractImageBlocks(vtkImageData *input, vtkImageData *result, vtkIntArray *frames);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for ExtractImageBlocks */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DiscardImageBlocks */
    if ( strcmp ( argv[2], "DiscardImageBlocks" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DiscardImageBlocks" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkIntArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DiscardImageBlocks */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int DiscardImageBlocks(vtkImageData *input, vtkImageData *result, vtkIntArray *frames);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for DiscardImageBlocks */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Ztransform */
    if ( strcmp ( argv[2], "Ztransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Ztransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Ztransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void Ztransform(vtkImageData *zmap, vtkImageData *output);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for Ztransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ROCCurve */
    if ( strcmp ( argv[2], "ROCCurve" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ROCCurve" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ROCCurve */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void ROCCurve(vtkImageData *mask, vtkImageData *groundtruth, vtkImageData *segmentedimage, int steps, vtkDoubleArray *output, const char *Filename);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisConnectivityUtility" );
    /* Closing for ROCCurve */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
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
