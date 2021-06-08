#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Utility/hexPrint.h"

class StackListCtrl : public wxListCtrl {
public:
	StackListCtrl(wxWindow* parent, unsigned short* stackArray);
	wxString OnGetItemText(long item, long column) const;

private:
	unsigned short* stackArrayP;
};