/*
 * Copyright 2018-2019 Bach Nguyen <bach5000@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MessageFilter.h>
#include <TextView.h>
#include <Window.h>

class MainWindow : public BWindow
{
public:
						MainWindow();
						MainWindow(entry_ref ref);
		virtual void	MessageReceived(BMessage *msg);
		virtual bool	QuitRequested(void);
		virtual void	FrameResized(float width, float height);
			
private:
		void			AddMenu();
		BTextView*		logView;
		BMenuBar* 		menuBar;
		BScrollView* 	scrollView;
};

#endif
