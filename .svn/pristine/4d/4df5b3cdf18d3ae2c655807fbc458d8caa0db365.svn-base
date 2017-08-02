#include <uf.h>
#include <uf_defs.h>
#include <uf_vec.h>
#include <math.h>
#include <cstdio>
#include <uf_obj.h>
#include <uf_modl.h>
#include <uf_modl_curves.h>
#include <uf_ui_ugopen.h>
#include "SortCurves.h"
#include "uf_ui.h"
#define  BYTE_4(x) ((0x00000000ffffffff)&(x))
#define UF_INITIALIZE() \
            UF_terminate();\
            UF_initialize();

int report_error(char *file, int line, char *call, int irc);
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))
#define DELETE_OBJ(tempObj) do{\
   if (UF_OBJ_ask_status(tempObj) != UF_OBJ_DELETED)\
{\
UF_CALL(UF_OBJ_delete_object(tempObj));\
}\
}while(0);
int report_error(char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133],
            msg[133];

        sprintf_s(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
            irc, line, file);
        UF_get_fail_message(irc, err);

        UF_print_syslog(msg, FALSE);
        UF_print_syslog(err, FALSE);
        UF_print_syslog("\n", FALSE);
        UF_print_syslog(call, FALSE);
        UF_print_syslog(";\n", FALSE);

        if (!UF_UI_open_listing_window())
        {
            UF_UI_write_listing_window(msg);
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(call);
            UF_UI_write_listing_window(";\n");
        }
    }

    return(irc);
}

double get_line_length(tag_t line, double * start_pt=NULL, double * end_pt=NULL)
{
    UF_CURVE_line_t line_coords;
    UF_CALL(UF_CURVE_ask_line_data(line, &line_coords));
    if (start_pt) UF_VEC3_copy(line_coords.start_point, start_pt);
    if (end_pt) UF_VEC3_copy(line_coords.end_point, end_pt);
    double length;
    UF_VEC3_distance(line_coords.start_point, line_coords.end_point, &length);
    return length;
}

double get_arc_length(tag_t arc, double * centerPt=NULL, double * radius=NULL)
{
    //UF_EVAL_p_t evaluator;
    //UF_EVAL_arc_t eval_arc;
    UF_CURVE_arc_t arc_coords;
    double arc_length;
    //int is_open = 0;
    //double param_range[2] = { 0,0 };
    //UF_EVAL_initialize(arc, &evaluator);
    //UF_EVAL_ask_arc(evaluator, &eval_arc);
    UF_CALL(UF_CURVE_ask_arc_data(arc, &arc_coords));
    //UF_CURVE_ask_arc_length(arc,eval_arc.limits[0],eval_arc.limits[1],UF_MODL_MMETER ,&arc_length);
    //UF_CURVE_ask_parameterization(arc, param_range, &is_open);
    if (centerPt) UF_VEC3_copy(arc_coords.arc_center, centerPt);
    if (radius) *radius = arc_coords.radius;

    double angle_;
    if (arc_coords.start_angle > arc_coords.end_angle)
    {
        angle_ = 2 * PI - arc_coords.start_angle + arc_coords.end_angle;
    }
    else
    {
        angle_ = arc_coords.end_angle - arc_coords.start_angle;
    }
    arc_length = 2 * PI * arc_coords.radius *  angle_ / (PI * 2);
    //UF_CURVE_ask_arc_length(arc,param_range[0],param_range[1],UF_MODL_MMETER ,&arc_length);
    return arc_length;
}

double get_spline_length(tag_t& spline, bool is_Edge = false)
{
    int num = 0, type = 0, subtype = 0;
    tag_p_t segments = NULL;
    tag_t curve_ = 0;
    double spline_length = 0;
    UF_initialize();
    UF_OBJ_ask_type_and_subtype(spline, &type, &subtype);
    if (is_Edge)
        UF_CALL(UF_MODL_create_curve_from_edge(spline, &curve_));
    else
        curve_ = spline;
    UF_CALL(UF_CURVE_create_simplified_curve(1, &curve_, 0.0254, &num, &segments));
    if (segments)
        for (int j = 0; j < num; j++)
        {
            UF_OBJ_ask_type_and_subtype(segments[j], &type, &subtype);
            if (type == UF_line_type)
                spline_length = spline_length + get_line_length(segments[j]);
            else
                spline_length = spline_length + get_arc_length(segments[j]);
            DELETE_OBJ(segments[j]);
        }

    int *statuses = 0;
    if (segments)
        UF_free(segments);
    if (is_Edge)
        DELETE_OBJ(curve_);

    return spline_length;
}

double* getCurvePt(const tag_t curve, double param, double * pt)
{
    double tangent1[3] = { 0 };
    double p_norm1[3] = { 0 };
    double b_norm1[3] = { 0 };
    double torsion = 0;
    double rad_of_cur = 0;
    UF_initialize();
    UF_CALL(UF_MODL_ask_curve_props(curve, param, pt, tangent1, p_norm1, b_norm1, &torsion, &rad_of_cur));
    return pt;
}

bool is_Equal(const double *one_3, const double *two_3, const double three_tolerance)
{
    if (one_3 == NULL || two_3 == NULL)
    {
        return false;
    }
    if (fabs((one_3[0] - two_3[0])) <= three_tolerance)
        if (fabs((one_3[1] - two_3[1])) <= three_tolerance)
            if (fabs((one_3[2] - two_3[2])) <= three_tolerance)
                return true;
            else
                return false;
        else
            return false;
    else
        return false;
}

inline void vec3Swp(double * a, double *b)
{
    double temp[3];
    UF_VEC3_copy(a, temp);
    UF_VEC3_copy(b, a);
    UF_VEC3_copy(temp, b);
}

double angleOfVectors(const double *dir1, const double *dir2)
{
    double dir3[3] = { 0 };
    double angle = .0;
    //UF_VEC3_unitize(dir1, .0000001, &angle, dir1);
    //UF_VEC3_unitize(dir2, .0000001, &angle, dir2);
    UF_VEC3_cross(dir1, dir2, dir3);
    //UF_VEC3_unitize(dir3, .0000001, &angle, dir3);
    UF_VEC3_angle_between(dir1, dir2, dir3, &angle);
    if (angle > PI)
    {
        angle = 2 * PI - angle;
    }
    return angle;
}

void is_parallel(const double one_3[3], const double two_3[3], double angle__, int * rt)
{
    double angle_ = angleOfVectors(one_3, two_3);
    if (angle_ != PI)
    {
        if (angle_ > PI)
            angle_ = 2 * PI - angle_;
        if (angle_ > PI / 2)
            angle_ = PI - angle_;

        if (angle_ > angle__)
            *rt = 0;
        else
            *rt = 1;
    }
    else
    {
        *rt = 1;
    }
}

double Max_D_value(const double *one_3, const double *two_3)
{
    double is_Equal_ = 99999.99999;
    double is_Equal__;
    if (one_3 == NULL || two_3 == NULL)
        return is_Equal_;
    if ((is_Equal_ = fabs(one_3[0] - two_3[0])) < (is_Equal__ = fabs(one_3[1] - two_3[1])))
    {
        is_Equal_ = is_Equal__;
        if (is_Equal_ < (is_Equal__ = fabs(one_3[2] - two_3[2])))
            return is_Equal__;
    }
    else
    {
        if (is_Equal_ < (is_Equal__ = fabs(one_3[2] - two_3[2])))
            return is_Equal__;
    }
    return is_Equal_;
}

void editCurveData(const CurveData& oldCD, CurveData &newCD) //cd1 和 cd2 收尾相接
{
    UF_INITIALIZE();
    double pt[3];
    if (newCD.type_ == UF_line_type)
    {
        UF_VEC3_copy(oldCD.vertexPt, newCD.start_point);
        UF_CURVE_edit_line_data(newCD.curve, (UF_CURVE_line_p_t)&newCD.start_point);
        newCD.SetData();
        if (is_Equal(oldCD.vertexPt, newCD.vertexPt, 0.001))
            newCD.swap();
    }
    else if (newCD.type_ == UF_circle_type)
    {
        UF_VEC3_copy(oldCD.vertexPt, newCD.start_point);
        getCurvePt(newCD.curve, 0.5, pt);
        UF_INITIALIZE();
        UF_CALL(UF_OBJ_delete_object(newCD.curve));
        UF_CURVE_create_arc_thru_3pts(1, newCD.start_point, pt, newCD.end_point, &newCD.curve);
        newCD.SetData();
        if (is_Equal(oldCD.vertexPt, newCD.vertexPt, 0.001))
            newCD.swap();
    }
    UF_OBJ_set_color(newCD.curve, 36);
    UF_OBJ_set_color(oldCD.curve, 26);
}

void ArcTranslateLine(vector<vector<CurveData*>>&temp_,double tolerance /*= 0.005*/)
{
    int type_ = 0;
    int subType_ = 0;

    UF_CURVE_line_t lt = { 0 };
    UF_initialize();
    double angle;
    double maxRadius;
    for (int i = 0; i < temp_.size(); i++)
    {
        for (int j = 0; j < temp_[i].size(); j++)
        {
            if (temp_[i][j]->type_ == UF_circle_type)
            {
                angle = (temp_[i][j]->dist_len / temp_[i][j]->radius) / 2;
                maxRadius = tolerance / (1 - cos(angle));
                if (temp_[i][j]->radius <= maxRadius /*&& temp_[i][j]->dist_len > 10*/)
                {
                    DELETE_OBJ(temp_[i][j]->curve);
                    UF_VEC3_copy(temp_[i][j]->start_point, lt.start_point);
                    UF_VEC3_copy(temp_[i][j]->end_point, lt.end_point);
                    UF_CALL(UF_CURVE_create_line(&lt, &temp_[i][j]->curve));
                    temp_[i][j]->type_ = UF_line_type;
                    UF_VEC3_sub(temp_[i][j]->end_point, temp_[i][j]->start_point, temp_[i][j]->dir_Center);
                }
            }
        }
    }
    UF_terminate();
}

void stdProcessCurves(vector<vector<CurveData*>>&temp_)
{
    int count_1 = 0;
    int count_2 = 1;
    int count_3 = 0;
    int currType = 0;

    vector<CurveData*>::iterator it;
    vector<CurveData*>::iterator it_start;
    vector<CurveData*>::iterator it_start_;
    vector<CurveData*>::iterator it_end;
    UF_CURVE_line_t line_coords = { 0 };
    int isParallel = 0;
    double pt[3] = { 0 };
    UF_initialize();

    double dist_center = 0;
    double avrR = 0;
    // 	MyFun::resetUpdata();
    // 	Sleep(1000);
    for (int i = 0; i < temp_.size(); i++)
    {
        currType = temp_[i][0]->type_;
        count_1 = 0;
        for (it_start_ = it = temp_[i].begin(); it != temp_[i].end(); it++)
        {
            if (((currType == (*it)->type_ /*|| (it->dist_len < 0.2&&it->type_==UF_line_type)*/)) && (*it)->curve != temp_[i].back()->curve)
            {
                count_1++;
            }
            else
            {
                if (count_1 >= 2 && currType == UF_line_type)
                {
                    it = it_start_;
                    count_2 = 1;
                    it_start = it;

                    for (int n = 1; n < count_1; n++)
                    {
                        it++;
                        if ((*it)->type_ == UF_line_type)
                        {
                            if ((*it)->dist_len > 1 && (*it)->dist_len <= 10)
                            {
                                is_parallel((*it_start)->dir_Center, (*it)->dir_Center, PI / 24, &isParallel);
                            }
                            else if ((*it)->dist_len <= 1 && (*it)->dist_len > 0.2)
                            {
                                is_parallel((*it_start)->dir_Center, (*it)->dir_Center, PI / 12, &isParallel);
                            }
                            else if ((*it)->dist_len <= 0.2)
                            {
                                is_parallel((*it_start)->dir_Center, (*it)->dir_Center, PI / 6, &isParallel);
                            }
                            else
                            {
                                is_parallel((*it_start)->dir_Center, (*it)->dir_Center, PI / 48, &isParallel);
                            }
                        }
                        else
                        {
                            isParallel = 0;
                        }

                        if (isParallel || (*it)->dist_len < 0.2)
                        {
                            count_2++;
                        }
                        else
                        {
                            if (count_2 > 1)
                            {
                                it_end = it_start;
                                UF_OBJ_delete_object((*it_end)->curve);
                                for (int j = 1; j < count_2; j++)
                                {
                                    it_end++;
                                    UF_OBJ_delete_object((*it_end)->curve);
                                }

                                UF_VEC3_copy((*it_start)->start_point, line_coords.start_point);
                                UF_VEC3_copy((*it_end)->end_point, line_coords.end_point);
                                UF_CALL(UF_CURVE_create_line(&line_coords, &(*it_start)->curve));
                                it_end = it = it_start;
                                it_end++;
                                for (int j = 1; j < count_2; j++)
                                {
                                    temp_[i].erase(it_end);
                                }
                                (*it_start)->SetData();
                                if (is_Equal((*it_end)->start_point, (*it_start)->start_point, 0.001))
                                    (*it_start)->swap();
                                it++;

                            }
                            count_2 = 1;
                            it_start = it;
                            // 							MyFun::resetUpdata();
                            // 							Sleep(1000);
                        }
                    }
                    if (count_2 > 1)
                    {
                        it_end = it_start;
                        UF_OBJ_delete_object((*it_end)->curve);
                        for (int j = 1; j < count_2; j++)
                        {
                            it_end++;
                            UF_OBJ_delete_object((*it_end)->curve);
                        }

                        UF_VEC3_copy((*it_start)->start_point, line_coords.start_point);
                        UF_VEC3_copy((*it_end)->end_point, line_coords.end_point);
                        UF_CALL(UF_CURVE_create_line(&line_coords, &(*it_start)->curve));
                        it_end = it = it_start;
                        it_end++;
                        for (int j = 1; j < count_2; j++)//
                        {
                            temp_[i].erase(it_end);
                        }
                        (*it_start)->SetData();
                        if (is_Equal((*it_end)->start_point, (*it_start)->start_point, 0.001))
                            (*it_start)->swap();
                        it++;
                    }
                    count_2 = 1;
                    it_start = it;
                    // 					MyFun::resetUpdata();
                    // 					Sleep(1000);
                }
                else if (count_1 >= 2 && currType == UF_circle_type)
                {
                    it = it_start_;
                    count_2 = 1;
                    it_start = it;

                    for (int m = 1; m < count_1; m++)
                    {

                        it++;
                        if ((*it_start)->type_ == (*it)->type_ && (*it)->type_ == UF_circle_type)
                        {
                            UF_VEC3_distance((*it_start)->dir_Center, (*it)->dir_Center, &dist_center);
                            avrR = ((*it_start)->radius + (*it)->radius) / 2;
                        }
                        else
                        {
                            dist_center = avrR = 0;
                        }


                        isParallel = 0;
                        if (((*it)->dist_len < 5 || (*it_start)->dist_len < 5) && dist_center < avrR)
                        {
                            isParallel = 1;
                        }
                        else if ((*it)->dist_len > 5 && (*it)->dist_len < 30)
                        {
                            if (dist_center < avrR / 10 && ((*it_start)->dist_len > 5 && (*it_start)->dist_len < 30))
                            {
                                isParallel = 1;
                            }
                            else if (dist_center < avrR / 20 && (*it_start)->dist_len >= 30)
                            {
                                isParallel = 1;
                            }
                        }
                        else
                        {
                            if (dist_center < avrR / 30)
                            {
                                isParallel = 1;
                            }
                        }

                        if ((isParallel || (*it)->dist_len < 0.2) && (*it)->dist_len / (*it)->radius < PI / 3)
                        {
                            count_2++;
                        }
                        else
                        {
                            if (count_2 > 1)
                            {
                                it_end = it_start;
                                UF_OBJ_delete_object((*it_end)->curve);

                                for (int j = 1; j < count_2; j++)
                                {
                                    it_end++;
                                    UF_OBJ_delete_object((*it_end)->curve);
                                }

                                UF_VEC3_copy((*it_start)->start_point, line_coords.start_point);
                                UF_VEC3_copy((*it_start)->end_point, pt);
                                UF_VEC3_copy((*it_end)->end_point, line_coords.end_point);
                                UF_CALL(UF_CURVE_create_arc_thru_3pts(1, line_coords.start_point, pt, line_coords.end_point, &(*it_start)->curve));
                                it_end = it = it_start;
                                it_end++;
                                for (int j = 1; j < count_2; j++)//
                                {
                                    temp_[i].erase(it_end);
                                }
                                (*it_start)->SetData();
                                if (is_Equal((*it_end)->start_point, (*it_start)->start_point, 0.001))
                                    (*it_start)->swap();
                                it++;

                            }
                            count_2 = 1;
                            it_start = it;
                        }
                    }
                    if (count_2 > 1)
                    {
                        it_end = it_start;
                        UF_OBJ_delete_object((*it_end)->curve);

                        for (int j = 1; j < count_2; j++)
                        {
                            it_end++;
                            UF_OBJ_delete_object((*it_end)->curve);
                        }

                        UF_VEC3_copy((*it_start)->start_point, line_coords.start_point);
                        UF_VEC3_copy((*it_start)->end_point, pt);
                        UF_VEC3_copy((*it_end)->end_point, line_coords.end_point);
                        UF_CALL(UF_CURVE_create_arc_thru_3pts(1, line_coords.start_point, pt, line_coords.end_point, &(*it_start)->curve));
                        it_end = it = it_start;
                        it_end++;
                        for (int j = 1; j < count_2; j++)//
                        {
                            temp_[i].erase(it_end);
                        }
                        (*it_start)->SetData();
                        if (is_Equal((*it_end)->start_point, (*it_start)->start_point, 0.001))
                            (*it_start)->swap();
                        it++;

                    }
                    count_2 = 1;
                    it_start = it;
                    // 
                    // 					MyFun::resetUpdata();
                    // 					Sleep(1000);
                }

                /*	if(it->dist_len>1)*/
                currType = (*it)->type_;
                // 				else
                // 				{
                // 					it_start_ = it;
                // 					it++;
                // 					currType = it->type_;
                // 					it = it_start_;
                // 				}

                count_1 = 1;
                it_start_ = it;
            }

        }
    }
    UF_terminate();
}

void sortCurvesPointor(vector<CurveData*> tempDelete, vector<vector<CurveData*>> &rt_simplePro_)
{
    if (tempDelete.size() == 0) return;
    vector<CurveData*>::iterator iter1;
    vector<CurveData*>::iterator iter2;
    UF_initialize();

    CurveData * m_pCurveDataB, *m_pCurveDataA;//已存队列
    m_pCurveDataB = m_pCurveDataA = tempDelete.back();
    tempDelete.pop_back();

    vector<CurveData*> tempA;
    vector<CurveData*> tempB;
    vector<CurveData*> tempAB;
    tempA.push_back(m_pCurveDataA);
    rt_simplePro_.clear();

    //辅助算法变量
    tag_t a;
    tag_t b;

    double tolerance_ = 0.0001;

    if (tempDelete.size() == 0)
    {
        rt_simplePro_.push_back(tempA);
    }

    while (!tempDelete.empty())
    {
        a = m_pCurveDataA->curve;
        for (iter1 = tempDelete.begin(); iter1 != tempDelete.end() && tempDelete.size() > 0; ++iter1)
        {
            while ((*iter1)->dist_len < 0.001&&iter1 != tempDelete.end())
            {
                tempDelete.erase(iter1);
            }
            if (iter1 == tempDelete.end()) break;
            if (is_Equal(m_pCurveDataA->end_point, (*iter1)->start_point, tolerance_))
            {
                m_pCurveDataA = *iter1;
                tempA.push_back(m_pCurveDataA);
                tempDelete.erase(iter1);
                break;
            }
            if (is_Equal(m_pCurveDataA->end_point, (*iter1)->end_point, tolerance_))
            {
                (*iter1)->swap();
                m_pCurveDataA = (*iter1);
                tempA.push_back(m_pCurveDataA);
                tempDelete.erase(iter1);
                break;
            }
        }
        b = m_pCurveDataB->curve;
        for (iter1 = tempDelete.begin(); iter1 != tempDelete.end() && tempDelete.size() > 0; ++iter1)
        {
            while ((*iter1)->dist_len < 0.001&&iter1 != tempDelete.end())
            {
                tempDelete.erase(iter1);
            }
            if (iter1 == tempDelete.end()) break;

            if (is_Equal(m_pCurveDataB->start_point, (*iter1)->start_point, tolerance_))
            {
                (*iter1)->swap();
                m_pCurveDataB = *iter1;
                tempB.push_back(m_pCurveDataB);
                tempDelete.erase(iter1);
                break;
            }
            if (is_Equal(m_pCurveDataB->start_point, (*iter1)->end_point, tolerance_))
            {
                m_pCurveDataB = (*iter1);
                tempB.push_back(m_pCurveDataB);
                tempDelete.erase(iter1);
                break;
            }
        }
        if ((a == m_pCurveDataA->curve&&b == m_pCurveDataB->curve) || tempDelete.empty())
        {
            {//<1可被认为相接 继续
                double dist_1 = 999.9999;
                double dist_2 = 999.9999;
                iter1 = tempDelete.begin();
                bool assignValue = false;

                while (iter1 != tempDelete.end())
                {
                    if (dist_1 > (dist_2 = Max_D_value((*iter1)->start_point, m_pCurveDataA->end_point)))
                    {
                        dist_1 = dist_2;
                        if (dist_1 > (dist_2 = Max_D_value((*iter1)->end_point, m_pCurveDataA->end_point)))
                        {
                            dist_1 = dist_2;
                            (*iter1)->swap();
                        }
                        iter2 = iter1;
                        assignValue = true;
                    }
                    else if (dist_1 > (dist_2 = Max_D_value((*iter1)->end_point, m_pCurveDataA->end_point)))
                    {
                        dist_1 = dist_2;
                        (*iter1)->swap();
                        iter2 = iter1;
                        assignValue = true;
                    }
                    ++iter1;
                }

                if (dist_1 < 1 && assignValue)
                {
                    editCurveData(*m_pCurveDataA, *(*iter2));
                    m_pCurveDataA = (*iter2);
                    tempA.push_back(m_pCurveDataA);
                    tempDelete.erase(iter2);
                }

                dist_1 = 999.9999;
                dist_2 = 999.9999;
                iter1 = tempDelete.begin();
                assignValue = false;
                while (iter1 != tempDelete.end())
                {
                    if (dist_1 > (dist_2 = Max_D_value((*iter1)->start_point, m_pCurveDataB->start_point)))
                    {
                        dist_1 = dist_2;
                        (*iter1)->swap();
                        if (dist_1 > (dist_2 = Max_D_value((*iter1)->end_point, m_pCurveDataB->start_point)))
                        {
                            dist_1 = dist_2;
                        }
                        assignValue = true;
                        iter2 = iter1;
                    }
                    else if (dist_1 > (dist_2 = Max_D_value((*iter1)->end_point, m_pCurveDataB->start_point)))
                    {
                        dist_1 = dist_2;
                        (*iter1)->swap();
                        assignValue = true;
                        iter2 = iter1;
                    }
                    ++iter1;
                }
                if (dist_1 < 1 && assignValue)
                {
                    editCurveData(*m_pCurveDataB, *(*iter2));
                    m_pCurveDataB = (*iter2);
                    tempB.push_back(m_pCurveDataB);
                    tempDelete.erase(iter2);
                }

                if ((a != m_pCurveDataA->curve || b != m_pCurveDataB->curve) && (!tempDelete.empty()))
                    continue;
            }

            if (!tempDelete.empty())//不考虑T形相接的情况
            {

                int num_A = BYTE_4(tempA.size());
                int num_B = BYTE_4(tempB.size());

                for (int i = num_B - 1; i >= 0; i--)
                {
                    tempAB.push_back(tempB[i]);
                }
                for (int i = 0; i < num_A; i++)
                {
                    tempAB.push_back(tempA[i]);
                }


                tempB.clear();
                tempA.clear();

                rt_simplePro_.push_back(tempAB);

                //                 if (tempAB.size() == 1)
                //                 {
                //                     tag_t aaa;
                //                     UF_CURVE_create_point(tempAB[0]->start_point,&aaa);
                //                     UF_OBJ_set_color(aaa, 186);
                //                     UF_CURVE_create_point(tempAB[0]->end_point, &aaa);
                //                     UF_OBJ_set_color(aaa, 186);
                //                 }
                //                 else
                //                 {
                //                     tag_t aaa;
                //                     UF_CURVE_create_point(tempAB[0]->start_point, &aaa);
                //                     UF_OBJ_set_color(aaa, 36);
                //                     UF_CURVE_create_point(tempAB.back()->end_point, &aaa);
                //                     UF_OBJ_set_color(aaa, 36);
                //                 }

                tempAB.clear();

                m_pCurveDataB = m_pCurveDataA = tempDelete.back();
                tempDelete.pop_back();

                if (tempDelete.empty())
                {
                    tempAB.push_back(m_pCurveDataA);
                    rt_simplePro_.push_back(tempAB);
                    break;
                }
                tempA.push_back(m_pCurveDataA);
            }
            else
            {
                int num_A = BYTE_4(tempA.size());
                int num_B = BYTE_4(tempB.size());

                for (int i = num_B - 1; i >= 0; i--)
                {
                    tempAB.push_back(tempB[i]);
                }
                for (int i = 0; i < num_A; i++)
                {
                    tempAB.push_back(tempA[i]);
                }

                tempB.clear();
                tempA.clear();
                rt_simplePro_.push_back(tempAB);

                //                 if (tempAB.size() == 1)
                //                 {
                //                     tag_t aaa;
                //                     UF_CURVE_create_point(tempAB[0]->start_point, &aaa);
                //                     UF_OBJ_set_color(aaa, 186);
                //                     UF_CURVE_create_point(tempAB[0]->end_point, &aaa);
                //                     UF_OBJ_set_color(aaa, 186);
                //                 }
                //                 else
                //                 {
                //                     tag_t aaa;
                //                     UF_CURVE_create_point(tempAB[0]->start_point, &aaa);
                //                     UF_OBJ_set_color(aaa, 36);
                //                     UF_CURVE_create_point(tempAB.back()->end_point, &aaa);
                //                     UF_OBJ_set_color(aaa, 36);
                //                 }

                tempAB.clear();
                break;
            }
        }
    }
    //     for (int i = 0; i < rt_simplePro_.size(); i++)
    //     {
    //         for (int j = 0; j < rt_simplePro_[i].size(); j++)
    //             tempDelete.push_back(rt_simplePro_[i][j]);
    //     }
}

int CurveData::isOnCurve(double * pt) //start_point 1, end_point 2,vertexPt 3,else 0
{
    retMidPt();
    if (is_Equal(start_point, pt, 0.001))
        return 1;
    if (is_Equal(end_point, pt, 0.001))
        return 2;
    if (is_Equal(vertexPt, pt, 0.001))
        return 3;
    return 0;
}

const double * CurveData::retMidPt()
{
    getCurvePt(curve, 0.5, vertexPt);
    return vertexPt;
}

void CurveData::reEditArc(double startPt[3], double pt[3], double endPt[3])
{
    del();
    UF_CALL(UF_CURVE_create_arc_thru_3pts(1, startPt, pt, endPt, &curve));
    SetData();
    UF_VEC3_copy(startPt, start_point);
    UF_VEC3_copy(endPt, end_point);
}

void CurveData::reEditLine(double startPt[3], double endPt[3])
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

CurveData::operator tag_t() const
{
    return this->curve;
}

CurveData::operator tag_t()
{
    return this->curve;
}

CurveData::operator int()
{
    return this->curve;
}

CurveData::operator void*()
{
    return this;
}

bool CurveData::isToAxisArc(const CD & r, double tol) //tol>1
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
    return false;
}

bool CurveData::isConnect(const CD & r) //处理交换端点.
{
    if (is_Equal(r.end_point, start_point, 0.001))
    {
        return true;
    }
    if (is_Equal(r.start_point, start_point, 0.001))
    {
        return true;
    }
    if (is_Equal(r.start_point, end_point, 0.001))
    {
        swap();
        return true;
    }
    if (is_Equal(r.end_point, end_point, 0.001))
    {
        swap();
        return true;
    }
    return false;
}

void CurveData::setVertexPt(double pt[3])
{
    if (pt)
        UF_VEC3_copy(pt, vertexPt);
}

void CurveData::setEndPt(double pt[3])
{
    if (pt)
        UF_VEC3_copy(pt, end_point);
}

void CurveData::setStartPt(double pt[3])
{
    if (pt)
        UF_VEC3_copy(pt, start_point);
}

void CurveData::swap()
{
    vec3Swp(start_point, end_point);
    UF_VEC3_copy(end_point, vertexPt);
    if (type_ == UF_line_type)
        UF_VEC3_sub(end_point, start_point, dir_Center);
}

void CurveData::SetData()
{
    UF_INITIALIZE()
        if (UF_OBJ_ask_status(curve) != UF_OBJ_DELETED)
        {
            int subType_;

            getCurvePt(curve, 0, start_point);
            getCurvePt(curve, 1, end_point);

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
                dist_len = get_line_length(curve, start_point, end_point);
                UF_VEC3_sub(end_point, start_point, dir_Center);
            }
            else if (type_ == UF_circle_type)
                dist_len = get_arc_length(curve, dir_Center, &radius);
            else
                if (baseType == 70)
                    dist_len = get_spline_length(curve, true);
                else
                    dist_len = get_spline_length(curve);
            UF_VEC3_copy(end_point, vertexPt);
        }
        else
        {
            uc1601("输入曲线不存在！", 1);
        }
}

void CurveData::del()
{
    if (UF_OBJ_ask_status(curve) != UF_OBJ_DELETED)
    {
        UF_OBJ_delete_object(curve);
    }
}

void CurveData::setData(tag_t eid)
{
    curve = eid;
    SetData();
}

void CurveData::set0()
{
    memset(&curve, 0, sizeof(CurveData));
}

CurveData& CurveData::operator=(const CD & r)
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

CurveData::~CurveData()
{

}

CurveData::CurveData(const CD & r)
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

CurveData::CurveData(tag_t eid)
{
    setData(eid);
}

CurveData::CurveData()
{
    set0();
}
