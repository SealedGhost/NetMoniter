#ifndef _WINMONITSET_H
#define _WINMONITSET_H
#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"
#include "DROPDOWN.h"

void _cbWindowMonitSet(WM_MESSAGE * pMsg);
void _cbWindowMessage(WM_MESSAGE * pMsg);
extern GUI_WIDGET_CREATE_INFO _aDialogCreate[3];
#endif
