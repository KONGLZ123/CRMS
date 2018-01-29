
// SnifferDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sniffer.h"
#include "SnifferDlg.h"
#include "afxdialogex.h"
/* 网络层协议类型 */
#define IP       0x0800          
#define ARP      0x0806          
#define RARP     0x8035 

/* 传输层类型 */
#define ICMP       0x01
#define IGMP       0x02 
#define TCP        0x06
#define EGP        0x08   
#define UDP        0x11 
#define IPv6       0x29
#define OSPF       0x59

/* 应用层类型 */
#define HTTP       0x50
#define DNS        0x35 

/* 6字节的MAC地址 */
typedef struct ethernet_address
{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
    u_char byte5;
    u_char byte6;
}ethernet_address;

/* 以太网帧首部 */
typedef struct ethernet_header
{
    ethernet_address daddr;		// 目的MAC地址
    ethernet_address saddr;		// 源MAC地址
    u_short type;				// 协议类型
}ethernet_header;



/* 4字节的IP地址 */
typedef struct ip_address
{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 首部 */
typedef struct ip_header
{
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service) 
    u_short tlen;           // 总长(Total length) 
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 生存时间(Time to live)
    u_char  type;           // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    ip_address  saddr;      // 源地址(Source address)
    ip_address  daddr;      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
}ip_header;

/* ARP 首部 */
typedef struct arp_header
{

    u_short arp_hdr;				//硬件类型：指明了发送方想知道的硬件接口类型，以太网的值为1
    u_short arp_pro;				//协议类型：指明了发送方提供的高层协议类型，IP为0800（16进制）
    u_char arp_hln;					//硬件长度，8位字段，定义对应物理地址长度，以太网中这个值为6
    u_char apr_pln;					//协议长度，8位字段，定义以字节为单位的逻辑地址长度，对IPV4协议这个值为4
    u_short arp_opt;				//操作类型：用来表示这个报文的类型，ARP请求为1，ARP响应为2，RARP请求为3，RARP响应为4
    ethernet_address arp_smac;		//发送端硬件地址，可变长度字段，对以太网这个字段是6字节长
    ip_address arp_sip;				//发送端协议地址，可变长度字段，对IP协议，这个字段是4字节长
    ethernet_address arp_dmac;		//接受端硬件地址
    ip_address arp_dip;				//接收端协议地址
}arp_header;

/* TCP 首部*/
typedef struct tcp_header
{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_int seq;              // 顺序号
    u_int ack;              // 确认号
    u_char len;				// TCP头部长度，数据偏移单位是4字节，这里只用前4位
    u_char flags;			// 后6位分别为：URG，ACK，PSH，RST，SYN，FIN
    u_short win;			// 窗口大小
    u_short crc;			// 校验和
    u_short urp;			// 紧急指针
}tcp_header;

/* UDP 首部*/
typedef struct udp_header
{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP报头和UDP数据的长度
    u_short crc;            // 校验和(Checksum)
}udp_header;

/* ICMP 首部*/
typedef struct icmp_header
{
    u_char type;			//类型
    u_char code;			//代码
    u_short checksum;		//校验和
}icmp_header;

/* DNS 首部*/
typedef struct dns_header
{
    u_short identification;     // 标识
    u_short flags;				// 标志
    u_short questions_num;      // 问题数
    u_short answers_num;        // 资源记录数
    u_short authority_num;      //授权资源记录数
    u_short addition_num;		//额外资源记录数
}dns_header;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI CapturePacket(LPVOID lpParam)
{
    CSnifferDlg *pDlg = (CSnifferDlg *)lpParam;
    pcap_t *pCap;
    char    strErrorBuf[PCAP_ERRBUF_SIZE];
    int res;
    struct pcap_pkthdr *pkt_header;
    const u_char *pkt_data;
    u_int netmask;
    struct bpf_program fcode;

    if ((pCap = pcap_open_live(pDlg->m_pDevice->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, strErrorBuf)) == NULL)
    {
        AfxMessageBox(_T("请选择要绑定的网卡"));
        return -1;
    }

    if (pDlg->m_pDevice->addresses != NULL)
        /* 获得接口第一个地址的掩码 */
        netmask = ((struct sockaddr_in *)(pDlg->m_pDevice->addresses->netmask))->sin_addr.S_un.S_addr;
    else
        /* 如果接口没有地址，那么我们假设一个C类的掩码 */
        netmask = 0xffffff;
    //编译过滤器
    if (pcap_compile(pCap, &fcode, pDlg->m_filtername, 1, netmask) <0)
    {
        AfxMessageBox(_T("请设置过滤规则"));
        return -1;
    }
    //设置过滤器
    if (pcap_setfilter(pCap, &fcode)<0)
        return -1;

    while ((res = pcap_next_ex(pCap, &pkt_header, &pkt_data)) >= 0)
    {

        if (res == 0)
            continue;
        if (!pDlg->m_bFlag)
            break;
        CSnifferDlg *pDlg = (CSnifferDlg *)AfxGetApp()->GetMainWnd();
        pDlg->ShowPacketList(pkt_header, pkt_data);
        pDlg = NULL;
    }

    pcap_close(pCap);
    pDlg = NULL;
    return 1;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSnifferDlg 对话框



CSnifferDlg::CSnifferDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SNIFFER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_list1);
    DDX_Control(pDX, IDC_TREE1, m_tree1);
    DDX_Text(pDX, IDC_EDIT3, m_tcpnum);
    DDX_Text(pDX, IDC_EDIT2, m_udpnum);
    DDX_Text(pDX, IDC_EDIT6, m_arpnum);
    DDX_Text(pDX, IDC_EDIT8, m_icmpnum);
    DDX_Text(pDX, IDC_EDIT5, m_igmpnum);
    DDX_Text(pDX, IDC_EDIT4, m_httpnum);
    DDX_Text(pDX, IDC_EDIT7, m_dnsnum);
    DDX_Control(pDX, IDC_EDIT1, m_edit1);
    DDX_Text(pDX, IDC_EDIT9, m_qqnum);
    DDX_Text(pDX, IDC_EDIT10, m_bullnum);
}

BEGIN_MESSAGE_MAP(CSnifferDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_ADP, &CSnifferDlg::OnAdp)
    ON_COMMAND(ID_FILTER, &CSnifferDlg::OnFilter)
    ON_COMMAND(ID_START, &CSnifferDlg::OnStart)
    ON_COMMAND(ID_STOP, &CSnifferDlg::OnStop)
END_MESSAGE_MAP()


// CSnifferDlg 消息处理程序

BOOL CSnifferDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    m_list1.SetExtendedStyle(m_list1.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 为列表视图控件添加全行选中和栅格风格
    m_list1.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 50);
    m_list1.InsertColumn(1, _T("时间"), LVCFMT_CENTER, 120);
    m_list1.InsertColumn(2, _T("源MAC地址"), LVCFMT_CENTER, 120);
    m_list1.InsertColumn(3, _T("目的MAC地址"), LVCFMT_CENTER, 120);
    m_list1.InsertColumn(4, _T("长度"), LVCFMT_CENTER, 50);
    m_list1.InsertColumn(5, _T("协议"), LVCFMT_CENTER, 70);
    m_list1.InsertColumn(6, _T("源IP地址"), LVCFMT_CENTER, 120);
    m_list1.InsertColumn(7, _T("目的IP地址"), LVCFMT_CENTER, 120);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnifferDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSnifferDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSnifferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSnifferDlg::OnAdp()
{
    CAdpDlg adpdlg;
    if (adpdlg.DoModal() == IDOK)
        m_pDevice = adpdlg.returnd();
}


void CSnifferDlg::OnFilter()
{
    CFilterDlg filterdlg;
    if (filterdlg.DoModal() == IDOK)
    {
        int len =WideCharToMultiByte(CP_ACP,0,filterdlg.GetFilterName(),-1,NULL,0,NULL,NULL); 
        WideCharToMultiByte(CP_ACP,0,filterdlg.GetFilterName(),-1,m_filtername,len,NULL,NULL );
        //m_filtername = filterdlg.GetFilterName();
    }
    UpdateData(FALSE);
}

void CSnifferDlg::ShowPacketList(pcap_pkthdr * pkt_header, const u_char * pkt_data)
{
    //用于存诸网络中的数据，并保存到CArray中,以备将来使用
    struct pcap_pkthdr *pHeader = new pcap_pkthdr;
    u_char *pData;
    unsigned char *pPosition = (unsigned char *)pkt_data;

    long nIndex = 0;//标识当前的数据包位置
    long nCount = 0;//标识后来

    pHeader->caplen = pkt_header->caplen;
    pHeader->len = pkt_header->len;

    pData = new unsigned char[pHeader->len];
    memcpy((void *)pData, pkt_data, pHeader->len);

    m_pktHeaders.Add(pHeader);
    m_pktDatas.Add(pData);

    nIndex = m_pktHeaders.GetSize() - 1;
    CString str;
    str.Format(_T("%d"), nIndex);
    nCount = m_list1.InsertItem(nIndex, str, 0);

    /*显示时间*/
    struct tm *ltime = NULL;
    time_t local_tv_sec;
    local_tv_sec = pkt_header->ts.tv_sec;
    //ltime = localtime(&local_tv_sec);
    localtime_s(ltime, &local_tv_sec);
    str.Format(_T("%d:%d:%d.%.6d"), ltime->tm_hour, ltime->tm_min, ltime->tm_sec, pkt_header->ts.tv_usec);
    m_list1.SetItemText(nCount, 1, str);
    /*处理链路层*/
    ethernet_header *eh;
    eh = (ethernet_header *)pkt_data;
    str.Format(_T("%x:%x:%x:%x:%x:%x"), eh->saddr.byte1, eh->saddr.byte2, eh->saddr.byte3, eh->saddr.byte4, eh->saddr.byte5, eh->saddr.byte6);

    m_list1.SetItemText(nCount, 2, str);
    str.Format(_T("%x:%x:%x:%x:%x:%x"), eh->daddr.byte1, eh->daddr.byte2, eh->daddr.byte3, eh->daddr.byte4, eh->daddr.byte5, eh->daddr.byte6);

    m_list1.SetItemText(nCount, 3, str);
    str.Format(_T("%ld"), pHeader->len);

    m_list1.SetItemText(nCount, 4, str);
    /*处理网络层*/
    switch (ntohs(eh->type))
    {
    case IP:
    {
        ip_header *ih;
        const u_char *ip_data;
        ip_data = pkt_data + 14;
        ih = (ip_header *)ip_data;
        u_int ip_len;//IP首部长度

        ip_len = (ih->ver_ihl & 0xf) * 4;
        //显示源ip
        str.Format(_T("%d.%d.%d.%d"), ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);
        m_list1.SetItemText(nCount, 6, str);
        //显示目的ip
        str.Format(_T("%d.%d.%d.%d"), ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
        m_list1.SetItemText(nCount, 7, str);

        /*处理传输层*/
        switch (ih->type)
        {
        case TCP:
        {
            m_tcpCount++;
            tcp_header *th;
            const u_char *tcp_data;
            tcp_data = ip_data + ip_len;
            th = (tcp_header *)tcp_data;
            if (ntohs(th->sport) == HTTP)
            {
                m_list1.SetItemText(nCount, 5, _T("源HTTP"));
                m_httpCount++;
            }
            else if (ntohs(th->dport) == HTTP)
            {
                m_list1.SetItemText(nCount, 5, _T("目的HTTP"));
                m_httpCount++;
            }
            else
                m_list1.SetItemText(nCount, 5, _T("TCP"));
            break;
        }
        case UDP:
        {
            m_udpCount++;
            udp_header *uh;
            const u_char *udp_data;
            udp_data = ip_data + ip_len;
            uh = (udp_header *)udp_data;
            m_list1.SetItemText(nCount, 5, _T("UDP"));
            break;
        }
        case ICMP:m_icmpCount++; m_list1.SetItemText(nCount, 5, _T("ICMP")); break;
        default:m_list1.SetItemText(nCount, 5, _T("未知"));
        }
        break;
    }
    case ARP:
    {
        m_arpCount++;
        arp_header *ah;
        const u_char *arp_data;
        arp_data = pkt_data + 14;
        ah = (arp_header *)arp_data;
        str.Format(_T("%d.%d.%d.%d"), ah->arp_sip.byte1, ah->arp_sip.byte2, ah->arp_sip.byte3, ah->arp_sip.byte4);
        m_list1.SetItemText(nCount, 6, str);
        str.Format(_T("%d.%d.%d.%d"), ah->arp_dip.byte1, ah->arp_dip.byte2, ah->arp_dip.byte3, ah->arp_dip.byte4);
        m_list1.SetItemText(nCount, 7, str);
        m_list1.SetItemText(nCount, 5, _T("ARP"));
        break;
    }
    default:
        m_list1.SetItemText(nCount, 5, _T("未知协议"));
    }
    ShowPckNum();
}

//协议树
void CSnifferDlg::ShowPacketTree(const pcap_pkthdr *pkt_header, const u_char *pkt_data, long index)
{
    m_tree1.DeleteAllItems();
    CString str;
    str.Format(_T("数据包:%ld"), index);
    HTREEITEM hRoot;
    HTREEITEM hSubItem;
    HTREEITEM hItem;
    HTREEITEM hItem2;

    hRoot = m_tree1.InsertItem(str);
    hSubItem = m_tree1.InsertItem(_T("数据链路层"), hRoot);
    ethernet_header *eh;
    eh = (ethernet_header *)pkt_data;
    str.Format(_T("源MAC:%x:%x:%x:%x:%x:%x"), eh->saddr.byte1, eh->saddr.byte2, eh->saddr.byte3, eh->saddr.byte4, eh->saddr.byte5, eh->saddr.byte6);
    hItem = m_tree1.InsertItem(str, hSubItem);
    str.Format(_T("目的MAC:%x:%x:%x:%x:%x:%x"), eh->daddr.byte1, eh->daddr.byte2, eh->daddr.byte3, eh->daddr.byte4, eh->daddr.byte5, eh->daddr.byte6);
    hItem = m_tree1.InsertItem(str, hSubItem);
    switch (ntohs(eh->type))
    {
    case IP:
    {
        hItem = m_tree1.InsertItem(_T("上层协议:IP"), hSubItem);
        hSubItem = m_tree1.InsertItem(_T("网络层"), hRoot);
        ip_header *ih;
        const u_char *ip_data;
        ip_data = pkt_data + 14;
        ih = (ip_header *)ip_data;
        str.Format(_T("版本：%d"), (ih->ver_ihl & 0xf0) / 0x10);
        hItem = m_tree1.InsertItem(str, hSubItem);
        u_int ip_len;//IP首部长度
        ip_len = (ih->ver_ihl & 0xf) * 4;
        str.Format(_T("首部长度：%d"), ip_len);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("服务类型：0x%x"), ih->tos);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("总长度：%d"), ntohs(ih->tlen));
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("标识：0x%x"), ntohs(ih->identification));
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("标志：0x%x"), ntohs(ih->flags_fo) & 0xe000 / 0x2000);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("片偏移：%d"), ntohs(ih->flags_fo) & 0x1fff);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("生存时间：%d"), ih->ttl);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("首部校验和：0x%x"), ntohs(ih->crc));
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("源IP地址：%d.%d.%d.%d"), ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("目的IP地址：%d.%d.%d.%d"), ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3, ih->saddr.byte4);
        hItem = m_tree1.InsertItem(str, hSubItem);
        switch (ih->type)
        {
        case TCP:
        {
            hItem = m_tree1.InsertItem(_T("上层协议:TCP"), hSubItem);
            hSubItem = m_tree1.InsertItem(_T("传输层"), hRoot);
            tcp_header *th;
            const u_char *tcp_data;
            tcp_data = ip_data + ip_len;
            th = (tcp_header *)tcp_data;
            str.Format(_T("源端口号：%d"), ntohs(th->sport));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("目的口号：%d"), ntohs(th->dport));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("顺序号：%d"), ntohs(th->seq));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("确认号：%d"), ntohs(th->ack));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("TCP头长：%d"), (th->len & 0xf0) / 0x10 * 4);
            hItem = m_tree1.InsertItem(str, hSubItem);
            hItem = m_tree1.InsertItem(_T("控制位"), hSubItem);
            str.Format(_T("紧急URG:%d"), (th->flags & 0x20) / 0x20);
            hItem2 = m_tree1.InsertItem(str, hItem);
            str.Format(_T("确认ACK:%d"), (th->flags & 0x10) / 0x10);
            hItem2 = m_tree1.InsertItem(str, hItem);
            str.Format(_T("推送PSH:%d"), (th->flags & 0x08) / 0x08);
            hItem2 = m_tree1.InsertItem(str, hItem);
            str.Format(_T("复位RSTG:%d"), (th->flags & 0x04) / 0x04);
            hItem2 = m_tree1.InsertItem(str, hItem);
            str.Format(_T("同步SYN:%d"), (th->flags & 0x02) / 0x02);
            hItem2 = m_tree1.InsertItem(str, hItem);
            str.Format(_T("结束FIN:%d"), (th->flags & 0x01) / 0x01);
            hItem2 = m_tree1.InsertItem(str, hItem);
            str.Format(_T("窗口：%d"), ntohs(th->win));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("校验和：0x%x"), ntohs(th->crc));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("紧急指针：0x%x"), ntohs(th->urp));
            hItem = m_tree1.InsertItem(str, hSubItem);
            break;
        }
        case UDP:
        {
            hItem = m_tree1.InsertItem(_T("上层协议:UDP"), hSubItem);
            hSubItem = m_tree1.InsertItem(_T("传输层"), hRoot);
            udp_header *uh;
            const u_char *udp_data;
            udp_data = ip_data + ip_len;
            uh = (udp_header *)udp_data;
            str.Format(_T("源端口号：%d"), ntohs(uh->sport));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("目的口号：%d"), ntohs(uh->dport));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("长度：%d"), ntohs(uh->len));
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("校验和：0x%x"), ntohs(uh->crc));
            hItem = m_tree1.InsertItem(str, hSubItem);
            if (ntohs(uh->dport) == DNS || ntohs(uh->sport) == DNS)
            {
                hSubItem = m_tree1.InsertItem(_T("应用层"), hRoot);
                dns_header *dh;
                const u_char *dns_data;
                dns_data = udp_data + 8;
                dh = (dns_header *)dns_data;
                str.Format(_T("标识：0x%x"), ntohs(dh->identification));
                hItem = m_tree1.InsertItem(str, hSubItem);
                str.Format(_T("标志：0x%x"), ntohs(dh->flags));
                hItem = m_tree1.InsertItem(str, hSubItem);
                str.Format(_T("问题数：%d"), ntohs(dh->questions_num));
                hItem = m_tree1.InsertItem(str, hSubItem);
                str.Format(_T("资源记录数：%d"), ntohs(dh->answers_num));
                hItem = m_tree1.InsertItem(str, hSubItem);
                str.Format(_T("授权资源记录数：%d"), ntohs(dh->authority_num));
                hItem = m_tree1.InsertItem(str, hSubItem);
                str.Format(_T("额外资源记录数：%d"), ntohs(dh->addition_num));
                hItem = m_tree1.InsertItem(str, hSubItem);
            }
            break;
        }
        case ICMP:
        {
            hItem = m_tree1.InsertItem(_T("上层协议:ICMP"), hSubItem);
            hSubItem = m_tree1.InsertItem(_T("传输层"), hRoot);
            icmp_header *icmph;
            const u_char *icmp_data;
            icmp_data = ip_data + ip_len;
            icmph = (icmp_header *)icmp_data;
            str.Format(_T("类型：%d"), icmph->type);
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("代码：%d"), icmph->code);
            hItem = m_tree1.InsertItem(str, hSubItem);
            str.Format(_T("校验和：0x%x"), ntohs(icmph->checksum));
            hItem = m_tree1.InsertItem(str, hSubItem);
            break;
        }
        case IGMP:hItem = m_tree1.InsertItem(_T("上层协议:IGMP"), hSubItem); break;
        case EGP:hItem = m_tree1.InsertItem(_T("上层协议:EGP"), hSubItem); break;
        case IPv6:hItem = m_tree1.InsertItem(_T("上层协议:IPv6"), hSubItem); break;
        case OSPF:hItem = m_tree1.InsertItem(_T("上层协议:OSPF"), hSubItem); break;
        default:hItem = m_tree1.InsertItem(_T("上层协议:未知"), hSubItem);
        }
        break;
    }
    case ARP:
    {
        hItem = m_tree1.InsertItem(_T("上层协议:ARP"), hSubItem);
        hSubItem = m_tree1.InsertItem(_T("网络层"), hRoot);
        arp_header *ah;
        const u_char *arp_data;
        arp_data = pkt_data + 14;
        ah = (arp_header *)arp_data;
        str.Format(_T("硬件类型：%d"), ntohs(ah->arp_hdr));
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("协议类型：0x%x"), ntohs(ah->arp_pro));
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("硬件长度：%d"), ah->arp_hln);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("协议长度：%d"), ah->apr_pln);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("操作类型：%d"), ntohs(ah->arp_opt));
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("发送端MAC地址：%x:%x:%x:%x:%x:%x"), ah->arp_smac.byte1, ah->arp_smac.byte2, ah->arp_smac.byte3, ah->arp_smac.byte4, ah->arp_smac.byte5, ah->arp_smac.byte6);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("发送端协议地址：%d.%d.%d.%d"), ah->arp_sip.byte1, ah->arp_sip.byte2, ah->arp_sip.byte3, ah->arp_sip.byte4);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("接收端MAC地址：%x:%x:%x:%x:%x:%x"), ah->arp_dmac.byte1, ah->arp_dmac.byte2, ah->arp_dmac.byte3, ah->arp_dmac.byte4, ah->arp_dmac.byte5, ah->arp_dmac.byte6);
        hItem = m_tree1.InsertItem(str, hSubItem);
        str.Format(_T("接收端协议地址：%d.%d.%d.%d"), ah->arp_dip.byte1, ah->arp_dip.byte2, ah->arp_dip.byte3, ah->arp_dip.byte4);
        hItem = m_tree1.InsertItem(str, hSubItem);
        break;
    }
    default:
        hItem = m_tree1.InsertItem(_T("上层协议:未知"), hSubItem);
    }

    m_tree1.Expand(hRoot, TVE_EXPAND);		//默认展开目录
    m_tree1.Expand(hSubItem, TVE_EXPAND);

    CString strHex;
    int nCount = 0;
    CString strText;
    for (unsigned short i = 0; i < pkt_header->caplen; i++)
    {
        CString hex;
        if ((i % 16) == 0)
        {
            hex.Format(_T("\x0d\x0a 0X%04x   "), nCount);
            nCount++;
            if (i != 0)
            {
                strHex += _T("  ") + strText;
                strText = _T("");
            }
            strHex += hex;
        }
        hex.Format(_T("%2.2x "), pkt_data[i - 1]);
        strHex += hex;
        if (pkt_data[i - 1] <= 127 && pkt_data[i - 1] >= 0)
            hex.Format(_T("%c"), pkt_data[i - 1]);
        else
            hex = _T(".");
        strText += hex;
    }
    if (strText != _T(""))
        strHex += strText;
    m_edit1.SetWindowText(strHex);
}

void CSnifferDlg::ShowPckNum()
{
    m_tcpnum.Format(_T("%ld"), m_tcpCount);
    this->SetDlgItemText(IDC_EDIT2, m_tcpnum);

    m_udpnum.Format(_T("%ld"), m_udpCount);
    this->SetDlgItemText(IDC_EDIT3, m_udpnum);

    m_arpnum.Format(_T("%ld"), m_arpCount);
    this->SetDlgItemText(IDC_EDIT4, m_arpnum);

    m_icmpnum.Format(_T("%ld"), m_icmpCount);
    this->SetDlgItemText(IDC_EDIT5, m_icmpnum);

    m_httpnum.Format(_T("%ld"), m_httpCount);
    this->SetDlgItemText(IDC_EDIT7, m_httpnum);
}





void CSnifferDlg::OnStart()
{
    if (m_pDevice == NULL)
    {
        AfxMessageBox(_T("请选择要绑定的网卡"));
        return;
    }
    m_bFlag = true;
    //启动线程开始抓包
    CreateThread(NULL, NULL, CapturePacket, (LPVOID)this, true, NULL);
}


void CSnifferDlg::OnStop()
{
    m_bFlag = false;
}
