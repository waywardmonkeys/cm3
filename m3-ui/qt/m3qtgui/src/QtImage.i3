(*******************************************************************************
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
*******************************************************************************)

INTERFACE QtImage;

FROM QtSize IMPORT QSize;
IMPORT QtRgb;
FROM QtByteArray IMPORT QByteArray;
FROM QtColor IMPORT QColor;
FROM QtMatrix IMPORT QMatrix;
FROM QGuiStubs IMPORT QPaintEngine, QIODevice;
FROM QtPoint IMPORT QPoint;
FROM QtTransform IMPORT QTransform;
FROM QtRect IMPORT QRect;
FROM QtNamespace IMPORT GlobalColor, ImageConversionFlags, AspectRatioMode,
                        TransformationMode, MaskMode;


FROM QtPaintDevice IMPORT QPaintDevice;
TYPE T = QImage;


TYPE                             (* Enum InvertMode *)
  InvertMode = {InvertRgb, InvertRgba};

TYPE                             (* Enum Format *)
  Format = {Format_Invalid, Format_Mono, Format_MonoLSB, Format_Indexed8,
            Format_RGB32, Format_ARGB32, Format_ARGB32_Premultiplied,
            Format_RGB16, Format_ARGB8565_Premultiplied, Format_RGB666,
            Format_ARGB6666_Premultiplied, Format_RGB555,
            Format_ARGB8555_Premultiplied, Format_RGB888, Format_RGB444,
            Format_ARGB4444_Premultiplied, NImageFormats};
PROCEDURE Image_TrueMatrix (arg1: QMatrix; w, h: INTEGER; ): QMatrix;

PROCEDURE Image_TrueMatrix1 (arg1: QTransform; w, h: INTEGER; ):
  QTransform;

PROCEDURE Image_FromData (data: ADDRESS; size: INTEGER; format: TEXT; ):
  QImage;

PROCEDURE Image_FromData1 (data: ADDRESS; size: INTEGER; ): QImage;

PROCEDURE Image_FromData2 (data: QByteArray; format: TEXT; ): QImage;

PROCEDURE Image_FromData3 (data: QByteArray; ): QImage;


TYPE
  QImage <: QImagePublic;
  QImagePublic =
    QPaintDevice BRANDED OBJECT
    METHODS
      init_0 (): QImage;
      init_1 (size: QSize; format: Format; ): QImage;
      init_2 (width, height: INTEGER; format: Format; ): QImage;
      init_3 (data: ADDRESS; width, height: INTEGER; format: Format; ):
              QImage;
      init_4 (data: ADDRESS; width, height: INTEGER; format: Format; ):
              QImage;
      init_5 (data                       : ADDRESS;
              width, height, bytesPerLine: INTEGER;
              format                     : Format;  ): QImage;
      init_6 (data                       : ADDRESS;
              width, height, bytesPerLine: INTEGER;
              format                     : Format;  ): QImage;
      init_7           (fileName, format: TEXT; ): QImage;
      init_8           (fileName: TEXT; ): QImage;
      init_9           (fileName, format: TEXT; ): QImage;
      init_10          (fileName: TEXT; ): QImage;
      init_11          (arg1: QImage; ): QImage;
      swap             (other: QImage; );
      isNull           (): BOOLEAN;
      devType          (): INTEGER; (* virtual *)
      detach           ();
      isDetached       (): BOOLEAN;
      copy             (rect: QRect; ): QImage;
      copy1            (): QImage;
      copy2            (x, y, w, h: INTEGER; ): QImage;
      format           (): Format;
      convertToFormat  (f: Format; flags: ImageConversionFlags; ): QImage;
      convertToFormat1 (f: Format; ): QImage;
      width            (): INTEGER;
      height           (): INTEGER;
      size             (): QSize;
      rect             (): QRect;
      depth            (): INTEGER;
      colorCount       (): INTEGER;
      bitPlaneCount    (): INTEGER;
      color            (i: INTEGER; ): QtRgb.T;
      setColor         (i: INTEGER; c: QtRgb.T; );
      setColorCount    (arg1: INTEGER; );
      allGray          (): BOOLEAN;
      isGrayscale      (): BOOLEAN;
      bits             (): ADDRESS;
      bits1            (): ADDRESS;
      constBits        (): ADDRESS;
      byteCount        (): INTEGER;
      scanLine         (arg1: INTEGER; ): ADDRESS;
      scanLine1        (arg1: INTEGER; ): ADDRESS;
      constScanLine    (arg1: INTEGER; ): ADDRESS;
      bytesPerLine     (): INTEGER;
      valid            (x, y: INTEGER; ): BOOLEAN;
      valid1           (pt: QPoint; ): BOOLEAN;
      pixelIndex       (x, y: INTEGER; ): INTEGER;
      pixelIndex1      (pt: QPoint; ): INTEGER;
      pixel            (x, y: INTEGER; ): QtRgb.T;
      pixel1           (pt: QPoint; ): QtRgb.T;
      setPixel         (x, y: INTEGER; index_or_rgb: CARDINAL; );
      setPixel1        (pt: QPoint; index_or_rgb: CARDINAL; );
      fill             (pixel: CARDINAL; );
      fill1            (color: QColor; );
      fill2            (color: GlobalColor; );
      hasAlphaChannel  (): BOOLEAN;
      setAlphaChannel  (alphaChannel: QImage; );
      alphaChannel     (): QImage;
      createAlphaMask  (flags: ImageConversionFlags; ): QImage;
      createAlphaMask1 (): QImage;
      createHeuristicMask  (clipTight: BOOLEAN; ): QImage;
      createHeuristicMask1 (): QImage;
      createMaskFromColor  (color: QtRgb.T; mode: MaskMode; ): QImage;
      createMaskFromColor1 (color: QtRgb.T; ): QImage;
      scaled (w, h      : INTEGER;
              aspectMode: AspectRatioMode;
              mode      : TransformationMode; ): QImage;
      scaled1 (w, h: INTEGER; aspectMode: AspectRatioMode; ): QImage;
      scaled2 (w, h: INTEGER; ): QImage;
      scaled3 (s         : QSize;
               aspectMode: AspectRatioMode;
               mode      : TransformationMode; ): QImage;
      scaled4         (s: QSize; aspectMode: AspectRatioMode; ): QImage;
      scaled5         (s: QSize; ): QImage;
      scaledToWidth   (w: INTEGER; mode: TransformationMode; ): QImage;
      scaledToWidth1  (w: INTEGER; ): QImage;
      scaledToHeight  (h: INTEGER; mode: TransformationMode; ): QImage;
      scaledToHeight1 (h: INTEGER; ): QImage;
      transformed  (matrix: QMatrix; mode: TransformationMode; ): QImage;
      transformed1 (matrix: QMatrix; ): QImage;
      transformed2 (matrix: QTransform; mode: TransformationMode; ):
                    QImage;
      transformed3  (matrix: QTransform; ): QImage;
      mirrored      (horizontally, vertically: BOOLEAN; ): QImage;
      mirrored1     (horizontally: BOOLEAN; ): QImage;
      mirrored2     (): QImage;
      rgbSwapped    (): QImage;
      invertPixels  (arg1: InvertMode; );
      invertPixels1 ();
      load          (device: QIODevice; format: TEXT; ): BOOLEAN;
      load1         (fileName, format: TEXT; ): BOOLEAN;
      load2         (fileName: TEXT; ): BOOLEAN;
      loadFromData  (buf: ADDRESS; len: INTEGER; format: TEXT; ): BOOLEAN;
      loadFromData1 (buf: ADDRESS; len: INTEGER; ): BOOLEAN;
      loadFromData2 (data: QByteArray; aformat: TEXT; ): BOOLEAN;
      loadFromData3 (data: QByteArray; ): BOOLEAN;
      save          (fileName, format: TEXT; quality: INTEGER; ): BOOLEAN;
      save1         (fileName, format: TEXT; ): BOOLEAN;
      save2         (fileName: TEXT; ): BOOLEAN;
      save3 (device: QIODevice; format: TEXT; quality: INTEGER; ): BOOLEAN;
      save4 (device: QIODevice; format: TEXT; ): BOOLEAN;
      save5 (device: QIODevice; ): BOOLEAN;
      serialNumber     (): INTEGER;
      cacheKey         (): CARDINAL;
      paintEngine      (): QPaintEngine; (* virtual *)
      dotsPerMeterX    (): INTEGER;
      dotsPerMeterY    (): INTEGER;
      setDotsPerMeterX (arg1: INTEGER; );
      setDotsPerMeterY (arg1: INTEGER; );
      offset           (): QPoint;
      setOffset        (arg1: QPoint; );
      data_ptr         (): UNTRACED REF CHAR;
      destroyCxx       ();
    END;


END QtImage.
