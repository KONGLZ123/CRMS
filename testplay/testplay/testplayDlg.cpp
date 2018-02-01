
// testplayDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CtestplayDlg �Ի���



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


// CtestplayDlg ��Ϣ�������

BOOL CtestplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestplayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

    // �����ļ�����¼��Ƶ��Ϣ
    fopen_s(&fileVideoInfo, "videoInfo.txt", "wb+");
    if (NULL == fileVideoInfo) {
        printf("Create file error\n");
        return -1;
    }

    // ����Ƶ������
    if (0 != avformat_open_input(&formatCtx, filename, NULL, NULL)) {
        printf("Open input error\n");
        return -1;
    }

    // ��ȡ��Ƶ����Ϣ
    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
        printf("Find stream info error\n");
        return -1;
    }

    AVInputFormat *inputFormat = formatCtx->iformat;

    int index = 0;
    // �ж���һ·����Ƶ��
    for (unsigned int i = 0; i < formatCtx->nb_streams; ++i) {
        if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            index = i;
            break;
        }
    }

    fprintf(fileVideoInfo, "�ļ���: %s\n", formatCtx->filename);
    fprintf(fileVideoInfo, "ʱ��: %lld\n", formatCtx->duration);
    fprintf(fileVideoInfo, "����: %d\n", formatCtx->bit_rate);
    fprintf(fileVideoInfo, "��չ��: %s\n", inputFormat->extensions);
    fprintf(fileVideoInfo, "��װ��ʽ: %s\n", inputFormat->name);
    fprintf(fileVideoInfo, "��װ��ʽ��������: %s\n", inputFormat->long_name);

    AVCodecContext *codecCtx = formatCtx->streams[index]->codec;
    // ���ҽ�����
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

    // ������ʾ����
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

                // ���˵���Ƶ֡
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
