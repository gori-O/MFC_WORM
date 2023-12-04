
// MFC_WARM_GAMEDlg.h: 헤더 파일
//

#pragma once

#define MAX_EAT_COUNT		300
#define MAX_LENGTH			300

struct MapPoint
{
	char x;
	char y;
};
// CMFCWARMGAMEDlg 대화 상자
class CMFCWARMGAMEDlg : public CDialogEx
{
private:
	CDC m_draw_dc;
	CImage m_draw_image;

	MapPoint m_eat_pos[MAX_EAT_COUNT];			// 먹은거만큼 잔상

	COLORREF m_table[60][80] = { 0, };			
	int m_count_map[60][80] = { 0, };			

	POINT m_pos = { 40,30 };
	int m_direction = 0;			// 0 left 1 up 2 right 3 down
	int m_eat_count = 0;

	int m_speed = 200;
	int m_strong = 0;
	CFont m_font1, m_font2;
// 생성입니다.
public:
	CMFCWARMGAMEDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_WARM_GAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void DrawMap();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GameOver(CString _msg);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ReGame();
};
