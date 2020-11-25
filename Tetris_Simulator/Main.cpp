//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cxClasses"
#pragma link "cxControls"
#pragma link "cxGraphics"
#pragma link "cxLookAndFeelPainters"
#pragma link "cxLookAndFeels"
#pragma link "dxBar"
#pragma link "dxRibbon"
#pragma link "dxRibbonCustomizationForm"
#pragma link "dxRibbonSkins"
#pragma link "dxSkinBlack"
#pragma link "dxSkinBlue"
#pragma link "dxSkinBlueprint"
#pragma link "dxSkinCaramel"
#pragma link "dxSkinCoffee"
#pragma link "dxSkinDarkRoom"
#pragma link "dxSkinDarkSide"
#pragma link "dxSkinDevExpressDarkStyle"
#pragma link "dxSkinDevExpressStyle"
#pragma link "dxSkinFoggy"
#pragma link "dxSkinGlassOceans"
#pragma link "dxSkinHighContrast"
#pragma link "dxSkiniMaginary"
#pragma link "dxSkinLilian"
#pragma link "dxSkinLiquidSky"
#pragma link "dxSkinLondonLiquidSky"
#pragma link "dxSkinMcSkin"
#pragma link "dxSkinMetropolis"
#pragma link "dxSkinMetropolisDark"
#pragma link "dxSkinMoneyTwins"
#pragma link "dxSkinOffice2007Black"
#pragma link "dxSkinOffice2007Blue"
#pragma link "dxSkinOffice2007Green"
#pragma link "dxSkinOffice2007Pink"
#pragma link "dxSkinOffice2007Silver"
#pragma link "dxSkinOffice2010Black"
#pragma link "dxSkinOffice2010Blue"
#pragma link "dxSkinOffice2010Silver"
#pragma link "dxSkinOffice2013DarkGray"
#pragma link "dxSkinOffice2013LightGray"
#pragma link "dxSkinOffice2013White"
#pragma link "dxSkinPumpkin"
#pragma link "dxSkinsCore"
#pragma link "dxSkinsDefaultPainters"
#pragma link "dxSkinsdxBarPainter"
#pragma link "dxSkinsdxRibbonPainter"
#pragma link "dxSkinSeven"
#pragma link "dxSkinSevenClassic"
#pragma link "dxSkinSharp"
#pragma link "dxSkinSharpPlus"
#pragma link "dxSkinSilver"
#pragma link "dxSkinSpringTime"
#pragma link "dxSkinStardust"
#pragma link "dxSkinSummer2008"
#pragma link "dxSkinTheAsphaltWorld"
#pragma link "dxSkinValentine"
#pragma link "dxSkinVisualStudio2013Blue"
#pragma link "dxSkinVisualStudio2013Dark"
#pragma link "dxSkinVisualStudio2013Light"
#pragma link "dxSkinVS2010"
#pragma link "dxSkinWhiteprint"
#pragma link "dxSkinXmas2008Blue"
#pragma link "AdvMemo"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{
	InitProgram();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	ExitProgram();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PrintMsg(UnicodeString _str) {
	int t_Idx = memo->Lines->Add(_str);
	memo->SetCursor(0, t_Idx);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::InitProgram() {

	// Common
	UnicodeString t_Str = L"";

	// Notebook Page Setting
	Notebook_Main->PageIndex = 0; // Main

	// Init Grid
	InitGrid();

	// Init Client Thread and Member Socket
	for(int i = 0 ; i < MAX_CLIENT_COUNT ; i++) {
		m_Client[i] = NULL;
		m_sock_Client[i] = INVALID_SOCKET;
	}





	// Socket Init
	WSADATA data;
	WORD version;
	int ret = 0;

	version = MAKEWORD(2, 2);
	ret = WSAStartup(version, &data);
	if(ret != 0) {
		ret = WSAGetLastError();
		if(ret == WSANOTINITIALISED) {
			t_Str.sprintf(L"Socket not initialised (error code : %d)", ret);
			PrintMsg(t_Str);
		} else {
			t_Str.sprintf(L"Socket error (error code : %d)", ret);
			PrintMsg(t_Str);
		}
	} else {
		PrintMsg(L"Socket init success");
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ExitProgram() {


	// Socket
	WSACleanup();
}
//---------------------------------------------------------------------------

bool __fastcall TFormMain::CreateTCPSocket(SOCKET* _socket) {

	// Common
	UnicodeString tempStr = L"";
	AnsiString t_AnsiStr = "";

	// Create Socket
	*_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(*_socket == INVALID_SOCKET) {
		PrintMsg(L"Fail to create socket");
		return false;
	}

	// Set Socket Option : REUSE
	int t_opt_reuse = 1;
	if(setsockopt(*_socket, SOL_SOCKET, SO_REUSEADDR,(char *)&t_opt_reuse, sizeof(t_opt_reuse)) == SOCKET_ERROR) {
		PrintMsg(L"Fail to set socket option (REUSE)");
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------

bool __fastcall TFormMain::DeleteTCPSocket(SOCKET* _socket) {
	closesocket(*_socket);
	*_socket = INVALID_SOCKET;
	return true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::InitGrid() {

	// Common
	UnicodeString tempStr = L"";

	// Load Images
	TBitmap* t_bmp = new TBitmap;
	t_bmp->LoadFromFile(L".\\Images\\green.bmp");
	ImgList->Add(t_bmp, t_bmp);
	t_bmp->FreeImage();

	t_bmp->LoadFromFile(L".\\Images\\gray.bmp");
	ImgList->Add(t_bmp, t_bmp);
	t_bmp->FreeImage();

	delete t_bmp;

	// Setting
	grid->ControlLook->NoDisabledButtonLook = true;

	// Init Grid
	int t_RowCnt = grid->RowCount;
	for(int i = 1 ; i < t_RowCnt ; i++) {
		grid->Cells[0][i] = i;
		grid->AddImageIdx(1, i, 1, haCenter, Advgrid::vaCenter);
		grid->AddButton(2, i, 92, 24, L"Connect", haCenter, Advgrid::vaCenter);
		grid->AddButton(3, i, 92, 24, L"Disconnect", haCenter, Advgrid::vaCenter);
		grid->Cells[4][i] = L"192.168.220.201";
		grid->Cells[5][i] = L"0000";
		grid->AddButton(6, i, 72, 24, L"Enter", haCenter, Advgrid::vaCenter);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MenuBtn_ViewClick(TObject *Sender)
{
	Notebook_Main->PageIndex = 0; // Main
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MenuBtn_SettingClick(TObject *Sender)
{
	Notebook_Main->PageIndex = 1; // Setting
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridButtonClick(TObject *Sender, int ACol, int ARow)
{
	// Common
	UnicodeString tempStr = L"";

	// Fix Index
	int t_Idx = ARow - 1;

	if(ACol == 2) { // Connect Button
		// Create Socket
		if(m_sock_Client[t_Idx] != INVALID_SOCKET) {
			tempStr.sprintf(L"Client Socket [%d] is already exists.", t_Idx);
			PrintMsg(tempStr);
			return;
		}

		if(CreateTCPSocket(&m_sock_Client[t_Idx])) {
			// Set Cell Icon as GREEN
			grid->RemoveImageIdx(1, ARow);
			grid->AddImageIdx(1, ARow, 0, haCenter, Advgrid::vaCenter);

			// Print Message
			tempStr.sprintf(L"Success to create Client Socket [%d]", t_Idx);
			PrintMsg(tempStr);

			// Create Thread
			if(m_Client[t_Idx] != NULL) {
				tempStr.sprintf(L"Client [%d] Thread is already exists.", t_Idx);
				PrintMsg(tempStr);
				return;
			}
			m_Client[t_Idx] = new CTcpSocketThread(&m_sock_Client[t_Idx]);

		} else {
			tempStr.sprintf(L"Fail to create Client Socket [%d]", t_Idx);
			PrintMsg(tempStr);
		}

	} else if(ACol == 3) { // Disconnect Button

		if(m_sock_Client[t_Idx] == INVALID_SOCKET) {
			tempStr.sprintf(L"There is no Client Socket [%d].", t_Idx);
			PrintMsg(tempStr);
			return;
		}

		if(DeleteTCPSocket(&m_sock_Client[t_Idx])) {
			// Set Cell Icon as GRAY
			grid->RemoveImageIdx(1, ARow);
			grid->AddImageIdx(1, ARow, 1, haCenter, Advgrid::vaCenter);

			// Print Message
			tempStr.sprintf(L"Success to delete Client Socket [%d]", t_Idx);
			PrintMsg(tempStr);

			// Destroy Thread
			if(m_Client[t_Idx] == NULL) {
				tempStr.sprintf(L"There is no Client [%d] Thread.", t_Idx);
				PrintMsg(tempStr);
				return;
			}
			m_Client[t_Idx]->DoTerminate();
			m_Client[t_Idx]->Terminate();
			delete m_Client[t_Idx];

		} else {
			tempStr.sprintf(L"Fail to delete Client Socket [%d]", t_Idx);
			PrintMsg(tempStr);
		}

	} else if(ACol == 6) { // Enter Button
		tempStr.sprintf(L"Entering to [%d] Client", t_Idx);
		PrintMsg(tempStr);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MenuBtn_VersionClick(TObject *Sender)
{
	TFormVersion *dlg = new TFormVersion(NULL);
	dlg->ShowModal();
	delete dlg;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ReceiveMsg(TMessage &_msg) {
	unsigned int t_wParam = _msg.WParam;
	int t_lParam = _msg.LParam;

	UnicodeString tempStr = L"";
	UnicodeString *p = NULL;
	p = (UnicodeString*)t_wParam;
	tempStr = *p;
	PrintMsg(tempStr);
}
//---------------------------------------------------------------------------

