#include <map>
#include <iostream>
#include <wx/wx.h>
#include <wx/event.h>

class ControlPicker : public wxDialog {
public:
	ControlPicker(wxWindow* parent, wxWindowID id, const wxString& title, std::map<int, int>& keys, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	~ControlPicker();

	void onKeyButtonClicked(wxCommandEvent& evt);
	void onKeyDown(wxKeyEvent& evt);
	void onConfirm(wxCommandEvent& evt);
	void onCancel(wxCommandEvent& evt);

private:
	const int nFieldWidth = 4;
	const int nFieldHeight = 4;

	wxButton** btn;
	wxButton* okBtn;
	wxButton* cancelBtn;

	bool waitingForKey = false;

	int keyWaiting = -1;

	std::map<int, int>* keysToBind;

	void connectKeyDownEvent(wxWindow* parent);

	DECLARE_EVENT_TABLE()
};