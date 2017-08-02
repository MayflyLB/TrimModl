#include "ImplEditCurves.h"



ImplEditCurves::ImplEditCurves(NXOpen::BlockStyler::CurveCollector* curvesSelect, NXOpen::BlockStyler::SpecifyVector* cutDir)
:m_uiCurvesSelect(curvesSelect),m_uiCutDir(cutDir)
{
    m_index = 0;
    m_pCur_CD = NULL;
    m_pPost_CD = NULL;
    m_hasSpline = false;
    m_tempCDVSET = nullptr;
    m_tempTrimCurves.reserve(256);
    m_tempTrimCurves.clear();              //need delete
    m_allTrimCurves.clear();
    m_trimCurvesGroupUP.clear();        //do not delete
    m_tempAssistCurves.clear();            //need delete
    m_assistCurvesTempGroup.clear();
    m_cutDir.setValue(NULL);
    m_hasSpline = false;
    m_color = 0;
    m_editLines = nullptr;
}

ImplEditCurves::~ImplEditCurves()
{
    if (m_editLines)
    {
        delete m_editLines;
        m_editLines = nullptr;
    }
    deleteTempCDVSET();
    deletePointerSet(m_generateDirs);
    deletePointerSet(m_tempTrimCurves);
    deletePointerSet(m_tempAssistCurves);
    deletePointerSet(m_tempCurves);
    
    deletePointerSet(m_assistCurvesTempGroup);
    DELETE_VECTOR_OBJS(m_tempSplines);
    

}

void ImplEditCurves::sortedArcsLines()
{
    deletePointerSet(m_tempTrimCurves);
    deletePointerSet(m_tempAssistCurves);
    deletePointerSet(m_assistCurvesTempGroup);
    DELETE_VECTOR_OBJS(m_tempSplines);
    std::vector<NXOpen::TaggedObject *>&&temp = m_uiCurvesSelect->GetSelectedObjects();
    if (temp.size() == 0)
    {
        SHOW_INFO_USR("修边线未选择！\n");
        return;
    }
    for (int i = 0; i < temp.size(); i++)
    {
        m_allTrimCurves.push_back(temp[i]->GetTag());
        m_tempTrimCurves.push_back(new CurveData(m_allTrimCurves.back()));
    }
    m_cutDir = m_uiCutDir->Vector();
    if (!hasSpline() && m_allTrimCurves.size() && !(is_3DCurve(m_allTrimCurves)))
        sortCurvesPointor(m_tempTrimCurves, m_trimCurvesGroupUP,true);
    else if (m_cutDir.Z != 0 && m_allTrimCurves.size())
        simplifySpline();
    else
        SHOW_INFO_USR("修边线处理异常！\n");
    m_uiCurvesSelect->SetEnable(false);
    autoDirBing();

    GenerateAssistLines();
}

void ImplEditCurves::adjustGenerateCurves(int i, Vector3d genDir, bool isCoverUp/*=false*/)
{
    //m_trimCurveDataUP m_trimCurveDataDW
    int count_;
    int index_ = BYTE_4(m_tempAssistCurves.size());
    autoCloseAssistCurves(m_trimCurvesGroupUP[i], m_cutDir, 40, m_tempAssistCurves, genDir, isCoverUp ? 0 : 1);
    count_ = BYTE_4(m_tempAssistCurves.size() - index_);
    for (int n = 0; n < m_trimCurveDataUP[i].assist_->size(); n++)
        DELETE_OBJ((*m_trimCurveDataUP[i].assist_)[n]->curve);
    m_trimCurveDataUP[i].assist_->clear();
    for (int j = 0; j < count_; j++)
        m_trimCurveDataUP[i].assist_->push_back(m_tempAssistCurves[j + index_]);
    setCurvesCol(*m_trimCurveDataUP[i].assist_, 134);
    index_ = BYTE_4(m_tempAssistCurves.size());
    autoCloseAssistCurves(m_trimCurvesGroupDW[i], m_cutDir, 40, m_tempAssistCurves, genDir, isCoverUp ? 1 : 0);

    count_ = BYTE_4(m_tempAssistCurves.size() - index_);
    for (int n = 0; n < m_trimCurveDataDW[i].assist_->size(); n++)
        DELETE_OBJ((*m_trimCurveDataDW[i].assist_)[n]->curve);
    m_trimCurveDataDW[i].assist_->clear();
    for (int j = 0; j < count_; j++)
        m_trimCurveDataDW[i].assist_->push_back(m_tempAssistCurves[j + index_]);
    setCurvesCol(*m_trimCurveDataDW[i].assist_, 134);
}

void ImplEditCurves::autoDirBing()
{
    double magnitude;
    double dirAngular[3];
    deletePointerSet(m_generateDirs);
    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)
    {
        if (MyFun::is_Equal(m_trimCurvesGroupUP[i][0]->end_point, m_trimCurvesGroupUP[i][1]->start_point, 0.001))
            m_trimCurvesGroupUP[i][0]->swap();
        UF_VEC3_unitize(m_trimCurvesGroupUP[i][0]->dir_Center, 0.000001, &magnitude, m_trimCurvesGroupUP[i][0]->dir_Center);
        UF_VEC3_unitize(m_trimCurvesGroupUP[i].back()->dir_Center, 0.000001, &magnitude, m_trimCurvesGroupUP[i].back()->dir_Center);
        int status;
        UF_VEC3_is_parallel(m_trimCurvesGroupUP[i][0]->dir_Center, m_trimCurvesGroupUP[i].back()->dir_Center, 0.001, &status);
        if (status == 0 || m_trimCurvesGroupUP[i][0]->dir_Center == m_trimCurvesGroupUP[i].back()->dir_Center)
        {
            UF_VEC3_add(m_trimCurvesGroupUP[i][0]->dir_Center, m_trimCurvesGroupUP[i].back()->dir_Center, dirAngular);
        }
        else
        {
            UF_VEC3_cross(m_trimCurvesGroupUP[i][0]->dir_Center, m_cutDir, dirAngular);
        }
        m_generateDirs.push_back(new GenerateDir(dirAngular, m_trimCurvesGroupUP[i][0]->vertexPt));
        if (MyFun::is_Equal(m_trimCurvesGroupUP[i][0]->end_point, m_trimCurvesGroupUP[i][1]->start_point, 0.001))
            m_trimCurvesGroupUP[i][0]->swap();
    }
}

template<class T>
void ImplEditCurves::deletePointerSet(vector<T*>&temp)
{
    for (int i = 0; i < temp.size(); i++)
        delete temp[i];
    temp.clear();
}

bool ImplEditCurves::hasSpline()
{
    m_hasSpline = false;
    for (int i = 0; i < m_tempTrimCurves.size(); i++)//hasSpline?
        if (m_tempTrimCurves[i]->type_ == UF_spline_type)
        {
            m_hasSpline = true;
            break;
        }
    return m_hasSpline;
}

void ImplEditCurves::simplifySpline()
{
    if (m_allTrimCurves.size() == 0) return;
    if (m_cutDir.Z < 0)
        UF_VEC3_negate((double*)&m_cutDir, (double*)&m_cutDir);
    double pt_[3];
    MyFun::getCurvePt(m_tempTrimCurves[0]->curve, 0.5, pt_);
    MyFun::getDirectionPos((double*)&m_cutDir, pt_, 100, pt_);
    MyFun::projectOperation(pt_, (double*)&m_cutDir, m_allTrimCurves);
    std::vector<vector<CurveData*>> tempCruveGroup;
    std::vector<CurveData*> tempCurveDatas;

    {//排除重复的曲线
        for (int i = 0; i < m_allTrimCurves.size(); i++)
        {
            tempCurveDatas.push_back(new CurveData(m_allTrimCurves[i]));
        }
        sortCurvesPointor(tempCurveDatas, tempCruveGroup, true);
        m_allTrimCurves.clear();
        for (int i = 0; i < tempCruveGroup.size(); i++)
            for (int j = 0; j < tempCruveGroup[i].size(); j++)
                m_allTrimCurves.push_back(tempCruveGroup[i][j]->curve);

        tempCruveGroup.clear();
        deletePointerSet(tempCurveDatas);
        deletePointerSet(m_tempTrimCurves);
    }

    joinSpline(m_allTrimCurves);

    int num = 0;
    tag_p_t segments = NULL;
    tag_t tempT = 0;
    int type_, subType;

    vector<tag_t> tempCurves;
    vector<tag_t> tempCurves_;
    CurveData*cd;
    m_tempSplines.clear();
    m_trimCurvesGroupUP.clear();
    m_tempSplines.clear();
    for (int i = 0; i < m_allTrimCurves.size(); i++)
    {
        UF_OBJ_ask_type_and_subtype(m_allTrimCurves[i], &type_, &subType);
        if (type_ == UF_line_type || type_ == UF_circle_type)
            tempCurves_.push_back(m_allTrimCurves[i]);
        else
        {
            tempT = m_allTrimCurves[i];
            UF_CURVE_create_simplified_curve(1, &tempT, 0.0254, &num, &segments);

            tempCurves.clear();
            for (int j = 0; j < num; j++)
                tempCurves.push_back(segments[j]);
            if (segments)
            {
                UF_free(segments);
                segments = NULL;
            }
            tempCurveDatas.clear();
            tempCruveGroup.clear();
            for (int n = 0; n < tempCurves.size(); n++)
            {
                cd = new CurveData(tempCurves[n]);
                m_tempTrimCurves.push_back(cd);
                tempCurveDatas.push_back(cd);
            }
            sortCurvesPointor(tempCurveDatas, tempCruveGroup);
            m_trimCurvesGroupUP.push_back(tempCruveGroup[0]);
            m_tempSplines.push_back(m_allTrimCurves[i]);
            UF_OBJ_set_color(m_allTrimCurves[i], 186);
        }
    }
    if (tempCurves_.size())
    {
        tempCurveDatas.clear();
        tempCruveGroup.clear();
        for (int i = 0; i < tempCurves_.size(); i++)
        {
            cd = new CurveData(tempCurves_[i]);
            m_tempTrimCurves.push_back(cd);
            tempCurveDatas.push_back(cd);
        }
        sortCurvesPointor(tempCurveDatas, tempCruveGroup);
        m_trimCurvesGroupUP.push_back(tempCruveGroup[0]);
        m_tempSplines.push_back(0);
    }
    ArcTranslateLine(m_trimCurvesGroupUP);
    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)
    {
        tempCruveGroup.clear();
        tempCruveGroup.push_back(m_trimCurvesGroupUP[i]);
        stdProcessCurves(tempCruveGroup, m_tempSplines[i]);
        stdProcessCurves(tempCruveGroup, m_tempSplines[i]);
        m_trimCurvesGroupUP[i] = tempCruveGroup[0];
    }
    ////////////////////
    m_allTrimCurves.clear();
    std::vector<CurveData*>tempSortCurves;
    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)
    {
        for (int j = 0; j < m_trimCurvesGroupUP[i].size(); j++)
        {
            m_allTrimCurves.push_back(*(m_trimCurvesGroupUP[i][j]));
            tempSortCurves.push_back(new CurveData(*(m_trimCurvesGroupUP[i][j])));
            m_tempCurves.push_back(tempSortCurves.back());
        }
    }
    m_trimCurvesGroupUP.clear();
    sortCurvesPointor(tempSortCurves, m_trimCurvesGroupUP);
}

void ImplEditCurves::GenerateAssistLines()
{
    m_uiCutDir->SetEnable(false);
    deletePointerSet(m_tempAssistCurves);
    deletePointerSet(m_assistCurvesTempGroup);
    deleteTempCDVSET();
    TrimCurveData tcd = { 0 };
    int count_;
    int index_;
    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)//上模
    {
        setCurvesCol(m_trimCurvesGroupUP[i],60);
        index_ = BYTE_4(m_tempAssistCurves.size());
        autoCloseAssistCurves(m_trimCurvesGroupUP[i], m_cutDir, 40, m_tempAssistCurves,*m_generateDirs[i], 1);
        tcd.assist_ = new vector<CurveData*>();
        count_ = BYTE_4(m_tempAssistCurves.size() - index_);
        for (int j = 0; j < count_; j++)
        {
            tcd.assist_->push_back(m_tempAssistCurves[j + index_]);
            m_allTrimCurves.push_back(*(m_tempAssistCurves[j + index_]));
        }
        
        tcd.trim_ = &m_trimCurvesGroupUP[i];
        m_trimCurveDataUP.push_back(tcd);
        m_assistCurvesTempGroup.push_back(tcd.assist_);
    }
    //在函数内写属性
    m_attr.writeAttrToCurves(m_trimCurveDataUP, CurveAttrE::UPType);


    
    m_tempCDVSET = new vector<CurveData*>[m_trimCurvesGroupUP.size()];


    if (m_cutDir.Z > 0)
        UF_VEC3_negate((double*)&m_cutDir, (double*)&m_cutDir);

    for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)
    {
        CurveData* tempCDPSET = new CurveData[m_trimCurvesGroupUP[i].size()];//
        for (int j = 0; j < m_trimCurvesGroupUP[i].size(); j++)
        {
            tempCDPSET[j].setData(MyFun::moveObj(m_trimCurvesGroupUP[i][j]->curve, m_cutDir, 200, false));
            m_tempCDVSET[i].push_back(&tempCDPSET[j]);
        }
        vector<vector<CurveData*>> rt_simplePro;
        sortCurvesPointor(m_tempCDVSET[i], rt_simplePro);
        if (rt_simplePro.size() != 1)
            SHOW_INFO_USR("排序处理异常！");
        m_trimCurvesGroupDW.push_back(rt_simplePro[0]);
    }

    for (int i = 0; i < m_trimCurvesGroupDW.size(); i++)//下模
    {
        setCurvesCol(m_trimCurvesGroupDW[i], 60);
        index_ = BYTE_4(m_tempAssistCurves.size());
        autoCloseAssistCurves(m_trimCurvesGroupDW[i], m_cutDir, 40, m_tempAssistCurves, *m_generateDirs[i], 0);

        tcd.assist_ = new vector<CurveData*>();
        count_ = BYTE_4(m_tempAssistCurves.size() - index_);
        for (int j = 0; j < count_; j++)
        {
            m_allTrimCurves.push_back(*(m_tempAssistCurves[j + index_]));
            tcd.assist_->push_back(m_tempAssistCurves[j + index_]);
        }

        tcd.trim_ = &m_trimCurvesGroupDW[i];
        m_trimCurveDataDW.push_back(tcd);
        m_assistCurvesTempGroup.push_back(tcd.assist_);
    }
    m_attr.writeAttrToCurves(m_trimCurveDataDW, CurveAttrE::DwType);
}

void ImplEditCurves::joinSpline(vector<tag_t> &temp)
{
    CurveData* x = new CurveData[temp.size()];
    std::vector<CurveData*> tempCurves;
    std::vector<std::vector<CurveData*>>tempGroups;
    for (int i = 0; i < temp.size(); i++)
    {
        x[i].setData(temp[i]);
        tempCurves.push_back(&x[i]);
    }
    sortCurvesPointor(tempCurves, tempGroups);

    temp.clear();
    std::vector<tag_t>tempv;
    tag_t * tempPointor;
    int count_;
    for (int i = 0; i < tempGroups.size(); i++)
    {
        tempPointor = new tag_t[tempGroups[i].size()];
        tempv.clear();
        for (int j = 0; j < tempGroups[i].size(); j++)
            tempv.push_back(tempGroups[i][j]->curve);

        UF_CALL(UF_CURVE_auto_join_curves(&tempv[0], BYTE_4(tempv.size()), 2, tempPointor, &count_));
        for (int n = 0; n < tempGroups[i].size(); n++)
            DELETE_OBJ(tempGroups[i][n]->curve);
        for (int i = 0; i < count_; i++)
            temp.push_back(tempPointor[i]);
        delete[] tempPointor;
    }
}
