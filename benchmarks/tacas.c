/*
 * tacas.c
 *
 *  Created on: 9 Sep 2019
 *      Author: xuanlinhha
 */

#include "../src/include/sym.h"
#include <stdio.h>
//#ifdef LLBMC
//#include <llbmc.h>
//#else
//#include <klee/klee.h>
//#endif
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int kappa = 0;
int Cur_Vertical_Sep;
int High_Confidence;
int Two_of_Three_Reports_Valid;
int Own_Tracked_Alt;
int Own_Tracked_Alt_Rate;
int Other_Tracked_Alt;
int Alt_Layer_Value;
int Positive_RA_Alt_Thresh__0;
int Positive_RA_Alt_Thresh__1;
int Positive_RA_Alt_Thresh__2;
int Positive_RA_Alt_Thresh__3;
int Up_Separation;
int Down_Separation;
int Other_RAC;
int Other_Capability;
int Climb_Inhibit;
int safety;

int main() {
  int enabled;
  int tcas_equipped;
  int intent_not_known;
  int need_upward_RA;
  int need_downward_RA;
  int alt_sep;
  int alim;
  int temp1;
  int temp2;
  int temp3;
  int temp4;
  int result_Non_Crossing_Biased_Climb;
  int result_Non_Crossing_Biased_Descend;
  int upward_preferred_1;
  int alim_Non_Crossing_Biased_Climb;
  int temp11;
  int temp12;
  int temp13;
  int upward_preferred_2;
  int alim_Non_Crossing_Biased_Descend;
  int temp21;
  int temp22;
  int temp23;
  int myCIL___tmp;
  {
    Cur_Vertical_Sep = symbolic_int("Cur_Vertical_Sep");
    High_Confidence = symbolic_int("High_Confidence");
    Two_of_Three_Reports_Valid = symbolic_int("Two_of_Three_Reports_Valid");
    Own_Tracked_Alt = symbolic_int("Own_Tracked_Alt");
    Own_Tracked_Alt_Rate = symbolic_int("Own_Tracked_Alt_Rate");
    Other_Tracked_Alt = symbolic_int("Other_Tracked_Alt");
    Alt_Layer_Value = symbolic_int("Alt_Layer_Value");
    Positive_RA_Alt_Thresh__0 = symbolic_int("Positive_RA_Alt_Thresh__0");
    Positive_RA_Alt_Thresh__1 = symbolic_int("Positive_RA_Alt_Thresh__1");
    Positive_RA_Alt_Thresh__2 = symbolic_int("Positive_RA_Alt_Thresh__2");
    Positive_RA_Alt_Thresh__3 = symbolic_int("Positive_RA_Alt_Thresh__3");
    Up_Separation = symbolic_int("Up_Separation");
    Down_Separation = symbolic_int("Down_Separation");
    Other_RAC = symbolic_int("Other_RAC");
    Other_Capability = symbolic_int("Other_Capability");
    Climb_Inhibit = symbolic_int("Climb_Inhibit");
    safety = symbolic_int("safety");
    enabled = symbolic_int("enabled");
    tcas_equipped = symbolic_int("tcas_equipped");
    intent_not_known = symbolic_int("intent_not_known");
    need_upward_RA = symbolic_int("need_upward_RA");
    need_downward_RA = symbolic_int("need_downward_RA");
    alt_sep = symbolic_int("alt_sep");
    alim = symbolic_int("alim");
    temp1 = symbolic_int("temp1");
    temp2 = symbolic_int("temp2");
    temp3 = symbolic_int("temp3");
    temp4 = symbolic_int("temp4");
    result_Non_Crossing_Biased_Climb =
        symbolic_int("result_Non_Crossing_Biased_Climb");
    result_Non_Crossing_Biased_Descend =
        symbolic_int("result_Non_Crossing_Biased_Descend");
    upward_preferred_1 = symbolic_int("upward_preferred_1");
    alim_Non_Crossing_Biased_Climb =
        symbolic_int("alim_Non_Crossing_Biased_Climb");
    temp11 = symbolic_int("temp11");
    temp12 = symbolic_int("temp12");
    temp13 = symbolic_int("temp13");
    upward_preferred_2 = symbolic_int("upward_preferred_2");
    alim_Non_Crossing_Biased_Descend =
        symbolic_int("alim_Non_Crossing_Biased_Descend");
    temp21 = symbolic_int("temp21");
    temp22 = symbolic_int("temp22");
    temp23 = symbolic_int("temp23");
    myCIL___tmp = symbolic_int("myCIL___tmp");

    myCIL___tmp = 0;
    Positive_RA_Alt_Thresh__0 = 400;
    Positive_RA_Alt_Thresh__1 = 500;
    Positive_RA_Alt_Thresh__2 = 640;
    Positive_RA_Alt_Thresh__3 = 740;
    enabled = 0;
    tcas_equipped = 0;
    intent_not_known = 0;
    need_upward_RA = 0;
    need_downward_RA = 0;
    if (Alt_Layer_Value == 0) {
      alim = Positive_RA_Alt_Thresh__0;
    }
    if (Alt_Layer_Value == 1) {
      alim = Positive_RA_Alt_Thresh__1;
    }
    if (Alt_Layer_Value == 2) {
      alim = Positive_RA_Alt_Thresh__2;
    }
    alim = Positive_RA_Alt_Thresh__3;
    if (High_Confidence != 0) {
      if (Own_Tracked_Alt_Rate <= 600) {
        if (Cur_Vertical_Sep > 600) {
          enabled = 1;
        }
      }
    }
    if (Other_Capability == 0) {
      tcas_equipped = 1;
    }
    if (Two_of_Three_Reports_Valid != 0) {
      if (Other_RAC == 0) {
        intent_not_known = 1;
      }
    }
    alt_sep = 0;
    if (enabled != 0) {
      if (tcas_equipped != 0) {
        if (intent_not_known != 0) {
          goto _L___0;
        } else {
          goto _L___1;
        }
      } else {
      _L___1:
        if (!(tcas_equipped != 0)) {
        _L___0:
          upward_preferred_1 = 0;
          result_Non_Crossing_Biased_Climb = 0;
          if (Alt_Layer_Value == 0) {
            alim_Non_Crossing_Biased_Climb = Positive_RA_Alt_Thresh__0;
          }
          if (Alt_Layer_Value == 1) {
            alim_Non_Crossing_Biased_Climb = Positive_RA_Alt_Thresh__1;
          }
          if (Alt_Layer_Value == 2) {
            alim_Non_Crossing_Biased_Climb = Positive_RA_Alt_Thresh__2;
          }
          alim_Non_Crossing_Biased_Climb = Positive_RA_Alt_Thresh__3;
          if (Climb_Inhibit == 1) {
            temp11 = Up_Separation + 100;
          } else {
            temp11 = Up_Separation;
          }
          if (temp11 > Down_Separation) {
            upward_preferred_1 = 1;
          }
          if (upward_preferred_1 == 1) {
            if (Own_Tracked_Alt < Other_Tracked_Alt) {
              temp12 = 1;
            } else {
              temp12 = 0;
            }
            if (!(temp12 != 0)) {
              result_Non_Crossing_Biased_Climb = 1;
            } else {
              if (temp12 != 0) {
                if (!(Down_Separation >= alim_Non_Crossing_Biased_Climb)) {
                  result_Non_Crossing_Biased_Climb = 1;
                }
              }
            }
          } else {
            if (Other_Tracked_Alt < Own_Tracked_Alt) {
              temp13 = 1;
            } else {
              temp13 = 0;
            }
            if (temp13 != 0) {
              if (Cur_Vertical_Sep >= 300) {
                if (Up_Separation >= alim_Non_Crossing_Biased_Climb) {
                  result_Non_Crossing_Biased_Climb = 1;
                }
              }
            }
          }
          temp1 = result_Non_Crossing_Biased_Climb;
          if (Own_Tracked_Alt < Other_Tracked_Alt) {
            temp2 = 1;
          } else {
            temp2 = 0;
          }
          if (temp1 != 0) {
            if (temp2 != 0) {
              need_upward_RA = 1;
            }
          }
          upward_preferred_2 = 0;
          result_Non_Crossing_Biased_Descend = 0;
          if (Alt_Layer_Value == 0) {
            alim_Non_Crossing_Biased_Descend = Positive_RA_Alt_Thresh__0;
          }
          if (Alt_Layer_Value == 1) {
            alim_Non_Crossing_Biased_Descend = Positive_RA_Alt_Thresh__1;
          }
          if (Alt_Layer_Value == 2) {
            alim_Non_Crossing_Biased_Descend = Positive_RA_Alt_Thresh__2;
          }
          alim_Non_Crossing_Biased_Descend = Positive_RA_Alt_Thresh__3;
          if (Climb_Inhibit == 1) {
            temp21 = Up_Separation + 100;
          } else {
            temp21 = Up_Separation;
          }
          if (temp21 > Down_Separation) {
            upward_preferred_2 = 1;
          }
          if (upward_preferred_2 == 1) {
            if (Own_Tracked_Alt < Other_Tracked_Alt) {
              temp22 = 1;
            } else {
              temp22 = 0;
            }
            if (temp22 != 0) {
              if (Cur_Vertical_Sep >= 300) {
                if (Down_Separation >= alim_Non_Crossing_Biased_Descend) {
                  result_Non_Crossing_Biased_Descend = 1;
                }
              }
            }
          } else {
            if (Other_Tracked_Alt < Own_Tracked_Alt) {
              temp23 = 1;
            } else {
              temp23 = 0;
            }
            if (!(temp23 != 0)) {
              result_Non_Crossing_Biased_Descend = 1;
            } else {
              if (temp23 != 0) {
                if (Up_Separation >= alim_Non_Crossing_Biased_Descend) {
                  result_Non_Crossing_Biased_Descend = 1;
                }
              }
            }
          }
          temp3 = result_Non_Crossing_Biased_Descend;
          if (Other_Tracked_Alt < Own_Tracked_Alt) {
            temp4 = 1;
          } else {
            temp4 = 0;
          }
          if (temp3 != 0) {
            if (temp4 != 0) {
              need_downward_RA = 1;
            }
          }
          if (need_upward_RA != 0) {
            if (need_downward_RA != 0) {
              alt_sep = 0;
            } else {
              goto _L;
            }
          } else {
          _L:
            if (need_upward_RA != 0) {
              alt_sep = 1;
            } else {
              if (need_downward_RA != 0) {
                safety = (Up_Separation + alim) + Down_Separation;
                alt_sep = 2;
              } else {
                alt_sep = 0;
              }
            }
          }
        }
      }
    }
    return 0;
  }
}

/*

#include <stdio.h>
#ifdef LLBMC
#include <llbmc.h>
#else
#include <klee/klee.h>
#endif
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int kappa = 0;
int Cur_Vertical_Sep;
int High_Confidence;
int Two_of_Three_Reports_Valid;
int Own_Tracked_Alt;
int Own_Tracked_Alt_Rate;
int Other_Tracked_Alt;
int Alt_Layer_Value;
int Positive_RA_Alt_Thresh__0;
int Positive_RA_Alt_Thresh__1;
int Positive_RA_Alt_Thresh__2;
int Positive_RA_Alt_Thresh__3;
int Up_Separation;
int Down_Separation;
int Other_RAC;
int Other_Capability;
int Climb_Inhibit;
int safety;

int main() {
  int enabled;
  int tcas_equipped;
  int intent_not_known;
  int need_upward_RA;
  int need_downward_RA;
  int alt_sep;
  int alim;
  int temp1;
  int temp2;
  int temp3;
  int temp4;
  int result_Non_Crossing_Biased_Climb;
  int result_Non_Crossing_Biased_Descend;
  int upward_preferred_1;
  int alim_Non_Crossing_Biased_Climb;
  int temp11;
  int temp12;
  int temp13;
  int upward_preferred_2;
  int alim_Non_Crossing_Biased_Descend;
  int temp21;
  int temp22;
  int temp23;
  int myCIL___tmp;
  {
    klee_make_symbolic(&Cur_Vertical_Sep, sizeof(int), "Cur_Vertical_Sep");
    klee_make_symbolic(&High_Confidence, sizeof(int), "High_Confidence");
    klee_make_symbolic(&Two_of_Three_Reports_Valid, sizeof(int),
"Two_of_Three_Reports_Valid"); klee_make_symbolic(&Own_Tracked_Alt, sizeof(int),
"Own_Tracked_Alt"); klee_make_symbolic(&Own_Tracked_Alt_Rate, sizeof(int),
"Own_Tracked_Alt_Rate"); klee_make_symbolic(&Other_Tracked_Alt, sizeof(int),
"Other_Tracked_Alt"); klee_make_symbolic(&Alt_Layer_Value, sizeof(int),
"Alt_Layer_Value"); klee_make_symbolic(&Positive_RA_Alt_Thresh__0, sizeof(int),
"Positive_RA_Alt_Thresh__0"); klee_make_symbolic(&Positive_RA_Alt_Thresh__1,
sizeof(int), "Positive_RA_Alt_Thresh__1");
    klee_make_symbolic(&Positive_RA_Alt_Thresh__2, sizeof(int),
"Positive_RA_Alt_Thresh__2"); klee_make_symbolic(&Positive_RA_Alt_Thresh__3,
sizeof(int), "Positive_RA_Alt_Thresh__3"); klee_make_symbolic(&Up_Separation,
sizeof(int), "Up_Separation"); klee_make_symbolic(&Down_Separation, sizeof(int),
"Down_Separation"); klee_make_symbolic(&Other_RAC, sizeof(int), "Other_RAC");
    klee_make_symbolic(&Other_Capability, sizeof(int), "Other_Capability");
    klee_make_symbolic(&Climb_Inhibit, sizeof(int), "Climb_Inhibit");
    klee_make_symbolic(&safety, sizeof(int), "safety");
    klee_make_symbolic(&enabled, sizeof(int), "enabled");
    klee_make_symbolic(&tcas_equipped, sizeof(int), "tcas_equipped");
    klee_make_symbolic(&intent_not_known, sizeof(int), "intent_not_known");
    klee_make_symbolic(&need_upward_RA, sizeof(int), "need_upward_RA");
    klee_make_symbolic(&need_downward_RA, sizeof(int), "need_downward_RA");
    klee_make_symbolic(&alt_sep, sizeof(int), "alt_sep");
    klee_make_symbolic(&alim, sizeof(int), "alim");
    klee_make_symbolic(&temp1, sizeof(int), "temp1");
    klee_make_symbolic(&temp2, sizeof(int), "temp2");
    klee_make_symbolic(&temp3, sizeof(int), "temp3");
    klee_make_symbolic(&temp4, sizeof(int), "temp4");
    klee_make_symbolic(&result_Non_Crossing_Biased_Climb, sizeof(int),
"result_Non_Crossing_Biased_Climb");
    klee_make_symbolic(&result_Non_Crossing_Biased_Descend, sizeof(int),
"result_Non_Crossing_Biased_Descend"); klee_make_symbolic(&upward_preferred_1,
sizeof(int), "upward_preferred_1");
    klee_make_symbolic(&alim_Non_Crossing_Biased_Climb, sizeof(int),
"alim_Non_Crossing_Biased_Climb"); klee_make_symbolic(&temp11, sizeof(int),
"temp11"); klee_make_symbolic(&temp12, sizeof(int), "temp12");
    klee_make_symbolic(&temp13, sizeof(int), "temp13");
    klee_make_symbolic(&upward_preferred_2, sizeof(int), "upward_preferred_2");
    klee_make_symbolic(&alim_Non_Crossing_Biased_Descend, sizeof(int),
"alim_Non_Crossing_Biased_Descend"); klee_make_symbolic(&temp21, sizeof(int),
"temp21"); klee_make_symbolic(&temp22, sizeof(int), "temp22");
    klee_make_symbolic(&temp23, sizeof(int), "temp23");
    klee_make_symbolic(&myCIL___tmp, sizeof(int), "myCIL___tmp");

    myCIL___tmp = 0;
    Positive_RA_Alt_Thresh__0 = 400;
    Positive_RA_Alt_Thresh__1 = 500;
    Positive_RA_Alt_Thresh__2 = 640;
    Positive_RA_Alt_Thresh__3 = 740;
    enabled = 0;
    tcas_equipped = 0;
    intent_not_known = 0;
    need_upward_RA = 0;
    need_downward_RA = 0;
    if (Alt_Layer_Value == 0) {
      alim = Positive_RA_Alt_Thresh__0;
    }
    if (Alt_Layer_Value == 1) {
      alim = Positive_RA_Alt_Thresh__1;
    }
    if (Alt_Layer_Value == 2) {
      alim = Positive_RA_Alt_Thresh__2;
    }
    alim = Positive_RA_Alt_Thresh__3;
    if (High_Confidence != 0) {
      if (Own_Tracked_Alt_Rate <= 600) {
        if (Cur_Vertical_Sep > 600) {
          enabled = 1;
        }
      }
    }
    if (Other_Capability == 0) {
      tcas_equipped = 1;
    }
    if (Two_of_Three_Reports_Valid != 0) {
      if (Other_RAC == 0) {
        intent_not_known = 1;
      }
    }
    alt_sep = 0;
    if (enabled != 0) {
      if (tcas_equipped != 0) {
        if (intent_not_known != 0) {
          goto _L___0;
        } else {
          goto _L___1;
        }
      } else {
      _L___1:
        if (!(tcas_equipped != 0)) {
        _L___0:
          upward_preferred_1 = 0;
          result_Non_Crossing_Biased_Climb = 0;
          if (Alt_Layer_Value == 0) {
            alim_Non_Crossing_Biased_Climb = Positive_RA_Alt_Thresh__0;
          }
          if (Alt_Layer_Value == 1) {
            alim_Non_Crossing_Biased_Climb = Positive_RA_Alt_Thresh__1;
          }
          if (Alt_Layer_Value == 2) {
            alim_Non_Crossing_Biased_Climb = Positive_RA_Alt_Thresh__2;
          }
          alim_Non_Crossing_Biased_Climb = Positive_RA_Alt_Thresh__3;
          if (Climb_Inhibit == 1) {
            temp11 = Up_Separation + 100;
          } else {
            temp11 = Up_Separation;
          }
          if (temp11 > Down_Separation) {
            upward_preferred_1 = 1;
          }
          if (upward_preferred_1 == 1) {
            if (Own_Tracked_Alt < Other_Tracked_Alt) {
              temp12 = 1;
            } else {
              temp12 = 0;
            }
            if (!(temp12 != 0)) {
              result_Non_Crossing_Biased_Climb = 1;
            } else {
              if (temp12 != 0) {
                if (!(Down_Separation >= alim_Non_Crossing_Biased_Climb)) {
                  result_Non_Crossing_Biased_Climb = 1;
                }
              }
            }
          } else {
            if (Other_Tracked_Alt < Own_Tracked_Alt) {
              temp13 = 1;
            } else {
              temp13 = 0;
            }
            if (temp13 != 0) {
              if (Cur_Vertical_Sep >= 300) {
                if (Up_Separation >= alim_Non_Crossing_Biased_Climb) {
                  result_Non_Crossing_Biased_Climb = 1;
                }
              }
            }
          }
          temp1 = result_Non_Crossing_Biased_Climb;
          if (Own_Tracked_Alt < Other_Tracked_Alt) {
            temp2 = 1;
          } else {
            temp2 = 0;
          }
          if (temp1 != 0) {
            if (temp2 != 0) {
              need_upward_RA = 1;
            }
          }
          upward_preferred_2 = 0;
          result_Non_Crossing_Biased_Descend = 0;
          if (Alt_Layer_Value == 0) {
            alim_Non_Crossing_Biased_Descend = Positive_RA_Alt_Thresh__0;
          }
          if (Alt_Layer_Value == 1) {
            alim_Non_Crossing_Biased_Descend = Positive_RA_Alt_Thresh__1;
          }
          if (Alt_Layer_Value == 2) {
            alim_Non_Crossing_Biased_Descend = Positive_RA_Alt_Thresh__2;
          }
          alim_Non_Crossing_Biased_Descend = Positive_RA_Alt_Thresh__3;
          if (Climb_Inhibit == 1) {
            temp21 = Up_Separation + 100;
          } else {
            temp21 = Up_Separation;
          }
          if (temp21 > Down_Separation) {
            upward_preferred_2 = 1;
          }
          if (upward_preferred_2 == 1) {
            if (Own_Tracked_Alt < Other_Tracked_Alt) {
              temp22 = 1;
            } else {
              temp22 = 0;
            }
            if (temp22 != 0) {
              if (Cur_Vertical_Sep >= 300) {
                if (Down_Separation >= alim_Non_Crossing_Biased_Descend) {
                  result_Non_Crossing_Biased_Descend = 1;
                }
              }
            }
          } else {
            if (Other_Tracked_Alt < Own_Tracked_Alt) {
              temp23 = 1;
            } else {
              temp23 = 0;
            }
            if (!(temp23 != 0)) {
              result_Non_Crossing_Biased_Descend = 1;
            } else {
              if (temp23 != 0) {
                if (Up_Separation >= alim_Non_Crossing_Biased_Descend) {
                  result_Non_Crossing_Biased_Descend = 1;
                }
              }
            }
          }
          temp3 = result_Non_Crossing_Biased_Descend;
          if (Other_Tracked_Alt < Own_Tracked_Alt) {
            temp4 = 1;
          } else {
            temp4 = 0;
          }
          if (temp3 != 0) {
            if (temp4 != 0) {
              need_downward_RA = 1;
            }
          }
          if (need_upward_RA != 0) {
            if (need_downward_RA != 0) {
              alt_sep = 0;
            } else {
              goto _L;
            }
          } else {
          _L:
            if (need_upward_RA != 0) {
              alt_sep = 1;
            } else {
              if (need_downward_RA != 0) {
                safety = (Up_Separation + alim) + Down_Separation;
                alt_sep = 2;
              } else {
                alt_sep = 0;
              }
            }
          }
        }
      }
    }
    return 0;
  }
}

*/
