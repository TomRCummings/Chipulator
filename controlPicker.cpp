#include "controlPicker.h"

const char indexToChip8Char[16] = { '1', '2', '3', 'C', '4', '5', '6', 'D', '7', '8', '9', 'E', 'A', '0', 'B', 'F' };
const int indexToChip8Key[16] = { 1, 2, 3, 12, 4, 5, 6, 13, 7, 8, 9, 14, 10, 0, 11, 15 };

wxBEGIN_EVENT_TABLE(ControlPicker, wxDialog)
	EVT_KEY_DOWN(ControlPicker::onKeyDown)
wxEND_EVENT_TABLE()

ControlPicker::ControlPicker(wxWindow* parent, wxWindowID id, const wxString& title, std::map<int, int>& keys, const wxPoint& pos, const wxSize& size, long style)
: wxDialog(parent, id, title, pos, size, style){

	keysToBind = &keys;

	int totalField = nFieldWidth * nFieldHeight;
	btn = new wxButton*[totalField];
	wxGridSizer* btnGrid = new wxGridSizer(nFieldHeight, nFieldWidth, 0, 0);

	for (int y = 0; y < nFieldWidth; y++) {
		for (int x = 0; x < nFieldHeight; x++) {
			std::string btnString(1, indexToChip8Char[y * nFieldWidth + x]);

			std::map<int, int>::iterator itr = keysToBind->find(indexToChip8Key[y * nFieldWidth + x]);
			if (itr != keysToBind->end()) {
				char keyValueAsChar = itr->second;
				std::string keyValueAsString(1, keyValueAsChar);

				btnString.append(": ");
				btnString.append(keyValueAsString);
			}

			btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x), btnString);
			btnGrid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);

			btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ControlPicker::onKeyButtonClicked, this);
		}
	}

	okBtn = new wxButton(this, 20000, wxT("OK"));
	okBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ControlPicker::onConfirm, this);
	cancelBtn = new wxButton(this, 20001, wxT("Cancel"));
	cancelBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ControlPicker::onCancel, this);

	wxBoxSizer* stdBtnSizer = new wxBoxSizer(wxHORIZONTAL);
	stdBtnSizer->Add(okBtn);
	stdBtnSizer->Add(cancelBtn);

	wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(btnGrid, 1, wxEXPAND);
	boxSizer->Add(stdBtnSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

	stdBtnSizer->Layout();
	btnGrid->Layout();
	boxSizer->Layout();

	this->SetSizer(boxSizer);

	this->connectKeyDownEvent(this);
}

ControlPicker::~ControlPicker() {
	delete[]btn;
}

void ControlPicker::onKeyButtonClicked(wxCommandEvent &evt) {

	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;

	if (x == 0) {
		if (y == 0) {
			keyWaiting = 1;
		}
		else if (y == 1) {
			keyWaiting = 4;
		}
		else if (y == 2) {
			keyWaiting = 7;
		}
		else if (y == 3) {
			keyWaiting = 10;
		}
	}
	else if (x == 1) {
		if (y == 0) {
			keyWaiting = 2;
		}
		else if (y == 1) {
			keyWaiting = 5;
		}
		else if (y == 2) {
			keyWaiting = 8;
		}
		else if (y == 3) {
			keyWaiting = 0;
		}
	}
	else if (x == 2) {
		if (y == 0) {
			keyWaiting = 3;
		}
		else if (y == 1) {
			keyWaiting = 6;
		}
		else if (y == 2) {
			keyWaiting = 9;
		}
		else if (y == 3) {
			keyWaiting = 11;
		}
	}
	else if (x == 3) {
		if (y == 0) {
			keyWaiting = 12;
		}
		else if (y == 1) {
			keyWaiting = 13;
		}
		else if (y == 2) {
			keyWaiting = 14;
		}
		else if (y == 3) {
			keyWaiting = 15;
		}
	}

	std::string oldText = btn[y * nFieldWidth + x]->GetLabelText();
	std::string newText = oldText.substr(0, 3);
	newText.append("?");
	btn[y * nFieldWidth + x]->SetLabelText(newText);

	waitingForKey = true;

	evt.Skip();
}

void ControlPicker::onKeyDown(wxKeyEvent& evt) {

	if (waitingForKey) {
		wxChar uc = evt.GetUnicodeKey();
		int oldValue;

		bool keyBoundAlready = false;
		int keyToSwapWith;

		std::map<int, int>::iterator itr;
		for (itr = keysToBind->begin(); itr != keysToBind->end(); itr++) {
			if (itr->first != keyWaiting && itr->second == uc) {
				keyBoundAlready = true;
				keyToSwapWith = itr->first;
			}
		}
		
		itr = keysToBind->find(keyWaiting);
		if (itr != keysToBind->end()) {
			oldValue = itr->second;
			itr->second = uc;

			for (int i = 0; i < 16; i++) {
				if (indexToChip8Key[i] == keyWaiting) {
					std::string oldText = btn[i]->GetLabelText();
					std::string newText = oldText.substr(0, 3);

					char keyValueAsChar = uc;
					std::string keyValueAsString(1, keyValueAsChar);

					newText.append(keyValueAsString);

					btn[i]->SetLabelText(newText);
				}
			}

			if (keyBoundAlready) {
				itr = keysToBind->find(keyToSwapWith);

				if (itr != keysToBind->end()) {
					itr->second = oldValue;

					for (int i = 0; i < 16; i++) {
						if (indexToChip8Key[i] == keyToSwapWith) {
							std::string oldText = btn[i]->GetLabelText();
							std::string newText = oldText.substr(0, 3);

							char keyValueAsChar = oldValue;
							std::string keyValueAsString(1, keyValueAsChar);

							newText.append(keyValueAsString);

							btn[i]->SetLabelText(newText);
						}
					}
				}
			}
		}
	}

	evt.Skip();
}

void ControlPicker::onConfirm(wxCommandEvent& evt) {
	EndModal(wxID_OK);
}

void ControlPicker::onCancel(wxCommandEvent& evt) {
	EndModal(wxID_CANCEL);
}

void ControlPicker::connectKeyDownEvent(wxWindow* parent) {
	if (parent) {
		parent->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(ControlPicker::onKeyDown), (wxObject*)NULL, this);

		wxWindowListNode* pc1Node = parent->GetChildren().GetFirst();
		while (pc1Node) {
			wxWindow* pc1Child = pc1Node->GetData();
			this->connectKeyDownEvent(pc1Child);

			pc1Node = pc1Node->GetNext();
		}
	}
}
