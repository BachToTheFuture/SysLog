
#include <Application.h>
#include <InterfaceKit.h>
#include <StorageKit.h>

#include <fs_attr.h>
#include <stdlib.h>

#include "App.h"
#include "MainWindow.h"


MainWindow::MainWindow()
	:	BWindow(BRect(100,100,500,400),"SysLog",B_DOCUMENT_WINDOW, 0)
{
	BRect logFrame = Bounds();
	BRect logRect;
	
	AddMenu();
	
	// Text view
	logFrame.top = menuBar->Bounds().bottom + 1.0;
	logFrame.right -= B_V_SCROLL_BAR_WIDTH;
	logRect = logFrame;
	logRect.OffsetTo(B_ORIGIN);
	logRect.InsetBy(3.0,3.0);
	logView = new BTextView(logFrame, "log_view", logRect,
		B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_PULSE_NEEDED);
	scrollView = new BScrollView("scroll_view", logView,
		B_FOLLOW_ALL_SIDES, 0, false, true, B_NO_BORDER);
	AddChild(scrollView);
	
	logView->MakeFocus(true);
	logView->MakeEditable(false);
}

MainWindow::MainWindow(entry_ref ref)
	:	BWindow(BRect(100,100,500,400),"SysLog",B_DOCUMENT_WINDOW, 0)
{
	BFile logFile;
	BRect logFrame = Bounds();
	BRect logRect;
	
	AddMenu();
	
	// Text view
	logFrame.top = menuBar->Bounds().bottom + 1.0;
	logFrame.right -= B_V_SCROLL_BAR_WIDTH;
	logRect = logFrame;
	logRect.OffsetTo(B_ORIGIN);
	logRect.InsetBy(3.0,3.0);
	logView = new BTextView(logFrame, "log_view", logRect,
		B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_PULSE_NEEDED);
	scrollView = new BScrollView("scroll_view", logView,
		B_FOLLOW_ALL_SIDES, 0, false, true, B_NO_BORDER);
	AddChild(scrollView);
	
	logView->MakeFocus(true);
	logView->MakeEditable(false);
	
	char name[B_FILE_NAME_LENGTH];
	BEntry entry;
	off_t size;
	attr_info style_info;

	entry.SetTo(&ref, true);
	if (entry.InitCheck() == B_NO_ERROR)
	{
		entry.GetName(name);
		SetTitle(name);
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
		logView->SetText(text_buf, amt_read);
		delete[] text_buf;
	}
	Show();
}


void
MainWindow::AddMenu()
{
	BMenu* menu;
	BMenuItem* item;
	
	menuBar = new BMenuBar(Bounds(), "menu_bar");

	menu = new BMenu("File");
	menu->AddItem(item=new BMenuItem("Open" B_UTF8_ELLIPSIS,
		new BMessage(kOpenFile), 'O'));
	item->SetTarget(be_app);
	menu->AddItem(new BMenuItem("Close",
		new BMessage(B_QUIT_REQUESTED), 'W'));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Quit",
		new BMessage(kQuit), 'Q'));
	menuBar->AddItem(menu);
	
	AddChild(menuBar);
}

void
MainWindow::FrameResized(float width, float height)
{
	BRect logRect = logView->TextRect();
	
	logRect.right = logRect.left + 
		((width - B_V_SCROLL_BAR_WIDTH) - 3.0);
	logView->SetTextRect(logRect);
}

void
MainWindow::MessageReceived(BMessage *msg)
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
	be_app->Lock();
	int32 windows = be_app->CountWindows();
	be_app->Unlock();

	if (windows < 3)
	{
		be_app->PostMessage(B_QUIT_REQUESTED);
	}
	
	return true;
}
