// AssetsHandoverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "AssetsHandoverDlg.h"
#include "afxdialogex.h"

#include "CApplication0.h" //word�������   
#include "CDocuments.h" //�ĵ�������   
#include "CDocument0.h" //docx����   
#include "CSelection.h" //��ѡ����   
#include "CCell.h" //������Ԫ��   
#include "CCells.h" //��Ԫ�񼯺�   
#include "CRange0.h" //�ĵ��е�һ��������Χ   
#include "CTable0.h" //�������   
#include "CTables0.h" //��񼯺�   
#include "CRow.h" //������   
#include "CRows.h" //�м���   
#include "CBookmark0.h" //   
#include "CBookmarks.h" //


// CAssetsHandoverDlg �Ի���

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


// CAssetsHandoverDlg ��Ϣ�������

// �ύ����
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
        ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
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

    ::MessageBox(NULL, _T("�ύ�ɹ�"), _T("��ʾ"), MB_OK);
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

    // ��COleVariant��������ȽϷ���
    COleVariant covTrue((short)TRUE),
        covFalse((short)FALSE),
        covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

    // �õ� IDispatch ָ�벢���ӵ�objWord������.
    if (!objWord.CreateDispatch(_T("word.application"), NULL))
    {
        MessageBox(_T("Couldn''t get Word object."));
        return;
    }

    objWord.put_Visible(TRUE);  //��ʾ���򴰿�.

    CDocuments docs(objWord.get_Documents());
    CDocument0 testDoc;

    TCHAR szFilePath[MAX_PATH + 1];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;//ɾ���ļ�����ֻ���·��
    CString curDir = szFilePath;
    curDir += _T("\\�ʲ����ӱ���.docx");

    testDoc.AttachDispatch(docs.Open(
        COleVariant(curDir, VT_BSTR),
        covFalse,    // ȷ��ת��
        covFalse,    // ֻ��
        covFalse,    // ��ӵ�����ļ���
        covOptional, // �ĵ�����.
        covOptional, // ģ�����.
        covFalse,    // �ָ�ԭ״.
        covOptional, // д�ĵ�����.
        covOptional, // дģ�����.
        covOptional, // ��ʽ. // �ò������� Word 97 ��ǰ
        covOptional, // ���� // ���� Word 2000/2002
        covTrue,     // �ɼ�
        covOptional, // OpenConflictDocument
        covOptional, // �򿪲��޸�
        COleVariant((long)0),     // DocumentDirection wdDocumentDirection LeftToRight
        covOptional  // �ޱ���Ի���
    )  // Close Open parameters
    ); // Close AttachDispatch(��)

    //MessageBox(_T("��ʼ��ӡǰԤ�������ȷ����ʼ��ӡ��"));

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
                               // �����ϣ����ӡǰԤ���ĵ��������ʹ��PrintPreview����PrintOut
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
        ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
        return;
    }

    //TCHAR path[MAX_PATH];
    //GetCurrentDirectory(MAX_PATH, path); // �ļ�Ŀ¼������path����ַ�������
    //CString curDir(path);

    TCHAR szFilePath[MAX_PATH + 1];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;//ɾ���ļ�����ֻ���·��
    CString curDir = szFilePath;

    COleVariant covZero((short)0),
        covTrue((short)TRUE),
        covFalse((short)FALSE),
        covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR),
        covDocxType((short)0),
        start_line, end_line,
        dot(curDir + _T("\\����ģ��.dot"));

    CApplication0 wordApp;
    CDocuments docs;
    CDocument0 docx;
    CBookmarks bookmarks;
    CBookmark0 bookmark;
    CRange0 range;
    CCell cell;
    if (!wordApp.CreateDispatch(_T("Word.Application"))) {
        AfxMessageBox(_T("����û�а�װword��Ʒ��"));
        return;
    }

    wordApp.put_Visible(FALSE);
    CString wordVersion = wordApp.get_Version();
    docs = wordApp.get_Documents();
    docx = docs.Add(dot, covOptional, covOptional, covOptional);
    bookmarks = docx.get_Bookmarks();

    bookmark = bookmarks.Item(&_variant_t(_T("�豸����")));
    range = bookmark.get_Range();
    range.put_Text(strDeviceName);

    bookmark = bookmarks.Item(&_variant_t(_T("������")));
    range = bookmark.get_Range();
    range.put_Text(strOwner);

    bookmark = bookmarks.Item(&_variant_t(_T("��������")));
    range = bookmark.get_Range();
    range.put_Text(strRoomNum);

    bookmark = bookmarks.Item(&_variant_t(_T("���ʱ��")));
    range = bookmark.get_Range();
    range.put_Text(strInDate);

    bookmark = bookmarks.Item(&_variant_t(_T("����ʱ��")));
    range = bookmark.get_Range();
    range.put_Text(strOutDate);

    bookmark = bookmarks.Item(&_variant_t(_T("��״")));
    range = bookmark.get_Range();
    range.put_Text(strStatus);

    bookmark = bookmarks.Item(&_variant_t(_T("����˵��")));
    range = bookmark.get_Range();
    range.put_Text(strReason);

    CString strSavePath = curDir;
    strSavePath += _T("\\�ʲ����ӱ���.docx");
    docx.SaveAs(COleVariant(strSavePath), covOptional, covOptional, covOptional, covOptional,
        covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);

    // �˳�wordӦ��
    docx.Close(covFalse, covOptional, covOptional);
    wordApp.Quit(covOptional, covOptional, covOptional);
    range.ReleaseDispatch();
    bookmarks.ReleaseDispatch();
    wordApp.ReleaseDispatch();

    AfxMessageBox(_T("�ʲ����ӱ���.docx���ɳɹ���"));
}


BOOL CAssetsHandoverDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)    //����ESC  
            return TRUE;

        if (pMsg->wParam == VK_RETURN)    //���λس� 
            return TRUE;

        if (pMsg->wParam == VK_F1) 	//����F1������
            return TRUE;
    }

    if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) 	//����ALT+F4
        return TRUE;

    return CDialogEx::PreTranslateMessage(pMsg);
}
