#ifndef EDITTRIMCURVESUI_H_INCLUDED
#define EDITTRIMCURVESUI_H_INCLUDED


#include <uf_defs.h>
#include <uf_ui_types.h>
#include <iostream>
#include <NXOpen/Session.hxx>
#include <NXOpen/UI.hxx>
#include <NXOpen/NXMessageBox.hxx>
#include <NXOpen/Callback.hxx>
#include <NXOpen/NXException.hxx>
#include <NXOpen/BlockStyler_UIBlock.hxx>
#include <NXOpen/BlockStyler_BlockDialog.hxx>
#include <NXOpen/BlockStyler_PropertyList.hxx>
#include <NXOpen/BlockStyler_Group.hxx>
#include <NXOpen/BlockStyler_CurveCollector.hxx>
#include <NXOpen/BlockStyler_SpecifyVector.hxx>
#include <NXOpen/BlockStyler_SpecifyPoint.hxx>
#include <NXOpen/BlockStyler_SpecifyOrientation.hxx>
#include <NXOpen/BlockStyler_Button.hxx>
#include <NXOpen/BlockStyler_LinearDimension.hxx>
#include <NXOpen/BlockStyler_DoubleBlock.hxx>
#include "../Include/assistFun.h"
#include "../include/CurveAttrE.h"
#include "ImplEditCurves.h"


using namespace std;
using namespace NXOpen;
using namespace NXOpen::BlockStyler;

class DllExport EditTrimCurves
{
    // class members
public:
    static Session *theSession;
    static UI *theUI;
    EditTrimCurves();
    ~EditTrimCurves();
    int Show();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(NXOpen::BlockStyler::UIBlock* block);
    
    PropertyList* GetBlockProperties(const char *blockID);
    DECLARE_CSYS_MFUN
private:
    const char* theDlxFileName;
    NXOpen::BlockStyler::BlockDialog* theDialog;
    NXOpen::BlockStyler::Group* groupGernate;// Block type: Group
    NXOpen::BlockStyler::CurveCollector* curvesSelect;// Block type: Curve Collector
    NXOpen::BlockStyler::SpecifyVector* cutDir;// Block type: Specify Vector
    NXOpen::BlockStyler::SpecifyVector* generateDir;// Block type: Specify Vector
    NXOpen::BlockStyler::Button* generateDirIsOK;// Block type: Button
    NXOpen::BlockStyler::CurveCollector* assistSelect;// Block type: Curve Collector
    NXOpen::BlockStyler::Button* cutButton;// Block type: Button
    NXOpen::BlockStyler::LinearDimension* linears[20];// Block type: Linear Dim
    NXOpen::BlockStyler::SpecifyOrientation* manipulator;// Block type: Specify Orientation

    ImplEditCurves * m_implEdit;

};
#endif //EDITTRIMCURVES_H_INCLUDED
