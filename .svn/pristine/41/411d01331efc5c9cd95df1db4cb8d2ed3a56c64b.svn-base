#ifndef UPPERSTEELINSERTTRIMMING_H_INCLUDED
#define UPPERSTEELINSERTTRIMMING_H_INCLUDED
#include <vector>
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
#include <NXOpen/BlockStyler_BodyCollector.hxx>
#include <NXOpen/BlockStyler_Toggle.hxx>
#include <NXOpen/BlockStyler_DoubleBlock.hxx>
#include <NXOpen/BlockStyler_CurveCollector.hxx>
#include <NXOpen/BlockStyler_SpecifyVector.hxx>
#include <NXOpen/BlockStyler_SpecifyPoint.hxx>
#include <NXOpen/BlockStyler_Enumeration.hxx>
#include <NXOpen/BlockStyler_SpecifyOrientation.hxx>
#include <NXOpen/BlockStyler_Button.hxx>
#include <NXOpen/BlockStyler_LinearDimension.hxx>
#include <NXOpen/BlockStyler_Label.hxx>

#include "../include/CommonOperation.h"
#include "../include/assistFun.h"
#include "../Include/CurveAttrE.h"
using namespace std;
using namespace NXOpen;
using namespace NXOpen::BlockStyler;

template class DllExport std::allocator<NXOpen::TaggedObject *>;
template class DllExport std::vector<NXOpen::TaggedObject *, allocator<NXOpen::TaggedObject *>>;

struct LineData
{
    UF_CURVE_line_t lt1;
    UF_CURVE_line_t lt2;
    UF_CURVE_line_t ltStart;
    UF_CURVE_line_t ltEnd;
    double midpt[3];
    tag_t line1;
    tag_t line2;
    tag_t lineStart;
    tag_t lineEnd;
};

class DllExport SteelInsertTrimming:public CommonTraits::Struct_Type
{
public:
    static Session *theSession;
    static UI *theUI;
    SteelInsertTrimming();
    ~SteelInsertTrimming();
    int Show();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(NXOpen::BlockStyler::UIBlock* block);
    PropertyList* GetBlockProperties(const char *blockID);
private:

    void setValueUE()
    {
        if (m_isInitial == false)
        {
            CommonTraits::initEntireUST(this);
            setValue();
        }
    }
    void setValueUL()
    {
        if (m_isInitial == false)
        {
            CommonTraits::initLocalUST(this);
            setValue();
        }
    }
    void setValueDL()
    {
        if (m_isInitial == false)
        {
            CommonTraits::initLocalDST(this);
            setValue();
        }
    }

    void setValue();
    void getValue();

    void getAllTrimCurves();
    void filterCurves()
    {
        std::vector<NXOpen::TaggedObject *>&&temp=curvesSelect->GetSelectedObjects();
        CurveAttrE curveAttre;
        std::vector<NXOpen::TaggedObject *>::iterator iter;
        for (iter=temp.begin();iter!=temp.end();)
        {
            if (!curveAttre.isSpecifyCurve((*iter)->GetTag()))
                temp.erase(iter);
            else
                iter++;
        }
        curvesSelect->SetSelectedObjects(temp);
    }

    CommonOperation* retOper();
    void generateOper()
    {
        CurveAttrE curveAttre;
        curveAttre.getTrimCurves(curvesSelect->GetSelectedObjects());
        CommonOperation* m_GenerateOper= retOper();
        (*m_GenerateOper)(curveAttre);
        delete m_GenerateOper;
    }
    DECLARE_CSYS_MFUN


private:
    const char* theDlxFileName;
    NXOpen::BlockStyler::BlockDialog* theDialog;
    NXOpen::BlockStyler::Group* group;// Block type: Group
    NXOpen::BlockStyler::BodyCollector* selectSheet;// Block type: Body Collector
    NXOpen::BlockStyler::Toggle* isMove;// Block type: Toggle
    NXOpen::BlockStyler::Group* CopyGroup;// Block type: Group
    NXOpen::BlockStyler::SpecifyVector* moveDirUI;// Block type: Specify Vector
    NXOpen::BlockStyler::DoubleBlock* sheetMoveDistUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* sheetOffsetDistUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* offsetDeviationUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* stepoverDeviationUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* intervalDistUI;// Block type: Double
    NXOpen::BlockStyler::CurveCollector* curvesSelect;// Block type: Curve Collector
    NXOpen::BlockStyler::Enumeration* modlTypeUI;// Block type: Enumeration
    NXOpen::BlockStyler::Group* groupParam;// Block type: Group
    NXOpen::BlockStyler::Enumeration* toolTypeUI;// Block type: Enumeration
    NXOpen::BlockStyler::DoubleBlock* cutDeltaLenUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* toolHeightUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* cutDirCutLenUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* cutDirToolBlankUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* cutShapeWideUI;// Block type: Double
    NXOpen::BlockStyler::DoubleBlock* cutShapeBlankUI;// Block type: Double
};
#endif //UPPERSTEELINSERTTRIMMING_H_INCLUDED
