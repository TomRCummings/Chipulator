/*
cApp: wxWidgets wrapper that implements main(), handles SDL intialization & destruction, and creates cMain.
*/

#pragma once

#include <wx/wx.h>
#include <SDL.h>
#include "cMain.h"

class cApp : public wxApp {
public:
	cApp();
	~cApp();

	virtual bool OnInit();
	virtual int OnExit();
};