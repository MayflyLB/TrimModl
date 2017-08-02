#include "CommonOperation.h"

using namespace MyFun;

tag_t MovLineData::lines1[4] = { 0 };
double MovLineData::ptSet1[12] = { 0 };

void mapWcsToAbs(double wcs[3], double abs_[3]) //Map point from wcs to csys.
{
    UF_CSYS_map_point(UF_CSYS_ROOT_WCS_COORDS, wcs, UF_CSYS_ROOT_COORDS, abs_);
    return;
}

CommonOperation::CommonOperation(const CommonTraits::Struct_Type& ParmData):CommonTraits::Struct_Type(ParmData)
{
    theSession = Session::GetSession();
    workPart = theSession->Parts()->Work();
    m_retOffsetSheet = new RetOffsetSheet(sheetsTag,sheetOffsetDist,offsetDeviation,stepoverDeviation,m_isMove, moveDir, sheetMoveDist);
}

CommonOperation::~CommonOperation()
{
}

vector<tag_t> CommonOperation::BooleanOper(tag_t& extrud, tag_t sheetsSet, tag_t flagT, bool is_sub,
    bool is_del, bool is_ThrowError, bool is_keepTarget, bool is_keepTool)
{
    vector<tag_t> temp;
    if (extrud == 0) return temp;
    if (sheetsSet == 0) return temp;
    UF_INITIALIZE()

        int num = 0;
    tag_t  resulting_bodies = NULL_TAG;

    double pt[3] = { 0 };
    int status = 0;
    tag_t tempT = 0;
    UF_INITIALIZE();
    int type_ = 0;
    int subtype_ = 0;
    double dist = 999.9;
    double dist_ = 999.9;


    Features::BooleanFeature *nullFeatures_BooleanFeature(NULL);
    Features::BooleanBuilder *booleanBuilder1 = workPart->Features()->CreateBooleanBuilderUsingCollector(nullFeatures_BooleanFeature);

    GeometricUtilities::BooleanRegionSelect *booleanRegionSelect1 = booleanBuilder1->BooleanRegionSelect();
    booleanBuilder1->SetTolerance(0.0254);
    if (is_sub)
        booleanBuilder1->SetOperation(Features::Feature::BooleanTypeSubtract);
    else
        booleanBuilder1->SetOperation(Features::Feature::BooleanTypeUnite);

    booleanBuilder1->SetCopyTools(is_keepTool);
    booleanBuilder1->SetCopyTargets(is_keepTarget);

    Body *body1 = (Body*)NXObjectManager::Get(extrud);
    booleanBuilder1->Targets()->Add(body1);
    std::vector<TaggedObject *> targets1(1, body1);
    booleanRegionSelect1->AssignTargets(targets1);////////////////

    ScCollector *scCollector2 = workPart->ScCollectors()->CreateCollector();
    Body *body2 = (Body*)NXObjectManager::Get(sheetsSet);
    std::vector<Body *> bodies1(1, body2);
    BodyDumbRule *bodyDumbRule1 = workPart->ScRuleFactory()->CreateRuleBodyDumb(bodies1);
    std::vector<SelectionIntentRule *> rules1(1, bodyDumbRule1);
    scCollector2->ReplaceRules(rules1, false);
    booleanBuilder1->SetToolBodyCollector(scCollector2);////
    bool isSuccess = true;
    if (is_ThrowError)
    {
        NXObject *nXObject1 = booleanBuilder1->Commit();
        tag_t intersect_feature;
        UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &intersect_feature);
        NXOpen::Features::BodyFeature *body_feat =
            (NXOpen::Features::BodyFeature  *)NXOpen::NXObjectManager::Get(intersect_feature);
        for (int i = 0; i < body_feat->GetBodies().size(); i++)
        {
            UF_OBJ_ask_type_and_subtype(body_feat->GetBodies().at(i)->GetTag(), &type_, &subtype_);
            if (type_ == 70 && subtype_ == 0)
            {
                UF_MODL_ask_body_type(body_feat->GetBodies().at(i)->GetTag(), &type_);
                if (UF_MODL_SOLID_BODY == type_)
                    temp.push_back(body_feat->GetBodies().at(i)->GetTag());
            }
        }
        MyFun::DeleteParms(1, &extrud);
        if (flagT)
            MinDimension(flagT, temp, extrud, pt);
        if (is_del&&flagT)
            for (int i = 0; i < temp.size(); i++)
                if (extrud != temp[i])
                    UF_CALL(UF_OBJ_delete_object(temp[i]));
        booleanBuilder1->Destroy();
        UF_TERMINATE();
        return temp;
    }
    else
    {
        try
        {
            NXObject *nXObject1 = booleanBuilder1->Commit();
            isSuccess = true;
        }
        catch (exception& ex)
        {
            MyFun::disp(ex.what());
            isSuccess = false;
            if (is_keepTool == false)
                /*DELETE_OBJ(sheetsSet);*/
                MyFun::setColorOfBodyFaces(sheetsSet, 186);
        }
        booleanBuilder1->Destroy();
        if (isSuccess)
        {
            tag_t intersect_feature;
            UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &intersect_feature);
            NXOpen::Features::BodyFeature *body_feat =
                (NXOpen::Features::BodyFeature  *)NXOpen::NXObjectManager::Get(intersect_feature);
            for (int i = 0; i < body_feat->GetBodies().size(); i++)
            {
                UF_OBJ_ask_type_and_subtype(body_feat->GetBodies().at(i)->GetTag(), &type_, &subtype_);
                if (type_ == 70 && subtype_ == 0)
                {
                    UF_MODL_ask_body_type(body_feat->GetBodies().at(i)->GetTag(), &type_);
                    if (UF_MODL_SOLID_BODY == type_)
                        temp.push_back(body_feat->GetBodies().at(i)->GetTag());
                }
            }
            MyFun::DeleteParms(1, &extrud);
            MinDimension(flagT, temp, extrud, pt);
            if (is_del)
                for (int i = 0; i < temp.size(); i++)
                    if (extrud != temp[i])
                        UF_CALL(UF_OBJ_delete_object(temp[i]));
        }
        else
        {
            if (UF_OBJ_ask_status(extrud) == UF_OBJ_DELETED)
                uc1601("DeleteD!!!", 1);
            temp.push_back(extrud);
        }
        UF_TERMINATE();
        return temp;
    }

}

vector<tag_t> CommonOperation::BooleanOperUDC_ST(tag_t& extrud, tag_t flagT, tag_t sheetsSet, bool flag, bool isSetEx)
{
    vector<tag_t> temp;
    if (extrud == 0) return temp;
    if (sheetsSet == 0) return temp;
    int num = 0;
    tag_t  resulting_bodies = NULL_TAG;

    double pt[3] = { 0 };
    int status = 0;
    tag_t tempT = 0;
    UF_initialize();
    int type_ = 0;
    int subtype_ = 0;
    double dist = 999.9;
    double dist_ = 999.9;


    Features::BooleanFeature *nullFeatures_BooleanFeature(NULL);
    Features::BooleanBuilder *booleanBuilder1 = workPart->Features()->CreateBooleanBuilderUsingCollector(nullFeatures_BooleanFeature);

    GeometricUtilities::BooleanRegionSelect *booleanRegionSelect1 = booleanBuilder1->BooleanRegionSelect();
    booleanBuilder1->SetTolerance(0.0254);
    booleanBuilder1->SetOperation(Features::Feature::BooleanTypeSubtract);
    booleanBuilder1->SetCopyTools(true);
    booleanBuilder1->SetCopyTargets(false);

    Body *body1 = (Body*)NXObjectManager::Get(extrud);
    booleanBuilder1->Targets()->Add(body1);
    std::vector<TaggedObject *> targets1(1, body1);
    booleanRegionSelect1->AssignTargets(targets1);////////////////

    ScCollector *scCollector2 = workPart->ScCollectors()->CreateCollector();
    Body *body2 = (Body*)NXObjectManager::Get(sheetsSet);
    std::vector<Body *> bodies1(1, body2);
    BodyDumbRule *bodyDumbRule1 = workPart->ScRuleFactory()->CreateRuleBodyDumb(bodies1);
    std::vector<SelectionIntentRule *> rules1(1, bodyDumbRule1);
    scCollector2->ReplaceRules(rules1, false);
    booleanBuilder1->SetToolBodyCollector(scCollector2);////
    bool isSuccess = true;
    if (isSetEx)
    {
        NXObject *nXObject1 = booleanBuilder1->Commit();
        tag_t intersect_feature;
        UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &intersect_feature);
        NXOpen::Features::BodyFeature *body_feat =
            (NXOpen::Features::BodyFeature  *)NXOpen::NXObjectManager::Get(intersect_feature);
        for (int i = 0; i < body_feat->GetBodies().size(); i++)
        {
            UF_OBJ_ask_type_and_subtype(body_feat->GetBodies().at(i)->GetTag(), &type_, &subtype_);
            if (type_ == 70 && subtype_ == 0)
            {
                UF_MODL_ask_body_type(body_feat->GetBodies().at(i)->GetTag(), &type_);
                if (UF_MODL_SOLID_BODY == type_)
                    temp.push_back(body_feat->GetBodies().at(i)->GetTag());
            }
        }
        MyFun::DeleteParms(1, &extrud);
        MinDimension(flagT, temp, extrud, pt);
        if (flag)
        {

            for (int i = 0; i < temp.size(); i++)
            {
                if (extrud != temp[i])
                    UF_CALL(UF_OBJ_delete_object(temp[i]));
            }
        }
        booleanBuilder1->Destroy();
        UF_terminate();
        return temp;
    }
    else
    {
        try
        {
            NXObject *nXObject1 = booleanBuilder1->Commit();
            isSuccess = true;
        }
        catch (exception& ex)
        {
            MyFun::disp(ex.what());
            isSuccess = false;
        }
    }

    booleanBuilder1->Destroy();
    if (isSuccess)
    {
        tag_t intersect_feature;
        UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &intersect_feature);
        NXOpen::Features::BodyFeature *body_feat =
            (NXOpen::Features::BodyFeature  *)NXOpen::NXObjectManager::Get(intersect_feature);
        for (int i = 0; i < body_feat->GetBodies().size(); i++)
        {
            UF_OBJ_ask_type_and_subtype(body_feat->GetBodies().at(i)->GetTag(), &type_, &subtype_);
            if (type_ == 70 && subtype_ == 0)
            {
                UF_MODL_ask_body_type(body_feat->GetBodies().at(i)->GetTag(), &type_);
                if (UF_MODL_SOLID_BODY == type_)
                    temp.push_back(body_feat->GetBodies().at(i)->GetTag());
            }
        }
        MyFun::DeleteParms(1, &extrud);
        MinDimension(flagT, temp, extrud, pt);
        if (flag)
        {

            for (int i = 0; i < temp.size(); i++)
            {
                if (extrud != temp[i])
                    UF_CALL(UF_OBJ_delete_object(temp[i]));
            }
        }
    }
    else
    {
        if (UF_OBJ_ask_status(extrud) == UF_OBJ_DELETED)
        {
            uc1601("DeleteD!!!", 1);
        }
        temp.push_back(extrud);
    }



    //extractFaces();
    UF_terminate();
    return temp;
}

void CommonOperation::extrudBlankBody(tag_t& extrud, tag_t sheetTag)
{
    if (!sheetTag)
    {
        MyFun::disp("拉大片体失败!");
        return;
    }
    double startP = 0;
    double endP = 0;

    if (fabs(cutShapeBlank)<0.1 || isTrimLClose == false)//普通模式+截断模式(看st_deltaCutLen够长就是普通模式,太短就是界面模式) 刃口(只有修边线是开口时,才有截断付型面的情况)
    {
        startP = m_minDist - toolHeight - 2;//去除付型避让长度
        endP = 360 + m_minDist;
    }
    else //付型面为平面->不用布尔运算(),即:|||-(st_isLimitCutLen == true && st_isTrimLClose == true)-|||
    {
        startP = 0;//去除付型避让长度
        endP = toolHeight;
    }

    UF_initialize();
    if (UF_OBJ_ask_status(extrud) != UF_OBJ_DELETED) UF_OBJ_delete_object(extrud);
    extrud = extrudorEx(trimLtag, m_trimVCC[0].a, cutDir, startP, endP, 0, 0);

    if (extrud)
    {
        if (fabs(cutShapeBlank)<0.1 || isTrimLClose == false)//st_isLimitCutLen == true && st_isTrimLClose == true//才是平面生成
        {
            try
            {
                BooleanOper(extrud, sheetTag, trimLtag[0], true, true, true);
            }
            catch (exception&)
            {
                sheetTag = m_retOffsetSheet->retOffsetSheet(0,moveDir, sheetMoveDist,m_isMove, 0.5, 1);
                BooleanOper(extrud, sheetTag, trimLtag[0]);
            }
        }
    }
    UF_terminate();
}

//拉伸体同时预判锻件厚度>=40 拉伸长度
void CommonOperation::extrudBlankBodyUDST(tag_t& extrud, tag_t sheetTag)
{
    if (!sheetTag)
    {
        MyFun::disp("拉大片体失败!");
        return;
    }
    double startP = 0;
    double endP = 0;

    if (fabs(cutShapeBlank)<0.1 || isTrimLClose == false)
    {
        startP = m_minDist - toolHeight - 2;//去除付型避让长度
        endP = cutDirDeltaLen + m_minDist;
    }
    else
    {
        startP = 0;//去除付型避让长度
        endP = toolHeight;
    }

    UF_initialize();
    if (UF_OBJ_ask_status(extrud) != UF_OBJ_DELETED) UF_OBJ_delete_object(extrud);
    extrud = extrudorEx(trimLtag, m_trimVCC[0].a, cutDir, startP, endP, 0, 0);

    if (extrud)
    {
        if (fabs(cutShapeBlank)<0.1 || isTrimLClose == false)
        {
            try
            {
                BooleanOperUDC_ST(extrud, trimLtag[0], sheetTag, true, true);
            }
            catch (exception&)
            {
                sheetTag = m_retOffsetSheet->retOffsetSheet(0,  moveDir, sheetMoveDist, m_isMove, 0.5, 1);
                BooleanOperUDC_ST(extrud, trimLtag[0], sheetTag);
            }

        }
    }

}

void CommonOperation::offsetFace(tag_t face_t, double dist)
{
    Features::Feature *nullFeatures_Feature(NULL);
    Features::OffsetFaceBuilder *offsetFaceBuilder1 = workPart->Features()->CreateOffsetFaceBuilder(nullFeatures_Feature);

    Face *face1 = (Face*)NXObjectManager::Get(face_t);
    std::vector<Face *> boundaryFaces1(0);
    FaceTangentRule *faceTangentRule1 = workPart->ScRuleFactory()->CreateRuleFaceTangent(face1, boundaryFaces1, 0.5);
    std::vector<SelectionIntentRule *> rules1(1);
    rules1[0] = faceTangentRule1;
    offsetFaceBuilder1->FaceCollector()->ReplaceRules(rules1, false);
    char buf[64] = { 0 };
    sprintf(buf, "%f", dist);
    offsetFaceBuilder1->Distance()->SetRightHandSide(buf);
    NXObject *nXObject1;
    nXObject1 = offsetFaceBuilder1->Commit();
    Expression *expression1(offsetFaceBuilder1->Distance());
    offsetFaceBuilder1->Destroy();
}

void CommonOperation::extractFaces(tag_t extrud, vector<tag_t>&sideFaces, vector<tag_t>&cutShapeFaces,vector<tag_t>&floorFaces, vector<tag_t>&backBoard, int col[4]) //提取毛坯的面底面 付型面
{
    UF_initialize();
    uf_list_p_t pt = NULL;
    tag_t temp = 0;
    tag_t pointT = 0;
    double dir[3] = { 0 };
    double point[3] = { 0 };
    int type_ = 0;
    UF_MODL_ask_body_faces(extrud, &pt);
    UF_MODL_ask_list_count(pt, &type_);
    sideFaces.clear();
    cutShapeFaces.clear();
    floorFaces.clear();
    backBoard.clear();
    int a = 0;
    int b = 0;

    UF_OBJ_disp_props_t disp_props = { 0 };

    for (int i = 0; i < type_; i++)
    {
        UF_MODL_ask_list_item(pt, i, &temp);
        MyFun::getFaceNormal(temp, dir);
        UF_VEC3_is_parallel(dir, cutDir, 0.001, &a);
        UF_VEC3_is_perpendicular(dir, cutDir, 0.001, &b);
        if (b)//垂直侧面
        {
            sideFaces.push_back(temp);
            if (col[0])
            {
                UF_OBJ_ask_display_properties(temp, &disp_props);
                if (disp_props.color != 161)
                {
                    UF_OBJ_set_color(temp, col[0]);
                }
            }
        }
        else if (a)//底面可能也会包含付型面
        {
            floorFaces.push_back(temp);
            if (col[2])
            {
                UF_OBJ_set_color(temp, col[2]);
            }
        }
        else//型面
        {
            cutShapeFaces.push_back(temp);
            if (col[1])
            {
                UF_OBJ_set_color(temp, col[1]);
            }
        }

    }
    vector<tag_t>::iterator iter;//sideFaces=>backBoard
    for (int i = 0; i < m_assistLD.size(); i++)
    {
        UF_VEC3_midpt(m_assistLD[i].start_point, m_assistLD[i].end_point, point);
        UF_CURVE_create_point(point, &pointT);
        MinDimension(pointT, sideFaces, temp, point);
        backBoard.push_back(temp);
        if (col[3])
        {
            UF_OBJ_set_color(temp, col[3]);
        }
        for (iter = sideFaces.begin(); iter != sideFaces.end() && sideFaces.size() > 0; iter++)
        {
            if (*iter == temp)
            {
                sideFaces.erase(iter);
                break;
            }
        }
        UF_OBJ_delete_object(pointT);
    }


    if (floorFaces.size() > 1)//取出底面容器中的付型面,放到付型面容器中.
    {
        UF_VEC3_midpt(m_trimLD[0].start_point, m_trimLD[0].end_point, point);
        UF_CURVE_create_point(point, &pointT);
        MinDimension(pointT, floorFaces, temp, point);
        //MaxDimension(m_sheetTag, floorFaces, temp, point);
        for (int i = 0; i < floorFaces.size(); i++)
        {
            if (floorFaces[i] != temp)
            {
                cutShapeFaces.push_back(floorFaces[i]);
                if (col[1])
                {
                    UF_OBJ_set_color(floorFaces[i], col[1]);
                }
            }
        }
        floorFaces.clear();
        floorFaces.push_back(temp);
        UF_OBJ_delete_object(pointT);
    }
    UF_MODL_delete_list(&pt);
    UF_terminate();
}

tag_t CommonOperation::createExtrudeBodyEx(const vccdata &vcc, double thiness, double ext)
{
    UF_initialize();
    if (MyFun::is_Equal(vcc.a.vertexPt, vcc.b.vertexPt, 0.001) != true && vcc.a.curve != vcc.b.curve)
        extrudeVertex(vcc, true, 5);
    for (int i = 0; i < vcc.vect.size(); i++)
    {
        MyFun::DeleteParms(1, &vcc.vect[i]);
    }
    tag_t temp = 0;
    try
    {
        temp = extrudorEx(vcc.vect, vcc.a, cutDir, m_minDist - toolHeight - 2, 100.0 + m_minDist - toolHeight, 1, 2);
        double dist = 0;
        double pt1[3] = { 0 };
        double pt2[3] = { 0 };
        UF_MODL_ask_minimum_dist(temp, m_extrud, 0, NULL, 0, NULL, &dist, pt1, pt2);
        DELETE_OBJ(temp);
        if (dist > 0)
            temp = extrudorEx(vcc.vect, vcc.a, cutDir, m_minDist - toolHeight - 2, 360 + m_minDist, -thiness, ext);
        else
            temp = extrudorEx(vcc.vect, vcc.a, cutDir, m_minDist - toolHeight - 2, 360 + m_minDist, -ext, thiness);
    }
    catch (exception& ex)
    {
        MyFun::disp(ex.what());
        temp = extrudorEx(vcc.vect, vcc.b, cutDir, m_minDist - toolHeight, 100.0 + m_minDist - toolHeight, 1, 2);
        double dist = 0;
        double pt1[3] = { 0 };
        double pt2[3] = { 0 };
        UF_MODL_ask_minimum_dist(temp, m_extrud, 0, NULL, 0, NULL, &dist, pt1, pt2);
        DELETE_OBJ(temp);
        if (dist > 0)
            temp = extrudorEx(vcc.vect, vcc.b, cutDir, m_minDist - toolHeight - 2, 360 + m_minDist, -thiness, ext);
        else
            temp = extrudorEx(vcc.vect, vcc.b, cutDir, m_minDist - toolHeight - 2, 360 + m_minDist, -ext, thiness);
    }
    if (MyFun::is_Equal(vcc.a.vertexPt, vcc.b.vertexPt, 0.001) != true && vcc.a.curve != vcc.b.curve)
        extrudeVertex(vcc, false, 5);
    MyFun::DeleteParms(1, &temp);
    UF_terminate();
    return temp;
}

void CommonOperation::extrudeVertex(const vccdata &vcc, bool flag, double len)
{
    if (!isTrimLClose)
    {
        UF_initialize();
        int a, b;
        UF_OBJ_ask_type_and_subtype(vcc.a.curve, &a, &b);
        if (a != UF_line_type) return;
        UF_OBJ_ask_type_and_subtype(vcc.b.curve, &a, &b);
        if (a != UF_line_type) return;
        UF_CURVE_line_t line_coords = { 0 };
        double dir[3] = { 0 };
        if (vcc.a.curve != vcc.b.curve)
        {
            if (flag)
            {
                UF_VEC3_copy(vcc.a.end_point, const_cast<double*> (vcc.a.vertexPt));
                UF_VEC3_copy(vcc.b.end_point, const_cast<double*> (vcc.b.vertexPt));

                UF_VEC3_sub(vcc.a.end_point, vcc.a.start_point, dir);
                MyFun::getDirectionPos(dir, vcc.a.end_point, len, const_cast<double*> (vcc.a.end_point));

                UF_VEC3_copy(vcc.a.start_point, line_coords.start_point);
                UF_VEC3_copy(vcc.a.end_point, line_coords.end_point);
                UF_CURVE_edit_line_data(vcc.a.curve, &line_coords);

                UF_VEC3_sub(vcc.b.end_point, vcc.b.start_point, dir);
                MyFun::getDirectionPos(dir, vcc.b.end_point, len, const_cast<double*> (vcc.b.end_point));

                UF_VEC3_copy(vcc.b.start_point, line_coords.start_point);
                UF_VEC3_copy(vcc.b.end_point, line_coords.end_point);
                UF_CURVE_edit_line_data(vcc.b.curve, &line_coords);
            }
            else
            {
                UF_VEC3_copy(vcc.a.start_point, line_coords.start_point);
                UF_VEC3_copy(vcc.a.vertexPt, line_coords.end_point);
                UF_VEC3_copy(vcc.a.vertexPt, const_cast<double*> (vcc.a.end_point));
                UF_CURVE_edit_line_data(vcc.a.curve, &line_coords);

                UF_VEC3_copy(vcc.b.start_point, line_coords.start_point);
                UF_VEC3_copy(vcc.b.vertexPt, line_coords.end_point);
                UF_VEC3_copy(vcc.b.vertexPt, const_cast<double*> (vcc.b.end_point));
                UF_CURVE_edit_line_data(vcc.b.curve, &line_coords);

            }
        }
        else if (vcc.a.curve == vcc.b.curve)
        {
            if (flag)
            {
                UF_VEC3_copy(vcc.a.end_point, const_cast<double*> (vcc.a.vertexPt));
                UF_VEC3_copy(vcc.b.end_point, const_cast<double*> (vcc.b.vertexPt));

                UF_VEC3_sub(vcc.a.end_point, vcc.a.start_point, dir);

                MyFun::getDirectionPos(dir, vcc.a.end_point, len, const_cast<double*> (vcc.a.end_point));
                UF_VEC3_copy(vcc.a.end_point, line_coords.end_point);

                UF_VEC3_negate(dir, dir);

                MyFun::getDirectionPos(dir, vcc.b.end_point, len, const_cast<double*> (vcc.b.end_point));
                UF_VEC3_copy(vcc.b.end_point, line_coords.start_point);

                UF_CALL(UF_CURVE_edit_line_data(vcc.b.curve, &line_coords));
            }
            else
            {
                UF_VEC3_copy(vcc.a.vertexPt, line_coords.start_point);
                UF_VEC3_copy(vcc.a.vertexPt, const_cast<double*> (vcc.a.end_point));
                UF_VEC3_copy(vcc.b.vertexPt, line_coords.end_point);
                UF_VEC3_copy(vcc.b.vertexPt, const_cast<double*> (vcc.b.end_point));
                if (UF_OBJ_ask_status(vcc.b.curve) != UF_OBJ_DELETED)
                    UF_CALL(UF_CURVE_edit_line_data(vcc.b.curve, &line_coords));
            }
        }
        UF_terminate();
    }

}

void CommonOperation::createBodyDST()
{

    UF_initialize();
    UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY);
    sheetTag = m_retOffsetSheet->retOffsetSheet(0,moveDir,sheetMoveDist, m_isMove, 1,2);
    m_minDist = measureMinDimensionBCS(m_trimLD, sheetTag, cutDir);//偏置后需要再次求出最小距离
    extrudBlankBodyUDST(m_extrud, sheetTag);
    setBodyColor(m_extrud, col[0], col[1]);	//COL确定修编刀口侧面颜色6以及内部体的颜色129
    UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
    UF_terminate();
}

void CommonOperation::createBodyUST()
{

    UF_initialize();
    UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY);
    sheetTag = m_retOffsetSheet->retOffsetSheet(sheetOffsetDist, moveDir, sheetMoveDist, m_isMove, offsetDeviation, stepoverDeviation);
    m_minDist = measureMinDimensionBCS(m_trimLD, sheetTag, cutDir);//偏置后需要再次求出最小距离
    extrudBlankBodyUDST(m_extrud, sheetTag);
    tag_t temp = 0;
    tag_t feat = 0;

    for (int i = 0; i < m_trimVCC.size() && intervalDist>0; i++)
    {
        temp = createExtrudeBodyEx(m_trimVCC[i], intervalDist);
        UF_MODL_subtract_bodies_with_retained_options(m_extrud, temp, false, false, &feat);
        UF_MODL_ask_feat_body(feat, &m_extrud);
        MyFun::DeleteParms(1, &m_extrud);
    }
    UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
    UF_terminate();
    //MyFun::resetUpdata();
    setBodyColor(m_extrud, col[0], col[1]);	//COL确定修编刀口侧面颜色6以及内部体的颜色129
}

void CommonOperation::createDetailDST()
{
    if (m_extrud)
    {
        UF_initialize();

        //某些参数的特别情况
        vector<tag_t> sideFaces;//侧面
        vector<tag_t> cutShapeFaces;//付型面(切割面)
        vector<tag_t> floorFaces;//底面
        vector<tag_t> backboard;//底面
        int a[4] = { 0,29,129,129 };//COL 修编刀口的颜色 0是不设置颜色
        if (is_0ffsetColor)
        {
            a[1] = 195;
        }
        else
        {
            a[1] = 29;
        }
        extractFaces(m_extrud, sideFaces, cutShapeFaces, floorFaces, backboard, a);

        UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY);


        if (fabs(cutDirToolBlank)>0.1)
        {
            sheetTag = m_retOffsetSheet->retOffsetSheet(- cutDirCutLen,  moveDir, sheetMoveDist, m_isMove, offsetDeviation, stepoverDeviation);
            for (int i = 0; i < m_trimVCC.size(); i++)
            {
                tag_t temp = createExtrudeBodyEx(m_trimVCC[i], /*intervalDist +*/ cutDirToolBlank);
                BooleanOperUDC_ST(temp, m_trimVCC[0].a, sheetTag);
                tag_t feat = 0;
                UF_MODL_subtract_bodies_with_retained_options(m_extrud, temp, false, false, &feat);
                UF_MODL_ask_feat_body(feat, &m_extrud);
                MyFun::DeleteParms(1, &m_extrud);
            }
        }
        UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
        //MyFun::resetUpdata();


        UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY);


        if (fabs(cutShapeBlank)>0.1)//有符型面
        {
            createShapeBlank(129, 129,1);
        }
        UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
        //MyFun::resetUpdata();
       
        UF_terminate();
    } 
    m_extrud = 0;
}

void CommonOperation::createDetailUST()
{
    if (m_extrud)
    {
        UF_initialize();
        //		UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY);
        //某些参数的特别情况
        vector<tag_t> sideFaces;//侧面
        vector<tag_t> cutShapeFaces;//付型面(切割面)
        vector<tag_t> floorFaces;//底面
        vector<tag_t> backboard;//底面
        int a[4] = { 0,29,129,129 };//COL 修编刀口的颜色 0是不设置颜色
        extractFaces(m_extrud, sideFaces, cutShapeFaces, floorFaces, backboard, a);

        if (fabs(cutDirToolBlank)>0.1)
        {
            sheetTag = m_retOffsetSheet->retOffsetSheet(sheetOffsetDist + cutDirCutLen,  moveDir, sheetMoveDist, m_isMove, offsetDeviation, stepoverDeviation);
            for (int i = 0; i < m_trimVCC.size(); i++)
            {
                tag_t temp = createExtrudeBodyEx(m_trimVCC[i], intervalDist + cutDirToolBlank);
                BooleanOperUDC_ST(temp, m_trimVCC[0].a, sheetTag);
                setBodyColor(temp, 129, 129);
                tag_t feat = 0;
                UF_MODL_subtract_bodies_with_retained_options(m_extrud, temp, false, false, &feat);
                UF_MODL_ask_feat_body(feat, &m_extrud);
                MyFun::DeleteParms(1, &m_extrud);
            }
        }

        UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
        //MyFun::resetUpdata();
        UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY);
        if (fabs(cutShapeBlank)>0.1)//有符型面
        {
            createShapeBlank(129, 129, 0);
        }
        UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
        UF_terminate();
        //MyFun::resetUpdata();
    }
    m_extrud = 0;
}

tag_t CommonOperation::createOffsetSheetEx(vccdata vcc)
{
    double dist;
    tag_t joined_curve = NULL;
    tag_t pointSetFeat = 0;
    tag_t tempfitfeat = 0;
    tag_t tempOffsetCurve = 0;

    UF_initialize();
   // UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY);

    CurveData aa;
    tag_t tempSheet = 0;

    double dir[3] = { 0 };
    int type_1, subType_1;
    //TODO


    UF_OBJ_ask_type_and_subtype(vcc.a.curve, &type_1, &subType_1);
    extrudeVertex(vcc, true);

    double dist_ = 0;
    for (int i = 0; i < vcc.vect.size(); i++)
    {
        dist_ += MyFun::get_spline_length(vcc.vect[i]);
    }

    int count_ = static_cast<int>(dist_ / 5.0);
    pointSetFeat = createPointSetFeat(vcc.vect, count_);
    tempfitfeat = MyFun::createFitCureFeat(pointSetFeat);
    UF_CURVE_line_t line_coords = { 0 };

    if (vcc.a.curve != vcc.b.curve || type_1 != UF_line_type)
        tempOffsetCurve = MyFun::offsetCurve(tempfitfeat, cutShapeWide, NULL, false);
    else
    {
        double dir_[3];
        UF_VEC3_sub(vcc.a.end_point, vcc.a.start_point, dir_);
        UF_VEC3_cross(dir_, cutDir, dir);

        MyFun::getCurvePt(vcc.vect[0], 0, line_coords.start_point);
        MyFun::getCurvePt(vcc.vect[0], 1, line_coords.end_point);
        MyFun::getDirectionPos(dir, line_coords.start_point, cutShapeWide*1.5, line_coords.start_point);
        MyFun::getDirectionPos(dir, line_coords.end_point, cutShapeWide*1.5, line_coords.end_point);
        UF_CURVE_edit_line_data(vcc.vect[0], &line_coords);
        tempOffsetCurve = vcc.vect[0];//注意这里使用的是原辅助线或者修边线
    }
    aa.curve = tempOffsetCurve;
    aa.SetData();
    tempSheet = createSheet(vector<tag_t>(1, tempOffsetCurve), aa,
        cutDir, m_minDist - toolHeight - 100, 360 + m_minDist);

    double pt1[3] = { 0 };
    double pt2[3] = { 0 };
    UF_MODL_ask_minimum_dist(tempSheet, m_extrud, 0, NULL, 0, NULL, &dist, pt1, pt2);
    if (dist > 0)
    {
        DELETE_OBJ(tempSheet);

        if (vcc.a.curve != vcc.b.curve || type_1 != UF_line_type)
        {
            DELETE_OBJ(tempOffsetCurve);
            tempOffsetCurve = MyFun::offsetCurve(tempfitfeat, cutShapeWide, NULL, true);
        }
        else
        {
            UF_VEC3_negate(dir, dir);
            MyFun::getDirectionPos(dir, line_coords.start_point, cutShapeWide * 3, line_coords.start_point);
            MyFun::getDirectionPos(dir, line_coords.end_point, cutShapeWide * 3, line_coords.end_point);
            UF_CURVE_edit_line_data(vcc.vect[0], &line_coords);
            tempOffsetCurve = vcc.vect[0];
        }
    }
    DELETE_OBJ(tempSheet);
    DELETE_OBJ(tempfitfeat);
    DELETE_OBJ(pointSetFeat);

    tag_t pointSetFeat_ = MyFun::createPointSetFeat(tempOffsetCurve, count_);
    tag_t tempfitfeat_ = MyFun::createFitCureFeat(pointSetFeat_);
    int num = 0;
    tag_p_t eid = 0;
    UF_CALL(UF_MODL_ask_feat_object(tempfitfeat_, &num, &eid));
    tag_t spline_ = eid[0];
    UF_free(eid);
    std::vector<tag_t> curves__;
    curves__.push_back(spline_);
    splinesProcessToLinesDUST(curves__);
    tempSheet = createSheet(curves__, CurveData(curves__[0]), cutDir, m_minDist - toolHeight - 100, 360 + m_minDist);


    DELETE_OBJ(spline_);
    DELETE_OBJ(tempfitfeat_);
    DELETE_OBJ(pointSetFeat_);
    if (vcc.a.curve != vcc.b.curve)
        DELETE_OBJ(tempOffsetCurve);
    DELETE_OBJ(joined_curve);
    extrudeVertex(vcc, false);
  //  UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
    for (int i = 0; i < curves__.size(); i++)
        DELETE_OBJ(curves__[i]);
    return tempSheet;
}

void CommonOperation::createShapeBlank(int col1, int col2, int flagUD)
{
    UF_initialize();
    vccdata tempVCC;

    int subtype = 0;
    int type_ = 0;
    tag_t tempLine = 0;
    UF_CURVE_line_t lt = { 0 };

    vector<tag_t> targetTs;
    tag_t toolT = 0;

    double strat_point[3] = { 0 };//访问曲线上的起点终点
    double end_point[3] = { 0 };
    double tangent1[3] = { 0 };
    double p_norm1[3] = { 0 };
    double b_norm1[3] = { 0 };
    double torsion = 0;
    double rad_of_cur = 0;

    tag_t  body = 0;
    tag_t shapeBlank = 0;
    double dist = 0;
    if (fabs(cutShapeBlank)>0.1)//有符型面
    {
        UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY);
        if (flagUD)
            extrudBlankBody(body, m_retOffsetSheet->retOffsetSheet(-cutShapeBlank,  moveDir, sheetMoveDist, m_isMove, offsetDeviation, stepoverDeviation));
        else
            extrudBlankBody(body, m_retOffsetSheet->retOffsetSheet(cutShapeBlank + sheetOffsetDist, moveDir, sheetMoveDist, m_isMove, offsetDeviation, stepoverDeviation));
        for (int i = 0; i < m_trimVCC.size(); i++)
        {
            tempVCC.vect.clear();
            tempVCC.a = m_trimVCC[i].a;
            tempVCC.b = m_trimVCC[i].b;

            for (int j = 0; j < m_trimVCC[i].vect.size(); j++)
            {
                UF_OBJ_ask_type_and_subtype(m_trimVCC[i].vect[j], &type_, &subtype);
                if (type_ == UF_line_type)
                {
                    tempVCC.vect.push_back(m_trimVCC[i].vect[j]);
                }
                else
                {
                    UF_MODL_ask_curve_props(m_trimVCC[i].vect[j], 0, strat_point, tangent1, p_norm1, b_norm1, &torsion, &rad_of_cur);
                    UF_MODL_ask_curve_props(m_trimVCC[i].vect[j], 1, end_point, tangent1, p_norm1, b_norm1, &torsion, &rad_of_cur);
                    UF_VEC3_distance(strat_point, end_point, &dist);
                    if (dist > 20)
                    {
                        tempVCC.vect.push_back(m_trimVCC[i].vect[j]);
                        continue;
                    }
                    UF_VEC3_copy(end_point, lt.end_point);
                    UF_VEC3_copy(strat_point, lt.start_point);
                    UF_CURVE_create_line(&lt, &tempLine);
                    tempVCC.vect.push_back(tempLine);
                    m_tempObj.push_back(tempLine);
                }
            }
            Session::UndoMarkId markId1 = theSession->SetUndoMark(Session::MarkVisibilityVisible, "OffsetSheet");
            tag_t sheet_t = createOffsetSheetEx(tempVCC);

            if (sheet_t==0)
            {
                return;
            }
            vector<tag_t> temp = BooleanOper(m_extrud, sheet_t, tempVCC.vect[0], true, false, false);//现在 m_exturd是切割后的外位置的体
            targetTs.push_back(m_extrud);
            if (temp.size() == 2)
            {
                if (temp[0] != m_extrud)
                {
                    m_extrud = temp[0];
                }
                else
                {
                    m_extrud = temp[1];
                }
                if (i == m_trimVCC.size() - 1)
                {
                    UF_OBJ_delete_object(m_extrud);
                }
            }
            else
            {
                uc1601("付型切割结果不是预期!", 1);
                theSession->UndoToMark(markId1, NULL);
                theSession->DeleteUndoMark(markId1, NULL);
                UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
                return;
            }

            vector<tag_t> temp1 = BooleanOper(body, sheet_t, tempVCC.vect[0], true, false, false);
            UF_OBJ_delete_object(sheet_t);

            if (temp1.size() == 2)
            {
                if (temp1[0] != body)
                {
                    shapeBlank = temp1[0];
                }
                else
                {
                    shapeBlank = temp1[1];
                }
                UF_OBJ_delete_object(body);
                body = shapeBlank;
                if (i == m_trimVCC.size() - 1)
                {
                    toolT = body;
                }
            }
            else
            {
                uc1601("付型结果不是预期!", 1);
                theSession->UndoToMark(markId1, NULL);
                theSession->DeleteUndoMark(markId1, NULL);
                shapeBlank = 0;
                UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY);
                return;
            }
            theSession->DeleteUndoMark(markId1, NULL);
        }
        if (targetTs.size() == m_trimVCC.size())
        {
            vector<tag_t> sideFaces1;//侧面
            vector<tag_t> cutShapeFaces1;//付型面(切割面)
            vector<tag_t> floorFaces1;//底面
            vector<tag_t> backboard1;//底面
            int b[4] = { 129,129,129 ,129 };
            extractFaces(toolT, sideFaces1, cutShapeFaces1, floorFaces1, backboard1, b);

            tag_t feat = 0;
            setBodyColor(toolT, col1, col2);
            for (int i = 0; i < targetTs.size(); i++)
            {
                UF_MODL_unite_bodies_with_retained_options(toolT, targetTs[i], false, false, &feat);
                UF_MODL_ask_feat_body(feat, &toolT);
                MyFun::DeleteParms(1, &toolT);
            }
            m_extrud = toolT;
        }
    }

}

void CommonOperation::splinesProcessToLinesDUST(vector<tag_t> &tempSplines) //刚件符型避让用.
{
    //样条打散:
    vector<tag_t> tempVVV;
    tag_p_t segments = NULL;
    int type_, subType, num = 0;
    UF_initialize();
    for (int i = 0; i < tempSplines.size(); i++)
    {
        UF_OBJ_ask_type_and_subtype(tempSplines[i], &type_, &subType);
        if (type_ == UF_line_type || type_ == UF_circle_type)
        {
            tempVVV.push_back(tempSplines[i]);
            //UF_OBJ_set_color(tempSplines[i], 77);
        }
        else
        {
            UF_CALL(UF_CURVE_create_simplified_curve(1, &tempSplines[i], 0.00254, &num, &segments));
            for (int j = 0; j < num; j++)
            {
                tempVVV.push_back(segments[j]);
                //UF_OBJ_set_color(segments[j], 77);
            }
            if (segments)
            {
                UF_free(segments);
                segments = NULL;
            }
        }
    }

    tempSplines = tempVVV;
    tempVVV.clear();
    double dist;
    tag_t tempLine;
    UF_CURVE_line_t lt;
    for (int j = 0; j < tempSplines.size(); j++)//直线变曲线
    {
        UF_OBJ_ask_type_and_subtype(tempSplines[j], &type_, &subType);
        if (UF_OBJ_ask_status(tempSplines[j]) == UF_OBJ_DELETED)
            uc1601("警告:曲线失效", 1);
        if (type_ == UF_line_type)
        {
            tempVVV.push_back(tempSplines[j]);
        }
        else
        {
            MyFun::getCurvePt(tempSplines[j], 0, lt.start_point);
            MyFun::getCurvePt(tempSplines[j], 1, lt.end_point);

            UF_VEC3_distance(lt.start_point, lt.end_point, &dist);
            if (dist >= 20)
            {
                tempVVV.push_back(tempSplines[j]);
                continue;
            }
            DELETE_OBJ(tempSplines[j]);
            UF_CALL(UF_CURVE_create_line(&lt, &tempLine));
            tempVVV.push_back(tempLine);
        }
    }

    //排序处理部分:
    vector<vector<CurveData>> simplePro;
    vector<vccdata> trimVCC;
    vector<CurveData> vertexPts;
    bool isTrimLClose;

    sortCurves(tempVVV, simplePro, trimVCC, vertexPts, isTrimLClose);
    ArcTranslateLine(simplePro);
    stdProcessCurves(simplePro);
    stdProcessCurves(simplePro);
    tempVVV.clear();
    for (int i = 0; i < simplePro.size(); i++)
    {
        for (int j = 0; j < simplePro[i].size(); j++)
            tempVVV.push_back(simplePro[i][j].curve);
    }
    sortCurves(tempVVV, simplePro, trimVCC, vertexPts, isTrimLClose);
    tempSplines = tempVVV;
}


void CommonOperation::filterZFace(vector<tag_t>&faces)
{
    vector<tag_t>::iterator iter;
    double angle = 0;
    double dir[3] = { 0 };
    double dir_z[3] = { 0,0,-1 };
    int a;
    for (iter = faces.begin(); iter != faces.end() && faces.size() > 0; iter++)
    {
        UF_INITIALIZE();
        UF_MODL_ask_face_type((*iter), &a);
        MyFun::getFaceNormal((*iter), dir);
        angle = MyFun::angleOfVectors(dir, dir_z);
        if (angle >= PI / 2 - 0.01 || retCol(*iter) != 129 || a != UF_MODL_PLANAR_FACE)
        {
            faces.erase(iter);
            iter--;
        }
        else
        {
            int a = 0;
        }
    }
}

void CommonOperation::finishTool(tag_t face, vector<tag_t>& bodys_)
{
    //进一步创建工具体

    vector<tag_t> edges;
    double dir_z[3] = { 0,0,-1 };
    uf_list_p_t pt = NULL;
    uf_list_p_t pt_ = NULL;
    uf_list_p_t ft = NULL;

    UF_INITIALIZE();
    UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));
    UF_MODL_create_list(&pt);
    UF_MODL_ask_face_edges(face, &pt);
    pt_ = pt;

    while (pt_)
    {
        edges.push_back(pt_->eid);
        pt_ = pt_->next;
    }
    UF_MODL_delete_list(&pt);
    UF_INITIALIZE();
    UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));
    tag_t tempbody;
    try
    {
        tempbody = extrudorEx(edges, CurveData(edges.back()), dir_z, 0, 500, 0, 0);
    }
    catch (exception& ex)
    {
        char buf[256] = { 0 };
        sprintf(buf, "/****************************\nFile:[%s]\nLineNum:[%d]\nProblem:[%s]\n****************************/\n\r", __FILE__, __LINE__, ex.what());
        MyFun::disp(buf);
        return;
    }

    int colors[2] = { 129,129 };
    setBodyColor(tempbody, colors[0], colors[1]);
    bodys_.push_back(tempbody);
}

void CommonOperation::getCurvesInfo(const TrimCurveData& info)
{
    m_assistLD.clear();
    m_trimLD.clear();
    m_trimVCC.clear();
    trimLtag.clear();
    UF_INITIALIZE();
    vector<vector<CurveData*>> rt_trimCruves;;
    vector<vector<CurveData*>> rt_assistCurves;
    sortCurvesPointor(*info.assist_, rt_assistCurves);
    sortCurvesPointor(*info.trim_, rt_trimCruves);
   
    for (int i = 0; i < rt_trimCruves.size(); i++)
    {
        vccdata temp;
        for (int j = 0; j < rt_trimCruves[i].size(); j++)
        {
            if (rt_trimCruves[i][j]->dist_len<0.01)
                continue;
            trimLtag.push_back(*rt_trimCruves[i][j]);
            m_trimLD.push_back(*rt_trimCruves[i][j]);
            temp.vect.push_back(*rt_trimCruves[i][j]);
        }
        temp.a = *rt_trimCruves[i][0];
        temp.b = *rt_trimCruves[i].back();
        temp.a.swap();
        m_trimVCC.push_back(temp);
    }
    for (int i = 0; i < rt_assistCurves.size(); i++)
    {
        for (int j = 0; j < rt_assistCurves[i].size(); j++)
        {
            if (rt_assistCurves[i][j]->dist_len < 0.01)
                continue;
            trimLtag.push_back(*rt_assistCurves[i][j]);
            m_assistLD.push_back(*rt_assistCurves[i][j]);
        }
    }
    CurveData temp;
    int status;
    for (int i=0;i<m_trimVCC[0].vect.size();i++)
    {
        temp.setData(m_trimVCC[0].vect[i]);
        if (temp.type_==UF_line_type)
        {
            UF_VEC3_is_parallel(m_trimVCC[0].a.dir_Center, temp.dir_Center,0.000001, &status);
            if (status==0)
            {
                UF_VEC3_cross(m_trimVCC[0].a.dir_Center, temp.dir_Center, cutDir);
                break;
            }
        }
    }
}

