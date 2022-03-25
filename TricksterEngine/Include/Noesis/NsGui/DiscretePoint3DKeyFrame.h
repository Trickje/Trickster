////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DISCRETEPOINT3DKEYFRAME_H__
#define __GUI_DISCRETEPOINT3DKEYFRAME_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Point3DKeyFrame.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Animates from the *Point3D* value of the previous key frame to its own *Value* using discrete
/// interpolation.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.discretepoint3dkeyframe.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API DiscretePoint3DKeyFrame final: public Point3DKeyFrame
{
public:
    // Hides Freezable methods for convenience
    //@{
    Ptr<DiscretePoint3DKeyFrame> Clone() const;
    Ptr<DiscretePoint3DKeyFrame> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From KeyFrame
    //@{
    Point3D InterpolateValueCore(const Point3D& baseValue, float keyFrameProgress) override;
    //@}

    NS_DECLARE_REFLECTION(DiscretePoint3DKeyFrame, Point3DKeyFrame)
};

}


#endif
