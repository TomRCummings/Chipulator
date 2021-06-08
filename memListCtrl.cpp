#include "memListCtrl.h"

//IMPLEMENT MORE COLUMNS//

MemListCtrl::MemListCtrl(wxWindow* parent, unsigned char* memArray) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL) {
	memArrayP = memArray;

	wxListItem col0;
	col0.SetId(0);
	col0.SetText(_("Address"));
	col0.SetWidth(50);
	InsertColumn(0, col0);

	wxListItem col1;
	col1.SetId(1);
	col1.SetText(_("0x0"));
	col1.SetWidth(50);
	InsertColumn(1, col1);

	wxListItem col2;
	col2.SetId(2);
	col2.SetText(_("0x1"));
	col2.SetWidth(50);
	InsertColumn(2, col2);

	wxListItem col3;
	col3.SetId(3);
	col3.SetText(_("0x2"));
	col3.SetWidth(50);
	InsertColumn(3, col3);

	wxListItem col4;
	col4.SetId(4);
	col4.SetText(_("0x3"));
	col4.SetWidth(50);
	InsertColumn(4, col4);

	wxListItem col5;
	col5.SetId(5);
	col5.SetText(_("0x4"));
	col5.SetWidth(50);
	InsertColumn(5, col5);

	wxListItem col6;
	col6.SetId(6);
	col6.SetText(_("0x5"));
	col6.SetWidth(50);
	InsertColumn(6, col6);

	wxListItem col7;
	col7.SetId(7);
	col7.SetText(_("0x6"));
	col7.SetWidth(50);
	InsertColumn(7, col7);

	wxListItem col8;
	col8.SetId(8);
	col8.SetText(_("0x7"));
	col8.SetWidth(50);
	InsertColumn(8, col8);

	wxListItem col9;
	col9.SetId(9);
	col9.SetText(_("0x8"));
	col9.SetWidth(50);
	InsertColumn(9, col9);

	wxListItem col10;
	col10.SetId(10);
	col10.SetText(_("0x9"));
	col10.SetWidth(50);
	InsertColumn(10, col10);

	wxListItem col11;
	col11.SetId(11);
	col11.SetText(_("0xA"));
	col11.SetWidth(50);
	InsertColumn(11, col11);

	wxListItem col12;
	col12.SetId(12);
	col12.SetText(_("0xB"));
	col12.SetWidth(50);
	InsertColumn(12, col12);

	wxListItem col13;
	col13.SetId(13);
	col13.SetText(_("0xC"));
	col13.SetWidth(50);
	InsertColumn(13, col13);

	wxListItem col14;
	col14.SetId(14);
	col14.SetText(_("0xD"));
	col14.SetWidth(50);
	InsertColumn(14, col14);

	wxListItem col15;
	col15.SetId(15);
	col15.SetText(_("0xE"));
	col15.SetWidth(50);
	InsertColumn(15, col15);

	wxListItem col16;
	col16.SetId(16);
	col16.SetText(_("0xF"));
	col16.SetWidth(50);
	InsertColumn(16, col16);

	SetItemCount(256);
}

wxString MemListCtrl::OnGetItemText(long item, long column) const {
	if (column == 0) {
		return wxString(hexPrintChar((unsigned char)item).c_str());
	}
	else {
		return wxString(hexPrintChar(memArrayP[((item*16)+(column-1))]).c_str());
	}
}