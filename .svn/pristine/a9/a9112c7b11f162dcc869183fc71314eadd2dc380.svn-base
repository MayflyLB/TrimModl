#ifndef OSHEETOPERATOR_H
#define OSHEETOPERATOR_H
#include "DataStruction.h"
#include "uf_part.h"
#include "uf_obj.h"
#include "uf_obj_types.h"
#include "uf_layer.h"
#include "../../Include/PublicFun.h"
#include "RetOffsetSheet.h"
#include "../Include/CurveAttrE.h"

class DllExport CommonOperation : public CommonTraits::Struct_Type
{
    typedef CommonTraits::Struct_Type Struct_Type;
public:

    CommonOperation(const CommonTraits::Struct_Type&);
    //释放指针指向的资源.
    ~CommonOperation();

    void operator()(const CurveAttrE& temp)
    {
        creataDST(temp(0));
        creataUST(temp());
        MyFun::resetUpdata();
        temp.writeOld();
    }

private:
    void creataDST(const vector<TrimCurveData> &m_CurveDataDw)
    {
       
        for (int i=0;i<m_CurveDataDw.size();i++)
        {
            if (m_CurveDataDw[i].trim_->size())
            {
                getCurvesInfo(m_CurveDataDw[i]);
                createBodyDST();
                createDetailDST();
            }
        }
        return;
    }
    void creataUST(const vector<TrimCurveData> &m_CurveDataUp)
    {
        for (int i = 0; i < m_CurveDataUp.size(); i++)
        {
            if (m_CurveDataUp[i].trim_->size())
            {
                getCurvesInfo(m_CurveDataUp[i]);
                createBodyUST();
                createDetailUST();
            }
        }
        return;
    }
    void getCurvesInfo(const TrimCurveData& info);
    void createBodyDST();
    void createBodyUST();

    void createDetailDST();
    void createDetailUST();
        //1下模 0上模
    void createShapeBlank(int col1, int col2, int flagUD = 1);
    void extrudBlankBody(tag_t& extrud, tag_t sheetTag);//创建坯体 flag位true时,
    void extrudBlankBodyUDST(tag_t& extrud, tag_t sheetTag);

    tag_t createExtrudeBodyEx(const vccdata &vcc, double thiness, double ext = 0);  //切削方向创建拉身体

    void offsetFace(tag_t face_t, double dist);//TODO

    void extractFaces(tag_t extrud, vector<tag_t>&sideFaces, vector<tag_t>&cutShapeFaces,
        vector<tag_t>&floorFaces,vector<tag_t>&backBoard, int col[4]);//提取毛坯的面底面 付型面
    void extrudeVertex(const vccdata &vcc, bool flag, double len = 100);//延长修边线的两端线

    vector<tag_t> BooleanOper(tag_t& extrud, tag_t sheetsSet, tag_t flagT = 0, bool is_sub = true, bool is_del = true,
        bool is_ThrowError = false, bool is_keepTarget = false, bool is_keepTool = true);

    vector<tag_t> BooleanOperUDC_ST(tag_t& extrud, tag_t flagT, tag_t sheetsSet, bool flag = true, bool isSetEx = false);

    tag_t createOffsetSheetEx(vccdata vcc);

    //过滤出法向和z轴负方向成角度小于90度的面
    void filterZFace(vector<tag_t>&faces);

    void finishTool(tag_t face, vector<tag_t>& bodys_);

    void splinesProcessToLinesDUST(vector<tag_t> &tempSplines);

private:
    Session *theSession;
    Part *workPart;

    std::vector<CurveData> m_assistLD;//辅助线
    std::vector<CurveData> m_trimLD;//修边线
    std::vector<vccdata> m_trimVCC;//分组修边线

    double m_minDist;//修边线于目标片体最短距离
    tag_t m_extrud;

    ////辅助变量
    tag_t sheetTag;
    RetOffsetSheet *m_retOffsetSheet;
    vector<tag_t> m_tempObj;
    vector<tag_t> m_tempBody;
};

#endif // !SHEETOPERATOR_H
