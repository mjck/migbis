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

int vtkmpjExtractLevelSetCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjExtractLevelSetNewCommand();
int vtkmpjFastMarchingMethodCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFastMarchingMethodNewCommand();
int vtkmpjFastMarchingExtensionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFastMarchingExtensionNewCommand();
int vtkmpjLevelSetNormalCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLevelSetNormalNewCommand();
int vtkmpjLevelSetNarrowBandFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLevelSetNarrowBandFilterNewCommand();
int vtkmpjLevelSetSignedDistanceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLevelSetSignedDistanceNewCommand();
int vtkmpjLevelSetMethodCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLevelSetMethodNewCommand();
int vtkmpjLevelSetTestCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLevelSetTestNewCommand();
int vtkmpjFastSweepingMethodCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFastSweepingMethodNewCommand();
int vtkmpjLaxFriedrichSweepingMethodCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLaxFriedrichSweepingMethodNewCommand();
int vtkmpjImageCharacteristicsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageCharacteristicsNewCommand();
int vtkmpjLaxFriedrichEuclideanCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLaxFriedrichEuclideanNewCommand();
int vtkmpjLaxFriedrichTestCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLaxFriedrichTestNewCommand();
int vtkmpjLaxConnectivityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLaxConnectivityNewCommand();
int vtkmpjLaxFriedrichSweepingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLaxFriedrichSweepingNewCommand();
int vtkmpjFastMarchingConnectivityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFastMarchingConnectivityNewCommand();
int vtkmpjFastMarchingImageFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFastMarchingImageFilterNewCommand();
int vtkmpjLevelSetNeighborhoodExtractorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLevelSetNeighborhoodExtractorNewCommand();
int vtkmpjReinitializeLevelSetImageFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjReinitializeLevelSetImageFilterNewCommand();
int vtkmpjFastMarchingVesselConnectivityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFastMarchingVesselConnectivityNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkmpjlevelsettcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkmpjlevelsettcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkmpjlevelsettcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkmpjlevelsettcl_Init(interp);
}


int VTK_EXPORT Vtkmpjlevelsettcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjExtractLevelSet"), vtkmpjExtractLevelSetNewCommand,
                  vtkmpjExtractLevelSetCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFastMarchingMethod"), vtkmpjFastMarchingMethodNewCommand,
                  vtkmpjFastMarchingMethodCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFastMarchingExtension"), vtkmpjFastMarchingExtensionNewCommand,
                  vtkmpjFastMarchingExtensionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLevelSetNormal"), vtkmpjLevelSetNormalNewCommand,
                  vtkmpjLevelSetNormalCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLevelSetNarrowBandFilter"), vtkmpjLevelSetNarrowBandFilterNewCommand,
                  vtkmpjLevelSetNarrowBandFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLevelSetSignedDistance"), vtkmpjLevelSetSignedDistanceNewCommand,
                  vtkmpjLevelSetSignedDistanceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLevelSetMethod"), vtkmpjLevelSetMethodNewCommand,
                  vtkmpjLevelSetMethodCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLevelSetTest"), vtkmpjLevelSetTestNewCommand,
                  vtkmpjLevelSetTestCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFastSweepingMethod"), vtkmpjFastSweepingMethodNewCommand,
                  vtkmpjFastSweepingMethodCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLaxFriedrichSweepingMethod"), vtkmpjLaxFriedrichSweepingMethodNewCommand,
                  vtkmpjLaxFriedrichSweepingMethodCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageCharacteristics"), vtkmpjImageCharacteristicsNewCommand,
                  vtkmpjImageCharacteristicsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLaxFriedrichEuclidean"), vtkmpjLaxFriedrichEuclideanNewCommand,
                  vtkmpjLaxFriedrichEuclideanCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLaxFriedrichTest"), vtkmpjLaxFriedrichTestNewCommand,
                  vtkmpjLaxFriedrichTestCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLaxConnectivity"), vtkmpjLaxConnectivityNewCommand,
                  vtkmpjLaxConnectivityCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLaxFriedrichSweeping"), vtkmpjLaxFriedrichSweepingNewCommand,
                  vtkmpjLaxFriedrichSweepingCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFastMarchingConnectivity"), vtkmpjFastMarchingConnectivityNewCommand,
                  vtkmpjFastMarchingConnectivityCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFastMarchingImageFilter"), vtkmpjFastMarchingImageFilterNewCommand,
                  vtkmpjFastMarchingImageFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLevelSetNeighborhoodExtractor"), vtkmpjLevelSetNeighborhoodExtractorNewCommand,
                  vtkmpjLevelSetNeighborhoodExtractorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjReinitializeLevelSetImageFilter"), vtkmpjReinitializeLevelSetImageFilterNewCommand,
                  vtkmpjReinitializeLevelSetImageFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFastMarchingVesselConnectivity"), vtkmpjFastMarchingVesselConnectivityNewCommand,
                  vtkmpjFastMarchingVesselConnectivityCommand);
  char pkgName[]="vtkmpjLevelsetTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
