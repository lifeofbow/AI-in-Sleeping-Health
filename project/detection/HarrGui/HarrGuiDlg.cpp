// HarrGuiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CvvImage.h"
#include "HarrGui.h"
#include "HarrGuiDlg.h"

using namespace std;
using namespace cv;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHarrGuiDlg 对话框,构造函数
CHarrGuiDlg::CHarrGuiDlg(CWnd* pParent)
	: CDialog(CHarrGuiDlg::IDD, pParent)
	//cvhaardetection函数的参数，通过改变参数可以提高精确率
{
    storage = NULL;
    //cascade = NULL;

    m_nInputType = 0; //camera

    m_bReset = 0;
    m_bReady = 1;

    pImage = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//析构函数
CHarrGuiDlg::~CHarrGuiDlg()
{
    if (pImage != NULL)
    {
        cvReleaseImage(&pImage);
    }

    if (storage != NULL)
    {
        cvReleaseMemStorage(&storage);
    }

    /*if (cascade != NULL)
    {
        cvRelease((void **)&cascade);
    }*/

    m_bReset = 1;
}

BEGIN_MESSAGE_MAP(CHarrGuiDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    // ON_BN_CLICKED(IDC_SelectCascade, &CHarrGuiDlg::OnBnClickedSelectcascade)
    ON_BN_CLICKED(IDC_OpenFile, &CHarrGuiDlg::OnBnClickedOpenfile)
    ON_BN_CLICKED(IDC_BUTTON3, &CHarrGuiDlg::OnOpenCamera)
    ON_BN_CLICKED(IDC_BUTTON4, &CHarrGuiDlg::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CHarrGuiDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CHarrGuiDlg::OnBnClickselectModel)
	
END_MESSAGE_MAP()


// CHarrGuiDlg 消息处理程序

BOOL CHarrGuiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
HCURSOR CHarrGuiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT MyThreadProc( LPVOID pParam )
{
    CvCapture* capture = 0;

    IplImage *frame;
    IplImage *frame_copy = NULL;

    CHarrGuiDlg *dlg = (CHarrGuiDlg *)pParam;

    if (dlg->m_nInputType == 0)
    {
        capture = cvCaptureFromCAM( 0 );//opencamera，在对话框中显示摄像头内容
    }
    else
    {
        capture = cvCaptureFromAVI(dlg->m_strImg.GetBuffer());//openfile，在对话框中显示视频
        dlg->m_strImg.ReleaseBuffer(-1);
    }

    if (capture == 0)
    {
        return -1;
    }

    dlg->m_bReady = 0;

	frame = cvQueryFrame(capture);

    for (;;)
    {
		frame = cvQueryFrame(capture);
        if( !frame )
            break;
        if( !frame_copy )
            frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
            IPL_DEPTH_8U, frame->nChannels );
        if( frame->origin == IPL_ORIGIN_TL )
            cvCopy( frame, frame_copy, 0 );
        else
            cvFlip( frame, frame_copy, 0 );
		//DrawPicToHDC(frame_copy, IDC_PIC);
		dlg->DrawPicToHDC(frame_copy, IDC_PIC);;//onprocessImage函数，对文件的每一帧进行检测，圈出车辆或其他。

        cvWaitKey( 1 );

        if (dlg->m_bReset)
        {
            break;
        }
    }	

    cvReleaseCapture( &capture );
    cvReleaseImage( &frame_copy );

    dlg->m_bReady = 1;

    return 0;
}

void CHarrGuiDlg::OnReset()
{
    UpdateData(TRUE);//每次鼠标点击run，便会调用此函数。

    m_bReset = 1;

    while(m_bReady != 1);

    if (pImage != NULL)
    {
        cvReleaseImage(&pImage);
    }

    if (storage != NULL)
    {
        cvReleaseMemStorage(&storage);
    }

    storage = cvCreateMemStorage(0);
    if (storage == NULL)
    {
        MessageBox(_T("Cannot cvCreateMemStorage"));
        return;
    }
    if (m_nInputType == 2)
    {
        pImage = cvLoadImage(m_strImg.GetBuffer(), CV_LOAD_IMAGE_COLOR);
        if (pImage == NULL)
        {
            MessageBox(_T("Cannot Load Image"));
            return;
        }

		DrawPicToHDC(pImage, IDC_PIC);
		OnProcessImage(pImage);
        return;
    }

    CWinThread *pThread = AfxBeginThread(MyThreadProc, this);
    if(NULL == pThread )
    {
        MessageBox("Cannot Create MyThreadProc", MB_OK);
        return;
    }

    m_bReset = 0;

}

void CHarrGuiDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
    CDC *pDC = GetDlgItem(ID)->GetDC();
    HDC hDC= pDC->GetSafeHdc();
    CRect rect;
    GetDlgItem(ID)->GetClientRect(&rect);
    CvvImage cimg;

    cimg.CopyOf(img);
    cimg.DrawToHDC(hDC,&rect);

    ReleaseDC(pDC);
}

void CHarrGuiDlg::OnProcessImage(IplImage *src){
	//void predict(FILE *input, FILE *output)
	
	
	//IplImage * src = cvLoadImage("C:\\Users\\zhangrun\\Desktop\\test2\\P50825-212146-30.jpg", 1);
	IplImage* hsv = cvCreateImage(cvGetSize(src), 8, 3);
	IplImage* h_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* s_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* v_plane = cvCreateImage(cvGetSize(src), 8, 1);
	IplImage* planes[] = { h_plane, s_plane, v_plane };

	//H 分量划分为16个等级，S分量划分为8个等级 */
	int h_bins = 16, s_bins = 8, v_bins = 8;
	int hist_size[] = { h_bins, s_bins, v_bins };

	//** H 分量的变化范围 */
	float h_ranges[] = { 0, 180 };

	//** S 分量的变化范围*/
	float s_ranges[] = { 0, 255 };
	float v_ranges[] = { 0, 255 };

	float* ranges[] = { h_ranges, s_ranges, v_ranges };

	//** 输入图像转换到HSV颜色空间 */
	cvCvtColor(src, hsv, CV_BGR2HSV);
	cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane, 0);

	//** 创建直方图，二维, 每个维度上均分 */
	CvHistogram * hist = cvCreateHist(3, hist_size, CV_HIST_ARRAY, ranges, 1);
	//** 根据H,S两个平面数据统计直方图 */
	cvCalcHist(planes, hist, 0, 0);

	//** 获取直方图统计的最大值，用于动态显示直方图 */
	float max_value;
	cvGetMinMaxHistValue(hist, 0, &max_value, 0, 0);


	//** 设置直方图显示图像 */
	int height = 100;
	int width = (h_bins*s_bins*v_bins * 5);

	int bin_w = width / (h_bins * s_bins);
	int count = 0;

	for (int h = 0; h < h_bins; h++){
		for (int s = 0; s < s_bins; s++){
			for (int v = 0; v < v_bins; v++){
				int i = h*s_bins + s*v_bins + v;
				/** 获得直方图中的统计次数，计算显示在图像中的高度 */
				float bin_val = cvQueryHistValue_3D(hist, h, s, v);
				//int intensity = cvRound(bin_val*height / max_value);
				float intensity = bin_val*height / max_value;
				x[count].index = count;
				x[count].value = intensity;

				//fout << " " << count << ":" << intensity;
				count++;
			}
		}
	}
	x[count].index = -1;

	double predict_label;
	predict_label = svm_predict(model_HSV, x);
	if (predict_label == 3){
		MessageBox("the location of the quilt is OK \n, the state of the air-condition needn't to be changed", MB_OK);
	}
	else{
		/*predict_label = OnProcessImage2(src);
		if (predict_label == 1){*/
		MessageBox("The quilt has been pushed off \n,the air-condition should increase the temperature! !", MB_OK);
		/*}
		else if (predict_label == 2){
			MessageBox("This picture is belong to Half !", MB_OK);
		}*/
	}
	//cout << predict_label << endl;
}

//double CHarrGuiDlg::OnProcessImage2(IplImage *src){
//
//	HOGDescriptor hog(Size(64, 64), Size(16, 16), Size(8, 8), Size(8, 8), 9);
//	Mat img;
//	resize(Mat(pImage), img,Size(64,64));
//	descriptors.clear();
//	hog.compute(img, descriptors);
//	int count = 1;
//	for (int i = 0; i < descriptors.size(); i++){
//		x[i].index = count++;
//		x[i].value = descriptors[i];
//	}
//	x[count - 1].index = -1;
//	double predict_label;
//	return predict_label = svm_predict(model_HoG, x);	
//}

void CHarrGuiDlg::OnBnClickedOpenfile()
{
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        NULL, NULL, 0);
    dlg.m_ofn.lpstrInitialDir = "D:/data/COMPLETE";

    if (dlg.DoModal() != IDOK)
        return;

    m_strImg = dlg.GetPathName();

    m_strImg.MakeLower();

    if (StrStr(m_strImg, ".avi"))
    {
        m_nInputType = 1; //avi
    }
    else
    {
        m_nInputType = 2; //image
    }

    SetWindowText(m_strImg);
}

void CHarrGuiDlg::OnOpenCamera()
{
    m_nInputType = 0;
}

void CHarrGuiDlg::OnBnClickedButton4()
{
    // TODO: 在此添加控件通知处理程序代码
    OnReset();
}

void CHarrGuiDlg::OnBnClickedButton5()
{
    // TODO: 在此添加控件通知处理程序代码
    m_bReset = 1;

    while(m_bReady != 1);
}


void CHarrGuiDlg::OnBnClickselectModel()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		NULL, NULL, 0);
	dlg.m_ofn.lpstrInitialDir = "D:/data/COMPLETE";

	if (dlg.DoModal() != IDOK)
		return;

	m_strmodel = dlg.GetPathName();

	model_HSV = svm_load_model(m_strmodel);// load model

	MessageBox("The Model for HSV test has been prepared !", MB_OK);

	SetWindowText(m_strImg);
}

//
//void CHarrGuiDlg::OnBnClickedselectmodelHoG()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
//		NULL, NULL, 0);
//	dlg.m_ofn.lpstrInitialDir = "D:/data/COMPLETE";
//
//	if (dlg.DoModal() != IDOK)
//		return;
//
//	m_strmodel = dlg.GetPathName();
//
//	model_HoG = svm_load_model(m_strmodel);// load model
//
//	MessageBox("The Model for HoG test has been prepared !", MB_OK);
//
//	SetWindowText(m_strImg);
//}
