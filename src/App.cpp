#include <Alert.h>
#include <Application.h>
#include <FilePanel.h>
#include <List.h>

#include "App.h"
#include "MainWindow.h"


App::App()
	:	BApplication("application/x-vnd.dw-TestApp")
{
	MainWindow *mainwin = new MainWindow();
	openPrompt = new BFilePanel();
	mainwin->Show();
}

App::~App()
{
	delete openPrompt;
}

void
App::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case kOpenFile:
		{
			openPrompt->Show();
			break;
		}
		
		default:
		{
			BApplication::MessageReceived(msg);
			break;
		}
	}
}

void
App::RefsReceived(BMessage* msg)
{
	int32 count;
	uint32 type;
	entry_ref ref;
	BEntry entry;
	MainWindow* window;
	
	msg->GetInfo("refs", &type, &count);
	if (type != B_REF_TYPE) return;
	
	for (int16 i = (count - 1); i >= 0; --i)
	{
		if (msg->FindRef("refs", i, &ref) == B_OK)
		{
			entry.SetTo(&ref, true);
			if ((entry.InitCheck() == B_NO_ERROR) && entry.Exists())
			{
				if (entry.IsFile())
					window = new MainWindow(ref);
			}
		}
	}
}

int
main (void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
