#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Utility/hexPrint.h"

class MemListCtrl : public wxListCtrl {
public:
	MemListCtrl(wxWindow* parent, unsigned char* memArray);
	wxString OnGetItemText(long item, long column) const;

private:
	unsigned char* memArrayP;
};