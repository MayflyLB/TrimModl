#ifndef _CURTTERATTRE_H
#define _CURTTERATTRE_H
#include "../Include/assistFun.h"
struct  TrimCurveData   //一级指针有内容！！！
{
    vector<CurveData*>* trim_;
    vector<CurveData*>* assist_;
};

class CurveAttrE
{
public:
    enum CurveType
    {
        UPType,
        DwType
    };

    CurveAttrE(){ srand((unsigned)time(NULL)); }
    ~CurveAttrE()
    {
        delelteCurveData();
    }
    void writeAttrToCurves(std::vector<TrimCurveData>& curveSet, CurveType cae);

    const vector<TrimCurveData>& retUpCurveData()const 
    {
        return m_CurveDataUp;
    }
    const vector<TrimCurveData>& retDwCurveData()const 
    {
        return m_CurveDataDw;
    }
    //上模修边线
    const vector<TrimCurveData>&operator()()const 
    {
        return m_CurveDataUp;
    }

    //下模修边线
    const vector<TrimCurveData>&operator()(int a)const
    {
        return m_CurveDataDw;
    }
    //处理完成 做旧


    inline void getTrimCurves()
    {
        getAllTrimCurves();
        getAtrrToCurves();
    }
    inline void getTrimCurves(const vector<NXOpen::TaggedObject *>&temp)
    {
        getCurrentTrimCurves(temp);
        getAtrrToCurves();
    }
    inline bool isSpecifyCurve(tag_t obj)
    {
        return hasTheAttr(obj, curveTypeTitle, UF_ATTR_string);
    }
    inline void readTrimCurves()
    {

    }
    void writeOld()const
    {
        writeOld(m_CurveDataUp);
        writeOld(m_CurveDataDw);
    }
private:
    void writeOld(std::vector<TrimCurveData>& curveSet)const;
    inline bool hasTheAttr(tag_t obj, const char*TypeTitle,int type_)
    {
        int ret;
        UF_ATTR_find_attribute(obj, type_, const_cast<char*>(curveTypeTitle), &ret);
        return type_ == ret;
    }
    inline void getCurrentTrimCurves(const vector<NXOpen::TaggedObject *>&temp)
    {
        m_tempCurves.clear();
        for (int i=0;i<temp.size();i++)
            m_tempCurves.push_back(temp[i]->GetTag());
    }

    size_t getAllTrimCurves();
    void getAtrrToCurves();
    void delelteCurveData();
private:
    mutable vector<TrimCurveData> m_CurveDataUp;//一级指针有内容！！！
    mutable vector<TrimCurveData> m_CurveDataDw;//一级指针有内容！！！
    vector<tag_t> m_tempCurves;

    vector<CurveData> m_allTrimCurves;
    static const char curveTypeTitle[64];//UF_ATTR_string
    static const char UPAssist[64];
    static const char UPTrime[64];
    static const char DwAssist[64];
    static const char DwTrim[64];
    static const char GroupFlagTitle[64];//UF_ATTR_integer
};




#endif
