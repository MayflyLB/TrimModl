#include "CurveAttrE.h"

const char CurveAttrE::curveTypeTitle[64] = "CurveType";//UF_ATTR_string
const char CurveAttrE::UPAssist[64] = "UPAssist";
static const char AngType[64] = "Processed";
const char CurveAttrE::UPTrime[64] = "UPTrime";
const char CurveAttrE::DwAssist[64] = "DwAssist";
const char CurveAttrE::DwTrim[64] = "DwTrim";
const char CurveAttrE::GroupFlagTitle[64] = "GroupFlag";//UF_ATTR_integer

void CurveAttrE::writeAttrToCurves(std::vector<TrimCurveData>& curveSet, CurveType cae)
{
    UF_ATTR_value_t attrValueTypeString;
    UF_ATTR_value_t attrValueGroupFlag;
    attrValueGroupFlag.type = UF_ATTR_integer;
    attrValueTypeString.type = UF_ATTR_string;
    UF_initialize();
    for (int i = 0; i < curveSet.size(); i++)
    {
        if (cae == UPType)
            attrValueTypeString.value.string = const_cast<char*> (UPTrime);
        else
            attrValueTypeString.value.string = const_cast<char*> (DwTrim);
        attrValueGroupFlag.value.integer = rand();
        for (int j = 0; j < curveSet[i].trim_->size(); j++)
        {
            UF_CALL(UF_ATTR_assign((*curveSet[i].trim_)[j]->curve, const_cast<char*>(curveTypeTitle), attrValueTypeString));
            UF_CALL(UF_ATTR_assign((*curveSet[i].trim_)[j]->curve, const_cast<char*>(GroupFlagTitle), attrValueGroupFlag));
        }
        if (cae == UPType)
            attrValueTypeString.value.string = const_cast<char*> (UPAssist);
        else
            attrValueTypeString.value.string = const_cast<char*> (DwAssist);
        for (int j = 0; j < curveSet[i].assist_->size(); j++)
        {
            UF_CALL(UF_ATTR_assign((*curveSet[i].assist_)[j]->curve, const_cast<char*>(curveTypeTitle), attrValueTypeString));
            UF_CALL(UF_ATTR_assign((*curveSet[i].assist_)[j]->curve, const_cast<char*>(GroupFlagTitle), attrValueGroupFlag));
        }
    }
}



void CurveAttrE::getAtrrToCurves()
{
    delelteCurveData();
    UF_initialize();
    UF_ATTR_value_t value = { 0 };
    vector<int> groupFlags;
    vector<vector<tag_t>> tempGroupcurve;
    int num;
    for (int i = 0; i < m_tempCurves.size(); i++) //ÏÈ·Ö×é
    {
        UF_ATTR_read_value(m_tempCurves[i], const_cast<char*>(GroupFlagTitle), UF_ATTR_integer, &value);
        for (num = 0; num < groupFlags.size(); num++)
            if (groupFlags[num] == value.value.integer)
                break;
        if (num == groupFlags.size())
        {
            groupFlags.push_back(value.value.integer);
            tempGroupcurve.push_back(vector<tag_t>());
        }
        tempGroupcurve[num].push_back(m_tempCurves[i]);
        memset(&value,0,sizeof(UF_ATTR_value_t));
    }
    vector<tag_t>trim_s;
    vector<tag_t>assist_s;
    TrimCurveData tctemp = { 0 };
    vector<CurveData*>* trim_p = new vector<CurveData*>[tempGroupcurve.size()];
    vector<CurveData*>* assist_p = new vector<CurveData*>[tempGroupcurve.size()];
    CurveData * tempCDs;

    for (int i = 0; i < tempGroupcurve.size(); i++)
    {
        trim_s.clear();
        assist_s.clear();

        for (int j = 0; j < tempGroupcurve[i].size(); j++)
        {
            UF_ATTR_read_value(tempGroupcurve[i][j], const_cast<char*>(curveTypeTitle), UF_ATTR_string, &value);
            if (value.value.string[2] == 'T')
                trim_s.push_back(tempGroupcurve[i][j]);
            else
                assist_s.push_back(tempGroupcurve[i][j]);
            if (value.value.string) UF_free(value.value.string);
            memset(&value, 0, sizeof(UF_ATTR_value_t));
        }
        tempCDs = new CurveData[trim_s.size()];
        for (int n = 0; n < trim_s.size(); n++)
        {
            tempCDs[n].setData(trim_s[n]);
            trim_p[i].push_back(&tempCDs[n]);
        }
        tempCDs = new CurveData[assist_s.size()];
        for (int n = 0; n < assist_s.size(); n++)
        {
            tempCDs[n].setData(assist_s[n]);
            assist_p[i].push_back(&tempCDs[n]);
        }
        tctemp.trim_ = &trim_p[i];
        tctemp.assist_ = &assist_p[i];
        UF_ATTR_read_value(tempGroupcurve[i][0], const_cast<char*>(curveTypeTitle), UF_ATTR_string, &value);
        if (value.value.string[0]=='U')
            m_CurveDataUp.push_back(tctemp);
        else
            m_CurveDataDw.push_back(tctemp);
         if (value.value.string) UF_free(value.value.string);
        memset(&value, 0, sizeof(UF_ATTR_value_t));
    }
}

void CurveAttrE::delelteCurveData()
{
    for (int i = 0; i < m_CurveDataUp.size(); i++)
    {
        delete[](*m_CurveDataUp[i].trim_)[0];
        delete[](*m_CurveDataUp[i].assist_)[0];
    }
    if (m_CurveDataUp.size())
    {
        delete[] m_CurveDataUp[0].trim_;
        delete[] m_CurveDataUp[0].assist_;
    }
    m_CurveDataUp.clear();
}

void CurveAttrE::writeOld(std::vector<TrimCurveData>& curveSet)
{
    UF_INITIALIZE();
    UF_ATTR_value_t attrValueTypeString;
    attrValueTypeString.type = UF_ATTR_string;
    attrValueTypeString.value.string = const_cast<char*> (AngType);
    for (int i = 0; i < curveSet.size(); i++)
    {
        for (int j = 0; j < curveSet[i].trim_->size(); j++)
            UF_CALL(UF_ATTR_assign((*curveSet[i].trim_)[j]->curve, const_cast<char*>(curveTypeTitle), attrValueTypeString));
        for (int j = 0; j < curveSet[i].assist_->size(); j++)
            UF_CALL(UF_ATTR_assign((*curveSet[i].assist_)[j]->curve, const_cast<char*>(curveTypeTitle), attrValueTypeString));
    }
}

size_t CurveAttrE::getAllTrimCurves()
{
    UF_initialize();
    int subtype = 0;
    int type = 0;
    tag_t obj = NULL_TAG;
    bool flag = false;
    UF_VIEW_cycle_objects(0, UF_VIEW_VISIBLE_OBJECTS, &obj);
    m_tempCurves.clear();
    UF_ATTR_value_t uavt = {0};
    while (NULL_TAG != obj)
    {
        UF_OBJ_ask_type_and_subtype(obj, &type, &subtype);
        if (type == UF_line_type ||type == UF_circle_type)
        {
            UF_ATTR_find_attribute(obj, UF_ATTR_string, const_cast<char*>(curveTypeTitle), &subtype);
            if (UF_ATTR_string == subtype)
            {
                UF_CALL(UF_ATTR_read_value(obj, const_cast<char*>(curveTypeTitle), UF_ATTR_string, &uavt));
                if (strncmp(uavt.value.string,AngType,5)!=0)    m_tempCurves.push_back(obj);
                if (uavt.value.string)  UF_free(uavt.value.string);
                memset(&uavt, 0, sizeof(UF_ATTR_value_t));
            }
                
        }
        UF_VIEW_cycle_objects(0, UF_VIEW_VISIBLE_OBJECTS, &obj);
    }
    return m_tempCurves.size();
}
