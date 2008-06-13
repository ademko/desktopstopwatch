
/**

  Desktop Stop Watch
  Copyright (C) 2008 Aleksander B. Demko

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  (and only version 2) as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License version 2 for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 **/

#include <iostream>

#include <wx/frame.h>
#include <wx/app.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/gbsizer.h>
#include <wx/stopwatch.h>
#include <wx/timer.h>

class WatchFrame : public wxFrame
{
    DECLARE_EVENT_TABLE()
  public:
    WatchFrame(wxWindow *parent);
  private:
    void OnStartStop(wxCommandEvent &event);
    void OnLap(wxCommandEvent &event);
    void OnReset(wxCommandEvent &event);
    void OnTick(wxTimerEvent &event);

    void UpdateLabel(void);
    void UpdateButtons(void);

  private:
    wxStopWatch dm_stopwatch;
    wxTimer dm_timer;

    enum {
      state_zero,
      state_running,
      state_stopped,
    };

    short dm_state;
    long dm_last_lap_start;

    wxStaticText *dm_main_label, *dm_lap_label;
    wxButton *dm_starttop_but, *dm_lap_but, *dm_reset_but;
};

class WatchApp : public wxApp
{
  public:
    bool OnInit(void);
};

enum {
  id_StartStop = 1,
  id_Lap,
  id_Reset,
  id_Tick,
};

static wxString TimeToString(long t)
{
  wxString output;

  output.Printf(L"%02d:%02d:%02d.%03d", t/(1000*60*60), t/(1000*60) %60, t/1000 %60, t%1000);

  return output;
}


BEGIN_EVENT_TABLE(WatchFrame, wxFrame)
  EVT_BUTTON(id_StartStop, WatchFrame::OnStartStop)
  EVT_BUTTON(id_Lap, WatchFrame::OnLap)
  EVT_BUTTON(id_Reset, WatchFrame::OnReset)
  EVT_TIMER(id_Tick, WatchFrame::OnTick)
END_EVENT_TABLE()

WatchFrame::WatchFrame(wxWindow *parent)
  : wxFrame(parent, -1, L"Desktop Stop Watch", wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN),
  dm_timer(this, id_Tick), dm_state(state_zero)
{
  wxGridBagSizer *sizer = new wxGridBagSizer;

  sizer->Add(dm_main_label = new wxStaticText(this, -1, L"00:00:00.000"), wxGBPosition(0,0), wxGBSpan(1,3), wxALIGN_CENTER);
  sizer->Add(dm_lap_label = new wxStaticText(this, -1, L"lap 00:00:00.000"), wxGBPosition(1,0), wxGBSpan(1,3), wxALIGN_CENTER);
  sizer->Add(dm_starttop_but = new wxButton(this, id_StartStop, L"Start"), wxGBPosition(2,0));
  sizer->Add(dm_lap_but = new wxButton(this, id_Lap, L"Lap"), wxGBPosition(2,1));
  sizer->Add(dm_reset_but = new wxButton(this, id_Reset, L"Reset"), wxGBPosition(2,2));

  dm_main_label->SetFont(wxFont(32, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
  dm_lap_label->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

  SetSizerAndFit(sizer);

  UpdateLabel();
  UpdateButtons();

  dm_timer.Start(219);
}

void WatchFrame::OnStartStop(wxCommandEvent &event)
{
  switch (dm_state) {
    case state_zero:
      dm_stopwatch.Start(0);
      dm_state = state_running;
      dm_last_lap_start = 0;
      break;
    case state_running:
      dm_stopwatch.Pause();
      dm_state = state_stopped;
      std::cout << "stop " << TimeToString(dm_stopwatch.Time()).mb_str() << '\n';
      break;
    case state_stopped:
      dm_stopwatch.Resume();
      dm_state = state_running;
      break;
  }
  UpdateButtons();
  UpdateLabel();
}

void WatchFrame::OnLap(wxCommandEvent &event)
{
  if (dm_state != state_running)
    return;

  long cur = dm_stopwatch.Time();

  wxString output = TimeToString(cur - dm_last_lap_start);

  dm_lap_label->SetLabel(L"lap " + output);

  std::cout << "lap " << output.mb_str() << '\n';

  dm_last_lap_start = cur;
}

void WatchFrame::OnReset(wxCommandEvent &event)
{
  switch (dm_state) {
    case state_stopped:
      dm_state = state_zero;
  }

  UpdateButtons();
  UpdateLabel();
  dm_lap_label->SetLabel(L"lap 00:00:00.000");
}

void WatchFrame::OnTick(wxTimerEvent &event)
{
  UpdateLabel();
}

void WatchFrame::UpdateLabel(void)
{
  if (dm_state == state_zero) {
    dm_main_label->SetLabel(L"00:00:00.000");
    return;
  }

  dm_main_label->SetLabel(TimeToString(dm_stopwatch.Time()));
}

void WatchFrame::UpdateButtons(void)
{
  switch (dm_state) {
    case state_zero:
        dm_starttop_but->SetLabel(L"Start");
        dm_lap_but->SetLabel(L"");
        dm_reset_but->SetLabel(L"");
        break;
    case state_running:
        dm_starttop_but->SetLabel(L"Stop");
        dm_lap_but->SetLabel(L"Lap");
        dm_reset_but->SetLabel(L"");
        break;
    case state_stopped:
        dm_starttop_but->SetLabel(L"Start");
        dm_lap_but->SetLabel(L"");
        dm_reset_but->SetLabel(L"Reset");
        break;
  }
}

bool WatchApp::OnInit(void)
{
  WatchFrame *frame = new WatchFrame(0);

  frame->Show();
  SetTopWindow(frame);
}

IMPLEMENT_APP(WatchApp)

