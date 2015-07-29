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

int vtkpxLogExponentialFitCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxLogExponentialFitNewCommand();
int vtkpxImageRobustHistogramCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageRobustHistogramNewCommand();
int vtkpxHistogramSegmentationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxHistogramSegmentationNewCommand();
int vtkpxMRFSegmentationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxMRFSegmentationNewCommand();
int vtkpxBrainPreSegmentCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxBrainPreSegmentNewCommand();
int vtkpxAbstractBiasFieldCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAbstractBiasFieldCorrectionNewCommand();
int vtkpxSliceBiasFieldCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSliceBiasFieldCorrectionNewCommand();
int vtkpxPolynomialBiasFieldCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxPolynomialBiasFieldCorrectionNewCommand();
int vtkpxImageTensorSplineCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageTensorSplineNewCommand();
int vtkpxLevelSetUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxLevelSetUtilNewCommand();
int vtkpxTissueLevelSetFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxTissueLevelSetFilterNewCommand();
int vtkpxCoupledTissueLevelSetFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCoupledTissueLevelSetFilterNewCommand();
int vtkjwsSignedDanielssonDistanceMapImageFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjwsSignedDanielssonDistanceMapImageFilterNewCommand();
int vtkbisBinaryThinningFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisBinaryThinningFilterNewCommand();
int vtkjoBETHistogramCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjoBETHistogramNewCommand();
int vtkjoBETImageFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjoBETImageFilterNewCommand();
int vtkjoPolyDataRobustBETTrainingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjoPolyDataRobustBETTrainingNewCommand();
int vtkjoScalePolyDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjoScalePolyDataNewCommand();
int vtkpxExponentialFitT1Command(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxExponentialFitT1NewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkpxsegmentationtcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkpxsegmentationtcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkpxsegmentationtcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkpxsegmentationtcl_Init(interp);
}


int VTK_EXPORT Vtkpxsegmentationtcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkpxLogExponentialFit"), vtkpxLogExponentialFitNewCommand,
                  vtkpxLogExponentialFitCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageRobustHistogram"), vtkpxImageRobustHistogramNewCommand,
                  vtkpxImageRobustHistogramCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxHistogramSegmentation"), vtkpxHistogramSegmentationNewCommand,
                  vtkpxHistogramSegmentationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxMRFSegmentation"), vtkpxMRFSegmentationNewCommand,
                  vtkpxMRFSegmentationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxBrainPreSegment"), vtkpxBrainPreSegmentNewCommand,
                  vtkpxBrainPreSegmentCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAbstractBiasFieldCorrection"), vtkpxAbstractBiasFieldCorrectionNewCommand,
                  vtkpxAbstractBiasFieldCorrectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSliceBiasFieldCorrection"), vtkpxSliceBiasFieldCorrectionNewCommand,
                  vtkpxSliceBiasFieldCorrectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxPolynomialBiasFieldCorrection"), vtkpxPolynomialBiasFieldCorrectionNewCommand,
                  vtkpxPolynomialBiasFieldCorrectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageTensorSpline"), vtkpxImageTensorSplineNewCommand,
                  vtkpxImageTensorSplineCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxLevelSetUtil"), vtkpxLevelSetUtilNewCommand,
                  vtkpxLevelSetUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxTissueLevelSetFilter"), vtkpxTissueLevelSetFilterNewCommand,
                  vtkpxTissueLevelSetFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCoupledTissueLevelSetFilter"), vtkpxCoupledTissueLevelSetFilterNewCommand,
                  vtkpxCoupledTissueLevelSetFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjwsSignedDanielssonDistanceMapImageFilter"), vtkjwsSignedDanielssonDistanceMapImageFilterNewCommand,
                  vtkjwsSignedDanielssonDistanceMapImageFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisBinaryThinningFilter"), vtkbisBinaryThinningFilterNewCommand,
                  vtkbisBinaryThinningFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjoBETHistogram"), vtkjoBETHistogramNewCommand,
                  vtkjoBETHistogramCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjoBETImageFilter"), vtkjoBETImageFilterNewCommand,
                  vtkjoBETImageFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjoPolyDataRobustBETTraining"), vtkjoPolyDataRobustBETTrainingNewCommand,
                  vtkjoPolyDataRobustBETTrainingCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjoScalePolyData"), vtkjoScalePolyDataNewCommand,
                  vtkjoScalePolyDataCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxExponentialFitT1"), vtkpxExponentialFitT1NewCommand,
                  vtkpxExponentialFitT1Command);
  char pkgName[]="vtkpxSegmentationTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
