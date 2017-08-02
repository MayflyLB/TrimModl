#ifndef DATASTRUCTION_H
#define DATASTRUCTION_H
#include "uf_defs.h"
#include <vector>
#include "uf_vec.h"
#include "../include/assistFun.h"
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
    typedef struct stss
    {
        tag_t curve;
        double *start_point;//Param 0
        double *end_point;//Param 1
    } CurveData_;

    enum ModlType
    {
        ULTYPE = 0,
        UETYPE,
        DLTYPE
    };

    typedef	struct st1
    {
        vector<tag_t>sheetsTag;
        ModlType m_modlType;
        bool is_0ffsetColor;
        bool m_isInitial;//是否初始化

        bool        m_isMove;
        double      sheetMoveDist;
        Vector3d    moveDir;

        double      sheetOffsetDist;
        double      offsetDeviation;
        double      stepoverDeviation;

        double      intervalDist; //冲裁间隙

        double	toolHeight;         //最薄厚度H			40 ≤ H ≤ 45
        double	toolBreadth;        //最薄宽度B			B/H ≥ 1.2//abolish

        double	cutDirCutLen;       //刃口(冲裁方向)长度	15	|| =st_UpToolPlaneL
        double	cutDirToolBlank;    //刃口(冲裁方向)空挡  2	|| 0(不使用)

        double	cutShapeWide;       //符型宽度			20
        double	cutShapeBlank;      //符型避让			 2 为0冲头
        double  cutDirDeltaLen;     // 刃口在切削方向高度差的不能超过的最大值 ,太短付型面会有平面出现

        Vector3d    cutDir;         //冲裁方向 切削方向

        std::vector<tag_t> trimLtag;
        bool	isTrimLClose;       //修边线是否闭合

        int col[2];
    } CommonData;//UpperSteelInsertStruct

#ifdef __cplusplus
}
#endif
inline void adjCurveDataBack(CurveData& a)
{
    if (a.vertexPt[0] != a.end_point[0] || a.vertexPt[1] != a.end_point[1] || a.vertexPt[2] != a.end_point[2])
    {
        UF_VEC3_copy(a.end_point, a.start_point);
        UF_VEC3_copy(a.vertexPt, a.end_point);
    }
}

struct  CommonTraits
{
    typedef CommonData Struct_Type;

    static inline void initLocalDST(Struct_Type * s)
    {
        s->sheetsTag.clear();
        s->m_modlType=DLTYPE;
        s->m_isInitial=false;//是否初始化

        s->m_isMove = false;
        s->sheetMoveDist=350;
        s->moveDir.setValue(NULL);

        s->sheetOffsetDist = -5;
        s->offsetDeviation=4;
        s->stepoverDeviation=8;
        s->intervalDist=0.5;

        s->toolHeight = 40.0f;
        s->toolBreadth = 1.2f*s->toolHeight;
        s->cutDirCutLen = 10.0f;
        s->cutDirToolBlank = 2.0f;
        s->cutShapeWide = 20.0f;
        s->cutShapeBlank = 2.0f;
        s->cutDirDeltaLen = 200;

        s->cutDir.setValue(NULL);

        s->trimLtag.clear();
        s->isTrimLClose = false;	//默认修边线不是闭合的.
        s->col[0] = 129;
        s->col[1] = 161;
    }

    static inline void initLocalUST(Struct_Type * s)
    {
        s->sheetsTag.clear();
        s->m_modlType = ULTYPE;
        s->m_isInitial = false;//是否初始化

        s->m_isMove = false;
        s->sheetMoveDist = 350;
        s->moveDir.setValue(NULL);

        s->sheetOffsetDist = -5;
        s->offsetDeviation = 4;
        s->stepoverDeviation = 8;
        s->intervalDist = 0;

        s->toolHeight = 40.0f;
        s->toolBreadth = 1.2f*s->toolHeight;
        s->cutDirCutLen = 15.0f;
        s->cutDirToolBlank = 2.0f;
        s->cutShapeWide = 20.0f;
        s->cutShapeBlank = 2.0f;
        s->cutDirDeltaLen = 100;

        s->cutDir.setValue(NULL);


        s->trimLtag.clear();
        s->isTrimLClose = false;	//默认修边线不是闭合的.
        s->col[0] = 129;
        s->col[1] = 161;

    }

    static inline void initEntireUST(Struct_Type * s)
    {
        s->sheetsTag.clear();
        s->m_modlType = UETYPE;
        s->m_isInitial=false;//是否初始化

        s->m_isMove = false;
        s->sheetMoveDist = 350;
        s->moveDir.setValue(NULL);

        s->sheetOffsetDist=-5;
        s->offsetDeviation = 4;
        s->stepoverDeviation = 8;
        s->intervalDist = 0;

        s->toolHeight = 40.0f;
        s->toolBreadth = 1.2f*s->toolHeight;
        s->cutDirCutLen = 0.0f;//此时无用
        s->cutDirToolBlank = 0.0f;//此时无用
        s->cutShapeWide = 20.0f;
        s->cutShapeBlank = 2.0f;
        s->cutDirDeltaLen = 100;

        s->cutDir.setValue(NULL);

        s->trimLtag.clear();
        s->isTrimLClose = false;
        s->col[0] = 129;
        s->col[1] = 161;
    }
};



#endif