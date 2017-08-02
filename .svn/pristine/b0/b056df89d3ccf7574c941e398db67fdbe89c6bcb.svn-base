#include "assistFun.h"


char szTrim[] = "trim";
char szAssist[] = "assist";

double measureMinDimensionBCS(const vector<CurveData>& cdPt, tag_t sheet, double *projectDir)
{
    double transform[16] = { 1.0,0.0,0.0,0.0,
                             0.0,1.0,0.0,0.0,
                             0.0,0.0,1.0,0.0,
                             0.0,0.0,0.0,1.0 };
    double retD = 999.999;
    double tempDelta = 999.999;
    double origin[3] = { 0 };
    int  num_results = 0;
    UF_MODL_ray_hit_point_info_p_t hit_list = NULL;
    if (projectDir[0] == 0 && projectDir[1] == 0 && projectDir[2] == 0) return 0;
    UF_initialize();
    if (UF_OBJ_ask_status(sheet) == UF_OBJ_DELETED) uc1601("sheet已被删除!", 1);
    for (int i = 0; i < cdPt.size(); i++)
    {
        UF_VEC3_midpt(cdPt[i].start_point, cdPt[i].end_point, origin);
        UF_CALL(UF_MODL_trace_a_ray(1, &sheet, origin, projectDir, transform, 1, &num_results, &hit_list));
        if (num_results)
            UF_VEC3_distance(hit_list[0].hit_point, origin, &tempDelta);

        if (retD > tempDelta) retD = tempDelta;
        if (hit_list)
            UF_free(hit_list);

        UF_VEC3_copy(cdPt[i].start_point, origin);
        UF_CALL(UF_MODL_trace_a_ray(1, &sheet, origin, projectDir, transform, 1, &num_results, &hit_list));
        if (num_results)
            UF_VEC3_distance(hit_list[0].hit_point, origin, &tempDelta);
        if (retD > tempDelta) retD = tempDelta;
        if (hit_list)
            UF_free(hit_list);
    }
    if (retD > 200)
    {
        UF_VEC3_negate(projectDir, projectDir);
        for (int i = 0; i < cdPt.size(); i++)
        {
            UF_VEC3_midpt(cdPt[i].start_point, cdPt[i].end_point, origin);
            UF_CALL(UF_MODL_trace_a_ray(1, &sheet, origin, projectDir, transform, 1, &num_results, &hit_list));
            if (num_results)
                UF_VEC3_distance(hit_list[0].hit_point, origin, &tempDelta);

            if (retD > tempDelta) retD = tempDelta;
            if (hit_list)
                UF_free(hit_list);

            UF_VEC3_copy(cdPt[i].start_point, origin);
            UF_CALL(UF_MODL_trace_a_ray(1, &sheet, origin, projectDir, transform, 1, &num_results, &hit_list));
            if (num_results)
                UF_VEC3_distance(hit_list[0].hit_point, origin, &tempDelta);
            if (retD > tempDelta) retD = tempDelta;
            if (hit_list)
                UF_free(hit_list);
        }
    }
    UF_terminate();
    //delete[] bodies;

    return retD;
}

double MinDimension(tag_t obj_, vector<tag_t> tagertObj, tag_t& retBody, double * hitPt)
{
    UF_initialize();
    double retD = 999.9;
    double dist = 0;
    double pt1[3] = { 0 };
    double pt2[3] = { 0 };
    for (int i = 0; i < tagertObj.size(); i++)
    {
        UF_MODL_ask_minimum_dist(obj_, tagertObj[i], 0, NULL, 0, NULL, &dist, pt1, pt2);
        if (retD > dist)
        {
            retD = dist;
            retBody = tagertObj[i];
        }
    }
    UF_terminate();
    return retD;
}

int setBodyColor(tag_t obj, int col_1, int col_2)
{
    UF_initialize();
    int type_ = 0;
    int subType_ = 0;
    UF_OBJ_ask_type_and_subtype(obj, &type_, &subType_);
    if (type_ != 70 && subType_ != 0)
    {
        return -1;
    }
    if (col_1)
        UF_OBJ_set_color(obj, col_1);
    uf_list_p_t pt = NULL;
    tag_t temp = 0;
    UF_MODL_ask_body_faces(obj, &pt);
    UF_MODL_ask_list_count(pt, &type_);
    for (int i = 0; i < type_; i++)
    {
        UF_MODL_ask_list_item(pt, i, &temp);
        if (col_2)
            UF_OBJ_set_color(temp, col_2);
    }
    UF_MODL_delete_list(&pt);
    UF_terminate();
    return 0;
}

size_t getAllSheets(vector<tag_t> & targetTag,const char* sheetname_)
{
    UF_initialize();
    int subtype = 0;
    int type = 0;
    targetTag.clear();
    tag_t obj = NULL_TAG;
    int layer_ = 0;
    UF_LAYER_ask_work_layer(&layer_);
    UF_LAYER_cycle_by_layer(layer_, &obj);
    bool flag = false;
    if (obj == NULL_TAG)
    {
        flag = true;
        UF_LAYER_cycle_by_layer(256, &obj);
    }
    if (sheetname_==NULL)
    {
        sheetname_ = sheetname;
    }
    //UF_VIEW_cycle_objects(0, UF_VIEW_VISIBLE_OBJECTS, &obj);
    char name_[133] = { 0 };
    while (NULL_TAG != obj)
    {
        UF_OBJ_ask_type_and_subtype(obj, &type, &subtype);
        if (type == 70 && subtype == 0)
        {
            UF_MODL_ask_body_type(obj, &subtype);
            if (subtype == UF_MODL_SHEET_BODY)
            {
                UF_OBJ_ask_name(obj, name_);
                if (strncmp(name_, sheetname_, 4) == 0)
                {
                    targetTag.push_back(obj);
                }
            }
        }
        if (flag == false)
        {
            UF_LAYER_cycle_by_layer(layer_, &obj);
            if (obj == NULL_TAG)
            {
                flag = true;
                UF_LAYER_cycle_by_layer(256, &obj);
            }
        }
        else
        {
            flag = true;
            UF_LAYER_cycle_by_layer(256, &obj);
        }
        //UF_VIEW_cycle_objects(0, UF_VIEW_VISIBLE_OBJECTS, &obj);
    }
    UF_terminate();
    return targetTag.size();
}

tag_t extrudorEx(std::vector<tag_t> curves, CurveData dirt, double *dir, double startVertical, double endVertical, double startOffset, double endOffset)
{
    Session *theSession = Session::GetSession();
    Part *workPart(theSession->Parts()->Work());
    Features::Feature *nullFeatures_Feature(NULL);

    if (!workPart->Preferences()->Modeling()->GetHistoryMode())
    {
        throw NXException::Create("Create or edit of a Feature was recorded in History Mode but playback is in History-Free Mode.");
    }
    Features::ExtrudeBuilder *extrudeBuilder1 = workPart->Features()->CreateExtrudeBuilder(nullFeatures_Feature);
    Section *section1 = workPart->Sections()->CreateSection(0.02413, 0.0254, 0.5);
    extrudeBuilder1->SetSection(section1);
    extrudeBuilder1->AllowSelfIntersectingSection(true);

//     UF_initialize();
//     for (int i = 0; i < curves.size();i++)
//         UF_DISP_set_highlight(curves[i],1);

    extrudeBuilder1->SetDistanceTolerance(0.0254);
    std::vector<Body *> targetBodies1(1);////////////////////////设为空
    Body *nullBody(NULL);
    targetBodies1[0] = nullBody;
    extrudeBuilder1->BooleanOperation()->SetTargetBodies(targetBodies1);

    char buf[32] = { 0 };///////////////////////////////////////拉伸尺寸
    sprintf(buf, "%.f", startVertical);
    extrudeBuilder1->Limits()->StartExtend()->Value()->SetRightHandSide(buf);
    sprintf(buf, "%.f", endVertical);
    extrudeBuilder1->Limits()->EndExtend()->Value()->SetRightHandSide(buf);
    extrudeBuilder1->BooleanOperation()->SetType(GeometricUtilities::BooleanOperation::BooleanTypeCreate);

    std::vector<Body *> targetBodies2(1);
    targetBodies2[0] = nullBody;
    extrudeBuilder1->BooleanOperation()->SetTargetBodies(targetBodies2);

    extrudeBuilder1->Draft()->FrontDraftAngle()->SetRightHandSide("2");
    extrudeBuilder1->Draft()->BackDraftAngle()->SetRightHandSide("2");
    extrudeBuilder1->Draft()->SetDraftOption(NXOpen::GeometricUtilities::SimpleDraft::SimpleDraftTypeNoDraft);
    extrudeBuilder1->FeatureOptions()->SetBodyType(GeometricUtilities::FeatureOptions::BodyStyleSolid);
    if (startOffset == 0 && endOffset == 0)
    {
        extrudeBuilder1->Offset()->SetOption(GeometricUtilities::TypeNoOffset);
    }
    else
    {
        extrudeBuilder1->Offset()->SetOption(GeometricUtilities::TypeNonsymmetricOffset);
        sprintf(buf, "%.2f", startOffset);///////////////////////////厚度
        extrudeBuilder1->Offset()->StartOffset()->SetRightHandSide(buf);
        sprintf(buf, "%.2f", endOffset);
        extrudeBuilder1->Offset()->EndOffset()->SetRightHandSide(buf);
    }

    GeometricUtilities::SmartVolumeProfileBuilder *smartVolumeProfileBuilder1 = extrudeBuilder1->SmartVolumeProfile();
    smartVolumeProfileBuilder1->SetOpenProfileSmartVolumeOption(false);
    smartVolumeProfileBuilder1->SetCloseProfileRule(GeometricUtilities::SmartVolumeProfileBuilder::CloseProfileRuleTypeFci);

    section1->SetAllowedEntityTypes(Section::AllowTypesOnlyCurves);

    std::vector<IBaseCurve *> curves1;
    std::vector<Edge *> edges1;

    NXOpen::Spline * spline = NULL;
    NXOpen::Arc * arc_ = NULL;
    NXOpen::Ellipse *ellipse = NULL;
    NXOpen::Hyperbola * htperbola_ = NULL;
    NXOpen::Parabola * parabola = NULL;
    NXOpen::Line *line_ = NULL;
    NXOpen::Edge* edge_ = NULL;
    int type_ = 0;
    int subtype_ = 0;
    int currentType = 0;

    int type_1 = 0;
    int subtype_1 = 0;
    UF_initialize();
    UF_OBJ_ask_type_and_subtype(curves[0], &type_1, &subtype_1);

    for (int i = 0; i < curves.size(); i++)
    {
        UF_OBJ_ask_type_and_subtype(curves[i], &type_, &subtype_);
        if (type_ == 70 && subtype_ == 3)
        {
            if (type_1 != type_ || subtype_1 != subtype_)
            {
                MyFun::disp("输入线型有误!");
                return 0;
            }
        }
        if (type_ == UF_line_type)
        {
            line_ = (NXOpen::Line *)NXObjectManager::Get(curves[i]);
            curves1.push_back(line_);
        }
        else if (type_ == UF_circle_type)
        {
            arc_ = (NXOpen::Arc *)NXObjectManager::Get(curves[i]);
            curves1.push_back(arc_);
        }
        else if (type_ == UF_solid_type&&subtype_ == UF_solid_edge_subtype)
        {
            edge_ = (NXOpen::Edge *)NXObjectManager::Get(curves[i]);
            edges1.push_back(edge_);
        }
        else if (type_ == UF_spline_type)
        {
            spline = (NXOpen::Spline *)NXObjectManager::Get(curves[i]);
            curves1.push_back(spline);
        }
        else if (type_ == UF_conic_type || subtype_ == UF_conic_ellipse_subtype)
        {
            ellipse = (NXOpen::Ellipse *)NXObjectManager::Get(curves[i]);
            curves1.push_back(ellipse);
        }
        else if (type_ == UF_conic_type || subtype_ == UF_conic_parabola_subtype)
        {
            parabola = (NXOpen::Parabola *)NXObjectManager::Get(curves[i]);
            curves1.push_back(parabola);

        }
        else if (type_ == UF_conic_type || subtype_ == UF_conic_hyperbola_subtype)
        {
            htperbola_ = (NXOpen::Hyperbola *)NXObjectManager::Get(curves[i]);
            curves1.push_back(htperbola_);
        }

    }

    {
        EdgeDumbRule *edgeDumbRule1 = NULL;
        std::vector<SelectionIntentRule *> rules1;

        if (type_ == 70)
            rules1.push_back(workPart->ScRuleFactory()->CreateRuleEdgeDumb(edges1));
        else
            rules1.push_back(workPart->ScRuleFactory()->CreateRuleBaseCurveDumb(curves1));
        section1->AllowSelfIntersection(true);
        NXObject *nullNXObject(NULL);
        Point3d helpPoint1((dirt.end_point[0] + dirt.start_point[0]) / 2, (dirt.end_point[1] + dirt.start_point[1]) / 2,
            (dirt.end_point[2] + dirt.start_point[2]) / 2);//////////
        UF_INITIALIZE();
        UF_OBJ_ask_type_and_subtype(dirt.curve, &type_, &subtype_);
        UF_TERMINATE();
        if (type_ == UF_line_type)
        {
            line_ = (NXOpen::Line *)NXObjectManager::Get(dirt.curve);
            section1->AddToSection(rules1, line_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        }
        else if (type_ == UF_circle_type)
        {
            arc_ = (NXOpen::Arc *)NXObjectManager::Get(dirt.curve);
            section1->AddToSection(rules1, arc_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        }
        else if ((type_ == 70 && subtype_ == 3) || type_ == UF_spline_type)
        {
            section1->AddToSection(rules1, nullNXObject, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        }
        else if (type_ == UF_conic_type || subtype_ == UF_conic_ellipse_subtype)
        {
            ellipse = (NXOpen::Ellipse *)NXObjectManager::Get(dirt.curve);
            section1->AddToSection(rules1, ellipse, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        }
        else if (type_ == UF_conic_type || subtype_ == UF_conic_parabola_subtype)
        {
            parabola = (NXOpen::Parabola *)NXObjectManager::Get(dirt.curve);
            section1->AddToSection(rules1, parabola, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        }
        else if (type_ == UF_conic_type || subtype_ == UF_conic_hyperbola_subtype)
        {
            htperbola_ = (NXOpen::Hyperbola *)NXObjectManager::Get(dirt.curve);
            section1->AddToSection(rules1, htperbola_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        }

    }

    MyFun::getDirectionPos(dir, dirt.start_point, startVertical, dirt.start_point);
    Point3d origin1(dirt.start_point[0], dirt.start_point[1], dirt.start_point[2]);/////////////
    Vector3d vector1(dir[0], dir[1], dir[2]);
    Direction *direction1 = workPart->Directions()->CreateDirection(origin1, vector1, SmartObject::UpdateOptionWithinModeling);

    extrudeBuilder1->SetDirection(direction1);
    extrudeBuilder1->SetParentFeatureInternal(false);
    Features::Feature *feature1 = extrudeBuilder1->CommitFeature();//此处异常不用补获,使用者补获
    extrudeBuilder1->Destroy();
    // ----------------------------------------------
    //   Menu: Tools->Journal->Stop Recording
    // ----------------------------------------------
    tag_t tempT = 0;
    if (feature1)
    {
        UF_initialize();

        UF_MODL_ask_feat_body(feature1->GetTag(), &tempT);
        MyFun::DeleteParms(1, &tempT);

    }
    return tempT;
}

tag_t createSheet(const std::vector<tag_t> &curves, CurveData& dirt, double *dir, double startVertical, double endVertical)
{
    Session *theSession = Session::GetSession();
    Part *workPart(theSession->Parts()->Work());

    // ----------------------------------------------
    //   Menu: Insert->Design Feature->Extrude...
    // ----------------------------------------------

    Features::Feature *nullFeatures_Feature(NULL);

    if (!workPart->Preferences()->Modeling()->GetHistoryMode())
    {
        throw NXException::Create("Create or edit of a Feature was recorded in History Mode but playback is in History-Free Mode.");
    }

    Features::ExtrudeBuilder *extrudeBuilder1 = workPart->Features()->CreateExtrudeBuilder(nullFeatures_Feature);

    Section *section1 = workPart->Sections()->CreateSection(0.00095, 0.001, 0.05);
    extrudeBuilder1->SetSection(section1);
    extrudeBuilder1->AllowSelfIntersectingSection(true);
    extrudeBuilder1->SetDistanceTolerance(0.0254);

    char buf[32] = { 0 };///////////////////////////////////////拉伸尺寸
    sprintf(buf, "%.2f", startVertical);
    extrudeBuilder1->Limits()->StartExtend()->Value()->SetRightHandSide(buf);
    sprintf(buf, "%.2f", endVertical);
    extrudeBuilder1->Limits()->EndExtend()->Value()->SetRightHandSide(buf);

    extrudeBuilder1->BooleanOperation()->SetType(GeometricUtilities::BooleanOperation::BooleanTypeCreate);
    extrudeBuilder1->Draft()->SetDraftType(NXOpen::GeometricUtilities::SimpleDraft::SimpleDraftTypeNoDraft);
    extrudeBuilder1->Offset()->SetOption(NXOpen::GeometricUtilities::TypeNoOffset);
    extrudeBuilder1->FeatureOptions()->SetBodyType(GeometricUtilities::FeatureOptions::BodyStyleSheet);


    GeometricUtilities::SmartVolumeProfileBuilder *smartVolumeProfileBuilder1 = extrudeBuilder1->SmartVolumeProfile();
    smartVolumeProfileBuilder1->SetOpenProfileSmartVolumeOption(false);
    smartVolumeProfileBuilder1->SetCloseProfileRule(GeometricUtilities::SmartVolumeProfileBuilder::CloseProfileRuleTypeFci);
    section1->SetDistanceTolerance(0.001);
    section1->SetChainingTolerance(0.00095);
    section1->SetAllowedEntityTypes(Section::AllowTypesOnlyCurves);
    section1->AllowSelfIntersection(true);

    std::vector<IBaseCurve *> curves1(0);
    std::vector<SelectionIntentRule *> rules1;
    NXOpen::Arc *arc1 = NULL;
    NXOpen::Spline *spline1 = NULL;
    int type_ = 0;
    int subtype_ = 0;
    int currentType = 0;
    UF_initialize();
    for (int i = 0; i < curves.size(); i++)
    {
        if (UF_OBJ_ask_status(curves[i]) == UF_OBJ_DELETED)
        {
            uc1601("警告:曲线无效!", 1);
            continue;
        }

        UF_OBJ_ask_type_and_subtype(curves[i], &type_, &subtype_);
        if (type_ == UF_line_type || type_ == UF_circle_type)
        {
            arc1 = (NXOpen::Arc *)(NXObjectManager::Get(curves[i]));
            curves1.push_back(arc1);
        }
        else
        {
            spline1 = (Spline *)(NXObjectManager::Get(curves[i]));
            curves1.push_back(spline1);
        }
        //uc1601((char*)typeid(arc1).name(), 1);
    }
    rules1.push_back(workPart->ScRuleFactory()->CreateRuleBaseCurveDumb(curves1));


    NXObject *nullNXObject(NULL);
    Point3d helpPoint1((dirt.end_point[0] + dirt.start_point[0]) / 2, (dirt.end_point[1] + dirt.start_point[1]) / 2,
        (dirt.end_point[2] + dirt.start_point[2]) / 2);//////////
    section1->AddToSection(rules1, arc1, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);


    MyFun::getDirectionPos(dir, dirt.start_point, startVertical, dirt.start_point);
    Point3d origin1(dirt.start_point[0], dirt.start_point[1], dirt.start_point[2]);/////////////
    Vector3d vector1(dir[0], dir[1], dir[2]);
    Direction *direction1 = workPart->Directions()->CreateDirection(origin1, vector1, SmartObject::UpdateOptionWithinModeling);

    extrudeBuilder1->SetDirection(direction1);

    Unit *unit2 = extrudeBuilder1->Offset()->StartOffset()->Units();
    Expression *expression2 = workPart->Expressions()->CreateSystemExpressionWithUnits("0", unit2);
    //bool success1 = direction1->ReverseDirection();

    extrudeBuilder1->SetDirection(direction1);
    extrudeBuilder1->SetParentFeatureInternal(false);
    Features::Feature *feature1 = NULL;
    try
    {
        feature1 = extrudeBuilder1->CommitFeature();
    }
    catch (exception& ex)
    {
        MyFun::disp(ex.what());
    }
    extrudeBuilder1->Destroy();
    tag_t tempT = 0;
    if (feature1)
    {
        UF_initialize();

        UF_MODL_ask_feat_body(feature1->GetTag(), &tempT);
        MyFun::DeleteParms(1, &tempT);

    }

    return tempT;
}

tag_t offsetFaceSheet(vector<tag_t>sheetfaces, double dist, bool flag)
{
    Session *theSession = Session::GetSession();
    Part *workPart(theSession->Parts()->Work());
    // ----------------------------------------------
    //   Menu: Insert->Offset/Scale->Offset Face...
    // ----------------------------------------------

    Features::Feature *nullFeatures_Feature(NULL);

    if (!workPart->Preferences()->Modeling()->GetHistoryMode())
    {
        throw NXException::Create("Create or edit of a Feature was recorded in History Mode but playback is in History-Free Mode.");
    }
    Features::OffsetFaceBuilder *offsetFaceBuilder1 = workPart->Features()->CreateOffsetFaceBuilder(nullFeatures_Feature);

    char buf[32] = { 0 };
    sprintf(buf, "%.0f", dist);
    offsetFaceBuilder1->Distance()->SetRightHandSide(buf);
    std::vector<Face *> faces8;
    Face* aa = NULL;
    for (int i = 0; i < sheetfaces.size(); i++)
    {
        aa = (Face*)NXObjectManager::Get(sheetfaces[i]);
        faces8.push_back(aa);
    }

    FaceDumbRule *faceDumbRule8 = workPart->ScRuleFactory()->CreateRuleFaceDumb(faces8);
    std::vector<SelectionIntentRule *> rules8(1, faceDumbRule8);
    offsetFaceBuilder1->FaceCollector()->ReplaceRules(rules8, false);
    offsetFaceBuilder1->SetDirection(flag);
    NXObject *nXObject1 = NULL;
    try
    {
        nXObject1 = offsetFaceBuilder1->Commit();
    }
    catch (exception& ex)
    {
        MyFun::disp(ex.what());
    }

    offsetFaceBuilder1->Destroy();
    if (nXObject1) return nXObject1->GetTag();
    else return 0;

}

tag_t offsetSheet(tag_t sheet, double offsetdist, double v1, double v2)
{

    UF_MODL_rough_offset_t ot = { 0 };
    UF_INITIALIZE();
    if (UF_OBJ_ask_status(sheet) == UF_OBJ_DELETED)
    {
        uc1601("片体被删除!", 1);
    }
    ot.entities = &sheet;
    ot.num_entities = 1;
    ot.smart_csys = NULL_TAG;
    char buf[32] = { 0 };
    if (offsetdist==0)
        offsetdist = -0.1;
    sprintf(buf, "%.1f", offsetdist);
    strcpy(ot.offset_distance, buf);
    if (v1)
        sprintf(buf, "%.1f", v1);
    else
        sprintf(buf, "%.1f", 8.0f);

    strcpy(ot.offset_deviation, buf);
    if (v2)
        sprintf(buf, "%.1f", v2);
    else
        sprintf(buf, "%.1f", 16.0f);
    strcpy(ot.stepover_distance, buf);
    ot.surf_method = UF_RSO_SURF_METHOD_ROUGH_FIT;
    ot.surf_ctrl_type = UF_RSO_SURF_CTRL_BY_SYSTEM;
    ot.u_patches = 0;
    ot.boundary_trim = UF_RSO_TRIM_OPT_NO;

    tag_t bodyTag = 0;
    tag_t feat = 0;
    UF_initialize();
    UF_CALL(UF_MODL_create_rough_offset(&ot, &feat));
    UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &feat);
    UF_MODL_ask_feat_body(feat, &bodyTag);
    UF_terminate();
    MyFun::DeleteParms(1, &bodyTag);
    return bodyTag;
}

tag_t sewSheets(vector<tag_t>& sheet_s)
{
    UF_initialize();
    if (sheet_s.size() == 1) return sheet_s[0];
    vector<tag_t>sheets;
    tag_t result_body;
    for (int i=0;i<sheet_s.size();i++)
    {
        uf6511(&sheet_s[i], &result_body);
        sheets.push_back(result_body);
    }
    tag_t bodyTag = 0;
    tag_t feat = 0;
    uf_list_p_t lpt = NULL;
    uf_list_p_t lpt_ = NULL;
    UF_MODL_create_sew(1, 1, &sheets[0], BYTE_4(sheets.size() - 1), &sheets[1], 0.0254, 0, &lpt, &feat);

    UF_MODL_ask_feat_body(feat, &bodyTag);
    MyFun::DeleteParms(1, &bodyTag);
    tag_p_t temp = 0;
    lpt_ = lpt;
    vector<tag_t> tempT;
    while (lpt_)
    {
        if (lpt_->eid)
            tempT.push_back(lpt_->eid);
        lpt_ = lpt_->next;
    }
    UF_MODL_delete_list(&lpt);
    vector<tag_t>::iterator iter;
    bool flag = true;;
    while (flag&&tempT.size())
    {
        flag = false;
        for (iter = tempT.begin(); iter != tempT.end();)
        {
            UF_MODL_create_sew(1, 1, &bodyTag, 1, &*iter, 0.0254, 0, &lpt, &feat);
            if (feat)
            {
                UF_MODL_ask_feat_body(feat, &bodyTag);
                MyFun::DeleteParms(1, &bodyTag);
                tempT.erase(iter);
                flag = true;
            }
            else
            {
                UF_MODL_delete_list(&lpt);
                iter++;
            }
        }
    }
    if (lpt)
        UF_MODL_delete_list(&lpt);
    
    if (tempT.size())
        SHOW_INFO_USR("警告片体缝合异常！");
    return bodyTag;
}

double MaxDimension(tag_t obj_, vector<tag_t> tagertObj, tag_t& retBody, double * hitPt)
{
    UF_initialize();
    double retD = 0;
    double dist = 0;
    double pt1[3] = { 0 };
    double pt2[3] = { 0 };
    for (int i = 0; i < tagertObj.size(); i++)
    {
        UF_MODL_ask_minimum_dist(obj_, tagertObj[i], 0, NULL, 0, NULL, &dist, pt1, pt2);
        if (retD < dist)
        {
            retD = dist;
            retBody = tagertObj[i];
        }
    }
    UF_terminate();
    return retD;
}

tag_t TrimExtendSheet(tag_t sheet)
{
    Session *theSession = Session::GetSession();
    Part *workPart(theSession->Parts()->Work());
    Part *displayPart(theSession->Parts()->Display());

    Features::Feature *nullFeatures_Feature(NULL);
    if (!workPart->Preferences()->Modeling()->GetHistoryMode())
    {
        throw NXException::Create("Create or edit of a Feature was recorded in History Mode but playback is in History-Free Mode.");
    }

    Features::TrimExtendBuilder *trimExtendBuilder1 = workPart->Features()->CreateTrimExtendBuilder(nullFeatures_Feature);
    trimExtendBuilder1->TargetExtendDistance()->SetRightHandSide("100");
    trimExtendBuilder1->ToolExtendDistance()->SetRightHandSide("10");
    UF_initialize();
    uf_list_p_t pt = NULL;
    uf_list_p_t pt2 = NULL;
    tag_t edge_ = 0;
    UF_MODL_ask_body_edges(sheet, &pt);
    int num = 0;
    int num_ = 0;
    UF_MODL_ask_list_count(pt, &num);
    for (int i = 0; i < num; i++)
    {
        UF_MODL_ask_list_item(pt, i, &edge_);
        UF_MODL_ask_edge_faces(edge_, &pt2);
        UF_MODL_ask_list_count(pt2, &num_);
        UF_MODL_delete_list(&pt2);
        if (num_ == 1)
            break;
    }

    UF_MODL_delete_list(&pt);
    Edge *edge1 = (Edge*)NXObjectManager::Get(edge_);
    Edge *nullEdge(NULL);
    Face *nullFace(NULL);
    EdgeChainRule *edgeChainRule1;
    edgeChainRule1 = workPart->ScRuleFactory()->CreateRuleEdgeChain(edge1, nullEdge, false, nullFace, true);

    std::vector<SelectionIntentRule *> rules2(1);
    rules2[0] = edgeChainRule1;
    trimExtendBuilder1->TargetCollector()->ReplaceRules(rules2, false);
    NXObject *nXObject1 = NULL;
    try
    {
        nXObject1 = trimExtendBuilder1->Commit();
    }
    catch (const std::exception&)
    {
        trimExtendBuilder1->TargetExtendDistance()->SetRightHandSide("50");
        trimExtendBuilder1->ToolExtendDistance()->SetRightHandSide("10");
        try
        {
            nXObject1 = trimExtendBuilder1->Commit();
        }
        catch (const std::exception&)
        {
            trimExtendBuilder1->TargetExtendDistance()->SetRightHandSide("25");
            trimExtendBuilder1->ToolExtendDistance()->SetRightHandSide("10");
            try
            {
                nXObject1 = trimExtendBuilder1->Commit();
            }
            catch (const std::exception&)
            {
                trimExtendBuilder1->TargetExtendDistance()->SetRightHandSide("10");
                trimExtendBuilder1->ToolExtendDistance()->SetRightHandSide("10");
                try
                {
                    nXObject1 = trimExtendBuilder1->Commit();
                }
                catch (const std::exception& ex)
                {
                    NX_CATCH_EX_INFO_USR("片体无法正常延长，可能导致无法切割坯体！");
                }
            }
        }
    }

    trimExtendBuilder1->Destroy();
    tag_t temp = 0;
    if (nXObject1)
    {
        UF_MODL_ask_feat_body(nXObject1->GetTag(), &temp);
        MyFun::DeleteParms(1, &temp);
    }
    UF_terminate();
    return temp;
}

bool isContainning(vector<tag_t> curves, tag_t obj_)
{
    UF_initialize();
    int status = 0;
    double pt[3];
    for (int i = 0; i < curves.size(); i++)
    {
        MyFun::getCurvePt(curves[i], 0.5, pt);
        UF_MODL_ask_point_containment(pt, obj_, &status);
        if (status != 2)
        {
            return true;
        }
    }
    UF_terminate();
    return false;
}

bool isContainning(vector<tag_t> obj, double pt[3])
{
    UF_initialize();
    int status = 0;
    for (int i = 0; i < obj.size(); i++)
    {
        UF_MODL_ask_point_containment(pt, obj[i], &status);
        if (status != 2)
        {
            UF_terminate();
            return true;
        }
    }
    UF_terminate();
    return false;
}

void stdProcessCurves_(vector<vector<CurveData>>&temp_)
{
    // 	MyFun::resetUpdata();
    // 	Sleep(1000);
    int currType = 0;
    vector<CurveData>::iterator it_sta;
    vector<CurveData>::iterator it_stas;//以已处理的曲线的后边界为本迭代器的起点
    vector<CurveData>::iterator it_end;
    vector<CurveData>::iterator it_ends;//以存在的曲线为边界,曲线删除,边界前移,曲线创建边界后移
    vector<CurveData>::iterator it_;
    UF_CURVE_line_t line_coords = { 0 };
    int isParallel = 0;
    double pt[3] = { 0 };
    UF_initialize();

    double dist_centers = 0;//圆心距
    double avrR = 0;//;半径均值
    double d_value = 0;//半径差值

    double dist = 0;
    double dist_ = 0;

    double toleranceProcess = 0.5;//长度小于1处理归于任意处理
    bool flag = false;
    for (int i = 0; i < temp_.size(); i++)
    {
        vector<CurveData> &temp_sub = temp_[i];
        currType = temp_sub[0].type_;

        it_end = it_sta = it_ends = it_stas = temp_sub.begin();
        for (it_ends; it_ends != temp_sub.end(); ++it_ends)
        {
            if ((currType == it_ends->type_ && it_ends->curve != temp_sub.back().curve) || it_ends->dist_len <= toleranceProcess)
                continue;
            else
            {
                it_ = it_end = it_sta = it_stas;
                it_++;
                if (it_ends != it_&& currType == UF_line_type)
                {
                    // 					for (it_= it_sta; it_ != it_ends;it_++)
                    // 					{
                    // 						if (it_->type_== UF_line_type)
                    // 							break;
                    // 					}
                    for (it_end; it_end != it_ends; ++it_end)
                    {
                        if (it_end->type_ == UF_line_type)
                        {
                            if (it_end->dist_len > 1 && it_end->dist_len <= 10)
                                is_parallel(it_->dir_Center, it_end->dir_Center, PI / 24, &isParallel);
                            else if (it_end->dist_len <= 1 && it_end->dist_len > 0.2)
                                is_parallel(it_->dir_Center, it_end->dir_Center, PI / 12, &isParallel);
                            else if (it_end->dist_len <= 0.2)
                                is_parallel(it_->dir_Center, it_end->dir_Center, PI / 6, &isParallel);
                            else
                                is_parallel(it_->dir_Center, it_end->dir_Center, PI / 48, &isParallel);
                            it_ = it_end;
                        }
                        else
                            isParallel = 0;

                        if (isParallel || it_end->dist_len <= toleranceProcess)
                            continue;
                        else
                        {
                            if (it_sta != it_end)//当前it_end是不符合要求的
                            {

                                for (it_ = it_sta; it_ != it_end; it_++)
                                {
                                    --it_ends;//移动边界
                                    DELETE_OBJ(it_->curve);
                                }

                                UF_VEC3_copy(it_sta->start_point, line_coords.start_point);
                                UF_VEC3_copy(it_end->start_point, line_coords.end_point);
                                UF_VEC3_copy(it_end->start_point, it_sta->end_point);
                                UF_VEC3_copy(it_sta->end_point, it_sta->vertexPt);
                                flag = true;
                                UF_CALL(UF_CURVE_create_line(&line_coords, &it_sta->curve));
                                ++it_ends;
                                temp_sub.erase(++it_sta, it_end);
                                it_end = --it_sta;
                                ++it_sta;
                                // 								for (it_ = it_sta; it_ != it_ends; it_++)
                                // 								{
                                // 									if (it_->type_ == UF_line_type)
                                // 										break;
                                // 								}
                            }
                        }
                    }
                    if (it_sta != it_end)//当前it_end是不符合要求的
                    {
                        for (it_ = it_sta; it_ != it_end; it_++)
                        {
                            --it_ends;//移动边界
                            DELETE_OBJ(it_->curve);
                        }

                        UF_VEC3_copy(it_sta->start_point, line_coords.start_point);
                        UF_VEC3_copy(it_end->start_point, line_coords.end_point);
                        UF_VEC3_copy(it_end->start_point, it_sta->end_point);
                        UF_VEC3_copy(it_sta->end_point, it_sta->vertexPt);
                        UF_CALL(UF_CURVE_create_line(&line_coords, &it_sta->curve));
                        flag = true;
                        ++it_ends;
                        temp_sub.erase(++it_sta, it_end);
                    }
                }
                else if (it_ends != it_ && currType == UF_circle_type)
                {
                    // 					for (it_ = it_sta; it_ != it_ends; it_++)
                    // 					{
                    // 						if (it_->type_ == UF_circle_type)
                    // 							break;
                    // 					}
                    for (it_end; it_end != it_ends; ++it_end)
                    {
                        if (it_end->type_ == it_->type_)
                        {
                            UF_VEC3_distance(it_end->dir_Center, it_->dir_Center, &dist_centers);//dist_center
                            avrR = (it_end->radius + it_->radius) / 2;//avrR
                            d_value = fabs(it_end->radius - it_->radius);//d_value
                            it_ = it_end;
                        }
                        else
                            dist_centers = avrR = d_value = 0;

                        dist = avrR / 10;

                        isParallel = 0;

                        if (dist_centers == avrR&&avrR == d_value&&d_value == 0)
                            isParallel = 1;
                        else if (dist_centers < dist&& d_value < dist)
                            isParallel = 1;

                        if (isParallel || it_ends->dist_len <= toleranceProcess)
                            continue;
                        else
                        {
                            if (it_end != it_sta)
                            {
                                dist = 0;
                                for (it_ = it_sta; it_ != it_end; it_++)
                                {
                                    --it_ends;//移动边界
                                    DELETE_OBJ(it_->curve);
                                    dist += it_->dist_len;
                                }
                                UF_VEC3_copy(it_sta->start_point, line_coords.start_point);
                                dist /= 2;
                                dist_ = 0;
                                for (it_ = it_sta; it_ != it_end; it_++)
                                {
                                    dist_ += it_->dist_len;
                                    if (dist_ > dist)	break;
                                }
                                UF_VEC3_copy(it_->start_point, pt);
                                UF_VEC3_copy(it_end->start_point, line_coords.end_point);
                                UF_VEC3_copy(it_end->start_point, it_sta->end_point);
                                UF_VEC3_copy(it_sta->end_point, it_sta->vertexPt);
                                UF_CALL(UF_CURVE_create_arc_thru_3pts(1, line_coords.start_point, pt, line_coords.end_point, &it_sta->curve));
                                ++it_ends;
                                temp_sub.erase(++it_sta, it_end);
                                it_end = --it_sta;
                                flag = true;
                                ++it_sta;
                                // 								for (it_ = it_sta; it_ != it_ends; it_++)
                                // 								{
                                // 									if (it_->type_ == UF_circle_type)
                                // 										break;
                                // 								}
                            }
                        }
                    }
                    if (it_end != it_sta)
                    {
                        dist = 0;
                        for (it_ = it_sta; it_ != it_end; it_++)
                        {
                            --it_ends;//移动边界
                            DELETE_OBJ(it_->curve);
                            dist += it_->dist_len;
                        }
                        UF_VEC3_copy(it_sta->start_point, line_coords.start_point);
                        dist /= 2;
                        dist_ = 0;
                        for (it_ = it_sta; it_ != it_end; it_++)
                        {
                            dist_ += it_->dist_len;
                            if (dist_ > dist)	break;
                        }
                        UF_VEC3_copy(it_->start_point, pt);
                        UF_VEC3_copy(it_end->start_point, line_coords.end_point);
                        UF_VEC3_copy(it_end->start_point, it_sta->end_point);
                        UF_VEC3_copy(it_sta->end_point, it_sta->vertexPt);
                        UF_CALL(UF_CURVE_create_arc_thru_3pts(1, line_coords.start_point, pt, line_coords.end_point, &it_sta->curve));
                        flag = true;
                        ++it_ends;
                        temp_sub.erase(++it_sta, it_end);
                    }
                }

                if (it_ends != temp_sub.end())//大于公差才是有效类型
                    currType = it_ends->type_;
                else
                    currType = 0;
                it_stas = it_ends;

                if (flag == true)
                {
                    --it_ends;
                    flag = false;
                }
            }
        }

    }
    UF_terminate();
}

void stdProcessCurves(vector<vector<CurveData>>&temp_)
{
    int count_1 = 0;
    int count_2 = 1;
    int count_3 = 0;
    int currType = 0;
    vector<CurveData>::iterator it;
    vector<CurveData>::iterator it_start;
    vector<CurveData>::iterator it_start_;
    vector<CurveData>::iterator it_end;
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
        currType = temp_[i][0].type_;
        count_1 = 0;
        for (it_start_ = it = temp_[i].begin(); it != temp_[i].end(); it++)
        {
            if (((currType == it->type_ /*|| (it->dist_len < 0.2&&it->type_==UF_line_type)*/)) && it->curve != temp_[i].back().curve)
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
                        if (it->type_ == UF_line_type)
                        {
                            if (it->dist_len > 1 && it->dist_len <= 10)
                            {
                                is_parallel(it_start->dir_Center, it->dir_Center, PI / 24, &isParallel);
                            }
                            else if (it->dist_len <= 1 && it->dist_len > 0.2)
                            {
                                is_parallel(it_start->dir_Center, it->dir_Center, PI / 12, &isParallel);
                            }
                            else if (it->dist_len <= 0.2)
                            {
                                is_parallel(it_start->dir_Center, it->dir_Center, PI / 6, &isParallel);
                            }
                            else
                            {
                                is_parallel(it_start->dir_Center, it->dir_Center, PI / 48, &isParallel);
                            }
                        }
                        else
                        {
                            isParallel = 0;
                        }

                        if (isParallel || it->dist_len < 0.2)
                        {
                            count_2++;
                        }
                        else
                        {
                            if (count_2 > 1)
                            {
                                it_end = it_start;
                                UF_OBJ_delete_object(it_end->curve);
                                for (int j = 1; j < count_2; j++)
                                {
                                    it_end++;
                                    UF_OBJ_delete_object(it_end->curve);
                                }

                                UF_VEC3_copy(it_start->start_point, line_coords.start_point);
                                UF_VEC3_copy(it_end->end_point, line_coords.end_point);
                                UF_CALL(UF_CURVE_create_line(&line_coords, &it_start->curve));
                                it_end = it = it_start;
                                it_end++;
                                for (int j = 1; j < count_2; j++)
                                    temp_[i].erase(it_end);
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
                        UF_OBJ_delete_object(it_end->curve);
                        for (int j = 1; j < count_2; j++)
                        {
                            it_end++;
                            UF_OBJ_delete_object(it_end->curve);
                        }

                        UF_VEC3_copy(it_start->start_point, line_coords.start_point);
                        UF_VEC3_copy(it_end->end_point, line_coords.end_point);
                        UF_CALL(UF_CURVE_create_line(&line_coords, &it_start->curve));
                        it_end = it = it_start;
                        it_end++;
                        for (int j = 1; j < count_2; j++)//
                            temp_[i].erase(it_end);
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
                        if (it_start->type_ == it->type_&&it->type_ == UF_circle_type)
                        {
                            UF_VEC3_distance(it_start->dir_Center, it->dir_Center, &dist_center);
                            avrR = (it_start->radius + it->radius) / 2;
                        }
                        else
                        {
                            dist_center = avrR = 0;
                        }


                        isParallel = 0;
                        if ((it->dist_len < 5 || it_start->dist_len < 5) && dist_center < avrR)
                        {
                            isParallel = 1;
                        }
                        else if (it->dist_len > 5 && it->dist_len < 30)
                        {
                            if (dist_center < avrR / 10 && (it_start->dist_len> 5 && it_start->dist_len < 30))
                            {
                                isParallel = 1;
                            }
                            else if (dist_center < avrR / 20 && it_start->dist_len >= 30)
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

                        if ((isParallel || it->dist_len < 0.2) && it->dist_len / it->radius < PI / 3)
                        {
                            count_2++;
                        }
                        else
                        {
                            if (count_2 > 1)
                            {
                                it_end = it_start;
                                UF_OBJ_delete_object(it_end->curve);

                                for (int j = 1; j < count_2; j++)
                                {
                                    it_end++;
                                    UF_OBJ_delete_object(it_end->curve);
                                }

                                UF_VEC3_copy(it_start->start_point, line_coords.start_point);
                                UF_VEC3_copy(it_start->end_point, pt);
                                UF_VEC3_copy(it_end->end_point, line_coords.end_point);
                                UF_CALL(UF_CURVE_create_arc_thru_3pts(1, line_coords.start_point, pt, line_coords.end_point, &it_start->curve));
                                it_end = it = it_start;
                                it_end++;
                                for (int j = 1; j < count_2; j++)//
                                    temp_[i].erase(it_end);
                                it++;

                            }
                            count_2 = 1;
                            it_start = it;
                        }
                    }
                    if (count_2 > 1)
                    {
                        it_end = it_start;
                        UF_OBJ_delete_object(it_end->curve);

                        for (int j = 1; j < count_2; j++)
                        {
                            it_end++;
                            UF_OBJ_delete_object(it_end->curve);
                        }

                        UF_VEC3_copy(it_start->start_point, line_coords.start_point);
                        UF_VEC3_copy(it_start->end_point, pt);
                        UF_VEC3_copy(it_end->end_point, line_coords.end_point);
                        UF_CALL(UF_CURVE_create_arc_thru_3pts(1, line_coords.start_point, pt, line_coords.end_point, &it_start->curve));
                        it_end = it = it_start;
                        it_end++;
                        for (int j = 1; j < count_2; j++)//
                            temp_[i].erase(it_end);
                        it++;

                    }
                    count_2 = 1;
                    it_start = it;
                    // 
                    // 					MyFun::resetUpdata();
                    // 					Sleep(1000);
                }

                /*	if(it->dist_len>1)*/
                currType = it->type_;
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
                        if ((*it_start)->type_ == (*it)->type_&& (*it)->type_ == UF_circle_type)
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
                            if (dist_center < avrR / 10 && ((*it_start)->dist_len> 5 && (*it_start)->dist_len < 30))
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

void detailProcessCurves(vector<vector<CurveData>>&temp_)
{
    int count_1 = 0;
    int count_2 = 1;
    int count_3 = 0;
    int currType = 0;
    vector<CurveData>::iterator it;
    vector<CurveData>::iterator it_start;
    vector<CurveData>::iterator it_start_;
    vector<CurveData>::iterator it_end;
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
        currType = temp_[i][0].type_;
        count_1 = 0;
        for (it_start_ = it = temp_[i].begin(); it != temp_[i].end(); it++)
        {
            if (((currType == it->type_ /*|| (it->dist_len < 0.2&&it->type_==UF_line_type)*/)) && it->curve != temp_[i].back().curve)
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
                        if (it->type_ == UF_line_type)
                        {
                            if (it->dist_len > 1 && it->dist_len <= 10)
                            {
                                is_parallel(it_start->dir_Center, it->dir_Center, PI / 24, &isParallel);
                            }
                            else if (it->dist_len <= 1 && it->dist_len > 0.2)
                            {
                                is_parallel(it_start->dir_Center, it->dir_Center, PI / 12, &isParallel);
                            }
                            else if (it->dist_len <= 0.2)
                            {
                                is_parallel(it_start->dir_Center, it->dir_Center, PI / 6, &isParallel);
                            }
                            else
                            {
                                is_parallel(it_start->dir_Center, it->dir_Center, PI / 48, &isParallel);
                            }
                        }
                        else
                        {
                            isParallel = 0;
                        }

                        if (isParallel || it->dist_len < 0.2)
                        {
                            count_2++;
                        }
                        else
                        {
                            if (count_2 > 1)
                            {
                                it_end = it_start;
                                UF_OBJ_delete_object(it_end->curve);
                                for (int j = 1; j < count_2; j++)
                                {
                                    it_end++;
                                    UF_OBJ_delete_object(it_end->curve);
                                }

                                UF_VEC3_copy(it_start->start_point, line_coords.start_point);
                                UF_VEC3_copy(it_end->end_point, line_coords.end_point);
                                UF_CALL(UF_CURVE_create_line(&line_coords, &it_start->curve));
                                it_end = it = it_start;
                                it_end++;
                                for (int j = 1; j < count_2; j++)
                                    temp_[i].erase(it_end);
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
                        UF_OBJ_delete_object(it_end->curve);
                        for (int j = 1; j < count_2; j++)
                        {
                            it_end++;
                            UF_OBJ_delete_object(it_end->curve);
                        }

                        UF_VEC3_copy(it_start->start_point, line_coords.start_point);
                        UF_VEC3_copy(it_end->end_point, line_coords.end_point);
                        UF_CALL(UF_CURVE_create_line(&line_coords, &it_start->curve));
                        it_end = it = it_start;
                        it_end++;
                        for (int j = 1; j < count_2; j++)//
                            temp_[i].erase(it_end);
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
                        if (it_start->type_ == it->type_&&it->type_ == UF_circle_type)
                        {
                            UF_VEC3_distance(it_start->dir_Center, it->dir_Center, &dist_center);
                            avrR = (it_start->radius + it->radius) / 2;
                        }
                        else
                        {
                            dist_center = avrR = 0;
                        }


                        isParallel = 0;
                        if ((it->dist_len < 5 || it_start->dist_len < 5) && dist_center < avrR)
                        {
                            isParallel = 1;
                        }
                        else if (it->dist_len > 5 && it->dist_len < 30)
                        {
                            if (dist_center < avrR / 10 && (it_start->dist_len> 5 && it_start->dist_len < 30))
                            {
                                isParallel = 1;
                            }
                            else if (dist_center < avrR / 20 && it_start->dist_len >= 30)
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

                        if ((isParallel || it->dist_len < 0.2) && it->dist_len / it->radius < PI / 3)
                        {
                            count_2++;
                        }
                        else
                        {
                            if (count_2 > 1)
                            {
                                it_end = it_start;
                                UF_OBJ_delete_object(it_end->curve);

                                for (int j = 1; j < count_2; j++)
                                {
                                    it_end++;
                                    UF_OBJ_delete_object(it_end->curve);
                                }

                                UF_VEC3_copy(it_start->start_point, line_coords.start_point);
                                UF_VEC3_copy(it_start->end_point, pt);
                                UF_VEC3_copy(it_end->end_point, line_coords.end_point);
                                UF_CALL(UF_CURVE_create_arc_thru_3pts(1, line_coords.start_point, pt, line_coords.end_point, &it_start->curve));
                                it_end = it = it_start;
                                it_end++;
                                for (int j = 1; j < count_2; j++)//
                                    temp_[i].erase(it_end);
                                it++;

                            }
                            count_2 = 1;
                            it_start = it;
                        }
                    }
                    if (count_2 > 1)
                    {
                        it_end = it_start;
                        UF_OBJ_delete_object(it_end->curve);

                        for (int j = 1; j < count_2; j++)
                        {
                            it_end++;
                            UF_OBJ_delete_object(it_end->curve);
                        }

                        UF_VEC3_copy(it_start->start_point, line_coords.start_point);
                        UF_VEC3_copy(it_start->end_point, pt);
                        UF_VEC3_copy(it_end->end_point, line_coords.end_point);
                        UF_CALL(UF_CURVE_create_arc_thru_3pts(1, line_coords.start_point, pt, line_coords.end_point, &it_start->curve));
                        it_end = it = it_start;
                        it_end++;
                        for (int j = 1; j < count_2; j++)//
                            temp_[i].erase(it_end);
                        it++;
                    }
                    count_2 = 1;
                    it_start = it;
                }
                currType = it->type_;
                count_1 = 1;
                it_start_ = it;
            }
        }
    }
    UF_terminate();
}

void copyCurvesData(vector<vector<CurveData>>& temp1, vector<vector<CurveData>>&temp2)
{
    temp2 = temp1;
}

void ArcTranslateLine(vector<vector<CurveData>>&temp_)
{
    int type_ = 0;
    int subType_ = 0;

    UF_CURVE_line_t lt = { 0 };
    UF_initialize();
    double a = PI / 64;
    for (int i = 0; i < temp_.size(); i++)
    {
        for (int j = 0; j < temp_[i].size(); j++)
        {
            if (temp_[i][j].type_ == UF_circle_type)
                if ((temp_[i][j].dist_len / temp_[i][j].radius < a) && (temp_[i][j].radius >= 100))
                {
                    DELETE_OBJ(temp_[i][j].curve);
                    UF_VEC3_copy(temp_[i][j].start_point, lt.start_point);
                    UF_VEC3_copy(temp_[i][j].end_point, lt.end_point);
                    UF_CALL(UF_CURVE_create_line(&lt, &temp_[i][j].curve));
                    temp_[i][j].type_ = UF_line_type;
                    UF_VEC3_sub(temp_[i][j].end_point, temp_[i][j].start_point, temp_[i][j].dir_Center);
                }
        }
    }
    UF_terminate();
}

void ArcTranslateLine(vector<vector<CurveData*>>&temp_)
{
    int type_ = 0;
    int subType_ = 0;

    UF_CURVE_line_t lt = { 0 };
    UF_initialize();
    double angle;
    double tolerance=0.25;
    double maxRadius;
    for (int i = 0; i < temp_.size(); i++)
    {
        for (int j = 0; j < temp_[i].size(); j++)
        {
            if (temp_[i][j]->type_ == UF_circle_type)
            {
                angle = (temp_[i][j]->dist_len / temp_[i][j]->radius)/2;
                maxRadius = tolerance / (1 - cos(angle));
                if (temp_[i][j]->radius<= maxRadius|| temp_[i][j]->dist_len>10)
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


void is_parallel(const double one_3[3], const double two_3[3], double angle__, int * rt)
{
    double angle_ = MyFun::angleOfVectors(one_3, two_3);
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

void reNameCurves(vector<tag_t> temp, char name_[])
{
    UF_INITIALIZE();
    char name[133];
    for (int i = 0; i < temp.size(); i++)
    {
        UF_OBJ_ask_name(temp[i], name);
        if (strncmp(szTrim, name, 4) != 0 || strncmp(szAssist, name, 4) != 0)
        {
            UF_OBJ_set_name(temp[i], name_);
        }
    }

    UF_TERMINATE();
}

void roughOffsetCurves(const double dist, const bool is_dir, vector<tag_t> &tempCurves)
{
    UF_INITIALIZE();
    UF_STRING_t intput_curves;
    UF_MODL_init_string_list(&intput_curves);
    UF_MODL_create_string_list(1, BYTE_4(tempCurves.size()), &intput_curves);
    intput_curves.dir[0] = is_dir ? 1 : 0;
    intput_curves.string[0] = BYTE_4(tempCurves.size());
    for (int i = 0; i < tempCurves.size(); ++i)
        intput_curves.id[i] = tempCurves[i];

    UF_CURVE_offset_distance_data_t curve_offset_dist_date;
    curve_offset_dist_date.rough_type = 1;
    char buf[64];
    sprintf(buf, "%f", dist);
    curve_offset_dist_date.distance = buf;

    UF_CURVE_offset_data_t offset_data;
    offset_data.input_curves = &intput_curves;
    offset_data.approximation_tolerance = 0.0254;
    offset_data.string_tolerance = 0.0254;
    offset_data.offset_type = UF_CURVE_OFFSET_DISTANCE_FILLET;
    offset_data.offset_def.distance_type1 = &curve_offset_dist_date;

    int num_curves;
    tag_p_t outputCurves;
    UF_CALL(UF_CURVE_create_offset_curve(&offset_data, &num_curves, &outputCurves));
    UF_MODL_free_string_list(&intput_curves);
    if (num_curves)
    {

        tempCurves.clear();
        for (int i = 0; i < num_curves; i++)
            tempCurves.push_back(outputCurves[i]);
    }
    else
        uc1601("警告:粗略偏置曲线失败,可能原因:偏置距离过大,无曲线产生!", 1);
    UF_free(outputCurves);
    UF_TERMINATE();
    return;
}

void editCurveData(const CurveData& oldCD,CurveData &newCD)//cd1 和 cd2 收尾相接
{
    UF_INITIALIZE();
    double pt[3];
    if (newCD.type_==UF_line_type)
    {
        UF_VEC3_copy(oldCD.vertexPt, newCD.start_point);
        UF_CURVE_edit_line_data(newCD.curve,(UF_CURVE_line_p_t)&newCD.start_point);
        newCD.SetData();
        if (MyFun::is_Equal(oldCD.vertexPt, newCD.vertexPt, 0.001))
            newCD.swap();
    }
    else if(newCD.type_ == UF_circle_type)
    {
        UF_VEC3_copy(oldCD.vertexPt, newCD.start_point);
        MyFun::getCurvePt(newCD.curve,0.5,pt);
        UF_INITIALIZE();
        UF_CALL(UF_OBJ_delete_object(newCD.curve));
        UF_CURVE_create_arc_thru_3pts(1, newCD.start_point,pt, newCD.end_point,&newCD.curve);
        newCD.SetData();
        if (MyFun::is_Equal(oldCD.vertexPt, newCD.vertexPt, 0.001))
            newCD.swap();
    }
    UF_OBJ_set_color(newCD.curve, 36);
    UF_OBJ_set_color(oldCD.curve, 26);

}

void sortCurves(vector<tag_t> & curves, vector<vector<CurveData>> &rt_simplePro_, vector<vccdata>& rt_trimVCC, vector<CurveData> &rt_vertexPts, bool& rt_isTrimLClose)
{
    vector<CurveData> temp;
    askCruvesDataEx(curves, temp);

    vector<CurveData>::iterator iter;
    vector<CurveData>::iterator iter_;
    UF_initialize();

    double pt[3] = { 0 };
    double pt1[3] = { 0 };
    double pt2[3] = { 0 };
    CurveData m_CurveDataB, m_CurveDataA;
    m_CurveDataB = m_CurveDataA = temp.back();
    m_CurveDataB.swap();
    temp.pop_back();
    vector<tag_t> temp1;
    vector<tag_t> temp2;
    vccdata tempVcc;
    temp1.push_back(m_CurveDataA.curve);

    vector<CurveData> tempA;
    vector<CurveData> tempB;
    vector<CurveData> tempAB;
    tempA.push_back(m_CurveDataA);

    rt_simplePro_.clear();
    rt_trimVCC.clear();
    rt_vertexPts.clear();

    //辅助算法变量
    tag_t a = 0;
    tag_t b = 0;
    int count = 0;

    int type_ = 0;
    int subType_ = 0;
    UF_CURVE_line_t lt = { 0 };
    double tolerance_ = 0.001;

    if (temp.size() == 0)
    {

        if (!MyFun::is_Equal(m_CurveDataA.vertexPt, m_CurveDataB.vertexPt, tolerance_))
        {  
            rt_isTrimLClose = false;
            rt_vertexPts.push_back(m_CurveDataA);//一条线也要加入
            rt_vertexPts.push_back(m_CurveDataB);
        }
        else
            rt_isTrimLClose = true;
        
        tempVcc.vect = temp1;
        if (rt_vertexPts.size())
        {
            tempVcc.a = m_CurveDataA;
            tempVcc.b = m_CurveDataB;
        }

        rt_trimVCC.push_back(tempVcc);
        rt_simplePro_.push_back(tempA);
    }

    while (!temp.empty())
    {
        a = m_CurveDataA.curve;
        for (iter = temp.begin(); iter != temp.end() && temp.size() > 0; ++iter)
        {
            if (MyFun::is_Equal(m_CurveDataA.vertexPt, (*iter).start_point, tolerance_))
            {
                m_CurveDataA = *iter;
                temp1.push_back(m_CurveDataA.curve);
                tempA.push_back(m_CurveDataA);
                temp.erase(iter);
                break;
            }
            if (MyFun::is_Equal(m_CurveDataA.vertexPt, (*iter).end_point, tolerance_))
            {
                iter->swap();
                m_CurveDataA = (*iter);
                temp1.push_back(m_CurveDataA.curve);
                tempA.push_back(m_CurveDataA);
                temp.erase(iter);
                break;
            }
        }
        b = m_CurveDataB.curve;
        for (iter = temp.begin(); iter != temp.end() && temp.size() > 0; ++iter)
        {
            if (MyFun::is_Equal(m_CurveDataB.vertexPt, (*iter).start_point, tolerance_))
            {
                m_CurveDataB = *iter;
                temp2.push_back(m_CurveDataB.curve);
                tempB.push_back(m_CurveDataB);
                temp.erase(iter);
                break;
            }
            if (MyFun::is_Equal(m_CurveDataB.vertexPt, (*iter).end_point, tolerance_))
            {
                iter->swap();
                m_CurveDataB = (*iter);
                tempB.push_back(m_CurveDataB);
                temp2.push_back(m_CurveDataB.curve);
                temp.erase(iter);
                break;
            }
        }
        if ((a == m_CurveDataA.curve&&b == m_CurveDataB.curve )|| temp.empty())
        {
            {//<1可被认为相接 继续
                double tempD = 999.9999;
                double tempD_ = 999.9999;
                iter = temp.begin();

                while (iter != temp.end())
                {
                    if (tempD > (tempD_ = MyFun::Max_D_value(iter->start_point, m_CurveDataA.vertexPt)))
                    {
                        tempD = tempD_;
                        if (tempD > (tempD_ = MyFun::Max_D_value(iter->end_point, m_CurveDataA.vertexPt)))
                        {
                            tempD = tempD_;
                            iter->swap();
                        }
                        iter_ = iter;
                    }
                    else if (tempD > (tempD_ = MyFun::Max_D_value(iter->end_point, m_CurveDataA.vertexPt)))
                    {
                        tempD = tempD_;
                        iter->swap();
                        iter_ = iter;
                    }
                    ++iter;
                }

                if (tempD < 1)
                {
                    editCurveData(m_CurveDataA, (*iter_));
                    m_CurveDataA = (*iter_);
                    temp1.push_back(m_CurveDataA.curve);
                    tempA.push_back(m_CurveDataA);
                    temp.erase(iter_);
                }

                tempD = 999.9999;
                tempD_ = 999.9999;
                iter = temp.begin();

                while (iter != temp.end())
                {
                    if (tempD > (tempD_ = MyFun::Max_D_value(iter->start_point, m_CurveDataB.vertexPt)))
                    {
                        tempD = tempD_;
                        if (tempD > (tempD_ = MyFun::Max_D_value(iter->end_point, m_CurveDataB.vertexPt)))
                        {
                            tempD = tempD_;
                            iter->swap();
                        }
                        iter_ = iter;
                    }
                    else if (tempD > (tempD_ = MyFun::Max_D_value(iter->end_point, m_CurveDataB.vertexPt)))
                    {
                        tempD = tempD_;
                        iter->swap();
                        iter_ = iter;
                    }
                    ++iter;
                }

                if (tempD < 1)
                {
                    editCurveData(m_CurveDataB, (*iter_));
                    m_CurveDataB = (*iter_);
                    tempB.push_back(m_CurveDataB);
                    temp2.push_back(m_CurveDataB.curve);
                    temp.erase(iter_);
                }

                if ((a != m_CurveDataA.curve || b != m_CurveDataB.curve) && !temp.empty())
                    continue;
            }

            if (!temp.empty())//不考虑T形相接的情况
            {
                int num_1 = BYTE_4(temp1.size());
                int num_2 = BYTE_4(temp2.size());
                int num_A = BYTE_4(tempA.size());
                int num_B = BYTE_4(tempB.size());
                for (int i = 0; i < num_1; i++)
                {
                    tempVcc.vect.push_back(temp1.back());
                    temp1.pop_back();
                }
                for (int i = 0; i < num_2; i++)
                {
                    tempVcc.vect.push_back(temp2[i]);
                }

                for (int i = 0; i < num_A; i++)
                {
                    tempA.back().swap();
                    tempAB.push_back(tempA.back());
                    tempA.pop_back();
                }

                for (int i = 0; i < num_B; i++)
                {
                    tempAB.push_back(tempB[i]);
                }
                temp2.clear();
                tempB.clear();
                temp1.clear();
                tempA.clear();

                rt_simplePro_.push_back(tempAB);
                tempAB.clear();
                tempVcc.a = m_CurveDataA;
                tempVcc.b = m_CurveDataB;
                rt_trimVCC.push_back(tempVcc);
                tempVcc.vect.clear();
                /////////////////////////////////////////////////////////////////////

                if (!MyFun::is_Equal(m_CurveDataA.vertexPt, m_CurveDataB.vertexPt, 1))
                {
                    rt_vertexPts.push_back(m_CurveDataA);
                    rt_vertexPts.push_back(m_CurveDataB);
                }

                m_CurveDataB = m_CurveDataA = temp.back();
                temp.pop_back();
                m_CurveDataB.swap();

                if (temp.empty())
                {
                    tempVcc.vect.push_back(m_CurveDataA.curve);
                    tempVcc.a = m_CurveDataA;
                    tempVcc.b = m_CurveDataB;
                    rt_trimVCC.push_back(tempVcc);
                    tempAB.push_back(m_CurveDataA);
                    rt_simplePro_.push_back(tempAB);
                    if (!MyFun::is_Equal(m_CurveDataA.vertexPt, m_CurveDataB.vertexPt, 1))
                    {
                        rt_vertexPts.push_back(m_CurveDataA);
                        rt_vertexPts.push_back(m_CurveDataB);
                    }
                }
                temp1.push_back(m_CurveDataA.curve);
                tempA.push_back(m_CurveDataA);
            }

            else
            {
                int num_1 = BYTE_4(temp1.size());
                int num_2 = BYTE_4(temp2.size());
                int num_A = BYTE_4(tempA.size());
                int num_B = BYTE_4(tempB.size());
                for (int i = 0; i < num_1; i++)
                {
                    tempVcc.vect.push_back(temp1.back());
                    temp1.pop_back();
                }
                for (int i = 0; i < num_2; i++)
                {
                    tempVcc.vect.push_back(temp2[i]);
                }

                for (int i = 0; i < num_A; i++)
                {
                    tempA.back().swap();
                    tempAB.push_back(tempA.back());
                    tempA.pop_back();
                }

                for (int i = 0; i < num_B; i++)
                {
                    tempAB.push_back(tempB[i]);
                }

                temp2.clear();
                tempB.clear();
                temp1.clear();
                tempA.clear();


                rt_simplePro_.push_back(tempAB);
                tempAB.clear();
                if (!MyFun::is_Equal(m_CurveDataA.vertexPt, m_CurveDataB.vertexPt, 1))
                {
                    rt_vertexPts.push_back(m_CurveDataA);
                    rt_vertexPts.push_back(m_CurveDataB);
                }
                tempVcc.a = m_CurveDataA;
                tempVcc.b = m_CurveDataB;
                rt_trimVCC.push_back(tempVcc);
                tempVcc.vect.clear();

                break;
            }

        }
    }
    curves.clear();
    for (int i = 0; i < rt_simplePro_.size(); i++)
    {
        for (int j = 0; j < rt_simplePro_[i].size(); j++)
            curves.push_back(rt_simplePro_[i][j].curve);
    }
}

void sortCurvesPointor(vector<CurveData*> tempDelete, vector<vector<CurveData*>> &rt_simplePro_)
{
    vector<CurveData*>::iterator iter1;
    vector<CurveData*>::iterator iter2;
    UF_initialize();

    CurveData * m_pCurveDataB, * m_pCurveDataA;//已存队列
    m_pCurveDataB = m_pCurveDataA = tempDelete.back();
    tempDelete.pop_back();

    vector<CurveData*> tempA;
    vector<CurveData*> tempB;
    vector<CurveData*> tempAB;
    tempA.push_back(m_pCurveDataA);
    rt_simplePro_.clear();

    //辅助算法变量
    tag_t a ;
    tag_t b ;

    double tolerance_ = 0.001;

    if (tempDelete.size() == 0)
    {
        rt_simplePro_.push_back(tempA);
    }

    while (!tempDelete.empty())
    {
        a = m_pCurveDataA->curve;
        for (iter1 = tempDelete.begin(); iter1 != tempDelete.end() && tempDelete.size() > 0; ++iter1)
        {
            if (MyFun::is_Equal(m_pCurveDataA->end_point, (*iter1)->start_point, tolerance_))
            {
                m_pCurveDataA = *iter1;
                tempA.push_back(m_pCurveDataA);
                tempDelete.erase(iter1);
                break;
            }
            if (MyFun::is_Equal(m_pCurveDataA->end_point, (*iter1)->end_point, tolerance_))
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
            if (MyFun::is_Equal(m_pCurveDataB->start_point, (*iter1)->start_point, tolerance_))
            {
                (*iter1)->swap();
                m_pCurveDataB = *iter1;
                tempB.push_back(m_pCurveDataB);
                tempDelete.erase(iter1);
                break;
            }
            if (MyFun::is_Equal(m_pCurveDataB->start_point, (*iter1)->end_point, tolerance_))
            {
                m_pCurveDataB = (*iter1);
                tempB.push_back(m_pCurveDataB);
                tempDelete.erase(iter1);
                break;
            }
        }
        if ((a == m_pCurveDataA->curve&&b == m_pCurveDataB->curve)|| tempDelete.empty())
        {
            {//<1可被认为相接 继续
                double dist_1 = 999.9999;
                double dist_2 = 999.9999;
                iter1 = tempDelete.begin();
                bool assignValue = false;

                while (iter1 != tempDelete.end())
                {
                    if (dist_1 > (dist_2 = MyFun::Max_D_value((*iter1)->start_point, m_pCurveDataA->end_point)))
                    {
                        dist_1 = dist_2;
                        if (dist_1 > (dist_2 = MyFun::Max_D_value((*iter1)->end_point, m_pCurveDataA->end_point)))
                        {
                            dist_1 = dist_2;
                            (*iter1)->swap();
                        }
                        iter2 = iter1;
                        assignValue = true;
                    }
                    else if (dist_1 > (dist_2 = MyFun::Max_D_value((*iter1)->end_point, m_pCurveDataA->end_point)))
                    {
                        dist_1 = dist_2;
                        (*iter1)->swap();
                        iter2 = iter1;
                        assignValue = true;
                    }
                    ++iter1;
                }

                if (dist_1 < 1&& assignValue)
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
                    if (dist_1 > (dist_2 = MyFun::Max_D_value((*iter1)->start_point, m_pCurveDataB->start_point)))
                    {
                        dist_1 = dist_2;
                        (*iter1)->swap();
                        if (dist_1 > (dist_2 = MyFun::Max_D_value((*iter1)->end_point, m_pCurveDataB->start_point)))
                        {
                            dist_1 = dist_2;
                        }
                        assignValue = true;
                        iter2 = iter1;
                    }
                    else if (dist_1 > (dist_2 = MyFun::Max_D_value((*iter1)->end_point, m_pCurveDataB->start_point)))
                    {
                        dist_1 = dist_2;
                        (*iter1)->swap();
                        assignValue = true;
                        iter2 = iter1;
                    }
                    ++iter1;
                }
                if (dist_1 < 1&& assignValue)
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

                for (int i = num_B-1; i >=0 ; i--)
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

void deleteReCurve(vector<CurveData>& temp)
{
    vector<CurveData>::iterator it;
    vector<CurveData>::iterator it_;
    UF_initialize();
    int type_ = 0;
    int subType_ = 0;
    for (it = temp.begin(); it != temp.end() && temp.size() > 0; it++)
    {
        it_ = it + 1;

        for (it_; it_ != temp.end() && temp.size() > 0; it_++)
        {
            if (MyFun::is_Equal((*it).start_point, (*it_).start_point, 0.0001) || MyFun::is_Equal((*it).start_point, (*it_).end_point, 0.0001))
                if (MyFun::is_Equal((*it).end_point, (*it_).start_point, 0.0001) || MyFun::is_Equal((*it).end_point, (*it_).end_point, 0.0001))
                {
                    UF_OBJ_ask_type_and_subtype((*it).curve, &type_, &subType_);
                    if (type_ == UF_circle_type)
                    {
                        UF_OBJ_delete_object((*it).curve);
                        MyFun::resetUpdata();
                        temp.erase(it);
                        it--;
                        break;;
                    }
                    else
                    {
                        UF_OBJ_delete_object((*it_).curve);
                        temp.erase(it_);
                        break;
                    }

                }
        }
    }
    UF_terminate();
}

bool is_3DCurve(const vector<tag_t> &curves, double *dir3) //spline不在一个平面上极为3d
{
    if (curves.size()==0)
    {
        return 0;
    }
    double pt[9];
    tag_t wcs;
    tag_t csys;
    UF_initialize();
    UF_CSYS_ask_wcs(&wcs);
    double mtx[9];
   
    if (dir3 && (dir3[0] || dir3[1] || dir3[2]))
    {
        UF_MTX3_initialize_z(dir3, mtx);
    }
    else
    {
        if (dir3)
        {
            double dir1[3];
            double dir2[3];
            for (int i = 0; i < 3; i++)
                MyFun::getCurvePt(curves[i], 0.3*i, &pt[i * 3]);
            UF_VEC3_sub(pt, &pt[3], dir1);
            UF_VEC3_sub(&pt[6], &pt[3], dir2);
            UF_VEC3_cross(dir1, dir2, dir3);
            UF_MTX3_initialize_z(dir3, mtx);
        }
        else
        {
            double dir3_[3];
            double dir1[3];
            double dir2[3];
            for (int i = 0; i < 3; i++)
                MyFun::getCurvePt(curves[i], 0.3*i, &pt[i * 3]);
            UF_VEC3_sub(pt, &pt[3], dir1);
            UF_VEC3_sub(&pt[6], &pt[3], dir2);
            UF_VEC3_cross(dir1, dir2, dir3_);
            UF_MTX3_initialize_z(dir3_, mtx);
        }
    }
    
    tag_t matrix_id;
    UF_CSYS_create_matrix(mtx, &matrix_id);
    double org[3] = { 0 };
    UF_CSYS_create_csys(org, matrix_id, &csys);
    UF_CSYS_set_wcs(csys);
    UF_OBJ_set_blank_status(csys, UF_OBJ_BLANKED);
    MyFun::CSolidObj tempObj(curves[0]);
    for (int i = 1; i < curves.size();i++)
    {
        tempObj.adjustObject(curves[i]);
    }
    UF_VEC3_distance(tempObj.m_bottomCenter, tempObj.m_topCenter, pt);
    UF_CSYS_set_wcs(wcs);
    DELETE_OBJ(csys);
    if (pt[0]>0.001)
        return true;
    else
        return false;
}

void autoCloseAssistCurves(vector<CurveData*>& trimCurves, double *dir, double width, vector<CurveData*>& assistLD,int &count_, int flag_type /*= 1*/)
{
    if (MyFun::is_Equal(trimCurves[0]->start_point, trimCurves.back()->end_point, 0.001))
    {
        count_ = 0;
        SHOW_INFO_USR("修边线闭合，目前不支持重头下模生成");
        return;
    }
    trimCurves[0]->swap();
    CurveData* vertexPts[2] = { trimCurves[0],trimCurves.back() };
    count_ =BYTE_4( assistLD.size());
    if (trimCurves.size() > 0)
    {
        double pt0[3];
        double len0 = 0;
        double len0_ = 0;
        double len1 = 0;
        double len1_ = 0;
        
        for (int i = 0; i < trimCurves.size(); i++)
        {
            MyFun::pointPerpToLine(trimCurves[i]->vertexPt, vertexPts[0]->start_point,
                vertexPts[0]->end_point, pt0);
            UF_VEC3_distance(pt0, vertexPts[0]->vertexPt, &len0_);
            if (len0 < len0_)
                len0 = len0_;
            MyFun::pointPerpToLine(trimCurves[i]->vertexPt, vertexPts[1]->start_point,
                vertexPts[1]->end_point, pt0);
            UF_VEC3_distance(pt0, vertexPts[1]->vertexPt, &len1_);
            if (len1 < len1_)
                len1 = len1_;
        }
//         if (assistLD.size())
//         {
//             closeAssistCurve(vertexPts, dir, width, len0, len1, 1, assistLD);
//         }
//         else
        {
            closeAssistCurve(vertexPts, dir, width, len0, len1, flag_type, assistLD);
        }
    }
    else
    {
//         if (assistLD.size())
//         {
//             closeAssistCurve(vertexPts, dir, width, 0, 0, 1, assistLD);
//         }
//         else
        {
            closeAssistCurve(vertexPts, dir, width, 0, 0, flag_type, assistLD);
        }

    }
    count_= BYTE_4(assistLD.size()- count_);
}

void adjustLineData(tag_t &line, UF_CURVE_line_t& lt) //修编模块
{
    UF_initialize();
    int a;
    UF_VEC3_is_equal(lt.start_point, lt.end_point, 0.001, &a);
    if (a)
    {
        if (UF_OBJ_ask_status(line) != UF_OBJ_DELETED)
        {
            UF_OBJ_delete_object(line);
            line = 0;
        }
    }
    else
    {
        if (UF_OBJ_ask_status(line) != UF_OBJ_DELETED)
        {
            UF_CURVE_edit_line_data(line, &lt);
        }
        else
        {
            UF_CURVE_create_line(&lt, &line);
        }
    }
    UF_terminate();
}

void closeAssistCurve(CurveData** tempCD, double dir[3], double width, double len0, double len1, int flag_type, vector<CurveData*>&assist)
{
    width = width*1.2;
    double dirX[3] = { 1,0,0 };
    double dirY[3] = { 0,1,0 };
    MyFun::VectorProjectInPlane(dir, dirX, dirX);
    MyFun::VectorProjectInPlane(dir, dirY, dirY);



    double dirX_[3];
    double dirY_[3];

    UF_VEC3_copy(dirX, dirX_);
    UF_VEC3_copy(dirY, dirY_);

    double dirAngular[3];
    double temp3D[3];
    double tempDir0[3];
    double tempDir1[3];
    double tempDir0_[3];
    double tempDir1_[3];
    double pt_[3];
    double pt0[3];
    double pt1[3];

    tag_t line_id;
    UF_CURVE_line_t lt;
    UF_initialize();
    if (tempCD[0]->type_ != UF_line_type || tempCD[1]->type_ != UF_line_type) return;
    double magnitude;
    UF_VEC3_unitize(tempCD[0]->dir_Center, 0.000001, &magnitude, tempCD[0]->dir_Center);
    UF_VEC3_unitize(tempCD[1]->dir_Center, 0.000001, &magnitude, tempCD[1]->dir_Center);
    //角平分线和xy比较得出靠板出的辅助线的方向,用于智能判断生成方位
    UF_VEC3_add(tempCD[0]->dir_Center, tempCD[1]->dir_Center, dirAngular);
    if ((dirAngular[0] >= -0.0001&& dirAngular[0] <= 0.0001) && (dirAngular[1] >= -0.0001
        && dirAngular[1] <= 0.0001) && (dirAngular[2] >= -0.0001&& dirAngular[2] <= 0.0001))
    {
        uc1601("警告:辅助线无法自动判断生成方位!", 1);
        return;
    }
    //angle1 = MyFun::angleOfVectors(tempCD[0]->dir_Center, tempCD[1]->dir_Center);///todo
    UF_VEC3_sub(tempCD[0]->vertexPt, tempCD[1]->vertexPt, temp3D);
    double angle0 = MyFun::angleOfVectors(tempCD[0]->dir_Center, temp3D);
    double angle1 = PI - MyFun::angleOfVectors(tempCD[1]->dir_Center, temp3D);//矫正角度


    const double angle_Max = PI * 4 / 9;
    const double angle_Min = PI * 1 / 18;
    if (angle0 > angle_Max&& angle1 > angle_Max&&flag_type)//上模直接相连
    {
        UF_VEC3_midpt(tempCD[0]->vertexPt, tempCD[1]->vertexPt, temp3D);
        MyFun::pointPerpToLine(temp3D, tempCD[0]->start_point, tempCD[0]->end_point, pt0);
        MyFun::pointPerpToLine(temp3D, tempCD[1]->start_point, tempCD[1]->end_point, pt1);

        UF_VEC3_sub(pt0, pt1, tempDir0);
        double angle = MyFun::angleOfVectors(dirX, tempDir0);
        if (angle > PI / 2)
            angle = PI - angle;
        //贴合XY轴.
        if (angle < PI / 18)//dirX
        {
            MyFun::getDirectionPos(dirX, temp3D, 5, pt_);
            MyFun::intersectPtOfLines(temp3D, pt_, tempCD[0]->start_point, tempCD[0]->end_point, pt0);
            MyFun::intersectPtOfLines(temp3D, pt_, tempCD[1]->start_point, tempCD[1]->end_point, pt1);
        }
        else if (angle >angle_Max)//dirY
        {
            MyFun::getDirectionPos(dirY, temp3D, 5, pt_);
            MyFun::intersectPtOfLines(temp3D, pt_, tempCD[0]->start_point, tempCD[0]->end_point, pt0);
            MyFun::intersectPtOfLines(temp3D, pt_, tempCD[1]->start_point, tempCD[1]->end_point, pt1);
        }

        UF_VEC3_copy(tempCD[0]->start_point, lt.start_point);
        UF_VEC3_copy(pt0, lt.end_point);
        UF_CURVE_edit_line_data(tempCD[0]->curve, &lt);
        tempCD[0]->SetData();
        if (MyFun::is_Equal(pt0, tempCD[0]->start_point, 0.001))
            tempCD[0]->swap();

        UF_VEC3_copy(tempCD[1]->start_point, lt.start_point);
        UF_VEC3_copy(pt1, lt.end_point);
        UF_CURVE_edit_line_data(tempCD[1]->curve, &lt);
        tempCD[1]->SetData();
        if (MyFun::is_Equal(pt1, tempCD[1]->start_point, 0.001))
            tempCD[1]->swap();

        UF_VEC3_copy(tempCD[0]->vertexPt, lt.start_point);
        UF_VEC3_copy(tempCD[1]->vertexPt, lt.end_point);

        UF_CURVE_create_line(&lt, &line_id);
        assist.push_back(new CurveData(line_id));
    }
    else//需要复杂处理
    {
        angle0 = MyFun::angleOfVectors(dirX, tempCD[0]->dir_Center);
        angle1 = MyFun::angleOfVectors(dirX, tempCD[1]->dir_Center);
        double *p_dir0;
        double *p_dir1;

        if (angle0 > PI / 2)
        {
            angle0 = angle0 - PI / 2;
            if (angle0 < angle_Min)
                p_dir0 = dirX_;
            else if (angle0 > angle_Max)
                p_dir0 = dirY_;
            else//tempDir0
            {
                p_dir0 = tempDir0;
                UF_VEC3_cross(dir, tempCD[0]->dir_Center, p_dir0);
            }
        }
        else if (angle0 > angle_Max)
            p_dir0 = dirX_;
        else if (angle0 < angle_Min)
            p_dir0 = dirY_;
        else//tempDir0
        {
            p_dir0 = tempDir0;
            UF_VEC3_cross(dir, tempCD[0]->dir_Center, p_dir0);
        }

        if (angle1 > PI / 2)
        {
            angle1 = angle1 - PI / 2;
            if (angle1 < angle_Min)
                p_dir1 = dirX;
            else if (angle1 > angle_Max)
                p_dir1 = dirY;
            else//tempDir1
            {
                p_dir1 = tempDir1;
                UF_VEC3_cross(dir, tempCD[1]->dir_Center, p_dir1);
            }
        }
        else if (angle1 > angle_Max)
            p_dir1 = dirX;
        else if (angle1 < angle_Min)
            p_dir1 = dirY;
        else//tempDir1
        {
            p_dir1 = tempDir1;
            UF_VEC3_cross(dir, tempCD[1]->dir_Center, p_dir1);
        }

        double vertexPt[3];
        angle0 = MyFun::angleOfVectors(p_dir0, p_dir1);
        if (angle0 != PI&&angle0 != 0)//方向校正
        {
            MyFun::intersectPtOfLines_(p_dir0, tempCD[0]->vertexPt, p_dir1, tempCD[1]->vertexPt, vertexPt);
            p_dir0 = tempDir0;
            p_dir1 = tempDir1;
            UF_VEC3_sub(vertexPt, tempCD[0]->vertexPt, p_dir0);
            UF_VEC3_sub(vertexPt, tempCD[1]->vertexPt, p_dir1);
        }
        else
        {
            double TempDir_[3];
            UF_VEC3_midpt(tempCD[0]->vertexPt, tempCD[1]->vertexPt, temp3D);
            UF_VEC3_sub(temp3D, tempCD[0]->vertexPt, TempDir_);
            UF_initialize();
            angle0 = MyFun::angleOfVectors(p_dir0, TempDir_);
            if (angle0 > PI / 2)
                UF_VEC3_negate(p_dir0, p_dir0);
            UF_VEC3_negate(TempDir_, TempDir_);
            angle0 = MyFun::angleOfVectors(p_dir1, TempDir_);
            if (angle0 > PI / 2)
                UF_VEC3_negate(p_dir1, p_dir1);
            UF_VEC3_copy(p_dir0, tempDir0);
            UF_VEC3_copy(p_dir1, tempDir1);
            p_dir0 = tempDir0;
            p_dir1 = tempDir1;
        }

        if (flag_type)//上模 创建多个辅助线
        {
            angle0 = MyFun::angleOfVectors(p_dir0, p_dir1);
            if (angle0 > angle_Max)//两条直线
            {
                if (angle0 < PI / 2)
                {
                    double angle_1 = MyFun::angleOfVectors(p_dir0, tempCD[0]->dir_Center);
                    double angle_2 = MyFun::angleOfVectors(p_dir1, tempCD[1]->dir_Center);

                    if (angle_1 < PI / 2)
                    {
                        UF_VEC3_cross(dir, p_dir0, p_dir1);
                        MyFun::intersectPtOfLines_(p_dir0, tempCD[0]->vertexPt, p_dir1, tempCD[1]->vertexPt, vertexPt);
                    }
                    else
                    {
                        UF_VEC3_cross(dir, p_dir1, p_dir0);
                        MyFun::intersectPtOfLines_(p_dir0, tempCD[0]->vertexPt, p_dir1, tempCD[1]->vertexPt, vertexPt);
                    }

                    UF_VEC3_copy(vertexPt, lt.end_point);
                    UF_VEC3_copy(tempCD[0]->vertexPt, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));
                    UF_VEC3_copy(tempCD[1]->vertexPt, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));
                }
                else
                {
                    UF_VEC3_copy(vertexPt, lt.end_point);
                    UF_VEC3_copy(tempCD[0]->vertexPt, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                    UF_VEC3_copy(tempCD[1]->vertexPt, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));
                }
            }
            else//上模 三条直线
            {
                MyFun::getDirectionPos(tempDir0, tempCD[0]->vertexPt, width, pt0);
                MyFun::getDirectionPos(tempDir1, tempCD[1]->vertexPt, width, pt1);
                UF_VEC3_copy(pt0, lt.end_point);
                UF_VEC3_copy(tempCD[0]->vertexPt, lt.start_point);
                UF_CURVE_create_line(&lt, &line_id);
                assist.push_back(new CurveData(line_id));

                UF_VEC3_copy(pt1, lt.end_point);
                UF_VEC3_copy(tempCD[1]->vertexPt, lt.start_point);
                UF_CURVE_create_line(&lt, &line_id);
                assist.push_back(new CurveData(line_id));

                UF_VEC3_copy(pt0, lt.end_point);
                UF_VEC3_copy(pt1, lt.start_point);
                UF_CURVE_create_line(&lt, &line_id);
                assist.push_back(new CurveData(line_id));
            }
        }
        else //下模处理方法
        {
            
            MyFun::getDirectionPos(tempDir0, tempCD[0]->vertexPt, -width, pt0);
            MyFun::getDirectionPos(tempDir1, tempCD[1]->vertexPt, -width, pt1);

            UF_VEC3_cross(dir, tempDir0, tempDir0_);
            UF_VEC3_cross(dir, tempDir1, tempDir1_);
            angle0 = MyFun::angleOfVectors(tempDir0_, tempCD[0]->dir_Center);
            angle1 = MyFun::angleOfVectors(tempDir1_, tempCD[1]->dir_Center);
            if (angle0 < PI / 2)
                UF_VEC3_negate(tempDir0_, tempDir0_);
            if (angle1 < PI / 2)
                UF_VEC3_negate(tempDir1_, tempDir1_);

            double dist0;
            double dist1;
            double pt0_[3];
            double pt1_[3];
            MyFun::intersectPtOfLines_(tempDir0_, pt0, tempDir1_, pt1, vertexPt);
            UF_VEC3_distance(pt0, vertexPt, &dist0);
            UF_VEC3_distance(pt1, vertexPt, &dist1);
            angle0 = MyFun::angleOfVectors(tempDir0_, tempDir1_);
            if (angle0 != PI&&angle0 != 0)
            {

                if (len0 == 0)
                    if (tempCD[0]->dist_len + width < dist0)
                        MyFun::getDirectionPos(tempDir0_, pt0, tempCD[0]->dist_len + width, pt0_);
                    else
                        UF_VEC3_copy(vertexPt, pt0_);
                else
                    if (len0 + width < dist0)
                        MyFun::getDirectionPos(tempDir0_, pt0, len0 + width, pt0_);
                    else
                        UF_VEC3_copy(vertexPt, pt0_);

                if (len1 == 0)
                    if (tempCD[1]->dist_len + width < dist1)
                        MyFun::getDirectionPos(tempDir1_, pt1, tempCD[1]->dist_len + width, pt1_);
                    else
                        UF_VEC3_copy(vertexPt, pt1_);
                else
                    if (len1 + width < dist1)
                        MyFun::getDirectionPos(tempDir1_, pt1, len1 + width, pt1_);
                    else
                        UF_VEC3_copy(vertexPt, pt1_);

                if (MyFun::is_Equal(pt0_, vertexPt, 0.001) || MyFun::is_Equal(pt1_, vertexPt, 0.001))//4条直线
                {
                    UF_VEC3_copy(vertexPt, pt0_);
                    UF_VEC3_copy(vertexPt, pt1_);
                    UF_VEC3_copy(pt0, lt.end_point);
                    UF_VEC3_copy(tempCD[0]->vertexPt, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                    UF_VEC3_copy(vertexPt, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                    UF_VEC3_copy(pt1, lt.end_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                    UF_VEC3_copy(tempCD[1]->vertexPt, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                }
                else//五条直线
                {
                    UF_VEC3_copy(pt0, lt.end_point);
                    UF_VEC3_copy(tempCD[0]->vertexPt, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                    UF_VEC3_copy(pt0_, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                    UF_VEC3_copy(pt1_, lt.end_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                    UF_VEC3_copy(pt1, lt.start_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));

                    UF_VEC3_copy(tempCD[1]->vertexPt, lt.end_point);
                    UF_CURVE_create_line(&lt, &line_id);
                    assist.push_back(new CurveData(line_id));
                }
            }
            else
            {
                double len;

                if (len0)
                {
                    len = len0 + width;
                }
                else
                {
                    len = tempCD[0]->dist_len + width;
                }
                MyFun::getDirectionPos(tempDir0_, pt0, len, pt0_);

                if (len1)
                {
                    len = len1 + width;
                }
                else
                {
                    len = tempCD[1]->dist_len + width;
                }
                MyFun::getDirectionPos(tempDir1_, pt1, len, pt1_);

                UF_VEC3_copy(pt0, lt.end_point);
                UF_VEC3_copy(tempCD[0]->vertexPt, lt.start_point);
                UF_CURVE_create_line(&lt, &line_id);
                assist.push_back(new CurveData(line_id));

                UF_VEC3_copy(pt0_, lt.start_point);
                UF_CURVE_create_line(&lt, &line_id);
                assist.push_back(new CurveData(line_id));

                UF_VEC3_copy(pt1_, lt.end_point);
                UF_CURVE_create_line(&lt, &line_id);
                assist.push_back(new CurveData(line_id));

                UF_VEC3_copy(pt1, lt.start_point);
                UF_CURVE_create_line(&lt, &line_id);
                assist.push_back(new CurveData(line_id));

                UF_VEC3_copy(tempCD[1]->vertexPt, lt.end_point);
                UF_CURVE_create_line(&lt, &line_id);
                assist.push_back(new CurveData(line_id));
            }
        }
    }

}

void splinesProcessToLines(vector<tag_t> &tempSplines) //刚件符型避让用.
{
    //样条打散:
    vector<tag_t> tempVVV;
    tag_p_t segments = NULL;
    tag_t tempT;
    int type_, subType, num = 0;
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
            tempT = tempSplines[i];
            UF_CURVE_create_simplified_curve(1, &tempT, 0.0254, &num, &segments);
            UF_OBJ_delete_object(tempT);
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
    double start_point[3];
    double end_point[3];
    double dist;
    tag_t tempLine;
    UF_CURVE_line_t lt;
    for (int j = 0; j < tempSplines.size(); j++)
    {
        UF_OBJ_ask_type_and_subtype(tempSplines[j], &type_, &subType);
        if (type_ == UF_line_type)
        {
            tempVVV.push_back(tempSplines[j]);
        }
        else
        {
            MyFun::getCurvePt(tempSplines[j], 0, start_point);
            MyFun::getCurvePt(tempSplines[j], 1, end_point);

            UF_VEC3_distance(start_point, end_point, &dist);
            if (dist > 20)
            {
                tempVVV.push_back(tempSplines[j]);
                continue;
            }
            UF_VEC3_copy(end_point, lt.end_point);
            UF_VEC3_copy(start_point, lt.start_point);
            UF_CURVE_create_line(&lt, &tempLine);
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
    sortCurves(tempVVV, simplePro, trimVCC, vertexPts, isTrimLClose);
    tempSplines = tempVVV;
}

inline bool hasAttr(tag_t &obj)
{
    int nItem = -1;
    UF_ATTR_count_attributes(obj, UF_ATTR_any, &nItem);
    if (nItem > 0)
        return true;
    else
        return false;
}

inline void readAttr(tag_t obj, char * title, int type /*= UF_ATTR_string*/, UF_ATTR_value_s&ret)
{
    UF_ATTR_read_value(obj, title, type, &ret);
}

inline int findAttr(tag_t obj, char * title, int type/*= UF_ATTR_string*/)
{
    int title_type;
    UF_ATTR_find_attribute(obj, type, title, &title_type);
    return title_type;
}

inline void selectedCurves(NXOpen::BlockStyler::CurveCollector*uiBlock, std::vector<tag_t>&curves)
{
    std::vector<NXOpen::TaggedObject *>temp;
    for (int i = 0; i < curves.size(); i++)
        temp.push_back(NXOpen::NXObjectManager::Get(curves[i]));
    uiBlock->SetSelectedObjects(temp);
}


