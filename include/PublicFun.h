//////////////////////////////////////////////////////////////////////////////////////
//	@file           PublicFun.h                                                     //
//	@brief          NX���ú�����װ����(���ֽ��̷��)                                   //
//	@version        1.1                                                             //
//	@author         ���	(2016��7��25���Ժ�Ҫ��,�㷨��UG����)                          //
//	@date           2016��6��7                                                      //
//////////////////////////////////////////////////////////////////////////////////////

#ifndef __MYFUNCTION__
#define __MYFUNCTION__
#define TOVAR TRUE
#define TODLG FALSE
#define _WIN32_WINNT _WIN32_WINNT_MAXVER
#include <afxwin.h>
#include <atlstr.h>
#include <afx.h>
#include <cctype>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <uf_group.h>
#include <uf_layout.h>
#undef  CreateDialog//��uf.h֮ǰȥ�� CreateDialog��ʶ��
#include <uf.h>
#include <uf_undo.h>
#include <uf_ui.h>
#include <uf_part.h>
#include <uf_layer.h>
#include <ug_session.hxx>
#include <ug_exception.hxx>
#include <uf_patt.h>
#include <uf_modl.h>
#include <uf_exit.h>
#include <ug_info_window.hxx>
#include <uf_part.h>
#include <uf_obj.h>
#include <uf_object_types.h>
#include <uf_modl_dfo.h>
#include <uf_disp.h>
#include <uf_vec.h>
#include <uf_exit.h>
#include <uf_view.h>
#include <uf_dirpath.h>
#include <uf_mtx.h>
#include <uf_so.h>
#include <uf_eval.h>
#include <uf_trns.h>
#include <uf_evalsf.h>
#include <uf_csys.h>
#include <uf_draw.h>
#include <uf_drf.h>
#include <uf_setup.h>
#include <uf_cam.h>
#include <uf_param_indices.h>
#include <uf_param.h>
#include <uf_ui_param.h>
#include <uf_ui_ont.h>
#include <uf_ncgroup.h>
#include <uf_csys.h>
#include <uf_attr.h>
#include <uf_oper.h>
#include <NXOpen/Arc.hxx>
#include <NXOpen/ArcCollection.hxx>
#include <NXOpen/Direction.hxx>
#include <NXOpen/DirectionCollection.hxx>
#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/BasePart.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/CurveDumbRule.hxx>
#include <NXOpen/Expression.hxx>
#include <NXOpen/ExpressionCollection.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>
#include <NXOpen/Features_PointSetBuilder.hxx>
#include <NXOpen/Features_PointSetFacePercentageBuilder.hxx>
#include <NXOpen/Features_PointSetFacePercentageBuilderList.hxx>
#include <NXOpen/Features_CurveFeature.hxx>
#include <NXOpen/IBaseCurve.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/NXObjectList.hxx>
#include <NXOpen/ObjectList.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Point.hxx>
#include <NXOpen/PointList.hxx>
#include <NXOpen/Preferences_PartModeling.hxx>
#include <NXOpen/Preferences_PartPreferences.hxx>
#include <NXOpen/ScCollector.hxx>
#include <NXOpen/ScRuleFactory.hxx>
#include <NXOpen/Section.hxx>
#include <NXOpen/SelectDisplayableObjectList.hxx>
#include <NXOpen/SelectFace.hxx>
#include <NXOpen/SelectObject.hxx>
#include <NXOpen/SelectionIntentRule.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Spline.hxx>
#include <NXOpen/SplineCollection.hxx>
#include <NXOpen/TaggedObject.hxx>
#include <NXOpen/Unit.hxx>
#include <NXOpen/Line.hxx>


#include <NXOpen/Direction.hxx>
#include <NXOpen/Features_FeatureCollection.hxx>
#include <NXOpen/Features_FitCurveBuilder.hxx>
#include <NXOpen/Features_GeometricConstraintData.hxx>
#include <NXOpen/Features_GeometricConstraintDataManager.hxx>
#include <NXOpen/Features_PointSet.hxx>
#include <NXOpen/GeometricUtilities_CurveExtensionBuilder.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Point.hxx>
#include <NXOpen/Preferences_PartModeling.hxx>
#include <NXOpen/Preferences_PartPreferences.hxx>
#include <NXOpen/SelectObject.hxx>
#include <NXOpen/SelectObjectList.hxx>
#include <NXOpen/SelectSpline.hxx>
#include <NXOpen/SelectTaggedObjectList.hxx>

#include <NXOpen/Features_OffsetCurveBuilder.hxx>
#include <NXOpen/GeometricUtilities_AlongSpineBuilder.hxx>
#include <NXOpen/GeometricUtilities_CurveFitData.hxx>
#include <NXOpen/GeometricUtilities_CurveFitOptions.hxx>
#include <NXOpen/GeometricUtilities_CurveOptions.hxx>
#include <NXOpen/GeometricUtilities_LawBuilder.hxx>
#include <NXOpen/GeometricUtilities_MultiTransitionLawBuilder.hxx>
#include <NXOpen/GeometricUtilities_NonInflectingLawBuilder.hxx>
#include <NXOpen/GeometricUtilities_SShapedLawBuilder.hxx>


#include <NXOpen/Edge.hxx>
#include <NXOpen/EdgeDumbRule.hxx>
#include <NXOpen/EdgeChainRule.hxx>
#include <NXOpen/Ellipse.hxx>
#include <NXOpen/EllipseCollection.hxx>
#include <NXOpen/Parabola.hxx>
#include <NXOpen/Hyperbola.hxx>
#include <NXOpen/HyperbolaCollection.hxx>


#include <Features_TrimBody.hxx>
#include <Part.hxx>
#include <PartCollection.hxx>
#include <Features_TrimBodyBuilder.hxx>
#include <ug_session.hxx>
#include <ug_exception.hxx>
#include <Body.hxx>
#include <Line.hxx>
#include <Session.hxx>
#include <CurveCollection.hxx>
#include <Features_FeatureCollection.hxx>
#include <NXObjectManager.hxx>
#include "uf_assem.h"
#include "uf_wave.h"

//#include "atlconv.h"
#pragma warning(disable:4996)


#include "windows.h"

#define UNDO_SIGNED_BIT 0x7fffffff
#define UserLimit private:
#define OutPut
#define InPut
#define InPutAndOutPut
#define I InPut
#define O OutPut
#define IO InPutAndOutPut

using namespace std;
using namespace NXOpen;
using namespace NXOpen::Features;

int report_error(char *file, int line, char *call, int irc);
#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))

#define UF_INITIALIZE() \
            UF_terminate();\
            UF_initialize();


#define UF_TERMINATE() \
		  UF_terminate();


#define DELETE_OBJ(tempObj) do{\
   if (UF_OBJ_ask_status(tempObj) != UF_OBJ_DELETED)\
{\
UF_CALL(UF_OBJ_delete_object(tempObj));\
}\
}while(0);


#define DELETE_VECTOR_OBJS(tempObjs) \
do\
{\
UF_INITIALIZE();\
for (int i = 0;i < tempObjs.size();i++)\
{\
	DELETE_OBJ(tempObjs[i]); \
}\
tempObjs.clear();\
}while(0);

#define  BYTE_4(x) ((0x00000000ffffffff)&(x))


#define NXCOMMIT_INFO_FEAT(NXTaggedObject_,retFeat) \
do{\
    try\
    {\
		NXTaggedObject_->Commit();\
        UF_INITIALIZE();\
        UF_CALL(UF_MODL_ask_current_feature(UF_ASSEM_ask_work_part(), &retFeat));\
        UF_TERMINATE();\
	}\
	catch (exception& ex)\
	{\
        retFeat=0;\
		char buf[256] = { 0 };\
		sprintf(buf, "\n/****************************\nFile:[%s]\nLineNum:[%d]\nProblem:[%s]\n****************************/\n\r", __FILE__, __LINE__, ex.what());\
		MyFun::disp(buf);\
	}\
} while (0);


#define NX_CATCH_EX_INFO do{\
    char buf[256] = { 0 };\
	sprintf(buf, "\nFile:%s\nLineNum:%d\nProblem:%s\n\r", __FILE__, __LINE__, ex.what());\
	MyFun::disp(buf);\
}while (0);

#define NX_CATCH_EX_INFO_USR(cstr_) do{\
    char buf[256] = { 0 };\
	sprintf(buf, "\nFile:%s\nLineNum:%d\nProblem:%s\nUSER STRING:%s\n\r", __FILE__, __LINE__, ex.what(),cstr_);\
	MyFun::disp(buf);\
}while (0);

#define SHOW_INFO_USR(cstr_) do{\
    char buf[256] = { 0 };\
	sprintf(buf, "\nFile:%s\nLineNum:%d\nUSER STRING:%s\n\r", __FILE__, __LINE__,cstr_);\
	MyFun::disp(buf);\
}while (0);



#define DEBUG_HIGHT_OBJS(temp) \
	UF_INITIALIZE();\
	UF_DISP_set_highlights(temp.size(),&temp[0],1);\
	MyFun::resetUpdata();\
	Sleep(1000);

#define DEBUG_UNHIGHT_OBJS(temp) \
	UF_INITIALIZE();\
	UF_DISP_set_highlights(temp.size(),&temp[0],0);\
	MyFun::resetUpdata();\
	Sleep(1000);


#define SET_STATUS(buf) do{UF_UI_set_status(buf);}while(0)

inline void getVectorTags(std::vector<NXOpen::TaggedObject *> buf, std::vector<tag_t> & temp)
{
	temp.clear();
	for (int i = 0;i < buf.size();i++)
	{
		temp.push_back(buf[i]->GetTag());
	}
}


namespace MyFun
{

	int GetModulePath(CString &rString);

	HMODULE GetCurrentModule();

	//..........................functions define..................................
	CString findUserPath();//�����û�Ŀ¼(��c:\program files\elec_tools\)
	CString findTempFilePath();//������ʱĿ¼
	CString getPartName(const tag_t &part = null_tag);


	//********************************************************************
	// Access:    	::public 
	// Description: ��������ķ�����(����ָ��ķ�����)
	// Par: 		const tag_t & faceTag
	// Par: 		double * pFaceNor
	// Par: 		double * pCen
	// Ret:   		bool
	//********************************************************************
	bool getFaceNormal(const tag_t & faceTag, double *pFaceNor, double* pCen = NULL);
	bool isHole(const tag_t & faceTag, double faceNor[3], double refPos_csys[3], double cen_csys[3]);

	tag_t createBoundaryBox(tag_t **faces, int &nFaces);
	tag_t SelectSingle(char* msg, UF_UI_mask_t *pTrips, int nTrips);

	int askApplication();                   //��ȡ��ǰʹ�õ�Ӧ��ģ��
	int SelectByClass(CString inf, UF_UI_mask_t *pTrips, int nTrips, tag_t** rstTags, int &nRst);

	//�������ִ���ʾ����Ϣ����
	void disp(char*);
	void disp(CString);
	void disp(int num);
	void disp(double num);

	void getTopAndBottomFace(const tag_t  body, tag_t &topFace, tag_t &bottomFace);

	//********************************************************************
	// Access:    	::public 
	// Description: ���������һ����p ����d �ó� ���������ӵ�pƫ�ƾ���d�ĵ�
	// Parame: 		double dir_csys[3]		����
	// Parame: 		double origin_csys[3]	�ο���
	// Parame: 		double distance			����
	// Par: 		double * rstPos			���ص�.
	// Ret:   		void
	//********************************************************************
	void getDirectionPos(const double dir_csys[3], const  double origin_csys[3], const double distance, double *rstPos);

	//����·���������Ŀ¼����c:\\temp\\temp1\\subtemp��ʽ
	void createDirectory(CString cstrPath);//һ��һ�������ļ���

	//Map point from wcs to csys.
	void mapWcsToCsys(double wcs[3], double csys[3]);

	//Map point from csys to wcs.
	void mapCsysToWcs(double csys[3], double wcs[3]);

	//Map direction from csys to wcs.
	void mapCsysToWcs_vector(double csys[3], double wcs[3]);
	void mapWcsToCsys_vector(double wcs[3], double csys[3]);

	char* getCurrentDir();
	char* getCurrentDate();
	void setUndoMark();
	//............................class define...................................................//

	class sInitInfo
	{
	public:
		sInitInfo();
		bool isFind;
		CString cstrPartSpec;
		CString cstrPartType;
		bool isHardness;
		double dStockRough, dStockSemi, dStockFinish;
		bool isCreateGroup;
		CString cstrLevel1Prefix, cstrLevel2Prefix;
		int iSubGroupQty;
		CString cstrMcsName;
	};

	class CObjManager
	{
	public:
		CObjManager();
		virtual ~CObjManager();
	public:
		void clearUselessData();//****Ӧ����ʹ���������в���ǰ���˼��
		int objCount();//debug ��ǰĿ���������
		void resetMap();//����������Ա��Ϊ0
		void addObjects(tag_t** objId, int nObj);//���ӳ�Ա
	public:
		map<tag_t, int> mapObjects;//1 ��ʾ����  //0����״̬�򲻿���  
	};

	class CObj
	{
		friend class CSolidObj;
	public:
		CObj(const tag_t  &Tag);
		CObj();
		void SetTag(tag_t &Obj);
		char* GetName();
		int SetName(char* Name);//If ok,then 0 returned
		int SetColor(int Color);//Input new color ,and old color will be returned;
		//UF_OBJ_BLUE  UF_OBJ_GREEN  UF_OBJ_CYAN  UF_OBJ_RED
		//UF_OBJ_MAGENTA UF_OBJ_YELLOW UF_OBJ_WHITE UF_OBJ_OLIVE
		//UF_OBJ_PINK  UF_OBJ_BROWN  UF_OBJ_ORANGE  UF_OBJ_PURPLE
		//UF_OBJ_DARK_RED UF_OBJ_AQUAMARINE UF_OBJ_GRAY
		int GetStatus();//UF_OBJ_DELETED UF_OBJ_TEMPORARY UF_OBJ_CONDEMNED UF_OBJ_ALIVE 
		int GetType();
		int GetSubtype();
		int  SetBlankStatus(int);//old value will returned:UF_OBJ_NOT_BLANKED UF_OBJ_BLANKED
		int SetLineWidth(int);//old value will returned:UF_OBJ_WIDTH_NORMAL UF_OBJ_WIDTH_THICK UF_OBJ_WIDTH_THIN 
		int SetLineFont(int);//old value will returned: 
		//UF_OBJ_FONT_SOLID
		//UF_OBJ_FONT_DASHED
		//UF_OBJ_FONT_PHANTOM
		//UF_OBJ_FONT_CENTERLINE
		//UF_OBJ_FONT_DOTTED
		//UF_OBJ_FONT_LONG_DASHED
		//UF_OBJ_FONT_DOTTED_DASHED 
		int SetLayer(int);//old value will returned.
		int GetDispStatus();//Result may be:  UF_OBJ_DELETED 
		//UF_OBJ_TEMPORARY 
		//UF_OBJ_CONDEMNED 
		//UF_OBJ_ALIVE 
		virtual ~CObj()
		{
		}
	private:
		tag_t objTag;
		char name[UF_OBJ_NAME_LEN + 1];
	public:
		UF_OBJ_disp_props_t dispProps;
	};

	class CSolidObj :public CObj
	{
	public:
		CSolidObj(const tag_t & sObj=0);
		~CSolidObj();
		tag_t CreateBoundaryBox();
		tag_t CreateBoundaryCylinder();
		void addOneObject(const tag_t &);//��е�������µ����box
		void adjustObject(tag_t SecondTag);//���ݵ�ǰtag,�������ӱ߽�
		CSolidObj& operator=(const CSolidObj& r);
	private:
		bool initialize(const tag_t & solidTag);
	private:
		bool m_isOK;
	public:
		size_t m_count;
		double m_limit[6], m_size[3];				//use wcs coordinate
		double m_topCenter[3], m_bottomCenter[3];	//�������box�������ĵ�����͵Ͷ����ĵ�����
	};
	
	/********************************************LB**********************************************/
	
	//Des:		 Need The Fun to Delete The RetValue:deleteList()
	uf_list_p_t  p_TagsCreateList(I const size_t numOfTag, I const tag_p_t temp);
	//Des:		 Need to Delete The RetValue: delete[] retValue
	tag_p_t		 p_listCreateTags(I const uf_list_p_t temp, O size_t & numOfTags);
	//Fun:		 Delete the list of tag
	void		 deleteList(uf_list_p_t&list_s);
	
	
	//Des:	�������
	//Ret:	��������������
	uf_list_p_t extrudedOperation(const size_t numOfTag, const tag_p_t temp, double*dir,
								  const double LimitUp, const double LimitDown);
	uf_list_p_t extrudedOperation(const uf_list_p_t temp, const Vector3d &vector3, const double LimitUp,
								  const double LimitDown);

	//Par:	featcCure��һ�����߼�(���߱����й�ͬ��)������ 
	//Des:	�����߼��뵽һ������tag��.��Ҫdelete;
	//Ret:	���ߺϲ���ı�ʶTag
	tag_p_t createJoinedCurves(tag_t * pCurvesIO, int &counts);


	double get_line_length(tag_t line, double * strat_pt = NULL, double * end_pt = NULL);
	double get_arc_length(tag_t arc, double * centerPt = NULL, double * radius = NULL);
	double  get_spline_length(tag_t &spline,bool is_Edge=false);
	//********************************************************************
	// Access:    	::public 
	// Description: ����Feature
	// Par: 		tag_t curve_
	// Par: 		int count_
	// Ret:   		tag_t
	//********************************************************************
	tag_t createPointSetFeat(tag_t spline, int count_);




	tag_t createFitCureFeat(tag_t pointSetFeat);
	//********************************************************************
	// Access:    	::public 
	// Description: 
	// Par: 		tag_t fitCurve �����������.
	// Par: 		double dist
	// Par: 		double * dir
	// Par: 		bool flag
	// Ret:   		tag_t
	//********************************************************************
	tag_t offsetCurve(tag_t fitCurve, double dist, double* dir = NULL, bool flag = false);
	tag_t smoothedCurves(vector<tag_t>curves, int stepover, double dist = 40);



	//********************************************************************
	// Access:    	::public 
	// Description: Ӱ������
	// Par: 		double origin_point[3] ���ڴ���Ӱ��ƽ��
	// Par: 		double plane_normal[3] ���ڴ���Ӱ��ƽ��
	// Par: 		vector<tag_t> & curveCollection ������ҪӰ�������,���Ӱ���������
	// Ret:   		tag_t  ƽ��
	//********************************************************************
	tag_t projectOperation(double origin_point[3], double plane_normal[3], vector<tag_t>& curveCollection);


	//********************************************************************
	// Access:    	::public 
	// Description: ��˳���� ����spline
	// Parame: 		I const tag_t spline_1
	// Parame: 		O double point[3]
	// Returns:   	tag_t
	//********************************************************************

	tag_t smoothedAndFittingCurves(const tag_t I spline_1, double O distPoint[3]);//


	//********************************************************************
	// Access:    	::public 
	// Description: ��һ��ֱ����ƽ��Ľ��� 
	// Parame: 		const double planeVector[3]ƽ��ķ�������
	// Parame: 		const double planePoint[3]ƽ�澭����һ������
	// Parame: 		const double lineVector[3]ֱ�ߵķ������� 
	// Parame: 		const double linePoint[3]ֱ�߾�����һ������
	// Returns:   	double*���ؽ������꣬����Ϊ3 
	//********************************************************************
	void CalPlaneLineIntersectPoint(const double planeVector[3], const double planePoint[3], const double lineVector[3],
									const double linePoint[3], double*returnResult);

	//************************************
	// Method:    pointPerpToLine
	// FullName:  MyFun::pointPerpToLine
	// Access:    public 
	// Returns:   void
	// Qualifier: �㵽ֱ�ߵĴ�ֱ��
	// Parameter: const double pt3[3]
	// Parameter: const double lineStartPt[3]
	// Parameter: const double lineEndPt[3]
	// Parameter: double * rt
	//************************************
	inline void pointPerpToLine(const double pt3[3], const double lineStartPt[3], const double lineEndPt[3], double rt[3])
	{
		double vec_diff[3];
		UF_VEC3_sub(lineStartPt, lineEndPt, vec_diff); 
		CalPlaneLineIntersectPoint(vec_diff, pt3, vec_diff, lineEndPt, rt);
		return;
	}
	//********************************************************************
	// Access:    	::public 
	// Description: �㵽ƽ�����
	// Par: 		const double planeVector[3]
	// Par: 		const double planePoint[3]
	// Par: 		double * pt
	// Ret:   		double
	//********************************************************************
	double distOfPtToPlane(const double planeVector[3], const double planePoint[3], double  pt[3]);


	inline void VectorProjectInPlane(const double planeVector[3],const  double  dir[3],double retDir[3])
	{
		double pt[3] = {0};
		CalPlaneLineIntersectPoint(planeVector, pt, planeVector, dir, retDir);
		return;
	}



	//********************************************************************
	// Access:    	::public 
	// Description: �㵽ֱ�ߵľ���
	// Parame: 		const tag_t point
	// Parame: 		const tag_t line
	// Returns:   	double
	//********************************************************************
	double distanceOfPointToLine(const tag_t point, const tag_t line);

	//********************************************************************
	// Access:    	::public 
	// Description: �㵽�߶εľ���	
	// Parame: 		const double pt3[3]
	// Parame: 		const double lineStartPt[3]�߶εĶ˵� 
	// Parame: 		const double lineEndPt[3] �߶εĶ˵�
	// Returns:   	double
	//********************************************************************
	double distanceOfPointToLine(const double pt3[3], const double lineStartPt[3], const double lineEndPt[3]);


	//********************************************************************
	// Access:    	::public 
	// Description: �㵽ֱ�ߵľ����
	// Par: 		const tag_t point
	// Par: 		const tag_t line
	// Ret:   		double *if the value is not null
	//********************************************************************
	void pointPorjectToLine(const double * pt3, const tag_t line, double* buf);

	//********************************************************************
	// Access:    	::public 
	// Description: �㵽ֱ�ߵľ����
	// Par: 		const tag_t point
	// Par: 	
	// Ret:   		double *if the value is not null
	//********************************************************************
	void  pointPorjectToLine(const double * pt3, const double *start_point, const double* end_point, double* buf);

	//********************************************************************
	// Access:    	::public 
	// Description: ����߶εĹ�ϵ 0 �����߶���,1�����߶ε�������,2����߶β�����;
	// Par: 		const double * point3
	// Par: 		const double * lineStartPoint
	// Par: 		const double * lineEndPoint
	// Ret:   		int
	//********************************************************************
	int relationOfPointAndLine(const double* point3, const double* lineStartPoint, const double* lineEndPoint);

	//********************************************************************
	// Access:    	::public 
	// Description: ����������
	// Parame: 		tag_t src_body
	// Parame: 		tag_t * linked_body
	// Returns:   	int
	//********************************************************************
	int create_linked_body(tag_t src_body, tag_t &linked_body);
	int create_linked_body(tag_t src_body, tag_t target_body, tag_t &linked_body);
	int create_linked_body(tag_t src_body, tag_t *linked_body);
	//********************************************************************
	// Access:    	::public 
	// Description: pt1�����pt2�ĶԳƵ�pt3 �����
	// Par: 		const double * pt1
	// Par: 		const double * pt2
	// Par: 		double * pt3 ���ص�
	// Ret:   		void
	//********************************************************************
	void  ProjectPoint(const double * pt1, const double * pt2, double * pt3);

	//********************************************************************
	// Access:    	::public 
	// Description: pt1�����ֱ�ߵĶԳƵ�pt3  
	// Par: 		const double * pt1
	// Par: 		
	// Par: 		double * pt3 ���ص�
	// Ret:   		void
	//********************************************************************
	void  ProjectPointToLine(const double * pt1, const double * L1, const double * L2, double * pt3);

	//********************************************************************
	// Access:    	::public 
	// Description:  ������������¼��򴴽�������.
	// Parame: 		tag_t target_body
	// Parame: 		tag_t * result_body
	// Returns:   	int
	//********************************************************************
	int duplicate_body(tag_t target_body, tag_t *result_body);


	//********************************************************************
	// Access:    	::public 
	// Description: Բ������������͹���ǰ���
	// Par: 		tag_t face_tag
	// Par: 		tag_t body_tag
	// Ret:   		BOOL
	//********************************************************************
	BOOL cylindricalFaceIsRaised(tag_t face_tag, tag_t body_tag);

	//********************************************************************
	// Access:    	::public 
	// Description: ��ȡƽ������ⷨ����
	// Par: 		const tag_t & faceTag
	// Par: 		double faceNor[3]
	// Ret:   		void
	//********************************************************************
	//void getPlaneNormal(const tag_t & faceTag, double faceNor[3]);

	//********************************************************************
	// Access:    	::public 
	// Description: ��������ƽ�о���ƽ���ܼ�ס�����Բ���İ뾶
	// Par: 		tag_t face1
	// Par: 		tag_t face2
	// Ret:   		double
	//********************************************************************
	double maxRadiusOfTwoFaces(tag_t face1, tag_t face2);

	//********************************************************************
	// Access:    	::public 
	// Description: ���Բ�����������İ뾶 ���ĵ� �᷽��
	// Par: 		tag_t face_tag
	// Ret:   		double
	//********************************************************************
	double askFaceData(tag_t face_tag, double * direction = NULL, double *cenPoint = NULL);

	//********************************************************************
	// Access:    	::public 
	// Description:  ��ֱ����1 ƽ�з���2, ��������0 ��ֱ�߷���-1
	// Par: 		tag_t line_
	// Par: 		double direction3[3]
	// Ret:   		int
	//********************************************************************
	int relateOfLineAndDirection(tag_t line_1, double direction3[3]);


	//********************************************************************
	// Access:    	::public 
	// Description: ��ֱ����1 ƽ�з���2, ��������0 ��ֱ�߷���-1 ��һ��ֱ����3
	// Par: 		tag_t line_1
	// Par: 		tag_t line_2
	// Ret:   		int
	//********************************************************************
	int relateOfLines(tag_t line_1, tag_t line_2);

	//********************************************************************
	// Access:    	::public 
	// Description: ȡ�� �������е� �� ��������  ƽ�е� ����
	// Par: 		tag_t face_
	// Par: 		double direction3[3]
	// Par: 		int * num
	// Ret:   		tag_p_t
	//********************************************************************
	tag_p_t askLineOfFaceFromDirection(tag_t I face_, double I direction3[3], int O * num);

	//********************************************************************
	// Access:    	::public 
	// Description: �ڸ�����Tags���󼯺��������͸���һ����tag���ո�������ƽ����ӵĶ���tag
	// Par: 		vector<TaggedObject * > & tagObjects
	// Par: 		TaggedObject * tagObject
	// Par: 		double * direction3
	// Ret:   		vector<TaggedObject*>
	//********************************************************************
	vector<TaggedObject*> getJoinedFaceInArea(TaggedObject  * tagObject, double  * direction3);

	//********************************************************************
	// Access:    	::public 
	// Description: �ж����ϵ������ƽ���ǰ���͹.
	// Par: 		tag_t face_1
	// Par: 		tag_t face_2
	// Ret:   		bool
	//********************************************************************
	bool twoBodyPlaneIsRaised(tag_t edge_);

	//********************************************************************
	// Access:    	::public 
	// Description: ���������ļнǻ���
	// Par: 		double * dir1
	// Par: 		double * dir2
	// Ret:   		double
	//********************************************************************
	double angleOfVectors(const double *dir1, const double *dir2);
	double angleOfVectors_(const double *dir1, const double *dir2);
	//********************************************************************
	// Access:    	::public 
	// Description: 
	// Par: 		double * dir1 ����
	// Par: 		double * dir2 ��ķ�����
	// Par: 		double * outdir ������ͶӰ������
	// Ret:   		void
	//********************************************************************
	void VectorProjectToPlane(double *dir, double *planeDir, double * outdir);
	//********************************************************************
	// Access:    	::public 
	// Description:  ����ƽ��Ļ���
	// Par: 		double dir_1[3] �淨����
	// Par: 		double dir_2[3]	�淨����
	// Ret:   		double
	//********************************************************************
	double radiansOfFaces(double dir_1[3], double dir_2[3]);

	//********************************************************************
	// Access:    	::public 
	// Description: ��Բ�ĵ�(���ר��)
	// Par: 		double * oriPos
	// Par: 		double * dir_
	// Par: 		double radius_
	// Par: 		double angle_
	// Par: 		double * retPos
	// Ret:   		int
	//********************************************************************
	int acuteAngleOfPt(double * oriPos, double * dir_, double radius_, double angle_, double * retPos);


	//********************************************************************
	// Access:    	::public 
	// Description: ����ƽ����Ѵ��ڵĽ��߶�
	// Par: 		tag_t face1
	// Par: 		tag_t face2
	// Ret:   		tag_t
	//********************************************************************
	tag_t getEdgeFromFaces(tag_t face1, tag_t face2);

	//********************************************************************
	// Access:    	::public 
	// Description: �ж����������Ƿ����
	// Par: 		double * one_3
	// Par: 		double * two_3
	// Par: 		double three_tolerance
	// Ret:   		int
	//********************************************************************
	bool is_Equal(const double *one_3, const double *two_3, const double three_tolerance);

	double Max_D_value(const double *one_3, const double *two_3);
    inline double distOfpts(const double *one_3, const double *two_3)
    {
        double dist;
        UF_VEC3_distance(one_3,two_3,&dist);
        return dist;
    }

	//********************************************************************
	// Access:    	::public 
	// Description: Ԫ�������,���ظ���������������true,�ظ�ɾ��������ظ���Ԫ�ز�����false
	// Par: 		std::vector<tag_t> & logTags
	// Par: 		std::vector<tag_t> & tempTags
	// Par: 		tag_t tempT
	// Ret:   		bool
	//********************************************************************
	bool vectorTagUnique(std::vector<tag_t>&logTags, tag_t &tempT);

	//********************************************************************
	// Access:    	::public 
	// Description:  ����
	// Par: 		int * cnt
	// Par: 		tag_t * * tags
	// Ret:   		void
	//********************************************************************
	void DeleteParms(int cnt, const tag_t* tags);

	inline double dist3(double *d1, double *d2)
	{
		double d3 = 0.0f;
		UF_VEC3_distance(d1, d2, &d3);
		return d3;
	}


	//********************************************************************
	// Access:    	::public 
	// Description: ������ֱ�ߵĽ��� 
	// Par: 		const double * L1p1
	// Par: 		const double * L1p2
	// Par: 		const double * L2p1
	// Par: 		const double * L2p2
	// Par: 		double * p5���ؽ���
	// Ret:   		void
	//********************************************************************
	void intersectPtOfLines(const double * L1p1, const double * L1p2, const double * L2p1, const double * L2p2, double * rtPt);

	inline void intersectPtOfLines_(const double * dir1, const double * L1p2, const double * dir2, const double * L2p2, double * rtPt)
	{
		double L1p1[3];
		MyFun::getDirectionPos(dir1, L1p2 , 5 , L1p1);
		double L2p1[3];
		MyFun::getDirectionPos(dir2, L2p2 , 5 , L2p1);
		intersectPtOfLines( L1p1, L1p2,  L2p1,  L2p2, rtPt);
	}

	struct tempS
	{
		tag_t tc;
		double pt1[3];
		double pt2[3];
		tempS()
		{
			tc = 0;
			pt1[0] = 0.0f;
			pt1[1] = 0.0f;
			pt1[2] = 0.0f;
			pt2[0] = 0.0f;
			pt2[1] = 0.0f;
			pt2[2] = 0.0f;
		}
		tempS(const tempS& ts)
		{
			this->tc = ts.tc;
			this->tc = ts.tc;
			this->pt1[0] = ts.pt1[0];
			this->pt1[1] = ts.pt1[1];
			this->pt1[2] = ts.pt1[2];
			this->pt2[0] = ts.pt2[0];
			this->pt2[1] = ts.pt2[1];
			this->pt2[2] = ts.pt2[2];
		}
		bool operator==(const tempS& ts)
		{
			return this->tc == ts.tc;
		}
		tempS& operator=(const tempS& ts)
		{
			if (&ts != this)
			{
				this->tc = ts.tc;
				this->pt1[0] = ts.pt1[0];
				this->pt1[1] = ts.pt1[1];
				this->pt1[2] = ts.pt1[2];
				this->pt2[0] = ts.pt2[0];
				this->pt2[1] = ts.pt2[1];
				this->pt2[2] = ts.pt2[2];
			}
			return *this;
		}
	};
	//********************************************************************
	// Access:    	::public 
	// Description: ȡ��Ƭ���Ͽ��ڴ�����,�����ݿ��ڷ�����.
	// Par: 		const uf_list_p_t pt
	// Par: 		const int n ��������tag*ָ��Ŀռ��С��Ϊn��tag
	// Par: 		tag_t * tempEdges1
	// Par: 		int & tNum1 �����tempEdges1����Чtag�ĸ���
	// Par: 		tag_t * tempEdges2
	// Par: 		int & tNum2 �����tempEdges2����Чtag�ĸ���
	// Ret:   		int
	//********************************************************************
	int orderCurvesList(const uf_list_p_t pt, const int nOfE, tag_t * tempEdges1, int & tNum1, tag_t *tempEdges2, int &tNum2);


	//********************************************************************
	// Access:    	::public 
	// Description: ������ȫ��ȷ���-1,����0.
	// Par: 		tag_t face
	// Par: 		tag_t pt
	// Par: 		double depth �׵����
	// Par: 		double deltaSafe	���Ҫ������
	// Ret:   		int
	//********************************************************************
	int thicknessIsSafe(tag_t body, tag_t ptT, const double *dir_, double depth, double deltaSafe = 0.0);

	//********************************************************************
	// Access:    	::public 
	// Description: �� solid��������ָ������ɫ
	// Par: 		tag_t body
	// Par: 		int color
	// Ret:   		int
	//********************************************************************
	int setColorOfBodyFaces(tag_t body, int color);

	//********************************************************************
	// Access:    	::public 
	// Description: ����һ�����ϵ����湲��
	// Par: 		tag_t f1
	// Par: 		tag_t f2
	// Ret:   		int
	//********************************************************************
	tag_t getTogetherEdge(tag_t f1, tag_t f2);

	double* getCurvePt(const tag_t curve, double param, double * pt);

	tag_t moveObj(const tag_t body, const  double *dir, const  double dist, bool is_Move = true);


	//********************************************************************
	// Access:    	::public 
	// Description: 
	// Par: 		tag_t obj_tag
	// Par: 		double centerPt[3] ����
	// Par: 		double dir[3]	����	
	// Par: 		double & dist	����
	// Ret:   		bool
	//********************************************************************
	bool sheetMoveInfo(tag_t obj_tag, char timeName[128], double centerPt[3], double dir[3], double &dist);//name : TIME000000X.....Y.....Z..... false Ϊ��һ����������λ��δ�� .


	//********************************************************************
	// Access:    	::public 
	// Description: 
	// Par: 		vector<tag_t> & sheets
	// Par: 		char * name
	// Par: 		double * centerPt
	// Par: 		double * dir
	// Par: 		double offsetDist
	// Ret:   		tag_t
	//********************************************************************
/*	tag_t getOffsetSheet(vector<tag_t>& sheets, const char* name, const double *centerPt, const double *dir, const double dist, const double  offsetDist);*/


	void resetUpdata();

	void scaleBody(double param, tag_t &solid_);

	tag_t createBoundSheet(vector<tag_t>curves_);

	//********************************************************************
	// Access:    	::public 
	// Description: 
	// Par: 		vector<tag_t> Curves__ ������ �� Ҳ��������ͨ����
	// Par: 		double dist
	// Par: 		vector<tag_t> & rt
	// Par: 		const double * curvesCenterPt ���߼������ĵ�,��������Ϊƫ�õķ����
	// Par: 		const bool is_reverseDirection
	// Ret:   		void
	//********************************************************************
	void offsetCurve(vector<tag_t> Curves__, double dist, vector<tag_t>& rt,const double* curvesCenterPt = NULL,const bool is_reverseDirection = false);

	void VT_VEC3_rotate(const double vec[3], const double rotation_axis[3], double angle, double rotated_vec[3]);
		
		
		
		
	//********************************************************************
	// Access:    	::public 
	// Description: �����������ӵ�����ڽǽǶ�. 
	// Par: 		tag_t face1
	// Par: 		tag_t face2
	// Par: 		tag_t edge�����߿ɲ��ṩ
	// Ret:   		double
	//********************************************************************
	double angleOfFaces(tag_t face1, tag_t face2, tag_t edge = 0);


	inline int retCol(tag_t obj)
	{
		UF_INITIALIZE();
		UF_OBJ_disp_props_t pt;
		UF_OBJ_ask_display_properties(obj, &pt);
		//UF_TERMINATE();
		return pt.color;
	}
	inline tag_t setAbsSys()
	{
		UF_initialize();
		tag_t csys_id;
		double csys_origin[3] = { 0 };
		tag_t matrix_id;
		double matrix_values[9] = { 1,0,0, 0,1,0, 0,0,1 };
		UF_CSYS_create_matrix(matrix_values, &matrix_id);
		UF_CSYS_create_csys(csys_origin, matrix_id, &csys_id);
		UF_CSYS_set_wcs(csys_id);
		return csys_id;
	}
}


#endif