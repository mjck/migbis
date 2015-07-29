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

int vtkbisDatabaseCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisDatabaseNewCommand();
int vtkbisBaseProcedureCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisBaseProcedureNewCommand();
int vtkbisPatientCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisPatientNewCommand();
int vtkbisProtocolCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisProtocolNewCommand();
int vtkbisQueryCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisQueryNewCommand();
int vtkbisVisitCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisVisitNewCommand();
int vtkbisSeriesCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisSeriesNewCommand();
int vtkbisStudyCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisStudyNewCommand();
int vtkbisInjectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisInjectionNewCommand();
int vtkimSQLiteCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkimSQLiteNewCommand();
int vtkbisSQLiteDatabaseCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisSQLiteDatabaseNewCommand();
int vtkbisFilenameTableCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisFilenameTableNewCommand();
int vtkbisAlgorithmTableCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisAlgorithmTableNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkbisdatabasetcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkbisdatabasetcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkbisdatabasetcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkbisdatabasetcl_Init(interp);
}


int VTK_EXPORT Vtkbisdatabasetcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkbisDatabase"), vtkbisDatabaseNewCommand,
                  vtkbisDatabaseCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisBaseProcedure"), vtkbisBaseProcedureNewCommand,
                  vtkbisBaseProcedureCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisPatient"), vtkbisPatientNewCommand,
                  vtkbisPatientCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisProtocol"), vtkbisProtocolNewCommand,
                  vtkbisProtocolCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisQuery"), vtkbisQueryNewCommand,
                  vtkbisQueryCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisVisit"), vtkbisVisitNewCommand,
                  vtkbisVisitCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisSeries"), vtkbisSeriesNewCommand,
                  vtkbisSeriesCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisStudy"), vtkbisStudyNewCommand,
                  vtkbisStudyCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisInjection"), vtkbisInjectionNewCommand,
                  vtkbisInjectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkimSQLite"), vtkimSQLiteNewCommand,
                  vtkimSQLiteCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisSQLiteDatabase"), vtkbisSQLiteDatabaseNewCommand,
                  vtkbisSQLiteDatabaseCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisFilenameTable"), vtkbisFilenameTableNewCommand,
                  vtkbisFilenameTableCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisAlgorithmTable"), vtkbisAlgorithmTableNewCommand,
                  vtkbisAlgorithmTableCommand);
  char pkgName[]="vtkbisDatabaseTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
