#include "counterListCtrl.h"

std::string nameLookUp[6] = { "I", "Program Counter", "OpCode", "Stack Pointer", "Delay Timer", "Sound Timer" };

CounterListCtrl::CounterListCtrl(wxWindow* parent, unsigned short* i, unsigned short* pc, unsigned short* opcode, unsigned short* stackPointer, unsigned char* delayTimer, unsigned char* soundTimer) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL) {
	iP = i;
	pcP = pc;
	opcodeP = opcode;
	stackPointerP = stackPointer;
	delayTimerP = delayTimer;
	soundTimerP = soundTimer;

	wxListItem col0;
	col0.SetId(0);
	col0.SetText(_("Counter/Timer"));
	col0.SetWidth(100);
	InsertColumn(0, col0);

	wxListItem col1;
	col1.SetId(1);
	col1.SetText(_("Value"));
	col1.SetWidth(50);
	InsertColumn(1, col1);

	SetItemCount(5);
}

wxString CounterListCtrl::OnGetItemText(long item, long column) const {
	if (column == 0) {
		return wxString(nameLookUp[item]);
	}
	else {
		switch (item) {
		case 0:
			return wxString(hexPrintShort(*iP).c_str());
			break;
		case 1:
			return wxString(hexPrintShort(*pcP).c_str());
			break;
		case 2:
			return wxString(hexPrintShort(*opcodeP).c_str());
			break;
		case 3:
			return wxString(hexPrintShort(*stackPointerP).c_str());
			break;
		case 4:
			return wxString(hexPrintChar(*delayTimerP).c_str());
			break;
		case 5:
			return wxString(hexPrintChar(*soundTimerP).c_str());
			break;
		}
	}
}