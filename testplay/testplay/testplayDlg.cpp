
// testplayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testplay.h"
#include "testplayDlg.h"
#include "afxdialogex.h"

#define __STDC_CONSTANT_MICROS

extern "C"
{
#include "SDL2/sdl.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "SDL2/sdl.h"
}

#define DISPLAY_VIDEO       (SDL_USEREVENT + 1)
#define BREAD_DISPALY       (SDL_USEREVENT + 2)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CtestplayDlg 对话框



CtestplayDlg::CtestplayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTPLAY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestplayDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PATH, m_url);
    DDX_Control(pDX, IDC_STATIC_VIDEO, m_videoWnd);
}

BEGIN_MESSAGE_MAP(CtestplayDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_PLAY, &CtestplayDlg::OnBnClickedBtnPlay)
    ON_BN_CLICKED(IDC_BTN_STOP, &CtestplayDlg::OnBnClickedBtnStop)
    ON_BN_CLICKED(IDC_BTN_BREAK, &CtestplayDlg::OnBnClickedBtnBreak)
    ON_BN_CLICKED(IDC_BTN_OPEN, &CtestplayDlg::OnBnClickedBtnOpen)
END_MESSAGE_MAP()


// CtestplayDlg 消息处理程序

BOOL CtestplayDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestplayDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CtestplayDlg::OnPaint()
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
HCURSOR CtestplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int exitCode = 0;
int VideoShowThread(void *data)
{
    SDL_Event event;
    while (!exitCode) {
        event.type = DISPLAY_VIDEO;
        SDL_PushEvent(&event);
        SDL_Delay(40);
    }

    event.type = BREAD_DISPALY;
    SDL_PushEvent(&event);

    return 0;
}

UINT ffmpegPlayer(LPVOID lpParam)
{
    CtestplayDlg *pDlg = static_cast<CtestplayDlg *>(lpParam);

    av_register_all();
    avformat_network_init();

    AVFormatContext *formatCtx = avformat_alloc_context();
    const char *filename = "Titanic.ts";
    FILE * fileVideoInfo = NULL;

    // 创建文件，记录视频信息
    fopen_s(&fileVideoInfo, "videoInfo.txt", "wb+");
    if (NULL == fileVideoInfo) {
        printf("Create file error\n");
        return -1;
    }

    // 打开视频输入流
    if (0 != avformat_open_input(&formatCtx, filename, NULL, NULL)) {
        printf("Open input error\n");
        return -1;
    }

    // 获取视频流信息
    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
        printf("Find stream info error\n");
        return -1;
    }

    AVInputFormat *inputFormat = formatCtx->iformat;

    int index = 0;
    // 判断那一路是视频流
    for (unsigned int i = 0; i < formatCtx->nb_streams; ++i) {
        if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            index = i;
            break;
        }
    }

    fprintf(fileVideoInfo, "文件名: %s\n", formatCtx->filename);
    fprintf(fileVideoInfo, "时长: %lld\n", formatCtx->duration);
    fprintf(fileVideoInfo, "码率: %d\n", formatCtx->bit_rate);
    fprintf(fileVideoInfo, "扩展名: %s\n", inputFormat->extensions);
    fprintf(fileVideoInfo, "封装格式: %s\n", inputFormat->name);
    fprintf(fileVideoInfo, "封装格式（完整）: %s\n", inputFormat->long_name);

    AVCodecContext *codecCtx = formatCtx->streams[index]->codec;
    // 查找解码器
    //AVCodec *codec = formatCtx->video_codec;
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    if (NULL == codec) {
        printf("find decoder error\n");
        return -1;
    }

    if (avcodec_open2(codecCtx, codec, NULL) < 0) {
        printf("avcodec open error\n");
        return -1;
    }

    AVFrame *frame = av_frame_alloc();
    AVFrame *frameYuv = av_frame_alloc();
    uint8_t *outBuffer = (uint8_t *)malloc(avpicture_get_size(PIX_FMT_YUV420P, codecCtx->width, codecCtx->height));
    avpicture_fill((AVPicture *)frameYuv, outBuffer, PIX_FMT_YUV420P, codecCtx->width, codecCtx->height);

    SwsContext *img_convert_ctx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
        codecCtx->width, codecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));

    // 创建显示窗口
    SDL_Init(SDL_INIT_VIDEO);
    int screenWidth = codecCtx->width;
    int screenHeight = codecCtx->height;
    //SDL_Window *sdlWnd = SDL_CreateWindow("DisplayVideoDemo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_Window *sdlWnd = SDL_CreateWindowFrom(pDlg->m_videoWnd.GetSafeHwnd());
    SDL_Renderer *render = SDL_CreateRenderer(sdlWnd, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
        screenWidth, screenHeight);

    SDL_Thread * thread = SDL_CreateThread(VideoShowThread, NULL, NULL);
    SDL_Event event;
    //SDL_Rect sdlRect;
    int got_picture;

    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == DISPLAY_VIDEO) {
            while (1) {
                if (av_read_frame(formatCtx, packet) < 0)
                    exitCode = 1;

                // 过滤掉音频帧
                if (packet->stream_index == index)
                    break;
            }
            avcodec_decode_video2(codecCtx, frame, &got_picture, packet);
            if (got_picture) {
                sws_scale(img_convert_ctx, frame->data, frame->linesize, 0,
                    codecCtx->height, frameYuv->data, frameYuv->linesize);

                SDL_UpdateTexture(texture, NULL, frameYuv->data[0], frameYuv->linesize[0]);
                SDL_RenderClear(render);
                SDL_RenderCopy(render, texture, NULL, NULL);
                SDL_RenderPresent(render);
            }
            av_free_packet(packet);
        }
        else if (event.type == SDL_QUIT) {
            exitCode = 1;
        }
        else if (event.type == BREAD_DISPALY) {
            break;
        }
    }

    sws_freeContext(img_convert_ctx);
    avformat_close_input(&formatCtx);
    avformat_free_context(formatCtx);
    avformat_network_deinit();
    SDL_Quit();

    return 0;
}

void CtestplayDlg::OnBnClickedBtnPlay()
{
    AfxBeginThread(ffmpegPlayer, this);
}


void CtestplayDlg::OnBnClickedBtnStop()
{

}


void CtestplayDlg::OnBnClickedBtnBreak()
{

}


void CtestplayDlg::OnBnClickedBtnOpen()
{
    CString strUrl;
    m_url.GetWindowText(strUrl);
}
