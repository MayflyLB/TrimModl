#ifndef SORTCURVES_H_
#define SORTCURVES_H_
#include <vector>
using namespace std;

class CurveData //元素数据不需要uf_initialize()
{
    typedef CurveData CD;
public:     //类属性函数
    CurveData();
    CurveData(tag_t eid);
    CurveData(const CD & r);
    ~CurveData();
    CD& operator=(const CD & r);

public:     //内外原子操纵
    void set0();

    void setData(tag_t eid);

    void del();

    //内部原子操作
    void SetData();
    void swap();

    void setStartPt(double pt[3]);
    void setEndPt(double pt[3]);
    void setVertexPt(double pt[3]);
public:     //对象关系

    bool isConnect(const CD & r);//处理交换端点.
    bool isToAxisArc(const CD & r, double  tol);//tol>1

    operator void*();
    operator int();
    operator tag_t();
    operator tag_t()const;

public: //原子重建

    void reEditLine(double startPt[3], double endPt[3]);

    void reEditArc(double startPt[3], double pt[3], double endPt[3]);

    const double * retMidPt();

    int isOnCurve(double * pt);//start_point 1, end_point 2,vertexPt 3,else 0

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

//************************************
// Parameter: vector<CurveData * > tempDelete 输入无序的线（可能不止一串线）本参数不是引用,所以本参数的实参记录内存分配的地址,用于释放
// Parameter: vector<vector<CurveData * >> & rt_simplePro_返回收尾相接的线（可能有多串线，按二维vector结构存放）
//************************************
void sortCurvesPointor(vector<CurveData*> tempDelete, vector<vector<CurveData*>> &rt_simplePro_);
    
//************************************
// Parameter: vector<vector<CurveData * >> & temp_  输入sortCurvesPointor()处理完成的返回的实参参数
// Parameter: double tolerance //误差 默认0.005
//************************************
void ArcTranslateLine(vector<vector<CurveData*>>&temp_,double tolerance = 0.005);

//************************************
// Parameter: vector<vector<CurveData * >> & temp_ 圆弧直线合并
//************************************
void stdProcessCurves(vector<vector<CurveData*>>&temp_);

#endif // SORTCURVES_H_
