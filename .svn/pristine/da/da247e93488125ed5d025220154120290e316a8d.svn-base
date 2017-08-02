#include "RetOffsetSheet.h"


static bool hasData(tag_t &obj)
{
    int nItem = -1;
    UF_ATTR_count_attributes(obj, UF_ATTR_any, &nItem);
    if (nItem > 0)
        return true;
    else
        return false;
}

inline int findData(tag_t obj, char * title, int type/*= UF_ATTR_string*/)
{
    UF_INITIALIZE();
    int title_type;
    UF_ATTR_find_attribute(obj, type, title, &title_type);
    return title_type;
}

static bool writeData(tag_t obj, char* title, int type, void * attr)
{
    UF_ATTR_value_t values;
    UF_ATTR_find_attribute(obj, type, title, &values.type);
    if (values.type)
        return false;
    values.type = type;
    memset(&values.value, 0, sizeof(values.value));
    if (type == UF_ATTR_bool)
        memcpy(&values.value, attr, 1);
    else if (type == UF_ATTR_string || type == UF_ATTR_real || type == UF_ATTR_time || type == UF_ATTR_reference)
        memcpy(&values.value, attr, 8);
    else if (type == UF_ATTR_integer)
        memcpy(&values.value, attr, 4);
    UF_CALL(UF_ATTR_assign(obj, title, values));
    return true;
}

static UF_ATTR_value_s readData(tag_t obj, char * title, int type /*= UF_ATTR_string*/) //c++

{
    UF_ATTR_value_s  ret = { 0 };
    UF_ATTR_read_value(obj, title, type, &ret);
    return ret;
}


static bool setSheetInfo(tag_t obj_tag, const char* str, double centerPt[3], double offsetDist,double* dir, double moveidst=0)
{//name : XXXXCenterX.....Y.....Z.....OFF....DirX.....Y.....Z.....DIST...
    UF_INITIALIZE()
        MyFun::CSolidObj  obj(obj_tag);
    char timeName[UF_OBJ_NAME_LEN + 1] = {0};
    sprintf(timeName, "%sCenterX%04.5lfY%04.5lfZ%04.5lfOFF%04.5lfDirX%04.5lfY%04.5lfZ%04.5lfDIST%04.5lf", 
        str,centerPt[0], centerPt[1], centerPt[2],offsetDist, dir[0],dir[1],dir[2], moveidst);
    obj.SetName(timeName);
    return false;
}

static tag_t readSheetInfo(tag_t sheets, double *centerPt, double& offsetDist,double *dir, double& moveDist)
{
    UF_INITIALIZE();
    //ORGCenterX.....Y.....Z.....OFF....DirX.....Y.....Z.....DIST...
    //OFFCenterX.....Y.....Z.....OFF....DirX.....Y.....Z.....DIST...
    MyFun::CSolidObj  obj(sheets);
    sscanf(obj.GetName(), "%*[^X]X%lf%Y%lf%Z%lf%*[OFF]%lf%*[^X]X%lf%Y%lf%Z%lf%*[DIST]%lf",
        &centerPt[0], &centerPt[1], &centerPt[2],&offsetDist, &dir[0], &dir[1], &dir[2], &moveDist);
    return 0;
}

size_t RetOffsetSheet::getAllSheets()
{
    UF_initialize();
    int subtype = 0;
    int type = 0;
    m_allSheets.clear();
    tag_t obj = NULL_TAG;
    int layer_ = 0;
    UF_LAYER_ask_work_layer(&layer_);
    UF_LAYER_cycle_by_layer(layer_, &obj);
    bool flag = false;
    if (obj == NULL_TAG)
    {
        flag = true;
        UF_LAYER_cycle_by_layer(256, &obj);
    }

    //UF_VIEW_cycle_objects(0, UF_VIEW_VISIBLE_OBJECTS, &obj);
    char name_[133] = { 0 };
    double pt[3];
    while (NULL_TAG != obj)
    {
        UF_OBJ_ask_type_and_subtype(obj, &type, &subtype);
        if (type == 70 && subtype == 0)
        {
            UF_MODL_ask_body_type(obj, &subtype);
            if (subtype == UF_MODL_SHEET_BODY)
            {
                if (readCenter(obj, pt))
                {
                    if (MyFun::is_Equal(pt,m_centerPt,0.001))
                        m_allSheets.push_back(obj);
                }
            }
        }
        if (flag == false)
        {
            UF_LAYER_cycle_by_layer(layer_, &obj);
            if (obj == NULL_TAG)
            {
                flag = true;
                UF_LAYER_cycle_by_layer(256, &obj);
            }
        }
        else
        {
            flag = true;
            UF_LAYER_cycle_by_layer(256, &obj);
        }
        //UF_VIEW_cycle_objects(0, UF_VIEW_VISIBLE_OBJECTS, &obj);
    }
    return m_allSheets.size();
}

RetOffsetSheet::RetOffsetSheet(std::vector<tag_t> sheets, double offsetDist, double offsetDeviation /*= 0*/, 
    double stepoverDeviation /*= 0*/, bool isMove, double *moveDir/*=NULL*/, double moveDist/*=0*/)
{
    //sheets个数大于1，等于1不同处理
    UF_initialize();
    if (sheets.size() <=1)
    {
        if (sheets.size()==0)
        {
            return;
        }
        m_targerSheet = sheets[0];
        if (is_orgSheet(m_targerSheet))
        {
            readCenter(m_targerSheet,m_centerPt);
            getAllSheets();
        }
        else if (is_offSheet(m_targerSheet))
        {
            readCenter(m_targerSheet, m_centerPt);
            getAllSheets();
            for (int i = 0; i < m_allSheets.size(); i++)
            {
                if (is_orgSheet(m_allSheets[i]))
                {
                    m_targerSheet = m_allSheets[i];
                    break;
                }
            }
            m_retSheet = retOffsetShet(offsetDist, moveDir, moveDist/*= 0*/,isMove, offsetDeviation /*= 0*/, stepoverDeviation /*= 0*/);
        }
        else
        {
            MyFun::CSolidObj temp(m_targerSheet);
            UF_VEC3_midpt(temp.m_topCenter, temp.m_bottomCenter, m_centerPt);
            setORGSheetInfo();
            m_allSheets.push_back(m_targerSheet);
            if (fabs(offsetDist) > 0.01)
            {
                m_retSheet = offsetSheet(m_targerSheet, offsetDist, offsetDeviation, stepoverDeviation);
                setOffSheetInfo(offsetDist);
                m_allSheets.push_back(m_retSheet);
            }
           
            if (moveDir&&moveDist)
            {
                if (isMove)
                {
                     m_retSheet = MyFun::moveObj(m_retSheet, moveDir, moveDist, false);
                     setSheetInfo(m_retSheet, OFF, m_centerPt, offsetDist, moveDir, moveDist);
                     m_allSheets.push_back(m_retSheet);
                }
            }
        }
    }
    else
    {
        m_targerSheet = sewSheets(sheets);
        MyFun::CSolidObj temp(m_targerSheet);
        UF_VEC3_midpt(temp.m_topCenter, temp.m_bottomCenter, m_centerPt);
        setORGSheetInfo();
        m_allSheets.push_back(m_targerSheet);
        if (fabs(offsetDist) > 0.01)
        {
            m_retSheet = offsetSheet(m_targerSheet, offsetDist, offsetDeviation, stepoverDeviation);
            setOffSheetInfo(offsetDist); 
            m_allSheets.push_back(m_retSheet);
        }
       
        if (moveDist&&moveDir)
        {
            if (isMove)
            {
                m_retSheet = MyFun::moveObj(m_retSheet, moveDir, moveDist, false);
                setSheetInfo(m_retSheet, OFF, m_centerPt, offsetDist, moveDir, moveDist);
                m_allSheets.push_back(m_retSheet);
            }
        }
    }
}

tag_t RetOffsetSheet::retOffsetShet(double offsetDist,double *moveDir,double moveDist/*= 0*/, bool isMove, double v1 /*= 0*/, double v2 /*= 0*/)
{
    if (has_offsetSheet(offsetDist, isMove, moveDir,moveDist))
        return m_retSheet;
    else
    {
        m_retSheet = offsetSheet(m_targerSheet, offsetDist, v1, v2);
        setOffSheetInfo(offsetDist);
        m_allSheets.push_back(m_retSheet);
        if (moveDist&&moveDir)
        {
            if (isMove)
            {
                m_retSheet = MyFun::moveObj(m_retSheet, moveDir, moveDist, false);
                setSheetInfo(m_retSheet, OFF, m_centerPt, offsetDist, moveDir, moveDist);
                m_allSheets.push_back(m_retSheet);
            }
        }
        return m_retSheet;
    }
}

bool RetOffsetSheet::has_offsetSheet(double offsetDist,bool isMove, double *moveDir, double moveDist)
{
    double offsetDist_;
    double dir_[3];
    double moveDist_;
    m_retSheet = 0;
    for (int i = 0; i < m_allSheets.size(); i++)
    {
        readOffsetSheetInfo(m_allSheets[i], offsetDist_, dir_, moveDist_);
        if (fabs(offsetDist_ - offsetDist) < 0.001)
        {
            if (isMove)
            {
                if (MyFun::is_Equal(dir_, moveDir, 0.001)&& fabs(moveDist_ - moveDist) < 0.001)
                {
                    m_retSheet = m_allSheets[i];
                    break;
                }
            }
            else
            {
                m_retSheet = m_allSheets[i];
                break;
            }
        }
    }
    if (m_retSheet)
        return true;
    else
        return false;
}

void RetOffsetSheet::readOffsetSheetInfo(tag_t obj, double &offsetDist, double* moveDir, double& moveDist)
{
    UF_INITIALIZE();
    double pt[3];
    double dir[3];
    if (moveDir==NULL) moveDir = dir;
    if (is_offSheet(obj) || is_orgSheet(obj))
    {
        readSheetInfo(obj, pt, offsetDist, moveDir, moveDist);
    }
    else
    {
        offsetDist = 0;
        moveDist= 0;
        memset(moveDir, 0, 3 * sizeof(double));
    }
}

void RetOffsetSheet::setOffSheetInfo(double offsetDistN)
{
    double dir[3] = {0};
    setSheetInfo(m_retSheet, OFF,m_centerPt, offsetDistN, dir,0.0);
}
//缝合的片体或者单个原始只存储 中心点，其他信息为0
void RetOffsetSheet::setORGSheetInfo()
{
    double dir[3] = { 0 };
    setSheetInfo(m_targerSheet, ORG, m_centerPt,0.0, dir,0.0);
}

bool RetOffsetSheet::readCenter(tag_t obj,double *center)
{
    UF_INITIALIZE();
    if (!(is_offSheet(obj) || is_orgSheet(obj)))
    {
        memset(center, 0, sizeof(double)*3);
        return false;
    }
    double offset;
    double dir[3];
    double moveDist;
    readSheetInfo(obj, center, offset,dir, moveDist);
    return true;
}

const char RetOffsetSheet::OFF[32] = "OFF";
const char RetOffsetSheet::ORG[32] = "ORG";