#pragma once

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/gbsizer.h>
#include "chip8.h"
#include "regListCtrl.h"
#include "counterListCtrl.h"
#include "stackListCtrl.h"
#include "memListCtrl.h"

wxDECLARE_EVENT(MEMVIEWER_CLOSE_EVT, wxCommandEvent);

class MemoryViewer : public wxFrame {
public:
	MemoryViewer(wxWindow* parent, chip8* machine, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
	~MemoryViewer();

	void updateMemoryViewer();

private:
	RegListCtrl* regListCtrl;
	CounterListCtrl* counterListCtrl;
	StackListCtrl* stackListCtrl;
	MemListCtrl* memListCtrl;

	chip8* chipMachine;

	const int closeMemoryViewerID = 99;
};