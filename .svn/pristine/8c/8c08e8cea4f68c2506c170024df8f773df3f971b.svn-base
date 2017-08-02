
#include "EditTrimCurves.hpp"
using namespace NXOpen;
using namespace NXOpen::BlockStyler;


Session *(EditTrimCurves::theSession) = NULL;
UI *(EditTrimCurves::theUI) = NULL;

EditTrimCurves::EditTrimCurves()
{
    try
    {
        EditTrimCurves::theSession = NXOpen::Session::GetSession();
        EditTrimCurves::theUI = UI::GetUI();
        theDlxFileName = "EditTrimCurves.dlx";
        theDialog = EditTrimCurves::theUI->CreateDialog(theDlxFileName);
        theDialog->AddApplyHandler(make_callback(this, &EditTrimCurves::apply_cb));
        theDialog->AddOkHandler(make_callback(this, &EditTrimCurves::ok_cb));
        theDialog->AddUpdateHandler(make_callback(this, &EditTrimCurves::update_cb));
        theDialog->AddInitializeHandler(make_callback(this, &EditTrimCurves::initialize_cb));
        theDialog->AddDialogShownHandler(make_callback(this, &EditTrimCurves::dialogShown_cb));
        INIT_ABS_CSYS();
    }
    catch(exception&)
    {
        throw;
    }
}

EditTrimCurves::~EditTrimCurves()
{
    if (theDialog != NULL)
    {
        TERM_ABS_CSYS();
        delete theDialog;
        theDialog = NULL;
    }
}

void EditTrimCurves::initialize_cb()
{
    try
    {
        curvesSelect = dynamic_cast<NXOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("curvesSelect"));
        assistSelect = dynamic_cast<NXOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("assistSelect"));
        cutDir = dynamic_cast<NXOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("cutDir"));
        manipulator = dynamic_cast<NXOpen::BlockStyler::SpecifyOrientation*>(theDialog->TopBlock()->FindBlock("Manipulator"));
        buttonDirType = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("buttonDirType"));

        m_pCur_CD = NULL;
        m_pPost_CD = NULL;
        m_hasSpline = false;
        m_pTrimCurves.reserve(256);
        m_pTrimCurves.clear();          //need delete
        m_allTrimCurves.clear();
        m_trimCurvesGroupUP.clear();      //do not delete
        m_pAssistCurves.clear();        //need delete
        m_assistCurvesTempGroup.clear();
        m_cutDir.setValue(NULL);
        m_hasSpline=false;
        m_color = 0;
  
    }
    catch(exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

int EditTrimCurves::update_cb(NXOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if(block == curvesSelect) { }
        else if(block == assistSelect)
        {
            std::vector<NXOpen::TaggedObject *>&&temp = assistSelect->GetSelectedObjects();
            for (int i = 0; i < temp.size(); i++)
                m_pAssistCurves.push_back(new CurveData(temp[i]->GetTag()));
        }  
        else if(block == cutDir)
        {
            std::vector<NXOpen::TaggedObject *>&&temp=curvesSelect->GetSelectedObjects();
            for (int i = 0; i < temp.size(); i++)
            {
                m_allTrimCurves.push_back(temp[i]->GetTag());
                m_pTrimCurves.push_back(new CurveData(m_allTrimCurves.back()));
            }
            m_cutDir = cutDir->Vector();
            if (!hasSpline()&& m_allTrimCurves.size()&&!(is_3DCurve(m_allTrimCurves)))
                sortCurvesPointor(m_pTrimCurves, m_trimCurvesGroupUP);
            else if (m_cutDir.Z != 0 && m_allTrimCurves.size())
            {
                simplifySpline();
            }
            else
                SHOW_INFO_USR("修边线处理异常！\n");

            GenerateAssistLines();
            curvesSelect->SetEnable(false);
            NXOpen::Point3d origin_;
            manipulator->SetEnable(true);
            manipulator->SetOrigin(origin_);
            origin_.X = 1;
            manipulator->SetXAxis((Vector3d)origin_);
            origin_.X = 0;
            origin_.Y = 1;
            manipulator->SetYAxis((Vector3d)origin_);
        }
        else if(block == manipulator)
        {
            NXOpen::Point3d &&temp=manipulator->Origin();
            
        }
        else if(block == buttonDirType)
        {

        }
    }
    catch(exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

void EditTrimCurves::simplifySpline()
{
    if (m_allTrimCurves.size() == 0) return;
    if (m_cutDir.Z < 0)
        UF_VEC3_negate((double*)&m_cutDir, (double*)&m_cutDir);
    double pt_[3];
    MyFun::getCurvePt(m_pTrimCurves[0]->curve, 0.5, pt_);
    MyFun::getDirectionPos((double*)&m_cutDir, pt_, 50, pt_);
    MyFun::projectOperation(pt_, (double*)&m_cutDir, m_allTrimCurves);

    vector<tag_t> tempVVV;

    int num = 0;
    tag_p_t segments = NULL;
    tag_t tempT = 0;
    int type_, subType;

    for (int i = 0; i < m_allTrimCurves.size(); i++)
    {
        UF_OBJ_ask_type_and_subtype(m_allTrimCurves[i], &type_, &subType);
        if (type_ == UF_line_type || type_ == UF_circle_type)
            tempVVV.push_back(m_allTrimCurves[i]);
        else
        {
            tempT = m_allTrimCurves[i];
            UF_CURVE_create_simplified_curve(1, &tempT, 0.0254, &num, &segments);
            UF_OBJ_delete_object(tempT);
            for (int j = 0; j < num; j++)
                tempVVV.push_back(segments[j]);
            if (segments)
            {
                UF_free(segments);
                segments = NULL;
            }
        }
    }
    deletePointerSet(m_pTrimCurves);

    m_allTrimCurves = tempVVV;

    for (int i = 0; i < m_allTrimCurves.size(); i++)
        m_pTrimCurves.push_back(new CurveData(m_allTrimCurves[i])); 
    ArcTranslateLine(m_trimCurvesGroupUP);
    sortCurvesPointor(m_pTrimCurves, m_trimCurvesGroupUP);
    
    stdProcessCurves(m_trimCurvesGroupUP);
    stdProcessCurves(m_trimCurvesGroupUP);

    m_allTrimCurves.clear();
    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)
    {
        for (int j = 0; j < m_trimCurvesGroupUP[i].size(); j++)
        {
            m_allTrimCurves.push_back(*(m_trimCurvesGroupUP[i][j]));
        }
    }
}

bool EditTrimCurves::hasSpline()
{
    m_hasSpline = false;
    for (int i = 0; i < m_pTrimCurves.size(); i++)//hasSpline?
        if (m_pTrimCurves[i]->type_ == UF_spline_type)
        {
            m_hasSpline = true;
            break;
        }
    return m_hasSpline;
}

int EditTrimCurves::apply_cb()
{
    int errorCode = 0;
    try
    {
        deletePointerSet(m_pTrimCurves);
        deletePointerSet(m_pAssistCurves);
        deletePointerSet(m_assistCurvesTempGroup);
    }
    catch(exception& ex)
    {

        errorCode = 1;
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

int EditTrimCurves::Show()
{
    try
    {
        theDialog->Show();
    }
    catch (exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

void EditTrimCurves::dialogShown_cb()
{
    try
    {

    }
    catch(exception& ex)
    {

        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

int EditTrimCurves::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(exception& ex)
    {
        errorCode = 1;
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

void EditTrimCurves::GenerateAssistLines()
{
    cutDir->SetEnable(false);
    deletePointerSet(m_pAssistCurves);
    deletePointerSet(m_assistCurvesTempGroup);
    TrimCurveData tcd = { 0 };
    int count_;
    int index_= m_trimCurvesGroupUP.size();
    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)//上模
    {
        index_ = BYTE_4(m_pAssistCurves.size());
        autoCloseAssistCurves(m_trimCurvesGroupUP[i], m_cutDir, 40, m_pAssistCurves, count_);
        tcd.assist_ = new vector<CurveData*>();
        for (int j = 0; j < count_; j++)
        {
            tcd.assist_->push_back(m_pAssistCurves[j + index_]);
            m_allTrimCurves.push_back(*(m_pAssistCurves[j + index_]));
        }

        {
            m_pAssistCurves.push_back(new CurveData(m_trimCurvesGroupUP[i][0]->curve));
            m_pAssistCurves.push_back(new CurveData(m_trimCurvesGroupUP[i].back()->curve));
        }
        tcd.trim_ = &m_trimCurvesGroupUP[i];
        m_trimCurveDataUP.push_back(tcd);
        m_assistCurvesTempGroup.push_back(tcd.assist_);
    }
    //在函数内写属性
    m_attr.writeAttrToCurves(m_trimCurveDataUP, CurveAttrE::UPType);


    std::vector<vector<CurveData*>> m_trimCurvesGroupDW;
    vector<CurveData*>* tempCDVSET = new vector<CurveData*>[m_trimCurvesGroupUP.size()];


    if (m_cutDir.Z > 0)
        UF_VEC3_negate((double*)&m_cutDir, (double*)&m_cutDir);

    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)
    {
        CurveData* tempCDPSET = new CurveData[m_trimCurvesGroupUP[i].size()];//
        for (int j = 0; j < m_trimCurvesGroupUP[i].size(); j++)
        {
            tempCDPSET[j].setData(MyFun::moveObj(m_trimCurvesGroupUP[i][j]->curve, m_cutDir, 100, false));
            tempCDVSET[i].push_back(&tempCDPSET[j]);
        }
        vector<vector<CurveData*>> rt_simplePro;
        sortCurvesPointor(tempCDVSET[i], rt_simplePro);
        if (rt_simplePro.size() != 1)
            SHOW_INFO_USR("排序处理异常！");
        m_trimCurvesGroupDW.push_back(rt_simplePro[0]);
    }

    for (int i = 0; i < m_trimCurvesGroupDW.size(); i++)//下模
    {
        index_ = index_ = BYTE_4(m_pAssistCurves.size());
        autoCloseAssistCurves(m_trimCurvesGroupDW[i], m_cutDir, 40, m_pAssistCurves, count_, 0);
        tcd.assist_ = new vector<CurveData*>();
        for (int j = 0; j < count_; j++)
        {
            m_allTrimCurves.push_back(*(m_pAssistCurves[j + index_]));
            tcd.assist_->push_back(m_pAssistCurves[j + index_]);
        }
        {
            m_pAssistCurves.push_back(new CurveData(m_trimCurvesGroupDW[i][0]->curve));
            m_pAssistCurves.push_back(new CurveData(m_trimCurvesGroupDW[i].back()->curve));
        }
        tcd.trim_ = &m_trimCurvesGroupDW[i];
        m_trimCurveDataDW.push_back(tcd);
        m_assistCurvesTempGroup.push_back(tcd.assist_);
    }
    m_attr.writeAttrToCurves(m_trimCurveDataDW, CurveAttrE::DwType);
    m_trimCurveDataDW.clear();
    m_trimCurveDataUP.clear();
    //输入参数需要在函数结束时释放，必须在函数内写属性

    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)
        delete[] tempCDVSET[i][0];
    delete[] tempCDVSET;
}

PropertyList* EditTrimCurves::GetBlockProperties(const char *blockID)
{
    return theDialog->GetBlockProperties(blockID);
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    EditTrimCurves *theEditTrimCurves = NULL;
    try
    {
        theEditTrimCurves = new EditTrimCurves();
        theEditTrimCurves->Show();
    }
    catch (exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    if (theEditTrimCurves != NULL)
    {
        delete theEditTrimCurves;
        theEditTrimCurves = NULL;
    }
}

extern "C" DllExport int ufusr_ask_unload()
{
    return (int)Session::LibraryUnloadOptionImmediately;
}

extern "C" DllExport void ufusr_cleanup(void)
{
    try
    {

    }
    catch (exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}