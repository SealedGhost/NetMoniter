#include "GUI.h"
#include "string.h"

int GUI__strlen(const char GUI_UNI_PTR * s) {
  int r = -1;
  if (s) {
    do {
      r++;
    } while (*s++);
  }
  return r;
}

int GUI__strcmp(const char GUI_UNI_PTR * s0, const char GUI_UNI_PTR * s1) {
  if (s0 == NULL) {
    s0 = "";
  }
  if (s1 == NULL) {
    s1 = "";
  }
  do {
    if (*s0 != *s1) {
      return 1;
    }
    s1++;
  } while (*++s0);
  if (*s1) {
    return 1;    /* Not equal, since s1 is longer than s0 */
  }
  return 0;      /* Equal ! */
}

int GUI__strcmp_hp(GUI_HMEM  hs0, const char GUI_UNI_PTR * s1) {
  const char* s0 = NULL;
  if (hs0) {
    s0 = (const char*)GUI_ALLOC_h2p(hs0);
  }
  return GUI__strcmp(s0, s1);
}


int GUI__SetText(GUI_HMEM* phText, const char* s) {
  int r = 0;
  if (GUI__strcmp_hp(*phText, s) != 0) {            /* Make sure we have a quick out if nothing changes */
    GUI_HMEM hMem;
    hMem = GUI_ALLOC_AllocNoInit(GUI__strlen(s) + 1);
    if (hMem) {
      char* pMem;
      pMem = (char*) GUI_ALLOC_h2p(hMem);
      strcpy(pMem, s);
      GUI_ALLOC_FreePtr(phText);
      *phText = hMem;
      r = 1;
    }
  }
  return r;
}
