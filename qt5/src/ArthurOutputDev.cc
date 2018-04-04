//========================================================================
//
// ArthurOutputDev.cc
//
// Copyright 2003 Glyph & Cog, LLC
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2005 Brad Hards <bradh@frogmouth.net>
// Copyright (C) 2005-2009, 2011, 2012, 2014, 2015, 2018 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2008, 2010 Pino Toscano <pino@kde.org>
// Copyright (C) 2009, 2011 Carlos Garcia Campos <carlosgc@gnome.org>
// Copyright (C) 2009 Petr Gajdos <pgajdos@novell.com>
// Copyright (C) 2010 Matthias Fauconneau <matthias.fauconneau@gmail.com>
// Copyright (C) 2011 Andreas Hartmetz <ahartmetz@gmail.com>
// Copyright (C) 2013 Thomas Freitag <Thomas.Freitag@alfa.de>
// Copyright (C) 2013 Dominik Haumann <dhaumann@kde.org>
// Copyright (C) 2013 Mihai Niculescu <q.quark@gmail.com>
// Copyright (C) 2017, 2018 Oliver Sander <oliver.sander@tu-dresden.de>
// Copyright (C) 2017 Adrian Johnson <ajohnson@redneon.com>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#include <config.h>

#ifdef USE_GCC_PRAGMAS
#pragma implementation
#endif

#include <string.h>
#include <math.h>

#include "goo/gfile.h"
#include "GlobalParams.h"
#include "Error.h"
#include "Object.h"
#include "GfxState.h"
#include "GfxFont.h"
#include "Link.h"
#include "FontEncodingTables.h"
#include <fofi/FoFiTrueType.h>
#include "ArthurOutputDev.h"

#include <QtCore/QtDebug>
#include <QRawFont>
#include <QGlyphRun>
#include <QtGui/QPainterPath>
#include <QPicture>

//------------------------------------------------------------------------

#ifdef HAVE_SPLASH
#include "splash/SplashFontFileID.h"
#include "splash/SplashFTFontFile.h"
#include "splash/SplashFontEngine.h"
//------------------------------------------------------------------------
// SplashOutFontFileID
//------------------------------------------------------------------------

class SplashOutFontFileID: public SplashFontFileID {
public:

  SplashOutFontFileID(Ref *rA) { r = *rA; }

  ~SplashOutFontFileID() {}

  GBool matches(SplashFontFileID *id) override {
    return ((SplashOutFontFileID *)id)->r.num == r.num &&
           ((SplashOutFontFileID *)id)->r.gen == r.gen;
  }

private:

  Ref r;
};

#endif

//------------------------------------------------------------------------
// ArthurOutputDev
//------------------------------------------------------------------------

ArthurOutputDev::ArthurOutputDev(QPainter *painter):
  m_lastTransparencyGroupPicture(nullptr),
  m_fontHinting(NoHinting)
{
  m_painter.push(painter);
  m_currentBrush = QBrush(Qt::SolidPattern);
  m_fontEngine = nullptr;
}

ArthurOutputDev::~ArthurOutputDev()
{
#ifdef HAVE_SPLASH
  delete m_fontEngine;
#endif
}

void ArthurOutputDev::startDoc(XRef *xrefA) {
  xref = xrefA;
#ifdef HAVE_SPLASH
  delete m_fontEngine;

  const bool isHintingEnabled = m_fontHinting != NoHinting;
  const bool isSlightHinting = m_fontHinting == SlightHinting;

  m_fontEngine = new SplashFontEngine(
  globalParams->getEnableFreeType(),
  isHintingEnabled,
  isSlightHinting,
  m_painter.top()->testRenderHint(QPainter::TextAntialiasing));
#endif
}

void ArthurOutputDev::startPage(int pageNum, GfxState *state, XRef *)
{
  // fill page with white background.
  int w = static_cast<int>(state->getPageWidth());
  int h = static_cast<int>(state->getPageHeight());
  QColor fillColour(Qt::white);
  QBrush fill(fillColour);
  m_painter.top()->save();
  m_painter.top()->setPen(fillColour);
  m_painter.top()->setBrush(fill);
  m_painter.top()->drawRect(0, 0, w, h);
  m_painter.top()->restore();
}

void ArthurOutputDev::endPage() {
}

void ArthurOutputDev::saveState(GfxState *state)
{
  m_currentPenStack.push(m_currentPen);
  m_currentBrushStack.push(m_currentBrush);
  m_rawFontStack.push(m_rawFont);
  m_codeToGIDStack.push(m_codeToGID);

  m_painter.top()->save();
}

void ArthurOutputDev::restoreState(GfxState *state)
{
  m_painter.top()->restore();

  m_codeToGID = m_codeToGIDStack.top();
  m_codeToGIDStack.pop();
  m_rawFont = m_rawFontStack.top();
  m_rawFontStack.pop();
  m_currentBrush = m_currentBrushStack.top();
  m_currentBrushStack.pop();
  m_currentPen = m_currentPenStack.top();
  m_currentPenStack.pop();
}

void ArthurOutputDev::updateAll(GfxState *state)
{
  OutputDev::updateAll(state);
  m_needFontUpdate = gTrue;
}

// Set CTM (Current Transformation Matrix) to a fixed matrix
void ArthurOutputDev::setDefaultCTM(double *ctm)
{
  m_painter.top()->setTransform(QTransform(ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]));
}

// Update the CTM (Current Transformation Matrix), i.e., compose the old
// CTM with a new matrix.
void ArthurOutputDev::updateCTM(GfxState *state, double m11, double m12,
				double m21, double m22,
				double m31, double m32)
{
  updateLineDash(state);
  updateLineJoin(state);
  updateLineCap(state);
  updateLineWidth(state);

  QTransform update(m11, m12, m21, m22, m31, m32);

  // We could also set (rather than update) the painter transformation to state->getCMT();
  m_painter.top()->setTransform(update, true);
}

void ArthurOutputDev::updateLineDash(GfxState *state)
{
  double *dashPattern;
  int dashLength;
  double dashStart;
  state->getLineDash(&dashPattern, &dashLength, &dashStart);

  // Special handling for zero-length patterns, i.e., solid lines.
  // Simply calling QPen::setDashPattern with an empty pattern does *not*
  // result in a solid line.  Rather, the current pattern is unchanged.
  // See the implementation of the setDashPattern method in the file qpen.cpp.
  if (dashLength==0)
  {
    m_currentPen.setStyle(Qt::SolidLine);
    m_painter.top()->setPen(m_currentPen);
    return;
  }

  QVector<qreal> pattern(dashLength);
  for (int i = 0; i < dashLength; ++i) {
    // pdf measures the dash pattern in dots, but Qt uses the
    // line width as the unit.
    pattern[i] = dashPattern[i] / state->getLineWidth();
  }
  m_currentPen.setDashPattern(pattern);
  m_currentPen.setDashOffset(dashStart);
  m_painter.top()->setPen(m_currentPen);
}

void ArthurOutputDev::updateFlatness(GfxState *state)
{
  // qDebug() << "updateFlatness";
}

void ArthurOutputDev::updateLineJoin(GfxState *state)
{
  switch (state->getLineJoin()) {
  case 0:
    // The correct style here is Qt::SvgMiterJoin, *not* Qt::MiterJoin.
    // The two differ in what to do if the miter limit is exceeded.
    // See https://bugs.freedesktop.org/show_bug.cgi?id=102356
    m_currentPen.setJoinStyle(Qt::SvgMiterJoin);
    break;
  case 1:
    m_currentPen.setJoinStyle(Qt::RoundJoin);
    break;
  case 2:
    m_currentPen.setJoinStyle(Qt::BevelJoin);
    break;
  }
  m_painter.top()->setPen(m_currentPen);
}

void ArthurOutputDev::updateLineCap(GfxState *state)
{
  switch (state->getLineCap()) {
  case 0:
    m_currentPen.setCapStyle(Qt::FlatCap);
    break;
  case 1:
    m_currentPen.setCapStyle(Qt::RoundCap);
    break;
  case 2:
    m_currentPen.setCapStyle(Qt::SquareCap);
    break;
  }
  m_painter.top()->setPen(m_currentPen);
}

void ArthurOutputDev::updateMiterLimit(GfxState *state)
{
  m_currentPen.setMiterLimit(state->getMiterLimit());
  m_painter.top()->setPen(m_currentPen);
}

void ArthurOutputDev::updateLineWidth(GfxState *state)
{
  m_currentPen.setWidthF(state->getLineWidth());
  m_painter.top()->setPen(m_currentPen);
  // The updateLineDash method needs to know the line width, but it is sometimes
  // called before the updateLineWidth method.  To make sure that the last call
  // to updateLineDash before a drawing operation is always with the correct line
  // width, we call it here, right after a change to the line width.
  updateLineDash(state);
}

void ArthurOutputDev::updateFillColor(GfxState *state)
{
  GfxRGB rgb;
  QColor brushColour = m_currentBrush.color();
  state->getFillRGB(&rgb);
  brushColour.setRgbF(colToDbl(rgb.r), colToDbl(rgb.g), colToDbl(rgb.b), brushColour.alphaF());
  m_currentBrush.setColor(brushColour);
}

void ArthurOutputDev::updateStrokeColor(GfxState *state)
{
  GfxRGB rgb;
  QColor penColour = m_currentPen.color();
  state->getStrokeRGB(&rgb);
  penColour.setRgbF(colToDbl(rgb.r), colToDbl(rgb.g), colToDbl(rgb.b), penColour.alphaF());
  m_currentPen.setColor(penColour);
  m_painter.top()->setPen(m_currentPen);
}

void ArthurOutputDev::updateBlendMode(GfxState * state)
{
  GfxBlendMode blendMode = state->getBlendMode();

  // missing composition modes in QPainter:
  // - CompositionMode_Hue
  // - CompositionMode_Color
  // - CompositionMode_Luminosity
  // - CompositionMode_Saturation

  switch(blendMode){
  case gfxBlendMultiply:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_Multiply);
    break;
  case gfxBlendScreen:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_Screen);
    break;
  case gfxBlendDarken:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_Darken);
    break;
  case gfxBlendLighten:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_Lighten);
    break;
  case gfxBlendColorDodge:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_ColorDodge);
    break;
  case gfxBlendColorBurn:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_ColorBurn);
    break;
  case gfxBlendHardLight:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_HardLight);
    break;
  case gfxBlendSoftLight:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_SoftLight);
    break;
  case gfxBlendDifference:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_Difference);
    break;
  case gfxBlendExclusion:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_Exclusion);
    break;
  case gfxBlendColor:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_Plus);
    break;
  default:
    qDebug() << "Unsupported blend mode, falling back to CompositionMode_SourceOver";
  case gfxBlendNormal:
    m_painter.top()->setCompositionMode(QPainter::CompositionMode_SourceOver);
    break;
  }
}

void ArthurOutputDev::updateFillOpacity(GfxState *state)
{
  QColor brushColour= m_currentBrush.color();
  brushColour.setAlphaF(state->getFillOpacity());
  m_currentBrush.setColor(brushColour);
}

void ArthurOutputDev::updateStrokeOpacity(GfxState *state)
{
  QColor penColour= m_currentPen.color();
  penColour.setAlphaF(state->getStrokeOpacity());
  m_currentPen.setColor(penColour);
  m_painter.top()->setPen(m_currentPen);
}

void ArthurOutputDev::updateFont(GfxState *state)
{
  GfxFont *gfxFont = state->getFont();
  if (!gfxFont)
  {
    return;
  }

  // is the font in the cache?
  ArthurFontID fontID = {*gfxFont->getID(), state->getFontSize()};
  auto cacheEntry = m_rawFontCache.find(fontID);

  if (cacheEntry!=m_rawFontCache.end()) {

    // Take the font from the cache
    m_rawFont = cacheEntry->second.get();

  } else {

    // New font: load it into the cache
    float fontSize = state->getFontSize();

    std::unique_ptr<GfxFontLoc> fontLoc(gfxFont->locateFont(xref, nullptr));

    if (fontLoc) {
      // load the font from respective location
      switch (fontLoc->locType) {
      case gfxFontLocEmbedded: {// if there is an embedded font, read it to memory
        int fontDataLen;
        const char* fontData = gfxFont->readEmbFontFile(xref, &fontDataLen);

        m_rawFont = new QRawFont(QByteArray(fontData, fontDataLen), fontSize);
        m_rawFontCache.insert(std::make_pair(fontID,std::unique_ptr<QRawFont>(m_rawFont)));

        // Free the font data, it was copied in the QByteArray constructor
        free((char*)fontData);
        break;
      }
      case gfxFontLocExternal:{ // font is in an external font file
        QString fontFile(fontLoc->path->getCString());
        m_rawFont = new QRawFont(fontFile, fontSize);
        m_rawFontCache.insert(std::make_pair(fontID,std::unique_ptr<QRawFont>(m_rawFont)));
        break;
      }
      case gfxFontLocResident:{ // font resides in a PS printer
        qDebug() << "Font Resident Encoding:" << fontLoc->encoding->getCString() << ", not implemented yet!";

      break;
      }
      }// end switch

    } else {
      qDebug() << "Font location not found!";
      return;
    }
  }

  if (!m_rawFont->isValid()) {
    qDebug() << "RawFont is not valid";
  }

  // *****************************************************************************
  //  We have now successfully loaded the font into a QRawFont object.  This
  //  allows us to draw all the glyphs in the font.  However, what is missing is
  //  the charcode-to-glyph-index mapping.  Apparently, Qt does not provide this
  //  information at all.  We therefore now load the font again, this time into
  //  a Splash font object.  This is wasteful, but I see no other way to access
  //  the important glyph-index mapping.
  // *****************************************************************************

#ifdef HAVE_SPLASH
  GfxFontType fontType;
  SplashFontFile *fontFile;
  SplashFontSrc *fontsrc = nullptr;
  FoFiTrueType *ff;
  GooString *fileName;
  char *tmpBuf;
  int tmpBufLen = 0;
  int *codeToGID;
  SplashCoord mat[4] = {0,0,0,0};
  int n;
  int faceIndex = 0;
  SplashCoord matrix[6] = {1,0,0,1,0,0};
  SplashFTFontFile* ftFontFile;

  m_needFontUpdate = false;
  fileName = nullptr;
  tmpBuf = nullptr;

  fontType = gfxFont->getType();
  if (fontType == fontType3) {
    return;
  }

  // Default: no codeToGID table
  m_codeToGID = nullptr;

  // check the font file cache
  SplashOutFontFileID *id = new SplashOutFontFileID(gfxFont->getID());
  if ((fontFile = m_fontEngine->getFontFile(id))) {
    delete id;

  } else {

    std::unique_ptr<GfxFontLoc> fontLoc(gfxFont->locateFont(xref, nullptr));
    if (!fontLoc) {
      error(errSyntaxError, -1, "Couldn't find a font for '{0:s}'",
	    gfxFont->getName() ? gfxFont->getName()->getCString()
	                       : "(unnamed)");
      goto err2;
    }

    // embedded font
    if (fontLoc->locType == gfxFontLocEmbedded) {
      // if there is an embedded font, read it to memory
      tmpBuf = gfxFont->readEmbFontFile(xref, &tmpBufLen);
      if (! tmpBuf)
	goto err2;

    // external font
    } else { // gfxFontLocExternal
      fileName = fontLoc->path;
      fontType = fontLoc->fontType;
    }

    fontsrc = new SplashFontSrc;
    if (fileName)
      fontsrc->setFile(fileName, gFalse);
    else
      fontsrc->setBuf(tmpBuf, tmpBufLen, gTrue);
    
    // load the font file
    switch (fontType) {
    case fontType1:
      if (!(fontFile = m_fontEngine->loadType1Font(
			   id,
			   fontsrc,
			   (const char **)((Gfx8BitFont *)gfxFont)->getEncoding()))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
	      gfxFont->getName() ? gfxFont->getName()->getCString()
	                         : "(unnamed)");
	goto err2;
      }
      break;
    case fontType1C:
      if (!(fontFile = m_fontEngine->loadType1CFont(
			   id,
			   fontsrc,
			   (const char **)((Gfx8BitFont *)gfxFont)->getEncoding()))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
	      gfxFont->getName() ? gfxFont->getName()->getCString()
	                         : "(unnamed)");
	goto err2;
      }
      break;
    case fontType1COT:
      if (!(fontFile = m_fontEngine->loadOpenTypeT1CFont(
			   id,
			   fontsrc,
			   (const char **)((Gfx8BitFont *)gfxFont)->getEncoding()))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
	      gfxFont->getName() ? gfxFont->getName()->getCString()
	                         : "(unnamed)");
	goto err2;
      }
      break;
    case fontTrueType:
    case fontTrueTypeOT:
	if (fileName)
	 ff = FoFiTrueType::load(fileName->getCString());
	else
	ff = FoFiTrueType::make(tmpBuf, tmpBufLen);
      if (ff) {
	codeToGID = ((Gfx8BitFont *)gfxFont)->getCodeToGIDMap(ff);
	n = 256;
	delete ff;
      } else {
	codeToGID = nullptr;
	n = 0;
      }
      if (!(fontFile = m_fontEngine->loadTrueTypeFont(
			   id,
			   fontsrc,
			   codeToGID, n))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
	      gfxFont->getName() ? gfxFont->getName()->getCString()
	                         : "(unnamed)");
	goto err2;
      }
      break;
    case fontCIDType0:
    case fontCIDType0C:
      if (!(fontFile = m_fontEngine->loadCIDFont(
			   id,
			   fontsrc))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
	      gfxFont->getName() ? gfxFont->getName()->getCString()
	                         : "(unnamed)");
	goto err2;
      }
      break;
    case fontCIDType0COT:
      if (((GfxCIDFont *)gfxFont)->getCIDToGID()) {
	n = ((GfxCIDFont *)gfxFont)->getCIDToGIDLen();
	codeToGID = (int *)gmallocn(n, sizeof(int));
	memcpy(codeToGID, ((GfxCIDFont *)gfxFont)->getCIDToGID(),
	       n * sizeof(int));
      } else {
	codeToGID = nullptr;
	n = 0;
      }      
      if (!(fontFile = m_fontEngine->loadOpenTypeCFFFont(
			   id,
			   fontsrc,
			   codeToGID, n))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
	      gfxFont->getName() ? gfxFont->getName()->getCString()
	                         : "(unnamed)");
	goto err2;
      }
      break;
    case fontCIDType2:
    case fontCIDType2OT:
      codeToGID = nullptr;
      n = 0;
      if (((GfxCIDFont *)gfxFont)->getCIDToGID()) {
	n = ((GfxCIDFont *)gfxFont)->getCIDToGIDLen();
	if (n) {
	  codeToGID = (int *)gmallocn(n, sizeof(int));
	  memcpy(codeToGID, ((GfxCIDFont *)gfxFont)->getCIDToGID(),
		  n * sizeof(Gushort));
	}
      } else {
	if (fileName)
	  ff = FoFiTrueType::load(fileName->getCString());
	else
	  ff = FoFiTrueType::make(tmpBuf, tmpBufLen);
	if (! ff)
	  goto err2;
	codeToGID = ((GfxCIDFont *)gfxFont)->getCodeToGIDMap(ff, &n);
	delete ff;
      }
      if (!(fontFile = m_fontEngine->loadTrueTypeFont(
			   id,
			   fontsrc,
			   codeToGID, n, faceIndex))) {
	error(errSyntaxError, -1, "Couldn't create a font for '{0:s}'",
	      gfxFont->getName() ? gfxFont->getName()->getCString()
	                         : "(unnamed)");
	goto err2;
      }
      break;
    default:
      // this shouldn't happen
      goto err2;
    }
  }

  ftFontFile = dynamic_cast<SplashFTFontFile*>(fontFile);
  if (ftFontFile)
    m_codeToGID = ftFontFile->getCodeToGID();

  // create dummy font
  // The font matrices are bogus, but we will never use the glyphs anyway.
  // However we need to call m_fontEngine->getFont, in order to have the
  // font in the Splash font cache.  Otherwise we'd load it again and again.
  m_fontEngine->getFont(fontFile, mat, matrix);

  if (fontsrc && !fontsrc->isFile)
      fontsrc->unref();
  return;

 err2:
  delete id;
#endif
}

static QPainterPath convertPath(GfxState *state, GfxPath *path, Qt::FillRule fillRule)
{
  GfxSubpath *subpath;
  int i, j;

  QPainterPath qPath;
  qPath.setFillRule(fillRule);
  for (i = 0; i < path->getNumSubpaths(); ++i) {
    subpath = path->getSubpath(i);
    if (subpath->getNumPoints() > 0) {
      qPath.moveTo(subpath->getX(0), subpath->getY(0));
      j = 1;
      while (j < subpath->getNumPoints()) {
	if (subpath->getCurve(j)) {
          qPath.cubicTo( subpath->getX(j),   subpath->getY(j),
                         subpath->getX(j+1), subpath->getY(j+1),
                         subpath->getX(j+2), subpath->getY(j+2));
	  j += 3;
	} else {
	  qPath.lineTo(subpath->getX(j), subpath->getY(j));
	  ++j;
	}
      }
      if (subpath->isClosed()) {
	qPath.closeSubpath();
      }
    }
  }
  return qPath;
}

void ArthurOutputDev::stroke(GfxState *state)
{
  m_painter.top()->strokePath( convertPath( state, state->getPath(), Qt::OddEvenFill ), m_currentPen );
}

void ArthurOutputDev::fill(GfxState *state)
{
  m_painter.top()->fillPath( convertPath( state, state->getPath(), Qt::WindingFill ), m_currentBrush );
}

void ArthurOutputDev::eoFill(GfxState *state)
{
  m_painter.top()->fillPath( convertPath( state, state->getPath(), Qt::OddEvenFill ), m_currentBrush );
}

GBool ArthurOutputDev::axialShadedFill(GfxState *state, GfxAxialShading *shading, double tMin, double tMax)
{
  double x0, y0, x1, y1;
  shading->getCoords(&x0, &y0, &x1, &y1);

  // get the clip region bbox
  double xMin, yMin, xMax, yMax;
  state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);

  // get the function domain
  double t0 = shading->getDomain0();
  double t1 = shading->getDomain1();

  // Max number of splits along the t axis
  constexpr int maxSplits = 256;

  // Max delta allowed in any color component
  const double colorDelta = (dblToCol(1 / 256.0));

  // Number of color space components
  auto nComps = shading->getColorSpace()->getNComps();

  // Helper function to test two color objects for 'almost-equality'
  auto isSameGfxColor = [&nComps,&colorDelta](const GfxColor &colorA, const GfxColor &colorB)
                        {
                          for (int k = 0; k < nComps; ++k) {
                            if (abs(colorA.c[k] - colorB.c[k]) > colorDelta) {
                              return false;
                            }
                          }
                          return true;
                        };

  // Helper function: project a number into an interval
  // With C++17 this is part of the standard library
  auto clamp = [](double v, double lo, double hi)
                  { return std::min(std::max(v,lo), hi); };

  // ta stores all parameter values where we evaluate the input shading function.
  // In between, QLinearGradient will interpolate linearly.
  // We set up the array with three values.
  std::array<double, maxSplits+1> ta;
  ta[0] = tMin;
  std::array<int, maxSplits+1> next;
  next[0] = maxSplits / 2;
  ta[maxSplits / 2] = 0.5 * (tMin + tMax);
  next[maxSplits / 2] = maxSplits;
  ta[maxSplits] = tMax;

  // compute the color at t = tMin
  double tt = clamp(t0 + (t1 - t0) * tMin, t0, t1);

  GfxColor color0, color1;
  shading->getColor(tt, &color0);

  // Construct a gradient object and set its color at one parameter end
  QLinearGradient gradient(QPointF(x0 + tMin * (x1 - x0), y0 + tMin * (y1 - y0)),
                           QPointF(x0 + tMax * (x1 - x0), y0 + tMax * (y1 - y0)));

  GfxRGB rgb;
  shading->getColorSpace()->getRGB(&color0, &rgb);
  QColor qColor(colToByte(rgb.r), colToByte(rgb.g), colToByte(rgb.b));
  gradient.setColorAt(0,qColor);

  // Look for more relevant parameter values by bisection
  int i = 0;
  while (i < maxSplits) {

    int j = next[i];
    while (j > i + 1) {

      // Next parameter value to try
      tt = clamp(t0 + (t1 - t0) * ta[j], t0, t1);
      shading->getColor(tt, &color1);

      // j is a good next color stop if the input shading can be approximated well
      // on the interval (ta[i], ta[j]) by a linear interpolation.
      // We test this by comparing the real color in the middle between ta[i] and ta[j]
      // with the linear interpolant there.
      auto midPoint = 0.5 * (ta[i] + ta[j]);
      GfxColor colorAtMidPoint;
      shading->getColor(midPoint, &colorAtMidPoint);

      GfxColor linearlyInterpolatedColor;
      for (int ii=0; ii<nComps; ii++)
        linearlyInterpolatedColor.c[ii] = 0.5 * (color0.c[ii] + color1.c[ii]);

      // If the two colors are equal, ta[j] is a good place for the next color stop; take it!
      if (isSameGfxColor(colorAtMidPoint, linearlyInterpolatedColor))
        break;

      // Otherwise: bisect further
      int k = (i + j) / 2;
      ta[k] = midPoint;
      next[i] = k;
      next[k] = j;
      j = k;
    }

    // set the color
    GfxRGB rgb;
    shading->getColorSpace()->getRGB(&color1, &rgb);
    qColor.setRgb(colToByte(rgb.r), colToByte(rgb.g), colToByte(rgb.b));
    gradient.setColorAt((ta[j] - tMin)/(tMax - tMin), qColor);

    // Move to the next parameter region
    color0 = color1;
    i = next[i];
  }

  state->moveTo(xMin, yMin);
  state->lineTo(xMin, yMax);
  state->lineTo(xMax, yMax);
  state->lineTo(xMax, yMin);
  state->closePath();

  // Actually paint the shaded region
  QBrush newBrush(gradient);
  m_painter.top()->fillPath( convertPath( state, state->getPath(), Qt::WindingFill ), newBrush );

  state->clearPath();

  // True means: The shaded region has been painted
  return gTrue;
}

void ArthurOutputDev::clip(GfxState *state)
{
  m_painter.top()->setClipPath(convertPath( state, state->getPath(), Qt::WindingFill ), Qt::IntersectClip );
}

void ArthurOutputDev::eoClip(GfxState *state)
{
  m_painter.top()->setClipPath(convertPath( state, state->getPath(), Qt::OddEvenFill ), Qt::IntersectClip );
}

void ArthurOutputDev::drawChar(GfxState *state, double x, double y,
			       double dx, double dy,
			       double originX, double originY,
			       CharCode code, int nBytes, Unicode *u, int uLen) {

  // check for invisible text -- this is used by Acrobat Capture
  int render = state->getRender();
  if (render == 3 || !m_rawFont) {
    qDebug() << "Invisible text found!";
    return;
  }

  if (!(render & 1))
  {
    quint32 glyphIndex = (m_codeToGID) ? m_codeToGID[code] : code;
    QPointF glyphPosition = QPointF(x-originX, y-originY);

    // QGlyphRun objects can hold an entire sequence of glyphs, and it would possibly
    // be more efficient to simply note the glyph and glyph position here and then
    // draw several glyphs at once in the endString method.  What keeps us from doing
    // that is the transformation below: each glyph needs to be drawn upside down,
    // i.e., reflected at its own baseline.  Since we have no guarantee that this
    // baseline is the same for all glyphs in a string we have to do it one by one.
    QGlyphRun glyphRun;
    glyphRun.setRawData(&glyphIndex, &glyphPosition, 1);
    glyphRun.setRawFont(*m_rawFont);

    // Store the QPainter state; we need to modify it temporarily
    m_painter.top()->save();

    // Apply the text matrix to the glyph.  The glyph is not scaled by the font size,
    // because the font in m_rawFont already has the correct size.
    // Additionally, the CTM is upside down, i.e., it contains a negative Y-scaling
    // entry.  Therefore, Qt will paint the glyphs upside down.  We need to temporarily
    // reflect the page at glyphPosition.y().

    // Make the glyph position the coordinate origin -- that's our center of scaling
    const double *textMat = state->getTextMat();

    m_painter.top()->translate(QPointF(glyphPosition.x(),glyphPosition.y()));

    QTransform textTransform(textMat[0] * state->getHorizScaling(),
                             textMat[1] * state->getHorizScaling(),
                             -textMat[2], // reflect at the horizontal axis,
                             -textMat[3], // because CTM is upside-down.
                             0,
                             0);

    m_painter.top()->setTransform(textTransform,true);

    // We are painting a filled glyph here.  But QPainter uses the pen to draw even filled text,
    // not the brush.  (see, e.g.,  http://doc.qt.io/qt-5/qpainter.html#setPen )
    // Therefore we have to temporarily overwrite the pen color.

    // Since we are drawing a filled glyph, one would really expect to have m_currentBrush
    // have the correct color.  However, somehow state->getFillRGB can change without
    // updateFillColor getting called.  Then m_currentBrush may not contain the correct color.
    GfxRGB rgb;
    state->getFillRGB(&rgb);
    QColor fontColor;
    fontColor.setRgbF(colToDbl(rgb.r), colToDbl(rgb.g), colToDbl(rgb.b), state->getFillOpacity());
    m_painter.top()->setPen(fontColor);

    // Actually draw the glyph
    m_painter.top()->drawGlyphRun(QPointF(-glyphPosition.x(),-glyphPosition.y()), glyphRun);

    // Restore transformation and pen color
    m_painter.top()->restore();
  }
}

GBool ArthurOutputDev::beginType3Char(GfxState *state, double x, double y,
				      double dx, double dy,
				      CharCode code, Unicode *u, int uLen)
{
  return gFalse;
}

void ArthurOutputDev::endType3Char(GfxState *state)
{
}

void ArthurOutputDev::type3D0(GfxState *state, double wx, double wy)
{
}

void ArthurOutputDev::type3D1(GfxState *state, double wx, double wy,
			      double llx, double lly, double urx, double ury)
{
}

void ArthurOutputDev::endTextObject(GfxState *state)
{
}


void ArthurOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
				    int width, int height, GBool invert,
				    GBool interpolate, GBool inlineImg)
{
  qDebug() << "drawImageMask";
#if 0
  unsigned char *buffer;
  unsigned char *dest;
  cairo_surface_t *image;
  cairo_pattern_t *pattern;
  int x, y;
  ImageStream *imgStr;
  Guchar *pix;
  double *ctm;
  cairo_matrix_t matrix;
  int invert_bit;
  int row_stride;

  row_stride = (width + 3) & ~3;
  buffer = (unsigned char *) malloc (height * row_stride);
  if (buffer == nullptr) {
    error(-1, "Unable to allocate memory for image.");
    return;
  }

  /* TODO: Do we want to cache these? */
  imgStr = new ImageStream(str, width, 1, 1);
  imgStr->reset();

  invert_bit = invert ? 1 : 0;

  for (y = 0; y < height; y++) {
    pix = imgStr->getLine();
    dest = buffer + y * row_stride;
    for (x = 0; x < width; x++) {

      if (pix[x] ^ invert_bit)
	*dest++ = 0;
      else
	*dest++ = 255;
    }
  }

  image = cairo_image_surface_create_for_data (buffer, CAIRO_FORMAT_A8,
					  width, height, row_stride);
  if (image == nullptr)
    return;
  pattern = cairo_pattern_create_for_surface (image);
  if (pattern == nullptr)
    return;

  ctm = state->getCTM();
  LOG (printf ("drawImageMask %dx%d, matrix: %f, %f, %f, %f, %f, %f\n",
	       width, height, ctm[0], ctm[1], ctm[2], ctm[3], ctm[4], ctm[5]));
  matrix.xx = ctm[0] / width;
  matrix.xy = -ctm[2] / height;
  matrix.yx = ctm[1] / width;
  matrix.yy = -ctm[3] / height;
  matrix.x0 = ctm[2] + ctm[4];
  matrix.y0 = ctm[3] + ctm[5];
  cairo_matrix_invert (&matrix);
  cairo_pattern_set_matrix (pattern, &matrix);

  cairo_pattern_set_filter (pattern, CAIRO_FILTER_BEST);
  /* FIXME: Doesn't the image mask support any colorspace? */
  cairo_set_source_rgb (cairo, fill_color.r, fill_color.g, fill_color.b);
  cairo_mask (cairo, pattern);

  cairo_pattern_destroy (pattern);
  cairo_surface_destroy (image);
  free (buffer);
  imgStr->close ();
  delete imgStr;
#endif
}

//TODO: lots more work here.
void ArthurOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
				int width, int height,
				GfxImageColorMap *colorMap,
				GBool interpolate, int *maskColors, GBool inlineImg)
{
  unsigned int *data;
  unsigned int *line;
  int x, y;
  ImageStream *imgStr;
  Guchar *pix;
  int i;
  QImage image;
  int stride;
  
  /* TODO: Do we want to cache these? */
  imgStr = new ImageStream(str, width,
			   colorMap->getNumPixelComps(),
			   colorMap->getBits());
  imgStr->reset();
  
  image = QImage(width, height, QImage::Format_ARGB32);
  data = (unsigned int *)image.bits();
  stride = image.bytesPerLine()/4;
  for (y = 0; y < height; y++) {
    pix = imgStr->getLine();
    // Invert the vertical coordinate: y is increasing from top to bottom
    // on the page, but y is increasing bottom to top in the picture.
    line = data+(height-1-y)*stride;
    colorMap->getRGBLine(pix, line, width);

    if (maskColors) {
      for (x = 0; x < width; x++) {
        for (i = 0; i < colorMap->getNumPixelComps(); ++i) {
            if (pix[i] < maskColors[2*i] * 255||
                pix[i] > maskColors[2*i+1] * 255) {
                *line = *line | 0xff000000;
                break;
            }
        }
        pix += colorMap->getNumPixelComps();
        line++;
      }
    } else {
      for (x = 0; x < width; x++) { *line = *line | 0xff000000; line++; }
    }
  }

  // At this point, the QPainter coordinate transformation (CTM) is such
  // that QRect(0,0,1,1) is exactly the area of the image.
  m_painter.top()->drawImage( QRect(0,0,1,1), image );
  delete imgStr;

}

void ArthurOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
                                          int width, int height,
                                          GfxImageColorMap *colorMap,
                                          GBool interpolate,
                                          Stream *maskStr,
                                          int maskWidth, int maskHeight,
                                          GfxImageColorMap *maskColorMap,
                                          GBool maskInterpolate)
{
  // Bail out if the image size doesn't match the mask size.  I don't know
  // what to do in this case.
  if (width!=maskWidth || height!=maskHeight)
  {
    qDebug() << "Soft mask size does not match image size!";
    drawImage(state, ref, str, width, height, colorMap, interpolate, nullptr, gFalse);
    return;
  }

  // Bail out if the mask isn't a single channel.  I don't know
  // what to do in this case.
  if (maskColorMap->getColorSpace()->getNComps() != 1)
  {
    qDebug() << "Soft mask is not a single 8-bit channel!";
    drawImage(state, ref, str, width, height, colorMap, interpolate, nullptr, gFalse);
    return;
  }

  /* TODO: Do we want to cache these? */
  std::unique_ptr<ImageStream> imgStr(new ImageStream(str, width,
                                                      colorMap->getNumPixelComps(),
                                                      colorMap->getBits()));
  imgStr->reset();

  std::unique_ptr<ImageStream> maskImageStr(new ImageStream(maskStr, maskWidth,
                                                            maskColorMap->getNumPixelComps(),
                                                            maskColorMap->getBits()));
  maskImageStr->reset();

  QImage image(width, height, QImage::Format_ARGB32);
  unsigned int *data = (unsigned int *)image.bits();
  int stride = image.bytesPerLine()/4;

  std::vector<Guchar> maskLine(maskWidth);

  for (int y = 0; y < height; y++) {

    Guchar *pix = imgStr->getLine();
    Guchar *maskPix = maskImageStr->getLine();

    // Invert the vertical coordinate: y is increasing from top to bottom
    // on the page, but y is increasing bottom to top in the picture.
    unsigned int* line = data+(height-1-y)*stride;
    colorMap->getRGBLine(pix, line, width);

    // Apply the mask values to the image alpha channel
    maskColorMap->getGrayLine(maskPix, maskLine.data(), width);
    for (int x = 0; x < width; x++)
    {
      *line = *line | (maskLine[x]<<24);
      line++;
    }
  }

  // At this point, the QPainter coordinate transformation (CTM) is such
  // that QRect(0,0,1,1) is exactly the area of the image.
  m_painter.top()->drawImage( QRect(0,0,1,1), image );
}

void ArthurOutputDev::beginTransparencyGroup(GfxState * /*state*/, double * /*bbox*/,
                                             GfxColorSpace * /*blendingColorSpace*/,
                                             GBool /*isolated*/, GBool /*knockout*/,
                                             GBool /*forSoftMask*/)
{
  // The entire transparency group will be painted into a
  // freshly created QPicture object.  Since an existing painter
  // cannot change its paint device, we need to construct a
  // new QPainter object as well.
  m_qpictures.push(new QPicture);
  m_painter.push(new QPainter(m_qpictures.top()));
}

void ArthurOutputDev::endTransparencyGroup(GfxState * /*state*/)
{
  // Stop painting into the group
  m_painter.top()->end();

  // Kill the painter that has been used for the transparency group
  delete(m_painter.top());
  m_painter.pop();

  // Store the QPicture object that holds the result of the transparency group
  // painting.  It will be painted and deleted in the method paintTransparencyGroup.
  if (m_lastTransparencyGroupPicture)
  {
    qDebug() << "Found a transparency group that has not been painted";
    delete(m_lastTransparencyGroupPicture);
  }
  m_lastTransparencyGroupPicture = m_qpictures.top();
  m_qpictures.pop();
}

void ArthurOutputDev::paintTransparencyGroup(GfxState * /*state*/, double * /*bbox*/)
{
  // Actually draw the transparency group
  m_painter.top()->drawPicture(0,0,*m_lastTransparencyGroupPicture);

  // And delete it
  delete(m_lastTransparencyGroupPicture);
  m_lastTransparencyGroupPicture = nullptr;
}

