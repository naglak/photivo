/*******************************************************************************
**
** Photivo
**
** Copyright (C) 2009-2011 Michael Munzert <mail@mm-log.com>
** Copyright (C) 2011 Bernd Schoeler <brjohn@brother-john.net>
**
** This file is part of Photivo.
**
** Photivo is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License version 3
** as published by the Free Software Foundation.
**
** Photivo is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Photivo.  If not, see <http://www.gnu.org/licenses/>.
**
*******************************************************************************/
/**
** Displays the preview image and manages all interactions that happen directly
** on the image itself, e.g. zoom, crop, spot repair
**
** - Create ptMainWindow and the global ptTheme BEFORE you create ptViewWindow.
**/
#ifndef PTVIEWWINDOW_H
#define PTVIEWWINDOW_H

#include <QLine>
#include <QMenu>
#include <QGraphicsView>

#include "ptImage.h"
#include "ptMainWindow.h"
#include "ptReportOverlay.h"
#include "ptLineInteraction.h"
#include "ptSimpleRectInteraction.h"
#include "ptRichRectInteraction.h"
#include "ptGridInteraction.h"


///////////////////////////////////////////////////////////////////////////
//
// Custom types used in ViewWindow
//
///////////////////////////////////////////////////////////////////////////

enum ptInteraction {
  iaNone = 0,
  iaCrop = 1,
  iaSelectRect = 2, // simple rectangle selection: e.g. for spot WB
  iaDrawLine = 3    // draw a single straight line: e.g. for rotate angle
};



///////////////////////////////////////////////////////////////////////////
//
// class ptViewWindow
//
///////////////////////////////////////////////////////////////////////////
class ptViewWindow : public QGraphicsView {
Q_OBJECT

///////////////////////////////////////////////////////////////////////////
//
// PUBLIC members
//
///////////////////////////////////////////////////////////////////////////
public:
  ptViewWindow(QWidget* Parent, ptMainWindow* mainWin);
  ~ptViewWindow();

  inline ptInteraction interaction() const { return m_Interaction; }
  inline int zoomPercent() { return qRound(m_ZoomFactor * 100); }
  inline float zoomFactor() const { return m_ZoomFactor; }

  // Save (and later restore) current zoom settings. Takes care of
  // everything incl. ptSettings. RestoreZoom() also updates the
  // viewport accordingly.
  void SaveZoom();
  void RestoreZoom();

  // Show status overlay in the top left viewport corner.
  // For mode use ptStatus_ constants.
  void ShowStatus(short mode);
  void ShowStatus(const QString text);    // shown for 1.5sec

  // Star, stop, control interactions
  // - StartSimpleRect: Pass the function that is called after the
  //   selection finishes.
  void StartLine();
  void StartSimpleRect(void (*CB_SimpleRect)(const ptStatus, QRect));
  void StartCrop();
  ptRichRectInteraction* crop() const { return m_Crop; }

  void setGrid(const short enabled, const uint linesX, const uint linesY);
  void UpdateImage(const ptImage* relatedImage);
  void ZoomTo(float factor);  // 1.0 means 100%
  int ZoomToFit(const short withMsg = 1);  // fit complete image into viewport

///////////////////////////////////////////////////////////////////////////
//
// PROTECTED members
//
///////////////////////////////////////////////////////////////////////////
protected:
  void contextMenuEvent(QContextMenuEvent* event);
  void dragEnterEvent(QDragEnterEvent* event);
  void dropEvent(QDropEvent* event);
  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);
  void paintEvent(QPaintEvent* event);
  void resizeEvent(QResizeEvent* event);
  void mouseDoubleClickEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);


///////////////////////////////////////////////////////////////////////////
//
// PRIVATE members
//
///////////////////////////////////////////////////////////////////////////
private:
  const float MinZoom;
  const float MaxZoom;
  QList<float> ZoomFactors;   // steps for wheel zoom

  short m_CtrlIsPressed;
  ptLineInteraction* m_DrawLine;
  ptSimpleRectInteraction* m_SelectRect;
  ptRichRectInteraction* m_Crop;
  ptGridInteraction* m_Grid;
  ptInteraction m_Interaction;
  short m_LeftMousePressed;
  void (*m_CB_SimpleRect)(const ptStatus, QRect);
  short m_ZoomIsSaved;
  float m_ZoomFactor;
  float m_ZoomFactorSav;
  short m_ZoomModeSav;

  QGraphicsPixmapItem* m_8bitImageItem;
  QLine* m_DragDelta;
  QGraphicsScene* m_ImageScene;
  ptReportOverlay* m_StatusOverlay;
  ptReportOverlay* m_ZoomSizeOverlay;

  // context menu stuff
  void ConstructContextMenu();
  QAction* ac_ZoomFit;
  QAction* ac_Zoom100;
  QAction* ac_Mode_RGB;
  QAction* ac_Mode_Structure;
  QAction* ac_Mode_L;
  QAction* ac_Mode_A;
  QAction* ac_Mode_B;
  QAction* ac_Mode_Gradient;
  QAction* ac_Clip_Indicate;
  QAction* ac_Clip_Over;
  QAction* ac_Clip_Under;
  QAction* ac_Clip_R;
  QAction* ac_Clip_G;
  QAction* ac_Clip_B;
  QAction* ac_SensorClip;
  QAction* ac_SensorClipSep;
  QAction* ac_ShowTools;
  QAction* ac_ShowZoomBar;
  QAction* ac_Fullscreen;
  QActionGroup* ac_ModeGroup;

  ptMainWindow* MainWindow;


///////////////////////////////////////////////////////////////////////////
//
// PRIVATE slots
//
///////////////////////////////////////////////////////////////////////////
private slots:
  void finishInteraction(ptStatus ExitStatus);

  // context menu stuff
  void Menu_Clip_Indicate();
  void Menu_Clip_Over();
  void Menu_Clip_Under();
  void Menu_Clip_R();
  void Menu_Clip_G();
  void Menu_Clip_B();
  void Menu_SensorClip();
  void Menu_ShowZoomBar();
  void Menu_ShowTools();
  void Menu_Fullscreen();
  void Menu_ZoomFit();
  void Menu_Zoom100();
  void Menu_Mode();


///////////////////////////////////////////////////////////////////////////
//
// signals
//
///////////////////////////////////////////////////////////////////////////
signals:
  void keyChanged(QKeyEvent* event);
  void mouseChanged(QMouseEvent* event);

};

#endif
