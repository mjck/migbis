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

int vtkmpjTensorLeastSquaresCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorLeastSquaresNewCommand();
int vtkmpjImageTensorInvariantsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorInvariantsNewCommand();
int vtkmpjImageTensorCoherenceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorCoherenceNewCommand();
int vtkmpjImageTensorProductCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorProductNewCommand();
int vtkmpjLatticeAnisotropyCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjLatticeAnisotropyNewCommand();
int vtkmpjImageToTensorsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageToTensorsNewCommand();
int vtkmpjImageToVectorsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageToVectorsNewCommand();
int vtkmpjImageSymmetricTensorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageSymmetricTensorNewCommand();
int vtkmpjTensorGeometryMetricsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorGeometryMetricsNewCommand();
int vtkmpjImageTensorMultiplyCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorMultiplyNewCommand();
int vtkmpjApparentDiffusionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjApparentDiffusionNewCommand();
int vtkmpjTensorToApparentDiffusionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorToApparentDiffusionNewCommand();
int vtkmpjImageTensorColorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorColorNewCommand();
int vtkmpjImageDotProductCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageDotProductNewCommand();
int vtkmpjImageTensorSimilarityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorSimilarityNewCommand();
int vtkmpjImageTensorVarianceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorVarianceNewCommand();
int vtkmpjQBallImagingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjQBallImagingNewCommand();
int vtkmpjImageTensorTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorTransformNewCommand();
int vtkmpjHeartFiberAngleCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjHeartFiberAngleNewCommand();
int vtkmpjHeartSheetAngleCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjHeartSheetAngleNewCommand();
int vtkmpjTensorBMatrixLeastSquaresCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorBMatrixLeastSquaresNewCommand();
int vtkmpjImageTensorEigenAnalysisCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTensorEigenAnalysisNewCommand();
int vtkmpjToroidalVolumeCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjToroidalVolumeNewCommand();
int vtkmpjToroidalDissociationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjToroidalDissociationNewCommand();
int vtkmpjToroidalVolumeNormalizedCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjToroidalVolumeNormalizedNewCommand();
int vtkmpjImagePieMaskCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImagePieMaskNewCommand();
int vtkmpjAxialRadialDiffusivityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjAxialRadialDiffusivityNewCommand();
int vtkmpjCardiacAngleCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjCardiacAngleNewCommand();
int vtkmpjCardiacCoordinatesCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjCardiacCoordinatesNewCommand();
int vtkmpjEstimateDiffusionS0Command(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjEstimateDiffusionS0NewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkmpjdiffusiontcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkmpjdiffusiontcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkmpjdiffusiontcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkmpjdiffusiontcl_Init(interp);
}


int VTK_EXPORT Vtkmpjdiffusiontcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorLeastSquares"), vtkmpjTensorLeastSquaresNewCommand,
                  vtkmpjTensorLeastSquaresCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorInvariants"), vtkmpjImageTensorInvariantsNewCommand,
                  vtkmpjImageTensorInvariantsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorCoherence"), vtkmpjImageTensorCoherenceNewCommand,
                  vtkmpjImageTensorCoherenceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorProduct"), vtkmpjImageTensorProductNewCommand,
                  vtkmpjImageTensorProductCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjLatticeAnisotropy"), vtkmpjLatticeAnisotropyNewCommand,
                  vtkmpjLatticeAnisotropyCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageToTensors"), vtkmpjImageToTensorsNewCommand,
                  vtkmpjImageToTensorsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageToVectors"), vtkmpjImageToVectorsNewCommand,
                  vtkmpjImageToVectorsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageSymmetricTensor"), vtkmpjImageSymmetricTensorNewCommand,
                  vtkmpjImageSymmetricTensorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorGeometryMetrics"), vtkmpjTensorGeometryMetricsNewCommand,
                  vtkmpjTensorGeometryMetricsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorMultiply"), vtkmpjImageTensorMultiplyNewCommand,
                  vtkmpjImageTensorMultiplyCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjApparentDiffusion"), vtkmpjApparentDiffusionNewCommand,
                  vtkmpjApparentDiffusionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorToApparentDiffusion"), vtkmpjTensorToApparentDiffusionNewCommand,
                  vtkmpjTensorToApparentDiffusionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorColor"), vtkmpjImageTensorColorNewCommand,
                  vtkmpjImageTensorColorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageDotProduct"), vtkmpjImageDotProductNewCommand,
                  vtkmpjImageDotProductCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorSimilarity"), vtkmpjImageTensorSimilarityNewCommand,
                  vtkmpjImageTensorSimilarityCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorVariance"), vtkmpjImageTensorVarianceNewCommand,
                  vtkmpjImageTensorVarianceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjQBallImaging"), vtkmpjQBallImagingNewCommand,
                  vtkmpjQBallImagingCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorTransform"), vtkmpjImageTensorTransformNewCommand,
                  vtkmpjImageTensorTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjHeartFiberAngle"), vtkmpjHeartFiberAngleNewCommand,
                  vtkmpjHeartFiberAngleCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjHeartSheetAngle"), vtkmpjHeartSheetAngleNewCommand,
                  vtkmpjHeartSheetAngleCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorBMatrixLeastSquares"), vtkmpjTensorBMatrixLeastSquaresNewCommand,
                  vtkmpjTensorBMatrixLeastSquaresCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTensorEigenAnalysis"), vtkmpjImageTensorEigenAnalysisNewCommand,
                  vtkmpjImageTensorEigenAnalysisCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjToroidalVolume"), vtkmpjToroidalVolumeNewCommand,
                  vtkmpjToroidalVolumeCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjToroidalDissociation"), vtkmpjToroidalDissociationNewCommand,
                  vtkmpjToroidalDissociationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjToroidalVolumeNormalized"), vtkmpjToroidalVolumeNormalizedNewCommand,
                  vtkmpjToroidalVolumeNormalizedCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImagePieMask"), vtkmpjImagePieMaskNewCommand,
                  vtkmpjImagePieMaskCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjAxialRadialDiffusivity"), vtkmpjAxialRadialDiffusivityNewCommand,
                  vtkmpjAxialRadialDiffusivityCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjCardiacAngle"), vtkmpjCardiacAngleNewCommand,
                  vtkmpjCardiacAngleCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjCardiacCoordinates"), vtkmpjCardiacCoordinatesNewCommand,
                  vtkmpjCardiacCoordinatesCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjEstimateDiffusionS0"), vtkmpjEstimateDiffusionS0NewCommand,
                  vtkmpjEstimateDiffusionS0Command);
  char pkgName[]="vtkmpjDiffusionTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
