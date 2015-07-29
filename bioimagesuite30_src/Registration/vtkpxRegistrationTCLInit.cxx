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

int vtkpxTransformationUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxTransformationUtilNewCommand();
int vtkbisTransformationTestUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisTransformationTestUtilNewCommand();
int vtkpxJointHistogramCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxJointHistogramNewCommand();
int vtkpxRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxRegistrationNewCommand();
int vtkpxSparseMatrixCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSparseMatrixNewCommand();
int vtkpxLinearTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxLinearTransformNewCommand();
int vtkpxGridTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGridTransformNewCommand();
int vtkpxComboTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxComboTransformNewCommand();
int vtkpxWeightedLandmarkTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxWeightedLandmarkTransformNewCommand();
int vtkpxAbstractIntensityRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAbstractIntensityRegistrationNewCommand();
int vtkpxLinearRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxLinearRegistrationNewCommand();
int vtkpxMidlineSymmetricRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxMidlineSymmetricRegistrationNewCommand();
int vtkpxNonLinearRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxNonLinearRegistrationNewCommand();
int vtkpxNonLinearLabelRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxNonLinearLabelRegistrationNewCommand();
int vtkpxDistortionCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxDistortionCorrectionNewCommand();
int vtkpxRPMCorrespondenceFinderCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxRPMCorrespondenceFinderNewCommand();
int vtkpxAbstractRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAbstractRPMRegistrationNewCommand();
int vtkpxLinearRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxLinearRPMRegistrationNewCommand();
int vtkpxTPSRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxTPSRPMRegistrationNewCommand();
int vtkpxBSplineRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxBSplineRPMRegistrationNewCommand();
int vtkpxBSplinePriorRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxBSplinePriorRPMRegistrationNewCommand();
int vtkpxNonLinearIntegratedRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxNonLinearIntegratedRegistrationNewCommand();
int vtkpxLinearMotionCorrectCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxLinearMotionCorrectNewCommand();
int vtkokKochCoefficientSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkokKochCoefficientSourceNewCommand();
int vtkpxSimilarityTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSimilarityTransformNewCommand();
int vtkpxPolyAbstractTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxPolyAbstractTransformNewCommand();
int vtkpxPolySimilarityTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxPolySimilarityTransformNewCommand();
int vtkpxArticulatedJointTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxArticulatedJointTransformNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkpxregistrationtcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkpxregistrationtcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkpxregistrationtcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkpxregistrationtcl_Init(interp);
}


int VTK_EXPORT Vtkpxregistrationtcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkpxTransformationUtil"), vtkpxTransformationUtilNewCommand,
                  vtkpxTransformationUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisTransformationTestUtil"), vtkbisTransformationTestUtilNewCommand,
                  vtkbisTransformationTestUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxJointHistogram"), vtkpxJointHistogramNewCommand,
                  vtkpxJointHistogramCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxRegistration"), vtkpxRegistrationNewCommand,
                  vtkpxRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSparseMatrix"), vtkpxSparseMatrixNewCommand,
                  vtkpxSparseMatrixCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxLinearTransform"), vtkpxLinearTransformNewCommand,
                  vtkpxLinearTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGridTransform"), vtkpxGridTransformNewCommand,
                  vtkpxGridTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxComboTransform"), vtkpxComboTransformNewCommand,
                  vtkpxComboTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxWeightedLandmarkTransform"), vtkpxWeightedLandmarkTransformNewCommand,
                  vtkpxWeightedLandmarkTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAbstractIntensityRegistration"), vtkpxAbstractIntensityRegistrationNewCommand,
                  vtkpxAbstractIntensityRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxLinearRegistration"), vtkpxLinearRegistrationNewCommand,
                  vtkpxLinearRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxMidlineSymmetricRegistration"), vtkpxMidlineSymmetricRegistrationNewCommand,
                  vtkpxMidlineSymmetricRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxNonLinearRegistration"), vtkpxNonLinearRegistrationNewCommand,
                  vtkpxNonLinearRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxNonLinearLabelRegistration"), vtkpxNonLinearLabelRegistrationNewCommand,
                  vtkpxNonLinearLabelRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxDistortionCorrection"), vtkpxDistortionCorrectionNewCommand,
                  vtkpxDistortionCorrectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxRPMCorrespondenceFinder"), vtkpxRPMCorrespondenceFinderNewCommand,
                  vtkpxRPMCorrespondenceFinderCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAbstractRPMRegistration"), vtkpxAbstractRPMRegistrationNewCommand,
                  vtkpxAbstractRPMRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxLinearRPMRegistration"), vtkpxLinearRPMRegistrationNewCommand,
                  vtkpxLinearRPMRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxTPSRPMRegistration"), vtkpxTPSRPMRegistrationNewCommand,
                  vtkpxTPSRPMRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxBSplineRPMRegistration"), vtkpxBSplineRPMRegistrationNewCommand,
                  vtkpxBSplineRPMRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxBSplinePriorRPMRegistration"), vtkpxBSplinePriorRPMRegistrationNewCommand,
                  vtkpxBSplinePriorRPMRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxNonLinearIntegratedRegistration"), vtkpxNonLinearIntegratedRegistrationNewCommand,
                  vtkpxNonLinearIntegratedRegistrationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxLinearMotionCorrect"), vtkpxLinearMotionCorrectNewCommand,
                  vtkpxLinearMotionCorrectCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkokKochCoefficientSource"), vtkokKochCoefficientSourceNewCommand,
                  vtkokKochCoefficientSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSimilarityTransform"), vtkpxSimilarityTransformNewCommand,
                  vtkpxSimilarityTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxPolyAbstractTransform"), vtkpxPolyAbstractTransformNewCommand,
                  vtkpxPolyAbstractTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxPolySimilarityTransform"), vtkpxPolySimilarityTransformNewCommand,
                  vtkpxPolySimilarityTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxArticulatedJointTransform"), vtkpxArticulatedJointTransformNewCommand,
                  vtkpxArticulatedJointTransformCommand);
  char pkgName[]="vtkpxRegistrationTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
