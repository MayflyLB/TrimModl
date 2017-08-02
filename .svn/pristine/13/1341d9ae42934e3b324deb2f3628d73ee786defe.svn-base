
#include "TestFun.hpp"
using namespace NXOpen;
using namespace NXOpen::BlockStyler;

//------------------------------------------------------------------------------
// Initialize static variables
//------------------------------------------------------------------------------
Session *(TestFun::theSession) = NULL;
UI *(TestFun::theUI) = NULL;
//------------------------------------------------------------------------------
// Constructor for NX Styler class
//------------------------------------------------------------------------------
TestFun::TestFun()
{
    try
    {
        // Initialize the NX Open C++ API environment
        TestFun::theSession = NXOpen::Session::GetSession();
        TestFun::theUI = UI::GetUI();
        theDlxFileName = "TestFun.dlx";
        theDialog = TestFun::theUI->CreateDialog(theDlxFileName);
        // Registration of callback functions
        theDialog->AddApplyHandler(make_callback(this, &TestFun::apply_cb));
        theDialog->AddOkHandler(make_callback(this, &TestFun::ok_cb));
        theDialog->AddUpdateHandler(make_callback(this, &TestFun::update_cb));
        theDialog->AddInitializeHandler(make_callback(this, &TestFun::initialize_cb));
        theDialog->AddDialogShownHandler(make_callback(this, &TestFun::dialogShown_cb));
    }
    catch(exception&)
    {
        //---- Enter your exception handling code here -----
        throw;
    }
}

//------------------------------------------------------------------------------
// Destructor for NX Styler class
//------------------------------------------------------------------------------
TestFun::~TestFun()
{
    if (theDialog != NULL)
    {
        delete theDialog;
        theDialog = NULL;
    }
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    TestFun *theTestFun = NULL;
    try
    {
        theTestFun = new TestFun();
        // The following method shows the dialog immediately
        theTestFun->Show();
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        TestFun::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    if(theTestFun != NULL)
    {
        delete theTestFun;
        theTestFun = NULL;
    }
}


extern "C" DllExport int ufusr_ask_unload()
{
    //return (int)Session::LibraryUnloadOptionExplicitly;
    return (int)Session::LibraryUnloadOptionImmediately;
    //return (int)Session::LibraryUnloadOptionAtTermination;
}

extern "C" DllExport void ufusr_cleanup(void)
{
    try
    {
        //---- Enter your callback code here -----
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        TestFun::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

int TestFun::Show()
{
    try
    {
        theDialog->Show();
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        TestFun::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}


void TestFun::initialize_cb()
{
    try
    {
        group0 = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        edge_select0 = dynamic_cast<NXOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("edge_select0"));
        button0 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));
        button01 = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button01"));	
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        TestFun::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

void TestFun::dialogShown_cb()
{
    try
    {
        //---- Enter your callback code here -----
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        TestFun::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}


int TestFun::apply_cb()
{
    int errorCode = 0;
    try
    {
        //---- Enter your callback code here -----
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        errorCode = 1;
        TestFun::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}


int TestFun::update_cb(NXOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if(block == edge_select0)
        {
            curves.clear();
            m_simplePro_.clear();
            m_trimVCC.clear();
            m_vertexPts.clear();
            std::vector<NXOpen::TaggedObject *>tempObj = edge_select0->GetSelectedObjects();
            for (int i = 0; i < tempObj.size(); i++)
                curves.push_back(tempObj[i]->GetTag());
            sortCurves(curves, m_simplePro_, m_trimVCC, m_vertexPts, st_isTrimLClose);
            UF_INITIALIZE();
            tag_t temp;
            UF_CALL(UF_CURVE_create_point(m_vertexPts[0].vertexPt, &temp));
            UF_OBJ_set_color(temp, 186);
            UF_CALL(UF_CURVE_create_point(m_vertexPts[1].vertexPt, &temp));
            UF_OBJ_set_color(temp, 186);

        }
        else if (block == button0)
        {
            tempObj.clear();
            edge_select0->SetSelectedObjects(tempObj);
           
            double z_[3] = { 0,0,-1 };
            vector<CurveData> assistLD;
            completeAssistCurves(&m_vertexPts[0], m_simplePro_[0], z_, 40, assistLD, 0);//下模
            for (int i = 0; i < assistLD.size(); i++)
                UF_OBJ_set_color(assistLD[i].curve, 36);
        }
        else if (block == button01)
        {
            tempObj.clear();
            edge_select0->SetSelectedObjects(tempObj);
          
            double z_[3] = { 0,0,-1 };
            vector<CurveData> assistLD;
            completeAssistCurves(&m_vertexPts[0], m_simplePro_[0], z_, 40, assistLD, 1);//上模
            for (int i = 0; i < assistLD.size();i++)
                UF_OBJ_set_color(assistLD[i].curve,36);
        }
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        TestFun::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}


int TestFun::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(exception& ex)
    {
        //---- Enter your exception handling code here -----
        errorCode = 1;
        TestFun::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}


PropertyList* TestFun::GetBlockProperties(const char *blockID)
{
    return theDialog->GetBlockProperties(blockID);
}
