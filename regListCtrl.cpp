#include "regListCtrl.h"

RegListCtrl::RegListCtrl(wxWindow* parent, unsigned char* registerArray) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL) {
	registerArrayP = registerArray;
	
	wxListItem col0;
	col0.SetId(0);
	col0.SetText(_("Address"));
	col0.SetWidth(50);
	InsertColumn(0, col0);

	wxListItem col1;
	col1.SetId(1);
	col1.SetText(_("Value"));
	col1.SetWidth(50);
	InsertColumn(1, col1);

	SetItemCount(16);
}

wxString RegListCtrl::OnGetItemText(long item, long column) const {
	if (column == 0) {
		return wxString(hexPrintChar((unsigned char)item).c_str());
	}
	else {
		return wxString(hexPrintChar(registerArrayP[item]).c_str());
	}
}