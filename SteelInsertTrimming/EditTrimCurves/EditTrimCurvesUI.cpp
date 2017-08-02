
#include "EditTrimCurvesUI.hpp"
using namespace NXOpen;
using namespace NXOpen::BlockStyler;
   

Session *(EditTrimCurves::theSession) = NULL;
UI *(EditTrimCurves::theUI) = NULL;

EditTrimCurves::EditTrimCurves()
{
    try
    {
        m_implEdit = nullptr;
        EditTrimCurves::theSession = NXOpen::Session::GetSession();
        EditTrimCurves::theUI = UI::GetUI();
        theDlxFileName = "EditTrimCurves.dlx";
        theDialog = EditTrimCurves::theUI->CreateDialog(theDlxFileName);
        theDialog->AddApplyHandler(make_callback(this, &EditTrimCurves::apply_cb));
        theDialog->AddOkHandler(make_callback(this, &EditTrimCurves::ok_cb));
        theDialog->AddUpdateHandler(make_callback(this, &EditTrimCurves::update_cb));
        theDialog->AddInitializeHandler(make_callback(this, &EditTrimCurves::initialize_cb));
        theDialog->AddDialogShownHandler(make_callback(this, &EditTrimCurves::dialogShown_cb));
        INIT_ABS_CSYS();
    }
    catch(exception&)
    {
        throw;
    }
}

EditTrimCurves::~EditTrimCurves()
{
    if (theDialog != NULL)
    {
        TERM_ABS_CSYS();
        delete theDialog;
        theDialog = NULL;
    }
}

void EditTrimCurves::initialize_cb()
{
    try
    {
        groupGernate = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("groupGernate"));
        curvesSelect = dynamic_cast<NXOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("curvesSelect"));
        cutDir = dynamic_cast<NXOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("cutDir"));
        generateDir = dynamic_cast<NXOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("generateDir"));
        generateDirIsOK = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("generateDirIsOK"));
        assistSelect = dynamic_cast<NXOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("assistSelect"));
        cutButton = dynamic_cast<NXOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("cutButton"));
        linears[0] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear0"));
        linears[1] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear1"));
        linears[2] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear2"));
        linears[3] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear3"));
        linears[4] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear4"));
        linears[5] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear5"));
        linears[6] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear6"));
        linears[7] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear7"));
        linears[8] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear8"));
        linears[9] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear9"));
        linears[10] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear10"));
        linears[11] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear11"));
        linears[12] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear12"));
        linears[13] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear13"));
        linears[14] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear14"));
        linears[15] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear15"));
        linears[16] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear16"));
        linears[17] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear17"));
        linears[18] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear18"));
        linears[19] = dynamic_cast<NXOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear19"));
        manipulator = dynamic_cast<NXOpen::BlockStyler::SpecifyOrientation*>(theDialog->TopBlock()->FindBlock("manipulator"));

        if (m_implEdit)
        {
            delete m_implEdit;
            m_implEdit = nullptr;
            m_implEdit = new ImplEditCurves(curvesSelect, cutDir);
        }
        else
        {
            m_implEdit = new ImplEditCurves(curvesSelect,cutDir);
        }


        generateDir->SetShow(false);
        generateDirIsOK->SetShow(false);
        assistSelect->SetShow(false);
        cutButton->SetShow(false);
        manipulator->SetShow(false);
    }
    catch(exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

int EditTrimCurves::update_cb(NXOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if(block == curvesSelect) 
        {

        }
        else if(block == cutDir)
        {
            if (curvesSelect->GetSelectedObjects().size())
            {
                m_implEdit->sortedArcsLines();
                NXOpen::Point3d origin_;
                manipulator->SetEnable(true);
                manipulator->SetOrigin(origin_);
                origin_.X = 1;
                manipulator->SetXAxis((Vector3d)origin_);
                origin_.X = 0;
                origin_.Y = 1;
                manipulator->SetYAxis((Vector3d)origin_);

                cutDir->SetShow(false);
                curvesSelect->SetShow(false);
                generateDir->SetShow(true);
                generateDirIsOK->SetShow(true);
                generateDir->Focus();
            }
        }
        else if (block == generateDir)
        {
            UF_INITIALIZE();
            m_implEdit->adjustCurves(generateDir->Point(), generateDir->Vector());
        }
        else if (block == generateDirIsOK)
        {
            generateDir->SetEnable(false);
            generateDir->SetShow(false);
            generateDirIsOK->SetShow(false);
            assistSelect->SetShow(true);
            cutButton->SetShow(true);
            manipulator->SetShow(true);
            assistSelect->Focus();
            m_implEdit->deleteAxisAndSplines(assistSelect, manipulator,linears);
        }
        else if (block == assistSelect)
        {
            manipulator->Focus();
            m_implEdit->setFlag();
        }
        else if(block == manipulator)
        {
            manipulator->SetVisibleManipulatorHandles(0x42);//0x2
            m_implEdit->moveOper();
        }
        else if (cutButton== block)
        {
            if (m_implEdit)
                m_implEdit->breakLineToTwo();
        }
        else
        {
            if(m_implEdit)
                m_implEdit->specifyLinear(block);
        }
    }
    catch(exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

int EditTrimCurves::apply_cb()
{
    int errorCode = 0;
    try
    {
        if (m_implEdit)
        {
            delete m_implEdit;
            m_implEdit = nullptr;
        }
    }
    catch(exception& ex)
    {
        errorCode = 1;
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

int EditTrimCurves::Show()
{
    try
    {
        theDialog->Show();
    }
    catch (exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

void EditTrimCurves::dialogShown_cb()
{
    try
    {

    }
    catch(exception& ex)
    {

        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

int EditTrimCurves::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(exception& ex)
    {
        errorCode = 1;
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

PropertyList* EditTrimCurves::GetBlockProperties(const char *blockID)
{
    return theDialog->GetBlockProperties(blockID);
}

extern "C" DllExport void  ufusr(char *, int *, int )
{
    EditTrimCurves *theEditTrimCurves = NULL;
    try
    {
        theEditTrimCurves = new EditTrimCurves();
        theEditTrimCurves->Show();
    }
    catch (exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    if (theEditTrimCurves != NULL)
    {
        delete theEditTrimCurves;
        theEditTrimCurves = NULL;
    }
}

extern "C" DllExport int ufusr_ask_unload()
{
    return (int)Session::LibraryUnloadOptionImmediately;
}

extern "C" DllExport void ufusr_cleanup(void)
{
    try
    {

    }
    catch (exception& ex)
    {
        EditTrimCurves::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}