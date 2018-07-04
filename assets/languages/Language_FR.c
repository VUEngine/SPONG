/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <I18n.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern EntityDefinition FLAG_FRANCE_EN;


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

/*
 * IMPORTANT: Ensure that this file is encoded in Windows-1252 or ISO-8859-1 ("ANSI") to make use
 * of the full extended ASCII character set including special characters of European languages.
 *
 * The following character is meant to prevent editors from loading this file as UTF-8, since it doesn't
 * form part of a valid UTF-8 byte sequence: �
 */

const char* const LANGUAGE_FR_STRINGS[] =
{
	/* General */

	/* STR_ARE_YOU_SURE */				"Es tu s�r?",
	/* STR_AUTOMATIC_PAUSE */			"Pause Automatique",
	/* STR_BACK */						"Arri�re",
	/* STR_CREDITS */					"Cr�dits",
	/* STR_IMPORTANT */					"Important",
	/* STR_LANGUAGE */					"Langue",
	/* STR_LANGUAGE_SELECT */			"Choix de la langue",
	/* STR_NO */						"Non",
	/* STR_OFF */						"Desactiv�",
	/* STR_ON */						"Activ�",
	/* STR_OPTIONS */					"Options",
	/* STR_PAUSE */						"Pause",
	/* STR_PRESS_START_BUTTON */		"Appuyer sur Start",
	/* STR_PROGRESS_WILL_BE_ERASED */	"Tout v�tre progr�s sera effac�.",
	/* STR_SELECT */					"S�lectionner",
	/* STR_TAKE_A_REST */				"Se il vous pla�t, prendre un repos!",
	/* STR_YES */						"Oui",

	/* Splash Screens */

	/* STR_PRECAUTION_SCREEN_TEXT */	"Prenez conaissance de la\n\nnotice et des pr�cautions\n\na avant de jouer",
	/* STR_AUTO_PAUSE_EXPLANATION */	"  La fonction Pause automatique va\nvous rappelerez de prendre une pause\n  de jeu chaque 30 minutes environ",
};

const LangROMDef LANGUAGE_FR =
{
	// Language Name
	"Fran�ais",

	// Flag Entity
	&FLAG_FRANCE_EN,

	// Strings
	(const char**)LANGUAGE_FR_STRINGS
};
