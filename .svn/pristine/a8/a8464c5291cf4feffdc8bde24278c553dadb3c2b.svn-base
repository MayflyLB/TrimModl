#ifndef _RETOFFSETSHEET_H
#define _RETOFFSETSHEET_H
#include "../Include/assistFun.h"

int findData(tag_t obj, char * title, int type/*= UF_ATTR_string*/);


//原片体用      ORGX ORGY ORGZ标题记录坐标
//偏执片体用    OFFX OFFY OFFZ标题记录坐标
class RetOffsetSheet//绝对坐标系下
{
public:
    RetOffsetSheet(std::vector<tag_t> sheets,double offsetDist, double offsetDeviation , 
        double stepoverDeviation ,bool isMove=false, double *moveDir=NULL,double movedist=0);

    tag_t retOffsetShet(double offsetDist,double *movedir,double movedist=0, bool isMove = false, double v1 = 0, double v2 = 0);
    ~RetOffsetSheet(){}
private:
    RetOffsetSheet(){}
    RetOffsetSheet(const RetOffsetSheet&){}

    bool has_offsetSheet(double offsetDist, bool isMove,double *dir,double movedist);

    void setORGSheetInfo();

    void setOffSheetInfo(double offsetDist);

    bool readCenter(tag_t obj,double *center);

    void readOffsetSheetInfo(tag_t obj,  double &offsetDist, double* moveDir, double& moveDist);
    size_t getAllSheets();
    bool is_orgSheet(tag_t obj)
    {
        MyFun::CSolidObj tempObj(obj);
        return strncmp(ORG, tempObj.GetName(), 3) == 0;
    }

    bool is_offSheet(tag_t obj)
    {
        MyFun::CSolidObj tempObj(obj);
        return strncmp(OFF, tempObj.GetName(),3) == 0;
    }




private:

    vector<tag_t> m_allSheets;
    tag_t m_targerSheet;
    tag_t m_retSheet;

    double m_centerPt[3];

    static const char OFF[32];
    static const char ORG[32];

//     static const char OFFX[32];
//     static const char OFFY[32];
//     static const char OFFZ[32];
//     static const char ORGX[32];
//     static const char ORGY[32];
//     static const char ORGZ[32];
//     static const char OffsetDist[32];

};



#endif // _RETOFFSETSHEET_H
