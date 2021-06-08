#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Utility/hexPrint.h"

class RegListCtrl : public wxListCtrl {
public:
	RegListCtrl(wxWindow* parent, unsigned char* registerArray);
	wxString OnGetItemText(long item, long column) const;

private:
	unsigned char* registerArrayP;
};