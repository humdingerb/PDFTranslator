/*
 * Copyright 2015, Gerasim Troeglazov, 3dEyes@gmail.com. All rights reserved.
 *
 * This file is part of PDFTranslator
 *
 * PDFTranslator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "ConfigView.h"
#include "PDFTranslator.h"

#include <stdio.h>


ConfigView::ConfigView(TranslatorSettings *settings)
	: BGroupView("PDFTranslator Settings", B_VERTICAL, 0)
{
	fSettings = settings;
	
	BPopUpMenu* dpiPopupMenu = new BPopUpMenu("popup_dpi");

	uint32 currentDPI = 
		fSettings->SetGetInt32(PDF_SETTING_DPI);

	_AddItemToMenu(dpiPopupMenu, "36",
		MSG_DPI_CHANGED, 36, currentDPI);
	_AddItemToMenu(dpiPopupMenu, "72",
		MSG_DPI_CHANGED, 72, currentDPI);
	_AddItemToMenu(dpiPopupMenu, "96",
		MSG_DPI_CHANGED, 96, currentDPI);
	_AddItemToMenu(dpiPopupMenu, "120",
		MSG_DPI_CHANGED, 120, currentDPI);
	_AddItemToMenu(dpiPopupMenu, "240",
		MSG_DPI_CHANGED, 240, currentDPI);
	_AddItemToMenu(dpiPopupMenu, "320",
		MSG_DPI_CHANGED, 320, currentDPI);
		
	fDPIField = new BMenuField("dpi", "DPI: ", dpiPopupMenu);

	
	BPopUpMenu* antialiasingPopupMenu = new BPopUpMenu("popup_antialiasing");

	uint32 currentAntialiasing = 
		fSettings->SetGetInt32(PDF_SETTING_ANTIALIASING);

	for (int i = 0; i <= 8; i++) {
		BString val;
		val << i;
		_AddItemToMenu(antialiasingPopupMenu, val.String(),
			MSG_ANTIALIASING_CHANGED, i, currentAntialiasing);
	}

	fAntialiasingField = new BMenuField("antialiasing", "Antialiasing bits: ", antialiasingPopupMenu);

	BAlignment leftAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_UNSET);

	BStringView *stringView = new BStringView("title", "PDF image translator");
	stringView->SetFont(be_bold_font);
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	float spacing = be_control_look->DefaultItemSpacing();
	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));

	char version[256];
	sprintf(version, "Version %d.%d.%d, %s",
		int(B_TRANSLATION_MAJOR_VERSION(PDF_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_MINOR_VERSION(PDF_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_REVISION_VERSION(PDF_TRANSLATOR_VERSION)),
		__DATE__);
	stringView = new BStringView("version", version);
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	stringView = new BStringView("my_copyright",
		B_UTF8_COPYRIGHT "2012-2015 Gerasim Troeglazov <3dEyes@gmail.com>");
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);
	
	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));

	AddChild(fDPIField);
	AddChild(fAntialiasingField);

	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));
	
	BString copyrightText;
	copyrightText << "\nBased on MuPDF 1.6\n\n"
	"MuPDF is free software: you can redistribute it and/or "
	"modify it under the terms of the Affero GNU General Pub"
	"lic License as published by the Free Software Foundatio"
	"n, either version 3 of the License, or (at your option)"
	" any later version.\n\n"
	"MuPDF is Copyright 2006-2013 Artifex Software, Inc.";
		
	fCopyrightView = new BTextView("CopyrightLibs");
	fCopyrightView->SetExplicitAlignment(leftAlignment);
	fCopyrightView->MakeEditable(false);
	fCopyrightView->MakeResizable(true);
	fCopyrightView->SetWordWrap(true);
	fCopyrightView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fCopyrightView->SetText(copyrightText.String());
	fCopyrightView->SetExplicitMinSize(BSize(300,200));

	BFont font;
	font.SetSize(font.Size() * 0.9);
	fCopyrightView->SetFontAndColor(&font, B_FONT_SIZE, NULL);
	AddChild(fCopyrightView);

	fCopyrightView->SetExplicitAlignment(leftAlignment);
	
	AddChild(BSpaceLayoutItem::CreateGlue());
	GroupLayout()->SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, 
		B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING);

	SetExplicitPreferredSize(GroupLayout()->MinSize());		
}


ConfigView::~ConfigView()
{
	fSettings->Release();
}


void
ConfigView::AllAttached()
{
	fDPIField->Menu()->SetTargetForItems(this);
	fAntialiasingField->Menu()->SetTargetForItems(this);
}


void
ConfigView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_DPI_CHANGED: {
			int32 value;
			if (message->FindInt32("value", &value) >= B_OK) {
				fSettings->SetGetInt32(PDF_SETTING_DPI, &value);
				fSettings->SaveSettings();
			}
			break;
		}
		case MSG_ANTIALIASING_CHANGED: {
			int32 value;
			if (message->FindInt32("value", &value) >= B_OK) {
				fSettings->SetGetInt32(PDF_SETTING_ANTIALIASING, &value);
				fSettings->SaveSettings();
			}
			break;
		}		
		default:
			BView::MessageReceived(message);
	}
}


void
ConfigView::_AddItemToMenu(BMenu* menu, const char* label,
	uint32 mess, uint32 value, uint32 current_value)
{
	BMessage* message = new BMessage(mess);
	message->AddInt32("value", value);
	BMenuItem* item = new BMenuItem(label, message);
	item->SetMarked(value == current_value);
	menu->AddItem(item);
}
