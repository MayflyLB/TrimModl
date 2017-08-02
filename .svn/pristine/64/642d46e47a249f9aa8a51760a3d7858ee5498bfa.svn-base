
#include "SteelInsertTrimming.hpp"
using namespace NXOpen;
using namespace NXOpen::BlockStyler;

const char TOOL_LOCAL_QUENCH_TYPE[] = "局部淬火结构";
const char TOOL_ENTIRE_QUENCH_TYPE[] = "整体淬火结构";
const double tempD[3] = { 0 };
Session *(SteelInsertTrimming::theSession) = NULL;
UI *(SteelInsertTrimming::theUI) = NULL;

SteelInsertTrimming::SteelInsertTrimming()
{
    try
    {
        INIT_ABS_CSYS();
        // Initialize the NX Open C++ API environment
        SteelInsertTrimming::theSession = NXOpen::Session::GetSession();
        SteelInsertTrimming::theUI = UI::GetUI();
        theDlxFileName = "SteelInsertTrimming.dlx";
        theDialog = SteelInsertTrimming::theUI->CreateDialog(theDlxFileName);
        // Registration of callback functions
        theDialog->AddApplyHandler(make_callback(this, &SteelInsertTrimming::apply_cb));
        theDialog->AddOkHandler(make_callback(this, &SteelInsertTrimming::ok_cb));
        theDialog->AddUpdateHandler(make_callback(this, &SteelInsertTrimming::update_cb));
        theDialog->AddInitializeHandler(make_callback(this, &SteelInsertTrimming::initialize_cb));
        theDialog->AddDialogShownHandler(make_callback(this, &SteelInsertTrimming::dialogShown_cb));
    }
    catch (exception& ex)
    {
        //---- Enter your exception handling code here -----
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
        throw;
    }
}

SteelInsertTrimming::~SteelInsertTrimming()
{
    if (theDialog != NULL)
    {
        TERM_ABS_CSYS();
        delete theDialog;
        theDialog = NULL;
    }
}

void SteelInsertTrimming::initialize_cb()
{
    try
    {
        group = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));
        selectSheet = dynamic_cast<NXOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("selectSheet"));
        isMove = dynamic_cast<NXOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("isMove"));
        CopyGroup = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("CopyGroup"));
        moveDirUI = dynamic_cast<NXOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("moveDirUI"));
        sheetMoveDistUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("sheetMoveDistUI"));
        sheetOffsetDistUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("sheetOffsetDistUI"));
        offsetDeviationUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("offsetDeviationUI"));
        stepoverDeviationUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("stepoverDeviationUI"));
        intervalDistUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("intervalDistUI"));
        curvesSelect = dynamic_cast<NXOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("curvesSelect"));
        modlTypeUI = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("modlTypeUI"));
        groupParam = dynamic_cast<NXOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("groupParam"));
        toolTypeUI = dynamic_cast<NXOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("toolTypeUI"));
        cutDeltaLenUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("cutDeltaLenUI"));
        toolHeightUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("toolHeightUI"));
        cutDirCutLenUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("cutDirCutLenUI"));
        cutDirToolBlankUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("cutDirToolBlankUI"));
        cutShapeWideUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("cutShapeWideUI"));
        cutShapeBlankUI = dynamic_cast<NXOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("cutShapeBlankUI"));
    
    }
    catch (exception& ex)
    {
        //---- Enter your exception handling code here -----
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}

int SteelInsertTrimming::apply_cb()
{
    int errorCode = 0;
    try
    {
        generateOper();
    }
    catch (exception& ex)
    {
        errorCode = 1;
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

int SteelInsertTrimming::update_cb(NXOpen::BlockStyler::UIBlock* block)
{
    try
    {
        /////////////////////////////////////
        if (block == selectSheet)
        {
            std::vector<NXOpen::TaggedObject *>&&sheet = selectSheet->GetSelectedObjects();
            UF_initialize();
            std::vector<NXOpen::TaggedObject *>::iterator iter;
            int bodyType = 0;
            sheetsTag.clear();
            for (iter = sheet.begin(); iter != sheet.end();)
            {
                UF_MODL_ask_body_type((*iter)->GetTag(), &bodyType);
                if (bodyType == UF_MODL_SHEET_BODY)
                {
                    sheetsTag.push_back((*iter)->GetTag());
                    iter++;
                }
                else
                {
                    sheet.erase(iter);
                }
            }
            selectSheet->SetSelectedObjects(sheet);
        }
        else if (block == isMove)
        {
            m_isMove = isMove->Value();
            if (m_isMove)
            {
                sheetMoveDistUI->SetShow(true);
                moveDirUI->SetShow(true);
            }
            else
            {
                sheetMoveDistUI->SetShow(false);
                moveDirUI->SetShow(false);
            }
        }
        else if (block == sheetMoveDistUI)
        {
            
        }
        else if (block == moveDirUI)
        {
            moveDir = moveDirUI->Vector();
        }
        else if (block == sheetOffsetDistUI)
        {
            sheetOffsetDist = sheetOffsetDistUI->Value();
        }
        else if (block == intervalDistUI)
        {
            intervalDist = intervalDistUI->Value();
        }
        else if (block == curvesSelect)
        {
            filterCurves();
        }
        else if (block == modlTypeUI)
        {
            m_isInitial = false;
             if (strncmp(modlTypeUI->ValueAsString().GetText(),"1",1)==0)
             {
                 toolTypeUI->SetShow(true);
                 
                 if (strncmp(toolTypeUI->ValueAsString().GetText(), "E", 1) == 0)
                 {
                     m_modlType = UETYPE;
                     setValueUE();
                 }
                 else
                 {
                     m_modlType = ULTYPE;
                     setValueUL();
                 }
             }
             else
             {
                 toolTypeUI->SetShow(false);
                 m_modlType = DLTYPE;
                 setValueDL();
             }

        }
        else if (block == toolTypeUI)
        {
            m_isInitial = false;
            if (strncmp(toolTypeUI->ValueAsString().GetText(), "E", 1) == 0)
            {
                m_modlType = UETYPE;
                setValueUE();
            }
            else
            {
                m_modlType = ULTYPE;
                setValueUL();
            }
        }
        else if (block == cutDeltaLenUI)
        {
            cutDirDeltaLen = cutDeltaLenUI->Value();
        }
        else if (block == toolHeightUI)
        {
            double buf = toolHeightUI->Value();
            if (buf >= 40.0f&&buf <= 45.0f)
            {
                toolHeight = buf;
                if (toolBreadth < (1.2*buf))
                {
                    toolBreadth = 1.2*buf;
                }
            }
        }
        else if (block == cutDirCutLenUI)
        {
            cutDirCutLen = cutDirCutLenUI->Value();
        }
        else if (block == cutDirToolBlankUI)
        {
            cutDirToolBlank = cutDirToolBlankUI->Value();
        }
        else if (block == cutShapeWideUI)
        {
            cutShapeWide = cutShapeWideUI->Value();
        }
        else if (block == cutShapeBlankUI)
        {
            cutShapeBlank = cutShapeBlankUI->Value();
        }
    }
    catch (exception& ex)
    {
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

void SteelInsertTrimming::setValue()
{
    getAllTrimCurves();
    m_isInitial = !m_isInitial;
    isMove->SetValue(m_isMove);

    sheetMoveDistUI->SetValue(sheetMoveDist);
    moveDirUI->SetVector(moveDir);
    //intervalDistUI->SetValue(intervalDist);

    cutDeltaLenUI->SetValue(cutDirDeltaLen);
    toolHeightUI->SetValue(toolHeight);

    cutDirCutLenUI->SetValue(cutDirCutLen);
    cutDirToolBlankUI->SetValue(cutDirToolBlank);
    cutShapeWideUI->SetValue(cutShapeWide);
    cutShapeBlankUI->SetValue(cutShapeBlank);
}

CommonOperation*  SteelInsertTrimming::retOper()
{
    getValue();
    return new CommonOperation(*this);
}

void SteelInsertTrimming::getValue()
{
    std::vector<NXOpen::TaggedObject *>&&tempSheets = selectSheet->GetSelectedObjects();
    std::vector<tag_t> sheets;
    for (int i = 0; i < tempSheets.size(); i++)
    {
        sheets.push_back(tempSheets[i]->GetTag());
    }
    m_isMove = isMove->Value();
    sheetMoveDist = sheetMoveDistUI->Value();
    moveDir = moveDirUI->Vector();

    sheetOffsetDist = sheetOffsetDistUI->Value();
    offsetDeviation = offsetDeviationUI->Value();
    stepoverDeviation = stepoverDeviationUI->Value();

    intervalDist = intervalDistUI->Value();
    cutDirCutLen = cutDirCutLenUI->Value();
    cutDirToolBlank = cutDirToolBlankUI->Value();
    cutShapeWide = cutShapeWideUI->Value();
    cutShapeBlank = cutShapeBlankUI->Value();
    cutDirDeltaLen = cutDeltaLenUI->Value();
    toolHeight = toolHeightUI->Value();

}

int SteelInsertTrimming::Show()
{
    try
    {
        theDialog->Show();
    }
    catch (exception& ex)
    {
        //---- Enter your exception handling code here -----
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}
void SteelInsertTrimming::dialogShown_cb()
{
    try
    {
        m_isInitial = false;
        setValueUL();
    }
    catch (exception& ex)
    {
        //---- Enter your exception handling code here -----
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}
int SteelInsertTrimming::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch (exception& ex)
    {
        //---- Enter your exception handling code here -----
        errorCode = 1;
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
PropertyList* SteelInsertTrimming::GetBlockProperties(const char *blockID)
{
    return theDialog->GetBlockProperties(blockID);
}

void SteelInsertTrimming::getAllTrimCurves()
{
    std::vector<NXOpen::TaggedObject *>  objectVector;
    CurveAttrE curveAttre;
    curveAttre.getTrimCurves();

    for (int i = 0; i < curveAttre().size(); i++)
    {
        for (int n = 0; n < curveAttre()[i].trim_->size(); n++)
        {
            objectVector.push_back(NXOpen::NXObjectManager::Get((*curveAttre()[i].trim_)[n]->curve));
        }
        for (int n = 0; n < curveAttre()[i].assist_->size(); n++)
        {
            objectVector.push_back(NXOpen::NXObjectManager::Get((*curveAttre()[i].assist_)[n]->curve));
        }
    }
    for (int i = 0; i < curveAttre(0).size(); i++)
    {
        for (int n = 0; n < curveAttre(0)[i].trim_->size(); n++)
        {
            objectVector.push_back(NXOpen::NXObjectManager::Get((*curveAttre(0)[i].trim_)[n]->curve));
        }
        for (int n = 0; n < curveAttre(0)[i].assist_->size(); n++)
        {
            objectVector.push_back(NXOpen::NXObjectManager::Get((*curveAttre(0)[i].assist_)[n]->curve));
        }
    }
    curvesSelect->SetSelectedObjects(objectVector);
}


extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
//     if (!getEncodeStatus(1))
//     {
//         MessageBox(::GetDesktopWindow(), _T("加密失败！"), _T("测试"), MB_OK);
//         return;
//     }
    SteelInsertTrimming *theUpperSteelInsertTrimming = NULL;
    try
    {
        theUpperSteelInsertTrimming = new SteelInsertTrimming();
        // The following method shows the dialog immediately
        theUpperSteelInsertTrimming->Show();
    }
    catch (exception& ex)
    {
        //---- Enter your exception handling code here -----
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
    if (theUpperSteelInsertTrimming != NULL)
    {
        delete theUpperSteelInsertTrimming;
        theUpperSteelInsertTrimming = NULL;
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
    catch (exception& ex)
    {
        //---- Enter your exception handling code here -----
        SteelInsertTrimming::theUI->NXMessageBox()->Show("Block Styler", NXOpen::NXMessageBox::DialogTypeError, ex.what());
    }
}
