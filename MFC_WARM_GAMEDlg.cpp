
// MFC_WARM_GAMEDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_WARM_GAME.h"
#include "MFC_WARM_GAMEDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCWARMGAMEDlg 대화 상자



CMFCWARMGAMEDlg::CMFCWARMGAMEDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_WARM_GAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCWARMGAMEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCWARMGAMEDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFCWARMGAMEDlg 메시지 처리기

BOOL CMFCWARMGAMEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_draw_image.Create(801, 601, 32);
	m_draw_dc.Attach(m_draw_image.GetDC());

	srand((unsigned int)time(NULL));
	int x, y;
	for (int i = 0; i < 300; i++)
	{
		x = rand() % 80;
		y = rand() % 60;

		if (m_table[y][x] == 0) { m_table[y][x] = RGB(255, 0, 0); }
		else i--;
	}

	SetDlgItemInt(IDC_LEVEL_EDIT, 1);
	DrawMap();

	SetTimer(1, m_speed, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCWARMGAMEDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_draw_image.Draw(dc, 0, 0);
		//CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCWARMGAMEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCWARMGAMEDlg::DrawMap()
{
	m_draw_dc.SelectStockObject(DC_BRUSH);
	m_draw_dc.SelectStockObject(DC_PEN);
	m_draw_dc.SetDCPenColor(RGB(47, 47, 27));
	m_draw_dc.SetDCBrushColor(RGB(12, 12, 12));

	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++) {
			if (m_count_map[y][x] > 0) { 
				m_count_map[y][x]--; 
				if (m_count_map[y][x] == 0) m_table[y][x] = 0;
			}
			m_draw_dc.SetDCBrushColor(m_table[y][x]);
			m_draw_dc.Rectangle(x * 10, y * 10, x * 10 + 11, y * 10 + 11);
		}
	}
}


void CMFCWARMGAMEDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
	m_draw_dc.Detach();
	m_draw_image.ReleaseDC();
}


void CMFCWARMGAMEDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		if (m_direction == 0) { m_pos.x--; }
		else if (m_direction == 1) { m_pos.y--; }
		else if (m_direction == 2) { m_pos.x++; }
		else m_pos.y++;

		if (m_pos.x >= 0 && m_pos.x <= 79 && m_pos.y >= 0 && m_pos.y <= 59) {
			if (m_table[m_pos.y][m_pos.x] == RGB(255, 0, 0)) {
				m_eat_count++;
				SetDlgItemInt(IDC_SCORE_EDIT, m_eat_count);
			}
			else if (m_table[m_pos.y][m_pos.x]) {
				GameOver(L"eat myself");
				return;
			}
			m_table[m_pos.y][m_pos.x] = RGB(0, 255, 0);
			m_count_map[m_pos.y][m_pos.x] = m_eat_count + 2;

			DrawMap();
			CClientDC dc(this);
			m_draw_image.Draw(dc, 0, 0);
		}
		else { 
			GameOver(L"hit the wall");
		}
	}
	if (m_eat_count >= 300) {
		GameOver(L"win");
	}

	if (m_eat_count == 20) {
		SetDlgItemInt(IDC_LEVEL_EDIT, 2);
		
		SetTimer(1, 120, NULL);
	}
	if (m_eat_count == 40) {
		SetDlgItemInt(IDC_LEVEL_EDIT, 3);
		SetTimer(1, 80, NULL);
	}
	if (m_eat_count == 60) {
		SetDlgItemInt(IDC_LEVEL_EDIT, 4);
		SetTimer(1, 60, NULL);
	}
	else {
		CDialogEx::OnTimer(nIDEvent);
	}
}


void CMFCWARMGAMEDlg::GameOver(CString _msg)
{
	KillTimer(1);

	// msg table
	// "win" : 게임 승리 "eat myself" : 스스로 부딪침,게임패배 "hit the wall" : 벽에 부딪침, 게임패배
	if (_msg == "win") {
		if (MessageBox(L"게임에서 승리했습니다!!\r\n다시하시겠습니까?", L"이겼습니다.!", MB_YESNO) == IDYES) {
			ReGame();
		}
		else { ::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL); }
	}
	else if (_msg == "eat myself") {
		if (MessageBox(L"스스로 부딪혀 패배했습니다.\r\n다시하시겠습니까?", L"게임오버!.", MB_YESNO) == IDYES) {
			ReGame();
		}
		else { ::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL); }
	}
	else if (_msg == "hit the wall") {
		if (MessageBox(L"벽에 부딪혀 패배했습니다.\r\n다시하시겠습니까?", L"게임오버!.", MB_YESNO) == IDYES) {
			ReGame();
		}
		else { ::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL); }
	}
}


BOOL CMFCWARMGAMEDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam >= VK_LEFT && pMsg->wParam <= VK_DOWN) {
			int old_direction = m_direction;
			m_direction = pMsg->wParam - VK_LEFT;
			if (old_direction > m_direction) {
				if ((old_direction - m_direction) == 2) m_direction = old_direction;
			}
			else {
				if ((m_direction - old_direction) == 2) m_direction = old_direction;
			}
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMFCWARMGAMEDlg::ReGame()
{
	m_speed = 200;
	m_pos = { 40,30 };
	m_eat_count = 0;
	m_direction = 0;
	SetDlgItemInt(IDC_LEVEL_EDIT, 1);
	SetDlgItemText(IDC_SCORE_EDIT, L"0");
	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++) {
			m_table[y][x] = 0;
			m_count_map[y][x] = 0;
		}
	}
	int x, y;
	for (int i = 0; i < 300; i++)
	{
		x = rand() % 80;
		y = rand() % 60;

		if (m_table[y][x] == 0) { m_table[y][x] = RGB(255, 0, 0); }
		else i--;
	}
	for (int y = 0; y < 60; y++) {
		for (int x = 0; x < 80; x++) {
			if (m_count_map[y][x] > 0) {
				m_count_map[y][x]--;
				if (m_count_map[y][x] == 0) m_table[y][x] = 0;
			}
			m_draw_dc.SetDCBrushColor(m_table[y][x]);
			m_draw_dc.Rectangle(x * 10, y * 10, x * 10 + 11, y * 10 + 11);
		}
	}
	Invalidate();
	SetTimer(1, 200, NULL);
}

