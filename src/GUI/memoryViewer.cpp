#include "memoryViewer.h"

wxDEFINE_EVENT(MEMVIEWER_CLOSE_EVT, wxCommandEvent);

MemoryViewer::MemoryViewer(wxWindow* parent, Chip8* machine, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style) {

	chipMachine = machine;

	regListCtrl = new RegListCtrl(this, chipMachine->getRegisters());
	counterListCtrl = new CounterListCtrl(this, chipMachine->getIPointer(), chipMachine->getPCPointer(), chipMachine->getOpCodePointer(), chipMachine->getStackPointerPointer(), chipMachine->getDelayTimerPointer(), chipMachine->getSoundTimerPointer());
	stackListCtrl = new StackListCtrl(this, chipMachine->getStack());
	memListCtrl = new MemListCtrl(this, chipMachine->getMemory());

	wxBoxSizer* topRow = new wxBoxSizer(wxHORIZONTAL);
	topRow->Add(regListCtrl, 1, wxEXPAND);
	topRow->Add(counterListCtrl, 1, wxEXPAND);
	topRow->Add(stackListCtrl, 1, wxEXPAND);

	wxBoxSizer* debugSizer = new wxBoxSizer(wxVERTICAL);
	debugSizer->Add(topRow, 1, wxEXPAND | wxRIGHT | wxLEFT | wxTOP | wxBOTTOM, 5);
	debugSizer->Add(memListCtrl, 1, wxEXPAND | wxRIGHT | wxLEFT | wxBOTTOM, 5);

	this->SetSizer(debugSizer);
	debugSizer->Fit(this);
	debugSizer->SetSizeHints(this);
	
}

MemoryViewer::~MemoryViewer() {
	wxCommandEvent evt(MEMVIEWER_CLOSE_EVT);
	ProcessWindowEvent(evt);
}

void MemoryViewer::updateMemoryViewer() {
	regListCtrl->RefreshItems(0, 15);
	counterListCtrl->RefreshItems(0, 4);
	stackListCtrl->RefreshItems(0, 15);
	memListCtrl->RefreshItems(0, 4095);
}