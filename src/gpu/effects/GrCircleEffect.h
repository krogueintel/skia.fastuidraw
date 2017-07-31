/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * This file was autogenerated from GrCircleEffect.fp; do not modify.
 */
#ifndef GrCircleEffect_DEFINED
#define GrCircleEffect_DEFINED
#include "SkTypes.h"
#if SK_SUPPORT_GPU
#include "GrFragmentProcessor.h"
#include "GrCoordTransform.h"
#include "GrColorSpaceXform.h"
class GrCircleEffect : public GrFragmentProcessor {
public:
    int edgeType() const { return fEdgeType; }
    SkPoint center() const { return fCenter; }
    float radius() const { return fRadius; }
    static sk_sp<GrFragmentProcessor> Make(int edgeType, SkPoint center, float radius) {
        return sk_sp<GrFragmentProcessor>(new GrCircleEffect(edgeType, center, radius));
    }
    GrCircleEffect(const GrCircleEffect& src);
    sk_sp<GrFragmentProcessor> clone() const override;
    const char* name() const override { return "CircleEffect"; }

private:
    GrCircleEffect(int edgeType, SkPoint center, float radius)
            : INHERITED((OptimizationFlags)kCompatibleWithCoverageAsAlpha_OptimizationFlag)
            , fEdgeType(edgeType)
            , fCenter(center)
            , fRadius(radius) {
        this->initClassID<GrCircleEffect>();
    }
    GrGLSLFragmentProcessor* onCreateGLSLInstance() const override;
    void onGetGLSLProcessorKey(const GrShaderCaps&, GrProcessorKeyBuilder*) const override;
    bool onIsEqual(const GrFragmentProcessor&) const override;
    GR_DECLARE_FRAGMENT_PROCESSOR_TEST
    int fEdgeType;
    SkPoint fCenter;
    float fRadius;
    typedef GrFragmentProcessor INHERITED;
};
#endif
#endif
