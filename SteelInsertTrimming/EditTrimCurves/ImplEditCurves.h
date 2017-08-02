
#ifndef IMPLEDITCURVES_H_
#define IMPLEDITCURVES_H_

#include <vector>
#include <NXOpen/BlockStyler_SpecifyOrientation.hxx>
#include <NXOpen/BlockStyler_LinearDimension.hxx>
#include "../Include/assistFun.h"
#include "../Include/CurveAttrE.h"
using namespace std;


inline void setLinearData(NXOpen::BlockStyler::LinearDimension* pLine, const Vector3d& dir, const Point3d &startPt, double len)
{
    pLine->SetHandleOrigin(startPt);
    pLine->SetValue(len);
    pLine->SetHandleOrientation(dir);
}
inline void setLinearStatus(NXOpen::BlockStyler::LinearDimension* pLine,bool flag=true)
{
    if(pLine->Show()&&!flag)
        pLine->SetValue(0);
    pLine->SetEnable(flag);
    pLine->SetShow(flag);
    pLine->SetShowFocusHandle(flag);
    pLine->SetShowHandle(flag);
}



class EditLines
{
public:
    EditLines(std::vector<TrimCurveData>&x, std::vector<TrimCurveData>&y, NXOpen::BlockStyler::CurveCollector* curvesSelect, NXOpen::BlockStyler::SpecifyOrientation* manipulator, NXOpen::Vector3d tempDir, NXOpen::BlockStyler::LinearDimension**ppLinears)
        :m_curvesSelect(curvesSelect), m_manipulator(manipulator), m_ppLinears(ppLinears)
    {
        m_focusPt = nullptr;
        m_pCurrentMovLines = nullptr;
        m_cutDir = tempDir;
        for (int i = 0; i < y.size(); i++)
        {
            m_curves.push_back((*y[i].trim_)[0]);
            for (int j=0;j<y[i].assist_->size();j++)
                m_curves.push_back((*y[i].assist_)[j]);
            m_curves.push_back((*y[i].trim_).back());
        }
        for (int i = 0; i < x.size(); i++)
        {
            m_curves.push_back((*x[i].trim_)[0]);
            for (int j = 0; j < x[i].assist_->size(); j++)
                m_curves.push_back((*x[i].assist_)[j]);
            m_curves.push_back((*x[i].trim_).back());
        }
        m_focusCurve0 = nullptr;
        m_focusCurve1 = nullptr;
        m_focusCurve2 = nullptr;
        sortCurvesPointor(m_curves, m_Groups);
        for (int i=0;i<m_Groups.size();i++)
        {
            m_Groups[i].back()->swap();//两端的修边线方向修正为射向开口端.
        }
        m_tempCurves.clear();
    }
    ~EditLines()
    {
        if (m_focusCurve1)
        {
            UF_INITIALIZE();
            if (m_focusCurve1&&UF_OBJ_ask_status(m_focusCurve1->curve) != UF_OBJ_DELETED)
                UF_CALL( UF_OBJ_set_color(m_focusCurve1->curve, m_col));
        }
        for (int i = 0; i < m_tempCurves.size(); i++)
           delete m_tempCurves[i];
        m_tempCurves.clear();
    }
    void setFlag()
    {
        if (m_curvesSelect==nullptr) return ;
        UF_INITIALIZE();
        tag_t temp = 0;
        if (m_curvesSelect->GetSelectedObjects().size())
            temp=m_curvesSelect->GetSelectedObjects().at(0)->GetTag();
        else
            return; 
        if (m_focusCurve1==nullptr||m_focusCurve1->curve != temp&&temp!=0)
        {
            bool flag = true;
            for ( int i=0;i<m_Groups.size();i++)
            {
                for (int j=0;j<m_Groups[i].size();j++)
                {
                    if (m_Groups[i][j]->curve == temp)
                    {

                        if (j>0) m_focusCurve0 = m_Groups[i][j-1];
                        else m_focusCurve0 = nullptr;
                        if(m_focusCurve1)
                            UF_OBJ_set_color(m_focusCurve1->curve, m_col);
                        m_focusCurve1 = m_Groups[i][j];
                        m_col = MyFun::retCol(m_focusCurve1->curve);
                        UF_OBJ_set_color(m_focusCurve1->curve, 6);
                        if (j + 1 < m_Groups[i].size()) m_focusCurve2 = m_Groups[i][j + 1];
                        else m_focusCurve2 = nullptr;

                        if (m_pCurrentMovLines != &m_Groups[i])
                        {
                            for (int x=0;x<20;x++)
                                setLinearStatus(m_ppLinears[x],false);
                            m_pCurrentMovLines = &m_Groups[i];
                            linearBing();
                        }


                        if (j==0||j+1== m_Groups[i].size())
                            m_status = 0;
                        else if(j == 1&& m_Groups[i].size()==3)
                            m_status = 1;
                        else if (j==1&& m_Groups[i].size()>3)
                            m_status = 2;
                        else if(j== m_Groups[i].size()-2&& m_Groups[i].size()>3)
                            m_status = 3;
                        else
                            m_status = 4;

                        if (m_status)
                        {
                            m_focusPt = m_midPt;
                            UF_VEC3_copy(m_focusCurve1->retMidPt(),m_focusPt);
                        }else
                            m_focusPt = m_focusCurve1->end_point;
                        m_manipulator->SetOrigin(m_focusPt);
                        if (m_status )
                        {
                            m_manipulator->SetYAxis(m_focusCurve1->dir_Center);
                            double y[3];
                            UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                            m_manipulator->SetXAxis(y);
                        }
                        else
                        {
                            m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
                            double y[3];
                            UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                            m_manipulator->SetYAxis(y);
                        }
                        flag = false;
                        break;
                    }
                }
                if (flag==false)  break;
            }
            if (flag)
            {
                std::vector<NXOpen::TaggedObject*> tempT;
                tempT.push_back(NXOpen::NXObjectManager::Get(m_focusCurve1->curve));
                m_curvesSelect->SetSelectedObjects(tempT);
                m_manipulator->SetOrigin(Point3d(0.0,0.0,0.0));
                m_curvesSelect->Focus();
            }
        }
    }

    void moveOper()
    {
        if (m_manipulator == nullptr) return;

        NXOpen::Point3d && tempOrigin= m_manipulator->Origin();
        NXOpen::Vector3d && tempDirX = m_manipulator->XAxis();

        UF_VEC3_copy(m_focusCurve1->retMidPt(), m_midPt);
        if (MyFun::is_Equal(m_midPt, tempOrigin, 0.001))//旋转判断(修边线只能延长)
        {
            NXOpen::Vector3d && tempDirY = m_manipulator->YAxis();
            m_focusPt = m_midPt;
            if (m_focusCurve0&&m_focusCurve2)
            {
                int status;
                UF_VEC3_is_parallel(tempDirY, m_focusCurve1->dir_Center, 0.001, &status);
                if (status)
                {
                    m_manipulator->SetOrigin(m_focusPt);
                    m_manipulator->SetYAxis(m_focusCurve1->dir_Center);
                    double y[3];
                    UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                    m_manipulator->SetXAxis(y);
                    return;
                }
                UF_VEC3_copy(tempDirY,m_focusCurve1->dir_Center);

                UF_VEC3_is_parallel(tempDirY, m_focusCurve0->dir_Center, 0.001, &status);
                if (status == 0)
                    MyFun::intersectPtOfLines_(m_focusCurve0->dir_Center, m_focusCurve0->vertexPt, m_focusCurve1->dir_Center, m_focusCurve1->vertexPt, m_focusCurve1->start_point);
                UF_VEC3_copy(m_focusCurve1->start_point, m_focusCurve0->end_point);
                UF_VEC3_copy(m_focusCurve0->end_point, m_focusCurve0->vertexPt);
                m_focusCurve0->reEditLine();

                if (m_status == 3 || m_status == 1)
                    m_focusCurve2->swap();
                UF_VEC3_is_parallel(tempDirY, m_focusCurve2->dir_Center, 0.001, &status);
                if (status == 0)
                    MyFun::intersectPtOfLines_(m_focusCurve2->dir_Center, m_focusCurve2->vertexPt, m_focusCurve1->dir_Center, m_focusCurve1->vertexPt, m_focusCurve1->end_point);
                UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve2->start_point);
                m_focusCurve2->reEditLine();
                if (m_status == 3 || m_status == 1)
                    m_focusCurve2->swap();
                
                UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve1->vertexPt);
                m_focusCurve1->reEditLine();
                m_manipulator->SetOrigin(m_focusCurve1->retMidPt());
            }
            else//是修边线固定 轴线
            {
                m_manipulator->SetOrigin(m_focusCurve1->end_point);
                m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
            }
        }
        else if (MyFun::is_Equal(m_focusCurve1->start_point, tempOrigin, 0.001))
        {



            if (m_status==2|| m_status==4)
            {
                m_focusPt = m_focusCurve1->start_point;
                m_manipulator->SetXAxis(-m_focusCurve1->dir_Center);
                double y[3];
                UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                m_manipulator->SetYAxis(y);
            }
            else
            {
                m_manipulator->SetOrigin(m_focusPt);
                if (m_status)
                {
                    m_manipulator->SetYAxis(m_focusCurve1->dir_Center);
                    double y[3];
                    UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                    m_manipulator->SetXAxis(y);
                }
                else
                {
                    m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
                    double y[3];
                    UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                    m_manipulator->SetYAxis(y);
                }
                return;
            }
            return;
        }
        else if (MyFun::is_Equal(m_focusCurve1->end_point, tempOrigin, 0.001))
        {
            if (m_status == 2 || m_status == 4)
            {
                m_focusPt = m_focusCurve1->end_point;
                m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
                double y[3];
                UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                m_manipulator->SetYAxis(y);
            }
            else
            {
                m_manipulator->SetOrigin(m_focusPt);
                if (m_status)
                {
                    m_manipulator->SetYAxis(m_focusCurve1->dir_Center);
                    double y[3];
                    UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                    m_manipulator->SetXAxis(y);
                }
                else
                {
                    m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
                    double y[3];
                    UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                    m_manipulator->SetYAxis(y);
                }
                return;
            }
            return;
        }
        else//移动和拉长. 拉长 方向同向 移动 方向垂直
        {//先确定是否是第一移动方位

            double dir[3];
            UF_VEC3_sub(tempOrigin, m_focusPt, dir);
            int status1,status2=0;
            UF_VEC3_is_parallel(m_focusCurve1->dir_Center, dir, 0.001, &status1);//判断是否跳点了
            if(m_focusPt==m_midPt)
                //判断移动状态方向是否正确
                UF_VEC3_is_perpendicular(m_focusCurve1->dir_Center, dir, 0.001, &status2);

            if (MyFun::is_Equal(tempOrigin, m_focusCurve1->end_point,0.001))
            {//调整起始状态
                if (status1==0|| status2==0)
                {
                    m_manipulator->SetOrigin(m_focusPt);
                    if (m_status)
                    {
                        m_manipulator->SetYAxis(m_focusCurve1->dir_Center);
                        double y[3];
                        UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                        m_manipulator->SetXAxis(y);
                    }
                    else
                    {
                        m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
                        double y[3];
                        UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                        m_manipulator->SetYAxis(y);
                    }
                    return;
                }
                m_focusPt = m_focusCurve1->end_point;
                m_manipulator->SetOrigin(m_focusPt);
                m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
                double y[3];
                UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                m_manipulator->SetYAxis(y);
                return;
            }
            else if (MyFun::is_Equal(tempOrigin, m_focusCurve1->start_point, 0.001))
            {//调整起始状体
                if (status1 == 0 || status2)
                {
                    m_manipulator->SetOrigin(m_focusPt);
                    if (m_status)
                    {
                        m_manipulator->SetYAxis(m_focusCurve1->dir_Center);
                        double y[3];
                        UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                        m_manipulator->SetXAxis(y);
                    }
                    else
                    {
                        m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
                        double y[3];
                        UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                        m_manipulator->SetYAxis(y);
                    }
                    return;
                }
                m_focusPt = m_focusCurve1->start_point;
                m_manipulator->SetOrigin(m_focusPt);
                m_manipulator->SetXAxis(-Vector3d(m_focusCurve1->dir_Center));
                double y[3];
                UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                m_manipulator->SetYAxis(y);
                return;
            }
            else
            {//移动
                if (status2)
                {//判断旋转状态或者移动状态方向是否正确
                    m_manipulator->SetOrigin(m_focusPt);
                    if (m_status)
                    {
                        m_manipulator->SetYAxis(m_focusCurve1->dir_Center);
                        double y[3];
                        UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                        m_manipulator->SetXAxis(y);
                    }
                    else
                    {
                        m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
                        double y[3];
                        UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
                        m_manipulator->SetYAxis(y);
                    }
                    return;
                }

                UF_VEC3_copy(tempOrigin, m_focusPt);
                int status_;
                UF_VEC3_is_parallel(tempDirX, m_focusCurve1->dir_Center, 0.001, &status_);
                if (status_)//拉长
                {
                    CurveData* temp;
                    if (m_focusPt==m_focusCurve1->start_point)
                    {
                        temp = m_focusCurve0;
                        UF_VEC3_copy(m_focusPt, temp->end_point);
                        UF_VEC3_copy(m_focusPt, temp->vertexPt);
                    }
                    else
                    {
                        if (m_status==0)
                        {
                            if (m_focusCurve0)
                            {
                                temp = m_focusCurve0;
                                UF_VEC3_copy(m_focusPt, temp->end_point);
                                UF_VEC3_copy(m_focusPt, temp->vertexPt);
                                UF_VEC3_copy(m_focusPt, m_focusCurve1->vertexPt);
                            }
                            else
                            {
                                temp = m_focusCurve2;
                                UF_VEC3_copy(m_focusPt, temp->start_point);
                                UF_VEC3_copy(m_focusPt, m_focusCurve1->vertexPt);
                            }

                        }
                        else if(m_status == 3)
                        {
                            temp = m_focusCurve2;
                            UF_VEC3_copy(m_focusPt, temp->end_point);
                            UF_VEC3_copy(m_focusPt, m_focusCurve1->vertexPt);
                            UF_VEC3_copy(m_focusPt, temp->vertexPt);
                        }
                        else
                        {
                            temp = m_focusCurve2;
                            UF_VEC3_copy(m_focusPt, temp->start_point);
                            UF_VEC3_copy(m_focusPt, m_focusCurve1->vertexPt);
                        }
                    }
                    temp->reEditLine();
                    m_focusCurve1->reEditLine();
                }
                else//移动
                {
                    MyFun::intersectPtOfLines_(m_focusCurve0->dir_Center, m_focusCurve0->vertexPt, m_focusCurve1->dir_Center, tempOrigin, m_focusCurve1->start_point);
                    UF_VEC3_copy(m_focusCurve1->start_point, m_focusCurve0->end_point);
                    m_focusCurve0->reEditLine();
                    UF_VEC3_copy(m_focusCurve0->end_point, m_focusCurve0->vertexPt);


                    if (m_status == 3|| m_status == 1)
                        m_focusCurve2->swap();
                    MyFun::intersectPtOfLines_(m_focusCurve2->dir_Center, m_focusCurve2->vertexPt, m_focusCurve1->dir_Center, tempOrigin, m_focusCurve1->end_point);
                    UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve2->start_point);
                    m_focusCurve2->reEditLine();
                    if (m_status == 3 || m_status == 1)
                        m_focusCurve2->swap();
                    UF_VEC3_copy(m_focusCurve2->end_point, m_focusCurve2->vertexPt);

                    m_focusCurve1->reEditLine();
                    UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve1->vertexPt);
                }
            }
        }
        linearBing();
    }

    void specifyLinear(NXOpen::BlockStyler::UIBlock* block)
    {
        
        m_linear = nullptr;
        for (int i=0;i<m_linearSet.size();i++)
        {
            int j = 0;
            for (j;j<3;j++)
            {
                if (m_linearSet[i][j] == block)
                {
                    m_linear = m_linearSet[i][j];
                    break;
                }
            }
            if (m_linear)
            {
                if (m_focusCurve1!= (*m_pCurrentMovLines)[i])
                {
                    std::vector<NXOpen::TaggedObject *>temp; 
                    temp.push_back(NXOpen::NXObjectManager::Get((*m_pCurrentMovLines)[i]->curve));
                    m_curvesSelect->SetSelectedObjects(temp);
                    setFlag();//m_focusCurve0;
                              //m_focusCurve1;
                              //m_focusCurve2;
                }
                moveLine(i, j);
                break;
            }
        }
    }

    void breakLineToTwo()
    {
        if (m_status==0&&m_focusCurve1)
        {
            UF_CURVE_line_t lt = {0};
            UF_VEC3_copy(m_focusCurve1->retMidPt(), m_midPt);

            UF_VEC3_copy(m_midPt, lt.start_point);
            UF_VEC3_copy(m_focusCurve1->end_point, lt.end_point);
            UF_VEC3_copy(m_midPt, m_focusCurve1->end_point);
            UF_VEC3_copy(m_midPt, m_focusCurve1->vertexPt);
            m_focusCurve1->reEditLine();
            tag_t tempLine;
            UF_CURVE_create_line(&lt,&tempLine);

            if (m_focusCurve2)
                copyAttr(m_focusCurve2->curve,tempLine);
            else
                copyAttr(m_focusCurve0->curve, tempLine);
            

            m_curves.push_back(new CurveData(tempLine));
            m_tempCurves.push_back(m_curves.back());
            sortCurvesPointor(m_curves, m_Groups);
            for (int i = 0; i < m_Groups.size(); i++)
            {
                m_Groups[i].back()->swap();//两端的修边线方向修正为射向开口端.
            }
            m_pCurrentMovLines = nullptr;
            m_focusCurve1 = nullptr;
            setFlag();
        }
    }
private:
    void moveLine(int i,int j)//j:   0 2 两端,1中间.
    {
        if (j == 0)
        {
            if (m_focusCurve2)
            {
                MyFun::getDirectionPos(m_focusCurve1->dir_Center, m_focusCurve1->start_point, m_linearSet[i][j]->Value(), m_focusCurve1->end_point);
                UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve1->vertexPt);
                m_focusCurve1->reEditLine();
                UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve2->start_point);
                m_focusCurve2->reEditLine();
            }
            else
            {
                MyFun::getDirectionPos(m_focusCurve1->dir_Center, m_focusCurve1->start_point, m_linearSet[i][j]->Value(), m_focusCurve1->end_point);
                UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve1->vertexPt);
                m_focusCurve1->reEditLine();
                UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve0->end_point);
                m_focusCurve0->reEditLine();
            }
        }
        else if(j==1)
        {
            double pt[3];
            MyFun::getDirectionPos(m_linearSet[i][j]->HandleOrientation(), m_linearSet[i][j]->HandleOrigin(), m_linearSet[i][j]->Value(), pt);
            MyFun::intersectPtOfLines_(m_focusCurve1->dir_Center, pt,
                m_focusCurve0->dir_Center,m_focusCurve0->end_point, m_focusCurve1->start_point);
            if (m_status == 3)
            {
                MyFun::intersectPtOfLines_(m_focusCurve1->dir_Center, pt,
                    m_focusCurve2->dir_Center, m_focusCurve2->end_point, m_focusCurve1->end_point);
                UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve2->end_point);
                UF_VEC3_copy(m_focusCurve2->end_point, m_focusCurve2->vertexPt);
            }
            else
            {
                MyFun::intersectPtOfLines_(m_focusCurve1->dir_Center, pt,
                    m_focusCurve2->dir_Center, m_focusCurve2->start_point, m_focusCurve1->end_point);
                UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve2->start_point);
            }
           

            UF_VEC3_copy(m_focusCurve1->end_point, m_focusCurve1->vertexPt);
            m_focusCurve1->reEditLine();

            UF_VEC3_copy(m_focusCurve1->start_point, m_focusCurve0->end_point);
            UF_VEC3_copy(m_focusCurve0->end_point, m_focusCurve0->vertexPt);
            m_focusCurve0->reEditLine();
            m_focusCurve2->reEditLine();
        }
        else
        {
            MyFun::getDirectionPos(-m_focusCurve1->dir_Center, m_focusCurve1->end_point, m_linearSet[i][j]->Value(), m_focusCurve1->start_point);
            m_focusCurve1->reEditLine();
            UF_VEC3_copy(m_focusCurve1->start_point, m_focusCurve0->end_point);
            UF_VEC3_copy(m_focusCurve0->end_point, m_focusCurve0->vertexPt);
            m_focusCurve0->reEditLine();
        }
        linearBing();
        if (m_status)
        {
            m_focusPt = m_midPt;
            UF_VEC3_copy(m_focusCurve1->retMidPt(), m_focusPt);
        }
        else
            m_focusPt = m_focusCurve1->end_point;
        m_manipulator->SetOrigin(m_focusPt);
        if (m_status)
        {
            m_manipulator->SetYAxis(m_focusCurve1->dir_Center);
            double y[3];
            UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
            m_manipulator->SetXAxis(y);
        }
        else
        {
            m_manipulator->SetXAxis(m_focusCurve1->dir_Center);
            double y[3];
            UF_VEC3_cross(m_focusCurve1->dir_Center, m_cutDir, y);
            m_manipulator->SetYAxis(y);
        }
    }

    void linearBing()
    {
        m_linearSet.clear();
        int count_=0;
        std::vector<NXOpen::BlockStyler::LinearDimension*> tempLinear;
        for (int i=0; i <m_pCurrentMovLines->size();i++)
        {
            tempLinear.clear();
            if (i == 0|| i == m_pCurrentMovLines->size() - 1)//起始 终止
            {
                setLinearStatus(m_ppLinears[count_]);
                setLinearData(m_ppLinears[count_], (*m_pCurrentMovLines)[i]->dir_Center, (*m_pCurrentMovLines)[i]->start_point, (*m_pCurrentMovLines)[i]->dist_len);
                tempLinear.push_back(m_ppLinears[count_]);
                count_++;
                tempLinear.push_back(nullptr);
                tempLinear.push_back(nullptr);
                m_linearSet.push_back(tempLinear);
            }
            else if (i == 1)//起始 后一直线
            {
                setLinearStatus(m_ppLinears[count_]);
                setLinearData(m_ppLinears[count_], (*m_pCurrentMovLines)[i]->dir_Center, (*m_pCurrentMovLines)[i]->start_point, (*m_pCurrentMovLines)[i]->dist_len);
                tempLinear.push_back(m_ppLinears[count_]);
                count_++;

                setLinearStatus(m_ppLinears[count_]);
                double tempDir[3];
                UF_VEC3_cross((*m_pCurrentMovLines)[i]->dir_Center,m_cutDir,tempDir);
                setLinearData(m_ppLinears[count_], tempDir, (*m_pCurrentMovLines)[i]->retMidPt(),/* m_ppLinears[count_]->Value()*/0);
                tempLinear.push_back(m_ppLinears[count_]);
                count_++;
                tempLinear.push_back(nullptr);
                m_linearSet.push_back(tempLinear);
            }
            else if(i==m_pCurrentMovLines->size()-2)//终止前一直线
            {
                tempLinear.push_back(nullptr);
                setLinearStatus(m_ppLinears[count_]);
                double tempDir[3];
                UF_VEC3_cross((*m_pCurrentMovLines)[i]->dir_Center, m_cutDir, tempDir);
                setLinearData(m_ppLinears[count_], tempDir, (*m_pCurrentMovLines)[i]->retMidPt(), /*m_ppLinears[count_]->Value()*/0);
                tempLinear.push_back(m_ppLinears[count_]);
                count_++;

                setLinearStatus(m_ppLinears[count_]);
                setLinearData(m_ppLinears[count_], -(*m_pCurrentMovLines)[i]->dir_Center, (*m_pCurrentMovLines)[i]->end_point, (*m_pCurrentMovLines)[i]->dist_len);
                tempLinear.push_back(m_ppLinears[count_]);
                count_++;

                m_linearSet.push_back(tempLinear);
            }
            else//中间的直线
            {
                setLinearStatus(m_ppLinears[count_]);
                setLinearData(m_ppLinears[count_], (*m_pCurrentMovLines)[i]->dir_Center, (*m_pCurrentMovLines)[i]->start_point, (*m_pCurrentMovLines)[i]->dist_len);
                tempLinear.push_back(m_ppLinears[count_]);
                count_++;

                setLinearStatus(m_ppLinears[count_]);
                double tempDir[3];
                UF_VEC3_cross((*m_pCurrentMovLines)[i]->dir_Center, m_cutDir, tempDir);
                setLinearData(m_ppLinears[count_], tempDir, (*m_pCurrentMovLines)[i]->retMidPt(),/* m_ppLinears[count_]->Value()*/0);
                tempLinear.push_back(m_ppLinears[count_]);
                count_++;

                setLinearStatus(m_ppLinears[count_]);
                setLinearData(m_ppLinears[count_], -(*m_pCurrentMovLines)[i]->dir_Center, (*m_pCurrentMovLines)[i]->end_point, (*m_pCurrentMovLines)[i]->dist_len);
                tempLinear.push_back(m_ppLinears[count_]);
                count_++;
         
                m_linearSet.push_back(tempLinear);
            }
        }
    }


    NXOpen::BlockStyler::CurveCollector* m_curvesSelect;
    NXOpen::BlockStyler::SpecifyOrientation* m_manipulator;
    NXOpen::BlockStyler::LinearDimension** m_ppLinears;

    std::vector<CurveData*>m_curves;
    std::vector<CurveData*>m_tempCurves;
    std::vector<std::vector<CurveData*>> m_Groups;

    std::vector<CurveData*>*m_pCurrentMovLines;
    std::vector<std::vector<NXOpen::BlockStyler::LinearDimension*>> m_linearSet;
    NXOpen::BlockStyler::LinearDimension* m_linear;

    //0结束+起始位置end  1两侧修边线 mid 2起始后一位置 mid start 3结束前位置 mid end 4两侧辅助线mid start end
    int m_status;

    CurveData* m_focusCurve0;
    CurveData* m_focusCurve1;
    CurveData* m_focusCurve2;

    double *m_focusPt;
    double m_midPt[3];
    int m_col;
    NXOpen::Vector3d m_cutDir;
};

class GenerateDir
{
public:
    ~GenerateDir()
    {
        DELETE_VECTOR_OBJS(m_tempAxiss);
    }
    GenerateDir(const GenerateDir & g_dir)
    {
        m_dir=g_dir.m_dir;
        m_dir_= g_dir.m_dir_;
        m_axis = g_dir.m_axis;
        is_Include = g_dir.is_Include;
    }
    GenerateDir(const Vector3d &dir_,const double *pt)
    {
       m_axis= createAxis(dir_,pt);
       m_tempAxiss.push_back(m_axis);
       m_dir_=m_dir = dir_;
       is_Include = false;
       m_origin = pt;
    }
    GenerateDir& operator=(const GenerateDir& g_dir)
    {
        if (this!=&g_dir)
        {
            m_dir = g_dir.m_dir;
            m_dir_ = g_dir.m_dir_;
            m_axis = g_dir.m_axis;
            is_Include = g_dir.is_Include;
        }
        return *this;
    }
    bool operator==(const Point3d& pt)
    {
        if (m_origin == pt)
            return true;
        else
            return false;
    }
    bool operator==(const Vector3d & vdir)
    {
        if (m_dir == vdir)
            return true;
        else
            return false;
    }
    bool operator!=(const Vector3d & vdir)
    {
        if (m_dir == vdir)
            return false;
        else
            return true;
    }

    operator tag_t()
    {
        return m_axis;
    }
    operator const tag_t()const 
    {
        return m_axis;
    }
    operator double*()
    {
        return reinterpret_cast<double *>((double*)m_dir);
    }
    operator const double*()const 
    {
        return reinterpret_cast<const double *>((const double*)m_dir);
    }

    void setDir(const double *dir)
    {
        UF_INITIALIZE();
        UF_OBJ_set_blank_status(m_axis,UF_OBJ_BLANKED);
        m_dir = dir;
        m_axis = createAxis(m_dir, m_origin);
        m_tempAxiss.push_back(m_axis);
    }
    bool is_Syntropy(const Vector3d&dir)
    {
        return  m_dir_ == dir;
    }
private:
    GenerateDir()
    {
        is_Include = false;
        m_axis = 0;
    }
private:
    Vector3d m_dir;
    tag_t m_axis;
    vector<tag_t> m_tempAxiss;
    Point3d m_origin;
    bool is_Include;//false(默认值) 为直接短距生成辅助线 true 为长距生成辅助线. int TODO
    Vector3d m_dir_;

};


class ImplEditCurves
{
public:
    ImplEditCurves(NXOpen::BlockStyler::CurveCollector* curvesSelect,NXOpen::BlockStyler::SpecifyVector* cutDir);
    ~ImplEditCurves(); 
    void sortedArcsLines();

    void adjustCurves(const Point3d &temp,const Vector3d &dir)
    {
        int i;
        GenerateDir* tempGenDir = hasOrient(temp, i);
        if (tempGenDir)
        {
            if (*tempGenDir != dir)
            {
                tempGenDir->setDir(dir);
                if (tempGenDir->is_Syntropy(dir))
                    adjustGenerateCurves(i, dir,false);
                else
                    adjustGenerateCurves(i, dir,true);
            }
        }
    }
    void breakLineToTwo()
    {
        if (m_editLines)
            m_editLines->breakLineToTwo();
    }
    void deleteAxisAndSplines(NXOpen::BlockStyler::CurveCollector* curvesSelect, NXOpen::BlockStyler::SpecifyOrientation* manipulator, NXOpen::BlockStyler::LinearDimension**ppLinears)
    {
        createEditLines(curvesSelect, manipulator, ppLinears);
        deletePointerSet(m_generateDirs);
        DELETE_VECTOR_OBJS(m_tempSplines);
    }
    void specifyLinear(NXOpen::BlockStyler::UIBlock* block)
    {
        if(m_editLines)
            m_editLines->specifyLinear(block);
    }
    void moveOper()
    {
        m_editLines->moveOper();
    }

    void  setFlag()
    {
        m_editLines->setFlag();
    }

private:

    void setCurvesCol(std::vector<CurveData*> &trimCurvesGroup, int col)
    {
        UF_INITIALIZE();
        for (int i=0; i<trimCurvesGroup.size();i++)
                UF_OBJ_set_color(trimCurvesGroup[i]->curve,col);
    }

    void createEditLines(NXOpen::BlockStyler::CurveCollector* curvesSelect, NXOpen::BlockStyler::SpecifyOrientation* manipulator, NXOpen::BlockStyler::LinearDimension**ppLinears)
    {
        m_editLines = new EditLines(m_trimCurveDataUP, m_trimCurveDataDW,curvesSelect,manipulator, m_cutDir, ppLinears);
    }

    GenerateDir* hasOrient(const Point3d& Orientation,int &index_)
    {
        m_index = 0;
        for (index_=0;index_<m_generateDirs.size();index_++)
        {
            if ( *m_generateDirs[index_]==Orientation )
            {
                return m_generateDirs[index_];
            }
        }
        return nullptr;
    }

    void  adjustGenerateCurves(int i, Vector3d genDir,bool isCoverUp=false);
    void deleteTempCDVSET()
    {//用于编辑完辅助线后调用删除内存空间
        if (m_tempCDVSET)
        {
            for (int i = 0; i < m_trimCurvesGroupUP.size(); i++)
                delete[] m_tempCDVSET[i][0];
            delete[] m_tempCDVSET;
            m_tempCDVSET = nullptr;
        }
    }
    void autoDirBing();

    template<class T>
    void deletePointerSet(vector<T*>&temp);
    bool hasSpline();
    void simplifySpline();
    void GenerateAssistLines();
    void joinSpline(vector<tag_t> &temp);

protected:
    std::vector<CurveData*> m_tempTrimCurves;                      //need delete
    std::vector<vector<CurveData*>> m_trimCurvesGroupUP;        //存指针需要delete，存对象不用delete
    std::vector<vector<CurveData*>> m_trimCurvesGroupDW;

    std::vector<GenerateDir*> m_generateDirs;
    int m_index;

    std::vector<tag_t> m_allTrimCurves;

    std::vector<CurveData*> m_tempAssistCurves;                    //need delete 加入两条修边线
    std::vector<vector<CurveData*>*> m_assistCurvesTempGroup;   //need delete!

    std::vector<TrimCurveData> m_trimCurveDataUP;
    std::vector<TrimCurveData> m_trimCurveDataDW;

    std::vector<vector<CurveData*>> m_assistCurvesGroup;
    std::vector<tag_t> m_tempSplines;
    std::vector<CurveData*> m_tempCurves;


    EditLines* m_editLines;
    NXOpen::BlockStyler::CurveCollector* m_uiCurvesSelect; 
    NXOpen::BlockStyler::SpecifyVector* m_uiCutDir;
    vector<CurveData*>* m_tempCDVSET;
    CurveAttrE m_attr;
    CurveData* m_pCur_CD;
    CurveData* m_pPost_CD;
    CurveData m_editCD1;
    CurveData m_editCD2;

    Vector3d m_cutDir;
    bool m_hasSpline;
    int m_color;
};
#endif