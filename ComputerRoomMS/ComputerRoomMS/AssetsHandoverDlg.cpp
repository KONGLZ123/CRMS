// AssetsHandoverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "AssetsHandoverDlg.h"
#include "afxdialogex.h"

#include "CApplication0.h" //word程序对象   
#include "CDocuments.h" //文档集对象   
#include "CDocument0.h" //docx对象   
#include "CSelection.h" //所选内容   
#include "CCell.h" //单个单元格   
#include "CCells.h" //单元格集合   
#include "CRange0.h" //文档中的一个连续范围   
#include "CTable0.h" //单个表格   
#include "CTables0.h" //表格集合   
#include "CRow.h" //单个行   
#include "CRows.h" //行集合   
#include "CBookmark0.h" //   
#include "CBookmarks.h" //


// CAssetsHandoverDlg 对话框

IMPLEMENT_DYNAMIC(CAssetsHandoverDlg, CDialogEx)

CAssetsHandoverDlg::CAssetsHandoverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ASSETS_HANDOVER, pParent)
{

}

CAssetsHandoverDlg::~CAssetsHandoverDlg()
{
}

void CAssetsHandoverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAssetsHandoverDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON4, &CAssetsHandoverDlg::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BTN_PRINTER, &CAssetsHandoverDlg::OnBnClickedBtnPrinter)
    ON_BN_CLICKED(IDC_BTN_EXPORT_WORD, &CAssetsHandoverDlg::OnBnClickedBtnExportWord)
END_MESSAGE_MAP()


// CAssetsHandoverDlg 消息处理程序

// 提交报告
void CAssetsHandoverDlg::OnBnClickedButton4()
{
    CString strDeviceName;
    CString strOwner;
    CString strRoomNum;
    CString strInDate;
    CString strOutDate;
    CString strReason;
    CString strStatus;

    GetDlgItem(IDC_EDIT_DEVICE_NAME)->GetWindowText(strDeviceName);
    GetDlgItem(IDC_EDIT_OWNER)->GetWindowText(strOwner);
    GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_IN_DATE)->GetWindowText(strInDate);
    GetDlgItem(IDC_EDIT_OUT_DATE)->GetWindowText(strOutDate);
    GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strStatus);
    GetDlgItem(IDC_EDIT_REASON11)->GetWindowText(strReason);

    if (strDeviceName.IsEmpty() ||
        strOwner.IsEmpty() ||
        strRoomNum.IsEmpty() ||
        strInDate.IsEmpty() ||
        strOutDate.IsEmpty() ||
        strReason.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        return;
    }

    ASSERT_DATA *pReportData = new ASSERT_DATA;
    pReportData->strDeviceName = strDeviceName;
    pReportData->strOwner = strOwner;
    pReportData->strRoomNum = strRoomNum;
    pReportData->strInDate = strInDate;
    pReportData->strOutDate = strOutDate;
    pReportData->strStatus = strStatus;
    pReportData->strReason = strReason;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_INSERT_ASSERT_DATA, (WPARAM)pReportData, 0);

    ::MessageBox(NULL, _T("提交成功"), _T("提示"), MB_OK);
}


void CAssetsHandoverDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}


void CAssetsHandoverDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}



void CAssetsHandoverDlg::OnBnClickedBtnPrinter()
{
    CApplication0 objWord;

    // 用COleVariant定义变量比较方便
    COleVariant covTrue((short)TRUE),
        covFalse((short)FALSE),
        covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

    // 得到 IDispatch 指针并附加到objWord对象中.
    if (!objWord.CreateDispatch(_T("word.application"), NULL))
    {
        MessageBox(_T("Couldn''t get Word object."));
        return;
    }

    objWord.put_Visible(TRUE);  //显示程序窗口.

    CDocuments docs(objWord.get_Documents());
    CDocument0 testDoc;

    TCHAR szFilePath[MAX_PATH + 1];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径
    CString curDir = szFilePath;
    curDir += _T("\\资产交接报告.docx");

    testDoc.AttachDispatch(docs.Open(
        COleVariant(curDir, VT_BSTR),
        covFalse,    // 确认转换
        covFalse,    // 只读
        covFalse,    // 添加到最近文件中
        covOptional, // 文档口令.
        covOptional, // 模板口令.
        covFalse,    // 恢复原状.
        covOptional, // 写文档口令.
        covOptional, // 写模板口令.
        covOptional, // 格式. // 该参数用于 Word 97 以前
        covOptional, // 编码 // 用于 Word 2000/2002
        covTrue,     // 可见
        covOptional, // OpenConflictDocument
        covOptional, // 打开并修复
        COleVariant((long)0),     // DocumentDirection wdDocumentDirection LeftToRight
        covOptional  // 无编码对话框
    )  // Close Open parameters
    ); // Close AttachDispatch(…)

    //MessageBox(_T("开始打印前预览，点击确定开始打印！"));

    testDoc.PrintPreview();

    testDoc.PrintOut(covFalse,              // Background.
        covOptional,           // Append.
        covOptional,           // Range.
        covOptional,           // OutputFileName.
        covOptional,           // From.
        covOptional,           // To.
        covOptional,           // Item.
        COleVariant((long)2),  // Copies.
        covOptional,           // Pages.
        covOptional,           // PageType.
        covOptional,           // PrintToFile.
        covOptional,           // Collate.
        covOptional,           // ActivePrinterMacGX.
        covOptional,           // ManualDuplexPrint.
        covOptional,           // PrintZoomColumn  New with Word 2002
        covOptional,           // PrintZoomRow          ditto
        covOptional,           // PrintZoomPaperWidth   ditto
        covOptional);          // PrintZoomPaperHeight  ditto
                               // 如果你希望打印前预览文档，你可以使用PrintPreview代替PrintOut
                               // testDoc[i].PrintPreview.

    objWord.Quit(covFalse,  // SaveChanges.
        covTrue,   // OriginalFormat.
        covFalse   // RouteDocument.
    );
}


void CAssetsHandoverDlg::OnBnClickedBtnExportWord()
{
    CString strDeviceName;
    CString strOwner;
    CString strRoomNum;
    CString strInDate;
    CString strOutDate;
    CString strReason;
    CString strStatus;

    GetDlgItem(IDC_EDIT_DEVICE_NAME)->GetWindowText(strDeviceName);
    GetDlgItem(IDC_EDIT_OWNER)->GetWindowText(strOwner);
    GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_IN_DATE)->GetWindowText(strInDate);
    GetDlgItem(IDC_EDIT_OUT_DATE)->GetWindowText(strOutDate);
    GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strStatus);
    GetDlgItem(IDC_EDIT_REASON11)->GetWindowText(strReason);

    if (strDeviceName.IsEmpty() ||
        strOwner.IsEmpty() ||
        strRoomNum.IsEmpty() ||
        strInDate.IsEmpty() ||
        strOutDate.IsEmpty() ||
        strReason.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        return;
    }

    //TCHAR path[MAX_PATH];
    //GetCurrentDirectory(MAX_PATH, path); // 文件目录保存在path这个字符数组中
    //CString curDir(path);

    TCHAR szFilePath[MAX_PATH + 1];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径
    CString curDir = szFilePath;

    COleVariant covZero((short)0),
        covTrue((short)TRUE),
        covFalse((short)FALSE),
        covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR),
        covDocxType((short)0),
        start_line, end_line,
        dot(curDir + _T("\\报告模板.dot"));

    CApplication0 wordApp;
    CDocuments docs;
    CDocument0 docx;
    CBookmarks bookmarks;
    CBookmark0 bookmark;
    CRange0 range;
    CCell cell;
    if (!wordApp.CreateDispatch(_T("Word.Application"))) {
        AfxMessageBox(_T("本机没有安装word产品！"));
        return;
    }

    wordApp.put_Visible(FALSE);
    CString wordVersion = wordApp.get_Version();
    docs = wordApp.get_Documents();
    docx = docs.Add(dot, covOptional, covOptional, covOptional);
    bookmarks = docx.get_Bookmarks();

    bookmark = bookmarks.Item(&_variant_t(_T("设备名称")));
    range = bookmark.get_Range();
    range.put_Text(strDeviceName);

    bookmark = bookmarks.Item(&_variant_t(_T("持有者")));
    range = bookmark.get_Range();
    range.put_Text(strOwner);

    bookmark = bookmarks.Item(&_variant_t(_T("所属机房")));
    range = bookmark.get_Range();
    range.put_Text(strRoomNum);

    bookmark = bookmarks.Item(&_variant_t(_T("入库时间")));
    range = bookmark.get_Range();
    range.put_Text(strInDate);

    bookmark = bookmarks.Item(&_variant_t(_T("出库时间")));
    range = bookmark.get_Range();
    range.put_Text(strOutDate);

    bookmark = bookmarks.Item(&_variant_t(_T("现状")));
    range = bookmark.get_Range();
    range.put_Text(strStatus);

    bookmark = bookmarks.Item(&_variant_t(_T("其他说明")));
    range = bookmark.get_Range();
    range.put_Text(strReason);

    CString strSavePath = curDir;
    strSavePath += _T("\\资产交接报告.docx");
    docx.SaveAs(COleVariant(strSavePath), covOptional, covOptional, covOptional, covOptional,
        covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);

    // 退出word应用
    docx.Close(covFalse, covOptional, covOptional);
    wordApp.Quit(covOptional, covOptional, covOptional);
    range.ReleaseDispatch();
    bookmarks.ReleaseDispatch();
    wordApp.ReleaseDispatch();

    AfxMessageBox(_T("资产交接报告.docx生成成功！"));
}


BOOL CAssetsHandoverDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)    //屏蔽ESC  
            return TRUE;

        if (pMsg->wParam == VK_RETURN)    //屏蔽回车 
            return TRUE;

        if (pMsg->wParam == VK_F1) 	//屏蔽F1帮助键
            return TRUE;
    }

    if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) 	//屏蔽ALT+F4
        return TRUE;

    return CDialogEx::PreTranslateMessage(pMsg);
}
