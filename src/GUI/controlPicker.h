/*
ControlPicker: Dialog that displays the current keybindings and allows the user to change them. Takes a pointer to map<int,int> from the parent window which it copies into a temporary variable to hold changes until
the user pushes the "Ok" button, upon which it will copy the temporary map back to the address indicated by the pointer.
*/

#pragma once

#include <map>
#include <iostream>
#include <string>
#include <wx/wx.h>
#include <wx/event.h>

class ControlPicker : public wxDialog {
public:
	ControlPicker(wxWindow* parent, wxWindowID id, const wxString& title, std::map<int, int>& keys, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	~ControlPicker();

	//Sets the control to be bound on the next key press
	void onKeyButtonClicked(wxCommandEvent& evt);
	//If a button has been clicked, binds a key to the control corresponding with that button
	void onKeyDown(wxKeyEvent& evt);
	//Writes the temporary keybindings back to permanent address
	void onConfirm(wxCommandEvent& evt);
	//Closes window without saving temporary keybindings
	void onCancel(wxCommandEvent& evt);

private:
	//Dimensions of the button array
	const int nFieldWidth = 4;
	const int nFieldHeight = 5;

	//wxButton array
	wxButton** btn;
	//Ok and Cancel buttons
	wxButton* okBtn;
	wxButton* cancelBtn;

	//Keeps track of if a control is waiting for a new keybinding
	bool waitingForKey = false;
	//Keeps track of which control is waiting for a new keybinding
	int keyWaiting = -1;

	//Temporary keybinding map
	std::map<int, int>* keysToBind;

	//Function which propagates keyDown events
	void connectKeyDownEvent(wxWindow* parent);

	DECLARE_EVENT_TABLE()
};