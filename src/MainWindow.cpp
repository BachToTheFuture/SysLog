/*
 * Copyright 1998 Will Dyson <will@cs.earlham.edu>
 * Copyright 2018-2019 Bach Nguyen <bach5000@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "MainWindow.h"

#include <Application.h>
#include <InterfaceKit.h>
#include <LayoutBuilder.h>
#include <StorageKit.h>

#include "App.h"


MainWindow::MainWindow(entry_ref ref)
	:
	BWindow(BRect(50, 50, 550, 450),"SysLog",B_DOCUMENT_WINDOW, 0)
{
	BFile logFile;
	
	AddMenu();
	
	fLogView = new BTextView("log_view", B_WILL_DRAW);
	BRect logRect = fLogView->TextRect();
	logRect.OffsetTo(B_ORIGIN);
	logRect.InsetBy(5, 5);
	fLogView->SetTextRect(logRect);
	fLogView->MakeFocus(true);
	fLogView->MakeEditable(false);
	
	fScrollView = new BScrollView("scroll_view", fLogView,
		B_WILL_DRAW, false, true, B_NO_BORDER);
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.Add(fMenuBar)
		.Add(fScrollView)
	.End();

	// Read the text from entry_ref
	char title[9] = "SysLog: ";
	char name[B_FILE_NAME_LENGTH];
	BEntry entry;
	off_t size;
	entry.SetTo(&ref, true);
	if (entry.InitCheck() == B_NO_ERROR)
	{
		entry.GetName(name);
		strcat(title, name);
		SetTitle(title);
	}
	logFile.SetTo(&ref, B_READ_ONLY);
	if (logFile.InitCheck() == B_NO_ERROR)
	{
		logFile.GetSize(&size);
		char* text_buf = new char[size];
		if (text_buf == NULL)
		{
			// Out of memory
		}
		ssize_t amt_read;
		amt_read = logFile.Read((void*)text_buf, size);
		fLogView->SetText(text_buf, amt_read);
		delete[] text_buf;
	}
	Show();
}


void
MainWindow::AddMenu()
{
	BMenu* menu;
	BMenuItem* item;
	
	fMenuBar = new BMenuBar(Bounds(), "menu_bar");

	menu = new BMenu("File");
	menu->AddItem(item = new BMenuItem("Open" B_UTF8_ELLIPSIS,
		new BMessage(kOpenFile), 'O'));
	item->SetTarget(be_app);
	menu->AddItem(new BMenuItem("Quit",
		new BMessage(kQuit), 'Q'));
	fMenuBar->AddItem(menu);
}


void
MainWindow::FrameResized(float width, float height)
{
	BRect logRect = fLogView->TextRect();
	logRect.right = logRect.left + 
		((width - B_V_SCROLL_BAR_WIDTH) - 3.0);
	fLogView->SetTextRect(logRect);
}


void
MainWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case kQuit:
		{
			be_app->PostMessage(B_QUIT_REQUESTED, be_app);
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
