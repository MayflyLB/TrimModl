
#include "PublicFun.h"

/**************************************功能函数***********************************/
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
namespace MyFun
{
    bool getFaceNormal(const tag_t &faceTag, double *pFaceNor, double *pCen/*=NULL*/)
    {
        double uvLimit[4] = { 0 };
        UF_MODL_ask_face_uv_minmax(faceTag, uvLimit);
        double uvPair[2] = { (uvLimit[0] + uvLimit[1]) / 2,(uvLimit[2] + uvLimit[3]) / 2 };

        double newPos[3] = { 0,0,0 }, unused[3] = { 0,0,0 }, unitNor[3] = { 0,0,0 };
        UF_MODL_ask_face_props(faceTag, uvPair, newPos, unused, unused, unused, unused, unitNor, unused);

        double faceNor[3] = { 0,0,0 };
        tag_t pos_so[3] = { null_tag }, pt = null_tag;
        UF_SO_create_scalar_double(faceTag, UF_SO_update_after_modeling, newPos[0], &pos_so[0]);
        UF_SO_create_scalar_double(faceTag, UF_SO_update_after_modeling, newPos[1], &pos_so[1]);
        UF_SO_create_scalar_double(faceTag, UF_SO_update_after_modeling, newPos[2], &pos_so[2]);
        UF_SO_create_point_3_scalars(faceTag, UF_SO_update_after_modeling, pos_so, &pt);

        tag_t direction = null_tag;
        if (0 == UF_SO_create_dirr_normal_to_surface_point(faceTag, UF_SO_update_after_modeling, faceTag, pt, FALSE, &direction))
        {
            UF_SO_ask_direction_of_dirr(direction, faceNor);
            for (int i = 0; i != 3; ++i)
            {
                *(pFaceNor + i) = faceNor[i];
                if (pCen)
                    *(pCen + i) = newPos[i];
            }
            return true;
        }
        else
        {
            for (int i = 0; i != 3; ++i)
            {
                *(pFaceNor + i) = unitNor[i];
                if (pCen)
                    *(pCen + i) = newPos[i];
            }
            return false;
        }
    }

    bool isHole(const tag_t & faceTag, double faceNor[3], double refPos_csys[3], double cen_csys[3])
    {
        bool isHoleFace = false;

        //圆心的距离到实体与面的距离相等为孔，不相等为柱
        tag_t tempBody = null_tag;
        UF_MODL_ask_face_body(faceTag, &tempBody);

        double tempPos_csys[3] = { 0 };
        getDirectionPos(faceNor, refPos_csys, 0.04, tempPos_csys);

        double dist1 = 0, dist2 = 0;
        UF_VEC3_distance(tempPos_csys, cen_csys, &dist1);
        UF_VEC3_distance(refPos_csys, cen_csys, &dist2);
        if (dist1 > dist2) isHoleFace = false;
        else isHoleFace = true;
        return isHoleFace;
    }

    CString getPartName(const tag_t & part)
    {
        CString cstrPartName = "";
        if (part == null_tag) return cstrPartName;
        char partSpec[MAX_FSPEC_SIZE + 1] = "";
        UF_PART_ask_part_name(part, partSpec);
        char partName[131] = "";
        uc4574(partSpec, 2, partName);
        void * partNameV = partName;
#ifdef UNICODE
        USES_CONVERSION;
        partNameV = A2W(partName);
#endif // UNICODE
        cstrPartName.Format(_T("%s"), partNameV);


        return cstrPartName;
    }

    int GetModulePath(CString &rString)
    {
        char szValue[1024];
        memset(szValue, 0, sizeof(szValue));
        GetModuleFileNameA(GetCurrentModule(), szValue, sizeof(szValue));
        char *slash_end = strrchr(szValue, '\\');
        if (slash_end != NULL) slash_end[1] = '\x00';
        rString = szValue;
        return true;
    }

    HMODULE GetCurrentModule()
    {
#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

        // Here's a trick that will get you the handle of the module

        // you're running in without any a-priori knowledge:

        MEMORY_BASIC_INFORMATION mbi;

        static int dummy;

        VirtualQuery(&dummy, &mbi, sizeof(mbi));

        return reinterpret_cast<HMODULE>(mbi.AllocationBase);

#else  

        // from ATL 7.0 sources

        return reinterpret_cast<HMODULE>(&__ImageBase);

#endif
    }

    CString findUserPath()//查找用户目录(如c:\program files\elec_tools\)
    {
        CString cstrRst = "";
        char *userDir = NULL;/*[UF_CFI_MAX_PATH_NAME_SIZE]="";*/
        UF_translate_variable("UGII_USER_DIR", &userDir);
        void*userDirV = userDir;
#ifdef UNICODE
        USES_CONVERSION;
        userDirV = A2W(userDir);
#endif // UNICODE
        cstrRst.Format(_T("%s"), userDirV);
        if (cstrRst.GetAt(cstrRst.GetLength() - 1) != '\\') cstrRst += "\\";

        CFileFind finder;
        if (!finder.FindFile(cstrRst + "*.*"))
        {
            uc1601("找不到用户目录($UGII_USER_DIR)或该变量不存在!", 1);
            cstrRst = "";
        }
        finder.Close();
        return cstrRst;
    }

    CString findTempFilePath()//查找临时目录
    {
        CString cstrRst = "";
        char *userDir = NULL;/*[UF_CFI_MAX_PATH_NAME_SIZE]="";*/
        UF_translate_variable("UGII_USER_DIR", &userDir);
        void*userDirV = userDir;
#ifdef UNICODE
        USES_CONVERSION;
        userDirV = A2W(userDir);
#endif // UNICODE
        cstrRst.Format(_T("%s"), userDirV);
        if (cstrRst.GetAt(cstrRst.GetLength() - 1) != '\\') cstrRst += "\\";

        CFileFind finder;
        if (!finder.FindFile(cstrRst + "*.*"))
        {
            uc1601("找不到用户目录(UGII_USER_DIR)或该变量不存在!", 1);
            cstrRst = "";
            finder.Close();
            return cstrRst;
        }
        else
            finder.Close();

        cstrRst += "temp\\";
        if (!finder.FindFile(cstrRst + "*.*"))
        {
            createDirectory(cstrRst);
        }
        finder.Close();
        return cstrRst;
    }

    tag_t createBoundaryBox(tag_t **faces, int &nFaces)
    {
        //setUndoMark();
        tag_t rstBox = NULL_TAG;
        if (nFaces <= 0) return rstBox;

        CSolidObj objFaces(**faces);
        for (int i = 1; i != nFaces; ++i)
        {
            objFaces.adjustObject(*(*faces + i));
        };
        /////caculate boundary box/////
        double dOffset = .5;
        for (int i = 0; i != 3; ++i)
        {
            objFaces.m_limit[i] -= dOffset;
            objFaces.m_limit[3 + i] += dOffset;
            objFaces.m_size[i] += 2 * dOffset;
        }

        if (objFaces.m_size[0] != 0 && objFaces.m_size[1] != 0 && objFaces.m_size[2] != 0)
            rstBox = objFaces.CreateBoundaryBox();

        return rstBox;
    }

    tag_t SelectSingle(char* msg, UF_UI_mask_t *pTrips, int nTrips)            //选择单个目标
    {
        UF_UI_set_cursor_view(0);
        //char *msg="Select single object";
        UF_UI_selection_options_t opts;
        int response = -1;
        tag_t ObjectTag = NULL_TAG;
        double cursor[3] = { 0,0,0 };
        tag_t ViewTag = NULL_TAG;

        /*
        UF_UI_mask_t tripels[2];

        tripels[0].object_type=UF_drafting_entity_type;
        tripels[0].object_subtype=0;
        tripels[0].solid_type=0;

        tripels[1].object_type=UF_circle_type;
        tripels[1].object_subtype=0;
        tripels[1].solid_type=0;

        tripels[2].object_type=UF_spline_type;
        tripels[2].object_subtype=0;
        tripels[2].solid_type=0;

        tripels[0].object_type= UF_dimension_type;
        tripels[0].object_subtype=0;
        tripels[0].solid_type=0;

        tripels[1].object_type=UF_solid_type;
        tripels[1].object_subtype=0;
        tripels[1].solid_type=UF_UI_SEL_FEATURE_ANY_EDGE;
        */

        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;
        opts.reserved = NULL;
        opts.other_options = 0;
        opts.num_mask_triples = nTrips;
        opts.mask_triples = pTrips;

        int returnCode = UF_UI_select_single(msg, &opts, &response, &ObjectTag, cursor, &ViewTag);
        if (returnCode != 0)
        {
            UF_CALL(returnCode);
            return NULL_TAG;
        }
        else if (response != 5) return NULL_TAG;
        else
        {
            //uc1601("Select single over",1);
            UF_DISP_set_highlight(ObjectTag, 0);
            return ObjectTag;
        }
    }


    char* getCurrentDate()
    {
        SYSTEMTIME time;
        ::GetSystemTime(&time);
        char *date = new char[15];
        sprintf(date, "%i/%i/%i", time.wYear, time.wMonth, time.wDay);
        return date;
    }

    void setUndoMark()
    {
        UF_UNDO_mark_id_t markID;
        UF_UNDO_set_mark(UF_UNDO_visible, "elec_tools", &markID);
    }

    char* getCurrentDir()
    {
        char *dir = new char[UF_CFI_MAX_PATH_NAME_LEN];
        tag_t fileTag = NULL_TAG;
        fileTag = UF_PART_ask_display_part();
        UF_PART_ask_part_name(fileTag, dir);
        int pos = 0;
        for (int i = 0; i != UF_CFI_MAX_PATH_NAME_LEN; ++i)
        {
            if (dir[i] == '\\') pos = i;

        }
        dir[pos + 1] = 0;
        char *CurDir = dir;
        return CurDir;
    }


    int askApplication()  //查寻当前UG应用模块
    {
        int appCode = -1;
        char* strApplication = "";
        UF_ask_application_module(&appCode);
        switch (appCode)
        {
        case UF_APP_DRAFTING:
            strApplication = "Drafting";
            break;
        case UF_APP_CAM:
            strApplication = "Manufacturing";
            break;
        case UF_APP_MODELING:
            strApplication = "Modeling";
            break;
        case UF_APP_GATEWAY:
            strApplication = "Gateway";
            break;
        default:
            strApplication = "Other Application";

        }
        //uc1601(strApplication,1);
        return appCode;
    }

    int SelectByClass(CString inf, UF_UI_mask_t *pTrips, int nTrips, tag_t** rstTags, int &nRst)
    {
        UF_UI_set_cursor_view(0);
        void *msgV = NULL;
#ifdef UNICODE
        wchar_t *msgs = inf.GetBuffer();
        wchar_t msg[131] = { 0 };
        wcscpy(msg, msgs);
        USES_CONVERSION;
        msgV = W2A(msg);
#else
        char *msg = inf.GetBuffer();
        msgV = msg;
#endif // UNICODE
        //"Select face....";
        UF_UI_selection_options_t opts;
        int response = -1;
        tag_t ObjectTag = NULL_TAG;
        double cursor[3] = { 0,0,0 };
        tag_t ViewTag = NULL_TAG;

        /*
        int nRst=-1;
        tag_t *tags=NULL;
        UF_UI_mask_t tripels[2];

        tripels[0].object_type=UF_drafting_entity_type;
        tripels[0].object_subtype=0;
        tripels[0].solid_type=0;

        tripels[1].object_type=UF_circle_type;
        tripels[1].object_subtype=0;
        tripels[1].solid_type=0;

        tripels[2].object_type=UF_spline_type;
        tripels[2].object_subtype=0;
        tripels[2].solid_type=0;

        tripels[0].object_type= UF_dimension_type;
        tripels[0].object_subtype=0;
        tripels[0].solid_type=0;

        tripels[1].object_type=UF_solid_type;
        tripels[1].object_subtype=0;
        tripels[1].solid_type=UF_UI_SEL_FEATURE_ANY_EDGE;
        */

        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;
        opts.reserved = NULL;
        opts.other_options = 0;
        opts.num_mask_triples = nTrips;
        opts.mask_triples = pTrips;

        int errCode = UF_UI_select_by_class((char*)msgV, &opts, &response, &nRst, rstTags);
        if (errCode != 0)
        {
            //UF_CALL(errCode);
        }
        for (int i = 0; i != nRst; ++i)
        {
            UF_DISP_set_highlight(*(*rstTags + i), 0);
        }
        return response;
    }

    void createDirectory(CString cstrPath)//根据路径定义产生目录，如c:\\temp\\temp1\\subtemp格式
    {
        vector<CString> vecSubPath;
        vecSubPath.push_back(cstrPath);
        int pos = -1;
        do
        {
            pos = cstrPath.ReverseFind('\\');
            if (pos != -1)
            {
                cstrPath = cstrPath.Left(pos);
                vecSubPath.push_back(cstrPath);
            }
        } while (pos != -1);

        for (int i = vecSubPath.size()&UNDO_SIGNED_BIT - 1; i >= 0; --i)
        {
            CFileFind finder;
            BOOL isFind = finder.FindFile(vecSubPath.at(i));
            if (!isFind)
            {
                finder.Close();
                CreateDirectory(vecSubPath.at(i), NULL);
            }
            else
            {
                finder.Close();
            }
        }
    }

    void getTopAndBottomFace(const tag_t  body, tag_t &topFace, tag_t &bottomFace)
    {
        //查索最高面，需检查其法向
        double dir[3] = { 0,0,0 };//wcs坐标的+z
        double dir2[3] = { 0,0,0 };//wcs坐标的-z
        tag_t wcsTag = NULL_TAG, mtxTag = NULL_TAG;
        double csysOrg[3] = { 0,0,0 }, mtxVal[9] = { 0,0,0,0,0,0,0,0,0 };
        UF_CSYS_ask_wcs(&wcsTag);
        UF_CSYS_ask_csys_info(wcsTag, &mtxTag, csysOrg);
        UF_CSYS_ask_matrix_values(mtxTag, mtxVal);
        UF_MTX3_z_vec(mtxVal, dir);
        for (int i = 0; i != 3; ++i) dir2[i] = -dir[i];

        CSolidObj solidSize(body);

        //遍历实体的面
        uf_list_p_t faceList;//need free1
        UF_MODL_create_list(&faceList);
        int nFace = 0;
        UF_MODL_ask_body_faces(body, &faceList);
        UF_MODL_ask_list_count(faceList, &nFace);
        for (int j = 0; j != nFace; ++j)
        {
            tag_t tempFace = NULL_TAG;
            UF_MODL_ask_list_item(faceList, j, &tempFace);

            int type = 0;//条件1--必须是平面
            UF_MODL_ask_face_type(tempFace, &type);
            if (type != UF_MODL_PLANAR_FACE) continue;

            double faceNor[3] = { 0,0,0 };//条件2--面法向必须与wcs的+Z轴一致
            UF_EVALSF_p_t evaluator;
            double uv[2] = { 0,0 };
            double uvLimit[4] = { 0 };
            UF_MODL_SRF_VALUE_t faceInfo;

            UF_EVALSF_initialize(tempFace, &evaluator);
            UF_EVALSF_ask_face_uv_minmax(evaluator, uvLimit);
            uv[0] = (uvLimit[0] + uvLimit[1]) / 2;
            uv[1] = (uvLimit[2] + uvLimit[3]) / 2;
            UF_EVALSF_evaluate(evaluator, UF_MODL_EVAL_ALL, uv, &faceInfo);

            CSolidObj faceSize(tempFace);//条件3--面尺寸的limit[5]与实体尺寸一致
            if (dir[0] == faceInfo.srf_unormal[0] && dir[1] == faceInfo.srf_unormal[1] && dir[2] == faceInfo.srf_unormal[2])
            {
                if (fabs(faceSize.m_limit[5] - solidSize.m_limit[5]) < 0.01)	topFace = tempFace;
            }

            if (dir2[0] == faceInfo.srf_unormal[0] && dir2[1] == faceInfo.srf_unormal[1] && dir2[2] == faceInfo.srf_unormal[2])
            {
                if (fabs(faceSize.m_limit[2] - solidSize.m_limit[2]) < 0.01)	bottomFace = tempFace;
            }
            UF_EVALSF_free(&evaluator);
        }
        UF_MODL_delete_list(&faceList);
    }

    void getDirectionPos(const double dir_csys[3], const double origin_csys[3], const double distance, double *rstPos)
    {
        double unitDir[3] = { 0,0,0 };
        double mag = 0;
        UF_VEC3_unitize(dir_csys, 0.001, &mag, unitDir);

        for (int i = 0; i != 3; ++i)
        {
            *(rstPos + i) = distance*unitDir[i] + origin_csys[i];
        }
    }

    void disp(char* str)
    {
        logical isOpen = FALSE;
        UF_UI_is_listing_window_open(&isOpen);
        if (!isOpen) UF_UI_open_listing_window();
        UF_UI_write_listing_window(str);
    }
    void disp(CString str)
    {
        logical isOpen = FALSE;
        UF_UI_is_listing_window_open(&isOpen);
        if (!isOpen) UF_UI_open_listing_window();
        void* buf = NULL;
#ifdef UNICODE
        USES_CONVERSION;
        buf = W2A(str.GetBuffer());
#else
        buf = str.GetBuffer();
#endif // UNICODE

        UF_UI_write_listing_window((char*)buf);
    }
    void disp(int num)
    {
        char buf[32] = { 0 };
        sprintf_s(buf, "-----%d----\n", num);
        disp(buf);
    }
    void disp(double num)
    {
        char buf[32] = { 0 };
        sprintf_s(buf, "-----%f----", num);
        disp(buf);
    }

    void mapWcsToCsys(double wcs[3], double csys[3])//Map point from wcs to csys.
    {
        UF_CSYS_map_point(UF_CSYS_ROOT_WCS_COORDS, wcs, UF_CSYS_ROOT_COORDS, csys);
    }

    void mapCsysToWcs(double csys[3], double wcs[3])//Map point from csys to wcs.
    {
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, csys, UF_CSYS_ROOT_WCS_COORDS, wcs);
    }

    void mapCsysToWcs_vector(double csys[3], double wcs[3])//Map direction from csys to wcs.
    {
        mapCsysToWcs(csys, wcs);
        double csysOrigin[3] = { 0,0,0 }, csysOrigin_wcs[3] = { 0,0,0 };
        mapCsysToWcs(csysOrigin, csysOrigin_wcs);

        for (int i = 0; i != 3; ++i)
            wcs[i] = wcs[i] - csysOrigin_wcs[i];
    }

    void mapWcsToCsys_vector(double wcs[3], double csys[3])//Map direction from csys to wcs.
    {
        double origin_wcs[3] = { 0,0,0 }, origin_temp[3] = { 0,0,0 };
        double vec_temp[3] = { 0,0,0 };
        mapWcsToCsys(origin_wcs, origin_temp);
        mapWcsToCsys(wcs, vec_temp);
        for (int i = 0; i != 3; ++i)  csys[i] = vec_temp[i] - origin_temp[i];
    }


    sInitInfo::sInitInfo()
    {
        tag_t part = UF_PART_ask_display_part();

        CString tempFileName = findTempFilePath() + "autoPgmRecord.ini";
        CFileFind finder;
        BOOL isFind = finder.FindFile(tempFileName);
        finder.Close();

        CStdioFile recordFile;
        if (recordFile.Open(tempFileName, CFile::modeRead | CFile::typeText))
        {
            CString readLine = "";
            recordFile.ReadString(readLine);//空
            recordFile.ReadString(cstrPartSpec);
            recordFile.ReadString(cstrPartType);

            void* buf = NULL;
#ifdef UNICODE
            USES_CONVERSION;
            buf = W2A(readLine.GetBuffer());
#else
            buf = readLine.GetBuffer();
#endif // UNICODE

            recordFile.ReadString(readLine);
            isHardness = atoi((char*)buf) != 0 ? true : false;

            recordFile.ReadString(readLine);
            dStockRough = atof((char*)buf);

            recordFile.ReadString(readLine);
            dStockSemi = atof((char*)buf);

            recordFile.ReadString(readLine);
            dStockFinish = atof((char*)buf);

            recordFile.ReadString(readLine);
            isCreateGroup = atoi((char*)buf) != 0 ? true : false;

            recordFile.ReadString(cstrLevel1Prefix);
            recordFile.ReadString(cstrLevel2Prefix);

            recordFile.ReadString(readLine);
            iSubGroupQty = atoi((char*)buf);

            recordFile.ReadString(cstrMcsName);
            recordFile.Close();
            isFind = true;

            if (1)
            {
                CString info;
                info.Format(_T("isFind=%i,,cstrPartSpec=%s,isHardness=%i,dStockRough=%.2f,dStockSemi=%.2f,dStockFinish=%.2f,isCreateGroup=%i,cstrLevel1Prefix=%s,cstrLevel2Prefix=%s,iSubGroupQty=%i,cstrMcsName=%s\n")
                    , isFind, cstrPartSpec, isHardness, dStockRough, dStockSemi, dStockFinish, isCreateGroup, cstrLevel1Prefix, cstrLevel2Prefix,
                    iSubGroupQty, cstrMcsName);
            }
        }
    }

    CObjManager::CObjManager()
    {
    }

    CObjManager::~CObjManager()
    {
    }

    void CObjManager::clearUselessData()
    {
        vector<tag_t> eraseObj;
        for (map<tag_t, int>::iterator iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
        {
            if (UF_OBJ_ALIVE != UF_OBJ_ask_status(iter->first))
                eraseObj.push_back(iter->first);//mapObjects.erase(iter);
            else
            {
                UF_OBJ_disp_props_s dispStatus;
                UF_OBJ_ask_display_properties(iter->first, &dispStatus);
                if (dispStatus.blank_status == UF_OBJ_NOT_BLANKED) iter->second = 1;
                else	iter->second = 0;
            }
        }
        for (int i = 0; i != eraseObj.size(); ++i)
        {
            mapObjects.erase(eraseObj.at(i));
        }
        eraseObj.clear();
    }

    int CObjManager::objCount()
    {
        return mapObjects.size()&UNDO_SIGNED_BIT;
    }

    void CObjManager::resetMap()
    {
        mapObjects.clear();
    }

    void CObjManager::addObjects(tag_t** objId, int nObj)
    {
        uf_list_p_t bodyList;
        UF_MODL_create_list(&bodyList);

        for (int i = 0; i != nObj; ++i)
        {
            UF_MODL_put_list_item(bodyList, *(*objId + i));
            if (UF_OBJ_ALIVE != UF_OBJ_ask_status(*(*objId + i))) continue;
            else
            {
                UF_OBJ_disp_props_s dispStatus;
                UF_OBJ_ask_display_properties(*(*objId + i), &dispStatus);
                if (dispStatus.blank_status == UF_OBJ_NOT_BLANKED)
                    mapObjects[*(*objId + i)] = 1;
                else
                    mapObjects[*(*objId + i)] = 0;
            }
        }
        //去参数
        UF_MODL_delete_body_parms(bodyList);
        UF_MODL_delete_list(&bodyList);
    }


    CObj::CObj(const tag_t &Tag) :objTag(Tag)
    {

        memset(name, 0, UF_OBJ_NAME_LEN + 1);
        if (objTag)
        {
            UF_OBJ_ask_display_properties(objTag, &dispProps);
            UF_OBJ_ask_name(objTag, name);
        }

    }

    CObj::CObj()
    {
        memset(name, 0, (UF_OBJ_NAME_LEN + 1));
    }

    void CObj::SetTag(tag_t &Obj)
    {
        objTag = Obj;
        UF_OBJ_ask_display_properties(objTag, &dispProps);
    }

    char* CObj::GetName()
    {
        return name;
    }

    int CObj::SetName(char* Name)
    {
        memset(name,0,sizeof(char)*(UF_OBJ_NAME_LEN + 1));
        strncpy(name,Name,strlen(Name));
        int err = UF_OBJ_set_name(objTag, name);
        return err;
    }

    int CObj::SetColor(int Color)//Input new color ,and old color will be returned;
    {
        int oldColor = dispProps.color;
        if (Color > 216)
        {
            uc1601("Wrong color number", 1);
            return oldColor;
        }
        else
        {
            UF_OBJ_set_color(objTag, Color);
            dispProps.color = Color;
        }

        return oldColor;
    }

    int CObj::GetStatus()
    {
        return UF_OBJ_ask_status(objTag);
    }

    int CObj::GetType()
    {
        int type = -1;
        int subtype = -1;
        UF_OBJ_ask_type_and_subtype(objTag, &type, &subtype);
        return type;

    }

    int CObj::GetSubtype()
    {
        int type = -1;
        int subtype = -1;
        UF_OBJ_ask_type_and_subtype(objTag, &type, &subtype);
        return subtype;
    }

    int CObj::SetBlankStatus(int bk)
    {
        if (UF_OBJ_NOT_BLANKED != bk || UF_OBJ_BLANKED != bk)
        {
            uc1601("Input wrong value!", 1);
            return -1;
        }
        int oldValue = dispProps.blank_status;
        UF_OBJ_set_blank_status(objTag, bk);
        UF_OBJ_ask_display_properties(objTag, &dispProps);
        return oldValue;

    }

    int CObj::SetLineWidth(int lw)
    {
        if (UF_OBJ_WIDTH_NORMAL != lw || UF_OBJ_WIDTH_THICK != lw || UF_OBJ_WIDTH_THIN != lw)
        {
            uc1601("Can not find this font.", 1);
            return -1;
        }
        int oldValue = dispProps.line_width;
        UF_OBJ_set_line_width(objTag, lw);
        UF_OBJ_ask_display_properties(objTag, &dispProps);
        return oldValue;
    }

    int CObj::SetLineFont(int lf)
    {
        int oldValue = dispProps.font;
        UF_OBJ_set_font(objTag, lf);
        UF_OBJ_ask_display_properties(objTag, &dispProps);
        return oldValue;
    }

    int CObj::SetLayer(int ly)
    {
        if (ly > 256 || ly <= 0)
        {
            uc1601("Over range(1~256)", 1);
            return -1;
        }
        int oldValue = dispProps.layer;
        UF_OBJ_set_layer(objTag, ly);
        UF_OBJ_ask_display_properties(objTag, &dispProps);
        return oldValue;
    }

    int CObj::GetDispStatus()
    {
        return UF_OBJ_ask_status(objTag);
    }

    //.................member Function of class CSolidObj..............//

    CSolidObj::CSolidObj(const tag_t & sObj) :CObj(sObj)
    {

        memset(m_limit, 0, sizeof(double) * 6);
        memset(m_size, 0, sizeof(double) * 3);
        memset(m_topCenter, 0, sizeof(double) * 3);
        memset(m_bottomCenter, 0, sizeof(double) * 3);
        m_isOK = true;
        if (sObj != 0)
            initialize(sObj);
    }

    CSolidObj::~CSolidObj()
    {
    }

    bool CSolidObj::initialize(const tag_t & solidTag)
    {
        double minCorner[3] = { 0 };
        double direction[3][3] = { 0 };
        double tempMin[3] = { 0 };
        tag_t curWcs = NULL_TAG;
        UF_CSYS_ask_wcs(&curWcs);
        UF_MODL_ask_bounding_box_exact(solidTag, curWcs, tempMin, direction, m_size);//csys output
        mapCsysToWcs(tempMin, minCorner);
        if (m_size[0] == 0 && m_size[1] == 0 && m_size[2] == 0)
        {
            m_count--;
            return false;
        }
        m_count++;
        m_limit[0] = minCorner[0];
        m_limit[1] = minCorner[1];
        m_limit[2] = minCorner[2];
        m_limit[3] = minCorner[0] + m_size[0];
        m_limit[4] = minCorner[1] + m_size[1];
        m_limit[5] = minCorner[2] + m_size[2];
        m_topCenter[0] = m_limit[0] + m_size[0] / 2;
        m_topCenter[1] = m_limit[1] + m_size[1] / 2;
        m_topCenter[2] = m_limit[2] + m_size[2];
        m_bottomCenter[0] = m_limit[0] + m_size[0] / 2;
        m_bottomCenter[1] = m_limit[1] + m_size[1] / 2;
        m_bottomCenter[2] = m_limit[2];
        return true;

    }

    tag_t CSolidObj::CreateBoundaryBox()
    {

        tag_t boxTag = NULL_TAG;
        tag_t boxBody = NULL_TAG;
        double tempPoint[3] = { m_limit[0],m_limit[1],m_limit[2] };
        double minCorner[3] = { m_limit[0],m_limit[1],m_limit[2] };

        mapWcsToCsys(minCorner, tempPoint);
        char boxSize[3][30] = { "" };
        char* pSize[3];
        for (int i = 0; i != 3; ++i)
        {
            //if(size[i]<0.001) return NULL_TAG;
            sprintf_s(boxSize[i], "%.3f", m_size[i]);
            pSize[i] = boxSize[i];
        }
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN, tempPoint, pSize, &boxTag));
        UF_MODL_ask_feat_body(boxTag, &boxBody);
        UF_CALL(UF_OBJ_set_color(boxBody, 200));
        UF_CALL(UF_OBJ_set_translucency(boxBody, 30));

        return boxBody;
    }

    void CSolidObj::addOneObject(const tag_t & SecondTag)
    {
        if (m_count == 0)
        {
            objTag = SecondTag;
            memset(name, 0, UF_OBJ_NAME_LEN + 1);
            UF_OBJ_ask_display_properties(objTag, &dispProps);
            UF_OBJ_ask_name(objTag, name);
            initialize(SecondTag);
            m_count++;
            return;
        }
        double oldLimit[6] = { 0 };
        double oldSize[3] = { 0 };
        for (int i = 0; i != 6; ++i)
        {
            oldLimit[i] = m_limit[i];
            if (i<3)
                oldSize[i] = m_size[i];
        }

        if (!initialize(SecondTag)) return;

        if (m_limit[0] > oldLimit[0]) m_limit[0] = oldLimit[0];
        if (m_limit[1] > oldLimit[1]) m_limit[1] = oldLimit[1];
        if (m_limit[2] > oldLimit[2]) m_limit[2] = oldLimit[2];
        for (int i = 0; i != 3; ++i)
        {
            m_size[i] += oldSize[i];
            m_limit[i + 3] = m_limit[i] + m_size[i];
        }

        m_topCenter[0] = m_limit[0] + m_size[0] / 2;
        m_topCenter[1] = m_limit[1] + m_size[1] / 2;
        m_topCenter[2] = m_limit[2] + m_size[2];
        m_bottomCenter[0] = m_limit[0] + m_size[0] / 2;
        m_bottomCenter[1] = m_limit[1] + m_size[1] / 2;
        m_bottomCenter[2] = m_limit[2];
    }

    void CSolidObj::adjustObject(tag_t SecondTag)
    {
        if (m_count == 0)
        {
            objTag = SecondTag;
            memset(name, 0, UF_OBJ_NAME_LEN + 1);
            UF_OBJ_ask_display_properties(objTag, &dispProps);
            UF_OBJ_ask_name(objTag, name);
            initialize(SecondTag);
            m_count++;
            return;
        }
        const int nMember = 6;
        double oldLimit[nMember];
        for (int i = 0; i != nMember; ++i)
        {
            oldLimit[i] = m_limit[i];
        }
        if (!initialize(SecondTag)) return;
        if (m_limit[0] > oldLimit[0]) m_limit[0] = oldLimit[0];
        if (m_limit[1] > oldLimit[1]) m_limit[1] = oldLimit[1];
        if (m_limit[2] > oldLimit[2]) m_limit[2] = oldLimit[2];
        if (m_limit[3] < oldLimit[3]) m_limit[3] = oldLimit[3];
        if (m_limit[4] < oldLimit[4]) m_limit[4] = oldLimit[4];
        if (m_limit[5] < oldLimit[5]) m_limit[5] = oldLimit[5];

        for (int i = 0; i != nMember / 2; ++i)
        {
            m_size[i] = fabs(m_limit[nMember / 2 + i] - m_limit[i]);
        }

        m_topCenter[0] = m_limit[0] + m_size[0] / 2;
        m_topCenter[1] = m_limit[1] + m_size[1] / 2;
        m_topCenter[2] = m_limit[2] + m_size[2];
        m_bottomCenter[0] = m_limit[0] + m_size[0] / 2;
        m_bottomCenter[1] = m_limit[1] + m_size[1] / 2;
        m_bottomCenter[2] = m_limit[2];
    }

    CSolidObj& CSolidObj::operator=(const CSolidObj& r)
    {
        if (this != &r)
        {
            this->m_isOK = r.m_isOK;
            memcpy(this->m_limit, r.m_limit, sizeof(double) * 6);
            memcpy(this->m_size, r.m_size, sizeof(double) * 3);
            memcpy(this->m_topCenter, r.m_topCenter, sizeof(double) * 3);
            memcpy(this->m_bottomCenter, r.m_bottomCenter, sizeof(double) * 3);
            strcpy(this->name, r.name);
            this->objTag = r.objTag;
        }
        return *this;
    }

    tag_t CSolidObj::CreateBoundaryCylinder()
    {

        tag_t boxTag = NULL_TAG;
        tag_t boxBody = NULL_TAG;
        double tempPoint[3] = { m_limit[0],m_limit[1],m_limit[2] };
        double minCorner[3] = { m_limit[0],m_limit[1],m_limit[2] };

        mapWcsToCsys(minCorner, tempPoint);
        char boxSize[3][30] = { "" };
        char* pSize[3];
        for (int i = 0; i != 3; ++i)
        {
            //if(size[i]<0.001) return NULL_TAG;
            sprintf_s(boxSize[i], "%.3f", m_size[i]);
            pSize[i] = boxSize[i];
        }

        char *dia, *height = pSize[2];
        if (m_size[0] > m_size[1]) dia = pSize[0]; else dia = pSize[1];

        double dir[3] = { 0,0,0 };
        tag_t wcsTag = NULL_TAG, mtxTag = NULL_TAG;
        double csysOrg[3] = { 0,0,0 }, mtxVal[9] = { 0,0,0,0,0,0,0,0,0 };
        UF_CSYS_ask_wcs(&wcsTag);
        UF_CSYS_ask_csys_info(wcsTag, &mtxTag, csysOrg);
        UF_CSYS_ask_matrix_values(mtxTag, mtxVal);
        UF_MTX3_z_vec(mtxVal, dir);

        for (int i = 0; i != 2; ++i) tempPoint[i] += (m_size[i] / 2);
        UF_MODL_create_cyl1(UF_NULLSIGN, tempPoint, height, dia, dir, &boxTag);

        UF_MODL_ask_feat_body(boxTag, &boxBody);
        UF_CALL(UF_OBJ_set_color(boxBody, 200));
        UF_CALL(UF_OBJ_set_translucency(boxBody, 30));

        return boxBody;
    }

    /**************************************************************************************************/


    tag_p_t p_listCreateTags(I const uf_list_p_t temp, O size_t & numOfTags)
    {
        numOfTags = 0;
        uf_list_p_t tempIndex = temp;
        while (tempIndex != NULL)
        {
            numOfTags++;
            tempIndex = tempIndex->next;
        }
        int i = 0;
        tag_p_t tagSet = new tag_t[numOfTags];
        tempIndex = temp;
        while (tempIndex != NULL)
        {
            tagSet[i] = tempIndex->eid;
            tempIndex = tempIndex->next;
            i++;
        }
        return tagSet;
    }

    uf_list_p_t p_TagsCreateList(I const size_t numOfTag, I const tag_p_t temp)
    {
        uf_list_p_t templist = NULL, nextList = NULL, tempIndex = NULL;
        nextList = new uf_list_t;
        templist = nextList;
        for (int i = 0; i < numOfTag; i++)
        {
            nextList->eid = temp[i];
            tempIndex = nextList;
            nextList->next = new uf_list_t;
            nextList = nextList->next;
        }
        delete nextList; nextList = NULL;
        tempIndex->next = NULL;
        return templist;
    }

    void deleteList(uf_list_p_t&list_s)
    {
        if (list_s == NULL)
        {
            return;
        }
        if (list_s->next != NULL)
        {
            deleteList(list_s->next);
        }
        delete list_s;
        list_s = NULL;
        return;
    }

    uf_list_p_t extrudedOperation(const size_t numOfTag, const tag_p_t temp, double *direction,
        const double LimitUp, const double LimitDown)
    {
        uf_list_p_t tempList = p_TagsCreateList(numOfTag, temp);
        char * taper_angle = "0.0";
        char * limit[2] = { NULL };
        char buf[32] = { 0 };
        char buf1[32] = { 0 };
        sprintf_s(buf, "%f", LimitUp);
        sprintf_s(buf1, "%f", LimitDown);
        limit[0] = buf;
        limit[1] = buf1;
        double point[3] = { 0 };
        UF_FEATURE_SIGN sign = UF_NULLSIGN;
        uf_list_p_t  features;

        int ret = UF_CALL(UF_MODL_create_extruded(tempList, taper_angle, limit, point, direction, sign, &features));
        deleteList(tempList);
        return features;
    }

    uf_list_p_t extrudedOperation(const uf_list_p_t temp, const Vector3d &vector3, const double LimitUp,
        const double LimitDown)
    {
        char * taper_angle = "0.0";
        char * limit[2] = { NULL };
        char buf[8] = { 0 };
        char buf1[8] = { 0 };
        sprintf_s(buf, "%f", LimitUp);
        sprintf_s(buf1, "%f", LimitDown);
        limit[0] = buf;
        limit[1] = buf1;
        double point[3] = { 0 };
        double direction[3] = { vector3.X,vector3.Y,vector3.Z };
        UF_FEATURE_SIGN sign = UF_NULLSIGN;
        uf_list_p_t  features;
        UF_CALL(UF_MODL_create_extruded(temp, taper_angle, limit, point, direction, sign, &features));
        return features;
    }


    tag_p_t createJoinedCurves(tag_t * pCurves, int &counts)
    {
        if (pCurves == NULL)
        {
            return NULL;
        }
        tag_t* joined_curve = new tag_t[counts];
        for (int i = 0; i < counts; i++)
            joined_curve[i] = null_tag;
        UF_initialize();
        UF_CALL(UF_CURVE_auto_join_curves(pCurves, UNDO_SIGNED_BIT&counts, 2, joined_curve, &counts));
        UF_TERMINATE();
        return joined_curve;
    }

    double get_line_length(tag_t line, double * start_pt, double * end_pt)
    {
        UF_CURVE_line_t line_coords;
        UF_CURVE_ask_line_data(line, &line_coords);
        if (start_pt) UF_VEC3_copy(line_coords.start_point, start_pt);
        if (end_pt) UF_VEC3_copy(line_coords.end_point, end_pt);
        double length;
        UF_VEC3_distance(line_coords.start_point, line_coords.end_point, &length);
        return length;
    }

    double get_arc_length(tag_t arc, double * centerPt, double * radius)
    {
        //UF_EVAL_p_t evaluator;
        //UF_EVAL_arc_t eval_arc;
        UF_CURVE_arc_t arc_coords;
        double arc_length;
        //int is_open = 0;
        //double param_range[2] = { 0,0 };
        //UF_EVAL_initialize(arc, &evaluator);
        //UF_EVAL_ask_arc(evaluator, &eval_arc);
        UF_CURVE_ask_arc_data(arc, &arc_coords);
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

    double get_spline_length(tag_t& spline, bool is_Edge /*= false*/)
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

    tag_t createPointSetFeat(tag_t curve_, int count_)
    {
        Session *theSession = Session::GetSession();
        Part *workPart(theSession->Parts()->Work());

        Features::PointSet *nullFeatures_PointSet(NULL);
        if (!workPart->Preferences()->Modeling()->GetHistoryMode())
        {
            throw NXException::Create("Create or edit of a Feature was recorded in History Mode but playback is in History-Free Mode.");
        }

        Features::PointSetBuilder *pointSetBuilder1 = workPart->Features()->CreatePointSetBuilder(nullFeatures_PointSet);
        pointSetBuilder1->StartPercentage()->SetRightHandSide("0");
        pointSetBuilder1->EndPercentage()->SetRightHandSide("100");
        pointSetBuilder1->Ratio()->SetRightHandSide("1");
        pointSetBuilder1->ChordalTolerance()->SetRightHandSide("1");
        pointSetBuilder1->ArcLength()->SetRightHandSide("1");

        pointSetBuilder1->NumberOfPointsInUDirectionExpression()->SetRightHandSide("10");
        pointSetBuilder1->NumberOfPointsInVDirectionExpression()->SetRightHandSide("10");
        pointSetBuilder1->SetPatternLimitsBy(Features::PointSetBuilder::PatternLimitsTypePercentages);

        pointSetBuilder1->PatternLimitsStartingUValue()->SetRightHandSide("0");
        pointSetBuilder1->PatternLimitsEndingUValue()->SetRightHandSide("100");

        pointSetBuilder1->PatternLimitsStartingVValue()->SetRightHandSide("0");
        pointSetBuilder1->PatternLimitsEndingVValue()->SetRightHandSide("100");

        Unit *nullUnit(NULL);
        Expression *expression1;
        expression1 = workPart->Expressions()->CreateSystemExpressionWithUnits("50", nullUnit);

        pointSetBuilder1->CurvePercentageList()->Append(expression1);

        Features::PointSetFacePercentageBuilder *pointSetFacePercentageBuilder1 = pointSetBuilder1->CreateFacePercentageListItem();
        pointSetBuilder1->FacePercentageList()->Append(pointSetFacePercentageBuilder1);
        expression1->SetRightHandSide("0");
        pointSetFacePercentageBuilder1->UPercentage()->SetRightHandSide("0");
        pointSetFacePercentageBuilder1->VPercentage()->SetRightHandSide("0");
        pointSetBuilder1->SingleCurveOrEdgeCollector()->SetDistanceTolerance(0.001);
        pointSetBuilder1->SingleCurveOrEdgeCollector()->SetChainingTolerance(0.00095);


        pointSetBuilder1->MultipleCurveOrEdgeCollector()->SetChainingTolerance(0.00095);
        pointSetBuilder1->MultipleCurveOrEdgeCollector()->SetAngleTolerance(0.05);

        pointSetBuilder1->StartPercentageSection()->SetDistanceTolerance(0.001);
        pointSetBuilder1->StartPercentageSection()->SetChainingTolerance(0.00095);
        pointSetBuilder1->StartPercentageSection()->SetAngleTolerance(0.05);

        pointSetBuilder1->EndPercentageSection()->SetDistanceTolerance(0.001);
        pointSetBuilder1->EndPercentageSection()->SetChainingTolerance(0.00095);
        pointSetBuilder1->EndPercentageSection()->SetAngleTolerance(0.05);

        pointSetBuilder1->IntersectionSection()->SetDistanceTolerance(0.001);
        pointSetBuilder1->IntersectionSection()->SetChainingTolerance(0.00095);
        pointSetBuilder1->IntersectionSection()->SetAngleTolerance(0.05);

        pointSetBuilder1->SingleCurveOrEdgeCollector()->SetAngleTolerance(0.05);
        pointSetBuilder1->SingleCurveOrEdgeCollector()->SetAllowedEntityTypes(Section::AllowTypesOnlyCurves);

        std::vector<IBaseCurve *> curves1(1);

        Spline *spline1 = (Spline *)(NXOpen::NXObjectManager::Get(curve_));

        curves1[0] = spline1;
        CurveDumbRule *curveDumbRule1;
        curveDumbRule1 = workPart->ScRuleFactory()->CreateRuleBaseCurveDumb(curves1);
        pointSetBuilder1->SingleCurveOrEdgeCollector()->AllowSelfIntersection(true);

        std::vector<SelectionIntentRule *> rules1(1);
        rules1[0] = curveDumbRule1;
        NXObject *nullNXObject(NULL);
        Point3d helpPoint1(2331.34369734255, 505.746802068877, 70.0);
        pointSetBuilder1->SingleCurveOrEdgeCollector()->AddToSection(rules1, spline1, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        pointSetBuilder1->SetCurvePointsBy(Features::PointSetBuilder::CurvePointsTypeEqualArcLength);

        char buf[8] = { 0 };
        sprintf(buf, "%d", count_);
        pointSetBuilder1->NumberOfPointsExpression()->SetRightHandSide(buf);
        NXObject *nXObject1 = pointSetBuilder1->Commit();
        pointSetBuilder1->Destroy();

        return nXObject1->Tag();
    }

    tag_t createFitCureFeat(tag_t pointSetFeat)
    {
        Session *theSession = Session::GetSession();
        Part *workPart(theSession->Parts()->Work());
        Part *displayPart(theSession->Parts()->Display());

        Features::FitCurve *nullFeatures_FitCurve(NULL);
        if (!workPart->Preferences()->Modeling()->GetHistoryMode())
            throw NXException::Create("Create or edit of a Feature was recorded in History Mode but playback is in History-Free Mode.");

        NXOpen::Features::FitCurveBuilder *fitCurveBuilder1;
        fitCurveBuilder1 = workPart->Features()->CreateFitCurveBuilder(nullFeatures_FitCurve);
        fitCurveBuilder1->SetTolerance(0.001);
        fitCurveBuilder1->SetTargetSourceType(Features::FitCurveBuilder::TargetSourceTypesChainedPoints);
        fitCurveBuilder1->SetCanUseAllPointsInPart(false);
        fitCurveBuilder1->SetProjectionDirectionOption(Features::FitCurveBuilder::ProjectionDirectionOptionsNormal);
        fitCurveBuilder1->Radius()->SetRightHandSide("50");
        fitCurveBuilder1->SetFittingParameters(Features::FitCurveBuilder::FittingParametersOptionsDegreeAndTolerance);
        fitCurveBuilder1->SetDegree(2);
        fitCurveBuilder1->Extender()->EndValue()->SetRightHandSide("0");
        fitCurveBuilder1->RejectionThreshold()->SetRightHandSide("10");
        std::vector<TaggedObject *> objects1;
        UF_initialize();

        int num = 0;
        tag_p_t eid = 0;
        UF_MODL_ask_feat_object(pointSetFeat, &num, &eid);
        for (int i = 0; i < num; i++)
        {
            objects1.push_back(NXObjectManager::Get(eid[i]));
        }
        UF_free(eid);

        UF_TERMINATE();

        bool added1 = fitCurveBuilder1->Target()->Add(objects1);
        Features::GeometricConstraintData *geometricConstraintData1;
        geometricConstraintData1 = fitCurveBuilder1->ConstraintManager()->FindItem(0);

        Point *point2;
        point2 = geometricConstraintData1->Point();

        Features::GeometricConstraintData *geometricConstraintData2;
        geometricConstraintData2 = fitCurveBuilder1->ConstraintManager()->FindItem(1);

        Point *point3;
        point3 = geometricConstraintData2->Point();
        fitCurveBuilder1->SetClosedBSpline(false);
        fitCurveBuilder1->Commit();
        fitCurveBuilder1->Destroy();
        UF_initialize();

        tag_t intersect_feature;
        UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &intersect_feature);

        return intersect_feature;
    }

    tag_t offsetCurve(tag_t fitCurve, double dist, double* dir, bool flag)
    {
        Session *theSession = Session::GetSession();
        Part *workPart(theSession->Parts()->Work());
        Part *displayPart(theSession->Parts()->Display());
        // ----------------------------------------------
        //   Menu: Insert->Curve from Curves->Offset...
        // ----------------------------------------------

        Features::Feature *nullFeatures_Feature(NULL);

        if (!workPart->Preferences()->Modeling()->GetHistoryMode())
        {
            throw NXException::Create("Create or edit of a Feature was recorded in History Mode but playback is in History-Free Mode.");
        }

        Features::OffsetCurveBuilder *offsetCurveBuilder1;
        offsetCurveBuilder1 = workPart->Features()->CreateOffsetCurveBuilder(nullFeatures_Feature);

        Unit *unit1;
        unit1 = offsetCurveBuilder1->OffsetDistance()->Units();

        Expression *expression1;
        expression1 = workPart->Expressions()->CreateSystemExpressionWithUnits("0", unit1);

        offsetCurveBuilder1->CurveFitData()->SetTolerance(0.01);
        offsetCurveBuilder1->CurveFitData()->SetAngleTolerance(0.05);
        char buf[64] = { 0 };
        sprintf(buf, "%f", dist);
        offsetCurveBuilder1->OffsetDistance()->SetRightHandSide(buf);//////////////////////
        offsetCurveBuilder1->DraftHeight()->SetRightHandSide("5");
        offsetCurveBuilder1->DraftAngle()->SetRightHandSide("0");
        offsetCurveBuilder1->LawControl()->Value()->SetRightHandSide("5");
        offsetCurveBuilder1->LawControl()->StartValue()->SetRightHandSide("5");
        offsetCurveBuilder1->LawControl()->EndValue()->SetRightHandSide("5");
        offsetCurveBuilder1->Offset3dDistance()->SetRightHandSide("5");
        offsetCurveBuilder1->InputCurvesOptions()->SetInputCurveOption(GeometricUtilities::CurveOptions::InputCurveRetain);
        offsetCurveBuilder1->SetTrimMethod(Features::OffsetCurveBuilder::TrimOptionExtendTangents);
        offsetCurveBuilder1->LawControl()->AlongSpineData()->SetFeatureSpine(offsetCurveBuilder1->CurvesToOffset());

        Expression *expression2;
        expression2 = workPart->Expressions()->CreateSystemExpressionWithUnits("0", unit1);

        offsetCurveBuilder1->CurvesToOffset()->SetDistanceTolerance(0.01);
        offsetCurveBuilder1->CurvesToOffset()->SetChainingTolerance(0.00095);
        offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetDistanceTolerance(0.01);
        offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetChainingTolerance(0.00095);
        offsetCurveBuilder1->LawControl()->LawCurve()->SetDistanceTolerance(0.01);
        offsetCurveBuilder1->LawControl()->LawCurve()->SetChainingTolerance(0.00095);
        offsetCurveBuilder1->CurvesToOffset()->SetAngleTolerance(0.05);
        offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetAngleTolerance(0.05);
        offsetCurveBuilder1->LawControl()->LawCurve()->SetAngleTolerance(0.05);
        offsetCurveBuilder1->CurvesToOffset()->SetAllowedEntityTypes(Section::AllowTypesOnlyCurves);

        std::vector<Features::Feature *> features1(1);
        Features::FitCurve *fitCurve1 = (Features::FitCurve *)NXObjectManager::Get(fitCurve);////////
        features1[0] = (Feature*)fitCurve1;
        CurveFeatureRule *curveFeatureRule1;
        curveFeatureRule1 = workPart->ScRuleFactory()->CreateRuleCurveFeature(features1);

        offsetCurveBuilder1->CurvesToOffset()->AllowSelfIntersection(true);

        std::vector<SelectionIntentRule *> rules1(1);
        rules1[0] = (SelectionIntentRule *)curveFeatureRule1;

        UF_initialize();
        int num = 0;
        tag_p_t eid = 0;
        UF_CALL(UF_MODL_ask_feat_object(fitCurve, &num, &eid));
        tag_t spline_ = eid[0];
        UF_free(eid);


        double pt[3] = { 0 };
        getCurvePt(spline_, 0.5, pt);
        Point3d helpPoint1(pt[0], pt[1], pt[2]);

        Spline *spline1 = (Spline *)NXObjectManager::Get(spline_);
        NXObject *nullNXObject(NULL);
        offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, spline1, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);

        if (dir)
        {
            double pt[3] = { 0 };
            MyFun::getCurvePt(spline_, 0, pt);
            Point3d origin1(pt[0], pt[1], pt[2]);/////////////Obsolete 
            Vector3d vector1(dir[0], dir[1], dir[2]);
            Direction *direction1 = workPart->Directions()->CreateDirection(origin1, vector1, SmartObject::UpdateOptionWithinModeling);
            offsetCurveBuilder1->SetAxialOffsetDirection(direction1);
        }
        else
        {
            Vector3d offsetdirection1;
            Point3d startpoint1;
            try
            {
                offsetCurveBuilder1->ComputeOffsetDirection(&offsetdirection1, &startpoint1);
            }
            catch (const NXException &ex)
            {
                ex.AssertErrorCode(671415);
            }
        }

        if (flag)
            offsetCurveBuilder1->SetReverseDirection(true);
        offsetCurveBuilder1->SetRoughOffset(true);
        NXObject *nXObject1 = offsetCurveBuilder1->Commit();

        offsetCurveBuilder1->Destroy();
        workPart->Expressions()->Delete(expression1);
        workPart->Expressions()->Delete(expression2);

        tag_t intersect_feature;
        UF_CALL(UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &intersect_feature));
        UF_CALL(UF_MODL_ask_feat_object(intersect_feature, &num, &eid));
        tag_t temp_ = eid[0];
        UF_free(eid);
        MyFun::DeleteParms(1, &temp_);
        UF_TERMINATE();

        return temp_;
    }

    void offsetCurve(vector<tag_t> Curves__, double dist, vector<tag_t>& rt, const double* curvesCenterPt /*= NULL*/, const bool is_reverseDirection /*= false*/)
    {
        Session *theSession = Session::GetSession();
        Part *workPart(theSession->Parts()->Work());
        Part *displayPart(theSession->Parts()->Display());
        // ----------------------------------------------
        //   Menu: Insert->Curve from Curves->Offset...
        // ----------------------------------------------

        Features::Feature *nullFeatures_Feature(NULL);

        if (!workPart->Preferences()->Modeling()->GetHistoryMode())
        {
            throw NXException::Create("Create or edit of a Feature was recorded in History Mode but playback is in History-Free Mode.");
        }

        Features::OffsetCurveBuilder *offsetCurveBuilder1;
        offsetCurveBuilder1 = workPart->Features()->CreateOffsetCurveBuilder(nullFeatures_Feature);

        offsetCurveBuilder1->CurveFitData()->SetTolerance(0.01);
        offsetCurveBuilder1->CurveFitData()->SetAngleTolerance(0.05);
        char buf[64] = { 0 };
        sprintf(buf, "%f", dist);
        offsetCurveBuilder1->OffsetDistance()->SetRightHandSide(buf);//////////////////////
        offsetCurveBuilder1->DraftHeight()->SetRightHandSide("5");
        offsetCurveBuilder1->DraftAngle()->SetRightHandSide("0");
        offsetCurveBuilder1->LawControl()->Value()->SetRightHandSide("5");
        offsetCurveBuilder1->LawControl()->StartValue()->SetRightHandSide("5");
        offsetCurveBuilder1->LawControl()->EndValue()->SetRightHandSide("5");
        offsetCurveBuilder1->Offset3dDistance()->SetRightHandSide("5");
        offsetCurveBuilder1->InputCurvesOptions()->SetInputCurveOption(GeometricUtilities::CurveOptions::InputCurveRetain);
        offsetCurveBuilder1->SetTrimMethod(Features::OffsetCurveBuilder::TrimOptionExtendTangents);
        offsetCurveBuilder1->LawControl()->AlongSpineData()->SetFeatureSpine(offsetCurveBuilder1->CurvesToOffset());

        offsetCurveBuilder1->CurvesToOffset()->SetDistanceTolerance(0.01);
        offsetCurveBuilder1->CurvesToOffset()->SetChainingTolerance(0.00095);
        offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetDistanceTolerance(0.01);
        offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetChainingTolerance(0.00095);
        offsetCurveBuilder1->LawControl()->LawCurve()->SetDistanceTolerance(0.01);
        offsetCurveBuilder1->LawControl()->LawCurve()->SetChainingTolerance(0.00095);
        offsetCurveBuilder1->CurvesToOffset()->SetAngleTolerance(0.05);
        offsetCurveBuilder1->LawControl()->AlongSpineData()->Spine()->SetAngleTolerance(0.05);
        offsetCurveBuilder1->LawControl()->LawCurve()->SetAngleTolerance(0.05);
        offsetCurveBuilder1->CurvesToOffset()->SetAllowedEntityTypes(Section::AllowTypesOnlyCurves);

        std::vector<IBaseCurve *> curves1;
        std::vector<Edge *> edges1;

        NXOpen::Line *line_ = NULL;
        NXOpen::Arc *arc_ = NULL;
        NXOpen::Spline *spline_ = NULL;
        NXOpen::Ellipse *ellipse_ = NULL;
        NXOpen::Hyperbola * htperbola_ = NULL;
        NXOpen::Parabola * parabola = NULL;
        NXOpen::Edge* edge_ = NULL;

        int type_ = 0;
        int subType_ = 0;
        int type_1 = 0;
        int subtype_1 = 0;

        UF_initialize();

        UF_OBJ_ask_type_and_subtype(Curves__[0], &type_1, &subtype_1);
        for (int i = 0; i < Curves__.size(); i++)
        {
            UF_OBJ_ask_type_and_subtype(Curves__[i], &type_, &subType_);
            if (type_ == 70 && subType_ == 3)
            {
                if (type_1 != type_ || subtype_1 != subType_)
                {
                    MyFun::disp("输入线型有误!");
                    return;
                }
            }

            if (type_ == UF_line_type)
            {
                line_ = (NXOpen::Line *)NXObjectManager::Get(Curves__[i]);
                curves1.push_back(line_);
            }
            else if (type_ == UF_circle_type)
            {
                arc_ = (NXOpen::Arc *)NXObjectManager::Get(Curves__[i]);
                curves1.push_back(arc_);
            }
            else if (type_ == UF_spline_type)
            {
                spline_ = (Spline  *)NXObjectManager::Get(Curves__[i]);
                curves1.push_back(spline_);
            }
            else if (type_ == UF_solid_type&&subType_ == UF_solid_edge_subtype)
            {
                edge_ = (NXOpen::Edge *)NXObjectManager::Get(Curves__[i]);
                edges1.push_back(edge_);
            }
            else if (type_ == UF_conic_type || subType_ == UF_conic_ellipse_subtype)
            {
                ellipse_ = (NXOpen::Ellipse *)NXObjectManager::Get(Curves__[i]);
                curves1.push_back(ellipse_);
            }
            else if (type_ == UF_conic_type || subType_ == UF_conic_parabola_subtype)
            {
                parabola = (NXOpen::Parabola *)NXObjectManager::Get(Curves__[i]);
                curves1.push_back(parabola);

            }
            else if (type_ == UF_conic_type || subType_ == UF_conic_hyperbola_subtype)
            {
                htperbola_ = (NXOpen::Hyperbola *)NXObjectManager::Get(Curves__[i]);
                curves1.push_back(htperbola_);
            }
        }

        vector<SelectionIntentRule *> rules1;
        if (type_ == 70)
            rules1.push_back(workPart->ScRuleFactory()->CreateRuleEdgeDumb(edges1));
        else
            rules1.push_back(workPart->ScRuleFactory()->CreateRuleBaseCurveDumb(curves1));

        double pt[3] = { 0 };
        getCurvePt(Curves__[0], 0.5, pt);
        Point3d helpPoint1(pt[0], pt[1], pt[2]);

        NXObject *nullNXObject(NULL);
        if (line_)
            offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, line_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        else if (arc_)
            offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, arc_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        else if (spline_)
            offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, spline_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        else if (ellipse_)
            offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, ellipse_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        else if (htperbola_)
            offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, htperbola_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        else if (parabola)
            offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, parabola, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);
        else if (edge_)
            offsetCurveBuilder1->CurvesToOffset()->AddToSection(rules1, edge_, nullNXObject, nullNXObject, helpPoint1, Section::ModeCreate, false);

        if (curvesCenterPt)
        {
            double pt[3] = { 0 };
            double dir[3] = { 0 };
            MyFun::getCurvePt(Curves__[0], 0, pt);
            Point3d origin1(pt[0], pt[1], pt[2]);//Obsolete 
            UF_VEC3_sub(pt, curvesCenterPt, dir);
            Vector3d vector1(dir[0], dir[1], dir[2]);
            Direction *direction1 = workPart->Directions()->CreateDirection(origin1, vector1, SmartObject::UpdateOptionWithinModeling);
            offsetCurveBuilder1->SetAxialOffsetDirection(direction1);
        }
        else
        {
            Vector3d offsetdirection1;
            Point3d startpoint1;
            try
            {
                offsetCurveBuilder1->ComputeOffsetDirection(&offsetdirection1, &startpoint1);
            }
            catch (const NXException &ex)
            {
                NX_CATCH_EX_INFO;
            }
        }

        if (is_reverseDirection)
            offsetCurveBuilder1->SetReverseDirection(true);
        offsetCurveBuilder1->SetRoughOffset(true);

        tag_t intersect_feature;

        NXCOMMIT_INFO_FEAT(offsetCurveBuilder1, intersect_feature);

        offsetCurveBuilder1->Destroy();
        tag_t  *eid = NULL;
        int num;
        UF_CALL(UF_MODL_ask_feat_object(intersect_feature, &num, &eid));
        tag_t temp_ = eid[0];

        MyFun::DeleteParms(1, &temp_);
        rt.clear();
        for (int i = 0; i < num; i++)
        {
            rt.push_back(eid[i]);
        }
        UF_free(eid);
    }

    void VT_VEC3_rotate(const double vec[3], const double rotation_axis[3], double angle, double rotated_vec[3])
    {
        double  kn[3];
        kn[0] = rotation_axis[0] / sqrt(rotation_axis[0] * rotation_axis[0] + rotation_axis[1] * rotation_axis[1] + rotation_axis[2] * rotation_axis[2]);
        kn[1] = rotation_axis[1] / sqrt(rotation_axis[0] * rotation_axis[0] + rotation_axis[1] * rotation_axis[1] + rotation_axis[2] * rotation_axis[2]);
        kn[2] = rotation_axis[2] / sqrt(rotation_axis[0] * rotation_axis[0] + rotation_axis[1] * rotation_axis[1] + rotation_axis[2] * rotation_axis[2]);  //旋转矩阵
        double rot[3][3], rad;
        rad = angle*PI / 180;
        //矩阵第一行
        rot[0][0] = kn[0] * kn[0] * (1 - cos(rad)) + cos(rad);
        rot[0][1] = kn[0] * kn[1] * (1 - cos(rad)) - kn[2] * sin(rad);
        rot[0][2] = kn[0] * kn[2] * (1 - cos(rad)) + kn[1] * sin(rad);
        //矩阵第二行
        rot[1][0] = kn[0] * kn[1] * (1 - cos(rad)) + kn[2] * sin(rad);
        rot[1][1] = kn[1] * kn[1] * (1 - cos(rad)) + cos(rad);
        rot[1][2] = kn[1] * kn[2] * (1 - cos(rad)) - kn[0] * sin(rad);
        //矩阵第三行
        rot[2][0] = kn[0] * kn[2] * (1 - cos(rad)) - kn[1] * sin(rad);
        rot[2][1] = kn[1] * kn[2] * (1 - cos(rad)) + kn[0] * sin(rad);
        rot[2][2] = kn[2] * kn[2] * (1 - cos(rad)) + cos(rad);  //转换
        rotated_vec[0] = rot[0][0] * vec[0] + rot[0][1] * vec[1] + rot[0][2] * vec[2];
        rotated_vec[1] = rot[1][0] * vec[0] + rot[1][1] * vec[1] + rot[1][2] * vec[2];
        rotated_vec[2] = rot[2][0] * vec[0] + rot[2][1] * vec[1] + rot[2][2] * vec[2];
    }

    double angleOfFaces(tag_t face1, tag_t face2, tag_t edge /*= 0*/)
    {
        double pt[3];
        double pt_[3];
        double pt__[3];
        double pt___[3];
        UF_INITIALIZE();
        if (edge)
        {
            MyFun::getCurvePt(edge, 0.5, pt);
        }
        else
        {
            uf_list_p_t pt1 = NULL;
            uf_list_p_t pt2 = NULL;
            uf_list_p_t pt1_ = NULL;
            uf_list_p_t pt2_ = NULL;
            UF_MODL_ask_face_edges(face1, &pt1);
            UF_MODL_ask_face_edges(face2, &pt2);
            pt1_ = pt1;

            while (pt1_)
            {
                pt2_ = pt2;
                while (pt2_)
                {
                    if (pt1_->eid != pt2_->eid)
                        pt2_ = pt2_->next;
                    else
                    {
                        edge = pt1_->eid;
                        break;
                    }
                }
                if (edge)
                    break;
                else
                    pt1_ = pt1_->next;
            }
            UF_MODL_delete_list(&pt1);
            UF_MODL_delete_list(&pt2);
            MyFun::getCurvePt(edge, 0.5, pt);
        }
        int type_;
        double angle;
        UF_MODL_ask_edge_type(edge, &type_);

        double dir_1[3] = { 0 };
        double dir_2[3] = { 0 };
        double dir_3[3] = { 0 };
        double magnitude;
        MyFun::getFaceNormal(face1, dir_1);
        MyFun::getFaceNormal(face2, dir_2);
        UF_VEC3_unitize(dir_1, 0.001, &magnitude, dir_1);
        UF_VEC3_unitize(dir_2, 0.001, &magnitude, dir_2);

        if (type_ != UF_MODL_CIRCULAR_EDGE)
        {
            UF_VEC3_add(dir_1, dir_2, dir_3);
            UF_VEC3_negate(dir_3, dir_3);
            MyFun::getDirectionPos(dir_3, pt, 1, pt_);
            MyFun::CalPlaneLineIntersectPoint(dir_1, pt, dir_1, pt_, pt__);
            MyFun::ProjectPoint(pt__, pt, pt___);
        }
        else if (type_ == UF_MODL_CIRCULAR_EDGE)//只支持直线圆弧
        {
            UF_CURVE_arc_t arc_coords;
            UF_CURVE_ask_arc_data(edge, &arc_coords);
            UF_VEC3_sub(pt, arc_coords.arc_center, dir_3);
            MyFun::getDirectionPos(dir_3, pt, 1, pt___);
            MyFun::getDirectionPos(dir_3, pt, -1, pt__);
        }
        tag_t body_;
        UF_MODL_ask_face_body(face1, &body_);
        int contain_ = 0;
        UF_MODL_ask_point_containment(pt___, body_, &contain_);
        if (type_ == UF_MODL_CIRCULAR_EDGE&&contain_ != 2)
            UF_MODL_ask_point_containment(pt__, body_, &contain_);
        angle = MyFun::angleOfVectors_(dir_1, dir_2);

        if (contain_ == 2)
            angle = PI - angle;
        else
            angle += PI;
        return angle;
    }

    tag_t smoothedCurves(vector<tag_t>curves, int stepover, double dist)
    {
        int a = BYTE_4(curves.size());

        tag_p_t joinCurve = createJoinedCurves(&curves[0], a);
        if (a != 1)
        {
            delete joinCurve;
            return 0;
        }
        tag_t curve = joinCurve[0];
        UF_initialize();
        int count_ = ((int)get_spline_length(curve)) / stepover;
        UF_TERMINATE();
        tag_t pointSetFeat = createPointSetFeat(curve, count_);
        tag_t temp = createFitCureFeat(pointSetFeat);
        tag_t temp1 = offsetCurve(temp, dist);
        return temp1;
    }

    tag_t projectOperation(double origin_point[3], double plane_normal[3], vector<tag_t>& curveCollection)
    {
        if (curveCollection.size() == 0) return 0;
        uf_list_p_t curve_refs = NULL;
        UF_initialize();
        curve_refs = p_TagsCreateList(curveCollection.size(), &curveCollection[0]);

        tag_t plane_temp = NULL_TAG;
        UF_CALL(UF_MODL_create_plane(origin_point, plane_normal, &plane_temp));

        uf_list_p_t face_refs = new uf_list_s;
        face_refs->eid = plane_temp;
        face_refs->next = NULL;
        tag_t  proj_curve_feature = null_tag;
        // 		int a = 0;
        // 		double b = 0.0001;
        // 		UF_MODL_set_distance_tolerance(&b,&a);
        UF_CALL(UF_MODL_create_proj_curves(curve_refs, face_refs, 0, plane_normal, &proj_curve_feature));

        delete face_refs;
        deleteList(curve_refs);
        curve_refs = NULL;
        UF_CALL(UF_MODL_ask_proj_curves(proj_curve_feature, &curve_refs));
        UF_CALL(UF_MODL_delete_object_parms(curve_refs));
        UF_OBJ_delete_object(plane_temp);
        int num = 0;
        UF_MODL_ask_list_count(curve_refs, &num);
        tag_t temp = 0;
        curveCollection.clear();
        for (int i = 0; i < num; i++)
        {
            UF_MODL_ask_list_item(curve_refs, i, &temp);
            curveCollection.push_back(temp);
        }
        UF_CALL(UF_MODL_delete_list(&curve_refs));
        UF_TERMINATE();
        return plane_temp;
    }

    class  TagPointInfo
    {
    public:
        TagPointInfo() :id(0)
        {
            startPoint[0] = 0;
            startPoint[1] = 0;
            startPoint[2] = 0;
            endPoint[0] = 0;
            endPoint[1] = 0;
            endPoint[2] = 0;
        }
        ~TagPointInfo()
        {
        };
        TagPointInfo& operator=(const TagPointInfo & or )
        {
            if (this != &or )
            {
                id = or .id;
                startPoint[0] = or .startPoint[0];
                startPoint[1] = or .startPoint[1];
                startPoint[2] = or .startPoint[2];
                endPoint[0] = or .endPoint[0];
                endPoint[1] = or .endPoint[1];
                endPoint[2] = or .endPoint[2];
            }
            return *this;
        }
    public:
        tag_t id;
        double startPoint[3];
        double endPoint[3];

    };

    BOOL isNearly(double point1[3], double point2[3])
    {
        for (int i = 0; i < 3; i++)
        {
            if (fabs(point1[i] - point2[i])>2.0f)
            {
                return FALSE;
            }
        }
        return TRUE;
    }

    // tag_p_t orderCurvesCreateList(I const size_t numOfCurves,I const tag_p_t &tagCurves,O  uf_list_p_t & pt)
    // { 
    // 	double  tangent[3] = { 0 };
    // 	double  p_norm[3] = { 0 };
    // 	double  b_norm[3] = { 0 };
    // 	double  torsion = 0.0f;
    // 	double  rad_of_cur = 0.0f;
    // 	TagPointInfo temp ;
    // 	std::vector<TagPointInfo> vectorTemp;
    // 	for(int i = 0; i < numOfCurves;i++)
    // 	{
    // 		temp.id = tagCurves[i];
    // 		UF_MODL_ask_curve_props(temp.id,0,temp.startPoint,tangent,p_norm,b_norm,&torsion,&rad_of_cur);
    // 		UF_MODL_ask_curve_props(temp.id,1,temp.endPoint,tangent,p_norm,b_norm,&torsion,&rad_of_cur);
    // 		vectorTemp.push_back(temp);
    // 	}
    // 	uf_list_p_t p = new uf_list_s,q=NULL;
    // 	std::vector<TagPointInfo>::iterator iterTemp=vectorTemp.begin();
    // 	p->eid = iterTemp->id; p->next = NULL; temp = *iterTemp; TagPointInfo temp1 = *iterTemp;
    // 	vectorTemp.erase(iterTemp);
    // 	bool flag = false;
    // 	while (vectorTemp.size())
    // 	{
    // 		flag = false;
    // 		for(iterTemp = vectorTemp.begin(); iterTemp < vectorTemp.end(); iterTemp++)
    // 		{
    // 			if (isNearly(temp.endPoint,iterTemp->startPoint))
    // 			{
    // 				flag = true;
    // 				p->next = new struct uf_list_s;
    // 				p->next->eid = iterTemp->id;
    // 				p->next->next = NULL;
    // 				p = p->next;
    // 				temp = *iterTemp;
    // 				vectorTemp.erase(iterTemp);
    // 				iterTemp--;
    // 			}
    // 			else if(isNearly(temp.endPoint,iterTemp->endPoint))
    // 			{
    // 				flag = true;
    // 				p->next = new struct uf_list_s;
    // 				p->next->eid = iterTemp->id;
    // 				p->next->next = NULL;
    // 				p = p->next;
    // 				temp = *iterTemp;
    // 				vectorTemp.erase(iterTemp);
    // 				iterTemp--;
    // 			}
    // 			if( isNearly(temp1.startPoint,iterTemp->endPoint))
    // 			{
    // 				flag = true;
    // 				uf_list_p_t x = new uf_list_s;
    // 				x->eid = iterTemp->id;
    // 				if(q)
    // 					x->next = q;
    // 				else
    // 					x->next = NULL;
    // 				q = x;
    // 				temp1 = *iterTemp;
    // 				vectorTemp.erase(iterTemp);
    // 				iterTemp--;
    // 			}
    // 			else if(isNearly(temp1.startPoint,iterTemp->startPoint))
    // 			{
    // 				flag = true;
    // 				uf_list_p_t x = new uf_list_s;
    // 				x->eid = iterTemp->id;
    // 				if(q)
    // 					x->next = q;
    // 				else
    // 					x->next = NULL;
    // 				q = x;
    // 				temp1 = *iterTemp;
    // 				vectorTemp.erase(iterTemp);
    // 				iterTemp--;
    // 			}				
    // 		}			
    // 		if(flag == false)
    // 		{
    // 			deleteList(p);
    // 			deleteList(q);
    // 			return NULL;
    // 		}
    // 	}
    // 	pt = q;
    // 	size_t sss;
    // 	return p_listCreateTags(p,sss);
    // }

    tag_t smoothedAndFittingCurves(const tag_t I spline_1, double O distPoint[3])
    {
        if (spline_1 == 0)
        {
            return 0;
        }
        int degree = 3;
        int periodicity = 0;     int save_def_data = 0;
        tag_t spline_tag = NULL_TAG;
        UF_CURVE_pt_slope_crvatr_t point_data[2000];
        double param = 0;
        double point1[3] = { 0 };
        double tangent1[3] = { 0 };
        double p_norm1[3] = { 0 };
        double p_normS[3] = { 0 };
        double p_normE[3] = { 0 };
        double b_norm1[3] = { 0 };
        double torsion = 0;
        double rad_of_cur = 0;
        int num_points = 2000;
        int i = 0;
        double dist = 0;
        UF_MODL_ask_curve_props(spline_1, 0, p_normS, tangent1, p_norm1, b_norm1, &torsion, &rad_of_cur);
        UF_MODL_ask_curve_props(spline_1, 1, p_normE, tangent1, p_norm1, b_norm1, &torsion, &rad_of_cur);
        for (i; i < num_points; i++)
        {
            point_data[i].crvatr_type = UF_CURVE_CRVATR_NONE;   //填充  UF_CURVE_pt_slope_crvatr_s结构体
            point_data[i].slope_type = UF_CURVE_SLOPE_NONE;
            param = (i + 1.0f) / num_points; //show_window("",i,param);  
            UF_MODL_ask_curve_props(spline_1, param, point1, tangent1, p_norm1, b_norm1, &torsion, &rad_of_cur);
            param = distanceOfPointToLine(point1, p_normS, p_normE);
            if (dist < param)
            {
                dist = param;
                UF_VEC3_copy(point1, distPoint);
            }
            UF_VEC3_copy(point1, point_data[i].point);
        }

        if (0 == UF_CALL(UF_CURVE_create_spline_thru_pts(degree, periodicity, num_points, point_data, NULL, save_def_data, &spline_tag)))
        {
            return spline_tag;
        }
        return 0;
    }

    void  CalPlaneLineIntersectPoint(const double planeVector[3], const double planePoint[3], const double lineVector[3]
        , const double linePoint[3], double*returnResult)
    {

        //首先判断直线是否与平面平行  
        double vpt = lineVector[0] * planeVector[0] + lineVector[1] * planeVector[1] + lineVector[2] * planeVector[2];
        if (vpt == 0) return;

        double t = ((planePoint[0] - linePoint[0]) * planeVector[0] + (planePoint[1] - linePoint[1]) * planeVector[1]
            + (planePoint[2] - linePoint[2]) * planeVector[2]) / vpt;
        returnResult[0] = linePoint[0] + lineVector[0] * t;
        returnResult[1] = linePoint[1] + lineVector[1] * t;
        returnResult[2] = linePoint[2] + lineVector[2] * t;
    }

    double distOfPtToPlane(const double planeVector[3], const double planePoint[3], double pt[3])
    {
        double bufD[3] = { 0 };
        CalPlaneLineIntersectPoint(planeVector, planePoint, planeVector, pt, bufD);
        double bufDist = 0.0f;
        UF_VEC3_distance(pt, bufD, &bufDist);
        return bufDist;
    }

    double distanceOfPointToLine(const tag_t point, const tag_t line)
    {
        double pt3[3] = { 0 };
        UF_CURVE_line_s lineStructInfo;
        double vec_diff[3] = { 0 };
        double distance = -1;

        UF_CURVE_ask_point_data(point, pt3);
        UF_CURVE_ask_line_data(line, &lineStructInfo);
        UF_VEC3_sub(lineStructInfo.start_point, lineStructInfo.end_point, vec_diff);
        double  buf[3] = { 0 };
        CalPlaneLineIntersectPoint(vec_diff, pt3, vec_diff, lineStructInfo.start_point, buf);
        UF_VEC3_distance(buf, pt3, &distance);
        return distance;
    }

    double distanceOfPointToLine(const double pt3[3], const double lineStartPt[3], const double lineEndPt[3])
    {
        double vec_diff[3] = { 0 };
        double distance = -1;
        UF_VEC3_sub(lineStartPt, lineEndPt, vec_diff); double  buf[3] = { 0 };
        CalPlaneLineIntersectPoint(vec_diff, pt3, vec_diff, lineEndPt, buf);
        UF_VEC3_distance(buf, pt3, &distance);
        return distance;
    }

    int create_linked_body(tag_t OccSrc, tag_t OccTag, tag_t &linked_body)
    {
        UF_initialize();
        tag_t  linked_fea, tag_part_occ,
            src_part_occ, xform, proto_src, proto_tag, result_body;
        uf_list_p_t body_list;
        int	ret;

        linked_body = NULL_TAG;

        if ((OccSrc == NULL_TAG) || !UF_ASSEM_is_occurrence(OccSrc))
            return 0;
        if ((OccTag == NULL_TAG) || !UF_ASSEM_is_occurrence(OccTag))
            return 0;

        if ((proto_src = UF_ASSEM_ask_prototype_of_occ(OccSrc)) == NULL_TAG)
            return (-1);

        if ((proto_tag = UF_ASSEM_ask_prototype_of_occ(OccTag)) == NULL_TAG)
            return (-1);

        tag_part_occ = UF_ASSEM_ask_part_occurrence(OccTag);
        src_part_occ = UF_ASSEM_ask_part_occurrence(OccSrc);
        UF_CALL(UF_SO_create_xform_assy_ctxt(proto_tag, src_part_occ, tag_part_occ, &xform));


        if ((ret = UF_WAVE_create_linked_body(proto_src, xform,
            proto_tag, false, &linked_fea)) != 0)
            return ret;

        if ((ret = UF_MODL_ask_feat_body(linked_fea, &result_body)) != 0)
            return ret;

        if ((ret = UF_MODL_create_list(&body_list)) != 0)
            return ret;

        if ((ret = UF_MODL_put_list_item(body_list, result_body)) != 0)
            return ret;

        if ((ret = UF_MODL_delete_body_parms(body_list)) != 0)
            return ret;

        if ((UF_MODL_delete_list(&body_list)) != 0)
            return ret;
        UF_TERMINATE();
        linked_body = result_body;
        return 0;
    }

    int create_linked_body(tag_t src_body, tag_t &linked_body)
    {
        UF_initialize();
        if ((src_body == NULL_TAG) || !UF_ASSEM_is_occurrence(src_body))
        {
            UF_TERMINATE();
            linked_body = src_body;
            return  -1;
        }

        tag_t work_part, linked_fea, root_occ,
            src_part_occ, xform, proto_body, result_body;
        uf_list_p_t body_list;
        int	ret;

        linked_body = NULL_TAG;
        if ((proto_body = UF_ASSEM_ask_prototype_of_occ(src_body)) == NULL_TAG)
        {
            UF_TERMINATE();
            return (-1);
        }

        src_part_occ = UF_ASSEM_ask_part_occurrence(src_body);


        if ((work_part = UF_ASSEM_ask_work_part()) == NULL_TAG)
        {
            UF_TERMINATE();
            return (-1);
        }
        root_occ = UF_ASSEM_ask_root_part_occ(work_part);


        if ((ret = UF_SO_create_xform_assy_ctxt(work_part,
            src_part_occ, root_occ, &xform)) != 0)
        {
            UF_TERMINATE();
            return ret;
        }


        if ((ret = UF_WAVE_create_linked_body(proto_body, xform,
            work_part, false, &linked_fea)) != 0)
        {
            UF_TERMINATE();
            return ret;
        }


        if ((ret = UF_MODL_ask_feat_body(linked_fea, &result_body)) != 0)
        {
            UF_TERMINATE();
            return ret;
        }


        if ((ret = UF_MODL_create_list(&body_list)) != 0)//删除参数
        {
            UF_TERMINATE();
            return ret;
        }


        if ((ret = UF_MODL_put_list_item(body_list, result_body)) != 0)//删除参数
        {
            UF_TERMINATE();
            return ret;
        }


        if ((ret = UF_MODL_delete_body_parms(body_list)) != 0)//删除参数
        {
            UF_TERMINATE();
            return ret;
        }

        if ((UF_MODL_delete_list(&body_list)) != 0)//删除参数
        {
            UF_TERMINATE();
            return ret;
        }


        linked_body = result_body;
        return 0;
    }

    int create_linked_body(tag_t src_body, tag_t *linked_body)
    {
        UF_initialize();
        tag_t work_part, linked_fea, root_occ,
            src_part_occ, xform, proto_body, result_body;
        uf_list_p_t body_list;
        int	ret;

        *linked_body = NULL_TAG;

        if ((work_part = UF_ASSEM_ask_work_part()) == NULL_TAG)
            return (-1);

        root_occ = UF_ASSEM_ask_root_part_occ(work_part);

        if ((src_body == NULL_TAG) || !UF_ASSEM_is_occurrence(src_body))
            return 0;

        if ((proto_body = UF_ASSEM_ask_prototype_of_occ(src_body)) == NULL_TAG)
            return (-1);

        src_part_occ = UF_ASSEM_ask_part_occurrence(src_body);
        if ((ret = UF_SO_create_xform_assy_ctxt(work_part,
            src_part_occ, root_occ, &xform)) != 0)
            return ret;

        if ((ret = UF_WAVE_create_linked_body(proto_body, xform,
            work_part, false, &linked_fea)) != 0)
            return ret;

        if ((ret = UF_MODL_ask_feat_body(linked_fea, &result_body)) != 0)
            return ret;

        if ((ret = UF_MODL_create_list(&body_list)) != 0)
            return ret;

        if ((ret = UF_MODL_put_list_item(body_list, result_body)) != 0)
            return ret;

        if ((ret = UF_MODL_delete_body_parms(body_list)) != 0)
            return ret;

        if ((UF_MODL_delete_list(&body_list)) != 0)
            return ret;
        UF_TERMINATE();
        *linked_body = result_body;
        return 0;
    }

    void ProjectPoint(const double * pt1, const double * pt2, double * pt3)
    {
        double dist = 0;
        double dir[3] = { 0 };
        UF_VEC3_distance(pt1, pt2, &dist);
        UF_VEC3_sub(pt2, pt1, dir);
        getDirectionPos(dir, pt2, dist, pt3);
    }

    void ProjectPointToLine(const double * pt1, const double * L1, const double * L2, double * pt3)
    {
        double vec_diff[3] = { 0 };
        double buf[3] = { 0 };
        UF_VEC3_sub(L1, L2, vec_diff);
        CalPlaneLineIntersectPoint(vec_diff, pt1, vec_diff, L1, buf);
        ProjectPoint(pt1, buf, pt3);
    }

    int duplicate_body(tag_t target_body, tag_t *result_body)
    {
        tag_t	proto_body, obj_in_part;
        int		ret;
#ifdef _DEBUG
        char debug_msg[133];
#endif

        *result_body = NULL_TAG;

        if (UF_ASSEM_is_occurrence(target_body))//事件
        {
            proto_body = UF_ASSEM_ask_prototype_of_occ(target_body);
            if ((ret = UF_OBJ_ask_owning_part(proto_body, &obj_in_part)) != 0)
                return ret;

            if (obj_in_part == UF_ASSEM_ask_work_part())
            {
#ifdef _DEBUG
                sprintf_s(debug_msg, 133,
                    "body[%d] is occurrence, but it is in the work part!\n", target_body);
                log_write(debug_msg);
#endif
                uf6511(&proto_body, result_body);
            }
            else
            {
                if ((ret = create_linked_body(target_body, *result_body)) != 0)
                    return ret;

#ifdef _DEBUG
                sprintf_s(debug_msg, 133, "body[%d] is occurrence, create linked body!\n",
                    target_body, result_body);
                log_write(debug_msg);
#endif
            }
        }
        else//建模档
        {
#ifdef _DEBUG
            sprintf_s(debug_msg, 133, "body[%d] is prototype, just make a copy of it!\n", target_body);
            log_write(debug_msg);
#endif
            uf6511(&target_body, result_body);
        }

        return 0;
    }

    BOOL cylindricalFaceIsRaised(tag_t face_tag, tag_t body_tag)
    {
        int faceType = 0;
        UF_MODL_ask_face_type(face_tag, &faceType);
        if (faceType != UF_MODL_CYLINDRICAL_FACE) return -1;

        int  type_id = 0;
        double point[3] = { 0 };
        double point2[3] = { 0 };
        double dir[3] = { 0 };
        double box[6] = { 0 };
        double  radius = 0.0;
        double  rad_data = 0.0;
        int  norm_dir = 0;
        UF_CALL(UF_MODL_ask_face_data(face_tag, &type_id, point, dir, box, &radius, &rad_data, &norm_dir));
        uf_list_p_t pt = NULL;
        UF_MODL_create_list(&pt);
        UF_MODL_ask_face_edges(face_tag, &pt);
        int num = 0;
        int type_n = 0;
        UF_MODL_ask_list_count(pt, &num);
        tag_t temp = 0;
        for (int i = 0; i < num; i++)
        {
            UF_MODL_ask_list_item(pt, i, &temp);
            UF_MODL_ask_edge_type(temp, &type_n);
            if (UF_MODL_LINEAR_EDGE == type_n)
                break;
        }UF_MODL_delete_list(&pt);
        UF_CURVE_line_t lt = { 0 };
        UF_CURVE_ask_line_data(temp, &lt);
        UF_VEC3_midpt(lt.end_point, lt.start_point, point2);
        UF_VEC3_sub(point2, point, dir);
        getDirectionPos(dir, point2, 0.01, point);

        int flag_id = 0;
        UF_CALL(UF_MODL_ask_point_containment(point, body_tag, &flag_id));
        if (flag_id == 1)
        {
            return FALSE;
        }
        return TRUE;
    }

    double maxRadiusOfTwoFaces(tag_t face1, tag_t face2)
    {
        int faceType = 0;
        UF_MODL_ask_face_type(face1, &faceType);
        if (faceType != UF_MODL_PLANAR_FACE) return -1;
        UF_MODL_ask_face_type(face2, &faceType);
        if (faceType != UF_MODL_PLANAR_FACE) return -1;

        tag_t body_id;
        UF_MODL_ask_face_body(face1, &body_id);

        int  type_id = 0;
        double point1[3] = { 0 };
        double point2[3] = { 0 };
        double dir1[3] = { 0 };
        double dir2[3] = { 0 };
        double box1[6] = { 0 };
        double box2[6] = { 0 };
        double  radius = 0.0;
        double  rad_data = 0.0;
        int  norm_dir = 0;
        UF_MODL_ask_face_data(face1, &type_id, point1, dir1, box1, &radius, &rad_data, &norm_dir);
        UF_MODL_ask_face_data(face2, &type_id, point2, dir2, box2, &radius, &rad_data, &norm_dir);
        double dir1_[3] = { 0 };
        double dir2_[3] = { 0 }; double xxx;
        UF_VEC3_unitize(dir1, 0.000001, &xxx, dir1_);
        UF_VEC3_unitize(dir2, 0.000001, &xxx, dir2_);

        UF_VEC3_midpt(box1, box1 + 3, point1);
        UF_VEC3_midpt(box2, box2 + 3, point2);
        double pointTest[3] = { 0 };
        int flag_id = 0;

        getDirectionPos(dir1_, point1, 0.5, pointTest);
        UF_MODL_ask_point_containment(pointTest, body_id, &flag_id);
        if (flag_id == 1)
        {
            UF_VEC3_negate(dir1_, dir1_);
        }
        getDirectionPos(dir2_, point2, 0.5, pointTest);
        UF_MODL_ask_point_containment(pointTest, body_id, &flag_id);
        if (flag_id == 1)
        {
            UF_VEC3_negate(dir2_, dir2_);
        }

        double orientation[3] = { 0 };
        UF_VEC3_cross(dir1_, dir2_, orientation);
        UF_VEC3_unitize(orientation, 0.000001, &xxx, orientation);

        double angle = 0.0f;
        UF_VEC3_angle_between(dir1_, dir2_, orientation, &angle);

        double tanValue = tan(angle / 2.0f);

        double dist = 999999.0f;
        int numOfEdges = 0;
        uf_list_p_t  edge_list = NULL;
        UF_MODL_create_list(&edge_list);
        UF_MODL_ask_face_edges(face1, &edge_list);
        UF_MODL_ask_list_count(edge_list, &numOfEdges);

        double edgeDirection[3] = { 0 };
        tag_t edge_id = 0;
        int edge_type = 0;
        UF_CURVE_line_t points = { 0 };
        double zdirestion[3] = { 0,0,1 };

        int  isPerpendicular = 0;
        for (int i = 0; i < numOfEdges; i++)
        {
            UF_MODL_ask_list_item(edge_list, i, &edge_id);
            UF_MODL_ask_edge_type(edge_id, &edge_type);
            if (edge_type != UF_MODL_LINEAR_EDGE)
            {
                continue;
            }
            UF_CURVE_ask_line_data(edge_id, &points);
            UF_VEC3_sub(points.start_point, points.end_point, edgeDirection);
            UF_VEC3_is_perpendicular(edgeDirection, zdirestion, 0.001, &isPerpendicular);
            if (isPerpendicular == 1)
            {
                double dists = 0.0;
                UF_VEC3_distance(points.start_point, points.end_point, &dists);
                if (dist > dists)
                {
                    dist = dists;
                }
            }
        }
        UF_MODL_delete_list(&edge_list);


        UF_MODL_create_list(&edge_list);
        UF_MODL_ask_face_edges(face2, &edge_list);
        UF_MODL_ask_list_count(edge_list, &numOfEdges);
        for (int i = 0; i < numOfEdges; i++)
        {
            UF_MODL_ask_list_item(edge_list, i, &edge_id);
            UF_MODL_ask_edge_type(edge_id, &edge_type);
            if (edge_type != UF_MODL_LINEAR_EDGE)
            {
                continue;
            }
            UF_CURVE_ask_line_data(edge_id, &points);
            UF_VEC3_sub(points.start_point, points.end_point, edgeDirection);
            UF_VEC3_is_perpendicular(edgeDirection, zdirestion, 0.001, &isPerpendicular);
            if (isPerpendicular == 1)
            {
                double dists = 0.0;
                UF_VEC3_distance(points.start_point, points.end_point, &dists);
                if (dist > dists)
                {
                    dist = dists;
                }
            }
        }
        UF_MODL_delete_list(&edge_list);
        return dist*tanValue;
    }

    double askFaceData(tag_t face_tag, double * direction, double *cenPoint)
    {
        // 		int faceType=0;
        // 		UF_CALL(UF_MODL_ask_face_type(face_tag,&faceType));
        // 		if(faceType!=UF_MODL_CYLINDRICAL_FACE) return 0;

        int  type_id = 0;
        double Point[3] = { 0 };
        if (cenPoint == NULL)
        {
            cenPoint = Point;
        }

        double dir[3] = { 0 };
        if (direction == NULL)
        {
            direction = dir;
        }
        double box[6] = { 0 };
        double  radius = 0.0;
        double  rad_data = 0.0;
        int  norm_dir = 0;
        UF_CALL(UF_MODL_ask_face_data(face_tag, &type_id, cenPoint, direction, box, &radius, &rad_data, &norm_dir));
        return radius;
    }

    int relateOfLineAndDirection(tag_t line_, double direction3[3])
    {
        int faceType = 0;
        UF_CALL(UF_MODL_ask_edge_type(line_, &faceType));
        if (faceType != UF_MODL_LINEAR_EDGE) return -1;

        UF_CURVE_line_t lt = { 0 };
        UF_CALL(UF_CURVE_ask_line_data(line_, &lt));
        double directionLine[3] = { 0 };;
        UF_VEC3_sub(lt.start_point, lt.end_point, directionLine);
        int is_ = 0;
        UF_VEC3_is_perpendicular(directionLine, direction3, 0.001, &is_);
        if (is_)
        {
            return is_;
        }
        UF_VEC3_is_parallel(directionLine, direction3, 0.001, &is_);
        if (is_)
        {
            return is_ + 1;
        }
        return 0;
    }

    int relateOfLines(tag_t line_1, tag_t line_2)
    {
        if (line_1 == line_2)
            return 4;

        double direction3[3] = { 0 };
        int faceType = 0;
        UF_MODL_ask_edge_type(line_1, &faceType);
        if (faceType != UF_MODL_LINEAR_EDGE) return -1;
        UF_MODL_ask_edge_type(line_2, &faceType);
        if (faceType != UF_MODL_LINEAR_EDGE) return -1;

        UF_CURVE_line_t lt1 = { 0 };
        UF_CURVE_ask_line_data(line_1, &lt1);
        double directionLine[3] = { 0 };;
        UF_VEC3_sub(lt1.start_point, lt1.end_point, directionLine);
        UF_CURVE_line_t lt2 = { 0 };
        UF_CURVE_ask_line_data(line_2, &lt2);
        UF_VEC3_sub(lt2.start_point, lt2.end_point, direction3);


        int is_ = 0;
        UF_VEC3_is_perpendicular(directionLine, direction3, 0.001, &is_);
        if (is_)
        {
            return is_;
        }
        UF_VEC3_is_parallel(directionLine, direction3, 0.001, &is_);
        if (is_)
        {
            if (lt1.start_point == lt2.start_point || lt1.start_point == lt2.end_point ||
                lt1.end_point == lt2.start_point || lt1.end_point == lt2.end_point)
                return 3;
            return is_ + 1;
        }
        return 0;
    }

    tag_p_t askLineOfFaceFromDirection(tag_t I face_, double I direction3[3], int O * num)
    {
        uf_list_p_t pt = NULL;
        UF_MODL_create_list(&pt);
        UF_CALL(UF_MODL_ask_face_edges(face_, &pt));
        int count_ = 0;
        UF_CALL(UF_MODL_ask_list_count(pt, &count_));
        tag_t objectedge = 0;
        tag_t *p = new tag_t[3]; *num = 0;
        memset(p, 0, sizeof(tag_t) * 3);
        for (int i = 0; i < count_; i++)
        {
            UF_MODL_ask_list_item(pt, i, &objectedge);
            if (relateOfLineAndDirection(objectedge, direction3) == 2)
            {
                p[*num] = objectedge;
                *num += 1;
                if (*num == 2) break;
            }
        }
        UF_MODL_delete_list(&pt);
        return p;
    }

    vector<TaggedObject*> getJoinedFaceInArea(TaggedObject*tagObject, double * direction3)
    {
        int num = 0; int index_ = 0;
        tag_p_t p_TempTag = askLineOfFaceFromDirection(tagObject->GetTag(), direction3, &num);
        uf_list_p_t lpt = NULL;

        vector<TaggedObject*> tempObjects; tempObjects.clear();
        while (index_ < num)
        {
            UF_MODL_create_list(&lpt);
            int count_ = 0; tag_t Temp_Object = NULL;
            UF_MODL_ask_edge_faces(p_TempTag[index_], &lpt);
            UF_MODL_ask_list_count(lpt, &count_);
            for (int i = 0; i < count_; i++)
            {
                UF_MODL_ask_list_item(lpt, i, &Temp_Object);
                if (tagObject->GetTag() == Temp_Object) continue;
                tempObjects.push_back(NXObjectManager::Get(Temp_Object));
                if (tempObjects.size() == 2) break;
            }
            if (tempObjects.size() == 2) break;
            UF_MODL_delete_list(&lpt);
            lpt = NULL;
            index_++;
        }
        delete p_TempTag;
        return tempObjects;
    }

    void  pointPorjectToLine(const double * pt3, const tag_t line, double* buf)
    {
        UF_CURVE_line_s lineStructInfo;
        double vec_diff[3] = { 0 };

        UF_CURVE_ask_line_data(line, &lineStructInfo);
        UF_VEC3_sub(lineStructInfo.start_point, lineStructInfo.end_point, vec_diff);
        CalPlaneLineIntersectPoint(vec_diff, pt3, vec_diff, lineStructInfo.start_point, buf);
    }
    void  pointPorjectToLine(const double * pt3, const double *start_point, const double* end_point, double* buf)
    {
        double vec_diff[3] = { 0 };
        UF_VEC3_sub(start_point, end_point, vec_diff);
        CalPlaneLineIntersectPoint(vec_diff, pt3, vec_diff, start_point, buf);
    }
    // 	void pointPorjectToLine(const double* point3,const tag_t line,double *buf)
    // 	{
    // 		UF_CURVE_line_s lineStructInfo;
    // 		double vec_diff[3] = { 0 };
    // 		double distance = -1;
    // 		UF_CURVE_ask_line_data(line,&lineStructInfo);
    // 		UF_VEC3_sub(lineStructInfo.start_point,lineStructInfo.end_point,vec_diff);
    // 
    // 		CalPlaneLineIntersectPoint(vec_diff,point3,vec_diff,lineStructInfo.start_point, buf);
    // 	}

    int relationOfPointAndLine(const double* point3, const double* lineStartPoint, const double* lineEndPoint)
    {
        if (is_Equal(point3, lineStartPoint, 0.001) || is_Equal(point3, lineEndPoint, 0.001))
            return 0;
        double templength = 0.0;
        double tempLength1 = 0.0;
        double tempLength2 = 0.0;
        double direction_1[3] = { 0 };
        double direction_2[3] = { 0 };
        UF_VEC3_sub(point3, lineStartPoint, direction_1);
        UF_VEC3_sub(point3, lineEndPoint, direction_2);
        int is_id = 0;
        UF_VEC3_is_parallel(direction_1, direction_2, 0.01f, &is_id);
        if (is_id == 0)
            return 2;
        UF_VEC3_distance(lineStartPoint, lineEndPoint, &templength);
        UF_VEC3_distance(point3, lineEndPoint, &tempLength1);
        UF_VEC3_distance(lineStartPoint, point3, &tempLength2);
        if ((tempLength2 + tempLength1) <= templength + 0.0001f) return 0;
        else return 1;
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

    double angleOfVectors_(const double *dir1, const double *dir2)
    {
        double dir3[3] = { 0 };
        double angle = .0;
        UF_VEC3_cross(dir1, dir2, dir3);
        UF_VEC3_angle_between(dir1, dir2, dir3, &angle);
        return angle;
    }

    void VectorProjectToPlane(double *dir, double *planeDir, double * outdir)
    {
        double dir_[3] = { 0 };
        double angle = .0;
        UF_VEC3_unitize(dir, .000001, &angle, dir);
        UF_VEC3_unitize(planeDir, .000001, &angle, planeDir);
        UF_VEC3_cross(dir, planeDir, dir_);
        UF_VEC3_unitize(dir_, .000001, &angle, dir_);
        UF_VEC3_cross(dir_, planeDir, outdir);
        UF_VEC3_unitize(outdir, .000001, &angle, outdir);
        angle = angleOfVectors(dir, outdir);
        if (angle > PI / 2)
        {
            UF_VEC3_negate(outdir, outdir);
        }
    }

    void getPlaneNormal(const tag_t & faceTag, double faceNor[3])
    {
        tag_t body_tag = 0;
        UF_MODL_ask_face_body(faceTag, &body_tag);
        int  type_id = 0;
        double point[3] = { 0 };
        double dir[3] = { 0 };
        double box[6] = { 0 };
        double  radius = 0.0;
        double  rad_data = 0.0;
        int  norm_dir = 0;
        UF_MODL_ask_face_data(faceTag, &type_id, point, dir, box, &radius, &rad_data, &norm_dir);
        getDirectionPos(dir, point, 0.4, point);
        int flag_id;
        UF_MODL_ask_point_containment(point, body_tag, &flag_id);
        if (flag_id == 2)
        {
            faceNor[0] = dir[0];
            faceNor[1] = dir[1];
            faceNor[2] = dir[2];
        }
        else
        {
            faceNor[0] = -dir[0];
            faceNor[1] = -dir[1];
            faceNor[2] = -dir[2];
        }
    }

    bool twoBodyPlaneIsRaised(tag_t edge_)
    {
        tag_t body_ = 0;
        UF_MODL_ask_edge_body(edge_, &body_);

        UF_CURVE_line_t lt;
        UF_CURVE_ask_line_data(edge_, &lt);
        double a[9] = { 0 };
        double Pt3[3] = { 0 };
        UF_VEC3_midpt(lt.start_point, lt.end_point, Pt3);
        UF_VEC3_sub(lt.start_point, lt.end_point, a);
        char bufH[64] = { 0 };
        char bufD[64] = { 0 };
        sprintf(bufD, "%f", .5);
        sprintf(bufH, "%f", .05);
        tag_t ctag = 0;
        UF_MODL_create_cylinder(UF_NULLSIGN, 0, Pt3, bufH, bufD, a, &ctag);
        UF_MODL_ask_feat_body(ctag, &ctag);
        DeleteParms(1, &ctag);
        uf_list_p_t pt = NULL;
        UF_MODL_create_list(&pt);
        UF_MODL_ask_body_edges(ctag, &pt);
        int num = 0; tag_t edge_t = 0; int ss = 0;
        UF_MODL_ask_list_count(pt, &num);
        for (int i = 0; i < num; i++)
        {
            UF_MODL_ask_list_item(pt, i, &edge_t);
            UF_MODL_ask_edge_type(edge_t, &ss);
            if (UF_MODL_CIRCULAR_EDGE == ss)
                break;
        }
        UF_MODL_delete_list(&pt);


        double flag_interPtNum = 0.0f;
        int numOfPt = 0;
        double * p = NULL;
        UF_MODL_ask_curve_points(edge_t, 0, PI / 50.0f, 0, &numOfPt, &p);
        int status_id = 0;
        for (int i = 0; i < 100; i++)
        {
            UF_MODL_ask_point_containment((p + i * 3), body_, &status_id);
            if (status_id == 1)
                flag_interPtNum += 1.0f;
            else if (status_id == 3)
                flag_interPtNum += 0.5f;
        }
        UF_free(p);
        UF_OBJ_delete_object(ctag);
        if (flag_interPtNum < 50.1f)
            return true; //凸和平
        else
            return false;
    }

    int acuteAngleOfPt(double * oriPos, double * dir_, double radius_, double angle_, double * retPos)
    {
        double dist = tan(PI / 2.0 - angle_ / 2.0)*radius_;
        getDirectionPos(dir_, oriPos, dist, retPos);
        return 0;
    }

    double radiansOfFaces(double dir_1[3], double dir_2[3])
    {
        double dir_3[3] = { 0 };
        double angle = 0;
        UF_VEC3_cross(dir_1, dir_2, dir_3);
        UF_VEC3_angle_between(dir_1, dir_2, dir_3, &angle);
        return PI - angle;
    }

    tag_t getEdgeFromFaces(tag_t face1, tag_t face2)
    {
        uf_list_p_t edge_list = NULL;
        uf_list_p_t edge_list_ = NULL;
        int countn = 0;
        int countn_ = 0;
        tag_t tempEdge1 = 0;
        tag_t tempEdge2 = 0;
        UF_MODL_create_list(&edge_list);
        UF_MODL_create_list(&edge_list_);
        UF_MODL_ask_face_edges(face1, &edge_list);
        UF_MODL_ask_face_edges(face2, &edge_list_);
        UF_MODL_ask_list_count(edge_list, &countn);
        UF_MODL_ask_list_count(edge_list_, &countn_);
        bool flag = false;
        for (int i = 0; i < countn; i++)
        {
            for (int j = 0; j < countn_; j++)
            {
                UF_MODL_ask_list_item(edge_list, i, &tempEdge1);
                UF_MODL_ask_list_item(edge_list_, j, &tempEdge2);
                if (tempEdge1 != tempEdge2) continue;
                else
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
                break;
        }
        UF_MODL_delete_list(&edge_list_); edge_list_ = NULL;
        UF_MODL_delete_list(&edge_list); edge_list = NULL;
        return tempEdge1;
    }



    bool vectorTagUnique(std::vector<tag_t>&logTags, tag_t &tempT)
    {
        std::vector<tag_t>::iterator tempIter;
        logTags.push_back(tempT);
        sort(logTags.begin(), logTags.end());
        tempIter = unique(logTags.begin(), logTags.end());
        if (tempIter == logTags.end())
            return true;
        else
        {
            logTags.erase(tempIter, logTags.end());
            return false;
        }
    }

    void DeleteParms(int cnt, const tag_t* tags)
    {
        uf_list_p_t list = NULL;
        UF_INITIALIZE();
        UF_MODL_create_list(&list);
        for (int i = 0; i != cnt; ++i)
        {
            UF_MODL_put_list_item(list, tags[i]);
        }
        UF_CALL(UF_MODL_delete_object_parms(list));
        UF_CALL(UF_MODL_delete_list(&list));
    }


    void intersectPtOfLines(const double * p1, const double * p2, const double * p3, const double * p4, double * rtPt)
    {
        double plr1_x = p2[0] - p1[0];
        double plr1_y = p2[1] - p1[1];
        double plr1_z = p2[2] - p1[2];
        double plr2_x = p4[0] - p3[0];
        double plr2_y = p4[1] - p3[1];
        double plr2_z = p4[2] - p3[2];
        double t = 1.0f;
        if (((plr1_x != 0) && (plr2_x == 0)) || ((plr1_x == 0) && (plr2_x != 0)))
        {
            if (plr2_x == 0)
            {
                t = (p3[0] - p1[0]) / plr1_x;
                rtPt[0] = (p1[0] + t * plr1_x);
                rtPt[1] = (p1[1] + t * plr1_y);
                rtPt[2] = (p1[2] + t * plr1_z);
            }
            else
            {
                t = (p1[0] - p3[0]) / plr2_x;
                rtPt[0] = (p3[0] + t * plr2_x);
                rtPt[1] = (p3[1] + t * plr2_y);
                rtPt[2] = (p3[2] + t * plr2_z);
            }
        }
        else if (((plr1_y != 0) && (plr2_y == 0)) || ((plr1_y == 0) && (plr2_y != 0)))
        {
            if (plr2_y == 0)
            {
                t = (p3[1] - p1[1]) / plr1_y;
                rtPt[0] = (p1[0] + t * plr1_x);
                rtPt[1] = (p1[1] + t * plr1_y);
                rtPt[2] = (p1[2] + t * plr1_z);
            }
            else
            {
                t = (p1[1] - p3[1]) / plr2_y;
                rtPt[0] = (p3[0] + t * plr2_x);
                rtPt[1] = (p3[1] + t * plr2_y);
                rtPt[2] = (p3[2] + t * plr2_z);
            }
        }
        else if (((plr1_z != 0) && (plr2_z == 0)) || ((plr1_z == 0) && (plr2_z != 0)))
        {
            if (plr2_z == 0)
            {
                t = (p3[2] - p1[2]) / plr1_z;
                rtPt[0] = (p1[0] + t * plr1_x);
                rtPt[1] = (p1[1] + t * plr1_y);
                rtPt[2] = (p1[2] + t * plr1_z);
            }
            else
            {
                t = (p1[2] - p3[2]) / plr2_z;
                rtPt[0] = (p3[0] + t * plr2_x);
                rtPt[1] = (p3[1] + t * plr2_y);
                rtPt[2] = (p3[2] + t * plr2_z);
            }
        }
        else
        {
            if (((plr1_x != 0) && (plr2_x != 0)) && ((plr1_y != 0) && (plr2_y != 0)))
            {
                double fz = (p3[0] * plr2_y - p3[1] * plr2_x - plr2_y * p1[0] + plr2_x * p1[1]);
                double fm = (plr1_x * plr2_y - plr1_y * plr2_x);
                t = fz / fm;
                rtPt[0] = (p1[0] + t * plr1_x);
                rtPt[1] = (p1[1] + t * plr1_y);
                rtPt[2] = (p1[2] + t * plr1_z);
            }
            else if (((plr1_x != 0) && (plr2_x != 0)) && ((plr1_z != 0) && (plr2_z != 0)))
            {
                double fz = (p3[0] * plr2_z - p3[2] * plr2_x - plr2_z * p1[0] + plr2_x * p1[2]);
                double fm = (plr1_x * plr2_z - plr1_z * plr2_x);
                t = fz / fm;
                rtPt[0] = (p1[0] + t * plr1_x);
                rtPt[1] = (p1[1] + t * plr1_y);
                rtPt[2] = (p1[2] + t * plr1_z);
            }
            else if (((plr1_y != 0) && (plr2_y != 0)) && ((plr1_z != 0) && (plr2_z != 0)))
            {
                double fz = (p3[1] * plr2_z - p3[2] * plr2_y - plr2_z * p1[1] + plr2_y * p1[2]);
                double fm = (plr1_y * plr2_z - plr1_z * plr2_y);
                t = fz / fm;
                rtPt[0] = (p1[0] + t * plr1_x);
                rtPt[1] = (p1[1] + t * plr1_y);
                rtPt[2] = (p1[2] + t * plr1_z);
            }
            else
            {
                rtPt[0] = 0;
                rtPt[1] = 0;
                rtPt[2] = 0;
            }
        }
    }


    bool is_PtsEqual(const tempS& ts1, const tempS& ts2)
    {
        int n = 0;
        UF_VEC3_is_equal(ts1.pt1, ts2.pt1, 0.001, &n);
        if (n == 1)
            return true;
        UF_VEC3_is_equal(ts1.pt1, ts2.pt2, 0.001, &n);
        if (n == 1)
            return true;
        UF_VEC3_is_equal(ts1.pt2, ts2.pt1, 0.001, &n);
        if (n == 1)
            return true;
        UF_VEC3_is_equal(ts1.pt2, ts2.pt2, 0.001, &n);
        if (n == 1)
            return true;
        return false;
    }


    int orderCurvesList(const uf_list_p_t pt, const int nOfE, tag_t * tempEdges1, int & tNum1, tag_t *tempEdges2, int &tNum2)
    {
        tNum1 = 0;
        tNum2 = 0;
        std::vector<tempS> tempCurves;
        tempS tempss;
        {
            int num = 0;
            int n = 0;
            UF_MODL_ask_list_count(pt, &num);
            tag_t tempC = 0;
            uf_list_p_t ps = NULL;

            double tangent1[3] = { 0 };
            double p_norm1[3] = { 0 };
            double b_norm1[3] = { 0 };
            double torsion = 0;
            double rad_of_cur = 0;
            for (int i = 0; i < num; i++)//取出 有价值的片体的线
            {
                UF_MODL_ask_list_item(pt, i, &tempC);
                UF_MODL_create_list(&ps);
                UF_MODL_ask_edge_faces(tempC, &ps);
                UF_MODL_ask_list_count(ps, &n);
                UF_MODL_delete_list(&ps);
                if (n == 1)
                {
                    tempss.tc = tempC;
                    UF_MODL_ask_curve_props(tempC, 0, tempss.pt1, tangent1, p_norm1, b_norm1, &torsion, &rad_of_cur);
                    UF_MODL_ask_curve_props(tempC, 1, tempss.pt2, tangent1, p_norm1, b_norm1, &torsion, &rad_of_cur);
                    UF_VEC3_is_equal(tempss.pt1, tempss.pt2, 0.001, &n);
                    if (n == 1 && tNum1 == 0)
                    {
                        tempEdges1[0] = tempC;
                        tNum1 = 1;
                        continue;
                    }
                    else if (n == 1 && tNum2 == 0)
                    {
                        tempEdges2[0] = tempC;
                        tNum2 = 1;
                        continue;
                    }
                    else if (n != 1)
                        tempCurves.push_back(tempss);
                }
            }
            if (tempCurves.size() == 0 && tNum1 != 0)
                return 0;
        }

        if (tempCurves.size() > 1)
        {
            std::vector<tempS>::iterator iter;
            tag_t *temp = NULL;
            int *p_n = 0;

            if (tNum1 == 0)
            {
                temp = tempEdges1;
                p_n = &tNum1;
            }
            else if (tNum2 == 0)
            {
                temp = tempEdges2;
                p_n = &tNum2;
            }
            else
                return -1;

            tempss = tempCurves.back();
            tempCurves.pop_back();
            temp[0] = tempss.tc; *p_n = 1;

            for (iter = tempCurves.begin(); iter != tempCurves.end();)
            {
                if (is_PtsEqual(tempss, *iter))
                {
                    tempss = *iter;
                    temp[*p_n] = tempss.tc;
                    tempCurves.erase(iter);
                    if (tempCurves.size() > 0)
                        iter = tempCurves.begin();
                    (*p_n)++;
                    if ((*p_n) > nOfE)
                        return -1;
                }
                else
                    ++iter;
            }

            if (tempCurves.size() > 1)
            {
                if (tNum2 == 0)
                {
                    temp = tempEdges2;
                    p_n = &tNum2;
                }
                else
                    return -1;

                tempss = tempCurves.back();
                tempCurves.pop_back();
                temp[0] = tempss.tc; *p_n = 1;
                for (iter = tempCurves.begin(); iter != tempCurves.end();)
                {

                    if (is_PtsEqual(tempss, *iter))
                    {
                        tempss = *iter;
                        temp[*p_n] = tempss.tc;
                        tempCurves.erase(iter);
                        if (tempCurves.size() > 0)
                            iter = tempCurves.begin();
                        (*p_n)++;
                        if ((*p_n) > nOfE)
                            return -1;
                    }
                    else
                        ++iter;
                }
                if (tempCurves.size() > 0)
                    return -1;
                else
                    return 0;
            }
            else if (tempCurves.size() == 1)
                return -1;
            else
                return 0;


        }
        else if (tempCurves.size() == 1)
            return -1;
        else
            return 0;
    }

    int thicknessIsSafe(tag_t body, tag_t ptT, const double *dir_, double depth, double deltaSafe/*=0.0*/)
    {
        UF_initialize();     
        tag_t arc;
        tag_t  matrix_id;
        int aa;
        double pt3[3] = { 0 };
        double xx = (deltaSafe + depth) / 5;
        UF_CURVE_ask_point_data(ptT, pt3);
        double org[3] = {0};
        double mtx[9];
        UF_MTX3_initialize_z(dir_,mtx);
        UF_CSYS_create_matrix(mtx,&matrix_id);
        UF_CURVE_arc_t arc_coords;
        arc_coords.matrix_tag = matrix_id;
        arc_coords.start_angle = 0;
        arc_coords.end_angle = 2*PI;
        arc_coords.radius = 8;
        UF_MTX3_vec_multiply(pt3,mtx, arc_coords.arc_center);
        UF_CALL(UF_CURVE_create_arc(&arc_coords, &arc));
        
        double pt_[3];
        for (int j = 0; j < 5; j++)
        {
            moveObj(arc,dir_,xx,true);
            for (double  i = 0; i < 16; i++)
            {
                getCurvePt(arc, i/16.0, pt_);
                UF_MODL_ask_point_containment(pt_, body, &aa);
//                 tag_t temp;
//                 UF_CURVE_create_point(pt_,&temp);
                if (aa == 2)
                {
                    DELETE_OBJ(arc);
                    return -1;
                }
            }
            
        }
        DELETE_OBJ(arc);
        return 0;
    }

    int setColorOfBodyFaces(tag_t body, int color)
    {
        uf_list_p_t facePt = NULL;
        int num = 0;
        tag_t temp = 0;
        UF_MODL_create_list(&facePt);
        UF_MODL_ask_body_faces(body, &facePt);
        UF_MODL_ask_list_count(facePt, &num);
        for (int i = 0; i < num; i++)
        {
            UF_MODL_ask_list_item(facePt, i, &temp);
            UF_OBJ_set_color(temp, color);
        }
        UF_MODL_delete_list(&facePt);
        return 0;
    }

    tag_t getTogetherEdge(tag_t f1, tag_t f2)
    {
        int num1 = 0;
        int num2 = 0;
        tag_t a1 = 0;
        tag_t a2 = 0;
        uf_list_p_t  edge_list1 = NULL;
        uf_list_p_t  edge_list2 = NULL;
        UF_MODL_ask_face_edges(f1, &edge_list1);
        UF_MODL_ask_face_edges(f2, &edge_list2);
        UF_MODL_ask_list_count(edge_list1, &num1);
        UF_MODL_ask_list_count(edge_list2, &num2);
        bool flag = false;
        for (int i = 0; i < num1; i++)
        {
            UF_MODL_ask_list_item(edge_list1, i, &a1);
            for (int j = 0; j < num2; j++)
            {
                UF_MODL_ask_list_item(edge_list2, j, &a2);
                if (a1 == a2)
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
                break;
        }
        UF_MODL_delete_list(&edge_list1);
        UF_MODL_delete_list(&edge_list2);
        if (flag)
            return a2;
        else
            return 0;
    }


    double*  getCurvePt(const tag_t curve, double param, double * pt)
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

    tag_t moveObj(const tag_t body, const  double *dir, const  double dist, bool is_Move)
    {
        double deltaDist[3] = { 0 };
        double matrix[12] = { 0 };
        MyFun::getDirectionPos(dir, deltaDist, dist, deltaDist);
        UF_initialize();
        uf5943(deltaDist, matrix);
        int move_or_copy;
        if (is_Move)
            move_or_copy = 1;
        else
            move_or_copy = 2;
        int n_objects = 1;
        int dest_layer = 0;
        int trace_curves = 2;
        int status = 0;
        tag_t copies = 0;
        uf5947(matrix, &body, &n_objects, &move_or_copy, &dest_layer, &trace_curves, &copies, NULL, &status);
        return copies;
    }

    bool sheetMoveInfo(tag_t obj_tag, char timeName[128], double centerPt[3], double dir[3], double &dist) //name : TIME000000X.....Y.....Z..... false 为第一次命名或者位置未变 .
    {
        //程序启动时调用一次
        UF_initialize();
        CSolidObj  obj(obj_tag);
        UF_VEC3_midpt(obj.m_topCenter, obj.m_bottomCenter, centerPt);
        double currPt[3];
        memset(timeName, 0, 128);
        if (strncmp(obj.GetName(), "TIME", 4) == 0)
        {
            sscanf(obj.GetName(), "%[^X]X%lfY%lfZ%lf", timeName, &currPt[0], &currPt[1], &currPt[2]);

            if (is_Equal(centerPt, currPt, 0.001))
            {
                memset(dir, 0, sizeof(double) * 3);
                strcpy(timeName, obj.GetName());
                dist = 0;
                UF_TERMINATE();
                return false;
            }
            else
            {

                UF_VEC3_sub(centerPt, currPt, dir);
                UF_VEC3_distance(centerPt, currPt, &dist);
                //sscanf(obj.GetName(),"%10s",timeName);
                char buf[64] = { 0 };
                sprintf(buf, "X%4.5lfY%4.5lfZ%4.5lf", centerPt[0], centerPt[1], centerPt[2]);
                strcat(timeName, buf);

                obj.SetName(timeName);
                UF_TERMINATE();
                return true;
            }
        }
        else
        {
            SYSTEMTIME nowTime;
            ::GetLocalTime(&nowTime);
            sprintf(timeName, "TIME%02d%02d%02dX%04.5lfY%04.5lfZ%04.5lf", nowTime.wHour, nowTime.wMinute, nowTime.wSecond,
                centerPt[0], centerPt[1], centerPt[2]);
            obj.SetName(timeName);

            //sprintf(timeName, "TIME%2d%2d%2d", nowTime.wHour, nowTime.wMinute, nowTime.wSecond);
            UF_TERMINATE();
            return false;
        }
    }

    tag_t getOffsetSheet(vector<tag_t>& sheets, const char* name, const double *centerPt, const double *dir, const double dist, const double offsetDist)
    {
        vector<tag_t>::iterator iter;
        double tempDist = 0;
        double tempPt[3] = { 0 };
        UF_initialize();
        for (iter = sheets.begin(); iter != sheets.end();)
        {
            CSolidObj  obj(*iter);
            if (strncmp(name, obj.GetName(), 10) == 0)
            {//TIME000000X0000.00000Y0000.00000Z0000.00000OFFSET_DIST000.00
                sscanf(obj.GetName(), "%*[^O]OFFSET_DIST%lf", &tempDist);
                if (fabs(offsetDist - tempDist) < 0.05)
                {
                    sscanf(obj.GetName(), "%*[^X]X%lf%Y%lf%Z%lf", &tempPt[0], &tempPt[1], &tempPt[2]);
                    if (is_Equal(tempPt, centerPt, 0.1))
                    {
                        return *iter;
                    }
//                     else
//                     {
//                         moveObj(*iter, dir, dist);
//                         char name_[128] = { 0 };
//                         char buf[128] = { 0 };
//                         sscanf(name, "%[^O]", name_);
//                         sscanf(obj.GetName(), "%*[^O]%s", buf);
//                         strcat(name_, buf);
//                         obj.SetName(name_);
//                         return *iter;
//                     }
                }
                iter++;
            }
            else
            {
                sheets.erase(iter);
            }
        }
        return 0;
    }


    void resetUpdata()
    {
        tag_t  view_t = 0;
        UF_initialize();
        UF_VIEW_ask_tag_of_view_name("", &view_t);
        UF_DISP_regenerate_view(view_t);
        UF_TERMINATE();
    }

    void scaleBody(double param, tag_t &solid_)
    {
        UF_initialize();
        tag_t feature_eid = 0;
        char buf[64] = { 0 };
        double org_1[3] = { 0 };
        double org_2[3] = { 0 };

        CSolidObj obj_1(solid_);
        UF_VEC3_midpt(obj_1.m_bottomCenter, obj_1.m_topCenter, org_1);

        sprintf(buf, "%1.2f", param);
        UF_MODL_create_uniform_scale(solid_, UF_CSYS_WORK_COORDS, buf, &feature_eid);

        UF_MODL_ask_feat_body(feature_eid, &solid_);
        DeleteParms(1, &solid_);

        CSolidObj obj_2(solid_);
        UF_VEC3_midpt(obj_2.m_bottomCenter, obj_2.m_topCenter, org_2);

        double dist = 0;

        UF_VEC3_distance(org_1, org_2, &dist);

        if (dist)
        {
            double dir[3] = { 0 };
            UF_VEC3_sub(org_1, org_2, dir);
            moveObj(solid_, dir, dist);
        }

        UF_TERMINATE();
    }

    tag_t createBoundSheet(vector<tag_t>curves_)
    {
        UF_STRING_t ge;
        UF_initialize();
        UF_MODL_init_string_list(&ge);
        UF_MODL_create_string_list(1, BYTE_4(curves_.size()), &ge);
        ge.dir[0] = 1;
        ge.string[0] = BYTE_4(curves_.size());
        for (int i = 0; i < curves_.size(); i++)
        {
            ge.id[i] = curves_[i];
        }
        double tol[3];
        tol[0] = 0.001f;
        tol[1] = 0.5f * (PI / 180);
        tol[2] = 0.02f;
        tag_t bpobj = null_tag;

        UF_CALL(UF_MODL_create_bplane(&ge, tol, &bpobj));
        UF_MODL_free_string_list(&ge);
        UF_TERMINATE();
        return bpobj;
    }




}