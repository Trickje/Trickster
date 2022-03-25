////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTUREPROVIDER_H__
#define __GUI_TEXTUREPROVIDER_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Delegate.h>


namespace Noesis
{

struct Uri;
class Texture;
class RenderDevice;
template<class T> class Ptr;

// Texture metadata
struct TextureInfo
{
    uint32_t width;
    uint32_t height;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for implementing providers of textures
////////////////////////////////////////////////////////////////////////////////////////////////////
class TextureProvider: public BaseComponent
{
public:
    /// Returns metadata for the texture at the given URI. 0 x 0 is returned if texture is not found
    virtual TextureInfo GetTextureInfo(const Uri& uri) = 0;

    /// Returns a texture compatible with the given device or null if texture is not found
    virtual Ptr<Texture> LoadTexture(const Uri& uri, RenderDevice* device) = 0;

    /// Delegate to notify changes to the texture file content
    typedef Delegate<void (const Uri&)> TextureChangedDelegate;
    TextureChangedDelegate& TextureChanged() { return mTextureChanged; }
    void RaiseTextureChanged(const Uri& uri) { mTextureChanged(uri); }

private:
    TextureChangedDelegate mTextureChanged;
};

}

#endif
