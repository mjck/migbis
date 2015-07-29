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

int vtkmpjTensorGlyphsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorGlyphsNewCommand();
int vtkmpjDiffusionProfileCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjDiffusionProfileNewCommand();
int vtkmpjTubeFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTubeFilterNewCommand();
int vtkmpjRibbonFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjRibbonFilterNewCommand();
int vtkmpjRationalGaussianSheetCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjRationalGaussianSheetNewCommand();
int vtkmpjVertexSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjVertexSourceNewCommand();
int vtkmpjSphereSubdivisionSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjSphereSubdivisionSourceNewCommand();
int vtkmpjIcosahedronSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjIcosahedronSourceNewCommand();
int vtkmpjToroidSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjToroidSourceNewCommand();
int vtkmpjToroidalSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjToroidalSourceNewCommand();
int vtkmpjToroidalExtSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjToroidalExtSourceNewCommand();
int vtkmpjSuperToroidalSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjSuperToroidalSourceNewCommand();
int vtkmpjSuperToroidalExtSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjSuperToroidalExtSourceNewCommand();
int vtkmpjEllipsoidSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjEllipsoidSourceNewCommand();
int vtkmpjSuperquadricsSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjSuperquadricsSourceNewCommand();
int vtkmpj3DOrientationMarkerWidgetCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpj3DOrientationMarkerWidgetNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkmpjgraphicstcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkmpjgraphicstcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkmpjgraphicstcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkmpjgraphicstcl_Init(interp);
}


int VTK_EXPORT Vtkmpjgraphicstcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorGlyphs"), vtkmpjTensorGlyphsNewCommand,
                  vtkmpjTensorGlyphsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjDiffusionProfile"), vtkmpjDiffusionProfileNewCommand,
                  vtkmpjDiffusionProfileCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTubeFilter"), vtkmpjTubeFilterNewCommand,
                  vtkmpjTubeFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjRibbonFilter"), vtkmpjRibbonFilterNewCommand,
                  vtkmpjRibbonFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjRationalGaussianSheet"), vtkmpjRationalGaussianSheetNewCommand,
                  vtkmpjRationalGaussianSheetCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjVertexSource"), vtkmpjVertexSourceNewCommand,
                  vtkmpjVertexSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjSphereSubdivisionSource"), vtkmpjSphereSubdivisionSourceNewCommand,
                  vtkmpjSphereSubdivisionSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjIcosahedronSource"), vtkmpjIcosahedronSourceNewCommand,
                  vtkmpjIcosahedronSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjToroidSource"), vtkmpjToroidSourceNewCommand,
                  vtkmpjToroidSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjToroidalSource"), vtkmpjToroidalSourceNewCommand,
                  vtkmpjToroidalSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjToroidalExtSource"), vtkmpjToroidalExtSourceNewCommand,
                  vtkmpjToroidalExtSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjSuperToroidalSource"), vtkmpjSuperToroidalSourceNewCommand,
                  vtkmpjSuperToroidalSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjSuperToroidalExtSource"), vtkmpjSuperToroidalExtSourceNewCommand,
                  vtkmpjSuperToroidalExtSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjEllipsoidSource"), vtkmpjEllipsoidSourceNewCommand,
                  vtkmpjEllipsoidSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjSuperquadricsSource"), vtkmpjSuperquadricsSourceNewCommand,
                  vtkmpjSuperquadricsSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpj3DOrientationMarkerWidget"), vtkmpj3DOrientationMarkerWidgetNewCommand,
                  vtkmpj3DOrientationMarkerWidgetCommand);
  char pkgName[]="vtkmpjGraphicsTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
