/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkSGText.h"

#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkPath.h"
#include "SkTArray.h"
#include "SkTypeface.h"

namespace sksg {

sk_sp<Text> Text::Make(sk_sp<SkTypeface> tf, const SkString& text) {
    return sk_sp<Text>(new Text(std::move(tf), text));
}

Text::Text(sk_sp<SkTypeface> tf, const SkString& text)
    : fTypeface(std::move(tf))
    , fText(text) {}

Text::~Text() = default;

SkPoint Text::alignedPosition(SkScalar advance) const {
    auto aligned = fPosition;

    switch (fAlign) {
    case SkTextUtils::kLeft_Align:
        break;
    case SkTextUtils::kCenter_Align:
        aligned.offset(-advance / 2, 0);
        break;
    case SkTextUtils::kRight_Align:
        aligned.offset(-advance, 0);
        break;
    }

    return aligned;
}

SkRect Text::onRevalidate(InvalidationController*, const SkMatrix&) {
    // TODO: we could potentially track invals which don't require rebuilding the blob.

    SkPaint font;
    font.setFlags(fFlags);
    font.setTypeface(fTypeface);
    font.setTextSize(fSize);
    font.setTextScaleX(fScaleX);
    font.setTextSkewX(fSkewX);
    font.setHinting(fHinting);

    // N.B.: fAlign is applied externally (in alignedPosition()), because
    //  1) SkTextBlob has some trouble computing accurate bounds with alignment.
    //  2) SkPaint::Align is slated for deprecation.

    // First, convert to glyphIDs.
    font.setTextEncoding(SkPaint::kUTF8_TextEncoding);
    SkSTArray<256, SkGlyphID, true> glyphs;
    glyphs.reset(font.textToGlyphs(fText.c_str(), fText.size(), nullptr));
    SkAssertResult(font.textToGlyphs(fText.c_str(), fText.size(), glyphs.begin()) == glyphs.count());
    font.setTextEncoding(SkPaint::kGlyphID_TextEncoding);

    // Next, build the cached blob.
    SkTextBlobBuilder builder;
    const auto& buf = builder.allocRun(font, glyphs.count(), 0, 0, nullptr);
    if (!buf.glyphs) {
        fBlob.reset();
        return SkRect::MakeEmpty();
    }

    memcpy(buf.glyphs, glyphs.begin(), glyphs.count() * sizeof(SkGlyphID));

    fBlob = builder.make();
    if (!fBlob) {
        return SkRect::MakeEmpty();
    }

    const auto& bounds = fBlob->bounds();
    const auto aligned_pos = this->alignedPosition(bounds.width());

    return bounds.makeOffset(aligned_pos.x(), aligned_pos.y());
}

void Text::onDraw(SkCanvas* canvas, const SkPaint& paint) const {
    const auto aligned_pos = this->alignedPosition(this->bounds().width());
    canvas->drawTextBlob(fBlob, aligned_pos.x(), aligned_pos.y(), paint);
}

SkPath Text::onAsPath() const {
    // TODO
    return SkPath();
}

void Text::onClip(SkCanvas* canvas, bool antiAlias) const {
    canvas->clipPath(this->asPath(), antiAlias);
}

sk_sp<TextBlob> TextBlob::Make(sk_sp<SkTextBlob> blob) {
    return sk_sp<TextBlob>(new TextBlob(std::move(blob)));
}

TextBlob::TextBlob(sk_sp<SkTextBlob> blob)
    : fBlob(std::move(blob)) {}

TextBlob::~TextBlob() = default;

SkRect TextBlob::onRevalidate(InvalidationController*, const SkMatrix&) {
    return fBlob ? fBlob->bounds() : SkRect::MakeEmpty();
}

void TextBlob::onDraw(SkCanvas* canvas, const SkPaint& paint) const {
    canvas->drawTextBlob(fBlob, fPosition.x(), fPosition.y(), paint);
}

SkPath TextBlob::onAsPath() const {
    // TODO
    return SkPath();
}

void TextBlob::onClip(SkCanvas* canvas, bool antiAlias) const {
    canvas->clipPath(this->asPath(), antiAlias);
}

} // namespace sksg
