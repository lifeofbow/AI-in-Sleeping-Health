// HarrGuiDlg.h : ͷ�ļ�
//

#pragma once

#pragma warning(disable:4996)

#include "cv.h"
#include "highgui.h"
#include "cvaux.h"
#include "cxcore.h"
#include "svm.h"
#include <opencv\cv.h>
#include <opencv\cvaux.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <vector>
using namespace std;
using namespace cv;
// CHarrGuiDlg �Ի���
class CHarrGuiDlg : public CDialog
{
// ����
public:
	CHarrGuiDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CHarrGuiDlg();

// �Ի�������
	enum { IDD = IDD_HARRGUI_DIALOG };

	protected:
	//virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    double m_nImgScale;
    double m_nSrchScale;
    int    m_nMinNeighbours;
    int    m_bDoCanny;
    int    m_nMinWidth;
    int    m_nMinHeight;
    int    m_nInputType;

    int    m_bReset;
    int    m_bReady;

    CString                  m_strCascade;
    CString                  m_strImg;
    CvMemStorage*            storage;
    CvHaarClassifierCascade* cascade;
	struct svm_model* model_HSV;
	struct svm_model* model_HoG;
	vector<float> descriptors;
	

	CString m_strmodel;
    IplImage *pImage;
	svm_node x[2000];
public:
    void OnReset();
    void OnProcessImage(IplImage *img);
	double OnProcessImage2(IplImage *img);
    void DrawPicToHDC(IplImage *img, UINT ID);

public:
    afx_msg void OnBnClickedSelectcascade();
    afx_msg void OnBnClickedOpenfile();
    afx_msg void OnOpenCamera();
public:
    afx_msg void OnBnClickedButton4();
public:
    afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickselectModel();
	
};
