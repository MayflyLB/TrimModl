#ifndef EDITTRIMCURVES_H_INCLUDED
#define EDITTRIMCURVES_H_INCLUDED


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
#include <NXOpen/BlockStyler_DoubleBlock.hxx>
#include "../Include/assistFun.h"
#include "../include/CurveAttrE.h"



using namespace std;
using namespace NXOpen;
using namespace NXOpen::BlockStyler;

class MoveLines
{
public:
    MoveLines(std::vector<vector<CurveData*>>&y):m_MoveLinesGroup(y)
    {
        m_currentGroup=NULL;
        m_currentMovLines.clear();
        m_focusCurve = NULL;
        m_focusPt = NULL;
        index = 0;
    }
    
    void adjustFocus(double *pt)
    {
        
    }

    void moveLine(double *pt)
    {
        if (m_currentMovLines.size()==2)
        {

        }
        else if (m_currentMovLines.size() == 3)
        {
            
        }
        else
        {

        }
    }
    
private:
    std::vector<vector<CurveData*>>& m_MoveLinesGroup;
    vector<CurveData*>* m_currentGroup;
    std::vector<CurveData*>m_currentMovLines;
    CurveData* m_focusCurve;
    double *m_focusPt;
    int index;
};

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

    template<class T>
    void deletePointerSet(vector<T*>&temp)
    {
        for (int i = 0; i < temp.size();i++)
            delete temp[i];
        temp.clear();
    }
    void simplifySpline();
    bool hasSpline();
    void GenerateAssistLines();

    
    PropertyList* GetBlockProperties(const char *blockID);
    DECLARE_CSYS_MFUN
private:
    const char* theDlxFileName;
    NXOpen::BlockStyler::BlockDialog* theDialog;
    NXOpen::BlockStyler::Group* groupGernate;// Block type: Group
    NXOpen::BlockStyler::CurveCollector* curvesSelect;// Block type: Curve Collector
    NXOpen::BlockStyler::SpecifyVector* cutDir;// Block type: Specify Vector
    NXOpen::BlockStyler::CurveCollector* assistSelect;// Block type: Curve Collector
    NXOpen::BlockStyler::SpecifyOrientation* manipulator;// Block type: Specify Orientation
    NXOpen::BlockStyler::Button* buttonDirType;// Block type: Button

    std::vector<CurveData*> m_pTrimCurves;                      //need delete
    std::vector<vector<CurveData*>> m_trimCurvesGroupUP;        //存指针需要delete，存对象不用delete

    std::vector<tag_t> m_allTrimCurves;

    std::vector<CurveData*> m_pAssistCurves;                    //need delete 加入两条修边线
    std::vector<vector<CurveData*>*> m_assistCurvesTempGroup;   //need delete!


    
    std::vector<TrimCurveData> m_trimCurveDataUP;
    std::vector<TrimCurveData> m_trimCurveDataDW;
    
    std::vector<vector<CurveData*>> m_assistCurvesGroup;

    CurveAttrE m_attr;

    CurveData* m_pCur_CD;
    CurveData* m_pPost_CD;
    CurveData m_editCD1;
    CurveData m_editCD2;

    Vector3d m_cutDir;
    bool m_hasSpline;
    int m_color;
};
#endif //EDITTRIMCURVES_H_INCLUDED
