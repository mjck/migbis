#include "vtkTclUtil.h"
#include "vtkVersion.h"
#define VTK_TCL_TO_STRING(x) VTK_TCL_TO_STRING0(x)
#define VTK_TCL_TO_STRING0(x) #x
extern "C"
{
#if (TCL_MAJOR_VERSION == 8) && (TCL_MINOR_VERSION >= 4)
  typedef int (*vtkTclCommandType)(ClientData, Tcl_Interp *,int, CONST84 char *[]);
#else
  typedef int (*vtkTclCommandType)(ClientData, Tcl_Interp *,int, char *[]);
#endif
}

int vtkpxContourSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxContourSourceNewCommand();
int vtkpxFemSolidCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxFemSolidNewCommand();
int vtkpxShapeTrackingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxShapeTrackingNewCommand();
int vtkpxSplineSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSplineSourceNewCommand();
int vtkpxSplineSourceUndoStackCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSplineSourceUndoStackNewCommand();
int vtkpxSplineStackSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSplineStackSourceNewCommand();
int vtkpxTriangulatedStackSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxTriangulatedStackSourceNewCommand();
int vtkpxTstackReaderCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxTstackReaderNewCommand();
int vtkpxComputeStrainsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxComputeStrainsNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkpxlegacytcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkpxlegacytcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkpxlegacytcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkpxlegacytcl_Init(interp);
}


int VTK_EXPORT Vtkpxlegacytcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkpxContourSource"), vtkpxContourSourceNewCommand,
                  vtkpxContourSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxFemSolid"), vtkpxFemSolidNewCommand,
                  vtkpxFemSolidCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxShapeTracking"), vtkpxShapeTrackingNewCommand,
                  vtkpxShapeTrackingCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSplineSource"), vtkpxSplineSourceNewCommand,
                  vtkpxSplineSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSplineSourceUndoStack"), vtkpxSplineSourceUndoStackNewCommand,
                  vtkpxSplineSourceUndoStackCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSplineStackSource"), vtkpxSplineStackSourceNewCommand,
                  vtkpxSplineStackSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxTriangulatedStackSource"), vtkpxTriangulatedStackSourceNewCommand,
                  vtkpxTriangulatedStackSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxTstackReader"), vtkpxTstackReaderNewCommand,
                  vtkpxTstackReaderCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxComputeStrains"), vtkpxComputeStrainsNewCommand,
                  vtkpxComputeStrainsCommand);
  char pkgName[]="vtkpxLegacyTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
