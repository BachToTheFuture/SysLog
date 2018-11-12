/*
 * Copyright 1998 Will Dyson <will@cs.earlham.edu>
 * Copyright 2018-2019 Bach Nguyen <bach5000@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef APP_H
#define APP_H

#include <Application.h>

class BFilePanel;
class BMessage;

const uint32 kOpenFile	= 'Kopf';
const uint32 kQuit		= 'Kqit';

class App : public BApplication
{
public:
					App();
					~App();
	virtual void	MessageReceived(BMessage* msg);
	virtual void	RefsReceived(BMessage* msg);

private:
	BFilePanel*		openPrompt;
};

#endif
