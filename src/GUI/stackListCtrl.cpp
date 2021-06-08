#include "stackListCtrl.h"

StackListCtrl::StackListCtrl(wxWindow* parent, unsigned short* stackArray) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL) {
	stackArrayP = stackArray;

	wxListItem col0;
	col0.SetId(0);
	col0.SetText(_("Index"));
	col0.SetWidth(50);
	InsertColumn(0, col0);

	wxListItem col1;
	col1.SetId(1);
	col1.SetText(_("Address"));
	col1.SetWidth(50);
	InsertColumn(1, col1);

	SetItemCount(16);
}

wxString StackListCtrl::OnGetItemText(long item, long column) const {
	if (column == 0) {
		return wxString(std::to_string(item).c_str());
	}
	else {
		return wxString(hexPrintShort(stackArrayP[item]).c_str());
	}
}