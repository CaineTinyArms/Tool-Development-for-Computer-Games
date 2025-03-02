//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitWelcome.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormWelcome *FormWelcome;
//---------------------------------------------------------------------------
__fastcall TFormWelcome::TFormWelcome(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormWelcome::Button1Click(TObject *Sender)
{
	ShellExecute(NULL, "open", "https://ko-fi.com/frankengraphics", "", NULL, SW_RESTORE);
	
}
//---------------------------------------------------------------------------
void __fastcall TFormWelcome::Button2Click(TObject *Sender)
{
	ShellExecute(NULL, "open", "https://www.patreon.com/frankengraphics", "", NULL, SW_RESTORE);	
}
//---------------------------------------------------------------------------
void __fastcall TFormWelcome::FormCreate(TObject *Sender)
{
	StaticText2->Caption = AnsiString("")
	+ "- bugfixes: save 2 subpalette sets to 1 file"
	+ "\n- bugfix: swap tiles with mmc2 mode enabled"
	;
	StaticText3->Caption = AnsiString("- More features for 'Pattern Paint' mode")
	+ "\n- Improvements to the Tiles [F3] tab"
	+ "\n- System palette librarian / browser"
	+ "\n- Basic export parser for metatile-based maps"
	+ "\n- Fixes to user-reported issues"
	;
	StaticText1->Caption = AnsiString("Thanks for using NEXXT! It's free && open source.")
	+ "\nHowever, it takes lots of time to develop,"
	+ " which wouldn't be possible for me on a normal dayjob."
	+ "\n\nIf possible, consider donating on ko-fi or subscribing to my patreon to help."
	+ " Thanks!  /FrankenGraphics"
	;
	StaticText4->Caption = AnsiString("Special thanks to:")
	+ "\nSarah (potatolain), hunterpage27"
	+ "\nwho donated or joined"
	+ "\nsince last feature version.";
}
//---------------------------------------------------------------------------


void __fastcall TFormWelcome::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	FormMain->CreateWelcomeConfig();
}
//---------------------------------------------------------------------------

