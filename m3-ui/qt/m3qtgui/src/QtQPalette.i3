(*******************************************************************************
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
*******************************************************************************)

INTERFACE QtQPalette;

FROM QtNamespace IMPORT GlobalColor;



TYPE
  T = QPalette;


CONST (* Enum ColorGroup *)
  Active = 0;
  Disabled = 1;
  Inactive = 2;
  NColorGroups = 3;
  Current = 4;
  All = 5;
  Normal = 0;

TYPE (* Enum ColorGroup *)
  ColorGroup = [0..5];

CONST (* Enum ColorRole *)
  WindowText = 0;
  Button = 1;
  Light = 2;
  Midlight = 3;
  Dark = 4;
  Mid = 5;
  Text = 6;
  BrightText = 7;
  ButtonText = 8;
  Base = 9;
  Window = 10;
  Shadow = 11;
  Highlight = 12;
  HighlightedText = 13;
  Link = 14;
  LinkVisited = 15;
  AlternateBase = 16;
  NoRole = 17;
  ToolTipBase = 18;
  ToolTipText = 19;
  NColorRoles = 20;
  Foreground = 106;
  Background = 10;

TYPE (* Enum ColorRole *)
  ColorRole = [0..106];

TYPE
QPalette <: QPalettePublic;
QPalettePublic =
 BRANDED OBJECT
cxxObj:ADDRESS;
METHODS
init_0 () : QPalette;
init_1 (READONLY button: QColor;
) : QPalette;
init_2 (button: GlobalColor;
) : QPalette;
init_3 (READONLY button, window: QColor;
) : QPalette;
init_4 (READONLY windowText, button, light, dark, mid, text, bright_text, base, window: QBrush;
) : QPalette;
init_5 (READONLY windowText, window, light, dark, mid, text, base: QColor;
) : QPalette;
init_6 (READONLY palette: QPalette;
) : QPalette;
Op_Brush_Assign(READONLY palette: QPalette;
): QPalette;
currentColorGroup(): QPalette::ColorGroup;
setCurrentColorGroup(cg: QPalette::ColorGroup;
);
color(cg: QPalette::ColorGroup;
cr: QPalette::ColorRole;
): QColor;
brush(cg: QPalette::ColorGroup;
cr: QPalette::ColorRole;
): QBrush;
setColor(cg: QPalette::ColorGroup;
cr: QPalette::ColorRole;
READONLY color: QColor;
);
setColor1(cr: QPalette::ColorRole;
READONLY color: QColor;
);
setBrush(cr: QPalette::ColorRole;
READONLY brush: QBrush;
);
isBrushSet(cg: QPalette::ColorGroup;
cr: QPalette::ColorRole;
): BOOLEAN;
setBrush1(cg: QPalette::ColorGroup;
cr: QPalette::ColorRole;
READONLY brush: QBrush;
);
setColorGroup(cr: QPalette::ColorGroup;
READONLY windowText, button, light, dark, mid, text, bright_text, base, window: QBrush;
);
isEqual(cr1, cr2: QPalette::ColorGroup;
): BOOLEAN;
color1(cr: QPalette::ColorRole;
): QColor;
brush1(cr: QPalette::ColorRole;
): QBrush;
foreground(): QBrush;
windowText(): QBrush;
button(): QBrush;
light(): QBrush;
dark(): QBrush;
mid(): QBrush;
text(): QBrush;
base(): QBrush;
alternateBase(): QBrush;
toolTipBase(): QBrush;
toolTipText(): QBrush;
background(): QBrush;
window(): QBrush;
midlight(): QBrush;
brightText(): QBrush;
buttonText(): QBrush;
shadow(): QBrush;
highlight(): QBrush;
highlightedText(): QBrush;
link(): QBrush;
linkVisited(): QBrush;
Op_Brush_Equals(READONLY p: QPalette;
): BOOLEAN;
Op_Brush_NotEquals(READONLY p: QPalette;
): BOOLEAN;
isCopyOf(READONLY p: QPalette;
): BOOLEAN;
serialNumber(): INTEGER;
cacheKey(): CARDINAL;
resolve(READONLY arg1: QPalette;
): QPalette;
resolve1(): CARDINAL;
resolve2(mask: CARDINAL;
);
destroyCxx();
END;


END QtQPalette.
