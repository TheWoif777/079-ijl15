#include "stdafx.h"
#include "d3d9.h"
#include <Resolution.h>

int m_slotXPos = 0;
int m_nGameHeight = 720;
int m_nGameWidth = 1280;
int m_nBarWidth = 0;
int m_nSlotWidth;
unsigned int m_nBackgrndWidth;
unsigned int  m_nBackgrnd2Width;

__declspec(naked) void refreshMap()
{
	__asm {
		mov esi, 0x00BDF54C
		mov esi, [esi]
		mov ecx, esi
		mov eax, 0x0066E59E
		call eax
		mov esi, 0x00BDD490
		mov esi, [esi]
		push[esi + 0x00000F28]
		mov ecx, 0x00BDD460
		mov ecx, [ecx]
		mov eax, 0x00437717
		call eax
		ret
	}
}

__declspec(naked) void reloadMap()   // error! Waiting for repair
{
	__asm {
		pushad
		pushfd
		//mov ebx, 0x00D8B50C
		//mov ecx, ebx
		//mov eax, 0x007B99E2
		//call eax
		mov ecx, [0x00BD9028]
		mov ecx, [ecx]
		lea eax, [ecx + 0x2FE4]
		mov esi, 0x1
		mov[eax], esi
		push 0x0
		mov eax, 0x00A1BD8F
		call eax
		mov ecx, [0x00BD9028]
		mov ecx, [ecx]
		lea eax, [ecx + 0x2FE4]
		mov esi, 0x0
		mov[eax], esi
		push 0x1
		mov eax, 0x00A1BD8F
		call eax
		mov ecx, [0x00BDD6F0]
		mov ecx, [ecx]
		mov edi, [0x00400FF4]
		mov edi, [edi]
		push edi
		mov eax, 0x008E4603
		call eax
		popfd
		popad
		ret
	}
}

__declspec(naked) void enterGame()
{
	__asm {
		pushad
		push Client::m_nGameHeight
		push Client::m_nGameWidth
		call Resolution::UpdateResolution
		pop eax
		pop eax
		popad
		mov eax, 0x008D49F0
		call eax
		push 0x00A1D90B
		ret
	}
}

__declspec(naked) void exitGame()
{
	__asm {
		pushad
		push 600
		push 800
		call Resolution::UpdateResolution
		pop eax
		pop eax
		popad
		mov ecx, [0x00BDD8AC]
		mov ecx, [ecx]
		push 0x00A0DA5E
		ret
	}
}

__declspec(naked) void enterShop()
{
	__asm {
		pushad
		push 600
		push 800
		call Resolution::UpdateResolution
		pop eax
		pop eax
		popad
		mov eax, [0x00AD5801]
		mov eax, [eax]
		push 0x00863BC8
		ret
	}
}

void _backgroundHook(DWORD* eax) {
	//std::cout << "backgroundHook eax" << *eax << std::endl;
	DWORD* IWzGr2DPtr = reinterpret_cast<DWORD*>(0x00BE2788);
	if (*eax == 0x008D6557) {  //切图
		if (*IWzGr2DPtr != NULL)
			Memory::WriteInt(*IWzGr2DPtr + 32, Client::m_nGameWidth);
	}
	if (*eax == 0x00669BF0) {//退出
		if (*IWzGr2DPtr != NULL)
			Memory::WriteInt(*IWzGr2DPtr + 32, Client::m_nGameWidth);
	}
}

__declspec(naked) void backgroundHook()
{
	__asm {
		pushad
		pushfd
		mov eax, esp
		add eax, 0x24
		push eax
		call _backgroundHook
		pop eax
		popfd
		popad
		push ebp
		mov ebp, esp
		push ecx
		push ebx
		push 0x00427EB8
		ret
	}
}

void _UpdateResolution(int nScreenWidth, int nScreenHeight) {

	m_nGameHeight = nScreenHeight;
	m_nGameWidth = nScreenWidth;

	//byte tempTest[] = { 131, 248, 1, 116, 103, 187, 12, 181, 216, 0, 137, 217, 184, 89, 75, 137, 0, 255, 208, 139, 13, 108, 196, 215, 0, 141, 129, 0, 54, 0, 0, 190, 1, 0, 0, 0, 137, 48, 106, 0, 184, 0, 243, 183, 0, 255, 208, 139, 13, 108, 196, 215, 0, 141, 129, 0, 54, 0, 0, 190, 0, 0, 0, 0, 137, 48, 106, 1, 184, 0, 243, 183, 0, 255, 208, 139, 13, 100, 11, 216, 0, 139, 61, 244, 15, 64, 0, 87, 184, 200, 90, 160, 0, 255, 208, 157, 97, 184, 16, 160, 194, 0, 104, 150, 71, 111, 0, 195, 157, 97, 199, 5, 168, 15, 64, 0, 0, 0, 0, 0, 184, 16, 160, 194, 0, 104, 150, 71, 111, 0, 195 };
	//Memory::WriteByteArray(0x00400D88, tempTest, sizeof(tempTest));
	if (Client::minimizeMaptitleColor)
		Memory::WriteInt(0x00864524 + 1, 0xFFFFFFFF);  //minimize map title color white
	Memory::CodeCave(SaveD3D, 0x009EC607, 5);
	Memory::CodeCave(gameRect, 0x009FFEFB, 6);

	nStatusBarY = nScreenHeight - 578;

	Memory::CodeCave(AdjustStatusBar, dwStatusBarVPos, 5);
	Memory::CodeCave(AdjustStatusBarBG, dwStatusBarBackgroundVPos, 5);
	Memory::CodeCave(AdjustStatusBarInput, dwStatusBarInputVPos, 11);

	Memory::WriteInt(dwApplicationHeight + 1, nScreenHeight);//push 600
	Memory::WriteInt(dwApplicationWidth + 1, nScreenWidth);	//push 800 ; CWvsApp::InitializeGr2D
	Memory::WriteInt(dwCursorVectorVPos + 2, (unsigned int)floor(-nScreenHeight / 2));//push -300				!!moves all interactable UI elements!!
	Memory::WriteInt(dwCursorVectorHPos + 2, (unsigned int)floor(-nScreenWidth / 2));	//push -400 ; CInputSystem::SetCursorVectorPos				!!moves all interactable UI elements!!
	Memory::WriteInt(dwUpdateMouseLimitVPos + 1, nScreenHeight);//mov ecx,600
	Memory::WriteInt(dwUpdateMouseLimitHPos + 1, nScreenWidth);	//mov ecx,800 ; CInputSystem::UpdateMouse
	Memory::WriteInt(dwCursorPosLimitVPos + 1, nScreenHeight);//mov eax,600
	Memory::WriteInt(dwCursorPosLimitHPos + 1, nScreenWidth);	//mov eax,800 ; CInputSystem::SetCursorPos
	Memory::WriteInt(dwViewPortHeight + 3, nScreenHeight);//lea eax,[esi+eax-600]
	Memory::WriteInt(dwViewPortWidth + 3, nScreenWidth);	//lea eax,[ecx+eax-800]

	Memory::WriteInt(dwToolTipLimitVPos + 1, nScreenHeight - 1); //mov eax,599 ; CUIToolTip::MakeLayer
	Memory::WriteInt(dwToolTipLimitHPos + 1, nScreenWidth - 1); //mov eax,799 ; CUIToolTip::MakeLayer

	Memory::WriteInt(dwTempStatToolTipDraw + 3, -nScreenWidth + 6); //lea eax,[eax+ecx-797] ; CTemporaryStatView::ShowToolTip
	Memory::WriteByte(0x007B9CB9 + 2, 26);   // 3 + 23                 buff tip y-pos range
	Memory::WriteByte(0x007B9CC2 + 2, 58);   // 35+23
	Memory::WriteInt(dwTempStatToolTipFind + 3, -nScreenWidth + 6); //lea eax,[eax+ecx-797] ; CTemporaryStatView::FindIcon
	Memory::WriteByte(0x007B9EA7 + 2, 26);   // 3 + 23                  y-pos cancel range
	Memory::WriteByte(0x007B9EAC + 2, 58);   // 35 + 23
	Memory::WriteInt(dwTempStatIconVPos + 2, (nScreenHeight / 2) - 23);	//sub ebx,277 ; Skill icon buff y-pos
	Memory::WriteInt(dwTempStatIconHpos + 3, (nScreenWidth / 2) - 3);	//lea eax,[eax+esi+397] ; Skill icon buff x-pos
	Memory::WriteInt(dwTempStatCoolTimeVPos + 2, (nScreenHeight / 2) - 23);	//sub ebx,277 ; Skill icon cooltime y-pos
	Memory::WriteInt(dwTempStatCoolTimeHPos + 3, (nScreenWidth / 2) - 3);	//lea eax,[eax+esi+397] ; Skill icon cooltime x-pos

	//Client::UpdateSlotPosition(540);

	//Memory::WriteInt(dwByteAvatarMegaHPos + 1, nScreenHeight + 100); //push 800 ; CAvatarMegaphone::ByeAvatarMegaphone ; IWzVector2D::RelMove ##BAK
	Memory::WriteInt(dwByteAvatarMegaHPos + 1, nScreenWidth); //push 800 ; CAvatarMegaphone::ByeAvatarMegaphone ; IWzVector2D::RelMove
	Memory::WriteInt(dwAvatarMegaWidth + 1, nScreenWidth); //push 800 ; CAvatarMegaphone ; CreateWnd

	Memory::WriteInt(0x00436E61 + 1, nScreenHeight);    //->079 00436E61 600
	Memory::WriteInt(0x00436E67 + 1, nScreenWidth);    // CreateWnd    //->079 00436E67 800
	Memory::WriteInt(0x00575D78 + 1, nScreenHeight);                          //->079 00575D78 600
	Memory::WriteInt(0x00575D7D + 1, nScreenWidth);    // RelMove?   //-> 079  00575D7D 800
	Memory::WriteInt(0x004E3250 + 1, nScreenWidth);    // CreateWnd //-> 079 004E3250 || 004D875F 800
	Memory::WriteInt(0x004E33B5 + 2, nScreenHeight);                          //->079 004E33B5 600
	Memory::WriteInt(0x004D88AF + 1, nScreenWidth);    // CreateWnd //->079 004D88AF 800        
	Memory::WriteInt(0x004D88C4 + 1, nScreenHeight);                         //-> 079 004D88C4 
	Memory::WriteInt(0x004E33A0 + 1, nScreenWidth);    // CreateWnd //->079  004E33A0
	Memory::WriteInt(0x0086D3BC + 2, nScreenWidth);                          //->079 0086D3BC 800
	Memory::WriteInt(0x0086D3C4 + 1, nScreenWidth - 80);                      // CreateDlg//->079 0086D3C4  (TMS 700 CMS 704)
	Memory::WriteInt(0x008F224E + 1, nScreenHeight);                        //-> 079 008F224E 800
	Memory::WriteInt(0x008F226A + 1, nScreenWidth);    // RelMove? //->079 008F226A 600
	Memory::WriteInt(0x009A3691 + 1, nScreenHeight);                      //->079 009A3691
	Memory::WriteInt(0x009A36A6 + 2, nScreenWidth);                        //->079 009A36A6 
	Memory::WriteInt(0x009A36AE + 1, nScreenWidth - 100);                     // CreateDlg //->079 009A36AE 700
	Memory::WriteInt(0x009AFFCD + 1, nScreenHeight);                        //->079 009AFFCD 600
	Memory::WriteInt(0x009AFFC0 + 1, nScreenWidth);    // CreateDlg //->079 009AFFC0 800
	Memory::WriteInt(0x0045B0C0 + 1, nScreenHeight - 25);                //->079 0045B0C0 575
	Memory::WriteInt(0x0045B1A6 + 1, nScreenWidth);    // RelMove? //->079 0045B1A6 900
	//Memory::WriteInt(0x004E7AF7 + 1, nScreenWidth);                        //->079 004E7AF7 800   会导致商城选项卡点击失效


	Memory::WriteInt(0x00575D78 + 1, nScreenHeight);                     //->079 00575D78 600
	Memory::WriteInt(0x00575D7D + 1, nScreenWidth);                     //->079 00575D7D 800
	Memory::WriteInt(0x00575DF4 + 1, nScreenWidth);    // RelMove? //->079 00575DF4 600
	Memory::WriteInt(0x007F7C0E + 1, nScreenWidth);    // CreateWnd//->079 007F7C0E 800
	Memory::WriteInt(0x007F7C09 + 1, nScreenHeight);                        //->079 007F7C09 600
	Memory::WriteInt(0x007F7C0E + 1, nScreenWidth);    // CWnd::GetCanvas //->079 007F7C0E
	Memory::WriteInt(0x008BB421 + 1, nScreenHeight);           //->079 008BB421
	Memory::WriteInt(0x008BB426 + 1, nScreenWidth);    // CreateWnd           //->079 008BB426
	//Memory::WriteInt(0x00A00FA0 + 1, nScreenHeight);          //->079 00A00FA0
	//Memory::WriteInt(0x00A00FA5 + 1, nScreenWidth);    // StringPool#1162 (MapleStoryClass) //->079 00A00FA5
	Memory::WriteInt(0x005E0E32 + 1, nScreenHeight);               //->079 005E0E32                     
	Memory::WriteInt(0x005E0E42 + 1, nScreenWidth);    // RelMove?     //->079 005E0E42
	Memory::WriteInt(0x005E1C80 + 1, nScreenHeight);                     //->079 005E1C80
	Memory::WriteInt(0x005E1C90 + 1, nScreenWidth);    // RelMove?                     //->079 005E1C90
	Memory::WriteInt(0x005DFE68 + 1, nScreenHeight);                    //->079 005DFE68
	Memory::WriteInt(0x005DFE78 + 1, nScreenWidth);    // RelMove?                   //->079 005DFE78
	Memory::WriteInt(0x005E0E32 + 1, nScreenHeight);                  //->079 005E0E32
	Memory::WriteInt(0x005E0E42 + 1, nScreenWidth);    // RelMove?                  //->079 005E0E42
	Memory::WriteInt(0x005E1C80 + 1, nScreenHeight);                //->079 005E1C80
	Memory::WriteInt(0x005E1C90 + 1, nScreenWidth);    // RelMove?                  //->079 005E1C90
	Memory::WriteInt(0x005F5290 + 1, nScreenHeight);                 //->079 005F5290
	Memory::WriteInt(0x005F52A0 + 1, nScreenWidth);    // RelMove?                   //->079 005F52A0
	Memory::WriteInt(0x005F676B + 1, nScreenHeight);                  //->079 005F676B
	Memory::WriteInt(0x005F677B + 1, nScreenWidth);    // RelMove?                     //->079 005F677B
	Memory::WriteInt(0x007D69DF + 1, nScreenHeight);                   //->079 007D69DF

	Memory::WriteInt(0x007D69ED + 1, nScreenWidth);    // IWzVector2D::RelMove //->079 007D69ED 800
	Memory::WriteInt(0x008B23AC + 1, nScreenHeight);                    //->079 008B23AC 600
	Memory::WriteInt(0x008B23BA + 1, nScreenWidth);    // IWzVector2D::RelMove //->079 008B23BA 800
	Memory::WriteInt(0x0065ACF4 + 1, nScreenHeight);                  //-> 079  0065ACF4 600 
	Memory::WriteInt(0x0065B4CB + 1, nScreenWidth);    // IWzGr2DLayer::Getcanvas//->079 0065B4CB 800
	Memory::WriteInt(0x0065B4C6 + 1, nScreenHeight);                        //->079 0065B4C6 600
	Memory::WriteInt(0x0065ACF9 + 1, nScreenWidth);    // IWzGr2DLayer::Getcanvas//->079 0065ACF9  800
	Memory::WriteInt(0x0080AB3D + 1, nScreenHeight);                       //->079 0080AB3D 600
	Memory::WriteInt(0x0080AB51 + 1, nScreenWidth);    // CWnd::CreateWnd        //->079 0080AB51 800
	Memory::WriteInt(0x0065A64D + 1, nScreenHeight);                    //->079 0065A64D || 0065A8E3 || 0065B030 600
	Memory::WriteInt(0x0065A652 + 1, nScreenWidth);    // (UI/Logo/Wizet)          //->079 0065A652 || 0065B035 || 0065A8E8 800
	Memory::WriteInt(0x0065B4C6 + 1, nScreenHeight);                       //->079 0065B4C6
	Memory::WriteInt(0x0065B4CB + 1, nScreenWidth);    // Rectangle                         //->079 0065B4CB
	Memory::WriteInt(0x0066CB0A + 1, nScreenHeight);                        //->079 0066CB0A
	Memory::WriteInt(0x0066CAF8 + 1, nScreenWidth);                        //->079 0066CAF8
	Memory::WriteInt(0x0066CC90 + 1, nScreenHeight);                        //->079 0066CC90
	Memory::WriteInt(0x0066CC7E + 1, nScreenWidth);                        //->079 0066CC7E
	Memory::WriteInt(0x0066CC18 + 1, nScreenHeight);                         //->079 0066CC18
	Memory::WriteInt(0x0066CC90 + 1, nScreenHeight);                        //->079 0066CC90
	Memory::WriteInt(0x0066CC1D + 1, nScreenHeight);                        //->079 0066CC1D
	Memory::WriteInt(0x0066CC4B + 1, nScreenHeight);                        //->079 0066CC4B
	Memory::WriteInt(0x0066CC06 + 1, nScreenWidth);                          //->079 0066CC06
	Memory::WriteInt(0x0066CC7E + 1, nScreenWidth);                          //->079 0066CC7E
	Memory::WriteInt(0x0066CC39 + 1, nScreenWidth);                          //->079 0066CC39
	Memory::WriteInt(0x0066CA3E + 1, floor(nScreenWidth / 2));             //->079 0066CA3E
	Memory::WriteInt(0x0066CA43 + 1, floor(nScreenHeight / 2));            //->079 0066CA43
	Memory::WriteInt(0x0066CC26 + 1, floor(nScreenWidth / 2));                // bunch of modulus stuff    //->079 0066CC26
	Memory::WriteInt(0x0066E0C6 + 3, nScreenHeight);                          //->079 0066E0C6
	Memory::WriteInt(0x0066E0BF + 3, nScreenWidth);    // idk               //->079 0066E0BF
	Memory::WriteInt(0x0066E1E5 + 3, nScreenHeight);                             //->079 0066E1E5
	Memory::WriteInt(0x0066E1DB + 3, nScreenWidth);    // CAnimationDisplayer::SetCenterOrigin          //->079 0066E1DB
	Memory::WriteInt(0x0046B007 + 1, nScreenHeight);                            //->079 0046B007
	Memory::WriteInt(0x0046B019 + 1, nScreenWidth);    // IWzVector2D::RelMove         //->079 0046B019
	Memory::WriteInt(0x009A4FDB + 1, nScreenHeight);                            //->079 009A4FDB 
	Memory::WriteInt(0x009A4FF0 + 2, nScreenWidth);                                //->079 009A4FF0
	Memory::WriteInt(0x009A4FF8 + 1, nScreenWidth - 100);                     // CreateDlg                             //->079 009A4FF8
	Memory::WriteInt(0x0066E675 + 1, floor(nScreenHeight / 2));               // VRRight        //->079 0066E675
	Memory::WriteInt(0x0066E73C + 1, floor(nScreenHeight / 2));               //->079 0066E73C
	Memory::WriteInt(0x0066E60E + 1, floor(nScreenWidth / 2));                // VRTop                              //->079 0066E60E
	Memory::WriteInt(0x006BE6EA + 1, floor(nScreenWidth / 2));                // RelMove?                         //->079 006BE6EA
	Memory::WriteInt(0x005CA78D + 6, floor(nScreenHeight / 2));             //->079 005CA78D
	Memory::WriteInt(0x005CA787 + 2, floor(nScreenWidth / 2));                // CInputSystem::LoadCursorState               //->079 005CA787
	Memory::WriteInt(0x0081C817 + 1, nScreenHeight);                           //->079 0081C817
	Memory::WriteInt(0x0081C804 + 1, nScreenWidth);    // CUIEventAlarm::CreateEventAlarm              //->079 0081C804
	Memory::WriteInt(0x008D4C75 + 1, nScreenHeight - 22);                    //->079 008D4C75
	Memory::WriteInt(0x008D4C7A + 1, nScreenWidth);                            //->079 008D4C7A


	Memory::WriteInt(0x00436F9C + 1, floor(-nScreenHeight / 2));            //->079 00436F9C
	Memory::WriteInt(0x00436F9C + 1, floor(-nScreenWidth / 2));               // RelMove?                         //->079 00436F9C
	Memory::WriteInt(0x0057609F + 1, floor(-nScreenHeight / 2));            //->079 0057609F
	Memory::WriteInt(0x005760A5 + 1, floor(-nScreenWidth / 2));               // RelMove?                          //->079 005760A5
	Memory::WriteInt(0x00436F97 + 1, floor(-nScreenHeight / 2));            //->079 00436F97
	Memory::WriteInt(0x00436F9C + 1, floor(-nScreenWidth / 2));               // RelMove?                          //->079 00436F9C
	Memory::WriteInt(0x006BF23E + 1, nScreenWidth);                            //->079 006BF23E
	Memory::WriteInt(0x007F8207 + 1, nScreenWidth);                            //->079 007F8207
	Memory::WriteInt(0x008DCDED + 1, nScreenHeight - 22);                    //->079 008DCDED
	Memory::WriteInt(0x008DCDF2 + 1, nScreenWidth);                             //->079 008DCDF2
	Memory::WriteInt(0x0093BEAD + 1, floor(nScreenHeight / 2));             //->079 0093BEAD     
	Memory::WriteInt(0x0093BEA8 + 1, nScreenWidth);    // RelMove? (Skills)                  //->079 0093BEA8
	Memory::WriteInt(0x009E9D8C + 1, nScreenWidth);                           //->079 009E9D8C
	Memory::WriteInt(0x009E9F02 + 1, nScreenHeight);                          // IWzVector2D::RelMove         //->079 009E9F02
	Memory::WriteInt(0x009FF84F + 1, floor(nScreenHeight / 2));               // MapleStoryClass               //->079 009FF84F
	Memory::WriteInt(0x006E7F58 + 1, nScreenHeight);                          // 006E8BFF 006E9379 006EA459    /->079 006E7F58 || 006E72BD || 006E8849
	Memory::WriteInt(0x0075930E + 1, nScreenHeight);                            //->079 0075930E
	Memory::WriteInt(0x007591B6 + 1, nScreenHeight);                            //->079 007591B6
	Memory::WriteInt(0x00758D04 + 1, nScreenHeight);                           //->079 00758D04
	Memory::WriteInt(0x00758C19 + 1, nScreenHeight);                           //->079 00758C19
	Memory::WriteInt(0x0066E860 + 1, nScreenHeight);                          // CSoundMan::PlayBGM         //->079 0066E860
	Memory::WriteInt(0x00634BB1 + 1, nScreenHeight);                          //->079 00634BB1
	//Memory::WriteInt(0x00634BBB + 1, floor(-nScreenHeight / 2));                                 //->079 00634BBB 选人界面底图
	Memory::WriteInt(0x0061ACCE + 1, nScreenWidth - 136);                           //->079 0061ACCE  nScreenHeight? 664? 
	Memory::WriteInt(0x005D4261 + 1, nScreenHeight);                          // CSoundMan::PlayBGM
	Memory::WriteInt(0x008D7458 + 1, nScreenHeight - 33);                     // IWzVector2D::RelMove
	Memory::WriteInt(0x008E36A8 + 1, nScreenHeight - 20);
	Memory::WriteInt(0x008E3944 + 1, nScreenHeight - 20);
	Memory::WriteInt(0x008D7739 + 1, nScreenHeight - 19);
	Memory::WriteInt(0x008D797F + 1, nScreenHeight - 19);
	Memory::WriteInt(0x008DD6F6 + 1, nScreenHeight - 19);
	Memory::WriteInt(0x008DDE76 + 1, nScreenHeight - 19);
	Memory::WriteInt(0x008DE5C1 + 1, nScreenHeight - 19);
	Memory::WriteInt(0x008DEC14 + 1, nScreenHeight - 19);	//push 581 临时经验条
	Memory::WriteInt(0x008D6D3B + 1, nScreenHeight - 22);

	//Memory::WriteInt(0x008D6D40 + 1, nScreenWidth);//Bar底图
	Memory::WriteInt(0x008D6FE3 + 1, nScreenHeight - 22);
	Memory::WriteInt(0x008D6FE8 + 1, nScreenWidth);    // CUIStatusBar
	Memory::WriteInt(0x0065B4C6 + 1, nScreenHeight);
	Memory::WriteInt(0x0065B4CB + 1, nScreenWidth);    // (UI/Logo/Nexon)
	Memory::WriteInt(0x004FD494 + 1, nScreenWidth);
	Memory::WriteInt(0x004FD483 + 1, nScreenHeight);                          // CreateWnd
	Memory::WriteInt(0x004FD3E3 + 1, nScreenWidth);
	Memory::WriteInt(0x004FD3CA + 1, nScreenHeight);
	Memory::WriteInt(0x009FFF02, nScreenHeight);
	Memory::WriteInt(0x009FFF07, nScreenWidth);
	Memory::WriteInt(0x00BD1788, floor(nScreenWidth / 2));
	Memory::WriteInt(0x00BD35E4, floor(nScreenHeight / 2));
	Memory::WriteInt(0x00BD35E0, floor(nScreenWidth / 2));
	Memory::WriteInt(0x0066CC56 + 2, floor(-nScreenWidth / 2));
	Memory::WriteInt(0x0066D6AD + 1, floor(-nScreenHeight / 2));
	Memory::WriteInt(0x0066D6B5 + 1, floor(-nScreenWidth / 2));
	Memory::WriteInt(0x006BF359 + 1, floor(-nScreenWidth / 2));
	Memory::WriteInt(0x009EC61A + 1, floor(-nScreenHeight / 2));
	Memory::WriteInt(0x009EC620 + 1, floor(-nScreenWidth / 2));
	Memory::WriteInt(0x0093B675 + 1, floor(-nScreenHeight / 2));
	Memory::WriteInt(0x0095DBA3 + 1, floor(-nScreenHeight / 2));
	Memory::WriteInt(0x0098EE27 + 1, floor(-nScreenHeight / 2));
	Memory::WriteInt(0x0098F84C + 2, floor(-nScreenHeight / 2));
	Memory::WriteInt(0x00A4EAC9 + 2, floor(-nScreenHeight / 2));              // CWvsPhysicalSpace2D::Load          //->079 00A4EAC9 || 00A55A79 -300
	Memory::WriteInt(0x00669275 + 2, floor(-nScreenWidth / 2));
	Memory::WriteInt(0x009CFD87 + 3, floor(-nScreenHeight / 2));
	Memory::WriteInt(0x009CFD95 + 3, floor(nScreenHeight / 2));

	//Memory::WriteInt(0x009F7078 + 1, nScreenHeight);//??related to application dimensions	//(ragezone release merge)//thanks mr mr of ragezone for these addresses
	//Memory::WriteInt(0x009F707D + 1, nScreenWidth);//??related to application dimensions

	//Memory::WriteInt(0x0058C8A6 + 1, nScreenWidth);//??

	//Memory::WriteInt(0x004EDABF + 1, nScreenHeight);//??

	//Memory::WriteInt(0x00991854 + 1, nScreenHeight);//??unknown cwnd function
	//Memory::WriteInt(0x0099185F + 1, (nScreenWidth / 2) - 134);//??unknown cwnd function
	//Memory::WriteInt(0x00991867 + 1, (nScreenWidth / 2) - 133);//??unknown cwnd function
	//Memory::WriteInt(0x00992BA7 + 1, (unsigned int)floor(nScreenWidth / 2));//??unknown cwnd function, possibly related to cutildlg
	//Memory::WriteInt(0x00992BAC + 1, (unsigned int)floor(nScreenHeight / 2));//??unknown cwnd function, possibly related to cutildlg

	Memory::WriteInt(0x007F8357 + 2, nScreenWidth);//related to displaying server message at top of screen
	Memory::WriteInt(0x007F7F1A + 2, nScreenWidth);//related to displaying server message at top of screen

	Memory::WriteInt(0x005462B2 + 1, (nScreenWidth / 2) - 129);//related to boss bar
	//Memory::WriteInt(0x005364AA + 2, (nScreenWidth / 2) - 128);//??related to boss bar
	Memory::WriteInt(0x00A2E4F9 + 1, (nScreenWidth / 2) - 129); //??
	Memory::WriteByte(0x005434BA, 0xB8);	//boss bar extend to window
	Memory::WriteInt(0x005434BA + 1, nScreenWidth - 2);	//boss bar	extend to window
	Memory::WriteByte(0x00543D27, 0xB9);	//boss bar	extend to window
	Memory::WriteInt(0x00543D27 + 1, nScreenWidth - 9);	//boss bar	extend to window

	//Memory::WriteInt(0x00592A08 + 1, (nScreenWidth / 2) - 125);//??likely related to mouse pos

	//Memory::WriteInt(0x00621226 + 1, (nScreenWidth / 2) - 216);//??possibly related to logo
	//Memory::WriteByte(0x0062121E + 1, 0x01);//??possibly related to logo

	//Memory::WriteInt(0x008C069F + 1, (nScreenHeight / 2) - 14);//??related to status bar
	//Memory::WriteInt(0x008C06A4 + 1, (nScreenWidth / 2) - 158);//???related to status bar

	//Memory::WriteInt(0x00A24D0B + 1, (nScreenWidth / 2) - 129);//??

	//Memory::WriteInt(0x00BE273C, 128);//??
	//Memory::WriteByte(0x00A5FC2B, 0x05);//??
	////Memory::WriteByte(0x008D1790 + 2, 0x01); //related to quickslots area presence		 originally 1U but changed because unsigned int crashes it after char select
	//Memory::WriteByte(0x00592A06 + 1, 0x01);//???likely related to mouse pos

	Memory::WriteInt(0x007518B2 + 1, nScreenWidth);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x007518B7 + 1, nScreenHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00751828 + 1, 3 * nScreenWidth * nScreenHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00751841 + 1, nScreenWidth * nScreenHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x007517A4 + 1, 4 * nScreenWidth * nScreenHeight);//??related to in-game taking screenshot functionality

	//Memory::WriteInt(0x008A8FA9 + 1, (nScreenWidth / 2) - 143);//??related to exp gain/item pick up msg

	Memory::WriteInt(0x0062A7DE + 1, (unsigned int)floor(-nScreenHeight / 2));	//push -300 ;
	Memory::WriteInt(0x0062A922 + 1, (unsigned int)floor(-nScreenHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00628EFB + 1, (unsigned int)floor(nScreenWidth / 2));//??related to login game frame

	//Memory::WriteInt(0x0060411C + 1, nScreenHeight);//push 600
	//Memory::WriteInt(0x00604126 + 1, floor(-nScreenWidth / 2));	//push -300 //moves characters side to side on char select //unnecessary atm
	Memory::WriteInt(0x0063C5B4 + 1, (nScreenHeight / 2) - 201);//??possibly related to login utildlg
	Memory::WriteInt(0x0063C5BD + 1, (nScreenHeight / 2) - 181);//??possibly related to login utildlg
	Memory::WriteInt(0x0063C5C5 + 1, (nScreenWidth / 2) - 201);//??possibly related to login utildlg
	Memory::WriteInt(0x0063C5CE + 1, (nScreenWidth / 2) - 181);//??possibly related to login utildlg
	//Memory::WriteInt(0x00613093 + 1, (nScreenHeight / 2) - 200);//??likely related to login pop-up msg
	//Memory::WriteInt(0x00613099 + 1, (nScreenWidth / 2) - 45);//??likely related to login pop-up msg
	//Memory::WriteInt(0x0061DAFF + 1, (nScreenHeight / 2) - 150);//??likely related to login pop-up msg
	//Memory::WriteInt(0x0061DB08 + 1, (nScreenHeight / 2) - 130);//??likely related to login pop-up msg
	//Memory::WriteInt(0x0061DB10 + 1, (nScreenWidth / 2) - 201);//??likely related to login pop-up msg
	//Memory::WriteInt(0x0061DB19 + 1, (nScreenWidth / 2) - 181);//??likely related to login pop-up msg

	int msgAmntOffset, msgAmnt; msgAmnt = Client::MsgAmount; msgAmntOffset = msgAmnt * 14;

	//Memory::WriteInt(0x008AC830 + 1, nScreenHeight - 6 - msgAmntOffset);//inventory/exp gain y axis //####hd100 //90  注释改用固定位置经验条位置
	Memory::WriteInt(0x008AC8EE + 1, nScreenWidth - 405);//inventory/exp gain x axis //310 //####hd415 //405
	//Memory::WriteInt(0x008AC98D + 1, nScreenHeight - 6 - msgAmntOffset); //related to quickslots area presence  注释改用固定位置经验条位置
	Memory::WriteInt(0x008ACBFA + 1, nScreenWidth - 405);
	//固定位置经验条位置（适配动态bar背景，不需要的话启用上面两条注释，注释下面的）
	if (nScreenWidth > 800) {
		Memory::WriteByte(0x008AC82D + 2, 0);
		Memory::WriteByte(0x008AC98A + 2, 0);
	}
	else {
		Memory::WriteByte(0x008AC82D + 2, 0xB2);
		Memory::WriteByte(0x008AC98A + 2, 0xB2);
	}
	Memory::WriteInt(0x008AC830 + 1, nScreenHeight - 6 - msgAmntOffset - 78);
	Memory::WriteInt(0x008AC98D + 1, nScreenHeight - 6 - msgAmntOffset - 78);

	//动态Bar背景宽度
	Memory::CodeCave(updateBarBackgrndWidth, 0x008E3547, 6);
	Memory::CodeCave(updateBarBackgrndaAnimationTime, 0x008E355F, 5);
	Memory::CodeCave(updateBarBackgrndaInit, 0x008D50CD, 5);  //CUIStatusBar::OnCreate

	Memory::WriteInt(0x008AC12A + 1, 400);//length of pick up and exp gain message canvas //found with help from Davi
	Memory::WriteInt(0x008AC4BD + 1, 400);//address to move the message in the canvas adjusted above to the center of the new canvas  //thanks chris

	Memory::WriteInt(0x008AC0D9 + 3, msgAmnt);//moregainmsgs part 1
	MoreGainMsgsOffset = msgAmnt;	//param for ccmoregainmssgs
	Memory::CodeCave(ccMoreGainMsgs, dwMoreGainMsgs, MoreGainMsgsNOPs); //moregainmsgs part 2
	MoreGainMsgsFadeOffset = 15000;	//param for ccmoregainmssgsFade
	Memory::CodeCave(ccMoreGainMsgsFade, dwMoreGainMsgsFade, MoreGainMsgsFadeNOPs); //moregainmsgsFade
	MoreGainMsgsFade1Offset = 255 * 4 / 3;	//param for ccmoregainmssgsFade
	Memory::CodeCave(ccMoreGainMsgsFade1, dwMoreGainMsgsFade1, MoreGainMsgsFade1NOPs); //moregainmsgsFade1

	//Memory::WriteInt(0x0045B337 + 1, nScreenWidth);//related to smega display  //likely screen area where pop up starts for smega
	//Memory::WriteInt(0x0045B417 + 1, nScreenWidth - 225);//smega with avatar x axis for duration on screen

	//Memory::WriteInt(0x007C2531 + 1, nScreenHeight - 80);//??

	Memory::WriteInt(0x0066D6A4 + 2, nScreenWidth);
	Memory::WriteInt(0x0066D69D + 2, nScreenHeight);

	Memory::WriteInt(0x005DDAAF + 1, nScreenWidth);
	Memory::WriteInt(0x005DDA9F + 1, nScreenHeight);

	Memory::WriteInt(0x0061FA6F + 1, nScreenWidth);
	Memory::WriteInt(0x0061FA65 + 1, nScreenHeight);

	Memory::WriteInt(0x007EC157 + 1, nScreenWidth);
	Memory::WriteInt(0x007EC146 + 1, nScreenHeight);

	Memory::WriteInt(0x00804E8F + 1, nScreenWidth);
	Memory::WriteInt(0x00804E7E + 1, nScreenHeight);

	Memory::WriteInt(0x007F7AD5 + 1, nScreenWidth);

	Memory::WriteInt(0x0065A8E8 + 1, nScreenWidth);   //未知
	Memory::WriteInt(0x0065A8E3 + 1, nScreenHeight);

	Memory::WriteInt(0x005B7123 + 1, nScreenWidth);  //顶部喇叭位置
	//喇叭
	Memory::WriteInt(0x00459F1B + 1, nScreenWidth);				 //   所有动画位置参数都是 左上角  右上角 宽 高
	Memory::WriteInt(0x0045AB28 + 1, nScreenWidth + 100);  //飘入喇叭起始位置
	Memory::WriteInt(0x0045AC0A + 1, nScreenWidth - 225);  //飘入喇叭终止位置
	Memory::WriteInt(0x0099EBBC + 1, nScreenWidth / 2 - 133); //蓝色提示框
	Memory::WriteInt(0x0099EBA9 + 1, nScreenHeight);
	Memory::WriteInt(0x005BBAE8 + 1, nScreenWidth); //顶部通知
	Memory::WriteInt(0x005BBAE3 + 1, nScreenHeight);
	Memory::WriteInt(0x005449E7 + 1, nScreenWidth / 2); //地图指示
	Memory::WriteInt(0x005449E2 + 1, 230);

	Memory::WriteInt(0x008581D3 + 1, nScreenHeight - 198); //system menu pop up
	Memory::WriteInt(0x008589E8 + 1, nScreenHeight - 281); //shortcuts pop up	//0x84A5BD -  System Options "X" Position. if needed

	//各种气泡
	int inviteXOffset = nScreenHeight > 600 ? 10 : 0;
	Memory::WriteInt(0x0052D694 + 1, nScreenHeight - 92 - inviteXOffset);// ??various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052D886 + 1, nScreenHeight - 92 - inviteXOffset); // ??various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052DA9F + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052DD77 + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052DFB3 + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up //quest complete y axis
	Memory::WriteInt(0x0052E1A9 + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052E39B + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052E5CF + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052E7C3 + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052E9C1 + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052EDF9 + 1, nScreenHeight - 92 - inviteXOffset);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052F0E1 + 1, nScreenHeight - 102 - inviteXOffset);//?? various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052EBAA + 1, nScreenHeight - 102 - inviteXOffset);//party quest available pop-up y axis


	int inviteY = nScreenWidth > 800 ? 582 : 464;
	Memory::WriteInt(0x0052D6A8 + 1, inviteY);
	Memory::WriteInt(0x0052D89A + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up //Party Invite
	Memory::WriteInt(0x0052DAB3 + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up //friend request
	Memory::WriteInt(0x0052DD8B + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up	// Guild Invite
	Memory::WriteInt(0x0052DFC7 + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up//??
	Memory::WriteInt(0x0052DAB3 + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up/??
	Memory::WriteInt(0x0052E1BD + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up/??
	Memory::WriteInt(0x0052E3AF + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up// ??
	Memory::WriteInt(0x0052E5E3 + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up// ??
	Memory::WriteInt(0x0052E7E3 + 1, inviteY);//various requests like party, guild, friend, family, invites that pop up//??
	Memory::WriteInt(0x0052E9D5 + 1, inviteY);
	Memory::WriteInt(0x0052EBBE + 1, inviteY);
	Memory::WriteInt(0x0052EE0D + 1, inviteY);
	Memory::WriteInt(0x0052F0F5 + 1, inviteY);


	myHeight = (nScreenHeight - 600) / 2;//cash shop fix for frame area	//recalc offsets
	myWidth = (nScreenWidth - 800) / 2;//cash shop fix for frame area		//recalc offsets

	// 频道变更居中
	Memory::WriteInt(0x009A1C15 + 1, myWidth + 182);

	// 现金商城居中
	nHeightOfsetted1 = 316; nWidthOfsetted1 = 256; nTopOfsetted1 = 0 + myHeight; nLeftOfsetted1 = 0 + myWidth; //parameters for fix1
	Memory::CodeCave(CashShopFix1, dwCashFix1, dwCashFix1NOPs);
	nHeightOfsetted2 = 104; nWidthOfsetted2 = 256; nTopOfsetted2 = 318 + myHeight; nLeftOfsetted2 = -1 + myWidth; //parameters for fix2
	Memory::CodeCave(CashShopFix2, dwCashFix2, dwCashFix2NOPs);
	nHeightOfsetted3 = 163; nWidthOfsetted3 = 246; nTopOfsetted3 = 426 + myHeight; nLeftOfsetted3 = 0 + myWidth; //parameters for fix3
	Memory::CodeCave(CashShopFix3, dwCashFix3, dwCashFix3NOPs);
	nHeightOfsetted4 = 78; nWidthOfsetted4 = 508; nTopOfsetted4 = 17 + myHeight; nLeftOfsetted4 = 272 + myWidth; //parameters for fix4   top
	Memory::CodeCave(CashShopFix4, dwCashFix4, dwCashFix4NOPs);
	nHeightOfsetted5 = 430; nWidthOfsetted5 = 412; nTopOfsetted5 = 95 + myHeight; nLeftOfsetted5 = 275 + myWidth; //parameters for fix5
	Memory::CodeCave(CashShopFix5, dwCashFix5, dwCashFix5NOPs);	//main part of shop, item listings	//thanks angel for stuff that helped
	Memory::CodeCave(CashShopFix9, dwCashFix9, dwCashFix9NOPs);	//main part of shop 2
	nHeightOfsetted6 = 358; nWidthOfsetted6 = 90; nTopOfsetted6 = 157 + myHeight; //parameters for fix6
	Memory::CodeCave(CashShopFix6, dwCashFix6, dwCashFix6NOPs);//code cave 6 //best sellers
	Memory::WriteInt(0x00468F7F + 1, myWidth + 690);//nleft, actual drawn part	//best sellers
	nHeightOfsetted7 = 56; nWidthOfsetted7 = 545; nTopOfsetted7 = 536 + myHeight; nLeftOfsetted7 = 254 + myWidth; //parameters for fix7   bottom exit
	Memory::CodeCave(CashShopFix7, dwCashFix7, dwCashFix7NOPs);
	nHeightOfsetted8 = 22; nWidthOfsetted8 = 89; nTopOfsetted8 = 97 + myHeight; nLeftOfsetted8 = 690 + myWidth; //parameters for fix8
	Memory::CodeCave(CashShopFix8, dwCashFix8, dwCashFix8NOPs);
	Memory::CodeCave(CashShopFixOnOff, dwCashFixOnOff, dwCashFixOnOffNOPs);	//fix for preview On/Off button not being accurate on entering cash shop //thanks windyboy

	nHeightOfsettedPrev = 165 + myHeight; nWidthOfsettedPrev = 212 + myWidth; nTopOfsettedPrev = 40 + myHeight; nLeftOfsettedPrev = 24 + myWidth; //parameters for fix cash preview
	Memory::CodeCave(CashShopFixPrev, dwCashFixPrev, dwCashFixPrevNOPs); //cash shop preview fix

	// 交易中心居中
	iHeightOfsetted1 = 200; iWidthOfsetted1 = 256; iTopOfsetted1 = 0 + myHeight; iLeftOfsetted1 = 0 + myWidth;
	Memory::CodeCave(ITCFix1, 0x005D40F9, 12);
	iHeightOfsetted2 = 110; iWidthOfsetted2 = 256; iTopOfsetted2 = 200 + myHeight; iLeftOfsetted2 = -1 + myWidth;
	Memory::CodeCave(ITCFix2, 0x005D4111, 13);
	iHeightOfsetted3 = 108; iWidthOfsetted3 = 256; iTopOfsetted3 = 310 + myHeight; iLeftOfsetted3 = 0 + myWidth;
	Memory::CodeCave(ITCFix3, 0x005D412D, 13);
	iHeightOfsetted4 = 180; iWidthOfsetted4 = 256; iTopOfsetted4 = 418 + myHeight; iLeftOfsetted4 = 0 + myWidth;
	Memory::CodeCave(ITCFix4, 0x005D4149, 16);
	iHeightOfsetted5 = 78; iWidthOfsetted5 = 509; iTopOfsetted5 = 17 + myHeight; iLeftOfsetted5 = 272 + myWidth;
	Memory::CodeCave(ITCFix5, 0x005D4167, 16);	//main part of shop, item listings	//thanks angel for stuff that helped
	iHeightOfsetted6 = 48; iWidthOfsetted6 = 509; iTopOfsetted6 = 98 + myHeight; iLeftOfsetted6 = 273 + myWidth;
	Memory::CodeCave(ITCFix6, 0x005D4186, 14);//code cave 6 //best sellers
	iHeightOfsetted7 = 365; iWidthOfsetted7 = 509; iTopOfsetted7 = 145 + myHeight; iLeftOfsetted7 = 273 + myWidth;
	Memory::CodeCave(ITCFix7, 0x005D41A3, 20);
	iHeightOfsetted8 = 56; iWidthOfsetted8 = 545; iTopOfsetted8 = 531 + myHeight; iLeftOfsetted8 = 255 + myWidth;
	Memory::CodeCave(ITCFix8, 0x005D41C6, 17);

	//Memory::WriteInt(0x0066E6DC + 2, (unsigned int)floor(nScreenWidth / 2));	//mov ebc,400 ;  VRright		//camera movement	//crashes
	Memory::WriteInt(0x0066E73C + 1, (unsigned int)floor(nScreenHeight / 2));	//add eax,300  ; VRbottom //camera movement //not working for most maps

	myAlwaysViewRestoreFixOffset = myHeight; //parameters for fix view restore all maps number ?????working????!!!
	Memory::CodeCave(AlwaysViewRestoreFix, dwAlwaysViewRestoreFix, dwAlwaysViewRestoreFixNOPs);

	//if (Client::CustomLoginFrame) {
	//	Memory::WriteInt(0x00629120 + 1, (unsigned int)floor(-nScreenHeight / 2));//push -300				!!game login frame!! turn this on if you edit UI.wz and use a frame that matches your res
	//	Memory::WriteInt(0x0062912F + 1, (unsigned int)floor(-nScreenWidth / 2));	//push -400 ; RelMove?				!!game login frame!! turn this on if you edit UI.wz and use a frame that matches your res
	//}

	//if (Client::bigLoginFrame) {
	//	Memory::WriteInt(0x00628EFB + 1, nScreenWidth - 145);	// 145?? mov eax,800 ; RelMove?	//game version number for login frames that hug the side of the screen //you will still need to offset ntop, and that may require a code cave if your height resolution is too big
	//}
	//else {
	//	nTopOfsettedVerFix = 10 + myHeight; nLeftOfsettedVerFix = 665 + myWidth; //parameters for fix version number
	//	Memory::CodeCave(VersionNumberFix, dwVersionNumberFix, dwVersionNumberFixNOPs);	//game version number fix //use this if you use no frame or default client frame
	//}

	//if (!Client::bigLoginFrame) {
	//	nHeightOfsettedLoginBackCanvasFix = 122 + myHeight; nWidthOfsettedLoginBackCanvasFix = 85 + myWidth;//para for world select buttonsViewRec
	//	nTopOfsettedLoginBackCanvasFix = 452 + myHeight; nLeftOfsettedLoginBackCanvasFix = 76 + myWidth;
	//	Memory::CodeCave(ccLoginBackCanvasFix, dwLoginBackCanvasFix, LoginBackCanvasFixNOPs);	//world select buttons fix		//thank you teto for pointing out my error in finding the constructor

	//	//yOffsetOfLoginBackBtnFix = 300 + myHeight; xOffsetOfLoginBackBtnFix = 0 + myWidth;	//para for back button
	//	//Memory::CodeCave(ccLoginBackBtnFix, dwLoginBackBtnFix, LoginBackBtnFixNOPs); //back button on world select //unnecessary as buttons move with canvas

	//	nHeightOfsettedLoginViewRecFix = 122 + myHeight; nWidthOfsettedLoginViewRecFix = 85 + myWidth;//para for ViewRec fix
	//	nTopOfsettedLoginViewRecFix = 452 + myHeight; nLeftOfsettedLoginViewRecFix = 76 + myWidth;
	//	Memory::CodeCave(ccLoginViewRecFix, dwLoginViewRecFix, LoginViewRecFixNOPs);	//world ViewRec fix	

	//	a1x = 0 + myWidth; a2x = -149 + myWidth; a2y = 0 + myHeight; a3 = 25; a1y = -250; //a4 = 0;	//LoginDescriptor params
	//	Memory::WriteInt(0x0063ABC4 + 1, 300 + a1y); //speed 1	//temporary fix by increasing the speed of display until i get good enough at procedural programming 
	//	//and memory management and reverse engineering to use nexon's own functions to put a black layer with greater z value to cover the tabs being shown off screen at origin
	//	Memory::CodeCave(ccLoginDescriptorFix, dwLoginDescriptorFix, LoginDescriptorFixNOPs);	//world LoginDescriptor fix	
	//}

	int customEngY = -62, customEngX = -22, dojangYoffset = 0;	//myHeight //-55-35 (myHeight*250/100)	-(myWidth*53/100) 140 -130
	yOffsetOfMuruengraidPlayer = 50 + dojangYoffset; xOffsetOfMuruengraidPlayer = 169 + myWidth; //params
	Memory::CodeCave(ccMuruengraidPlayer, dwMuruengraidPlayer, MuruengraidPlayerNOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidClock = 26 + dojangYoffset; xOffsetOfMuruengraidClock = 400 + myWidth; //params
	Memory::CodeCave(ccMuruengraidClock, dwMuruengraidClock, MuruengraidClockNOPs);	//muruengraid scaling
	yOffsetOfMuruengraidMonster = 50 + dojangYoffset; xOffsetOfMuruengraidMonster = 631 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster, dwMuruengraidMonster, MuruengraidMonsterNOPs);	//muruengraid scaling
	yOffsetOfMuruengraidMonster1 = 32 + dojangYoffset; xOffsetOfMuruengraidMonster1 = 317 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster1, dwMuruengraidMonster1, MuruengraidMonster1NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidMonster2 = 32 + dojangYoffset; xOffsetOfMuruengraidMonster2 = 482 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster2, dwMuruengraidMonster2, MuruengraidMonster2NOPs);	//muruengraid scaling
	yOffsetOfMuruengraidEngBar = 86 + dojangYoffset + customEngY; xOffsetOfMuruengraidEngBar = 17 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar, dwMuruengraidEngBar, MuruengraidEngBarNOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidEngBar1 = 130 + dojangYoffset + customEngY; xOffsetOfMuruengraidEngBar1 = 20 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar1, dwMuruengraidEngBar1, MuruengraidEngBar1NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidEngBar2 = 80 + dojangYoffset + customEngY; xOffsetOfMuruengraidEngBar2 = 9 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar2, dwMuruengraidEngBar2, MuruengraidEngBar2NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidClearRoundUI = 260 + myHeight; xOffsetOfMuruengraidClearRoundUI = 400 + myWidth; //params
	Memory::CodeCave(ccMuruengraidClearRoundUI, dwMuruengraidClearRoundUI, MuruengraidClearRoundUINOPs);	//muruengraid scaling
	//yOffsetOfMuruengraidTimerCanvas = 28 + dojangYoffset; xOffsetOfMuruengraidTimerCanvas = 112 + myWidth; //params
	//Memory::CodeCave(ccMuruengraidTimerCanvas, dwMuruengraidTimerCanvas, MuruengraidTimerCanvasNOPs);	//muruengraid scaling	
	//yOffsetOfMuruengraidTimerMinutes = 0 + dojangYoffset; xOffsetOfMuruengraidTimerMinutes = 0 + myWidth; //params	//not needed, bar moves all, kept for referrence or if change are needed
	//Memory::CodeCave(ccMuruengraidTimerMinutes, dwMuruengraidTimerMinutes, MuruengraidTimerMinutesNOPs);	//muruengraid scaling	
	//yOffsetOfMuruengraidTimerSeconds = 0 + dojangYoffset; xOffsetOfMuruengraidTimerSeconds = 68 + myWidth; //params
	//Memory::CodeCave(ccMuruengraidTimerSeconds, dwMuruengraidTimerSeconds, MuruengraidTimerSecondsNOPs);	//muruengraid scaling
	yOffsetOfMuruengraidTimerBar = 16 + dojangYoffset; xOffsetOfMuruengraidTimerBar = 345 + myWidth; //params
	Memory::CodeCave(ccMuruengraidTimerBar, dwMuruengraidTimerBar, MuruengraidTimerBarNOPs);	//muruengraid scaling
	xOffsetOfMuruengraidMonster1_2 = 318 + myWidth; //params	//finally fixed this bugger
	Memory::CodeCave(ccMuruengraidMonster1_2, dwMuruengraidMonster1_2, MuruengraidMonster1_2NOPs);

	//黑暗地图
	darkCircleX = nScreenWidth / 2 - 163;
	darkCircleY = nScreenHeight / 2 - 190;
	Memory::CodeCave(darkMap1cc, 0x00576456, 9);
	Memory::CodeCave(darkMap2cc, 0x005765EF, 12);
	Memory::CodeCave(darkMap3cc, 0x00576735, 13);
}

void Resolution::UpdateSlotPosition(int width) {
	m_nSlotWidth = width;
	//std::cout << "UpdateSlotPosition width = " << width << std::endl;
	if (m_nGameWidth <= 800) {
		m_slotXPos = m_nGameWidth;
		Memory::WriteInt(dwQuickSlotInitHPos + 1, 580); //push 647 //hd800
		Memory::WriteInt(dwQuickSlotHPos + 1, 580); //push 647 //hd800
		Memory::WriteInt(dwQuickSlotCWndHPos + 2, -580); //lea ebx,[eax-647]
		Memory::WriteInt(0x008E4448 + 1, 587);  //装备按钮宽度
		Memory::WriteInt(0x008E53FB + 1, 587);  //装备闪烁宽度
		Memory::WriteInt(0x008E44C5 + 1, 621);  //背包按钮宽度
		Memory::WriteInt(0x008E549E + 1, 621);  //背包闪烁宽度
		Memory::WriteInt(0x008E4542 + 1, 587);  //能力值按钮宽度
		Memory::WriteInt(0x008E5358 + 1, 587);  //能力值闪烁图标宽度
		Memory::WriteInt(0x008E45BF + 1, 621);  //技能按钮宽度
		Memory::WriteInt(0x008E5157 + 1, 621);  //技能按钮宽度
	}
	else {
		m_slotXPos = m_nGameWidth - width;
		if (m_nGameWidth < 1366 && width >= 540) {
			m_slotXPos = m_nGameWidth > 1024 ? 815 : 805;
		}
		Memory::WriteInt(dwQuickSlotInitVPos + 1, m_nGameHeight + 1);//add eax,533
		Memory::WriteInt(dwQuickSlotInitHPos + 1, m_slotXPos); //push 647 //hd800
		Memory::WriteInt(dwQuickSlotVPos + 2, m_nGameHeight + 1);//add esi,533
		Memory::WriteInt(dwQuickSlotHPos + 1, m_slotXPos); //push 647 //hd800
		Memory::WriteInt(dwQuickSlotCWndVPos + 2, -500); //lea edi,[eax-427]6
		Memory::WriteInt(dwQuickSlotCWndHPos + 2, -m_slotXPos); //lea ebx,[eax-647]
		Memory::WriteInt(0x008D6547 + 1, 540); //快捷面板宽度
		//4个快捷按钮位置
		Memory::WriteInt(0x008E4443 + 1, 506);  //装备按鈕高度
		Memory::WriteInt(0x008E4448 + 1, m_slotXPos + 7);  //装备按鈕寬度
		Memory::WriteInt(0x008E53F6 + 1, 506);  //装备闪烁高度
		Memory::WriteInt(0x008E53FB + 1, m_slotXPos + 7);  //装备闪烁寬度

		Memory::WriteInt(0x008E44C0 + 1, 506);  //背包按鈕高度
		Memory::WriteInt(0x008E44C5 + 1, m_slotXPos + 41);  //背包按鈕寬度
		Memory::WriteInt(0x008E5499 + 1, 506);  //背包闪烁高度
		Memory::WriteInt(0x008E549E + 1, m_slotXPos + 41);  //背包闪烁寬度

		Memory::WriteInt(0x008E453D + 1, 540);  //能力值按鈕高度
		Memory::WriteInt(0x008E4542 + 1, m_slotXPos + 7);  //能力值按鈕寬度
		Memory::WriteInt(0x008E5353 + 1, 540);  //能力值闪烁图标高度
		Memory::WriteInt(0x008E5358 + 1, m_slotXPos + 7);  //能力值闪烁图标宽度

		Memory::WriteInt(0x008E45BA + 1, 540);  //技能按鈕高度
		Memory::WriteInt(0x008E45BF + 1, m_slotXPos + 41);  //技能按鈕寬度
		Memory::WriteInt(0x008E5152 + 1, 540);  //技能按鈕高度
		Memory::WriteInt(0x008E5157 + 1, m_slotXPos + 41);  //技能按鈕寬度
	}
}

void Resolution::UpdateBarWidth(int width) {
	m_nBarWidth = width;
	/*std::cout << "UpdateBarWidth width = " << width << std::endl;*/
	Memory::WriteInt(0x008D6D40 + 1, width);//Bar底图
	nBarBackgrndWidth = m_nGameWidth;
	nBarBackgrndOpenWidth = m_slotXPos + 2;
	/*if (width >= m_nGameWidth) {
		nBarBackgrndWidth = m_nGameWidth;
		nBarBackgrndOpenWidth = slotXPos + 2;
	}
	else {
		float r = (float)width / m_nGameWidth;
		nBarBackgrndWidth = m_nGameWidth / r;
		nBarBackgrndOpenWidth = (slotXPos + 2) / r;
	}*/
}

void Resolution::updateStatResolution(unsigned int  backgrndWidth, unsigned int  backgrnd2Width)
{
	m_nBackgrndWidth = backgrndWidth;
	m_nBackgrnd2Width = backgrnd2Width;
	Memory::WriteInt(0x008CA0C2 + 1, backgrndWidth - 20); // 面板关闭按钮x  176-20
	Memory::WriteInt(0x008CA226 + 1, backgrndWidth); // 面板宽度       176
	Memory::WriteInt(0x008CA780 + 1, backgrnd2Width + Client::StatDetailBackgrndWidthEx); // 详情面板Rect宽度
	Memory::WriteInt(0x008CD155 + 1, backgrndWidth - 23); // 加属性按钮x    176-23
	Memory::WriteInt(0x008C7FCF + 1, backgrnd2Width - 23); // 详情面板关闭按钮x    177 - 23
	Memory::WriteInt(0x008CC2F5 + 1, backgrndWidth); // 移动时详情面板x
	// 偏移面板位置
	Memory::WriteInt(0x008CADD1 + 1, backgrndWidth);
	Memory::WriteInt(0x008CA519 + 1, backgrndWidth);
	statDetailBtnX = backgrndWidth - 52; // 详情按钮 176-52
	Memory::CodeCave(apDetailBtn, 0x008CA489, 7);
	statAutoBtnX = backgrndWidth - 79;   //自动加点 176-79
	Memory::CodeCave(apAutoBtn, 0x008CD3DC, 7);
}

class CWndMan : public TSingleton<CWndMan, 0x00BDD6F4> {
};

void Resolution::Init()
{
	Memory::CodeCave(enterGame, 0x00A1D906, 5);
	Memory::CodeCave(exitGame, 0x00A0DA58, 6);
	Memory::CodeCave(enterShop, 0x00863BC3, 5);
	////Memory::CodeCave(backgroundHook, 0x00427EB3, 5);
	_UpdateResolution(800, 600);
	//_UpdateResolution(Client::m_nGameWidth, Client::m_nGameHeight);
}

void Resolution::UpdateResolution(unsigned int nScreenWidth, unsigned int nScreenHeight)
{
	DWORD* IWzGr2DPtr = reinterpret_cast<DWORD*>(0x00BE2788);
	if (*IWzGr2DPtr == NULL)
		return;
	DWORD* D3DPtr = reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(Resolution::D3Dptr) + 36);

	//std::cout << *reinterpret_cast<DWORD*>((*IWzGr2DPtr)) << " " << *D3DPtr << " " << *reinterpret_cast<DWORD*>(Resolution::D3Dptr)
	//	<< " " << *reinterpret_cast<DWORD*>((*IWzGr2DPtr + 48))
	//	<< " " << *reinterpret_cast<DWORD*>((*IWzGr2DPtr + 32))
	//	<< " " << *reinterpret_cast<DWORD*>((*IWzGr2DPtr + 52))
	//	<< " " << *reinterpret_cast<DWORD*>((*IWzGr2DPtr + 36))
	//	<< " " << (int)(*reinterpret_cast<DWORD*>((*D3DPtr + 32)))
	//	<< " " << (int)(*reinterpret_cast<DWORD*>((*D3DPtr + 36)))
	//	<< " " << Client::m_nGameWidth << "->" << nScreenWidth
	//	<< " " << Client::m_nGameHeight << "->" << nScreenHeight
	//	<< std::endl;

	unsigned int width = 0;
	_UpdateResolution(nScreenWidth, nScreenHeight);
	UpdateSlotPosition(m_nSlotWidth);
	if (m_nBarWidth != 0)
		UpdateBarWidth(m_nBarWidth);

	//分辨率无限制
	//Memory::WriteByte(0x60E082, 0xEB);   //0x7E
	Memory::WriteInt(*IWzGr2DPtr + 48, nScreenWidth);
	Memory::WriteInt(*IWzGr2DPtr + 32, nScreenWidth);
	Memory::WriteInt(*IWzGr2DPtr + 52, nScreenHeight);
	Memory::WriteInt(*IWzGr2DPtr + 36, nScreenHeight);
	Memory::WriteInt(*IWzGr2DPtr + 148, D3DERR_DEVICENOTRESET);  // 0x88760869  D3DERR_DEVICENOTRESET
	Memory::WriteInt(*D3DPtr + 32, -floor(nScreenWidth / 2));
	Memory::WriteInt(*D3DPtr + 36, -floor(nScreenHeight / 2));
	SetWindowPos(FindWindow(L"MapleStoryClass", nullptr), HWND(-2), 0, 0, Client::m_nGameWidth, Client::m_nGameHeight, SWP_NOMOVE);
	//refreshMap();
	int full = 0;
	//getIWzGr2DPtr()->raw_RenderFrame();
}

