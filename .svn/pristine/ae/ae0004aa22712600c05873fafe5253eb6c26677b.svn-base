#ifndef _ASSISTFUN_H_
#define  _ASSISTFUN_H_


// NX 8.5.0.23
// Journal created by liban on Wed Sep 28 10:44:19 2016 中国标准时间
//
#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Body.hxx>
#include <NXOpen/BodyCollection.hxx>
#include <NXOpen/BodyDumbRule.hxx>
#include <NXOpen/Builder.hxx>

#include <NXOpen/Direction.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/ExpressionCollection.hxx>
#include <NXOpen/Face.hxx>
#include <NXOpen/Features_Brep.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>


#include <NXOpen/MeasureDistanceBuilder.hxx>
#include <NXOpen/MeasureManager.hxx>
#include <NXOpen/ModelingView.hxx>
#include <NXOpen/ModelingViewCollection.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/ScCollector.hxx>
#include <NXOpen/ScRuleFactory.hxx>
#include <NXOpen/SelectDisplayableObject.hxx>
#include <NXOpen/SelectDisplayableObjectList.hxx>
#include <NXOpen/SelectObject.hxx>
#include <NXOpen/SelectionIntentRule.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Unit.hxx>
#include <NXOpen/UnitCollection.hxx>
#include <NXOpen/Update.hxx>
#include <NXOpen/View.hxx>
#include <NXOpen/DisplayableObject.hxx>
#include <NXOpen/Features_BooleanBuilder.hxx>
#include <NXOpen/GeometricUtilities_BooleanRegionSelect.hxx>
#include <NXOpen/Preferences_PartModeling.hxx>
#include <NXOpen/Preferences_PartPreferences.hxx>
#include <NXOpen/ScCollectorCollection.hxx>
#include <NXOpen/ScRuleFactory.hxx>
#include <NXOpen/SelectBodyList.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/FaceTangentRule.hxx>
#include <NXOpen/Features_OffsetFaceBuilder.hxx>
#include <NXOpen/ModelingView.hxx>
#include <NXOpen/ModelingViewCollection.hxx>

#include <NXOpen/CurveDumbRule.hxx>
#include <NXOpen/IBaseCurve.hxx>
#include <NXOpen/Arc.hxx>
#include <NXOpen/ArcCollection.hxx>
#include <NXOpen/Line.hxx>
#include <NXOpen/LineCollection.hxx>
#include <NXOpen/Edge.hxx>
#include <NXOpen/EdgeDumbRule.hxx>
#include <NXOpen/EdgeChainRule.hxx>
#include <NXOpen/Parabola.hxx>
#include <NXOpen/ParabolaCollection.hxx>
#include <NXOpen/Ellipse.hxx>
#include <NXOpen/EllipseCollection.hxx>
#include <NXOpen/Hyperbola.hxx>
#include <NXOpen/HyperbolaCollection.hxx>



#include <NXOpen/DisplayableObject.hxx>
#include <NXOpen/Features_ExtrudeBuilder.hxx>
#include <NXOpen/Features_Feature.hxx>
#include <NXOpen/Features_FeatureBuilder.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>
#include <NXOpen/GeometricUtilities_BooleanOperation.hxx>
#include <NXOpen/GeometricUtilities_Extend.hxx>
#include <NXOpen/GeometricUtilities_FeatureOffset.hxx>
#include <NXOpen/GeometricUtilities_FeatureOptions.hxx>
#include <NXOpen/GeometricUtilities_Limits.hxx>
#include <NXOpen/GeometricUtilities_MultiDraft.hxx>
#include <NXOpen/GeometricUtilities_SimpleDraft.hxx>
#include <NXOpen/GeometricUtilities_SmartVolumeProfileBuilder.hxx>
#include <NXOpen/IBaseCurve.hxx>

#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Preferences_PartModeling.hxx>
#include <NXOpen/Preferences_PartPreferences.hxx>
#include <NXOpen/ScRuleFactory.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SectionCollection.hxx>
#include <NXOpen/SelectionIntentRule.hxx>
#include <NXOpen/FaceDumbRule.hxx>
#include <NXOpen/Features_TrimExtendBuilder.hxx>

#include <NXOpen/Features_OffsetFaceBuilder.hxx>
#include <NXOpen/Features_Extrude.hxx>
#include <NXOpen/Features_ExtrudeBuilder.hxx>

#include <vector>
#include "../../Include/PublicFun.h"
#include <NXOpen/BlockStyler_CurveCollector.hxx>


using namespace std;
using namespace NXOpen;

class CurveData;
struct TrimCurvesInfo
{
    vector<vector<CurveData*>> trimCruves;//修边线和辅助线成对前后相接存储在本变量中。
    Vector3d dir;
};



static const char sheetname[133] = "TIME";


inline void vec3Swp(double * a, double *b)
{
    double temp[3];
    UF_VEC3_copy(a, temp);
    UF_VEC3_copy(b, a);
    UF_VEC3_copy(temp, b);
}
void is_parallel(const double one_3[3], const double two_3[3], double angle, int * rt);
void copyCurvesData(vector<vector<CurveData>>& temp1, vector<vector<CurveData>>&temp2);

void ArcTranslateLine(vector<vector<CurveData>>&temp_);
void ArcTranslateLine(vector<vector<CurveData*>>&temp_);

void stdProcessCurves(vector<vector<CurveData>>&temp_);
void stdProcessCurves(vector<vector<CurveData*>>&temp_);
void sortCurvesPointor(vector<CurveData*> tempDelete, vector<vector<CurveData*>> &rt_simplePro_);

class CurveData //元素数据不需要uf_initialize()
{
    typedef CurveData CD;
public:     //类属性函数
    CurveData()
    {
        set0();
    }
    CurveData(tag_t eid)
    {
        setData(eid);
    }
    CurveData(const CD & r)
    {
        curve = r.curve;
        UF_VEC3_copy(r.start_point, start_point);
        UF_VEC3_copy(r.end_point, end_point);
        UF_VEC3_copy(r.vertexPt, vertexPt);
        UF_VEC3_copy(r.dir_Center, dir_Center);
        type_ = r.type_;
        dist_len = r.dist_len;
        radius = r.radius;
    }
    ~CurveData()
    {
    }
    CD& operator=(const CD & r)
    {
        curve = r.curve;
        UF_VEC3_copy(r.start_point, start_point);
        UF_VEC3_copy(r.end_point, end_point);
        UF_VEC3_copy(r.vertexPt, vertexPt);
        UF_VEC3_copy(r.dir_Center, dir_Center);
        type_ = r.type_;
        dist_len = r.dist_len;
        radius = r.radius;
        return *this;
    }

public:     //内外原子操纵
    void set0()
    {
        memset(&curve, 0, sizeof(CurveData));
    }

    void setData(tag_t eid)
    {
        curve = eid;
        SetData();
    }

    void del()
    {
        if (UF_OBJ_ask_status(curve) != UF_OBJ_DELETED)
        {
            UF_OBJ_delete_object(curve);
        }
    }

    //内部原子操作
    void SetData()
    {
        UF_INITIALIZE()
            if (UF_OBJ_ask_status(curve) != UF_OBJ_DELETED)
            {
                int subType_;

                MyFun::getCurvePt(curve, 0, start_point);
                MyFun::getCurvePt(curve, 1, end_point);

                UF_INITIALIZE();
                UF_OBJ_ask_type_and_subtype(curve, &type_, &subType_);
                baseType = 0;
                if (type_ == 70 && subType_ == 2)
                {
                    baseType = 70;
                    UF_MODL_ask_edge_type(curve, &subType_);
                    if (subType_ == UF_MODL_LINEAR_EDGE)
                        type_ = UF_line_type;
                    else if (subType_ == UF_MODL_CIRCULAR_EDGE)//其他类型的边 不处理
                        type_ = UF_circle_type;
                    else
                        type_ = 0;
                }
                if (type_ == UF_line_type)
                {
                    dist_len = MyFun::get_line_length(curve, start_point, end_point);
                    UF_VEC3_sub(end_point, start_point, dir_Center);
                }
                else if (type_ == UF_circle_type)
                    dist_len = MyFun::get_arc_length(curve, dir_Center, &radius);
                else
                    if (baseType == 70)
                        dist_len = MyFun::get_spline_length(curve, true);
                    else
                        dist_len = MyFun::get_spline_length(curve);
                UF_VEC3_copy(end_point, vertexPt);
            }
            else
            {
                uc1601("输入曲线不存在！", 1);
            }
    }
    void swap()
    {
        vec3Swp(start_point, end_point);
        UF_VEC3_copy(end_point, vertexPt);
        if (type_ == UF_line_type)
            UF_VEC3_sub(end_point, start_point, dir_Center);
    }

    void setStartPt(double pt[3])
    {
        if (pt)
            UF_VEC3_copy(pt, start_point);
    }
    void setEndPt(double pt[3])
    {
        if (pt)
            UF_VEC3_copy(pt, end_point);
    }
    void setVertexPt(double pt[3])
    {
        if (pt)
            UF_VEC3_copy(pt, vertexPt);
    }
public:     //对象关系

    bool isParallel(const CD& r, double angle = 0.001)//直线才有的平行判断判断 0<=angle<=PI/2
    {
        int rt = 0;
        if (type_ == r.type_&&type_ == UF_line_type)
        {
            is_parallel(dir_Center, r.dir_Center, angle, &rt);
            if (rt)
                return true;
            else
                return false;
        }
        else
        {
            return false;
        }

    }
    bool isConnect(const CD & r)//处理交换端点.
    {
        if (MyFun::is_Equal(r.end_point, start_point, 0.001))
        {
            return true;
        }
        if (MyFun::is_Equal(r.start_point, start_point, 0.001))
        {
            return true;
        }
        if (MyFun::is_Equal(r.start_point, end_point, 0.001))
        {
            swap();
            return true;
        }
        if (MyFun::is_Equal(r.end_point, end_point, 0.001))
        {
            swap();
            return true;
        }
        return false;
    }
    bool isToAxisArc(const CD & r, double  tol)//tol>1
    {
        if (type_ == r.type_&&type_ == UF_circle_type)
        {
            double dist;
            UF_VEC3_distance(dir_Center, r.dir_Center, &dist);
            double aveRadius;
            aveRadius = (radius + r.radius) / 2;
            if (dist < aveRadius / tol)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }


    operator void*()
    {
        return this;
    }
    operator tag_t()
    {
        return this->curve;
    }


public: //原子重建

    void reEditLine(double startPt[3], double endPt[3])
    {
        //del();
        UF_CURVE_line_t lt;
        UF_VEC3_copy(startPt, lt.start_point);
        UF_VEC3_copy(endPt, lt.end_point);
        if (UF_OBJ_ask_status(curve) != UF_OBJ_DELETED)
        {
            UF_CURVE_edit_line_data(curve, &lt);
        }
        else
        {
            UF_CURVE_create_line(&lt, &curve);
        }
        SetData();
        UF_VEC3_copy(startPt, start_point);
        UF_VEC3_copy(endPt, end_point);
    }

    void reEditArc(double startPt[3], double pt[3], double endPt[3])
    {
        del();
        UF_CALL(UF_CURVE_create_arc_thru_3pts(1, startPt, pt, endPt, &curve));
        SetData();
        UF_VEC3_copy(startPt, start_point);
        UF_VEC3_copy(endPt, end_point);
    }

    const double * retMidPt()
    {
        MyFun::getCurvePt(curve, 0.5, vertexPt);
        return vertexPt;
    }

    int isOnCurve(double * pt)//start_point 1, end_point 2,vertexPt 3,else 0
    {
        retMidPt();
        if (MyFun::is_Equal(start_point, pt, 0.001))
            return 1;
        if (MyFun::is_Equal(end_point, pt, 0.001))
            return 2;
        if (MyFun::is_Equal(vertexPt, pt, 0.001))
            return 3;
        return 0;
    }

public:
    tag_t curve;
    double start_point[3];//Param 0
    double end_point[3];  //Param 1
    double vertexPt[3];

    int type_; //UF_line_type UF_circle_type
    int baseType;//边70，否则0 
    double dir_Center[3];//直线 ->方向,圆弧->中心点坐标
    double dist_len;//直线->直线->长度 圆弧->长度
    double radius; //圆弧半径
};

struct vccdata
{
    CurveData a;
    CurveData b;
    vector<tag_t> vect;
};




extern char szTrim[];
extern char szAssist[];

/************以成员的方式增加设置绝对坐标系功能**************************************/

#define DECLARE_CSYS_MFUN                       \
    private:                                    \
        tag_t   cur_sys;                        \
        tag_t   abs_sys;                        \
        void SetAbsSys(){                       \
        UF_initialize();                        \
            UF_CSYS_ask_wcs(&cur_sys);          \
            UF_CSYS_set_wcs_display(0);         \
            abs_sys = MyFun::setAbsSys();}      \
                                                \
        void SetOrgSys(){                       \
        UF_initialize();                        \
        UF_CSYS_set_wcs(cur_sys);               \
        UF_CSYS_set_wcs_display(1);             \
        DELETE_OBJ(abs_sys);                    \
		}	
#define INIT_ABS_CSYS()     SetAbsSys()         //UI类中放置在构造里
#define TERM_ABS_CSYS()     SetOrgSys()         //UI类中放置在析构里
/*********************************************************************************/



//********************************************************************
// Access:    	::public 
// Description: 求碰点最小距离,并矫正映射方向
// Par: 		const vector<CurveData> & cdPt
// Par: 		vector<tag_t> & sheets
// Par: 		double * projectDir
// Ret:   		double
//********************************************************************
double measureMinDimensionBCS(const vector<CurveData>& cdPt, tag_t sheet, double *projectDir);


//********************************************************************
// Access:    	::public 
// Description: 求一个对象和一群对象之间的最小距离,返回最小距离的对象群里的对象
// Par: 		tag_t obj_
// Par: 		vector<tag_t> tagertObj
// Par: 		tag_t & hitBody
// Par: 		double * hitPt
// Ret:   		double
//********************************************************************
double MinDimension(tag_t obj_, vector<tag_t> tagertObj, tag_t& retBody, double * hitPt);

//********************************************************************
// Access:    	::public 
// Description: 最小距离中最大值的tag
// Par: 		tag_t obj_
// Par: 		vector<tag_t> tagertObj
// Par: 		tag_t & retBody
// Par: 		double * hitPt
// Ret:   		double
//********************************************************************
double MaxDimension(tag_t obj_, vector<tag_t> tagertObj, tag_t& retBody, double * hitPt);
//********************************************************************
// Access:    	::public 
// Description: 用来判断tag的对象是否是在激活的图层的有效对象.
// Parame: 		tag_t obj
// Returns:   	int 返回值用以判断,tag是在激活的图层里并且不是毛坯对象,值设为0
//********************************************************************
int is_invisible_of(tag_t obj);


//********************************************************************
// Access:    	::public 
// Description: 取出显示部件中的所有片体和片体的个数;
// Parame: 		vector<tag_t> & targetTag只做输出
// Returns:   	size_t;
//********************************************************************
size_t getAllSheets(vector<tag_t> & targetTag,const char*sheetName=NULL);

int  setBodyColor(tag_t obj, int col_1, int col_2);


//********************************************************************
// Access:    	::public 
// Description: 此处异常不用补获,调用者补获 默认创建体不是片体(有加粗功能.)
// Par: 		std::vector<CurveData> curves
// Par: 		CurveData dirt 拉伸曲线集中在端处的曲线,闭合曲线集可以随意放置一个曲线集中的曲线,一端创建失败,用另一端创建
// Par: 		double * dir
// Par: 		double startVertical 拉伸起始位置,相对于曲线
// Par: 		double endVertical
// Par: 		double startOffset 加厚起始位置 相对于曲线
// Par: 		double endOffset
// Ret:   		tag_t
//********************************************************************
tag_t extrudorEx(std::vector<tag_t> curves, CurveData dirt, double *dir,
    double startVertical, double endVertical, double startOffset, double endOffset);


//********************************************************************
// Access:    	::public 
// Description: 拉伸片体
// Par: 		std::vector<tag_t> curves
// Par: 		CurveData dirt
// Par: 		double * dir
// Par: 		double startVertical
// Par: 		double endVertical
// Ret:   		tag_t
//********************************************************************
tag_t createSheet(const std::vector<tag_t>& curves, CurveData& dirt, double *dir,
    double startVertical, double endVertical);

//********************************************************************
// Access:    	::public 
// Description: 偏置面 的功能偏置片体
// Par: 		vector<tag_t>sheetfaces
// Par: 		double dist
// Ret:   		tag_t feat;
//********************************************************************
tag_t  offsetFaceSheet(vector<tag_t>sheetfaces, double dist, bool flags);




//********************************************************************
// Access:    	::public 
// Description: 大致偏置 的功能 偏置片体
// Par: 		tag_t sheet
// Par: 		double offsetdist
// Ret:   		tag_t
//********************************************************************
tag_t offsetSheet(tag_t sheet, double offsetdist, double v1 = 0, double v2 = 0);

//********************************************************************
// Access:    	::public 
// Description: 缝合片体
// Par: 		vector<tag_t> sheets
// Ret:   		tag_t
//********************************************************************
tag_t sewSheets(vector<tag_t>& sheets);

//********************************************************************
// Access:    	::public 
// Description: 延展片体
// Par: 		tag_t sheet
// Ret:   		tag_t
//********************************************************************
tag_t TrimExtendSheet(tag_t sheet);

bool isContainning(vector<tag_t> curves, tag_t obj_);

bool isContainning(vector<tag_t> obj, double pt[3]);

void reNameCurves(vector<tag_t> temp, char name_[]);

void roughOffsetCurves(const double dist, const bool is_dir, vector<tag_t> &tempCurves);

//********************************************************************
// Access:    	::public 
// Description: 曲线不相接情况,编辑曲线
// Par: 		CurveData & L1  这里假设L1的 end_point 和 L2的 start_point应该是相接的.但实际两者有距离误差,0.5以内 
// Par: 		CurveData & L2
// Par: 		double tolerance L1和L2相接点坐标单值最大差值
// Ret:   		void
//********************************************************************
// inline void editTwoLines(CurveData & L1, CurveData & L2,const double& tolerance) 
// {
// 	double pt[3];
// 	double tolerance_;
// 	getDirectionPos(L1.dir_Center,L1.end_point,tolerance,pt);
// 	tolerance_=Max_D_value(pt,L2.start_point);
// 	UF_CURVE_line_t lt;
// 
// 	if (tolerance_<tolerance)
// 	{
// 		UF_VEC3_copy(L2.start_point, L1.end_point);
// 		UF_VEC3_copy(L2.start_point, lt.end_point);
// 		UF_VEC3_copy(L1.start_point, lt.start_point);
// 		
// 	//	UF_CURVE_edit_line_data();
// 	}
// 	else
// 	{
// 
// 	}
// }
typedef struct st_
{
    tag_t *lineID;
    tag_t *lineID1;
    tag_t *lineID2;
    double *startPt;
    double *endPt;
    double *startPt1;
    double *endPt1;
    double *startPt2;
    double *endPt2;
    double dir[3];
    static tag_t lines1[4];
    static double ptSet1[12];
} MovLineData;

inline void askCruvesDataEx(const vector<tag_t> &temp_1, vector<CurveData>&temp_2)
{
    temp_2.clear();
    for (int i = 0; i < temp_1.size(); i++)
        temp_2.push_back(temp_1[i]);
}

//cd1 和 cd2 收尾相接
void editCurveData(const CurveData& cd1, CurveData &cd2);

void sortCurves(vector<tag_t> & curves, vector<vector<CurveData>> &m_simplePro_, vector<vccdata>& m_trimVCC, vector<CurveData> &m_vertexPts, bool& st_isTrimLClose);

void deleteReCurve(vector<CurveData>& temp);

//********************************************************************
// Access:    	CommonOperation::private 
// Description: 
// Par: Int Out	const vector<tag_t> & tempSplines 还有样条或 直线圆弧
// Ret:   		void
//********************************************************************
void splinesProcessToLines(vector<tag_t> &tempSplines); //刚件符型避让用.

inline tag_t createAxis(const double *dir, const double *org_t)
{
    UF_initialize();
    double pt[3];
    MyFun::getDirectionPos(dir, org_t, 100, pt);
    tag_t temp;
    UF_MODL_create_fixed_daxis(const_cast<double*>(org_t), pt, &temp);
    return temp;
}


//**********************************************
// Ret	:   void
// Disc	:	
// Param: 	CurveData * * tempCD
// Param: 	double dir[3]冲裁方向
// Param: 	double width辅助线拉长长度
// Param: 	double len0
// Param: 	double len1
// Param: 	int flag_type 0下模 1上模刀块 
// Param: 	vector<CurveData * > & assist
//**********************************************
void closeAssistCurve(CurveData** tempCD, double dir[3], double width, double len0, double len1, int flag_type, vector<CurveData*>&assist);



// Ret	:   void
// Disc	:	
// Param: 	vector<CurveData * > & trimCurves
// Param: 	double * dir
// Param: 	double width
// Param: 	vector<CurveData * > & assistLD
// Param: 	int flag_type 0下模 1上模刀块 
void autoCloseAssistCurves(vector<CurveData*>& trimCurves, double *dir, double width, vector<CurveData*>& assistLD,int &count, int flag_type = 1);

void adjustLineData(tag_t &line, UF_CURVE_line_t& lt);//修编模块

void selectedCurves(NXOpen::BlockStyler::CurveCollector*uiBlock, std::vector<tag_t>&curves);

bool hasAttr(tag_t &obj);

void readAttr(tag_t obj, char * title, int type /*= UF_ATTR_string*/,UF_ATTR_value_s&ret);

int findAttr(tag_t obj, char * title, int type/*= UF_ATTR_string*/);

bool is_3DCurve(const vector<tag_t> &curves, double *dir3=NULL);//spline不在一个平面上极为3d





#endif
