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