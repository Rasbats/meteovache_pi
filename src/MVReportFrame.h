/***************************************************************************
 *                                                                         *
 * Project:  meteovache_pi                                                 *
 * Purpose:  Weather forecast plugin for OpenCPN                           *
 * Author:   Ronan Demoment                                                *
 *                                                                         *
 ***************************************************************************
 *   Copyright (C) 2020 by Ronan Demoment                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#ifndef _MVREPORTFRAME_H__
#define _MVREPORTFRAME_H__

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <MeteoVacheThread.h>
#include <JobQueue.h>
#include <SpotForecasts.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include <wx/event.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class MVReportFrame: public wxDialog {
private:
	wxStaticText *MVReportModelLabel;
	wxComboBox *MVReportModelSelector;
	wxTextCtrl *MVReportTextArea;
	MeteoVacheThread *workerThread;
	SpotForecasts spotForecast;
	JobQueue *jobQueue;
	wxString selectedString;

	virtual void MVReportFrameOnClose(wxCloseEvent &event);
	virtual void MVModelOnSelect(wxCommandEvent &event);
    wxString getLatitudeString(float latitude);
    wxString getLongitudeString(float longitude);
    wxString getTextDirection(float windDirectionDeg);

    void startThread();
	void stopThread();

public:
	friend class MeteoVacheThread;

	MVReportFrame(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = _("MeteoVache"), const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxSize(497, 445), long style = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxSTAY_ON_TOP | wxTAB_TRAVERSAL);
	~MVReportFrame();

	void SetReportText(const wxString &text);
	void OnThreadEvent(wxCommandEvent&);
	void PublishWeatherReport(int model);
	void RequestForecast(float latitude, float longitude);
	const wxString &getSelectedModelName();
	void setSelectedModelName(wxString modelName);

	DECLARE_EVENT_TABLE()
};

#endif //_MVREPORTFRAME_H__
