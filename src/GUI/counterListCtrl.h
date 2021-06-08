#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Utility/hexPrint.h"

class CounterListCtrl : public wxListCtrl {
public:
	CounterListCtrl(wxWindow* parent, unsigned short* i, unsigned short* pc, unsigned short* opcode, unsigned short* stackPointer, unsigned char* delayTimer, unsigned char* soundTimer);
	wxString OnGetItemText(long item, long column) const;

private:
	unsigned short* iP;
	unsigned short* pcP;
	unsigned short* opcodeP;
	unsigned short* stackPointerP;
	unsigned char* delayTimerP;
	unsigned char* soundTimerP;
};